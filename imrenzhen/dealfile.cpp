#include "dealfile.h"
#include "hnode.h"
#include "huffmancode_node.h"
#include "course.h"
#include "courseku.h"
#include "manahome.h"
#include "homeku.h"
#include "quanju.h"
#include <QDebug>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QDataStream>
#include <QTextStream>
#include <QDir>
#include <QByteArray>
#include <QTextCodec>
#include <QDateTime>

int compress_doc(QString catalog, QString fp)
{
    if(CheckFilename(fp, catalog) == 1){
        qDebug() << "File Existed";
        return 2;
    }

    QFileInfo info(fp);
    QString filename = info.fileName();
    //QString sp = "d:\\dataexample\\course\\";
    QString sp = catalog;
    sp += "\\";
    filename += ".huf";
    sp += filename;

    QFile out(sp);
    out.open(QIODevice::WriteOnly);
    out.close();//创建新文件
    out.open(QIODevice::WriteOnly);

    QVector<Hnode> HuffmanTree;//存储哈夫曼树的数组
    QVector<Huffmancode_node> Huffmancode;//存储哈夫曼编码的数组
    int H_num = 0;//字符的种类数

    QFile in1(fp);
    in1.open(QIODevice::ReadOnly);
    if(!in1.isOpen()){
        qDebug() << "Not Open!";
        return -1;
    }

    //HuffmanTree.resize(1);
    //Huffmancode.resize(1);

    //QByteArray tp;
    //QString ttp;
    unsigned char temp = '\0';
    Hnode *temperary = new Hnode[256];
    while(true){

        //读取文件内容

        QByteArray tp = in1.readLine();
        //QString ttp = tp;
        //qDebug() << ttp;
        unsigned char *p = (unsigned char*)tp.data();
        for(int i = 0; i < tp.size(); i ++){
            temp = p[i];
            temperary[temp].name = temp;
            temperary[temp].weight ++;
        }
        if(in1.atEnd())     break;
    }
    in1.close();

    /*
    for(int i = 0; i < 256; i ++){
        qDebug() << i << ":" << temperary[i].name;
    }
    */

    Hnode elem;
    HuffmanTree.push_back(elem);
    for(int i = 0; i < 256; i ++){
        if(temperary[i].weight != 0){
            H_num ++;//统计字符种类
            HuffmanTree.push_back(elem);
            *(HuffmanTree.end()-1) = temperary[i];
        }
    }
    if(HuffmanTree.size() == 1)
        qDebug() << "File Empty!";
    delete[]temperary;

    for(int i = 1; i <= (H_num-1); i ++)//为叶子结点的双亲开辟内存
        HuffmanTree.push_back(elem);
    for(int i = 0; i < 2*H_num; i ++)//为哈夫曼数组的所有元素建立索引
        HuffmanTree[i].Hnodeindex = i;

    CreateHuffmantree(H_num, HuffmanTree);//构造哈夫曼树
    CreateHuffmancode(H_num, HuffmanTree, Huffmancode);//哈夫曼编码

    out.write((char*)&H_num, sizeof(int));
    for(int i = 1; i <= H_num; ++ i){
        out.write((char*)&(HuffmanTree[i].name), sizeof(char));
        out.write((char*)&(HuffmanTree[i].weight), sizeof(int));
    }

    QFile in2(fp);
    in2.open(QIODevice::ReadOnly);
    if(!in2.isOpen()){
        qDebug() << "Failed Open!";
        return -1;
    }

    unsigned char char_temp = '\0';//用8位的无符号变量作为读写字符的单元
    QString unit = "";//存储哈夫曼编码，编码的个数大于等于八位就和char_temp进行|操作

    while(true){

        if(in2.atEnd()) break;
        QByteArray ctp = in2.readLine();
        //QString cttp = ctp;
        unsigned char *cp = (unsigned char*)ctp.data();
        //qDebug() << cttp;
        //char_temp = in2.get();//在原文中读取一个字符


        for(int j = 0; j < ctp.size(); j ++){
            char_temp = cp[j];
            //qDebug() << char_temp;
            for(int i = 1; i <= H_num; ++i){
                if(Huffmancode[i].name == char_temp){
                    for(auto elem : Huffmancode[i].code){//向unit中输入编码
                        if(elem == 1)
                            unit.push_back('1');
                        else
                            unit.push_back('0');
                    }
                    break;
                }
            }
            while(unit.length() >= 8){
                char_temp = '\0';
                for(int i = 0; i < 8; ++i){
                    char_temp = char_temp << 1;
                    if(unit[i] == '1')
                        char_temp = char_temp | 1;
                }
                //auto p = unit.begin();
                //int q = (int)p;
                unit.remove(0, 8);
                //unit.erase(p, p + 8);//清空unit中前八位编码
                out.write((char*)&char_temp, sizeof(unsigned char));
            }

        }
    }
    if(unit.length() > 0){//处理最后不足八位的哈夫曼编码
        char_temp = '\0';
        for(auto i = 0; i < unit.length(); ++ i){
            char_temp = char_temp << 1;
            if(unit[i] == '1')
                char_temp = char_temp | 1;
        }
        for(auto i = 0; i < (8 - unit.length()); ++ i)//将编码移到到高位
            char_temp = char_temp << 1;
        out.write((char*)&char_temp, sizeof(unsigned char));
    }

    in2.close();
    out.close();

    HuffmanTree.resize(0);
    Huffmancode.resize(0);

    return 1;
}

