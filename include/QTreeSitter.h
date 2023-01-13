#pragma once

#include <tree_sitter/api.h>
#include <memory>
#include <optional>
#include <functional>
#include <QFile>
#include <QString>
#include <QVector>

extern "C" {
    const TSLanguage* tree_sitter_qmljs();
}

bool operator==(const TSQueryMatch& a, const TSQueryMatch& b);

// Wrapper
typedef TSRange QTSRange;
typedef TSPoint QTSPoint;
typedef TSQueryCapture QTSCapture;
typedef TSLanguage QTSLanguage;
typedef TSInputEdit QTSInputEdit;

class QTreeSitter;
class QTSQueryCursor;
class QTSTreeCursor;
class QTSMatches;

class QTSNode
{
public:
    QTSNode();
    QTSNode(const TSNode& node, const QTreeSitter* owner);

    const char* type() const;
    QStringView text() const;

    bool isNull() const;
    bool isNamed() const;
    bool isMissing() const;
    bool isExtra() const;
    bool hasError() const;

    const QTreeSitter* owner() const;

    QTSNode parent() const;

    QTSNode child(uint32_t index, bool include_anonymous = true) const;
    QTSNode child(const std::string& field_name) const;
    uint32_t childCount(bool include_anonymous = true) const;

    QTSNode previousSibling(bool include_anonymous = true) const;
    QTSNode nextSibling(bool include_anonymous = true) const;

    void edit(const QTSInputEdit& edit);

    uint32_t start() const;
    uint32_t end() const;

    QTSPoint startPos() const;
    QTSPoint endPos() const;

    QTSRange range() const;

    bool operator==(const QTSNode& node) const;

private:
    friend class QTreeSitter;
    friend class QTSQueryCursor;
    friend class QTSTreeCursor;
    
    const QTreeSitter* m_owner;
    TSNode m_node;
};

class QTSTreeCursor
{
public:
    QTSTreeCursor(const QTSNode& node);
    ~QTSTreeCursor();

    void reset(const QTSNode& node);
    QTSNode current() const;

    bool visitParent();
    bool visitNextSibling();
    bool visitFirstChild();

private:
    const QTreeSitter* m_owner;
    TSTreeCursor m_cursor;
};

class QTSQuery
{
public:
    QTSQuery(const std::string& source, const TSLanguage* language);

private:
    friend class QTSQueryCursor;

    std::shared_ptr<TSQuery> m_query;
};

class CursorIterator;

class QTSMatch
{
public:
    // Iterator over the captures
    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        using value_type = QTSNode;
        using pointer_type = const QTSCapture*;

        iterator(pointer_type ptr, const QTreeSitter* owner) : m_ptr(ptr), m_owner(owner) {}

        value_type operator*() const { return QTSNode(m_ptr->node, m_owner); }

        // Prefix increment
        iterator& operator++() { m_ptr++; return *this; }

        // Postfix increment
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        const QTreeSitter* m_owner;
        pointer_type m_ptr;
    };

    // QTSMatch 
    QTSMatch();
    QTSMatch(nullptr_t);
    QTSMatch(const TSQueryMatch& match, const QTreeSitter* owner);

    const QTreeSitter* owner() const;
    bool valid() const;
    QTSNode at(uint32_t i) const;
    uint32_t size() const;

    iterator begin() const;
    iterator end() const;

    friend bool operator==(const QTSMatch& a, const QTSMatch& b) { return (a.m_owner == b.m_owner && 
                                                                          (a.m_match == b.m_match)); };
    friend bool operator!=(const QTSMatch& a, const QTSMatch& b) { return !(a == b); };

private:
    friend class QTSQueryCursor;

    const QTreeSitter* m_owner;
    std::optional<TSQueryMatch> m_match;
};

class QTSQueryCursor
{
public:
    QTSQueryCursor(const QTSQuery& query, const QTSNode& node, const QTreeSitter* owner);
    QTSQueryCursor(nullptr_t);

    QTSMatch next();

    friend bool operator==(const QTSQueryCursor& a, const QTSQueryCursor& b);
    friend bool operator!=(const QTSQueryCursor& a, const QTSQueryCursor& b);

private:
    const QTreeSitter* m_owner;
    std::shared_ptr<TSQueryCursor> m_cursor;
};

class CursorIterator
{
public:
    using iterator_category = std::input_iterator_tag;

    using value_type = QTSMatch;
    using reference_type = value_type&;
    using creference_type = const value_type&;

    CursorIterator(const QTreeSitter* owner, QTSQueryCursor cursor = nullptr) : m_cursor(cursor)
    {
        if (cursor != nullptr)  // Initialize to first value if cursor
            m_qmatch = m_cursor.next();
    }

    creference_type operator*() const { return m_qmatch; }

    // Prefix increment
    CursorIterator& operator++()
    {
        if (!m_qmatch.valid())
            return *this; // Do nothing if we're at the end
        m_qmatch = m_cursor.next();

        return *this;
    }

    // Postfix increment
    CursorIterator operator++(int) { CursorIterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const CursorIterator& a, const CursorIterator& b) { return (a.m_cursor == b.m_cursor && a.m_qmatch == b.m_qmatch); };
    friend bool operator!= (const CursorIterator& a, const CursorIterator& b) { return !(a == b); };

private:
    QTSQueryCursor m_cursor;
    QTSMatch m_qmatch;
};

class QTSMatches
{
public:
    using iterator = CursorIterator;

    QTSMatches(const QTSQuery& query, const QTSNode& node);

    iterator begin() const;
    iterator end() const;

private:
    QTSQuery m_query;
    QTSNode m_node;
};

class QTreeSitter : public QObject
{
public:
    QTreeSitter(const QTSLanguage* language, QObject* parent = nullptr);
    ~QTreeSitter();

    QTreeSitter(const QTreeSitter& parser) = delete;
    QTreeSitter(const QTreeSitter&& parser) = delete;
    QTreeSitter& operator&(const QTreeSitter& parser) = delete;
    QTreeSitter& operator&&(const QTreeSitter& parser) = delete;

    void setSource(const QString& source);
    const QString& source() const;

    void setLanguage(const QTSLanguage* language);
    const QTSLanguage* language() const;

    void edit(const QTSInputEdit& edit);
    void update();

    QTSNode root() const;

    QStringView text(const QTSNode& node) const;

    QTSTreeCursor rootCursor() const;
    QTSQuery build(const std::string& source) const;

    QTSMatches match(const std::string& query_source) const;
    QTSMatches match(const std::string& query_source, const QTSNode& node) const;
    QTSMatches match(const QTSQuery& query) const;
    QTSMatches match(const QTSQuery& query, const QTSNode& node) const;

    static QTreeSitter QML(QObject* parent = nullptr);

    // Debug
    const char* print() const;
private:
    QString m_source;
    TSInput m_input;

    const TSLanguage* m_language;
    TSParser* m_parser = nullptr;
    TSTree* m_tree = nullptr;
};