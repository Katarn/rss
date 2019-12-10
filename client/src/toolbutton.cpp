#include "toolbutton.h"

#ifdef Q_OS_MAC
ToolButton::ToolButton(QWidget *parent)
  : QPushButton(parent)
  , m_autoRise(false)
  , m_buttonFixedSize(18, 18)
{
}

// =====================================================================================================================

void ToolButton::setIconSize(const QSize &size)
{
    QPushButton::setIconSize(size);
    m_buttonFixedSize = QSize(size.width() + 2, size.height() + 2);
}

// =====================================================================================================================

void ToolButton::setAutoRaise(bool enable)
{
    m_autoRise = enable;

    setFlat(enable);

    if (enable) {
        setFixedSize(m_buttonFixedSize);
    }
}

// =====================================================================================================================

bool ToolButton::autoRaise() const
{
    return m_autoRise;
}
#else
ToolButton::ToolButton(QWidget *parent)
  : QToolButton(parent)
{
}
#endif
