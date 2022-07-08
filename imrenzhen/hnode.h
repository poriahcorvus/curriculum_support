#ifndef HNODE_H
#define HNODE_H


class Hnode
{
public:
    unsigned char name;    //8位的存储单元来存储字符（256种）
    unsigned  weight;  //存储字符的频度
    int parent; //双亲节点
    int lchild; //左孩子
    int rchild; //右孩子
    int Hnodeindex;  //节点索引
    Hnode();
};

#endif // HNODE_H
