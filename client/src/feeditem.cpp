#include "feeditem.h"

FeedItem::FeedItem(FeedItem *parent)
{
    m_parentItem = parent;
}

// =====================================================================================================================

FeedItem::~FeedItem()
{
    qDeleteAll(m_items);
}

// =====================================================================================================================

void FeedItem::parseJson(QJsonObject *json) {
    m_id          = json->value("id").toInt(0);
    m_parentId    = json->value("parent_id").toInt(0);
    m_title       = json->value("title").toString();
    m_isFolder    = json->value("type").toString() == "folder";
    m_newCount    = json->value("new_count").toInt(0);
    m_newsCount   = json->value("news_count").toInt(0);
    m_unreadCount = json->value("unread_count").toInt(0);
    m_isExpanded  = json->value("is_expanded").toBool(false);
    m_isUpdating  = json->value("is_updating").toBool(false);
    m_isError     = json->value("is_error").toBool(false);

    // Parse icon
    if (m_isFolder) {
        m_icon = QPixmap(":/images/folder");
    }
    else {
        const QString icon = json->value("icon").toString();

        if (icon.isEmpty()) {
            m_icon = QPixmap(":/images/feed-icon");
        }
        else {
            const QByteArray base64Data = json->value("icon").toString().toUtf8();
            QImage image;

            image.loadFromData(QByteArray::fromBase64(base64Data));
            image = image.scaled(16, 16); // TODO: Scale icons on the server

            m_icon = QPixmap::fromImage(image);
        }
    }

    if (m_isFolder) {
        m_id = m_id;
    }

    // Parse child items
    if (m_isFolder) {
        const QJsonArray items = json->value("items").toArray();

        for (int i = 0; i < items.count(); i++) {
            FeedItem *feedItem = new FeedItem(this);
            QJsonObject obj    = items.at(i).toObject();

            feedItem->parseJson(&obj);

            appendItem(feedItem);
        }
    }
}

// =====================================================================================================================

void FeedItem::appendItem(FeedItem *item)
{
    m_items.append(item);
}

// =====================================================================================================================

FeedItem *FeedItem::item(int row)
{
    return m_items.value(row);
}

// =====================================================================================================================

int FeedItem::itemsCount() const
{
    return m_items.count();
}

// =====================================================================================================================

int FeedItem::row() const
{
    if (m_parentItem) {
        return m_parentItem->m_items.indexOf(const_cast<FeedItem*>(this));
    }

    return 0;
}

// =====================================================================================================================

FeedItem *FeedItem::parentItem()
{
    return m_parentItem;
}

// =====================================================================================================================

int FeedItem::id() {
    return m_id;
}

// =====================================================================================================================

QString FeedItem::title() const {
    return m_title;
}

// =====================================================================================================================

int FeedItem::parentId() const {
return m_parentId;
}

// =====================================================================================================================

bool FeedItem::isFolder() const {
    return m_isFolder;
}

// =====================================================================================================================

QPixmap FeedItem::icon() const {
    return m_icon;
}

// =====================================================================================================================

int FeedItem::newCount() const {
    return m_newCount;
}

// =====================================================================================================================

int FeedItem::newsCount() const {
    return m_newsCount;
}

// =====================================================================================================================

int FeedItem::unreadCount() const {
    return m_unreadCount;
}

// =====================================================================================================================

bool FeedItem::isExpanded() const {
    return m_isExpanded;
}

// =====================================================================================================================

bool FeedItem::isUpdating() const {
    return m_isUpdating;
}

// =====================================================================================================================

bool FeedItem::isError() const {
    return m_isError;
}

// =====================================================================================================================

void FeedItem::setTitle(QString title) {
    m_title = title;
}

// =====================================================================================================================

void FeedItem::setParentId(int parentId) {
    m_parentId = parentId;
}

// =====================================================================================================================

void FeedItem::setIcon(QPixmap icon) {
    m_icon = icon;
}

// =====================================================================================================================

void FeedItem::setNewCount(int newCount) {
    m_newCount = newCount;
}

// =====================================================================================================================

void FeedItem::setNewsCount(int newsCount) {
    m_newsCount = newsCount;
}

// =====================================================================================================================

void FeedItem::setUnreadCount(int unreadCount) {
    m_unreadCount = unreadCount;
}

// =====================================================================================================================

void FeedItem::setIsExpanded(bool isExpanded) {
    m_isExpanded = isExpanded;
}

// =====================================================================================================================

void FeedItem::setIsUpdating(bool isUpdating) {
    m_isUpdating = isUpdating;
}

// =====================================================================================================================

void FeedItem::setIsError(bool isError) {
    m_isError = isError;
}
