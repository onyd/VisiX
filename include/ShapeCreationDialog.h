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
    ShapeCreationDialog(QTreeWidgetItem* item, QWidget* parent = nullptr);

    virtual int exec();
    virtual void open();

public slots:
    void addShape(const QString& name);

private:
    QTreeWidgetItem* m_item;
};

#endif // SHAPECREATIONDIALOG_H
