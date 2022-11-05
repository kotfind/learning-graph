#pragma once

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
        enum EdgeDirection {
            LEFT_DIRECTION,
            RIGHT_DIRECTION,
            CANCEL_DIRECTION
        };

        // Creates generator which reads data from Wikipedia
        //     and writes it as package with id packageId
        // Stops generation if search depth is greather than depthLimit
        //     or if generated more than quantityLimit themes
        PackageGenerator(
            int packageId,
            int depthLimit,
            int quantityLimit,
            QObject* parent = nullptr
          );
        
        // Starts generator from article "articleName"
        void exec(const QString& articleName);

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

        int packageId;
        int depthLimit;
        int quantityLimit;

        const QString baseRequestUrl = "https://en.wikipedia.org/w/api.php?action=query&format=json&prop=links%7Cextracts&titles=%1&formatversion=2&pllimit=500&exsentences=1&exintro=1&explaintext=1";

        QNetworkAccessManager* manager;

        QQueue<QueueItem> queue;

        QHash<QString, int> nameToId;

    signals:
        void done();
        void edgeDirectionQuestionRequested(
            const QString& first,
            const QString& second
        );

    public slots:
        // As a response for edgeDirectionQuestionRequested
        void onDirrectionReplyGot(EdgeDirection dir);

    private slots:
        // Processes http response
        void onNetworkReplyGot(QNetworkReply* reply);
};
