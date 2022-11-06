#include "PackageGenerator.h"

#include "db/db.h"

#include <QDebug>

PackageGenerator::PackageGenerator(QObject* parent)
        : QObject(parent), queue(), nameToId() {
    manager = new QNetworkAccessManager(this);

    connect(
        manager,
        &QNetworkAccessManager::finished,
        this,
        &PackageGenerator::onNetworkReplied
    );
}

void PackageGenerator::exec(
        int packageId,
        const QString& articleName,
        int depthLimit,
        int quantityLimit
    ) {

    qDebug() << "Exec" << articleName;

    // Init
    queue.clear();
    nameToId.clear();
    this->packageId = packageId;
    this->depthLimit = depthLimit;
    this->quantityLimit = quantityLimit;

    // Insert dummy to db
    Theme t;
    t.id = -1;
    t.name = articleName;
    t.package.id = packageId;
    t.id = nameToId[articleName] = db::theme::write(t);

    // Init request
    manager->get(QNetworkRequest(getApiRequestUrl(articleName)));
}

void PackageGenerator::processNext() {
    qDebug() << "ProcessNext";
    if (queue.empty()) { // TODO: Check limits
        emit done();
        return;
    }

    qDebug() << "Ask"
        << db::theme::name(queue.front().parentId)
        << queue.front().currentName;
    emit edgeDirectionQuestionRequested(
        db::theme::name(queue.front().parentId), 
        queue.front().currentName
    );
}

void PackageGenerator::onDirrectionReplied(EdgeDirection dir) {
    qDebug() << "Reply got";

    if (dir == CANCEL_DIRECTION) {
        queue.pop_front();
        processNext();
    } else {
        if (!db::package::hasTheme(
                packageId, 
                queue.front().currentName
            )) {
            // Insert dummy theme into db
            Theme t;
            t.id = -1;
            t.name = queue.front().currentName;
            t.package.id = packageId;
            t.id = nameToId[queue.front().currentName] = db::theme::write(t);

            if (dir == RIGHT_DIRECTION) {
                db::themeEdge::createByThemes(
                    queue.front().parentId,
                    t.id
                );
            } else {
                db::themeEdge::createByThemes(
                    t.id,
                    queue.front().parentId
                );
            }

            manager->get(QNetworkRequest(getApiRequestUrl(queue.front().currentName)));

            queue.pop_front();
        }
    }
}

QUrl PackageGenerator::getApiRequestUrl(const QString& name) {
    return QUrl(baseRequestUrl.arg(name));
}

void PackageGenerator::parseReplyData(const QByteArray& data, QString& name, QString& prettyName, QString& description, QList<QString>& linkNames) {
    auto json = QJsonDocument::fromJson(data);
    const auto& query = json["query"];
    name = query["normalized"][0]["from"].toString();
    prettyName = query["normalized"][0]["to"].toString();
    description = query["pages"][0]["extract"].toString();
    const auto links = query["pages"][0]["links"].toArray();
    for (int i = 0; i < links.size(); ++i) { // foreach won't work
        const auto& link = links[i];
        const auto linkName = link["title"].toString();
        if (!linkName.contains(':')) {
            linkNames.append(linkName);
        }
    }
}

void PackageGenerator::onNetworkReplied(QNetworkReply* reply) {
    QString name;
    QString prettyName;
    QString description;
    QList<QString> linkNames;
    parseReplyData(
        reply->readAll(),
        name,
        prettyName,
        description,
        linkNames
    );
    reply->deleteLater();

    qDebug() << "Networkd Replied" << name;

    Theme t = db::theme::read(nameToId[name]);
    t.name = name;
    t.description = description;
    db::theme::write(t);

    for (const auto& linkName : linkNames) {
        queue.push_back({
            t.id,
            linkName
        });
    }

    processNext();
}