int compress_hom(QString catalog, QString fp)
{
    QFileInfo info(fp);
    QString filename = info.fileName();
    //QString sp = "d:\\PoriaHua\\Poria'sGarbageCan\\shujujiegoukeshe\\homework\\";
    QString sp = catalog;
    sp += "\\";
    filename += ".huf";
    sp += filename;

    QFile out(sp);
    out.open(QIODevice::WriteOnly);
    out.close();//创建新文件
    out.open(QIODevice::WriteOnly);

    QVector<Hnode> HuffmanTree;//存储哈夫曼树的数组
    QVector<Huffmancode_node> Huffmancode;//存储哈夫曼编码的数组
    int H_num = 0;//字符的种类数
    QFile in1(fp);
    in1.open(QIODevice::ReadOnly);
    if(!in1.isOpen()){
        qDebug() << "Not Open!";
        return -1;
    }

    //HuffmanTree.resize(1);
    //Huffmancode.resize(1);

    //QByteArray tp;
    //QString ttp;
    unsigned char temp = '\0';
    Hnode *temperary = new Hnode[256];
    while(true){

        //读取文件内容

        QByteArray tp = in1.readLine();
        //QString ttp = tp;
        //qDebug() << ttp;
        unsigned char *p = (unsigned char*)tp.data();
        for(int i = 0; i < tp.size(); i ++){
            temp = p[i];
            temperary[temp].name = temp;
            temperary[temp].weight ++;
        }
        if(in1.atEnd())     break;
    }
    in1.close();

    Hnode elem;
    HuffmanTree.push_back(elem);
    for(int i = 0; i < 256; i ++){
        if(temperary[i].weight != 0){
            H_num ++;//统计字符种类
            HuffmanTree.push_back(elem);
            *(HuffmanTree.end()-1) = temperary[i];
        }
    }
    if(HuffmanTree.size() == 1)
        qDebug() << "File Empty!";
    delete[]temperary;

    for(int i = 1; i <= (H_num-1); i ++)//为叶子结点的双亲开辟内存
        HuffmanTree.push_back(elem);
    for(int i = 0; i < 2*H_num; i ++)//为哈夫曼数组的所有元素建立索引
        HuffmanTree[i].Hnodeindex = i;
    CreateHuffmantree(H_num, HuffmanTree);//构造哈夫曼树
    CreateHuffmancode(H_num, HuffmanTree, Huffmancode);//哈夫曼编码
    //qDebug() << "a";

    out.write((char*)&H_num, sizeof(int));
    for(int i = 1; i <= H_num; ++i){
        out.write((char*)&(HuffmanTree[i].name), sizeof(char));
        out.write((char*)&(HuffmanTree[i].weight), sizeof(int));
    }

    QFile in2(fp);
    in2.open(QIODevice::ReadOnly);
    if(!in2.isOpen()){
        qDebug() << "Failed Open!";
        return -1;
    }
    //qDebug() << "b";

    //QByteArray ctp;
    //QString cttp;
    unsigned char char_temp = '\0';//用8位的无符号变量作为读写字符的单元
    QString unit = "";//存储哈夫曼编码，编码的个数大于等于八位就和char_temp进行|操作
    while(true){
        QByteArray ctp = in2.readLine();
        //QString cttp = ctp;
        unsigned char *cp = (unsigned char*)ctp.data();

        //char_temp = in2.get();//在原文中读取一个字符
        //qDebug() << cttp;
        //qDebug() << cttp.size();
        for(int j = 0; j < ctp.size(); j ++){
            //qDebug() << j;
            char_temp = cp[j];
            for(int i = 1; i <= H_num; ++i){
                if(Huffmancode[i].name == char_temp){
                    for(auto elem : Huffmancode[i].code){//向unit中输入编码
                        if(elem == 1)
                            unit.push_back('1');
                        else
                            unit.push_back('0');
                    }
                    break;
                }
            }
            while(unit.length() >= 8){
                char_temp = '\0';
                for(int i = 0; i < 8; ++i){
                    char_temp = char_temp << 1;
                    if(unit[i] == '1')
                        char_temp = char_temp | 1;
                }
                unit.remove(0, 8);
                out.write((char*)&char_temp, sizeof(unsigned char));
            }

        }
       if(in2.atEnd()) break;
    }
    if(unit.length() > 0){//处理最后不足八位的哈夫曼编码
        char_temp = '\0';
        for(int i = 0; i < unit.length(); ++ i){
            char_temp = char_temp << 1;
            if(unit[i] == '1')
                char_temp = char_temp | 1;
        }
        for(int i = 0; i < (8 - unit.length()); ++ i)//将编码移到到高位
            char_temp = char_temp << 1;
        out.write((char*)&char_temp, sizeof(unsigned char));
    }

    in2.close();
    out.close();

    HuffmanTree.resize(0);
    Huffmancode.resize(0);

    return 1;
}


