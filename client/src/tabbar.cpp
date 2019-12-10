#include "tabbar.h"

TabBar::TabBar(QWidget *parent) : QTabBar(parent)
{
    setObjectName("tabBar");
    setFocusPolicy(Qt::NoFocus);
    setDocumentMode(true);
    setMouseTracking(true);
    setExpanding(false);
    setMovable(true);
    setElideMode(Qt::ElideNone);
    setIconSize(QSize(0, 0));
    setContextMenuPolicy(Qt::CustomContextMenu);

    /*
    setStyleSheet(QString("#tabBar_ QToolButton {border: 1px solid %1; border-radius: 2px; background: %2;}").
                  arg(qApp->palette().color(QPalette::Dark).name()).
                  arg(palette().background().color().name()));
   */

    //addTab("");
}
