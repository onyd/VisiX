#pragma once

#include <tree_sitter/api.h>
#include <QFile>
#include <QString>
#include <QVector>

extern "C" {
    const TSLanguage* tree_sitter_qmljs();
}

typedef TSRange QTSRange;

class QTSNode
{
public:
    QTSNode(const TSNode& node);

    uint32_t start() const;
    uint32_t end() const;

    TSPoint startPos() const;
    TSPoint endPos() const;

    QTSRange range() const;

private:
    friend class QTreeSitter;
    
    TSNode m_node;
};

class QTSQuery
{
public:
    QTSQuery(const QString& source, const TSLanguage* language);
    ~QTSQuery();

private:
    friend class QTreeSitter;

    TSQuery* m_query;
};

typedef QTSNode QTSCapture;
typedef QVector<QTSCapture> QTSCaptures;

class QTSMatches
{
public:
    QTSMatches() = default;

    bool next();
    QTSCapture currentCapture() const;
    QTSCaptures currentCaptures() const;

private:
    friend class QTreeSitter;

    TSQueryMatch m_match;
    bool m_finished = false;
};

class QTreeSitter : public QObject
{
public:
    QTreeSitter(const TSLanguage* language, QObject* parent = nullptr);
    ~QTreeSitter();

    void setSource(const QString& source);
    const QString& source() const;

    void setLanguage(const TSLanguage* language);
    const TSLanguage* language() const;

    void edit(const TSInputEdit& edit);
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

private:
    QString m_source;
    TSInput m_input;

    const TSLanguage* m_language;
    TSParser* m_parser = nullptr;
    TSTree* m_tree = nullptr;
    TSQueryCursor* m_query_cursor = nullptr;
};