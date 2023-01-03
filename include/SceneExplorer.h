#ifndef SCENEEXPLORER_H
#define SCENEEXPLORER_H

#include <QWidget>
#include <QTreeWidget>

class SceneExplorer : public QWidget
{
    Q_OBJECT

public:
    SceneExplorer(QWidget* parent = nullptr);

//public slots:
//    void addShape();
//    void removeShape();

private:
    void openActionsMenu(const QPoint& pos);

    QTreeWidget* m_scene_root;
};

#endif // SCENEEXPLORER_H