int uncompress(QString fp)
{
    QFileInfo info(fp);
    QString filename = info.fileName();
    QString fname = info.completeBaseName();
    QString fsuffix = info.suffix();
    if(fsuffix != "huf"){
        qDebug() << "ERROR!";
        return -1;
    }
    QString sf = "d:\\zuoye\\";
    sf += fname;

    QVector<Hnode> HuffmanTree;//存储哈夫曼树的数组
    QVector<Huffmancode_node> Huffmancode;//存储哈夫曼编码的数组
    int H_num = 0;//字符的种类数

    QFile in(fp);
    in.open(QIODevice::ReadOnly);
    if(!in.isOpen()){
        qDebug() << "Download Wrong";
        return -1;
    }
    in.read((char*)&H_num, sizeof(int));
    Hnode elem;
    HuffmanTree.push_back(elem);
    for(int i = 1; i <= H_num; ++ i){//读取字符种类及频度
        HuffmanTree.push_back(elem);
        in.read((char*)&(HuffmanTree[i].name), sizeof(char));
        in.read((char*)&(HuffmanTree[i].weight), sizeof(int));
    }
    //qDebug() << "a";
    for(int i = 1; i <= H_num - 1; ++ i)//建立完整的哈夫曼数组
        HuffmanTree.push_back(elem);
    //qDebug() << "b";
    for (int i = 0; i < 2 * H_num; i ++)//为哈夫曼数组元素建立索引
        HuffmanTree[i].Hnodeindex = i;
    CreateHuffmantree(H_num, HuffmanTree);//生成哈夫曼树
    //qDebug() << "c";
    unsigned  char temp_char = '\0';
    unsigned long long length = 0;
    for (int i = 1; i <= H_num; ++ i)//计算原文件的字符的个数
        length = length + HuffmanTree[i].weight;
    int top = HuffmanTree[2*H_num-1].Hnodeindex;//top存储的是结点的索引，初始化时根节点的索引

    QFile out(sf);
    out.open(QIODevice::WriteOnly);
    out.close();
    out.open(QIODevice::WriteOnly);
    //qDebug() << out.exists();
    if(!out.isOpen()){
        qDebug() << "Download Wrong";
        return -1;
    }
    QTextStream textout(&out);
    //textout.setCodec(QTextCodec::codecForName("GBK"));
    //qDebug() << length;
    //QByteArray tp;
    //QString ttp;
    //QString cn = "";
    //textout.setCodec(QTextCodec::codecForName("utf-8"));
    while(length){

        QByteArray tp = in.readLine();
        //QString ttp = tp;
        unsigned char *p = (unsigned char*)tp.data();
       //temp_char = in.get();
        QByteArray dd;

        for(int j = 0; j < tp.size(); j ++){
            temp_char = p[j];
            for (int i = 0; i < 8; ++ i){
                if (temp_char & 128)//如果最高位为1
                    top = HuffmanTree[top].rchild;//top存储结点的右孩子的索引
                else
                    top = HuffmanTree[top].lchild;//如果最高位为0，top存储结点的左孩子的索引
                if (top <= H_num){//如果从根节点到达叶节点
                    //textout << HuffmanTree[top].name;
                    //out.write(HuffmanTree[top].name, sizeof(unsigned char));
                   // textout << HuffmanTree[top].name;//输出字符
                    //cn += HuffmanTree[top].name;
                    //QByteArray dd;
                    dd += HuffmanTree[top].name;
                    //char m = (char)HuffmanTree[top].name;
                    //qDebug() << HuffmanTree[top].name << "\t" << m;
                    //textout << dd;
                    length --;//更新原文件中未输出的字符的个数
                    //qDebug() << length;
                    if (length == 0){
                        qDebug() << "LENGTH 0";
                        break;//如果原文件中未输出的个数为0，表明原文件的字符已经全部输出，循环不需要再继续
                    }

                    top = HuffmanTree[2*H_num-1].Hnodeindex;//否则top复位为根节点的索引
                }
                temp_char = temp_char << 1;//如果没到达叶节点，那么temp_char左移一位，继续循环
                //textout << cn;
                //cn = "";
                //qDebug() << "e";

            }
        }
        //QString cn = dd;
        textout << dd;
        if(in.atEnd())
            break;
   }


    in.close();
    out.close();

    HuffmanTree.resize(0);
    Huffmancode.resize(0);
    return 1;
}

