#include "LeftTabWidget.h"
#include <QTabWidget>
LeftTabWidget::LeftTabWidget(QWidget* parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout(this);
    setLayout(layout);

    // Left tab buttons
    m_button_group = new QButtonGroup(this);
    m_buttons_layout = new QVBoxLayout(this);
    layout->addLayout(m_buttons_layout);

    // Right tab content
    m_tab_content = new QFrame(this);
    m_content_layout = new QVBoxLayout(m_tab_content);
    m_tab_content->setLayout(m_content_layout);
    m_tab_content->setFrameStyle(QFrame::WinPanel | QFrame::Plain);
    layout->addWidget(m_tab_content);
}

QPushButton* LeftTabWidget::button(int index)
{
    return m_tab_buttons[index];
}

QWidget* LeftTabWidget::content(int index)
{
    return m_tabs[index];
}

void LeftTabWidget::addTab(const QString& text, QWidget* content)
{
    int index = m_tabs.size();

    auto button = new QPushButton(text, this);
    button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    m_buttons_layout->addWidget(button);
    m_button_group->addButton(button);

    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    content->setParent(this);
    content->setVisible(false);
    m_tabs.append(content);

    connect(button, &QPushButton::clicked, this, [=]() { switchTab(index); });
}

void LeftTabWidget::switchTab(int index)
{
    if (m_current != nullptr) {
        m_content_layout->removeWidget(m_current);
        m_current->setVisible(false);
    }
    m_current = m_tabs[index];
    m_content_layout->addWidget(m_current);
    m_current->setVisible(true);
}


