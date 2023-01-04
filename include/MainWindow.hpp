#pragma once

// Qt
#include <QMainWindow>
#include <QString>

#include "SceneExplorer.h"

class QSyntaxStyle;
class QCompleter;
class QStyleSyntaxHighlighter;
class QCodeEditor;

/**
 * @brief Class, that describes demo main window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief Constructor.
     * @param parent Pointer to parent widget.
     */
    explicit MainWindow(QWidget* parent=nullptr);

private:

    void setStyle(QString path);

    QString loadCode(QString path);

    // Scene explorer
    SceneExplorer* m_scene_explorer;

    // Shader editor
    QCodeEditor* m_shader_editor;
    QSyntaxStyle* m_style;
};

