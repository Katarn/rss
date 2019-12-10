#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QPushButton>
#include <QToolButton>

#ifdef Q_OS_MAC
class ToolButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ToolButton(QWidget *parent = nullptr);

    void setIconSize(const QSize &size);

    void setAutoRaise(bool enable);
    bool autoRaise() const;

private:
    bool m_autoRise;
    QSize m_buttonFixedSize;

};
#else
class ToolButton : public QToolButton
{
  Q_OBJECT
public:
  explicit ToolButton(QWidget *parent = nullptr);

};
#endif

#endif // TOOLBUTTON_H
