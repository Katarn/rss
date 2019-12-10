#ifndef NEWSTABWIDGET_H
#define NEWSTABWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QMovie>
#include <QSplitter>
#include <QToolButton>

#include "newsview.h"
#include "newsmodel.h"
#include "newsheader.h"
#include "feedsmodel.h"

class NewsTabWidget : public QWidget
{
    Q_OBJECT
public:
    enum TabType {
        Feed,
        Unread,
        Star,
        Del,
        Label,
        Web,
        Downloads
    };

    explicit NewsTabWidget(QWidget *parent, FeedsModel *feedsModel);
    QWidget &titleLabel();

    void disconnectObjects();
    void setType(TabType type);
    NewsTabWidget::TabType type();
    void setFeedItem(FeedItem *feedItem);
    void retranslateStrings();
    NewsHeader *header() const;

private:
    Server *m_server;
    NewsModel *m_newsModel;
    NewsHeader *m_newsHeader;
    FeedItem *m_feedItem;
    QWidget *m_newsTitleLabel;
    QLabel *m_newsIconTitle;
    QMovie *m_newsIconMovie;
    QLabel *m_newsTextTitle;
    NewsView *m_newsView;
    QWidget *m_newsWidget;
    QSplitter *m_newsTabWidgetSplitter;
    TabType m_type;
    QToolButton *m_closeButton;
    FeedsModel *m_feedsModel;

    void createNewsList();

private slots:
    void slotNewsViewClicked(QModelIndex index);
    void slotNewsViewSelected(QModelIndex index, bool clicked = false);
    void slotSetItemRead(QModelIndex index, int read);
    void slotSetItemStar(QModelIndex index, int starred);

signals:

public slots:
};

#endif // NEWSTABWIDGET_H
