#ifndef FEEDITEM_H
#define FEEDITEM_H

#include <QJsonArray>
#include <QJsonObject>
#include <QPixmap>
#include <QVariant>

class FeedItem
{
public:
    explicit FeedItem(FeedItem *parentItem = nullptr);
    ~FeedItem();

    void appendItem(FeedItem *child);
    void parseJson(QJsonObject *json);

    FeedItem *item(int row);
    int itemsCount() const;
    int row() const;
    FeedItem *parentItem();

    int id();
    QString title() const;
    int parentId() const;
    bool isFolder() const;
    QPixmap icon() const;
    int newCount() const;
    int newsCount() const;
    int unreadCount() const;
    bool isExpanded() const;
    bool isUpdating() const;
    bool isError() const;

    void setTitle(QString title);
    void setParentId(int parentId);
    void setIcon(QPixmap icon);
    void setNewCount(int newCount);
    void setNewsCount(int newsCount);
    void setUnreadCount(int unreadCount);
    void setIsExpanded(bool isExpanded);
    void setIsUpdating(bool isUpdating);
    void setIsError(bool isError);

private:
    QList<FeedItem*> m_items;
    FeedItem *m_parentItem;

    int m_id;
    QString m_title;
    int m_parentId;
    bool m_isFolder;
    QPixmap m_icon;
    int m_newCount;
    int m_newsCount;
    int m_unreadCount;
    bool m_isExpanded;
    bool m_isUpdating;
    bool m_isError;
};

#endif // FEEDITEM_H
