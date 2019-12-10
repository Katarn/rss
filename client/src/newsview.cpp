#include <QScrollBar>
#include <QDebug>

#include "newsview.h"

NewsView::NewsView(QWidget * parent) : QTreeView(parent)
{
    setObjectName("newsView");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setMinimumWidth(120);
    setSortingEnabled(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setRootIsDecorated(false); // TODO
    setUniformRowHeights(true);

    DelegateWithoutFocus *itemDelegate = new DelegateWithoutFocus(this);
    setItemDelegate(itemDelegate);

    m_server = Server::instance();

    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollChanged()));

    m_timer = new QTimer(this);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(fetchNews()));
}

// =====================================================================================================================

NewsView::ItemRange NewsView::visibleItems() const
{
    int startIndex = -1;
    int endIndex   = -1;

    if (m_sourceModel->news().length()) {
        const QModelIndex modelIndexStart = indexAt(rect().topLeft());
        const QModelIndex modelIndexEnd   = indexAt(rect().bottomLeft());

        startIndex = modelIndexStart.row();
        endIndex   = modelIndexEnd.row();

        if (endIndex == -1) {
            endIndex = m_sourceModel->news().length() - 1;
        }
    }

    NewsView::ItemRange itemRange;

    itemRange.start = startIndex;
    itemRange.end   = endIndex;

    return itemRange;
}

// =====================================================================================================================

NewsView::ItemRange NewsView::updateRangeItems(const ItemRange &range) const
{
    NewsView::ItemRange itemRange;

    itemRange.start = -1;
    itemRange.end   = -1;

    int startIndex = range.start;
    int endIndex   = range.end;

    if (startIndex != -1 && endIndex != -1) {
        startIndex -= 50;
        endIndex   += 50;

        startIndex = qMax(0, startIndex);
        endIndex   = qMin(endIndex, m_sourceModel->news().length() - 1);

        for (int i = startIndex; i <= endIndex; i++) {
            if (!m_sourceModel->news().at(i).isLoaded) {
                itemRange.start = i;
                break;
            }
        }

        for (int i = endIndex; i >= startIndex; i--) {
            if (!m_sourceModel->news().at(i).isLoaded) {
                itemRange.end   = i;
                break;
            }
        }
    }

    return itemRange;
}

// =====================================================================================================================

void NewsView::scrollChanged()
{
    fetchNewsDebounce();
}

// =====================================================================================================================

void NewsView::resizeEvent(QResizeEvent* event)
{
    QTreeView::resizeEvent(event);

    fetchNewsDebounce();
}

// =====================================================================================================================

void NewsView::fetchNewsDebounce()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }

    m_timer->start(150);
}

// =====================================================================================================================

void NewsView::fetchNews()
{
    m_timer->stop();

    NewsView::ItemRange visibleItems = this->visibleItems();
    NewsView::ItemRange updateRangeItems = this->updateRangeItems(visibleItems);

    const int startIndex = updateRangeItems.start;
    const int endIndex   = updateRangeItems.end;

    if (startIndex != -1 && endIndex != -1) {
        m_server->getNewsList(m_feedItem->id(), startIndex, endIndex - startIndex + 1);
    }
}

// =====================================================================================================================

void NewsView::setSourceModel(NewsModel *model)
{
    m_sourceModel = model;
}

// =====================================================================================================================

void NewsView::setFeedItem(FeedItem *feedItem)
{
    m_feedItem = feedItem;

    m_sourceModel->setFeed(feedItem);

    fetchNews();
}

// =====================================================================================================================

void NewsView::mousePressEvent(QMouseEvent *event)
{
  if (!indexAt(event->pos()).isValid()) {
      return;
  }

  m_indexClicked = indexAt(event->pos());

  QTreeView::mousePressEvent(event);
}

// =====================================================================================================================

void NewsView::mouseDoubleClickEvent(QMouseEvent *event)
{
  if (!indexAt(event->pos()).isValid()){
      return;
  }

  if (m_indexClicked == indexAt(event->pos())) {
      emit signalDoubleClicked(indexAt(event->pos()));
  }
  else {
      mousePressEvent(event);
  }
}

