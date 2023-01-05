#pragma once

// Qt
#include <QMainWindow>
#include <QQuickWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>

#include "SceneExplorer.h"

class QSyntaxStyle;
class QCompleter;
class QStyleSyntaxHighlighter;
class QCodeEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget* parent = nullptr);

private:

    void setStyle(QString path);

    QString loadCode(QString path);

    // Scene explorer
    SceneExplorer* m_scene_explorer;

    // Shader editor
    QCodeEditor* m_shader_editor;
    QSyntaxStyle* m_style;

    // 3D view
    QQuickWidget* m_scene_view;
};

