#include "QTreeSitter.h"

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

/***
    QTSNode
***/
QTSNode::QTSNode(const TSNode& node)
    : m_node(node)
{}

uint32_t QTSNode::start() const
{
    return ts_node_start_byte(m_node);
}

uint32_t QTSNode::end() const
{
    return ts_node_end_byte(m_node);
}

TSPoint QTSNode::startPos() const
{
    return ts_node_start_point(m_node);
}

TSPoint QTSNode::endPos() const
{
    return ts_node_end_point(m_node);
}

QTSRange QTSNode::range() const
{
    QTSRange range;
    range.start_byte = start();
    range.end_byte = end();
    range.start_point = startPos();
    range.end_point = endPos();
    return range;
}

/***
    QTSQuery
***/
QTSQuery::QTSQuery(const QString& source, const TSLanguage* language)
{
    auto source_bytes = source.toUtf8();
    uint32_t error_offset;
    TSQueryError error_type;
    m_query = ts_query_new(language, source_bytes.data(), source_bytes.size(), &error_offset, &error_type);

    if (m_query == nullptr) {
        std::string message;
        message += "QTSQuery: Failed to build query with source:\n";
        message += source_bytes.data();
        throw std::runtime_error(message);
    }
}

QTSQuery::~QTSQuery()
{
    ts_query_delete(m_query);
}

/***
    QTSMatches
***/

/***
    QTreeSitter
***/
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

    auto d = text(root());
    qDebug() << d.toUtf8().data();
}


QTSNode QTreeSitter::root() const
{
    if (m_tree == nullptr) {
        qErrnoWarning("QTreeSitter: Try getting root of empty syntax tree.");
        return TSNode{ {}, nullptr, nullptr };
    }

    return ts_tree_root_node(m_tree);
}



QStringView QTreeSitter::text(const QTSNode& node) const
{
    uint32_t start_byte = node.start();
    uint32_t end_byte = node.end();
    return QStringView(m_source.data() + start_byte, end_byte - start_byte);
}

QStringView QTreeSitter::text(const TSRange& range) const
{
    return QStringView(m_source.data() + range.start_byte, range.end_byte - range.start_byte);
}

QTSQuery QTreeSitter::build(const QString& source) const
{
    return QTSQuery(source, m_language);
}

QTSMatches QTreeSitter::match(const QString& query_source) const
{
    match(query_source, root());
}

QTSMatches QTreeSitter::match(const QString& query_source, const QTSNode& node) const
{
    QTSQuery query = build(query_source);
    ts_query_cursor_exec(m_query_cursor, query.m_query, node.m_node);

    QTSMatches query_matches;
    TSQueryMatch match;
    while (ts_query_cursor_next_match(m_query_cursor, &match))
        query_matches.m_matches.emplaceBack(match);

    return query_matches;
}

QTSMatches QTreeSitter::match(const QTSQuery& query) const
{
    match(query, root());
}

QTSMatches QTreeSitter::match(const QTSQuery& query, const QTSNode& node) const
{
    ts_query_cursor_exec(m_query_cursor, query.m_query, node.m_node);
    
    QTSMatches query_matches;
    TSQueryMatch match;
    while (ts_query_cursor_next_match(m_query_cursor, &match))
        query_matches.m_matches.emplaceBack(match);

    return query_matches;
}

QTreeSitter QTreeSitter::QML(QObject* parent)
{
    return QTreeSitter(tree_sitter_qmljs(), parent);
}
