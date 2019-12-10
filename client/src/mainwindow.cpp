#include "mainwindow.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName("mainWindow");
    setWindowTitle("QuiteRSS");
    setContextMenuPolicy(Qt::CustomContextMenu);

    createSettings();

    createFeedsWidget();
    createToolBarNull();

    createActions();
    createShortcuts();
    createMenu();
    createToolBar();

    createStatusBar();
    createTray();

    createTabBarWidget();
    createCentralWidget();

    setStyleApp();

    //setStyleSheet("QMainWindow::separator { width: 1px }");

    loadSettings();
    retranslateStrings();

    m_server = Server::instance();

    m_server->connect();

    /*
    installEventFilter(this);
    */
}

// =====================================================================================================================

MainWindow::~MainWindow()
{

}

// =====================================================================================================================

void MainWindow::createSettings()
{
    QString fileName = QCoreApplication::applicationName() % ".ini";

    QSettings::setDefaultFormat(QSettings::IniFormat);
    Settings::createSettings(fileName);
}

// =====================================================================================================================

void MainWindow::createActions()
{
    m_actionAdd = new QAction(this);
    m_actionAdd->setIcon(QIcon(":/images/add"));
    addAction(m_actionAdd);
    //connect(m_actionAdd, SIGNAL(triggered()), this, SLOT(newFeed()));

    m_actionAddFeed = new QAction(this);
    m_actionAddFeed->setIcon(QIcon(":/images/add-feed"));
    addAction(m_actionAddFeed);
    //connect(m_actionAddFeed, SIGNAL(triggered()), this, SLOT(newFeed()));

    m_actionAddFolder = new QAction(this);
    m_actionAddFolder->setIcon(QIcon(":/images/add-folder"));
    addAction(m_actionAddFolder);
    //connect(m_actionAddFolder, SIGNAL(triggered()), this, SLOT(newFeedFolder()));

    m_actionImportFeeds = new QAction(this);
    m_actionImportFeeds->setIcon(QIcon(":/images/import-feeds"));
    addAction(m_actionImportFeeds);
    //connect(m_actionImportFeeds, SIGNAL(triggered()), this, SLOT(slotImportFeeds()));

    m_actionExportFeeds = new QAction(this);
    m_actionExportFeeds->setIcon(QIcon(":/images/export-feeds"));
    addAction(m_actionExportFeeds);
    //connect(m_actionExportFeeds, SIGNAL(triggered()), this, SLOT(slotExportFeeds()));

    m_actionShowMenuBar = new QAction(this);
    m_actionShowMenuBar->setCheckable(true);
    connect(m_actionShowMenuBar, SIGNAL(triggered()), this, SLOT(showMenuBar()));

    m_actionExit = new QAction(this);
    this->addAction(m_actionExit);
    connect(m_actionExit, SIGNAL(triggered()), this, SLOT(quitApp()));

    m_actionFeedsWidgetVisible = new QAction(this);
    m_actionFeedsWidgetVisible->setObjectName("visibleFeedsWidgetAct");
    m_actionFeedsWidgetVisible->setCheckable(true);

    m_actionMainToolBarToggle = new QAction(this);
    m_actionMainToolBarToggle->setCheckable(true);

    connect(m_actionFeedsWidgetVisible, SIGNAL(triggered()), this, SLOT(slotVisibledFeedsWidget()));
    connect(m_pushButtonNull, SIGNAL(clicked()), m_actionFeedsWidgetVisible, SLOT(trigger()));
    this->addAction(m_actionFeedsWidgetVisible);

    m_actionToolBarLock = new QAction(this);
    m_actionToolBarLock->setCheckable(true);

    m_actionToolBarHide = new QAction(this);

    m_actionCategoriesPanelToggle = new QAction(this);
    m_actionCategoriesPanelToggle->setCheckable(true);

    connect(m_actionCategoriesPanelToggle, SIGNAL(toggled(bool)), m_categoriesWidget, SLOT(setVisible(bool)));

    m_actionShowDownloadManager = new QAction(this);
    m_actionShowDownloadManager->setObjectName("showDownloadManagerAct");
    m_actionShowDownloadManager->setIcon(QIcon(":/images/download"));
    this->addAction(m_actionShowDownloadManager);
    //connect(m_actionShowDownloadManager, SIGNAL(triggered()), this, SLOT(showDownloadManager()));

    m_actionSetNewsFilters = new QAction(this);
    m_actionSetNewsFilters->setObjectName("setNewsFiltersAct");
    m_actionSetNewsFilters->setIcon(QIcon(":/images/filters"));
    this->addAction(m_actionSetNewsFilters);
    //connect(m_actionSetNewsFilters, SIGNAL(triggered()), this, SLOT(showNewsFiltersDlg()));
    /*
    setFilterNewsAct_ = new QAction(this);
    setFilterNewsAct_->setObjectName("setFilterNewsAct");
    setFilterNewsAct_->setIcon(QIcon(":/images/filterOff"));
    this->addAction(setFilterNewsAct_);
    connect(setFilterNewsAct_, SIGNAL(triggered()), this, SLOT(showFilterRulesDlg()));
    */

    m_actionOptions = new QAction(this);
    m_actionOptions->setObjectName("optionsAct");
    m_actionOptions->setIcon(QIcon(":/images/options"));
    this->addAction(m_actionOptions);
    //connect(m_actionOptions, SIGNAL(triggered()), this, SLOT(showOptionDlg()));

    m_actionUpdateApp = new QAction(this);
    m_actionUpdateApp->setObjectName("UpdateApp");
    //connect(m_actionUpdateApp, SIGNAL(triggered()), this, SLOT(slotShowUpdateAppDlg()));

    m_actionReportProblem = new QAction(this);
    m_actionReportProblem->setObjectName("reportProblemAct_");
    //connect(m_actionReportProblem, SIGNAL(triggered()), this, SLOT(slotReportProblem()));

    m_actionAbout = new QAction(this);
    m_actionAbout->setObjectName("About");
    //connect(m_actionAbout, SIGNAL(triggered()), this, SLOT(slotShowAboutDlg()));
}

