#include <QHeaderView>

#include "newstabwidget.h"

#define MAX_TAB_WIDTH 150

NewsTabWidget::NewsTabWidget(QWidget *parent, FeedsModel *feedsModel)
    : QWidget(parent)
    , m_feedsModel(feedsModel)
{
    m_newsIconTitle = new QLabel();
    m_newsIconMovie = new QMovie(":/images/loading");
    m_newsIconTitle->setMovie(m_newsIconMovie);
    m_newsTextTitle = new QLabel();
    m_newsTextTitle->setObjectName("newsTextTitle");

    m_closeButton = new QToolButton();
    m_closeButton->setFixedSize(15, 15);
    m_closeButton->setCursor(Qt::ArrowCursor);
    m_closeButton->setObjectName("tabClose");

    //connect(m_closeButton, SIGNAL(clicked()), this, SLOT(slotTabClose()));

    QHBoxLayout *newsTitleLayout = new QHBoxLayout();
    newsTitleLayout->setMargin(0);
    newsTitleLayout->setSpacing(0);
    newsTitleLayout->addWidget(m_newsIconTitle);
    newsTitleLayout->addSpacing(3);
    newsTitleLayout->addWidget(m_newsTextTitle, 1);
    newsTitleLayout->addWidget(m_closeButton);

    m_newsTitleLabel = new QWidget();
    m_newsTitleLabel->setObjectName("newsTitleLabel");
    m_newsTitleLabel->setMinimumHeight(16);
    m_newsTitleLabel->setLayout(newsTitleLayout);
    m_newsTitleLabel->setVisible(false);

    //if (!showCloseButtonTab) {
//        m_closeButton->hide();
        m_newsTitleLabel->setFixedWidth(MAX_TAB_WIDTH - 15);
    //}
    //else {
    //    m_newsTitleLabel->setFixedWidth(MAX_TAB_WIDTH);
    //}

    m_newsTextTitle->setText("!!!!!!!!");

    createNewsList();

    m_newsTabWidgetSplitter = new QSplitter(this);
    m_newsTabWidgetSplitter->setObjectName("newsTabWidgetSplitter");

    //m_newsTabWidgetSplitter->addWidget(webWidget_);
    m_newsTabWidgetSplitter->addWidget(m_newsWidget);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(m_newsTabWidgetSplitter);

    setLayout(layout);

    m_newsTabWidgetSplitter->setHandleWidth(1);
    m_newsTabWidgetSplitter->setOrientation(Qt::Horizontal);
    /*
    m_newsTabWidgetSplitter->setStyleSheet(
          QString("QSplitter::handle {background: qlineargradient("
                  "x1: 0, y1: 0, x2: 0, y2: 1,"
                  "stop: 0 %1, stop: 0.07 %2);}")
          .arg(m_newsPanelWidget->palette().background().color().name())
          .arg(qApp->palette().color(QPalette::Dark).name()));
    */

    m_server = Server::instance();
}

// =====================================================================================================================

void NewsTabWidget::disconnectObjects()
{
    disconnect(this);

    //if (type_ != TabTypeDownloads) {
    //    webView_->disconnect(this);
    //    webView_->disconnectObjects();
    //    qobject_cast<WebPage*>(webView_->page())->disconnectObjects();
    //}
}

// =====================================================================================================================

QWidget& NewsTabWidget::titleLabel()
{
    return *m_newsTitleLabel;
}

// =====================================================================================================================

