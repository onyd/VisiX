#include "LeftTabWidget.h"

LeftTabWidget::LeftTabWidget(QWidget* parent)
    : QWidget(parent)
{
    m_tab_content = new QFrame(this);

    m_tab_layout = new QVBoxLayout();
    m_tab_content->setLayout(m_tab_layout);

    m_tab_group = new QButtonGroup(this);
}

 void LeftTabWidget::addTab(const QString& text, QWidget* content)
 {
    m_tab_buttons.append(new QPushButton(text, this));
    m_tabs.append(content);
 }
