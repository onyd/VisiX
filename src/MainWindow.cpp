#include <MainWindow.hpp>

// QCodeEditor
#include <QCodeEditor>
#include <QGLSLCompleter>
#include <QSyntaxStyle>
#include <QGLSLHighlighter>

// Qt
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_scene_explorer(nullptr),
    m_shader_editor(nullptr),
    m_style(nullptr)
{
    auto container = new QWidget(this);
    setCentralWidget(container);
    resize(2000, 1200);

    // Main view 
    auto main_hbox = new QHBoxLayout(container);

    m_scene_explorer = new SceneExplorer(this);
    m_scene_explorer->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    main_hbox->addWidget(m_scene_explorer);

    auto editor_vbox = new QVBoxLayout(this);
    main_hbox->addLayout(editor_vbox);

    m_shader_editor = new QCodeEditor(this);
    m_shader_editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    editor_vbox->addWidget(m_shader_editor);

    // Default parameters
    setWindowTitle("VisiX");
    setStyle(":/styles/drakula.xml");
    m_shader_editor->setPlainText(loadCode(":/code_samples/shader.glsl"));
    m_shader_editor->setSyntaxStyle(m_style);
    m_shader_editor->setCompleter(new QGLSLCompleter(this));
    m_shader_editor->setHighlighter(new QGLSLHighlighter);

}

QString MainWindow::loadCode(QString path)
{
    QFile fl(path);

    if (!fl.open(QIODevice::ReadOnly))
    {
        return QString();
    }

    return fl.readAll();
}

void MainWindow::setStyle(QString path)
{
    QFile fl(path);

    if (!fl.open(QIODevice::ReadOnly))
    {
        return;
    }

    auto style = new QSyntaxStyle(this);

    if (!style->load(fl.readAll()))
    {
        delete style;
        return;
    }

    m_style = style;
}

