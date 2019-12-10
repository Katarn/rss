#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QWebSocket>

#include "structs.h"

class Server : public QObject
{
Q_OBJECT

public:
    Server(QObject *parent = nullptr);
    ~Server();

    static Server *instance();
    void connect();

    void send(const QString method);
    void send(const QString method, const QJsonObject params);
    void initCommand();
    void setCategoryCollapsed(int feedId);
    void setCategoryExpanded(int feedId);
    void getNewsList(int feedId, int offset, int limit);
    void setNewsRead(const int feedId, QVector<int> newsIds, bool isRead);
    void setNewsStarred(const int feedId, QVector<int> newsIds, bool isStarred);
    void setNewsState(const QVector<int> &newsItems, int feedId, NewsState state);
    void setFeedViewed(int feedId);

private:
    static Server *s_instance;

    QWebSocket *m_ws;

private slots:
    void onConnected();
    void onClosed();
    void onTextMessageReceived(const QString &message);

signals:
    void finished();
    void markAsRead(const QString type, const int id);

    void onServerConnected();
    void onServerDisconnected();

    void signalServerInit(const QJsonObject &json);
    void signalServerGetNewsList(const QJsonObject &json);
    void signalServerRecalcFeedsCounters(const QJsonObject &json);
};

#endif // SERVER_H
