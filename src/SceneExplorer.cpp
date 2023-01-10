#include "SceneExplorer.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QAction>

#include "ShapeCreationDialog.h"

SceneExplorer::SceneExplorer(QWidget* parent)
    : QWidget(parent),
      m_scene_root(nullptr)
{
    auto layout = new QVBoxLayout(this);

    m_scene_root = new QTreeWidget();
    m_scene_root->setContextMenuPolicy(Qt::CustomContextMenu);
    m_scene_root->setColumnCount(1);
    m_scene_root->setHeaderLabel("Scene explorer");
    connect(m_scene_root, &QTreeWidget::customContextMenuRequested, this, [this](const QPoint& pos) { openActionsMenu(pos); });

    layout->addWidget(m_scene_root);
}

void SceneExplorer::openActionsMenu(const QPoint& pos)
{
    auto clicked = m_scene_root->itemAt(pos);

    // Actions
    QAction *new_shape_action = new QAction(QIcon(":/icons/shapes.png"), tr("&Add shape"), this);
    new_shape_action->setStatusTip(tr("Open shape creation window."));

    QAction *remove_shape_action = new QAction(QIcon(":/icons/trash.png"), tr("&Remove"), this);
    remove_shape_action->setStatusTip(tr("Remove selected object hierarchy."));

    // Connections
    auto shape_dialog = new ShapeCreationDialog(m_scene_root, clicked, this);
    connect(new_shape_action, &QAction::triggered, shape_dialog, &ShapeCreationDialog::exec);
    // TODO connect to remove

    // Open menu on right click
    QMenu menu(this);
    menu.addAction(new_shape_action);
    menu.addAction(remove_shape_action);
    menu.exec(m_scene_root->mapToGlobal(pos));
}
