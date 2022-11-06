#pragma once

#include "DependencyDirection.h"

#include <QObject>
#include <QString>
#include <QHash>
#include <QQueue>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>

class PackageGenerator : public QObject {
    Q_OBJECT

    public:
        // Creates generator that reads data from Wikipedia
        PackageGenerator(
            QObject* parent = nullptr
        );
        
        // Starts generator from article "articleName"
        // Writes result to package with id packageId
        // Stops generation if search depth is greather than depthLimit
        //     or if generated more than quantityLimit themes
        void exec(
            int packageId,
            const QString& language,
            const QString& articleName,
            int depthLimit,
            int quantityLimit
        );

    private:
        struct QueueItem {
            int parentId;
            QString currentName;
        };

        // Processes first item in queue
        void processNext();

        // Parses data data from QNetworkResponse
        //     and writes into name, prettyName, description and linkNames
        void parseReplyData(const QByteArray& data,
            QString& name,
            QString& prettyName,
            QString& description,
            QList<QString>& linkNames
        );

        // Returns url for api request by Wikipedia article's name
        QUrl getApiRequestUrl(const QString& name);

        QString language;
        int packageId;
        int depthLimit;
        int quantityLimit;

        const QString baseRequestUrl = "https://{language}.wikipedia.org/w/api.php?action=query&format=json&prop=links%7Cextracts%7Cinfo&titles={articleName}&formatversion=2&pllimit=500&exsentences=1&explaintext=1&exsectionformat=plain";

        QNetworkAccessManager* manager;

        QQueue<QueueItem> queue;

        QHash<QString, int> nameToId;

    signals:
        void done();
        void dependencyDirectionQuestionRequested(
            const QString& first,
            const QString& second
        );

    public slots:
        // As a response for dependencyDirectionQuestionRequested
        void onDirrectionReplied(DependencyDirection dir);

    private slots:
        // Processes http response
        void onNetworkReplied(QNetworkReply* reply);
};
