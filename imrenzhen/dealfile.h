#ifndef DEALFILE_H
#define DEALFILE_H


#include <QVector>
#include <QString>
#include "hnode.h"
#include "huffmancode_node.h"
#include "course.h"
#include "courseku.h"
#include"homeku.h"

    int compress_doc(QString catalog, QString fp);//-1异常， 1压缩，上传成功， 2存在重名文件，上传失败
    int compress_hom(QString catalog, QString fp);//-1异常， 1压缩成功
    int uncompress(QString fp);
    void CreateHuffmantree(int H_num, QVector<Hnode> &HuffmanTree);
    void CreateHuffmancode(int H_num, QVector<Hnode> &HuffmanTree, QVector<Huffmancode_node> &Huffmancode);
    int CheckFilename(QString fp, QString catalog);//资料去重，返回1说明存在重名资料
    double CheckHomework(QString fp1, QString fp2);//返回两个指定文件的相似度
    double SimilarityMax(QString catalog, QString fp1);//返回文件最高重复率
    void unfile(QString fp);
    int lcs_len(QString s1, QString s2);
    void FindFile(QString catalog, QVector<QString>&filesn);
    void FindBaseFile(QString catalog, QVector<QString>&filesn);
    void FindFilePath(QString catalog, QVector<QString>&filesn);
    void FindCoursename(QString catalog, QVector<QString>&dirsn);
    void HadUp(QString catalog, QVector<QString>&hnumber, QVector<QString>&hname, QVector<QString>&hfilename, QVector<QString>&htime);//已提交作业信息
    void ToBeUp(homeKu *allhomework, Course cou[], QVector<QString>&tnumber, QVector<QString>&tname, QVector<QString>&tddl, QVector<QString>hnumber, QVector<QString>hname);//待提交作业
    QString NumtoName(Course cou[], QString conum);//传入课程编号返回课程名（已选）
    QString NametoNum(Course cou[], QString coname);//传入课程名称返回课程编号（已选）
    QString GetTime();

#endif // DEALFILE_H
