#include "ShapeCreationDialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>

ShapeCreationDialog::ShapeCreationDialog(QTreeWidget* scene_root, QTreeWidgetItem* item, QWidget* parent)
    : QDialog(parent), m_scene_root(scene_root), m_item(item)
{
    setModal(true);
    resize(1000, 600);
}

int ShapeCreationDialog::exec()
{
    setup();
    return QDialog::exec();
}

void ShapeCreationDialog::open()
{
    setup();
    QDialog::open();
}

void ShapeCreationDialog::addShape(const QString& name)
{
    auto new_shape = new QTreeWidgetItem(static_cast<QTreeWidget*>(nullptr), QStringList(name));
    if (m_item != nullptr)
        m_item->addChild(new_shape);
    else 
        m_scene_root->addTopLevelItem(new_shape);
}

void ShapeCreationDialog::setup()
{
    auto layout = new QVBoxLayout();
    setLayout(layout);

    auto shape_creation_tabs = new LeftTabWidget(this);
    shape_creation_tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto shape_type_selection_layout = new QVBoxLayout();
    shape_type_selection_layout->addWidget(shape_creation_tabs);
    layout->addLayout(shape_type_selection_layout);

    auto name_label = new QLabel("name:");
    name_label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    auto name_field = new QLineEdit();
    name_field->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    auto name_field_layout = new QHBoxLayout();
    name_field_layout->addWidget(name_label);
    name_field_layout->addWidget(name_field);
    layout->addLayout(name_field_layout);

    // Mesh creation tab
    auto mesh_shape_tab = new QScrollArea();
    auto mesh_shape_layout = new QVBoxLayout(mesh_shape_tab);
    
    QList<QPushButton*> mesh_shape_buttons;
    mesh_shape_buttons.append(new QPushButton("Ball"));
    mesh_shape_buttons.append(new QPushButton("Cube"));
    mesh_shape_buttons.append(new QPushButton("Cuboid"));

    for (auto button : mesh_shape_buttons) {
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mesh_shape_layout->addWidget(button);
        connect(button, &QPushButton::clicked, this, [=]() { addShape(name_field->text()); });
    }
    shape_creation_tabs->addTab("Basic shape", mesh_shape_tab);

    // Signed Distance Field creation tab
    auto sdf_shape_tab = new QPushButton("SDF");
    shape_creation_tabs->addTab("SDF shape", sdf_shape_tab);

    // Voxel-based creation based
    auto voxel_shape_tab = new QPushButton("Voxel");
    shape_creation_tabs->addTab("Voxel shape", voxel_shape_tab);

    shape_creation_tabs->switchTab(0);
}