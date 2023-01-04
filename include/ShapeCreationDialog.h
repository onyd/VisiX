#ifndef SHAPECREATIONDIALOG_H
#define SHAPECREATIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QButtonGroup>>
#include <QPushButton>
#include <QFrame>

#include "LeftTabWidget.h"

class ShapeCreationDialog : public QDialog
{
    Q_OBJECT

public:
    ShapeCreationDialog(QWidget* parent = nullptr);



private:
    LeftTabWidget* m_shape_creation_tabs;

};

#endif // SHAPECREATIONDIALOG_H