// =====================================================================================================================

void MainWindow::createShortcuts()
{
    m_actionAddFeed->setShortcut(QKeySequence(QKeySequence::New));
    //listActions_.append(m_actionAddFeed);
    m_actionAddFolder->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
    //listActions_.append(m_actionAddFolder);
    //listActions_.append(deleteFeedAct_);
    //listActions_.append(createBackupAct_);
    m_actionExit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
}

// =====================================================================================================================

void MainWindow::createMenu()
{
    m_fileAddMenu = new QMenu(this);
    m_fileAddMenu->addAction(m_actionAddFeed);
    m_fileAddMenu->addAction(m_actionAddFolder);
    m_actionAdd->setMenu(m_fileAddMenu);

    m_fileMenu = new QMenu(this);

    m_fileMenu->addAction(m_actionAdd);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_actionImportFeeds);
    m_fileMenu->addAction(m_actionExportFeeds);
    m_fileMenu->addSeparator();
#ifndef Q_OS_MAC
    m_fileMenu->addAction(m_actionShowMenuBar);
    m_fileMenu->addSeparator();
#endif
    m_fileMenu->addAction(m_actionExit);

    m_toolbarsMenu = new QMenu(this);
    m_toolbarsMenu->addAction(m_actionMainToolBarToggle);
    /*
    m_toolbarsMenu->addAction(feedsToolbarToggle_);
    m_toolbarsMenu->addAction(newsToolbarToggle_);
    m_toolbarsMenu->addAction(browserToolbarToggle_);
    */
    m_toolbarsMenu->addAction(m_actionCategoriesPanelToggle);
    //m_toolbarsMenu->addAction(statusBarToggle_);

    m_viewMenu  = new QMenu(this);
    m_viewMenu->addMenu(m_toolbarsMenu);
    //m_viewMenu->addMenu(m_customizeToolbarMenu);
    m_viewMenu->addSeparator();
    /*
    m_viewMenu->addMenu(layoutMenu_);
    m_viewMenu->addMenu(browserPositionMenu_);
    m_viewMenu->addMenu(styleMenu_);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(stayOnTopAct_);
    m_viewMenu->addAction(fullScreenAct_);
    */

    m_toolsMenu = new QMenu(this);
    m_toolsMenu->addAction(m_actionShowDownloadManager);
    m_toolsMenu->addSeparator();
    m_toolsMenu->addAction(m_actionSetNewsFilters);
    m_toolsMenu->addSeparator();
    m_toolsMenu->addAction(m_actionOptions);

    m_helpMenu = new QMenu(this);
    m_helpMenu->addAction(m_actionUpdateApp);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_actionReportProblem);
    m_helpMenu->addAction(m_actionAbout);

    menuBar()->addMenu(m_fileMenu);
    menuBar()->addMenu(m_viewMenu);
    menuBar()->addMenu(m_toolsMenu);
    menuBar()->addMenu(m_helpMenu);

#ifndef Q_OS_MAC
    m_mainMenu = new QMenu(this);
    m_mainMenu->addAction(m_actionAdd);
    m_mainMenu->addSeparator();
    m_mainMenu->addAction(m_actionImportFeeds);
    m_mainMenu->addAction(m_actionExportFeeds);
    //m_mainMenu->addSeparator();
    //m_mainMenu->addAction(createBackupAct_);
    m_mainMenu->addSeparator();
    m_mainMenu->addMenu(m_viewMenu);
    /*
    m_mainMenu->addMenu(feedMenu_);
    m_mainMenu->addMenu(newsMenu_);
    m_mainMenu->addMenu(browserMenu_);
    */
    m_mainMenu->addSeparator();
    m_mainMenu->addMenu(m_toolsMenu);
    m_mainMenu->addSeparator();
    m_mainMenu->addMenu(m_helpMenu);

    m_mainMenu->addSeparator();
    m_mainMenu->addAction(m_actionShowMenuBar);
    m_mainMenu->addSeparator();
    m_mainMenu->addAction(m_actionExit);
