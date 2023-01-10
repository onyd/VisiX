#include "QMLTreeSitter.h"

#include <QDebug>
#include <QString>

static const char* ts_qstring_input_read(
    void* payload,
    uint32_t offset,
    TSPoint pt,
    uint32_t* length
) {
    QString* qstr = (QString*)payload;
    if (offset >= qstr->size()) {
        *length = 0;
        return "";
    }
    else {
        *length = qstr->size() - offset;
        return (char*)(qstr->data() + offset);
    }
}

QTreeSitter::QTreeSitter(const TSLanguage* language, QObject * parent)
    : QObject(parent), 
      m_language(language),
      m_parser(ts_parser_new()), 
      m_query_cursor(ts_query_cursor_new()),
      m_input({
        nullptr,
        ts_qstring_input_read,
        TSInputEncodingUTF16
      })
{
    ts_parser_set_language(m_parser, m_language);
}

QTreeSitter::~QTreeSitter()
{
    ts_parser_delete(m_parser);
    if (m_tree != nullptr)
        ts_tree_delete(m_tree);
}


void QTreeSitter::setSource(const QString& source)
{
    m_source = source;
    m_input.payload = (void*)&m_source;
}

const QString& QTreeSitter::source() const
{
    return m_source;
}

void QTreeSitter::setLanguage(const TSLanguage* language)
{
    m_language = language;
}

const TSLanguage* QTreeSitter::language() const
{
    return m_language;
}

void QTreeSitter::edit(const TSInputEdit& edit)
{
    if (m_tree == nullptr) {
        qErrnoWarning("QTreeSitter: Try editing empty syntax tree.");
        return;
    }

    ts_tree_edit(m_tree, &edit);
}

void QTreeSitter::update()
{
    m_tree = ts_parser_parse(
        m_parser,
        m_tree,
        m_input
    );

    auto text = nodeText(root());
    qDebug() << text.toUtf8().data();
}


TSNode QTreeSitter::root()
{
    if (m_tree == nullptr) {
        qErrnoWarning("QTreeSitter: Try getting root of empty syntax tree.");
        return TSNode{ {}, nullptr, nullptr };
    }

    return ts_tree_root_node(m_tree);
}

TSRange QTreeSitter::nodeRange(const TSNode& node)
{
    TSRange range;
    range.start_byte = ts_node_start_byte(node);
    range.end_byte = ts_node_end_byte(node);
    range.start_point = ts_node_start_point(node);
    range.end_point = ts_node_end_point(node);
    return range;
}

QStringView QTreeSitter::nodeText(const TSNode& node)
{
    uint32_t start_byte = ts_node_start_byte(node);
    uint32_t end_byte = ts_node_end_byte(node);
    return QStringView(m_source.data() + start_byte, end_byte - start_byte);
}

QStringView QTreeSitter::text(const TSRange& range)
{
    return QStringView(m_source.data() + range.start_byte, range.end_byte - range.start_byte);
}

QTSQuery QTreeSitter::build(const QString& source) const
{
    return QTSQuery(source, m_language);
}

QTSMatches QTreeSitter::match(const QTSQuery& query)
{
    match(query, root());
}

QTSMatches QTreeSitter::match(const QTSQuery& query, const QTSNode& node)
{
    QTSMatches query_matches;
    query_matches.m_matches.emplaceBack(); 
    ts_query_cursor_exec(m_query_cursor, query.m_query, node.m_node);

    while (ts_query_cursor_next_match(m_query_cursor, query_matches.m_matches.back()))
        query_matches.m_matches.emplaceBack();

    return query_matches;
}

QTreeSitter QTreeSitter::QML(QObject* parent)
{
    return QTreeSitter(tree_sitter_qmljs(), parent);
}
