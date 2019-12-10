#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "feedsmodel.h"
#include "feedsview.h"
#include "categoriestreewidget.h"
#include "newstabwidget.h"
#include "tabbar.h"
#include "toolbutton.h"
#include "server.h"

#define TAB_WIDGET_PERMANENT 0

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void closeEvent(QCloseEvent *event);

    void createSettings();

    void createActions();
    void createShortcuts();

    void createMenu();
    void createToolBar();
    void createStatusBar();
    void createTray();

    void createTabBarWidget();
    void createFeedsWidget();
    void createToolBarNull();
    void createCentralWidget();
    void setStyleApp();

    void loadSettings();
    void saveSettings();
    void retranslateStrings();

    int addTab(NewsTabWidget *widget);

    QMenu *m_fileAddMenu;
    QMenu *m_fileMenu;
    QMenu *m_viewMenu;
    QMenu *m_toolsMenu;
    QMenu *m_helpMenu;
    QMenu *m_toolbarsMenu;

    QMenu *m_mainMenu;

    QToolBar *m_mainToolBar;
    TabBar *m_tabBar;

    QProgressBar *m_progressBar;
    QLabel *m_statusUnread;
    QLabel *m_statusAll;

    QStackedWidget *m_stackedWidget;
    QSplitter *m_mainSplitter;
    QWidget *m_centralWidget;

    FeedsModel *m_feedsModel;
    FeedsView *m_feedsView;
    CategoriesTreeWidget *m_categoriesTree;
    QWidget *m_categoriesPanel;

    QSplitter *m_feedsSplitter;
    QFrame *m_feedsWidget;
    QWidget *m_feedsPanel;
    QWidget *m_categoriesWidget;
    QWidget *m_tabBarWidget;
    QLabel *m_categoriesLabel;
    QPushButton *m_pushButtonNull;
    ToolButton *m_showCategoriesButton;
    ToolButton *m_mainMenuButton;

    QByteArray m_feedsWidgetSplitterState;

    QAction *m_actionAdd;
    QAction *m_actionAddFeed;
    QAction *m_actionAddFolder;
    QAction *m_actionImportFeeds;
    QAction *m_actionExportFeeds;
    QAction *m_actionShowMenuBar;
    QAction *m_actionExit;
    QAction *m_actionMainToolBarToggle;
    QAction *m_actionToolBarLock;
    QAction *m_actionToolBarHide;
    QAction *m_actionCategoriesPanelToggle;
    QAction *m_actionShowDownloadManager;
    QAction *m_actionSetNewsFilters;
    QAction *m_actionOptions;
    QAction *m_actionUpdateApp;
    QAction *m_actionReportProblem;
    QAction *m_actionAbout;

    QAction *m_actionFeedsWidgetVisible;

    NewsTabWidget *m_currentNewsTab;

    Server *m_server;
signals:
    void signalQuitApp();

private slots:
    void showNewsCategoriesTree();
    void feedsSplitterMoved(int pos);
    void slotVisibledFeedsWidget();
    void updateIconToolBarNull(bool isFeedsWidgetVisible);
    void showMenuBar();
    void showMainMenu();
    void lockMainToolBar(bool lock);
    void hideMainToolBar();
    void showContextMenuToolBar(const QPoint &pos);

public slots:
    void slotFeedClicked(QModelIndex index);
    void slotFeedSelected(QModelIndex index, bool createTab);
    void quitApp();
};

#endif // MAINWINDOW_H
