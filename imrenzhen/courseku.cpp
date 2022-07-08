#include "courseku.h"
#include<QString>
#include<QFile>
#include<QDebug>
#include <QByteArray>
#include"quanju.h"

//在构造函数中要把这几个都准备好
courseKu::courseKu(){

    coursenumber = new int;
    *coursenumber = 0;

    //这里放课程库的文件的地址
    QString path = "D:\\dataexample\\course\\allCourse.txt";

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    int i = 0;

    while( !file.atEnd() ){
    QString result = file.readLine();
        all[i].cNum = cut(result);
        all[i].cName = cut(result);
        all[i].teacher = cut(result);
        all[i].cGroup = cut(result);
        all[i].cWeek = cut(result);
        all[i].cStartHour = cut(result);
        all[i].cStartMin = cut(result);
        all[i].cEndHour = cut(result);
        all[i].cEndMin = cut(result);
        all[i].cPlace = cut(result);
        all[i].cDetail = cut(result);
        all[i].kTime = cut(result);
        all[i].kPlace = cut(result);
        all[i].kDetail = cut(result);
        all[i].cPopular = cut(result);
        all[i].similar = 0;
        i++;
   }

    *coursenumber = i;
    file.close();

    int place = 0;

    //准备两个哈希数组
    for(int j = 0; j < i; j++){
        place = Hash(all[j].cName);
        if(name[place].cNum == "")
            name[place] = all[j];
        else{
            while(name[place].cNum != ""){
                place++;
                place = place%100;
            }
            name[place] = all[j];
        }
    }

    for(int j = 0; j < i; j++){
        place = Hash(all[j].cNum);
        if(num[place].cNum == "")
            num[place] = all[j];
        else{
            while(num[place].cNum != ""){
                place++;
                place = place%100;
            }
            num[place] = all[j];
        }
    }


}

//一个哈希函数 一个模糊搜索函数
int courseKu::Hash(QString tocu){

   int i = 0;

   std::string s;
   const char* a;
   int result = 0;

   s = tocu.toStdString();
   a = s.data();

   while(a[i] != 0){
       if(a[i] > 0){
           result += a[i];
           i++;
       }else{
           for(int j = 0; j < 3; j++){
               result += -a[i];
               i++;
           }
       }
   }
    return result%100;
}

int courseKu::search(QString tofd,int type){

    //name1 num2
    int result = -1;
    int place;

    if(type == 1){
        place = Hash(tofd);
        while(name[place].cNum != ""){
           if( QString::compare(tofd,name[place].cName) == 0){
               result = place;
               break;
           }
           place++;
        }
    }else{
        place = Hash(tofd);
        while(num[place].cNum != ""){

           if( QString::compare(tofd,num[place].cNum) == 0){
               result = place;
               break;
           }
           place++;
        }
    }
    return result;
}

//现在来解决模糊搜索部分的问题
void courseKu::Muhu(QString tofd,Course* mh){
    int j = 1;
    int result = 0;  

//    int start = 1;
//    int maxsimi = 0;
//    int strsize = tofd.size();
//    for(int www = 0; www < strsize; www++)
//        maxsimi += www+1;

    QString ch;
    std::string s;
    std::string ss;
    const char* aa;
    const char* tofds;

    s = tofd.toStdString();
    tofds = s.data();

//    if(mh[1].cNum != "")
//        start = 2;
//        j = start;

    for(int i = 0; i < *coursenumber ; i++){
        ch = change(all[i]);
        ss = ch.toStdString();
        aa = ss.data();
        result = howsimi(aa,tofds);

        if(result > 0){
              mh[j] = all[i];
              mh[j].similar = result;
              j++;
              }
    }

////        if(start == 1){
////            if(result > 0){
////            mh[j] = all[i];
////            mh[j].similar = result;
////            j++;
////            }
////            i++;
////        }else{
////            if(result > 0 && result != maxsimi){
////            mh[j] = all[i];
////            mh[j].similar = result;
////            j++;
////            }
////            i++;
////        }

        mh[0].similar = j - 1;
}

