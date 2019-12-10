#include <QDebug>

#include "server.h"

Server *Server::s_instance = nullptr;

// =====================================================================================================================

Server::Server(QObject *parent) : QObject(parent)
{
    m_ws = new QWebSocket();

    QObject::connect(m_ws, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(m_ws, SIGNAL(disconnected()), this, SLOT(onClosed()));
    QObject::connect(m_ws, SIGNAL(textMessageReceived(const QString&)), this, SLOT(onTextMessageReceived(const QString&)));
}

// =====================================================================================================================

Server::~Server()
{
    m_ws->disconnect();

    QObject::disconnect(m_ws, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::disconnect(m_ws, SIGNAL(disconnected()), this, SLOT(onClosed()));
    QObject::disconnect(m_ws, SIGNAL(textMessageReceived(const QString&)), this, SLOT(onTextMessageReceived(const QString&)));

    delete m_ws;
}

// =====================================================================================================================

Server *Server::instance()
{
    if (!s_instance) {
        s_instance = new Server();
    }

    return s_instance;
}

// =====================================================================================================================

void Server::connect()
{
    m_ws->open(QUrl("ws://206.189.116.3:8080"));
}

// =====================================================================================================================

void Server::onConnected()
{
    qDebug() << "onServerConnected";

    emit onServerConnected();

    initCommand();
}

// =====================================================================================================================

void Server::onClosed()
{
    qDebug() << "onServerDisconnected";

    emit onServerDisconnected();
}

// =====================================================================================================================

void Server::onTextMessageReceived(const QString &message)
{
    //qDebug() << message;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc.object();

    QString method = obj.value("method").toString();
    QJsonObject data = obj.value("data").toObject();

    if (method == "init") {
        emit signalServerInit(data);
    }
    else if (method == "getNewsList") {
        emit signalServerGetNewsList(data);
    }
    else if (method == "recalcFeedsCounters") {
        emit signalServerRecalcFeedsCounters(data);
    }
}

// =====================================================================================================================

void Server::send(const QString method)
{
    QJsonObject obj;

    send(method, obj);
}

// =====================================================================================================================

void Server::send(const QString method, const QJsonObject params)
{
    QJsonDocument *doc = new QJsonDocument();
    QJsonObject obj;

    obj.insert("method", method);
    obj.insert("params", params);

    doc->setObject(obj);

    m_ws->sendTextMessage(doc->toJson());
}

// =====================================================================================================================

void Server::initCommand()
{
    send("init");
}

// =====================================================================================================================

void Server::setCategoryCollapsed(int feedId)
{
    QJsonObject obj;

    obj.insert("category_id", feedId);

    send("setCategoryCollapsed", obj);
}

// =====================================================================================================================

void Server::setCategoryExpanded(int feedId)
{
    QJsonObject obj;

    obj.insert("category_id", feedId);

    send("setCategoryExpanded", obj);
}

// =====================================================================================================================

void Server::getNewsList(int feedId, int offset, int limit)
{
    QJsonObject obj;

    obj.insert("feed_id", feedId);
    obj.insert("offset", offset);
    obj.insert("limit", limit);

    send("getNewsList", obj);
}

// =====================================================================================================================

void Server::setNewsState(const QVector<int> &newsItems, int feedId, NewsState state)
{
    QJsonObject obj;
    QJsonArray items;

    obj.insert("feed_id", feedId);

    for (int i = 0; i < newsItems.length(); i++) {
        items.append(newsItems.at(i));
    }

    obj.insert("items", items);

    switch (state) {
        case NewsState::Read:
            obj.insert("is_read", true);
            break;

        case NewsState::Unread:
            obj.insert("is_read", false);
            break;

        case NewsState::Starred:
            obj.insert("is_starred", true);
            break;

        case NewsState::Unstarred:
            obj.insert("is_starred", false);
            break;
    }

    send("setNewsState", obj);
}

// =====================================================================================================================

void Server::setNewsRead(const int feedId, QVector<int> newsIds, bool isRead)
{
    QJsonObject obj;
    QJsonArray items;

    for (int i = 0; i < newsIds.length(); i++) {
        items.append(newsIds.at(i));
    }

    obj.insert("feed_id", feedId);
    obj.insert("items", items);
    obj.insert("is_read", isRead);

    send("setNewsRead", obj);
}

// =====================================================================================================================

void Server::setNewsStarred(const int feedId, QVector<int> newsIds, bool isStarred)
{
    QJsonObject obj;
    QJsonArray items;

    for (int i = 0; i < newsIds.length(); i++) {
        items.append(newsIds.at(i));
    }

    obj.insert("feed_id", feedId);
    obj.insert("items", items);
    obj.insert("is_read", isStarred);

    send("setNewsStarred", obj);
}

// =====================================================================================================================

void Server::setFeedViewed(int feedId)
{
    QJsonObject obj;

    obj.insert("feed_id", feedId);

    send("setFeedViewed", obj);
}
