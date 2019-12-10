#include "feedsmodel.h"

FeedsModel::FeedsModel(QObject *parent) : QAbstractItemModel(parent)
{
    m_server = Server::instance();

    m_rootItem = new FeedItem();

    connect(m_server, SIGNAL(signalServerInit(const QJsonObject&)), this, SLOT(slotInitCommand(const QJsonObject&)));
    connect(m_server, SIGNAL(signalServerRecalcFeedsCounters(const QJsonObject&)), this, SLOT(slotRecalcFeedsCountersCommand(const QJsonObject&)));
}

// =====================================================================================================================

FeedsModel::~FeedsModel()
{
    clear();
}

// =====================================================================================================================

void FeedsModel::clear()
{
    delete m_rootItem;

    m_rootItem = new FeedItem();
}

// =====================================================================================================================

Qt::ItemFlags FeedsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemFlag::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

// =====================================================================================================================

int FeedsModel::rowCount(const QModelIndex &parent) const
{
    FeedItem *parentItem;

    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    }
    else {
        parentItem = static_cast<FeedItem*>(parent.internalPointer());
    }

    return parentItem->itemsCount();
}

// =====================================================================================================================

int FeedsModel::columnCount(const QModelIndex&) const
{
    return 2;
}

// =====================================================================================================================

QModelIndex FeedsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    FeedItem *parentItem;

    if (!parent.isValid()) {
        parentItem = m_rootItem;
    }
    else {
        parentItem = static_cast<FeedItem*>(parent.internalPointer());
    }

    FeedItem *childItem = parentItem->item(row);

    if (childItem) {
        return createIndex(row, column, childItem);
    }
    else {
        return QModelIndex();
    }
}

// =====================================================================================================================

QModelIndex FeedsModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    FeedItem *childItem = static_cast<FeedItem*>(index.internalPointer());
    FeedItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

// =====================================================================================================================

QVariant FeedsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    FeedItem *item = static_cast<FeedItem*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0:
                return item->title();
            case 1:
                if (item->unreadCount()) {
                    const QString unread = QString("(%1)").arg(item->unreadCount());

                    return unread;
                }
        }
    }
    else if (role == Qt::DecorationRole) {
        if (index.column() == 0) {
            return item->icon();
        }
    }
    else if (role == Qt::TextColorRole) {
        if (index.column() == 0) {
            if (item->newCount()) {
                return QColor(0, 0, 255);
            }
        }
        else if (index.column() == 1) {
            return QColor(0, 0, 255);
        }
    }
    else if (role == Qt::FontRole) {
        QFont font;

        if (index.column() == 0 && item->unreadCount()) {
            font.setBold(true);
        }

        return font;
    }
    else if (role == Qt::EditRole) {
        if (index.column() == FeedColumn::Id) {
            return item->id();
        }
        else if (index.column() == FeedColumn::IsFolder) {
            return item->isFolder();
        }
    }

    return QVariant();
}

// =====================================================================================================================

FeedItem *FeedsModel::feedItem(int feedId)
{
    QModelIndexList indexList = match(index(0, FeedColumn::Id),
                                      Qt::EditRole, feedId, 1,
                                      Qt::MatchFlags(Qt::MatchExactly|Qt::MatchRecursive));

    if (indexList.count()) {
        return feedItem(indexList[0]);
    }
}

// =====================================================================================================================

FeedItem *FeedsModel::feedItem(QModelIndex index)
{
    FeedItem *feedItem = static_cast<FeedItem*>(index.internalPointer());

    return feedItem;
}

// =====================================================================================================================

QModelIndex FeedsModel::indexFromFeedId(int feedId)
{
    QModelIndexList indexList = match(index(0, FeedColumn::Id),
                                      Qt::EditRole, feedId, 1,
                                      Qt::MatchFlags(Qt::MatchExactly|Qt::MatchRecursive));

    if (indexList.count()) {
        return indexList[0];
    }

    return QModelIndex();
}

// =====================================================================================================================

QModelIndexList FeedsModel::folders()
{
    QModelIndexList indexList = match(index(0, FeedColumn::IsFolder),
                                      Qt::EditRole, true, -1,
                                      Qt::MatchFlags(Qt::MatchExactly|Qt::MatchRecursive));
int zz = indexList.count();


    if (indexList.count()) {
        return indexList;
    }

    return QModelIndexList();
}

// =====================================================================================================================

void FeedsModel::updateFeedItemView(int feedId)
{
    const QModelIndexList indexList = match(index(0, FeedColumn::Id), Qt::EditRole, feedId, 1, Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap|Qt::MatchRecursive));

    if (indexList.count()) {
        const QModelIndex startIndex = indexList[0];
        const QModelIndex endIndex   = index(startIndex.row(), columnCount() - 1, startIndex.parent());

        emit dataChanged(startIndex, endIndex);
    }
}

// =====================================================================================================================

void FeedsModel::slotInitCommand(const QJsonObject &json)
{
    beginResetModel();

    clear();

    QJsonArray items = json.value("items").toArray();

    for (int i = 0; i < items.count(); i++) {
        FeedItem *feedItem = new FeedItem(m_rootItem);
        QJsonObject obj    = items.at(i).toObject();

        feedItem->parseJson(&obj);

        m_rootItem->appendItem(feedItem);
    }

    endResetModel();
}

// =====================================================================================================================

void FeedsModel::slotRecalcFeedsCountersCommand(const QJsonObject &json)
{
    QJsonArray jsonItems = json.value("items").toArray();

    for (int i = 0; i < jsonItems.count(); i++) {
        const QJsonObject jsonItem = jsonItems.at(i).toObject();
        const int feedId = jsonItem.value("id").toInt(0);

        FeedItem *feedItem = this->feedItem(feedId);

        if (feedItem) {
            feedItem->setNewCount(jsonItem.value("new_count").toInt());
            feedItem->setNewsCount(jsonItem.value("news_count").toInt());
            feedItem->setUnreadCount(jsonItem.value("unread_count").toInt());
        }

        updateFeedItemView(feedId);
    }
}
