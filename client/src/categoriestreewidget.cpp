#include "categoriestreewidget.h"

CategoriesTreeWidget::CategoriesTreeWidget(QWidget * parent) : QTreeWidget(parent)
{
    //setObjectName("newsCategoriesTree");
    setFrameStyle(QFrame::NoFrame);
    setContextMenuPolicy(Qt::CustomContextMenu);
    //setStyleSheet(
    //      QString("#newsCategoriesTree { border-top: 1px solid %1 }")
    //      .arg(qApp->palette().color(QPalette::Dark).name()));

    setColumnCount(5);
    setColumnHidden(1, true);
    setColumnHidden(2, true);
    setColumnHidden(3, true);

    header()->hide();
    header()->setSectionResizeMode(0, QHeaderView::Stretch);
    header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    header()->setStretchLastSection(false);
}