#endif
}

// =====================================================================================================================

void MainWindow::createToolBar()
{
    m_mainToolBar = new QToolBar(this);

    m_mainToolBar->setObjectName("ToolBar_General");
    m_mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_mainToolBar->setContextMenuPolicy(Qt::CustomContextMenu);

    addToolBar(m_mainToolBar);

    connect(m_actionMainToolBarToggle, SIGNAL(toggled(bool)), m_mainToolBar, SLOT(setVisible(bool)));
    connect(m_mainToolBar, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenuToolBar(const QPoint &)));

    connect(m_actionToolBarLock, SIGNAL(toggled(bool)), this, SLOT(lockMainToolBar(bool)));
    connect(m_actionToolBarHide, SIGNAL(triggered()), this, SLOT(hideMainToolBar()));
}

// =====================================================================================================================

void MainWindow::createStatusBar()
{
#if defined(HAVE_X11) || defined(Q_OS_MAC)
    statusBar()->setStyleSheet(QString("QStatusBar::item { border-right: 1px solid %1; margin: 1px }")
                               .arg(qApp->palette().color(QPalette::Dark).name()));
#endif

    m_progressBar = new QProgressBar(this);

    m_progressBar->setObjectName("m_progressBar");
    m_progressBar->setFormat("%p%");
    m_progressBar->setAlignment(Qt::AlignCenter);
    m_progressBar->setFixedWidth(100);
    m_progressBar->setFixedHeight(15);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(0);
    m_progressBar->setValue(0);
    m_progressBar->setVisible(false);

    statusBar()->installEventFilter(this);
    statusBar()->addPermanentWidget(m_progressBar);

    m_statusUnread = new QLabel(this);
    m_statusUnread->hide();
    statusBar()->addPermanentWidget(m_statusUnread);

    m_statusAll = new QLabel(this);
    m_statusAll->hide();
    statusBar()->addPermanentWidget(m_statusAll);

    statusBar()->setVisible(true);
}

// =====================================================================================================================

void MainWindow::createTray()
{
    //traySystem = new QSystemTrayIcon(QIcon(":/images/quiterss128"), this);
    //traySystem->setToolTip("QuiteRSS");
/*
#ifndef Q_OS_MAC
    connect(traySystem,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(slotActivationTray(QSystemTrayIcon::ActivationReason)));
    connect(this, SIGNAL(signalPlaceToTray()),
            this, SLOT(slotPlaceToTray()), Qt::QueuedConnection);
#endif
*/
    //createTrayMenu();
}

// =====================================================================================================================

void MainWindow::createTabBarWidget()
{
    m_tabBar = new TabBar();

    m_mainMenuButton = new ToolButton(this);
    m_mainMenuButton->setObjectName("mainMenuButton");
    m_mainMenuButton->setIcon(QIcon(":/images/menu"));

    QHBoxLayout *tabBarLayout = new QHBoxLayout();
    tabBarLayout->setContentsMargins(0, 0, 0, 0);
    tabBarLayout->setSpacing(0);
    tabBarLayout->addWidget(m_mainMenuButton);
    tabBarLayout->addSpacing(5);
    tabBarLayout->addWidget(m_tabBar, 1);

    m_tabBarWidget = new QWidget(this);
    m_tabBarWidget->setObjectName("tabBarWidget");
    m_tabBarWidget->setMinimumHeight(1);
    m_tabBarWidget->setLayout(tabBarLayout);

    m_mainMenuButton->setStyleSheet("#mainMenuButton { border: none; padding: 0px 0px 0px 5px }");

    connect(m_mainMenuButton, SIGNAL(clicked()), this, SLOT(showMainMenu()));
}

// =====================================================================================================================

