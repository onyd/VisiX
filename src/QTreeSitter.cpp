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

const char* QTSNode::type() const
{
    return ts_node_type(m_node);
}

bool QTSNode::isNull() const
{
    return ts_node_is_null(m_node);
}

bool QTSNode::isNamed() const
{
    return ts_node_is_named(m_node);
}

bool QTSNode::isMissing() const
{
    return ts_node_is_missing(m_node);
}

bool QTSNode::isExtra() const
{
    return ts_node_is_extra(m_node);
}

bool QTSNode::hasError() const
{
    return ts_node_has_error(m_node);
}

QTSNode QTSNode::parent() const
{
    return QTSNode(ts_node_parent(m_node));
}

QTSNode QTSNode::child(uint32_t index, bool include_anonymous = true) const
{
    if (include_anonymous)
        return QTSNode(ts_node_child(m_node, index));
    return QTSNode(ts_node_named_child(m_node, index));
}

QTSNode QTSNode::child(const std::string& field_name) const
{
    return QTSNode(ts_node_child_by_field_name(m_node, field_name.c_str(), field_name.size()));
}

uint32_t QTSNode::childCount(bool include_anonymous = true) const
{
    if (include_anonymous)
        return ts_node_child_count(m_node);
    return ts_node_named_child_count(m_node);
}

QTSNode QTSNode::previousSibling(bool include_anonymous = true) const
{
    if (include_anonymous)
        return QTSNode(ts_node_prev_sibling(m_node));
    return QTSNode(ts_node_prev_named_sibling(m_node));
}

QTSNode QTSNode::nextSibling(bool include_anonymous = true) const
{
    if (include_anonymous)
        return QTSNode(ts_node_next_sibling(m_node));
    return QTSNode(ts_node_next_named_sibling(m_node));
}

void QTSNode::edit(const QTSInputEdit& edit)
{
    ts_node_edit(&m_node, &edit);
}

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

bool QTSNode::operator==(const QTSNode& node) const
{
    return ts_node_eq(m_node, node.m_node);
}

/***
    QTSTreeCursor
***/
QTSTreeCursor::QTSTreeCursor(const QTSNode& node)
    : m_cursor(ts_tree_cursor_new(node.m_node))
{}

QTSTreeCursor::~QTSTreeCursor()
{
    ts_tree_cursor_delete(&m_cursor);
}

void QTSTreeCursor::reset(const QTSNode& node)
{
    ts_tree_cursor_reset(&m_cursor, node.m_node);
}

QTSNode QTSTreeCursor::current() const
{
    return QTSNode(ts_tree_cursor_current_node(&m_cursor));
}

bool QTSTreeCursor::visitParent()
{
    return ts_tree_cursor_goto_parent(&m_cursor);
}

bool QTSTreeCursor::visitNextSibling()
{
    return ts_tree_cursor_goto_next_sibling(&m_cursor);
}

bool QTSTreeCursor::visitFirstChild()
{
    return ts_tree_cursor_goto_first_child(&m_cursor);
}

/***
    QTSQuery
***/
QTSQuery::QTSQuery(const std::string& source, const TSLanguage* language)
{
    uint32_t error_offset;
    TSQueryError error_type;
    m_query = ts_query_new(language, source.data(), source.size(), &error_offset, &error_type);

    if (m_query == nullptr) 
        throw std::runtime_error("QTSQuery: Failed to build query with source:\n" + source);
}

QTSQuery::~QTSQuery()
{
    ts_query_delete(m_query);
}

/***
    QTSMatch
***/
QTSMatch::QTSMatch()
    : m_match(std::nullopt)
{}

QTSMatch::QTSMatch(const TSQueryMatch& match)
    : m_match(std::make_optional(match))
{}

bool QTSMatch::valid() const
{
    return m_match.has_value();
}

QTSNode QTSMatch::at(qsizetype i) const
{
    return QTSNode(m_match->captures[i].node);
}

qsizetype QTSMatch::size() const
{
    return m_match->capture_count;
}

QTSMatch::iterator QTSMatch::begin() const
{
    return QTSMatch::iterator(&m_match->captures[0]);
}

QTSMatch::iterator QTSMatch::end() const
{
    return QTSMatch::iterator(&m_match->captures[size()]);
}

/***
    QTSMatches
***/
QTSMatches::QTSMatches(const QTSQuery& query, const QTSNode& node)
    : m_query(query),
      m_node(node),
      m_cursor(ts_query_cursor_new())
{
    ts_query_cursor_exec(m_cursor, m_query.m_query, m_node.m_node);
}

QTSMatches::~QTSMatches()
{
    ts_query_cursor_delete(m_cursor);
}


QTSMatches::iterator QTSMatches::begin() const
{
    return iterator(this);
}

QTSMatches::iterator QTSMatches::end() const
{
    return iterator(this);

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

QTSTreeCursor QTreeSitter::rootCursor() const
{
    return QTSTreeCursor(root());
}

QTSQuery QTreeSitter::build(const std::string& source) const
{
    return QTSQuery(source, m_language);
}

QTSMatches QTreeSitter::match(const std::string& query_source) const
{
    return match(query_source, root());
}

QTSMatches QTreeSitter::match(const std::string& query_source, const QTSNode& node) const
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
