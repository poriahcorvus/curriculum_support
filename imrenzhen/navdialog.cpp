#include "navdialog.h"
#include "ui_navdialog.h"
#include<QTextCodec>
#include<QDateTime>
#include<QDate>
#include<QTime>
#include"quanju.h"
#define MAXEDGE 10000
#define TRUE 1
#define FALSE 0
const int maxn = 1005;//点数最大值
struct node_data
{
    QString name;
    int id;
    int x;
    int y;
};
node_data node_name[maxn];
QString route_string;
enum navoption { mindis=1, mintime ,byc};
double dis[maxn];
int n, m, cnt;//n个点，m条边
const int inf=9999999;
int  conf[maxn], pre[maxn];
int startindex,endindex,wayindex;
struct Edge
{
    int start,to, length,next;//终点，边权，同起点的上一条边的编号
    double time, crowed,time_bike;
    int bike_allowed;
}edge[maxn];//边集
int head[maxn];//head[i],表示以i为起点的第一条边在边集数组的位置（编号）

void init()//初始化
{
    for (int i = 0; i <= n; i++) head[i] = 0;
    cnt = 1;
}

void add_edge(int u, int v, int w,double crowd, int bk_allowed)//加边，u起点，v终点，w边权
{
    edge[cnt].start = u;
    edge[cnt].to = v; //终点
    edge[cnt].length = w; //权值
    edge[cnt].crowed = crowd;
    edge[cnt].time = (double)w / crowd;
    edge[cnt].bike_allowed = bk_allowed;
    edge[cnt].time_bike = bk_allowed * 0.5 * (double)w / crowd;
    edge[cnt].next = head[u];//以u为起点上一条边的编号，也就是与这个边起点相同的上一条边的编号
    head[u] = cnt++;//更新以u为起点上一条边的编号
}

struct node {
    int id;//每个节点的编号
    int total;//走到当前节点的最小化非
    node() {};//构造函数
    node(int Id, int Total) {
        id = Id;
        total = Total;
    }//构造函数
    bool operator < (const node& x) const {
        return total > x.total;
    }
};

void output(int start, int end,int* pre,int way,double *dis)
{
    QString final_route;
    int end_m = end;
    if((start<=48&&end>=49)||(start>=49&&end<=48))
        final_route.append("跨校区出行：\n");
    else final_route.append("同校区出行：\n");
    final_route.append("起点：");
    final_route.append(node_name[start].name);
    final_route.append("  \n终点：");
    final_route.append(node_name[end].name);
    final_route.append("  \n出行方式:");
    if (way == mindis) final_route.append("最短路径\n");
    else if (way == mintime) final_route.append("最短时间\n");
    else if (way == byc) final_route.append("骑行时间\n");
    if (start == end)
    {
        final_route.append("你没有走动");
        return;
    }
    int ans[maxn];
    int base = 0;
    int top = 0;
    while (pre[end] != start) {
        ans[top++] = end;
        end = pre[end];
    }
    ans[top++] = end;
    final_route.append("路线:\n");
     final_route.append(node_name[start].name);
    //cout << start;
    while (top!=base) {
        final_route.append("->");
        final_route.append(node_name[ans[top-1]].name);
        //printf("->%d", ans[top-1]);
        top = top - 1;
    }
    final_route.append("\ncost:");
    final_route.append(QString::number(dis[end_m]));
    if(way==1)  final_route.append("米");
    else final_route.append("秒");
    route_string=final_route;
}


void dijkstra(int start,int end,int way) {
    memset(dis, inf, sizeof(dis));//dis[i]表示的是从源点到点i的最短路的长度，初始时不知道能不能到达，设为inf（无穷大）
    memset(conf, FALSE, sizeof(conf));// conf[i]表示的是点i的最短路径是否确认，若是，则为true，否则为false。
    memset(pre, 0, sizeof(pre));//数组pre[i]表示的是点i的前驱，即到点i的前一个点的编号。 例如有一条路径为1->5->4->7->9 那么pre[9]=7 pre[7]=4 pre[4]=5
    dis[start] = 0;
    priority_queue <node> Q;
    Q.push(node(start, 0));
    while (Q.size()) {
        int u = Q.top().id;
        Q.pop();
        if (conf[u])
            continue;
        conf[u] = TRUE;
        for (int i = head[u]; i; i = edge[i].next) {
            int v = edge[i].to;
            double cost;
            if (way == mindis) cost = (double)dis[u] + (double)edge[i].length;
            else if (way == mintime)  cost = (double)dis[u] + edge[i].time;
            else if (way == byc)
            {
                if(edge[i].bike_allowed==0) cost = (double)dis[u] + edge[i].time;
                else cost= (double)dis[u] + edge[i].time_bike;
            }
            if (cost < dis[v]) {
                dis[v] = cost;
                pre[v] = u;
                Q.push(node(v, dis[v]));
            }
        }
    }
    output(start, end,  pre,way,dis);
}