void MainWindow::createFeedsWidget()
{
    m_feedsModel = new FeedsModel(this);

    m_feedsView = new FeedsView(this);
    m_feedsView->setModel(m_feedsModel);
    m_feedsView->setSourceModel(m_feedsModel);

    m_feedsView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_feedsView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    //m_feedsView->refresh();

    //m_feedsView->setSourceModel(m_feedsModel);
    //m_feedsModel->setView(m_feedsView);
/*
    for (int i = 0; i < m_feedsView->model()->columnCount(); ++i) {
        m_feedsView->hideColumn(i);
    }

    m_feedsView->showColumn(m_feedsView->columnIndex("text"));

    m_feedsView->header()->setSectionResizeMode(m_feedsView->columnIndex("text"), QHeaderView::Stretch);
    m_feedsView->header()->setSectionResizeMode(m_feedsView->columnIndex("unread"), QHeaderView::ResizeToContents);
    m_feedsView->header()->setSectionResizeMode(m_feedsView->columnIndex("undeleteCount"), QHeaderView::ResizeToContents);
    m_feedsView->header()->setSectionResizeMode(m_feedsView->columnIndex("updated"), QHeaderView::ResizeToContents);

    feedsToolBar_ = new QToolBar(this);
    feedsToolBar_->setObjectName("feedsToolBar");
    feedsToolBar_->setStyleSheet("QToolBar { border: none; padding: 0px }");
*/
    QHBoxLayout *feedsPanelLayout = new QHBoxLayout();
    feedsPanelLayout->setMargin(2);
//    feedsPanelLayout->addWidget(feedsToolBar_, 1);

    m_feedsPanel = new QWidget(this);
    m_feedsPanel->setObjectName("feedsPanel");
    m_feedsPanel->setStyleSheet(
          QString("#m_feedsPanel { border-bottom: 1px solid %1 }")
                     .arg(qApp->palette().color(QPalette::Dark).name()));
    m_feedsPanel->setLayout(feedsPanelLayout);

    m_categoriesTree = new CategoriesTreeWidget(this);

    m_categoriesLabel = new QLabel(this);
    m_categoriesLabel->setObjectName("categoriesLabel");
    m_categoriesLabel->setText("Категории");

    m_showCategoriesButton = new ToolButton(this);
    m_showCategoriesButton->setFocusPolicy(Qt::NoFocus);
    m_showCategoriesButton->setMaximumSize(16, 16);
    m_showCategoriesButton->setAutoRaise(true);

    QHBoxLayout *categoriesPanelLayout = new QHBoxLayout();
    categoriesPanelLayout->setMargin(2);
    categoriesPanelLayout->addSpacing(2);
    categoriesPanelLayout->addWidget(m_categoriesLabel, 1);
    categoriesPanelLayout->addWidget(m_showCategoriesButton);

    m_categoriesPanel = new QWidget(this);
    m_categoriesPanel->setObjectName("categoriesPanel");
    m_categoriesPanel->setLayout(categoriesPanelLayout);

    QVBoxLayout *categoriesLayout = new QVBoxLayout();
    categoriesLayout->setMargin(0);
    categoriesLayout->setSpacing(0);
    categoriesLayout->addWidget(m_categoriesPanel);
    categoriesLayout->addWidget(m_categoriesTree, 1);

    m_categoriesWidget = new QWidget(this);
    m_categoriesWidget->setLayout(categoriesLayout);

    m_feedsSplitter = new QSplitter(Qt::Vertical);
    m_feedsSplitter->setChildrenCollapsible(false);
    m_feedsSplitter->setHandleWidth(1);
    m_feedsSplitter->setStyleSheet(
          QString("QSplitter::handle { background: %1 }").
          arg(qApp->palette().color(QPalette::Dark).name()));
    m_feedsSplitter->addWidget(m_feedsView);
    m_feedsSplitter->addWidget(m_categoriesWidget);
    m_feedsSplitter->setStretchFactor(0, 1);

#define CATEGORIES_HEIGHT 210

    QList <int> sizes;
    sizes << QApplication::desktop()->height() << CATEGORIES_HEIGHT;
    m_feedsSplitter->setSizes(sizes);

    QVBoxLayout *feedsLayout = new QVBoxLayout();
    feedsLayout->setMargin(0);
    feedsLayout->setSpacing(0);
    feedsLayout->addWidget(m_feedsPanel);
    //feedsLayout->addWidget(findFeedsWidget_);
    feedsLayout->addWidget(m_feedsSplitter, 1);

    m_feedsWidget = new QFrame(this);
    m_feedsWidget->setFrameStyle(QFrame::NoFrame);
    m_feedsWidget->setLayout(feedsLayout);

    connect(m_feedsView, SIGNAL(pressed(QModelIndex)), this, SLOT(slotFeedClicked(QModelIndex)));

    connect(m_showCategoriesButton, SIGNAL(clicked()), this, SLOT(showNewsCategoriesTree()));

    connect(m_feedsSplitter, SIGNAL(splitterMoved(int, int)), this, SLOT(feedsSplitterMoved(int)));

    //categoriesLabel_->installEventFilter(this);
    //categoriesTree_->installEventFilter(this);
}

// =====================================================================================================================

void MainWindow::createToolBarNull()
{
    m_pushButtonNull = new QPushButton(this);

    m_pushButtonNull->setObjectName("pushButtonNull");
    m_pushButtonNull->setIcon(QIcon(":/images/arrow-right"));
    m_pushButtonNull->setFixedWidth(6);
    m_pushButtonNull->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pushButtonNull->setFocusPolicy(Qt::NoFocus);
    m_pushButtonNull->setStyleSheet("background: #E8E8E8; border: none; padding: 0px");
}

// =====================================================================================================================