void CreateHuffmantree(int H_num, QVector<Hnode> &HuffmanTree)
{
    //HuffmanTree.resize(1);

    if (H_num == 1)
        HuffmanTree[1].lchild = HuffmanTree[1].rchild = 1;
    int mark = H_num + 1;
    int x = 0, y = 0;
    int min = 99999999, secmin = 0;
    while (mark < 2*H_num){
        for (int j = 1; j < mark; ++ j){
            //HuffmanTree.resize(j);
            if (HuffmanTree[j].parent == 0 && HuffmanTree[j].weight < min){
                y = x;
                secmin = min;
                min = HuffmanTree[j].weight;
                x = HuffmanTree[j].Hnodeindex;
            }
            else if (HuffmanTree[j].parent == 0 && HuffmanTree[j].weight < secmin){
                secmin = HuffmanTree[j].weight;
                y = HuffmanTree[j].Hnodeindex;
            }
        }
        HuffmanTree[mark].weight = HuffmanTree[x].weight + HuffmanTree[y].weight;
        HuffmanTree[mark].lchild = x;
        HuffmanTree[mark].rchild = y;
        HuffmanTree[x].parent = mark;
        HuffmanTree[y].parent = mark;
        mark ++;
        x = y = 0;
        min = 99999999, secmin = 0;
    }
    HuffmanTree[2*H_num-1].parent = 0;
}

void CreateHuffmancode(int H_num, QVector<Hnode> &HuffmanTree, QVector<Huffmancode_node> &Huffmancode)
{
    //HuffmanTree.resize(1);
    //Huffmancode.resize(1);

    Huffmancode_node elem;
    Huffmancode.push_back(elem);
    if (H_num == 1){
        Huffmancode.push_back(elem);
        Huffmancode[1].name = HuffmanTree[1].name;
        Huffmancode[1].code.push_back(1);
    }
    for (int i = 1; i <= H_num; ++ i){
        //Huffmancode.resize(i);
        Huffmancode.push_back(elem);
        Huffmancode[i].name = HuffmanTree[i].name;
        int temp_p = HuffmanTree[i].parent;
        int temp_nodeindex = HuffmanTree[i].Hnodeindex;
        auto p = Huffmancode[i].code.end();
        while (temp_p != 0){
            if (HuffmanTree[temp_p].lchild == temp_nodeindex)
                p = Huffmancode[i].code.insert(p, 0);
            else
                p = Huffmancode[i].code.insert(p, 1);
            temp_nodeindex = HuffmanTree[temp_p].Hnodeindex;
            temp_p = HuffmanTree[temp_p].parent;
        }
    }
}

