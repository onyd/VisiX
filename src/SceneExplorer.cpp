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

    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < 10; ++i)
        items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("item: %1").arg(i))));
    m_scene_root->insertTopLevelItems(0, items);

    layout->addWidget(m_scene_root);
}

void SceneExplorer::openActionsMenu(const QPoint& pos)
{
    // Actions
    QAction *new_shape_action = new QAction(QIcon("/resources/icons/add.png"), tr("&Add shape"), this);
    new_shape_action->setStatusTip(tr("Open shape creation window."));

    QAction *remove_shape_action = new QAction(QIcon("/resources/icons/remove.png"), tr("&Remove"), this);
    remove_shape_action->setStatusTip(tr("Remove selected hierarchy."));

    // Connections
    auto clicked = m_scene_root->itemAt(pos);
    auto shape_dialog = new ShapeCreationDialog(clicked);
    connect(new_shape_action, &QAction::triggered, shape_dialog, &ShapeCreationDialog::exec);
    // TODO connect to remove

    // Open menu on right click
    QMenu menu(this);
    menu.addAction(new_shape_action);
    menu.addAction(remove_shape_action);
    menu.exec(m_scene_root->mapToGlobal(pos));
}
