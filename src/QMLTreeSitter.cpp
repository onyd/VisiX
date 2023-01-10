#include "QMLTreeSitter.h"

#include <QDebug>

static const char* ts_qstring_input_read(
    void* _self,
    uint32_t byte,
    TSPoint pt,
    uint32_t* length
) {
    (void)pt;
    QString* self = (QString*)_self;
    if (byte >= self->size()) {
        *length = 0;
        return "";
    }
    else {
        *length = self->size() - byte;
        return (char*)self->data() + byte;
    }
}

QMLTreeSitter::QMLTreeSitter(QObject * parent)
    : QObject(parent)
{
    m_parser = ts_parser_new();
    ts_parser_set_language(m_parser, tree_sitter_qmljs());

}

void QMLTreeSitter::setSource(const QString& source)
{
    m_source = source;
    m_input = TSInput{
        (void*)&m_source,
        ts_qstring_input_read,
        TSInputEncodingUTF16
    };
}

QString ts_node_text(const TSNode& node, const QString& source) {

    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);

    std::string str((char*)source.data() + start, (char*)source.data() + start + end);
    QString string(str.c_str());
    return string;
}

void QMLTreeSitter::update()
{
    m_tree = ts_parser_parse(
        m_parser,
        NULL,
        m_input
    );

    QString text = ts_node_text(ts_tree_root_node(m_tree), m_source);
    qDebug() << text.data();
}


