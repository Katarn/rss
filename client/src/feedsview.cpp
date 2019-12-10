#include "feedsview.h"

FeedsView::FeedsView(QWidget *parent) : QTreeView(parent)
{
    setObjectName("feedsView");
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setUniformRowHeights(true);
    setRootIsDecorated(false); // TODO

    DelegateWithoutFocus *itemDelegate = new DelegateWithoutFocus(this);
    setItemDelegate(itemDelegate);

    header()->setStretchLastSection(false);
    header()->setVisible(false);

    m_isInited = false;

    m_server = Server::instance();

    connect(this, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(onCollapsed(const QModelIndex&)));
    connect(this, SIGNAL(expanded(const QModelIndex&)), this, SLOT(onExpanded(const QModelIndex&)));
}

// =====================================================================================================================

void FeedsView::setSourceModel(FeedsModel *model)
{
    m_sourceModel = model;

    connect(m_sourceModel, SIGNAL(modelReset()), this, SLOT(refresh()));
}

// =====================================================================================================================

void FeedsView::refresh()
{
    expandFeeds();

    m_isInited = true;
}

// =====================================================================================================================

void FeedsView::expandFeeds()
{
    const QModelIndexList folders = m_sourceModel->folders();

    qDebug() << folders.size();

    for (int i = 0; i < folders.size(); i++) {
        FeedItem *feedItem = m_sourceModel->feedItem(folders.at(i));

        if (feedItem->isExpanded()) {
            expand(folders.at(i));
        }
    }
}

// =====================================================================================================================

void FeedsView::onCollapsed(const QModelIndex &index)
{
    if (m_isInited) {
        FeedItem *feedItem = m_sourceModel->feedItem(index);

        if (feedItem) {
            m_server->setCategoryCollapsed(feedItem->id());
        }
    }
}

// =====================================================================================================================

void FeedsView::onExpanded(const QModelIndex &index)
{
    if (m_isInited) {
        FeedItem *feedItem = m_sourceModel->feedItem(index);

        if (feedItem) {
            m_server->setCategoryExpanded(feedItem->id());
        }
    }
}
