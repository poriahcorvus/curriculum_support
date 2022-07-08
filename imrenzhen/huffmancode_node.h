#ifndef HUFFMANCODE_NODE_H
#define HUFFMANCODE_NODE_H

#include <QVector>

class Huffmancode_node
{
public:
    unsigned char name;  //字符的名称
    QVector<int> code;    //用vector容器存储哈夫曼编码
    Huffmancode_node();
};

#endif // HUFFMANCODE_NODE_H