void MainWindow::createCentralWidget()
{
    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setObjectName("stackedWidget");
    m_stackedWidget->setFrameStyle(QFrame::NoFrame);

    m_mainSplitter = new QSplitter(this);
    m_mainSplitter ->setFrameStyle(QFrame::NoFrame);
    m_mainSplitter->setHandleWidth(1);

    m_mainSplitter->setStyleSheet(
        QString("QSplitter::handle {background: qlineargradient("
                "x1: 0, y1: 0, x2: 0, y2: 1,"
                "stop: 0 %1, stop: 0.07 %2);}")
                .arg(m_feedsPanel->palette().background().color().name())
                .arg(qApp->palette().color(QPalette::Dark).name()));

    m_mainSplitter->setChildrenCollapsible(false);
    m_mainSplitter->addWidget(m_feedsWidget);
    m_mainSplitter->addWidget(m_stackedWidget);
    m_mainSplitter->setStretchFactor(1, 1);

#define FEEDS_WIDTH 180

    QList <int> sizes;
    sizes << FEEDS_WIDTH << QApplication::desktop()->width();
    m_mainSplitter->setSizes(sizes);

    QHBoxLayout *mainLayout1 = new QHBoxLayout();
    mainLayout1->addWidget(m_pushButtonNull);
    mainLayout1->addWidget(m_mainSplitter, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_tabBarWidget);
    mainLayout->addLayout(mainLayout1, 1);

    m_centralWidget = new QWidget(this);
    m_centralWidget->setLayout(mainLayout);

    setCentralWidget(m_centralWidget);

    NewsTabWidget *widget = new NewsTabWidget(this, m_feedsModel);

    // ============
    m_currentNewsTab = widget;
    // ============

    addTab(widget);






    m_currentNewsTab->retranslateStrings();
}

// =====================================================================================================================

void MainWindow::setStyleApp()
{
    QFile file(":/styles/purple");
    file.open(QFile::ReadOnly);

    qApp->setStyleSheet(QLatin1String(file.readAll()));
    file.close();
}

// =====================================================================================================================

void MainWindow::loadSettings()
{
    m_mainToolBar->setIconSize(QSize(32, 32));

    Settings settings;
    settings.beginGroup("Settings");

    resize(800, 600);
    restoreGeometry(settings.value("GeometryState").toByteArray());

    m_mainSplitter->restoreState(settings.value("MainSplitterState").toByteArray());
    m_actionFeedsWidgetVisible->setChecked(settings.value("FeedsWidgetVisible", true).toBool());
    slotVisibledFeedsWidget();

    m_feedsWidgetSplitterState = settings.value("FeedsWidgetSplitterState").toByteArray();

    bool isShowCategories = settings.value("NewsCategoriesTreeVisible", true).toBool();

    m_categoriesTree->setVisible(isShowCategories);

    if (isShowCategories) {
        m_showCategoriesButton->setIcon(QIcon(":/images/panel-hide"));
        m_showCategoriesButton->setToolTip(tr("Hide Categories"));

        m_feedsSplitter->restoreState(m_feedsWidgetSplitterState);
    }
    else {
        m_showCategoriesButton->setIcon(QIcon(":/images/panel-show"));
        m_showCategoriesButton->setToolTip(tr("Show Categories"));

        QList<int> sizes;
        sizes << QApplication::desktop()->height() << 20;
        m_feedsSplitter->setSizes(sizes);
    }

#ifndef Q_OS_MAC
    m_actionShowMenuBar->setChecked(settings.value("showMenuBar", false).toBool());
#else
    m_actionShowMenuBar->setChecked(true);
#endif

    m_actionMainToolBarToggle->setChecked(settings.value("mainToolbarShow2", false).toBool());
    m_actionCategoriesPanelToggle->setChecked(settings.value("categoriesPanelShow", true).toBool());
    m_categoriesWidget->setVisible(m_actionCategoriesPanelToggle->isChecked());

    settings.endGroup();

    if (!m_actionMainToolBarToggle->isChecked()) {
        m_mainToolBar->hide();
    }

    showMenuBar();
}

// =====================================================================================================================

void MainWindow::saveSettings()
{
    qDebug() << "Save settings...";

    Settings settings;
    settings.beginGroup("Settings");

    settings.setValue("GeometryState", saveGeometry());

    settings.setValue("MainSplitterState", m_mainSplitter->saveState());
    //settings.setValue("FeedsWidgetVisible", showFeedsTabPermanent_);

    bool isShowCategories = true;

    if (m_categoriesWidget->height() <= (m_categoriesPanel->height() + 2)) {
        isShowCategories = false;

        settings.setValue("FeedsWidgetSplitterState", m_feedsWidgetSplitterState);
    }
    else {
        settings.setValue("FeedsWidgetSplitterState", m_feedsSplitter->saveState());
    }

    settings.setValue("NewsCategoriesTreeVisible", isShowCategories);
    //settings.setValue("categoriesTreeExpanded", m_categoriesTree->topLevelItem(CategoriesTreeWidget::LabelsItem)->isExpanded());

    settings.setValue("mainToolbarShow2", m_actionMainToolBarToggle->isChecked());
    settings.setValue("categoriesPanelShow", m_actionCategoriesPanelToggle->isChecked());

    settings.setValue("showMenuBar", m_actionShowMenuBar->isChecked());

    settings.endGroup();

    if (m_stackedWidget->count()) {
        NewsTabWidget *widget;

        if (m_currentNewsTab->type() < NewsTabWidget::TabType::Web) {
            widget = m_currentNewsTab;
        }
        else {
            widget = (NewsTabWidget*)m_stackedWidget->widget(TAB_WIDGET_PERMANENT);
        }

        widget->header()->saveStateColumns(widget);

//        settings.setValue("NewsTabSplitterState",
//                          widget->newsTabWidgetSplitter_->saveState());
    }

    settings.syncSettings();
}