void NewsTabWidget::createNewsList()
{
    m_newsView = new NewsView(this);
    m_newsView->setFrameStyle(QFrame::NoFrame);
    m_newsModel = new NewsModel(this, m_newsView, m_feedsModel);
    m_newsHeader = new NewsHeader(m_newsModel, m_newsView);

    m_newsView->setModel(m_newsModel);
    m_newsView->setHeader(m_newsHeader);
    m_newsView->setSourceModel(m_newsModel);

    m_newsHeader->init();
    m_newsHeader->retranslateStrings();

    QVBoxLayout *newsLayout = new QVBoxLayout();
    newsLayout->setMargin(0);
    newsLayout->setSpacing(0);
    //newsLayout->addWidget(newsPanelWidget_);
    newsLayout->addWidget(m_newsView);

    m_newsWidget = new QWidget(this);
    m_newsWidget->setLayout(newsLayout);

    connect(m_newsView, SIGNAL(pressed(QModelIndex)), this, SLOT(slotNewsViewClicked(QModelIndex)));
//    connect(newsView_, SIGNAL(pressKeyUp(QModelIndex)),
//            this, SLOT(slotNewsUpPressed(QModelIndex)));
//    connect(newsView_, SIGNAL(pressKeyDown(QModelIndex)),
//            this, SLOT(slotNewsDownPressed(QModelIndex)));
//    connect(newsView_, SIGNAL(pressKeyHome(QModelIndex)),
//            this, SLOT(slotNewsHomePressed(QModelIndex)));
//    connect(newsView_, SIGNAL(pressKeyEnd(QModelIndex)),
//            this, SLOT(slotNewsEndPressed(QModelIndex)));
//    connect(newsView_, SIGNAL(pressKeyPageUp(QModelIndex)),
//            this, SLOT(slotNewsPageUpPressed(QModelIndex)));
//    connect(newsView_, SIGNAL(pressKeyPageDown(QModelIndex)),
//            this, SLOT(slotNewsPageDownPressed(QModelIndex)));
    connect(m_newsView, SIGNAL(signalSetItemRead(QModelIndex, int)), this, SLOT(slotSetItemRead(QModelIndex, int)));
//    connect(m_newsView, SIGNAL(signalSetItemStar(QModelIndex, int)), this, SLOT(slotSetItemStar(QModelIndex, int)));
}

// =====================================================================================================================

void NewsTabWidget::setType(TabType type)
{
    m_type = type;
}

// =====================================================================================================================

NewsTabWidget::TabType NewsTabWidget::type()
{
    return m_type;
}

// =====================================================================================================================

void NewsTabWidget::setFeedItem(FeedItem *feedItem)
{
    m_newsIconTitle->setPixmap(feedItem->icon());

    int padding = 15;

    if (m_closeButton->isHidden()) {
        padding = 0;
    }

    QString textTab = m_newsTextTitle->fontMetrics().elidedText(
        feedItem->title(), Qt::ElideRight, m_newsTitleLabel->width() - 16 - 3 - padding);
    m_newsTextTitle->setText(textTab);
    m_newsTitleLabel->setToolTip(feedItem->title());

    m_feedItem = feedItem;
    m_newsView->setFeedItem(feedItem);

    //emit signalSetTextTab(text, this);
}

// =====================================================================================================================

void NewsTabWidget::retranslateStrings() {
    if (type() != TabType::Downloads) {
        //webViewProgress_->setFormat(tr("Loading... (%p%)"));

        //webHomePageAct_->setText(tr("Home"));
        //webExternalBrowserAct_->setText(tr("Open Page in External Browser"));
        //urlExternalBrowserAct_->setText(tr("Open Link in External Browser"));

        if (type() != TabType::Web) {
        //    findText_->retranslateStrings();
            m_newsHeader->retranslateStrings();
        }

        //if (mainWindow_->currentNewsTab == this) {
        //    if (autoLoadImages_) {
        //        mainWindow_->autoLoadImagesToggle_->setText(tr("Load Images"));
        //        mainWindow_->autoLoadImagesToggle_->setToolTip(tr("Auto Load Images to News View"));
        //    }
        //    else {
        //        mainWindow_->autoLoadImagesToggle_->setText(tr("No Load Images"));
        //        mainWindow_->autoLoadImagesToggle_->setToolTip(tr("No Load Images to News View"));
        //    }
        //}
    }

    m_closeButton->setToolTip(tr("Close Tab"));
}

// =====================================================================================================================

NewsHeader *NewsTabWidget::header() const
{
    return m_newsHeader;
}

// =====================================================================================================================

void NewsTabWidget::slotNewsViewClicked(QModelIndex index)
{
    slotNewsViewSelected(index);
}

// =====================================================================================================================

