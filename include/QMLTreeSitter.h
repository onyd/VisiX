#pragma once

#include <tree_sitter/api.h>
#include <QFile>
#include <string>

extern "C" {
    const TSLanguage* tree_sitter_qmljs();
}

class QMLTreeSitter : public QObject
{
public:
    QMLTreeSitter(QObject* parent = nullptr);

    void setSource(const QString& source);
    void update();
private:
    QString m_source;
    TSInput m_input;

    TSParser* m_parser;
    TSTree* m_tree;
};