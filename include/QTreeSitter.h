#pragma once

#include <tree_sitter/api.h>
#include <optional>
#include <functional>
#include <QFile>
#include <QString>
#include <QVector>

extern "C" {
    const TSLanguage* tree_sitter_qmljs();
}

typedef TSRange QTSRange;
typedef TSPoint QTSPoint;
typedef TSQueryCapture QTSCapture;
typedef TSLanguage QTSLanguage;
typedef TSInputEdit QTSInputEdit;

template <class T>
class UpdateIterator
{
    using iterator_category = std::input_iterator_tag;

    using value_type = T;
    using reference_type = value_type&;

    using update_t = std::function<value_type(reference_type)>;

    UpdateIterator(const reference_type first, const update_t& update_fn) : m_value(first), m_update(update_fn) {}

    const reference_type operator*() const
    {
        return m_match;
    }

    // Prefix increment
    UpdateIterator& operator++()
    {
        m_value = m_update(m_value);
        return *this;
    }

    // Postfix increment
    UpdateIterator operator++(int) { UpdateIterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const UpdateIterator& a, const UpdateIterator& b) { return (a.m_value == b.m_value); };
    friend bool operator!= (const UpdateIterator& a, const UpdateIterator& b) { return !(a == b); };

private:
    update_t m_update;
    value_type m_value;
};

class QTSNode
{
public:
    QTSNode();
    QTSNode(const TSNode& node);

    const char* type() const;

    bool isNull() const;
    bool isNamed() const;
    bool isMissing() const;
    bool isExtra() const;
    bool hasError() const;

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
    friend class QTSTreeCursor;
    friend class QTSMatches;
    
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
    TSTreeCursor m_cursor;
};

class QTSQuery
{
public:
    QTSQuery(const std::string& source, const TSLanguage* language);
    ~QTSQuery();

private:
    friend class QTSMatches;

    TSQuery* m_query;
};

class QTSMatch
{
public:
    
    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        using value_type = QTSNode;
        using pointer_type = const QTSCapture*;

        iterator(pointer_type ptr) : m_ptr(ptr) {}

        value_type operator*() const { return QTSNode(m_ptr->node); }

        // Prefix increment
        iterator& operator++() { m_ptr++; return *this; }

        // Postfix increment
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer_type m_ptr;
    };

public:

    QTSMatch();
    QTSMatch(const TSQueryMatch& match);

    bool valid() const;
    QTSNode at(qsizetype i) const;
    qsizetype size() const;

    iterator begin() const;
    iterator end() const;

    friend bool operator==(const QTSMatch& a, const QTSMatch& b) { return (a.m_match == std::nullopt && a.m_match == std::nullopt ||
                                                                          (a.m_match != std::nullopt && a.m_match != std::nullopt && a.m_match->id == b.m_match->id)); };
    friend bool operator!=(const QTSMatch& a, const QTSMatch& b) { return !(a == b); };

private:

    std::optional<TSQueryMatch> m_match;
};

class QTSMatches
{
public:
    class iterator : public UpdateIterator<QTSMatch>
    {
    public:
        iterator(const QTSMatch& match) 
            : UpdateIterator<QTSMatch>(match, 
                [](const QTSMatch& current) 
                { return QTSMatch(); 
                }) 
        {}

    private:
        TSQueryCursor* m_cursor;
    };

    QTSMatches(const QTSQuery& query, const QTSNode& node);
    ~QTSMatches();

    iterator begin() const;
    iterator end() const;

private:
    friend struct iterator;

    QTSQuery m_query;
    QTSNode m_node;
};

class QTreeSitter : public QObject
{
public:
    QTreeSitter(const QTSLanguage* language, QObject* parent = nullptr);
    ~QTreeSitter();

    void setSource(const QString& source);
    const QString& source() const;

    void setLanguage(const QTSLanguage* language);
    const QTSLanguage* language() const;

    void edit(const QTSInputEdit& edit);
    void update();

    QTSNode root() const;

    QStringView text(const QTSNode& node) const;
    QStringView text(const QTSRange& range) const;

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