void NewsTabWidget::slotNewsViewSelected(QModelIndex index, bool clicked)
{
    //if (mainWindow_->newsLayout_ == 1) return;

    const int newsId = m_newsModel->dataField(index.row(), NewsModel::Columns::Id).toInt();

    slotSetItemRead(index, 1);

    //if (mainWindow_->markNewsReadOn_ && mainWindow_->markPrevNewsRead_ && (newsId != currentNewsIdOld)) {
    /*
        QModelIndex startIndex    = m_newsModel->index(0, NewsModel::Columns::Id);
        QModelIndexList indexList = m_newsModel->match(startIndex, Qt::EditRole, currentNewsIdOld);

        if (!indexList.isEmpty()) {
            slotSetItemRead(indexList.first(), 1);
        }
    */
    //}

    if (!index.isValid()) {
        //hideWebContent();
        //currentNewsIdOld = newsId;

        return;
    }

    //if (!((newsId == currentNewsIdOld) && m_newsModel->dataField(index.row(), "read").toInt() >= 1) || clicked) {
        //markNewsReadTimer_->stop();

        //if (mainWindow_->markNewsReadOn_ && mainWindow_->markCurNewsRead_) {
        //    if (mainWindow_->markNewsReadTime_ == 0) {
        //        slotSetItemRead(index, 1);
        //    }
        //    else {
        //        markNewsReadTimer_->start(mainWindow_->markNewsReadTime_ * 1000);
        //    }
        //}

        if (type() == TabType::Feed) {
            // Write current news to feed
            //QString qStr = QString("UPDATE feeds SET currentNews='%1' WHERE id=='%2'").
            //    arg(newsId).arg(feedId_);

            //mainApp->sqlQueryExec(qStr);

            //QModelIndex feedIndex = feedsModel_->indexById(feedId_);
            //feedsModel_->setData(feedsModel_->indexSibling(feedIndex, "currentNews"), newsId);
        }
        else if (type() == TabType::Label) {
            //QString qStr = QString("UPDATE labels SET currentNews='%1' WHERE id=='%2'").
            //    arg(newsId).
            //    arg(mainWindow_->categoriesTree_->currentItem()->text(2).toInt());
            //mainApp->sqlQueryExec(qStr);

            //mainWindow_->categoriesTree_->currentItem()->setText(3, QString::number(newsId));
        }

        //updateWebView(index);
        //mainWindow_->statusBar()->showMessage(linkNewsString_, 3000);
    //}

    //currentNewsIdOld = newsId;
}

// =====================================================================================================================

void NewsTabWidget::slotSetItemRead(QModelIndex index, int read)
{
    //markNewsReadTimer_->stop();

    if (!index.isValid() || (m_newsModel->rowCount() == 0)) {
        return;
    }

    bool changed = false;
    int newsId = m_newsModel->dataField(index.row(), NewsModel::Columns::Id).toInt();

    if (read == 1) {
        if (m_newsModel->dataField(index.row(), NewsModel::Columns::New).toBool() == true) {
            m_newsModel->setData(m_newsModel->index(index.row(), NewsModel::Columns::New), false);
            //mainApp->sqlQueryExec(QString("UPDATE news SET new=0 WHERE id=='%1'").arg(newsId));
        }

        if (m_newsModel->dataField(index.row(), NewsModel::Columns::Read).toBool() == false) {
            m_newsModel->setData(m_newsModel->index(index.row(), NewsModel::Columns::Read), true);

            QVector<int> items;

            const int id = m_newsModel->dataField(index.row(), NewsModel::Columns::Id).toInt();

            items.append(id);

            m_server->setNewsState(items, m_newsModel->dataField(index.row(), NewsModel::Columns::FeedId).toInt(), NewsState::Read);

            //mainApp->sqlQueryExec(QString("UPDATE news SET read=1 WHERE id=='%1'").arg(newsId));
            changed = true;
        }
    }
    else {
        if (m_newsModel->dataField(index.row(), NewsModel::Columns::Read).toBool() == true) {
            m_newsModel->setData(m_newsModel->index(index.row(), NewsModel::Columns::Read), false);
            //mainApp->sqlQueryExec(QString("UPDATE news SET read=0 WHERE id=='%1'").arg(newsId));
            changed = true;
        }
    }

    if (changed) {
        m_newsView->viewport()->update();
        int feedId = m_newsModel->dataField(index.row(), NewsModel::Columns::FeedId).toInt();
        //mainWindow_->slotUpdateStatus(feedId);
        //mainWindow_->recountCategoryCounts();
    }
}

// =====================================================================================================================

void NewsTabWidget::slotSetItemStar(QModelIndex index, int starred)
{
    /*
    if (!index.isValid()) return;

    newsModel_->setData(index, starred);

    int newsId = newsModel_->dataField(index.row(), "id").toInt();
    mainApp->sqlQueryExec(QString("UPDATE news SET starred='%1' WHERE id=='%2'").
                        arg(starred).arg(newsId));
    mainWindow_->recountCategoryCounts();
    */
}

/*
void NewsTabWidget::slotMarkReadTimeout()
{
  slotSetItemRead(newsView_->currentIndex(), 1);
}*/