// =====================================================================================================================

void MainWindow::retranslateStrings()
{
    m_fileMenu->setTitle(tr("&File"));
    m_viewMenu->setTitle(tr("&View"));
    m_toolsMenu->setTitle(tr("&Tools"));
    m_helpMenu->setTitle(tr("&Help"));

    m_actionAdd->setText(tr("&Add"));
    m_actionAdd->setToolTip(tr("Add New Feed"));

    m_actionAddFeed->setText(tr("&Feed..."));
    m_actionAddFeed->setToolTip(tr("Add New Feed"));

    m_actionAddFolder->setText(tr("F&older..."));
    m_actionAddFolder->setToolTip(tr("Add New Folder"));

    m_actionImportFeeds->setText(tr("&Import Feeds..."));
    m_actionImportFeeds->setToolTip(tr("Import Feeds from OPML File"));

    m_actionExportFeeds->setText(tr("&Export Feeds..."));
    m_actionExportFeeds->setToolTip(tr("Export Feeds to OPML File"));

    m_actionShowMenuBar->setText(tr("S&how Menu Bar"));

    m_actionExit->setText(tr("E&xit"));

    m_actionFeedsWidgetVisible->setText(tr("Show/Hide Tree Feeds"));

    m_actionToolBarLock->setText(tr("Lock Toolbar"));
    m_actionToolBarHide->setText(tr("Hide Toolbar"));

    m_toolbarsMenu->setTitle(tr("Show/Hide"));
    m_actionMainToolBarToggle->setText(tr("Main Toolbar"));
    m_actionCategoriesPanelToggle->setText(tr("Panel Categories"));

    m_actionShowDownloadManager->setText(tr("Downloads"));
    m_actionSetNewsFilters->setText(tr("News Filters..."));

    m_actionOptions->setText(tr("Options..."));
    m_actionOptions->setToolTip(tr("Open Options Dialog"));

    m_actionUpdateApp->setText(tr("Check for Updates..."));
    m_actionReportProblem->setText(tr("Report a Problem..."));

    m_actionAbout ->setText(tr("About..."));
    m_actionAbout->setToolTip(tr("Show \"About\" Dialog"));

}

// =====================================================================================================================

int MainWindow::addTab(NewsTabWidget *widget)
{
    int tabIndex;

    if (!m_stackedWidget->count()) {
        m_tabBar->addTab("");
    }

    tabIndex = m_stackedWidget->addWidget(widget);

    m_tabBar->setTabButton(tabIndex, QTabBar::LeftSide, &widget->titleLabel());

    return tabIndex;
}

// =====================================================================================================================

void MainWindow::showNewsCategoriesTree()
{
    if (m_categoriesTree->isHidden()) {
        m_showCategoriesButton->setIcon(QIcon(":/images/panel-hide"));
        m_showCategoriesButton->setToolTip(tr("Hide Categories"));

        m_categoriesTree->show();

        m_feedsSplitter->restoreState(m_feedsWidgetSplitterState);
    }
    else {
        m_feedsWidgetSplitterState = m_feedsSplitter->saveState();

        m_showCategoriesButton->setIcon(QIcon(":/images/panel-show"));
        m_showCategoriesButton->setToolTip(tr("Show Categories"));

        m_categoriesTree->hide();

        QList<int> sizes;
        sizes << height() << 20;

        m_feedsSplitter->setSizes(sizes);
   }
}

// =====================================================================================================================