int CheckFilename(QString fp, QString catalog)
{
    QFileInfo info(fp);
    QString fn = info.fileName();

    QVector<QString> filesn;
    FindBaseFile(catalog, filesn);

    int filenumber = filesn.size();
    int flag = 0;


    for(int i = 0; i < filenumber; i ++){
        if(fn == filesn[i]){
            flag = 1;
            break;
        }
    }

    return flag;
}

void FindFile(QString catalog, QVector<QString>&filesn)
{
    QDir dir(catalog);
    if(!dir.exists()){
        qDebug() << "catalog not exist";
        return;
    }
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    if(list.size() < 1)
        return;

    int i = 0;
    do{
        QFileInfo fileinfo = list.at(i);

        //文件夹则递归
        bool bisDir = fileinfo.isDir();
        if(bisDir)
            FindFile(fileinfo.filePath(), filesn);
        else{
            filesn.push_back(fileinfo.fileName());
        }
        i ++;
    }while(i < list.size());
}

void FindBaseFile(QString catalog, QVector<QString>&filesn)
{
    QDir dir(catalog);
    if(!dir.exists()){
        qDebug() << "catalog not exist";
        return;
    }
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    if(list.size() < 1)
        return;

    int i = 0;
    do{
        QFileInfo fileinfo = list.at(i);

        //文件夹则递归
        bool bisDir = fileinfo.isDir();
        if(bisDir)
            FindBaseFile(fileinfo.filePath(), filesn);
        else{
            if(fileinfo.suffix() == "huf"){
                filesn.push_back(fileinfo.completeBaseName());
            }
        }
        i ++;
    }while(i < list.size());
}

void FindFilePath(QString catalog, QVector<QString> &filesn)
{
    QDir dir(catalog);
    if(!dir.exists()){
        qDebug() << "catalog not exist";
        return;
    }
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    if(list.size() < 1)
        return;

    int i = 0;
    do{
        QFileInfo fileinfo = list.at(i);

        //文件夹则递归
        bool bisDir = fileinfo.isDir();
        if(bisDir)
            FindFilePath(fileinfo.filePath(), filesn);
        else{
            if(fileinfo.suffix() == "huf"){
                //qDebug() << fileinfo.filePath();
                filesn.push_back(fileinfo.filePath());
            }

        }
        i ++;
    }while(i < list.size());
}

void FindCoursename(QString catalog, QVector<QString>&dirsn)
{
    QDir dir(catalog);
    QFileInfoList infoList = dir.entryInfoList(QDir::Dirs | QDir:: NoDotAndDotDot);
    QListIterator<QFileInfo> it(infoList);
    QFileInfo info;

    while(it.hasNext()){
        info = it.next();
        if(info.isDir())
            dirsn.push_back(QDir(info.absoluteFilePath()).dirName());
    }
}

double SimilarityMax(QString catalog, QString fp1)
{
    QVector<QString> filesn;
    FindFilePath(catalog, filesn);
    double simmax = 0;
    double tmp = 0;
    if(filesn.size() == 0)
        return 0;
    for(int i = 0; i < filesn.size(); i ++){
        tmp = CheckHomework(fp1, filesn[i]);
        if(tmp > simmax)
            simmax = tmp;
    }
    return simmax;
}

double CheckHomework(QString fp1, QString fp2)
{
    //unfile(fp1);
    unfile(fp2);
    //QString sp1 = "d:\\PoriaHua\\Poria'sGarbageCan\\shujujiegoukeshe\\cache\\";
    QString sp2 = "d:\\zuoye\\";
    //QFileInfo info1(fp1);
    QFileInfo info2(fp2);
    //QString filename1 = info1.completeBaseName();
    QString filename2 = info2.completeBaseName();
    //sp1 += filename1;
    sp2 += filename2;
    qDebug() << sp2;
    //QFile in1(filename1);
    QFile in1(fp1);
    in1.open(QIODevice::ReadOnly);
    if(!in1.isOpen()){
        qDebug() << "file1 cannot open";
        return -1;
    }
    QString content1 = in1.readAll();
    QFile in2(sp2);
    in2.open(QIODevice::ReadOnly);
    if(!in2.isOpen()){
        qDebug() << "file2 cannot open";
        return -1;
    }
    QString content2 = in2.readAll();

    QString s1 = "";
    QString s2 = "";

    if(content1.size() < content2.size()){
        s1 = content1;
        s2 = content2;
    }
    else{
        s1 = content2;
        s2 = content1;
    }

    double sim = 0;
    int sublen = lcs_len(s1, s2);
    sim = (double)sublen/s1.size();

    if(in2.remove() == 0){
        qDebug() << "WRONG";
        return -1;
    }

    return sim;
}

