
#ifndef WORKSHOP_GLOBAL_H
#define WORKSHOP_GLOBAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR -1 
#define MAXN 1200
#define COL 9//列 
#define ROW 9//行 
#define AC 0
#define WA -1

typedef int ElemType;
typedef int status;

//子句的变元结点的定义 
typedef struct CNode{  
      ElemType data;
      struct CNode *next;
}CNode, *PNode;

//子句的头结点的定义 
typedef struct CHead{
	ElemType num;//存储子句中的变元数量 
	struct CHead *down;  //与每个子句头节点相连
	struct CNode *right;  //与第一个子句相连
}CHead, *PHead;

typedef struct times{
	double count;   //统计变元出现次数
	ElemType data;
	double positive;
	double negative; //统计变元正负权重以便在stragy2中合理选择决策变元
	ElemType value;//0：未赋值；1：赋正值：-1：赋负值 
}times;

times counter[MAXN];//计数器
int player[ROW][COL];

void test(CHead **C);
int opp(int x);  //返回相反值函数

CHead* IniClause(CHead **C);  //初始化子句函数
CNode* IniNode(CNode **right);//初始化结点函数
status DPLLSolver(CHead **C); //启动DPLL算法
CHead* isSingle(CHead *C);   //判断是否存在单子句函数
status Simplification(int uni, CHead **C);  //简化子句函数
status DelClause(int x, CHead **C);  //简化过程中删除含有决策变元的子句
status DelNode(int x, CHead **C);    //简化过程中删除决策变元负数的结点
status FreeCNodes(CHead **node);     //清除子句函数
status isEmptyClause(CHead *C);     //判断是否存在空子句函数
int strategy1(CHead **C);    
int strategy2(CHead **C);           //两个不同的变元选择策略
CHead* Duplication(CHead **C);      //复制文本函数
status AddUniClause(int x,CHead**C);//增加决策变元单子句函数
int StartSAT(char FileName[]);     //开始解决SAT问题函数
status SaveSATFile(int re, double t, char filename[]);//将结果保存在新的文件中
status FileReader(CHead **C, char filename[]);  //将文件处理为链表形式
status CreateSudokuToFile(char SudokuFile[], int holes);//将数独转化为cnf文件
void CreateSudoku(int rows[][COL]);//生成完整数独棋盘
status OtherRows(int rows[][COL], int i, int j);
void FirstRow(int firstrow[]);     //生成第一行和其他行格子
void CreatePlay(int a[][COL], int b[][COL], int numDigits);//生成有一定空洞的数独
status TranToCNF(int a[][COL], int holes, char SudokuFile[]);//把数独盘转化cnf文件
status EntrySUDOKU(CHead **C);//进入数独的函数
status SaveSudokuFile(int re, char filename[]);//将结果保存在res文件中
void print(int a[][COL]);//输出数独棋盘
#endif