void MainWindow::slotFeedClicked(QModelIndex index)
{
    if (!index.isValid()){
        return;
    }

    slotFeedSelected(index, false);

  //if (feedsView_->selectionModel()->selectedRows(0).count() > 1) return;

  //int feedIdCur = feedsModel_->idByIndex(feedsProxyModel_->mapToSource(index));
/*
  if (stackedWidget_->count() && currentNewsTab->type_ < NewsTabWidget::TabTypeWeb) {
    currentNewsTab->newsHeader_->saveStateColumns(currentNewsTab);
  }

  // Search open tab containing this feed
  int indexTab = -1;
  for (int i = 0; i < stackedWidget_->count(); i++) {
    NewsTabWidget *widget = (NewsTabWidget*)stackedWidget_->widget(i);
    if (widget->feedId_ == feedIdCur) {
      indexTab = i;
      break;
    }
  }

  if (indexTab == -1) {
    if (tabBar_->currentIndex() != TAB_WIDGET_PERMANENT) {
      setFeedRead(currentNewsTab->type_, currentNewsTab->feedId_, FeedReadSwitchingTab, currentNewsTab);

      updateCurrentTab_ = false;
      tabBar_->setCurrentIndex(TAB_WIDGET_PERMANENT);
      updateCurrentTab_ = true;
      QModelIndex currentIndex = feedsProxyModel_->mapFromSource(feedIdCur);
      feedsView_->setCurrentIndex(currentIndex);

      currentNewsTab = (NewsTabWidget*)stackedWidget_->widget(TAB_WIDGET_PERMANENT);
      newsModel_ = currentNewsTab->newsModel_;
      newsView_ = currentNewsTab->newsView_;
    } else {
      if (stackedWidget_->count() && currentNewsTab->type_ != NewsTabWidget::TabTypeFeed) {
        setFeedRead(currentNewsTab->type_, currentNewsTab->feedId_, FeedReadSwitchingFeed, currentNewsTab);
      } else {
        // Mark previous feed Read while switching to another feed
        setFeedRead(NewsTabWidget::TabTypeFeed, feedIdOld_, FeedReadSwitchingFeed, 0, feedIdCur);
      }

      categoriesTree_->setCurrentIndex(QModelIndex());
    }

    slotFeedSelected(feedsModel_->indexById(feedIdCur));
    feedsView_->repaint();
  } else if (indexTab != -1) {
    tabBar_->setCurrentIndex(indexTab);
  }
  feedIdOld_ = feedIdCur;
  */
}

// =====================================================================================================================

void MainWindow::slotFeedSelected(QModelIndex index, bool createTab)
{
    /*
  int feedId = feedsModel_->idByIndex(index);
  int feedParId = feedsModel_->paridByIndex(index);

  // Open or create feed tab
  if (!stackedWidget_->count() || createTab) {
    NewsTabWidget *widget = new NewsTabWidget(this, NewsTabWidget::TabTypeFeed, feedId, feedParId);
    int indexTab = addTab(widget);

    createNewsTab(indexTab);

    if (indexTab == 0)
      currentNewsTab->closeButton_->setVisible(false);
    if (!index.isValid())
      currentNewsTab->setVisible(false);

    emit signalSetCurrentTab(indexTab);
  } else {
  */
    m_currentNewsTab->setType(NewsTabWidget::Feed);
    //m_currentNewsTab->feedId_ = feedId;
    //m_currentNewsTab->feedParId_ = feedParId;
    //m_currentNewsTab->setSettings(true, false);
    //m_currentNewsTab->setVisible(index.isValid());
  /*}
  statusUnread_->setVisible(index.isValid());
  statusAll_->setVisible(index.isValid());

  // Set icon for tab has opened
  bool isFeed = (index.isValid() && feedsModel_->isFolder(index)) ? false : true;

  QPixmap iconTab;
  QByteArray byteArray = feedsModel_->dataField(index, "image").toByteArray();
  if (!isFeed) {
    iconTab.load(":/images/folder");
  } else {
    if (byteArray.isNull() || defaultIconFeeds_) {
      iconTab.load(":/images/feed");
    } else if (isFeed) {
      iconTab.loadFromData(QByteArray::fromBase64(byteArray));
    }
  }
  currentNewsTab->newsIconTitle_->setPixmap(iconTab);
*/
    // Set icon and title for tab has opened

    FeedItem *feedItem = static_cast<FeedItem*>(index.internalPointer());

    //FeedItem *feedItem = m_feedsModel->feedItem(index);

    if (feedItem) {
        m_currentNewsTab->setFeedItem(feedItem);

        if (feedItem->newCount()) {
            m_server->setFeedViewed(feedItem->id());
        }
    }
/*
  feedProperties_->setEnabled(index.isValid());

  setNewsFilter(newsFilterGroup_->checkedAction(), false);

  // Search feed news that displayed before
  int newsRow = -1;
  if (openingFeedAction_ == 0) {
    QModelIndex feedIndex = feedsModel_->indexById(feedId);
    int newsIdCur = feedsModel_->dataField(feedIndex, "currentNews").toInt();
    QModelIndex index = newsModel_->index(0, newsModel_->fieldIndex("id"));
    QModelIndexList indexList = newsModel_->match(index, Qt::EditRole, newsIdCur);

    if (!indexList.isEmpty()) newsRow = indexList.first().row();
  } else if (openingFeedAction_ == 1) {
    newsRow = 0;
  } else if ((openingFeedAction_ == 3) || (openingFeedAction_ == 4)) {
    QModelIndex index = newsModel_->index(0, newsModel_->fieldIndex("read"));
    QModelIndexList indexList;
    if ((newsView_->header()->sortIndicatorOrder() == Qt::DescendingOrder) &&
        (openingFeedAction_ != 4))
      indexList = newsModel_->match(index, Qt::EditRole, 0, -1);
    else
      indexList = newsModel_->match(index, Qt::EditRole, 0);

    if (!indexList.isEmpty()) newsRow = indexList.last().row();
  }

  // Focus feed news that displayed before
  newsView_->setCurrentIndex(newsModel_->index(newsRow, newsModel_->fieldIndex("title")));
  if (newsRow == -1) newsView_->verticalScrollBar()->setValue(newsRow);

  if ((openingFeedAction_ != 2) && openNewsWebViewOn_) {
    currentNewsTab->slotNewsViewSelected(newsModel_->index(newsRow, newsModel_->fieldIndex("title")));
  } else {
    currentNewsTab->slotNewsViewSelected(newsModel_->index(-1, newsModel_->fieldIndex("title")));
    int newsId = newsModel_->index(newsRow, newsModel_->fieldIndex("id")).data(Qt::EditRole).toInt();
    QString qStr = QString("UPDATE feeds SET currentNews='%1' WHERE id=='%2'").arg(newsId).arg(feedId);
    mainApp->sqlQueryExec(qStr);
    QModelIndex feedIndex = feedsModel_->indexById(feedId);
    feedsModel_->setData(feedsModel_->indexSibling(feedIndex, "currentNews"), newsId);
  }
  */
}

