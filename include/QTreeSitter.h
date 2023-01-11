#pragma once

#include <tree_sitter/api.h>
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

class QTSNode
{
public:
    QTSNode();
    QTSNode(const TSNode& node);

    uint32_t start() const;
    uint32_t end() const;

    QTSPoint startPos() const;
    QTSPoint endPos() const;

    QTSRange range() const;

private:
    friend class QTreeSitter;
    friend class QTSMatches;
    
    TSNode m_node;
};

class QTSQuery
{
public:
    QTSQuery(const QString& source, const TSLanguage* language);
    ~QTSQuery();

private:
    friend class QTSMatches;

    TSQuery* m_query;
};

class QTSMatch
{
public:
    
    struct constIterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        using value_type = QTSNode;

        using carrier_type = QTSCapture;
        using pointer_type = const carrier_type*;

        constIterator(pointer_type ptr) : m_ptr(ptr) {}

        value_type operator*() const { return QTSNode(m_ptr->node); }

        // Prefix increment
        constIterator& operator++() { m_ptr++; return *this; }

        // Postfix increment
        constIterator operator++(int) { constIterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const constIterator& a, const constIterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const constIterator& a, const constIterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer_type m_ptr;
    };

    using const_iterator = constIterator;

public:
    using QTSCaptures = QVector<QTSNode>;

    QTSMatch(const TSQueryMatch& match);

    QTSNode at(qsizetype i) const;
    qsizetype size() const;

    QTSCaptures::iterator begin();
    QTSCaptures::iterator end();

    QTSCaptures::ConstIterator cbegin() const;
    QTSCaptures::ConstIterator cend() const;

private:
    QTSCaptures m_captures;
};

class QTSMatches
{
public:
    QTSMatches(const QTSQuery& query, const QTSNode& node);
    ~QTSMatches();

    bool next();
    QTSMatch current() const;

private:
    QTSQuery m_query;
    QTSNode m_node;
    TSQueryMatch m_match;
    TSQueryCursor* m_cursor;
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

    QTSQuery build(const QString& source) const;

    QTSMatches match(const QString& query_source) const;
    QTSMatches match(const QString& query_source, const QTSNode& node) const;
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