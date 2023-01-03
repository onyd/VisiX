#include "ShapeCreationDialog.h"

#include <QVBoxLayout>
#include <QLabel>

ShapeCreationDialog::ShapeCreationDialog(QWidget* parent)
    : QDialog(parent)
{
    setModal(true);
    auto layout = new QHBoxLayout(this);
    auto label = new QLabel("Not yet implemented");
    layout->addWidget(label);
}
