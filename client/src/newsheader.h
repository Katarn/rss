#ifndef NEWSHEADER_H
#define NEWSHEADER_H

#include <QHeaderView>
#include <QPushButton>
#include <QMenu>
#include <QActionGroup>

#include "newsmodel.h"

class NewsTabWidget;

class NewsHeader : public QHeaderView
{
    Q_OBJECT

private:
    NewsModel *m_model;
    QPushButton *m_buttonColumnView;
    QActionGroup *m_actionGroupColumnVisible;
    QMenu *m_viewMenu;
    bool m_move;

    void createMenu();
    void adjustAllColumnsWidths(const int newWidth);

public:
    NewsHeader(NewsModel *model, QWidget *parent);

    void init();
    void retranslateStrings();

    QString columnsList();
    void saveStateColumns(NewsTabWidget *newsTabWidget);

private slots:
    void slotButtonColumnView();
    void slotColumnVisible(QAction*);
    void slotSectionMoved(int, int, int);
};

#endif // NEWSHEADER_H
