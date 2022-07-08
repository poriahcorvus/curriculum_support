#include "clockthread.h"
#include"quanju.h"
#include<QDebug>
#include<QDialog>
#include<QMessageBox>
#include<msgbox.h>

void clockThread::run(){

    cPath = new QString;
    *cPath = "";

    while(*cPath == ""){

    }
    naozhong = new clockKu(*cPath);

    QString toshow = "";

    while(1){
        if(*base != 0){
            for(int i = 0 ; i < *allclock; i++){
                switch (naozhong->clock[i].Cfrequen) {
                case 1:
                    {
                    if(naozhong->clock[i].Cisuse == 0){
                        if(naozhong->clock[i].Cweek == curWeek && naozhong->clock[i].Chour == curHour && naozhong->clock[i].Cmin == curMin){
                            toshow += naozhong->clock[i].Ccontent + "、";
                        naozhong->clock[i].Cisuse = 1;
                        }
                    }
                    break;
                        }
                case 2:
                    {
                    if(naozhong->clock[i].Cweek == curWeek && naozhong->clock[i].Chour == curHour && naozhong->clock[i].Cmin == curMin)
                        toshow += naozhong->clock[i].Ccontent + "、";
                    break;
                        }
                default:
                    {

                     if(naozhong->clock[i].Chour == curHour && naozhong->clock[i].Cmin == curMin)
                         toshow += naozhong->clock[i].Ccontent + "、";
                    break;
                    }
                }
            }
            if(toshow != ""){
            *nowbase = *base;
            *base = 0;
            msgBox::show("提醒","请注意  活动： " + toshow + " 即将开始！",1);

            toshow = "";
            }
            msleep(*base);
        }

    }

}
