#include "ShapeCreationDialog.h"

#include <QVBoxLayout>
#include <QLabel>

ShapeCreationDialog::ShapeCreationDialog(QTreeWidgetItem* item, QWidget* parent)
    : QDialog(parent), m_item(item)
{
    setModal(true);
    resize(1000, 600);
}

int ShapeCreationDialog::exec()
{
    auto shape_creation_tabs = new LeftTabWidget(this);
    auto layout = new QVBoxLayout();
    setLayout(layout);
    layout->addWidget(shape_creation_tabs);

    auto mesh_shape_tab = new QPushButton("Mesh");
    shape_creation_tabs->addTab("Basic shape", mesh_shape_tab);
    connect(mesh_shape_tab, &QPushButton::clicked, this, [=]() { addShape("Test"); });
    
    auto sdf_shape_tab = new QPushButton("SDF");
    shape_creation_tabs->addTab("SDF shape", sdf_shape_tab);

    auto voxel_shape_tab = new QPushButton("Voxel");
    shape_creation_tabs->addTab("Voxel shape", voxel_shape_tab);

    shape_creation_tabs->switchTab(0);

    return QDialog::exec();
}

void ShapeCreationDialog::open()
{
    auto shape_creation_tabs = new LeftTabWidget(this);
    auto layout = new QVBoxLayout();
    setLayout(layout);
    layout->addWidget(shape_creation_tabs);

    auto mesh_shape_tab = new QLabel("Mesh");
    shape_creation_tabs->addTab("Basic shape", mesh_shape_tab);

    auto sdf_shape_tab = new QLabel("SDF");
    shape_creation_tabs->addTab("SDF shape", sdf_shape_tab);

    auto voxel_shape_tab = new QLabel("Voxel");
    shape_creation_tabs->addTab("Voxel shape", voxel_shape_tab);

    shape_creation_tabs->switchTab(0);

    QDialog::open();
}

void ShapeCreationDialog::addShape(const QString& name)
{
    m_item->addChild(new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr), QStringList(name)));
}