#ifndef LEFTTABWIDGET_H
#define LEFTTABWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QButtonGroup>

#include <QVector>
#include <QString>

class LeftTabWidget : public QWidget
{
    Q_OBJECT

public:
    LeftTabWidget(QWidget* parent = nullptr);

    void addTab(const QString& text, QWidget* content);


private:
    QFrame* m_tab_content;
    QVBoxLayout* m_tab_layout;

    QButtonGroup* m_tab_group;

    QVector<QPushButton*> m_tab_buttons;
    QVector<QWidget*> m_tabs;
};

#endif // LEFTTABWIDGET_H