// =====================================================================================================================

void MainWindow::feedsSplitterMoved(int pos)
{
    if (m_categoriesTree->isHidden()) {
        int height = pos + m_categoriesPanel->height() + 2;

        if (height < m_feedsSplitter->height()) {
            m_showCategoriesButton->setIcon(QIcon(":/images/panel-hide"));
            m_showCategoriesButton->setToolTip(tr("Hide Categories"));

            m_categoriesTree->show();

            //recountCategoryCounts();
        }
    }
}

// =====================================================================================================================

void MainWindow::closeEvent(QCloseEvent *event)
{
    //if (mainApp->isClosing())
    //    return;

    //if (closingTray_ && showTrayIcon_) {
    //    event->ignore();
    //    isOpeningLink_ = false;

    //    oldState = windowState();
    //    emit signalPlaceToTray();
    //}
    //else {
#if defined(Q_OS_MAC)
        event->ignore();
#endif
        quitApp();
    //}
}

// =====================================================================================================================

void MainWindow::quitApp()
{
    //mainApp->setClosing();
    //isMinimizeToTray_ = true;
    disconnect(this);

    //if (updateAppDialog_) {
    //    updateAppDialog_->disconnectObjects();
    //    updateAppDialog_->deleteLater();
    //}

    //if (optionsDialog_) {
    //    optionsDialog_->close();
    //}

    //mainApp->showClosingWidget();

    hide();
    //traySystem->hide();

    saveSettings();

    for (int i = 0; i < m_stackedWidget->count(); i++) {
        NewsTabWidget *widget = qobject_cast<NewsTabWidget *>(m_stackedWidget->widget(i));

        widget->disconnectObjects();
    }

    //mainApp->networkManager()->disconnect(mainApp->networkManager());
    //mainApp->updateFeeds()->disconnectObjects();

    emit signalQuitApp();

    QApplication::quit();
}

// =====================================================================================================================

void MainWindow::slotVisibledFeedsWidget()
{
    /*
    if (m_tabBar->currentIndex() == TAB_WIDGET_PERMANENT) {
        m_showFeedsTabPermanent = m_feedsWidgetVisibleAct->isChecked();
    }
    */

    m_feedsWidget->setVisible(m_actionFeedsWidgetVisible->isChecked());

    updateIconToolBarNull(m_actionFeedsWidgetVisible->isChecked());
}

// =====================================================================================================================

void MainWindow::updateIconToolBarNull(bool isFeedsWidgetVisible)
{
    if (isFeedsWidgetVisible) {
        m_pushButtonNull->setIcon(QIcon(":/images/arrow-right"));
    }
    else {
        m_pushButtonNull->setIcon(QIcon(":/images/arrow-left"));
    }
}

// =====================================================================================================================

void MainWindow::showMenuBar()
{
    if (m_actionShowMenuBar->isChecked()) {
        m_mainMenuButton->hide();

        if (/*isFullScreen()*/ false) {
            menuBar()->hide();
        }
        else {
            menuBar()->show();
        }
    }
    else {
        m_mainMenuButton->show();

        menuBar()->hide();
    }
}

// =====================================================================================================================

void MainWindow::showMainMenu()
{
#ifndef Q_OS_MAC
    m_mainMenu->popup(m_mainMenuButton->mapToGlobal(QPoint(0, m_mainMenuButton->height())));
#endif
}

// =====================================================================================================================

void MainWindow::showContextMenuToolBar(const QPoint &pos)
{
    QMenu menu;

    //menu.addAction(customizeMainToolbarAct2_);
    //menu.addSeparator();
    menu.addAction(m_actionToolBarLock);
    menu.addAction(m_actionToolBarHide);

    menu.exec(m_mainToolBar->mapToGlobal(pos));
}

// =====================================================================================================================

void MainWindow::lockMainToolBar(bool lock)
{
    m_mainToolBar->setMovable(!lock);
}

// =====================================================================================================================

void MainWindow::hideMainToolBar()
{
    m_actionMainToolBarToggle->setChecked(false);
    m_mainToolBar->hide();
}