void unfile(QString fp)
{
    QFileInfo info(fp);
    QString filename = info.fileName();
    QString fname = info.completeBaseName();
    QString fsuffix = info.suffix();
    if(fsuffix != "huf"){
        qDebug() << "ERROR!";
        return;
    }
    QString sf = "d:\\zuoye\\";
    sf += fname;

    QVector<Hnode> HuffmanTree;//存储哈夫曼树的数组
    QVector<Huffmancode_node> Huffmancode;//存储哈夫曼编码的数组
    int H_num = 0;//字符的种类数

    QFile in(fp);
    in.open(QIODevice::ReadOnly);
    if(!in.isOpen()){
        qDebug() << "Download Wrong";
        return;
    }
    in.read((char*)&H_num, sizeof(int));
    Hnode elem;
    HuffmanTree.push_back(elem);
    for(int i = 1; i <= H_num; ++ i){//读取字符种类及频度
        HuffmanTree.push_back(elem);
        in.read((char*)&(HuffmanTree[i].name), sizeof(char));
        in.read((char*)&(HuffmanTree[i].weight), sizeof(int));
    }
    //qDebug() << "a";
    for(int i = 1; i <= H_num - 1; ++ i)//建立完整的哈夫曼数组
        HuffmanTree.push_back(elem);
    //qDebug() << "b";
    for (int i = 0; i < 2 * H_num; i ++)//为哈夫曼数组元素建立索引
        HuffmanTree[i].Hnodeindex = i;
    CreateHuffmantree(H_num, HuffmanTree);//生成哈夫曼树
    //qDebug() << "c";
    unsigned  char temp_char = '\0';
    unsigned long long length = 0;
    for (int i = 1; i <= H_num; ++ i)//计算原文件的字符的个数
        length = length + HuffmanTree[i].weight;
    int top = HuffmanTree[2*H_num-1].Hnodeindex;//top存储的是结点的索引，初始化时根节点的索引

    QFile out(sf);
    out.open(QIODevice::WriteOnly);
    out.close();
    out.open(QIODevice::WriteOnly);
    //qDebug() << out.exists();
    if(!out.isOpen()){
        qDebug() << "Download Wrong!";
        return;
    }
    QTextStream textout(&out);
    //textout.setCodec(QTextCodec::codecForName("GBK"));
    //qDebug() << length;
    //QByteArray tp;
    //QString ttp;
    //QString cn = "";
    //textout.setCodec("utf-8");
    while(length){

        QByteArray tp = in.readLine();
        //QString ttp = tp;
        unsigned char *p = (unsigned char*)tp.data();
       //temp_char = in.get();
        QByteArray dd;

        for(int j = 0; j < tp.size(); j ++){
            temp_char = p[j];
            for (int i = 0; i < 8; ++ i){
                if (temp_char & 128)//如果最高位为1
                    top = HuffmanTree[top].rchild;//top存储结点的右孩子的索引
                else
                    top = HuffmanTree[top].lchild;//如果最高位为0，top存储结点的左孩子的索引
                if (top <= H_num){//如果从根节点到达叶节点
                    //textout << HuffmanTree[top].name;
                    //out.write(HuffmanTree[top].name, sizeof(unsigned char));
                   // textout << HuffmanTree[top].name;//输出字符
                    //cn += HuffmanTree[top].name;
                    //QByteArray dd;
                    dd += HuffmanTree[top].name;
                    //char m = (char)HuffmanTree[top].name;
                    //qDebug() << HuffmanTree[top].name << "\t" << m;
                    //textout << dd;
                    length --;//更新原文件中未输出的字符的个数
                    //qDebug() << length;
                    if (length == 0){
                        qDebug() << "LENGTH 0";
                        break;//如果原文件中未输出的个数为0，表明原文件的字符已经全部输出，循环不需要再继续
                    }

                    top = HuffmanTree[2*H_num-1].Hnodeindex;//否则top复位为根节点的索引
                }
                temp_char = temp_char << 1;//如果没到达叶节点，那么temp_char左移一位，继续循环
                //textout << cn;
                //cn = "";
                //qDebug() << "e";

            }
        }
        textout << dd;
        if(in.atEnd())
            break;
   }


    in.close();
    out.close();

    HuffmanTree.resize(0);
    Huffmancode.resize(0);
}

