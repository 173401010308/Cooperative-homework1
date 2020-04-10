#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
using  namespace std;

struct Graph
{
    int vexnum;  //城市个数
    int edge;   //道路条数
    int arc[100][100]; //邻接矩阵
    char information[100][20]; //城市名称
};

void Create_Graph(Graph &g)
{
    FILE *fp;
    fp=fopen("in.txt","r");
    fscanf(fp,"%d %d",&g.vexnum,&g.edge);
    int i=0,j=0;
    for (i=0;i<g.vexnum;i++)
    {
        fscanf(fp,"%s",g.information[i]);
        for (j=0;j<g.vexnum;j++)
        {
            g.arc[i][j]=INT_MAX;          //初始化邻接矩阵
        }
    }
    for(i=0;i<g.edge;i++)
    {
        int start;
        int end;
        int weight;
        fscanf(fp,"%d",&start);  //输入道的起点
        fscanf(fp,"%d",&end);    //输入道的终点
        fscanf(fp,"%d",&weight); //输入道的距离
        g.arc[start-1][end-1]=weight;
        g.arc[end-1][start-1]=weight;
    }
    fclose(fp);
}

struct Edge
{
    int start;
    int end;
    int weight;
};

void Prim_Graph(Graph g,int begin)
{
    FILE *fp;
    fp=fopen("out.txt","w");
    fprintf(fp,"最佳方案道路:起点——终点=道路的距离\n");
    Edge *close_edge=new Edge[g.vexnum];
    int j,min=INT_MAX,k,index;
    //初始化
    for(j=0;j<g.vexnum;j++)
    {
        if(j!=begin-1)
        {
            close_edge[j].start=begin-1;
            close_edge[j].end=j;
            close_edge[j].weight=g.arc[begin-1][j];
        }
    }
    close_edge[begin-1].weight=-1; //-1代表已经加入到集合U
    for(j=1;j<g.vexnum;j++)
    {
        min=INT_MAX;
        //寻找数组close_edge中距离最短的路
        for(k=0;k<g.vexnum;k++)
        {
            if(close_edge[k].weight!=-1)
            {
                if(close_edge[k].weight<min)
                {
                    min=close_edge[k].weight;
                    index=k;
                }
            }
        }
        //将权重最小的那条边的终点也加入到集合U
        close_edge[index].weight=-1;
        //输出对应的边的信息
        fprintf(fp,"%s——%s=%d\n",g.information[close_edge[index].start],g.information[close_edge[index].end],g.arc[close_edge[index].start][close_edge[index].end]);
        //更新数组
        for (k=0;k<g.vexnum;k++)
        {
            if(g.arc[close_edge[index].end][k]<close_edge[k].weight)
            {
                close_edge[k].weight=g.arc[close_edge[index].end][k];
                close_edge[k].start=close_edge[index].end;
                close_edge[k].end=k;
            }
        }
    }
    fclose(fp);
}

void Increase_Graph(Graph &g)
{
    FILE *fp;
    int increase_vexnum;  //增加的城市数
    int increase_edge;    //增加的道路数
    fp=fopen("increase.txt","r");
    fscanf(fp,"%d %d",&increase_vexnum,&increase_edge);
    int i=0,j=0;
    g.vexnum=g.vexnum+increase_vexnum;
    for(i=g.vexnum-increase_vexnum;i<g.vexnum;i++)
    {
        fscanf(fp,"%s",g.information[i]);
    }
    for(i=0;i<g.vexnum;i++)
    {
        for (j=g.vexnum-increase_vexnum;j<g.vexnum;j++)
        {
            g.arc[i][j]=INT_MAX;
            g.arc[j][i]=INT_MAX;          //初始化增点后邻接矩阵
        }
    }
    g.edge=g.edge+increase_edge;
    for(i=g.edge-increase_edge;i<g.edge;i++)
    {
        int start;
        int end;
        int weight;
        fscanf(fp,"%d",&start);  //输入道的起点
        fscanf(fp,"%d",&end);    //输入道的终点
        fscanf(fp,"%d",&weight); //输入道的距离
        g.arc[start-1][end-1]=weight;
        g.arc[end-1][start-1]=weight;
    }
    fclose(fp);
}

void Decrease_Graph(Graph &g)
{
    FILE *fp;
    int decrease_vexnum;                  //删除点的数量（并删除相关边）
    int decrease_vexnum_arrangement;      //删除的点的位置
    int decrease_edge;                    //删除边的数量（不包含删除点时删除的边）
    fp=fopen("decrease.txt","r");
    fscanf(fp,"%d %d",&decrease_vexnum,&decrease_edge);
    int i=0,j=0,k=0;
    for(i=0;i<decrease_edge;i++)
    {
        int start;
        int end;
        fscanf(fp,"%d",&start);  //输入道的起点
        fscanf(fp,"%d",&end);    //输入道的终点
        g.arc[start-1][end-1]=INT_MAX;
        g.arc[end-1][start-1]=INT_MAX;
    }
    g.edge=g.edge-decrease_edge;
    for(k=0;k<decrease_vexnum;k++)
    {
        fscanf(fp,"%d",&decrease_vexnum_arrangement);
        decrease_vexnum_arrangement=decrease_vexnum_arrangement-1;
        for(i=decrease_vexnum_arrangement;i<g.vexnum;i++)
        {
            strcpy(g.information[i],g.information[i+1]);    //城市名前移
        }
        i=decrease_vexnum_arrangement;
        for (j=0;j<g.vexnum;j++)
        {
            g.arc[i][j]=g.arc[i+1][j];                      //邻接矩阵前移
        }
        for (j=0;j<g.vexnum;j++)
        {
            g.arc[j][i]=g.arc[j][i+1];                      //邻接矩阵前移
        }
    }
    g.vexnum=g.vexnum-decrease_vexnum;
    fclose(fp);
}

int main()
{
    Graph g;
    Create_Graph(g);
    Increase_Graph(g);
    Decrease_Graph(g);
    Prim_Graph(g, 1);
    return 0;
}