int courseKu::howsimi(const char* foun,const char* cond){

    char* p=(char*)cond;
    int len=Strlen(cond);
    int count=0;    //记录相似程度
    for(int i=0 ;i < len;i++)  //条件字符串的准长度
    {
     p=(char*)cond;
     for(int j = 0 ; j < i+1 ; j++)  //有多少个准长度相同的子串就循环多少次
     {
      char *s=p;
      char temp[20];
      char *t=temp;
      for(int k=0;k<len-i;k++)  //子串的准长度是多少就循环多少次
      {
       if(*s>0)     //如果当前的字符的ASCII码为正(赋值一位),为负(赋值三位)
       {
        *t=*s;   //赋值一位
       }
       else
       {
        *t=*s;
        *(++t)=*(++s);   //赋值三位
        *(++t)=*(++s);
       }
       s++;
       t++;
      }
      *t=0;
      t=new char[20];
      strcpy(t,temp);
      if(Contain(foun,t)) count++;   //判断姓名字符串是否包含刚拆分的条件字符串的子串
      delete t;
      (*p>0)?p++:p=p+3;    //如果当前的字符的ASCII码为正(指针下移一位),为负(指针下移三位)
     }
    }
    return count;
}

int courseKu::Strlen(const char* p){
    int len=0;
    while(*p!= 0)
    {
     len++;
     (*p>0)?p++:p=p+3;  //如果当前的字符的ASCII码为正(指针下移一位),为负(指针下移三位)
    }
    return len;
}

int courseKu::Contain(const char* a,const char*b){
    int len=Strlen(b);
    while(*a!= 0)
    {
     bool flag;
     if(*a<0)  //如果当前的字符的ASCII码为正(判断一位是否相等),为负(判断两位是否相等)
     {
      flag=(*a==*b&&*(a+1)==*(b+1)&&*(a+2)==*(b+2));   //判断三位
     }
     else
     {
      flag=(*a==*b);    //判断一位
     }
     if(flag)
     {
      char *p=(char*)a,*q=(char*)b,count=0;
      while(*p!=0 &&*q!=0 )
      {
       if(*p<0)   //如果当前的字符的ASCII码为正(判断一位是否相等),为负(判断三位是否相等)
       {
        flag=(*p==*q&&*(p+1)==*(q+1)&&*(a+2)==*(b+2));   //判断三位
       }
       else
       {
        flag=(*p==*q);    //判断一位
       }
       if(flag) count++;  //相等count加1
       (*p>0)?p++:p=p+3;  //如果当前的字符的ASCII码为正(指针下移一位),为负(指针下移三位)
       (*q>0)?q++:q=q+3;
      }
      if(count==len)   //count等于字符串b的准长度表示字符串a包含字符串b返回1
      {
       return 1;
      }
     }
     (*a>0)?a++:a=a+3;  //如果当前的字符的ASCII码为正(指针下移一位),为负(指针下移三位)
    }
    return 0;
}

QString courseKu::change(Course a){   
    QString s;
    bool ok;
    s += a.cNum;
    s += a.cName;
    s += a.cGroup;
    s += a.teacher;
    s += "星期";
    switch(a.cWeek.toInt(&ok,10)) {
    case 1:
        s += "一";
        break;
    case 2:
        s += "二";
        break;
    case 3:
        s += "三";
        break;
    case 4:
        s += "四";
        break;
    default:
        s += "五";
        break;
    }
    return s;
}

QString courseKu::cut(QString result){

    QString s = "";
    static int m = 0;

    while(result[m] != " "  && result[m] != "\r"){
        s += result[m];
        m++;
    }
    m++;

    if(result[m-1] == "\r")
        m = 0;

    return s;
}
