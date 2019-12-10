#ifndef CATEGORIESTREEWIDGET_H
#define CATEGORIESTREEWIDGET_H

#include <QTreeWidget>
#include <QHeaderView>

class CategoriesTreeWidget : public QTreeWidget
{
public:
    explicit CategoriesTreeWidget(QWidget *parent = nullptr);

    enum Items {
        UnreadItem,
        StarredItem,
        DeletedItem,
        LabelsItem
    };
};

#endif // CATEGORIESTREEWIDGET_H
