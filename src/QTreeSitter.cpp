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

bool operator==(const TSQueryMatch& a, const TSQueryMatch& b)
{
    return a.id == b.id && a.pattern_index == b.pattern_index;
}

/***
    QTSNode
***/
QTSNode::QTSNode()
    : m_node({ {}, nullptr, nullptr }), m_owner(nullptr)
{}

QTSNode::QTSNode(const TSNode& node, const QTreeSitter* owner)
    : m_node(node), m_owner(owner)
{}

const char* QTSNode::type() const
{
    return ts_node_type(m_node);
}

QStringView QTSNode::text() const
{
    return m_owner->text(*this);
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

const QTreeSitter* QTSNode::owner() const
{
    return m_owner;
}

QTSNode QTSNode::parent() const
{
    return QTSNode(ts_node_parent(m_node), m_owner);
}

QTSNode QTSNode::child(uint32_t index, bool include_anonymous) const
{
    if (include_anonymous)
        return QTSNode(ts_node_child(m_node, index), m_owner);
    return QTSNode(ts_node_named_child(m_node, index), m_owner);
}

QTSNode QTSNode::child(const std::string& field_name) const
{
    return QTSNode(ts_node_child_by_field_name(m_node, field_name.c_str(), (uint32_t)field_name.size()), m_owner);
}

uint32_t QTSNode::childCount(bool include_anonymous) const
{
    if (include_anonymous)
        return ts_node_child_count(m_node);
    return ts_node_named_child_count(m_node);
}

QTSNode QTSNode::previousSibling(bool include_anonymous) const
{
    if (include_anonymous)
        return QTSNode(ts_node_prev_sibling(m_node), m_owner);
    return QTSNode(ts_node_prev_named_sibling(m_node), m_owner);
}

QTSNode QTSNode::nextSibling(bool include_anonymous) const
{
    if (include_anonymous)
        return QTSNode(ts_node_next_sibling(m_node), m_owner);
    return QTSNode(ts_node_next_named_sibling(m_node), m_owner);
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
    : m_cursor(ts_tree_cursor_new(node.m_node)), m_owner(node.m_owner)
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
    return QTSNode(ts_tree_cursor_current_node(&m_cursor), m_owner);
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
    m_query = std::shared_ptr<TSQuery>(ts_query_new(language, source.data(), (uint32_t)source.size(), &error_offset, &error_type),
                                       [](TSQuery* query) {ts_query_delete(query); });

    if (m_query == nullptr) 
        throw std::runtime_error("QTSQuery: Failed to build query with source:\n" + source);
}

/***
    QTSMatch
***/
QTSMatch::QTSMatch()
    : m_match(std::nullopt), m_owner(nullptr)
{}

QTSMatch::QTSMatch(nullptr_t ptr)
    : QTSMatch()
{}
 
QTSMatch::QTSMatch(const TSQueryMatch& match, const QTreeSitter* owner)
    : m_match(std::make_optional(match)), m_owner(owner)
{}

const QTreeSitter* QTSMatch::owner() const
{
    return m_owner;
}

bool QTSMatch::valid() const
{
    return m_match.has_value();
}

QTSNode QTSMatch::at(uint32_t i) const
{
    return QTSNode(m_match->captures[i].node, m_owner);
}

uint32_t QTSMatch::size() const
{
    return m_match->capture_count;
}

QTSMatch::iterator QTSMatch::begin() const
{
    return QTSMatch::iterator(&m_match->captures[0], m_owner);
}

QTSMatch::iterator QTSMatch::end() const
{
    return QTSMatch::iterator(&m_match->captures[size()], m_owner);
}

/***
    QTSQueryCursor
***/
QTSQueryCursor::QTSQueryCursor(const QTSQuery& query, const QTSNode& node, const QTreeSitter* owner) : m_owner(owner)
{
    m_cursor = std::shared_ptr<TSQueryCursor>(ts_query_cursor_new(), [](TSQueryCursor* cursor) { ts_query_cursor_delete(cursor); });
    ts_query_cursor_exec(m_cursor.get(), query.m_query.get(), node.m_node);
}

QTSQueryCursor::QTSQueryCursor(nullptr_t)
{
    m_cursor = nullptr;
}

QTSMatch QTSQueryCursor::next()
{
    TSQueryMatch next_match;
    if (ts_query_cursor_next_match(m_cursor.get(), &next_match)) 
        return QTSMatch(next_match, m_owner); 
    
    m_cursor = nullptr;
    return nullptr; // No more match so return null match
} 

bool operator==(const QTSQueryCursor& a, const QTSQueryCursor& b)
{
    return a.m_cursor == b.m_cursor;
}

bool operator!=(const QTSQueryCursor& a, const QTSQueryCursor& b)
{
    return !(a == b);
}

/***
    QTSMatches
***/
QTSMatches::QTSMatches(const QTSQuery& query, const QTSNode& node)
    : m_query(query),
      m_node(node)
{}

QTSMatches::iterator QTSMatches::begin() const
{
    return QTSMatches::iterator(m_node.owner(), QTSQueryCursor(m_query, m_node, m_node.owner()));
}

QTSMatches::iterator QTSMatches::end() const
{
    return QTSMatches::iterator(nullptr);
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

    return QTSNode(ts_tree_root_node(m_tree), this);
}

QStringView QTreeSitter::text(const QTSNode& node) const
{
    uint32_t start_byte = node.start();
    uint32_t end_byte = node.end();
    return QStringView(m_source.data() + start_byte, end_byte - start_byte);
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
