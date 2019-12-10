#ifndef NEWSVIEW_H
#define NEWSVIEW_H

#include <QTreeView>
#include <QTimer>

#include "delegatewithoutfocus.h"
#include "newsmodel.h"

class NewsView : public QTreeView
{
    Q_OBJECT
public:
    NewsView(QWidget * parent = nullptr);

    void setSourceModel(NewsModel *model);
    void setFeedItem(FeedItem *feedItem);

    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void signalDoubleClicked(QModelIndex index);
    void signalSetItemRead(QModelIndex index, int read);
    void signalSetItemStar(QModelIndex index, int starred);

private:
    struct ItemRange {
        int start;
        int end;
    };

    void fetchNewsDebounce();
    ItemRange visibleItems() const;
    ItemRange updateRangeItems(const ItemRange &range) const;

    NewsModel *m_sourceModel;
    FeedItem *m_feedItem;
    Server *m_server;

    QModelIndex m_indexClicked;
    QTimer *m_timer;

private slots:
    void fetchNews();

public slots:
    void scrollChanged();
};

#endif // NEWSVIEW_H