navdialog::navdialog(int start_index,int end_index,int way_index,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::navdialog)
{
    ui->setupUi(this);
     memset(pre, 0, sizeof(pre));
    QFile mapfile("D:\\dataexample\\navdata.txt");
    if(!mapfile.open(QIODevice::ReadOnly))
        qDebug()<<mapfile.errorString();
    QTextStream TS_mapfile(&mapfile);
    TS_mapfile.setCodec("UTF-8");
    int mapnodenum;
    int mapedgenum;
    TS_mapfile >> mapedgenum ;
    TS_mapfile >> mapnodenum ;
    for(int count_node=1;count_node<=mapnodenum;count_node++)
    {
        QString nodename;
        TS_mapfile >> nodename;
        node_name[count_node].name=nodename;
        node_name[count_node].id=count_node;
        TS_mapfile >> node_name[count_node].x;
        TS_mapfile >> node_name[count_node].y;
        if(count_node!=1) ui->startnode_comboBox->addItem(nodename);
        ui->endnode_comboBox->addItem(nodename);
    }
    int edge_start,edge_end,edge_weight,edge_bike_allowed;
    double edge_crowd;
    for (int count_edge = 0; count_edge < mapedgenum; count_edge++)
    {
        TS_mapfile >> edge_start;
        TS_mapfile >> edge_end;
        TS_mapfile >> edge_weight;
        TS_mapfile >> edge_crowd;
        TS_mapfile >> edge_bike_allowed;
        add_edge(edge_start, edge_end, edge_weight, edge_crowd, edge_bike_allowed);
        add_edge(edge_end, edge_start, edge_weight, edge_crowd, edge_bike_allowed);
    }
    mapfile.close();
    ui->startnode_comboBox->setCurrentIndex(start_index);
    ui->endnode_comboBox->setCurrentIndex(end_index);
    ui->way_comboBox->setCurrentIndex(way_index);
    dijkstra(ui->startnode_comboBox->currentIndex()+1,ui->endnode_comboBox->currentIndex(),ui->way_comboBox->currentIndex());

    ui->textBrowser->setText(route_string);
    this->update();
    connect(ui->start_pushButton,&QPushButton::clicked,[=](){
        QFile op("d:\\dataexample\\op.txt");
        op.open(QIODevice::Append);
        QTextStream opp(&op);
        opp.setCodec(QTextCodec::codecForName("utf-8"));
        QDateTime dateTime(QDateTime::currentDateTime());
        QString opr="操作时间：";
        QString oppr="学生学号：";
        QString timestr=dateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
        opp <<opr<<timestr << oppr << *xuehao <<"\n";
        QString s_end=ui->endnode_comboBox->currentText();
        QString s_start=ui->startnode_comboBox->currentText();
        QString s_Way=ui->way_comboBox->currentText();
        QString aa="用户生成导航路线图\n用户选择起点：";
        QString bb="用户选择终点：";
        QString cc="用户选择的出行方式：";
         opp << aa << s_start <<"\n";
        opp << bb << s_end<<"\n";
         opp << cc << s_Way<<"\n";
        op.close();
            dijkstra(ui->startnode_comboBox->currentIndex()+1,ui->endnode_comboBox->currentIndex(),ui->way_comboBox->currentIndex());
            ui->textBrowser->setText(route_string);
            this->update();

    });

}
void navdialog::paintEvent(QPaintEvent *)
{
    //创建画家
    QPixmap benbupic("D://dataexample//buptmap201608.jpg");
    QPixmap shahepic("D://dataexample//shahemap.jpg");
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(255,0,0));
    pen.setWidth(5);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawPixmap(0,-10,641,841,benbupic);
    painter.drawPixmap(620,0,621,441,shahepic);
    startindex=ui->startnode_comboBox->currentIndex();
    endindex=ui->endnode_comboBox->currentIndex();
    int start=startindex+1;
    int end=endindex;
    while(pre[end]!=start&&(start!=0&&end!=0)&&start!=end)
    {
        if(!(end==89&&pre[end]==47)&&!(end==47&&pre[end]==89))
        painter.drawLine(QPoint(node_name[end].x,node_name[end].y),QPoint(node_name[pre[end]].x,node_name[pre[end]].y));
        else
        {
            painter.drawPoint(node_name[47].x,node_name[47].y);
            painter.drawPoint(node_name[89].x,node_name[89].y);
            if(end==89&&pre[end]==47)
            {
                 painter.drawText(node_name[47].x,node_name[47].y,"乘坐校车");
                 painter.drawText(node_name[89].x,node_name[89].y,"下校车");
            }
            if(end==47&&pre[end]==89)
            {
                painter.drawText(node_name[47].x,node_name[47].y,"下校车");
                painter.drawText(node_name[89].x,node_name[89].y,"乘坐校车");
            }
        }
        end=pre[end];
    }
    if(pre[end]==start&&!((start==47&&end>48)||(start==89&&end<49))) painter.drawLine(QPoint(node_name[end].x,node_name[end].y),QPoint(node_name[pre[end]].x,node_name[pre[end]].y));
    if(start==47&&end>48)
    {
            painter.drawText(node_name[47].x,node_name[47].y,"乘坐校车");
            painter.drawText(node_name[89].x,node_name[89].y,"下校车");
    }
    if(start==89&&end<49)
    {
       painter.drawText(node_name[47].x,node_name[47].y,"下校车");
       painter.drawText(node_name[89].x,node_name[89].y,"乘坐校车");
     }
    QPainter endpainter(this);
    QPainter startpainter(this);
    QPen startpen;
    startpen.setColor(QColor(0,0,255));
    startpen.setWidth(6);
    startpainter.setPen(startpen);
    startpainter.drawEllipse(node_name[startindex+1].x-5,node_name[startindex+1].y,10,10);
    startpainter.drawText(node_name[startindex+1].x-5,node_name[startindex+1].y,"起点");
    QPen endpen;
    endpen.setColor(QColor(128,0,128));
    endpen.setWidth(6);
    endpainter.setPen(endpen);
    endpainter.drawEllipse(node_name[endindex].x-5,node_name[endindex].y,10,10);
    startpainter.drawText(node_name[endindex].x-5,node_name[endindex].y,"终点");
}
navdialog::~navdialog()
{
    delete ui;
}
