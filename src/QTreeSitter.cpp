#include "QTreeSitter.h"

#include <QDebug>
#include <QString>

static const char* ts_qstring_input_read(
    void* payload,
    uint32_t byte_offset,
    TSPoint pt,
    uint32_t* length
) {
    QString* qstr = (QString*)payload;
    if (byte_offset >= 2 * qstr->size()) {
        *length = 0;
        return "";
    }
    else {
        *length = 2 * (qstr->size() - byte_offset);
        return (char*)qstr->data() + byte_offset;
    }
}

/***
    QTSNode
***/
QTSNode::QTSNode()
    : m_node({ {}, nullptr, nullptr })
{}

QTSNode::QTSNode(const TSNode& node)
    : m_node(node)
{}

uint32_t QTSNode::start() const
{
    return ts_node_start_byte(m_node) / 2;
}

uint32_t QTSNode::end() const
{
    return ts_node_end_byte(m_node) / 2;
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
    QTSMatch
***/
QTSMatch::QTSMatch(const TSQueryMatch& match)
    : m_captures(match.capture_count)
{
    for (int i = 0; i < match.capture_count; i++)
        m_captures[i] = QTSNode(match.captures[i].node);
}

QTSNode QTSMatch::at(qsizetype i) const
{
    return m_captures[i];
}

qsizetype QTSMatch::size() const
{
    return m_captures.size();
}

QTSMatch::QTSCaptures::iterator QTSMatch::begin()
{
    return m_captures.begin();
}

QTSMatch::QTSCaptures::iterator QTSMatch::end()
{
    return m_captures.end();
}

QTSMatch::QTSCaptures::ConstIterator QTSMatch::cbegin() const
{
    return m_captures.cbegin();
}

QTSMatch::QTSCaptures::ConstIterator QTSMatch::cend() const
{
    return m_captures.cend();
}

/***
    QTSMatches
***/
QTSMatches::QTSMatches(const QTSQuery& query, const QTSNode& node)
    : m_query(query),
      m_node(node),
      m_match({}),
      m_cursor(ts_query_cursor_new())
{
    ts_query_cursor_exec(m_cursor, m_query.m_query, m_node.m_node);
}

QTSMatches::~QTSMatches()
{
    ts_query_cursor_delete(m_cursor);
}

bool QTSMatches::next()
{
    return ts_query_cursor_next_match(m_cursor, &m_match);
}

QTSMatch QTSMatches::current() const
{
    return QTSMatch(m_match);
}

/***
    QTreeSitter
***/
QTreeSitter::QTreeSitter(const TSLanguage* language, QObject * parent)
    : QObject(parent), 
      m_language(language),
      m_parser(ts_parser_new()), 
      m_source(""),
      m_input({
        &m_source,
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
}

QTSNode QTreeSitter::root() const
{
    if (m_tree == nullptr) {
        qErrnoWarning("QTreeSitter: Try getting root of empty syntax tree.");
        return QTSNode();
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
    return match(query_source, root());
}

QTSMatches QTreeSitter::match(const QString& query_source, const QTSNode& node) const
{
    QTSQuery query = build(query_source);
    return QTSMatches(query, node);
}

QTSMatches QTreeSitter::match(const QTSQuery& query) const
{
    return match(query, root());
}

QTSMatches QTreeSitter::match(const QTSQuery& query, const QTSNode& node) const
{
    return QTSMatches(query, node);
}

QTreeSitter QTreeSitter::QML(QObject* parent)
{
    return QTreeSitter(tree_sitter_qmljs(), parent);
}

const char* QTreeSitter::print() const
{
    return ts_node_string(root().m_node);
}
