#include "ShapeCreationDialog.h"

#include <QVBoxLayout>
#include <QLabel>

ShapeCreationDialog::ShapeCreationDialog(QWidget* parent)
    : QDialog(parent)
{
    setModal(true);
    resize(1000, 600);

    m_shape_creation_tabs = new LeftTabWidget(this);
    auto layout = new QVBoxLayout();
    setLayout(layout);
    layout->addWidget(m_shape_creation_tabs);

    auto mesh_shape_tab = new QLabel("Mesh");
    m_shape_creation_tabs->addTab("Basic shape", mesh_shape_tab);
    m_shape_creation_tabs->switchTab(0);

    auto sdf_shape_tab = new QLabel("SDF");
    m_shape_creation_tabs->addTab("SDF shape", sdf_shape_tab);

    auto voxel_shape_tab = new QLabel("Voxel");
    m_shape_creation_tabs->addTab("Voxel shape", voxel_shape_tab);
}
