#include <MainWindow.hpp>

// QCodeEditor
#include <QCodeEditor>
#include <QGLSLCompleter>
#include <QSyntaxStyle>
#include <QGLSLHighlighter>

// Qt
#include <QFile>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent)
{
    auto container = new QWidget(this);
    setCentralWidget(container);
    resize(2000, 1200);

    // Main view 
    auto window_layout = new QHBoxLayout(container);

    // Scene explorer
    m_scene_explorer = new SceneExplorer(this);
    m_scene_explorer->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    window_layout->addWidget(m_scene_explorer);

    auto main_layout = new QHBoxLayout(this);
    window_layout->addLayout(main_layout);

    m_shader_editor = new QCodeEditor(this);
    m_shader_editor->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    main_layout->addWidget(m_shader_editor);

    auto view_edit_layout = new QVBoxLayout();
    main_layout->addLayout(view_edit_layout);

    // Scene view
    m_scene_view = new QQuickWidget(this);
    m_scene_view->setWindowTitle("Scene view");
    m_scene_view->setResizeMode(QQuickWidget::SizeRootObjectToView);

    auto url = QUrl::fromLocalFile(":/qml/scene.qml");
    m_scene_view->setSource(url);
    view_edit_layout->addWidget(m_scene_view);

    auto edit_view = new QWidget(this);
    view_edit_layout->addWidget(edit_view);

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
        return QString();

    return fl.readAll();
}

void MainWindow::setStyle(QString path)
{
    QFile fl(path);

    if (!fl.open(QIODevice::ReadOnly))
        return;

    auto style = new QSyntaxStyle(this);

    if (!style->load(fl.readAll()))
    {
        delete style;
        return;
    }

    m_style = style;
}

