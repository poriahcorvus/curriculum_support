#include "quanju.h"

int curWeek;
int curHour;
int curMin;
int allweek;
int* base;
int* allclock;
QString* cPath;
int* allact;
clockKu* naozhong;
int* nowbase;
int* allcourse;
int* coursenumber;
QString Whereplace[200];
int* allwork;
int* allgroupAct;
QString *xuehao;

QString where(int place){
    if(Whereplace[place] != "")
    return Whereplace[place];
    else return "N楼";
}
