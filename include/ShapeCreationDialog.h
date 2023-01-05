#ifndef SHAPECREATIONDIALOG_H
#define SHAPECREATIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QButtonGroup>>
#include <QPushButton>
#include <QFrame>
#include <QTreeWidget>

#include "LeftTabWidget.h"

class ShapeCreationDialog : public QDialog
{
    Q_OBJECT

public:
    ShapeCreationDialog(QTreeWidget* scene_root, QTreeWidgetItem* item = nullptr, QWidget* parent = nullptr);

    virtual int exec();
    virtual void open();

public slots:
    void addShape(const QIcon& icon, const QString& name);

private:
    void setup();

    QTreeWidget* m_scene_root;
    QTreeWidgetItem* m_item;
};

#endif // SHAPECREATIONDIALOG_H
