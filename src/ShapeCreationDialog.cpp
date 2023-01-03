#include "ShapeCreationDialog.h"

#include <QVBoxLayout>
#include <QLabel>

ShapeCreationDialog::ShapeCreationDialog(QWidget* parent)
    : QDialog(parent)
{
    setModal(true);

    m_shape_tab_content = new QWidget(this);
    auto layout = new QHBoxLayout();
    m_shape_tab_content->setLayout(layout);
    auto label = new QLabel("Not yet implemented");
    layout->addWidget(label);

    m_tab_content = new QFrame(this);
    m_shape_tab_button = new QPushButton("&New Shape", this);
    m_tab_group = new QButtonGroup(this);

    m_tab_layout = new QVBoxLayout();
    m_tab_content->setLayout(m_tab_layout);
    m_tab_layout->addWidget(m_shape_tab_content);
}
