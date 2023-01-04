#include "LeftTabWidget.h"

LeftTabWidget::LeftTabWidget(QWidget* parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout();
    setLayout(layout);

    // Left tab buttons
    m_button_group = new QButtonGroup(this);
    m_buttons_layout = new QVBoxLayout();
    layout->addLayout(m_buttons_layout);

    // Right tab content
    m_tab_content = new QFrame(this);
    m_content_layout = new QVBoxLayout();
    m_tab_content->setLayout(m_content_layout);
    m_tab_content->setFrameStyle(QFrame::WinPanel | QFrame::Plain);
    layout->addWidget(m_tab_content);
}

void LeftTabWidget::switchTab(int index)
{
    if (m_current != nullptr)
        m_content_layout->removeWidget(m_current);
    m_content_layout->addWidget(m_tabs[index]);
    m_current = m_tabs[index];
}

 void LeftTabWidget::addTab(const QString& text, QWidget* content)
 {
    auto button = new QPushButton(text, this);
    button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_buttons_layout->addWidget(button);
    m_button_group->addButton(button);

    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tabs.append(content);

    connect(button, &QPushButton::clicked, this, [=]() { switchTab(m_tabs.size()-1); });
 }
