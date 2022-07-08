#include "timethread.h"
#include"quanju.h"
#include<QDebug>
#include<QMessageBox>

void timeThread::run(){
    allweek = 1;
    curWeek = 1;
    curHour = curMin = 0;
    base = new int;
    *base = 500;
    nowbase = new int;
    *nowbase = *base;
    while(1){
        if(*base != 0){
            if(curWeek == 7 && curHour == 23 && curMin == 59){
                curWeek = 1;
                curHour = curMin = 0;
                allweek++;
            }else if(curHour == 23 && curMin == 59){
                curWeek ++;
                curHour = curMin = 0;
            }else if(curMin == 59){
                curHour ++;
                curMin = 0;
            }else{
                curMin ++;
            }
//            qDebug() << curWeek << curHour << curMin;
            msleep(*base);
        }

    }

}