int lcs_len(QString s1, QString s2)//动态规划
{
    int len1 = s1.size();
    int len2 = s2.size();
    if(len1 == 0 || len2 == 0)
        return 0;
    //定义二维数组dp[i][j]  代表串1从0~i这段与串2从0~j这段的公共子串的最大值
    //赋初值dp[0~len1][0]=0   dp[0][0~len2]=0

    QVector<QVector<int>> dp(len1+1, QVector<int>(len2+1, 0));
    for(int i = 1; i <= len1; i ++){
        for(int j = 1; j <= len2; j ++){
            if(s1[i-1] == s2[j-1]){
                //若相等，上层值+1
                dp[i][j] = dp[i-1][j-1] + 1;
            }
            else//若不相等则等于交错值中的最大值
                dp[i][j] = qMax(dp[i-1][j], dp[i][j-1]);
        }
    }
    return dp[len1][len2];
}

void HadUp(QString catalog, QVector<QString> &hnumber, QVector<QString> &hname, QVector<QString> &hfilename, QVector<QString> &htime)
{
    QString ca = catalog;
    ca += "\\homework.txt";
    QFile hfile(ca);
    hfile.open(QIODevice::ReadOnly);
    while(!hfile.atEnd()){
        QString ss = hfile.readLine();
        QString mm = "";
        int cc = 0;
        for(int i = 0; i < ss.size(); i ++){
            if(ss[i] != ' ' && (ss[i] != '\r' && ss[i+1] != '\n'))
                mm += ss[i];
            else{

                if(cc == 0)
                    hnumber.push_back(mm);
                else if(cc == 1)
                    hname.push_back(mm);
                else if(cc == 2)
                    hfilename.push_back(mm);
                else if(cc == 3)
                    htime.push_back(mm);
                mm = "";
                cc += 1;
            }
        }
    }
}

void ToBeUp(homeKu *allhomework, Course cou[], QVector<QString> &tnumber, QVector<QString> &tname, QVector<QString> &tddl, QVector<QString> hnumber, QVector<QString> hname)
{
    //库里所有的作业 - 已经提交的作业
    int len = (*allcourse);
    for(int i = 0; i < len; i ++){
        //某门课的所有作业信息
        QList<Manahome> listall = (*allhomework).thehomo(cou[i].cNum);
        //一一与已经提交的作业比对
        for(int j = 0; j < listall.size(); j ++){
            int flag = 0;
            for(int k = 0; k < hnumber.size(); k ++){
                if(hnumber[k] == listall[j].cu_num && hname[k] == listall[j].homo_name){
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                tnumber.push_back(listall[j].cu_num);//课程编号
                tname.push_back(listall[j].homo_name);//作业名
                //截止时间
                QString s1 = QString::number(listall[j].ddl_howweek);

                QString s2 = "";
                if(listall[j].ddl_week == 1)
                    s2 = "星期一";
                else if(listall[j].ddl_week == 2)
                    s2 = "星期二";
                else if(listall[j].ddl_week == 3)
                    s2 = "星期三";
                else if(listall[j].ddl_week == 4)
                    s2 = "星期四";
                else if(listall[j].ddl_week == 5)
                    s2 = "星期五";
                else if(listall[j].ddl_week == 6)
                    s2 = "星期六";
                else if(listall[j].ddl_week == 7)
                    s2 = "星期日";

                QString s3 = QString::number(listall[j].ddl_hour);
                QString s4 = QString::number(listall[j].ddl_min);

                QString ss = "";
                ss += "第";
                ss += s1;
                ss += "周";
                ss += s2;
                ss += s3;
                ss += ":";
                ss += s4;

                tddl.push_back(ss);
            }

        }
    }
}

QString NumtoName(Course cou[], QString conum)
{
    int len = (*allcourse);
    QString s = "";
    for(int i = 0; i < len; i ++){
        if(conum == cou[i].cNum){
            s = cou[i].cName;
            break;
        }
    }
    return s;
}

QString NametoNum(Course cou[],QString coname)
{
    int len = (*allcourse);
    QString s = "";
    for(int i = 0; i < len; i ++){
        if(coname == cou[i].cName){
            s = cou[i].cNum;
            qDebug() << cou[i].cNum;
            break;
        }

    }
    return s;
}

QString GetTime()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    QString tt = dateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
    return tt;
}

