
#include "Global.h"

status FileReader(CHead **C, char filename[])
{
	//以只读方式打开文件 
	FILE *fp = NULL;
	fp = fopen(filename, "r");
	if(!fp) exit(ERROR);
	
	//跳过注释行
	char ch;
	int num1, num2;
	while((ch=fgetc(fp))!='p'){//读到p跳出循环 
		if(ch=='c'){//跳过注释行 
			while((ch=fgetc(fp))!='\n'){
				;
			}
		}
	}
	while((ch=fgetc(fp))!='f'){//跳到cnf结束
		;
	}
	//读取变元数和子句数
	fscanf(fp, "%d %d", &num1, &num2);
	
	//初始化变元计数器 
	int i;
	for(i=0; i<=num1; i++){
		counter[i].count = 0.0;
		counter[i].data = 0;
		counter[i].negative = 0.0;
		counter[i].positive = 0.0;
		counter[i].value = 0;
	}
	
	//读取文件，存入到链表中 
	int x;
	PHead dnow = NULL;
	PNode rnow = NULL;
	i = num2;
	while(i>0){
		i--;
		dnow = IniClause(C);
		do{
			fscanf(fp, "%d", &x);
			counter[abs(x)].data = abs(x);
			rnow = IniNode(&(dnow->right));
			rnow->data = x;
			dnow->num++;
		}while(x!=0);
	}
	fclose(fp);
	test(C);//此处可检验是否正确构建链表
	//将子句长度作为权重来处理变元频率，子句长度越长，权重越低，权重函数：(1/2)^字句长度 
	i = num2;
	dnow = *C;
	while(i>0){
		i--;
		rnow = dnow->right;
		do{
			x = rnow->data;
			counter[abs(x)].count += pow(0.5, (double)(dnow->num - 1));
			if(x>0){
				counter[abs(x)].positive += pow(0.5, (double)(dnow->num - 1));
			}
			else{
				counter[abs(x)].negative += pow(0.5, (double)(dnow->num - 1));
			}
			rnow = rnow->next;
		}while(x!=0);
		dnow = dnow->down;
	}
	return OK;
}


CHead* IniClause(CHead **C)
{
	CHead *ne = NULL, *temp = *C;
	ne = (PHead)malloc(sizeof(CHead));   //建立新的头节点
	if(!ne) return FALSE;
	ne->down = NULL;
	ne->right = NULL;
	ne->num = -1;//子句变元数初始化为-1，便于读取CNF文件时对一个字句含有变元数进行计数 
	if(*C==NULL){
		*C = ne;
		return ne;
	}
	while(temp->down!=NULL){
		temp = temp->down;
	}
	temp->down = ne;
	return ne;
}


CNode* IniNode(CNode **right)
{
	CNode *new = (PNode)malloc(sizeof(CNode));
	CNode *temp = *right;
	if(new == NULL) return FALSE;
	new->data = 0;
	new->next = NULL;
	if(temp == NULL){
		*right = new;
		return new;
	}
	else{
		while(temp->next!=NULL) temp = temp->next;
		temp->next = new;
		return new;
	}
}

int opp(int x)
{
	return -x;
}


void test(CHead **C)
{
	PHead temp1 = *C;
	PNode temp2 = NULL;
	while(temp1!=NULL){
		printf("%d ", temp1->num);
		temp2 = temp1->right;
		while(temp2!=NULL){
			printf("%d ", temp2->data);
			temp2 = temp2->next;
		}
		printf("\n");
		temp1 = temp1->down;
	}
	printf("\n");
	/* 此处可以检查计数器
	int i;
	for(i=111; i<729; i++){
		printf("data=%d\n", counter[i].data);
		printf("count=%f\n", counter[i].count);
		printf("positive=%f\n", counter[i].positive);
		printf("negative=%f\n", counter[i].negative);
		printf("counter[%d].value=%d\n",i, counter[i].value);
		printf("\n");
	}
	printf("\n");
	*/
}