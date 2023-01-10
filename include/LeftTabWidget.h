#pragma once

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

    QPushButton* button(int index);
    QWidget* content(int index);

public slots:
    void switchTab(int index);

private:
    QFrame* m_tab_content;
    QVBoxLayout* m_content_layout;
    QWidget* m_current = nullptr;

    QVBoxLayout* m_buttons_layout;
    QButtonGroup* m_button_group;

    QVector<QPushButton*> m_tab_buttons;
    QVector<QWidget*> m_tabs;
};
