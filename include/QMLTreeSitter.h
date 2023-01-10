#pragma once

#include <tree_sitter/api.h>
#include <QFile>
#include <QString>
#include <QVector>

extern "C" {
    const TSLanguage* tree_sitter_qmljs();
}

class QTSNode
{
public:
    QTSNode(const TSNode& node);

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

class QTSMatches
{
public:
    QTSMatches();
    ~QTSMatches();

    QVector<QTSNode>::iterator begin();
    QVector<QTSNode>::iterator end();

private:
    friend class QTreeSitter;

    QVector<TSQueryMatch*> m_matches;
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

    TSNode root();

    TSRange nodeRange(const TSNode& node);
    QStringView nodeText(const TSNode& node);

    QStringView text(const TSRange& range);

    QTSQuery build(const QString& source) const;
    QTSMatches match(const QTSQuery& query);
    QTSMatches match(const QTSQuery& query, const QTSNode& node);

    static QTreeSitter QML(QObject* parent = nullptr);

private:
    QString m_source;
    TSInput m_input;

    const TSLanguage* m_language;
    TSParser* m_parser = nullptr;
    TSTree* m_tree = nullptr;
    TSQueryCursor* m_query_cursor = nullptr;
};