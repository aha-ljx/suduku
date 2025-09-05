#include"Global.h"

status DPLLSolver(CHead **C)
{
	/* ---------- 单子句传播 ---------- */
    PHead dnow;
    while ((dnow = isSingle(*C)) != NULL) {
        int uni = dnow->right->data;
        counter[abs(uni)].value = (uni > 0) ? 1 : -1;
        Simplification(uni, C);
        if (*C == NULL) return TRUE;
        if (isEmptyClause(*C)) return FALSE;
    }

    /* ---------- 决策点 ---------- */
    int var = strategy2(C);               // 选择决策变量
    int lit = var;                        // 先尝试正文字

    /* 保存旧值以便回溯 */
    int oldVal = counter[abs(var)].value;

    /* 分支 1：尝试 lit */
    counter[abs(var)].value = (lit > 0) ? 1 : -1;
    PHead copy1 = Duplication(C);
    AddUniClause(lit, &copy1);
    if (DPLLSolver(&copy1)) return TRUE;

    /* 分支 2：尝试 -lit */
    counter[abs(var)].value = (lit > 0) ? -1 : 1;   // 切换赋值
    PHead copy2 = Duplication(C);
    AddUniClause(opp(lit), &copy2);
    if (DPLLSolver(&copy2)) return TRUE;

    /* 回溯：恢复变量未赋值状态 */
    counter[abs(var)].value = oldVal;
    return FALSE;
}

CHead* isSingle(CHead *C)
{
	CHead *temp = C;
	while(temp!=NULL){
		if(temp->num == 1){//该子句变元数为1 
			return temp;
		}
		temp = temp->down;
	}
	return NULL;//无单子句，返回NULL 
}

status Simplification(int uni, CHead **C)
{
	DelClause(uni, C);//删除含有uni的整个子句 
	DelNode(uni, C);  //删除值为-uni的结点 
	return OK;
}


status DelClause(int x, CHead **C)
{
	PHead p = *C, q = NULL;
	PNode m = NULL;
	int flag = 1;
	//首先分析首句
	//当首句有决策变元就删除，一直到首句没有决策变元为止
	while(p!=NULL){
		m = p->right;
		while(m!=NULL){      //查找子句中是否含有决策变元
			if(m->data==x){
				flag = 0;
				break;
			}
			m = m->next;
		}
		if(flag==1){
			break;//说明第一个子句不用删，退出循环 
		}
		else if(flag==0){//需要删除，且再次进入循环验证第一个子句 
			q = *C;
			*C = (*C)->down;
			p = *C;   //这里p是记录新起点的位置
			FreeCNodes(&q);
			free(q);
			q = NULL; 
			flag = 1;   //注意此处flag应该复原为1
		}
	}
	//当出现只有一个子句，删完就没的情况时
	if(*C==NULL){
		return OK;
	}
	p = *C;
	q = p->down;
	flag = 0;
	//分析除首句以外的子句的情况
	while(q!=NULL){
		m = q->right;
		while(m!=NULL){
			if(m->data==x){
				flag = 1;
				p->down = q->down; 
				FreeCNodes(&q);//释放一个子句 
				free(q);
				q = NULL;
				break;
			}
			m = m->next;
		}
		if(flag==0){
			p = p->down;
			q = q->down;
		}
		else if(flag==1){
			q = p->down;
			flag = 0;
		}
	}
	return OK;
}
status FreeCNodes(CHead **node)
{
    CNode *p = (*node)->right;
    while (p) {
        CNode *next = p->next;
        free(p);
        p = next;
    }
    (*node)->right = NULL;
    return OK;
}


status DelNode(int x, CHead **C)
{
	PHead temp1 = *C;
	PNode temp2 = NULL, p = NULL, q = NULL; 
	while(temp1!=NULL){
		temp2 = temp1->right;
		while(temp2->data==opp(x)){//特殊情况，需要删掉第一个结点
			temp1->right = temp2->next;
			free(temp2);
			temp2 = temp1->right;
			temp1->num--;   //注意子句的变元数目变化
		}
		p = temp2;
		q = p->next;
		while(q!=NULL){
			if(q->data==opp(x)){
				p->next = q->next;
				free(q);
				q = p->next;
				temp1->num--;
			}
			else{
				p = q;
				q = p->next;
			}
		}
		temp1 = temp1->down;
	}
	return OK;	
}

status isEmptyClause(CHead *C)
{
	CHead *temp = C;
	while(temp!=NULL){
		if(temp->num == 0){
			return OK;//有空子句，返回OK 
		}
		temp = temp->down;
	}
	return FALSE;//无空子句，返回FALSE
}

int strategy1(CHead **C)
{
	return (*C)->right->data;
}

int strategy2(CHead **C)
{
	PHead dnow = *C;
	PNode rnow = NULL;
	int x, pos = 0;
	double max = 0.0;
	for(int i=0;i<MAXN && counter[i].data!=0;i++){
		counter[i].count = 0.0;
		counter[i].positive = 0.0;
		counter[i].negative = 0.0;
	}
	while(dnow!=NULL){
		rnow = dnow->right;
		while(rnow!=NULL){
			x = rnow->data;
			counter[abs(x)].count++;
			if(x>0){
				counter[abs(x)].negative++;
			}
			else{
				counter[abs(x)].negative++;
			}
			rnow = rnow->next;
		}
		dnow = dnow->down;
	}
	for(int i=0;i<MAXN && counter[i].data!=0;i++){
		if(counter[i].count>max){
			max = counter[i].count;
			pos = i;
		}
	}
	if(pos==0) pos = (*C)->right->data;//用优化方法没找到决策变元时，使用strategy1 
	if(counter[pos].positive>counter[pos].negative) return pos;//正值频率大于负值，返回正值作为决策变元 
	else return opp(pos);
}

CHead* Duplication(CHead **C)
{
	PHead p = *C, q = NULL,dnow = NULL;//dt用来追踪正本中的子句头结点 
	PNode rnow = NULL, rt = NULL;//rt用来追踪正本中的变元结点 
	while(p!=NULL){
		dnow = IniClause(&q);
		dnow->num = p->num;
		rt = p->right;
		while(rt!=NULL){
			rnow = IniNode(&(dnow->right));
			rnow->data = rt->data;
			rt = rt->next;
		}
		p = p->down;
	}
	return q;
}


status AddUniClause(int x, CHead **C)
{
	PHead ne = NULL;
	ne = (PHead)malloc(sizeof(CHead));
	if(!ne) exit(ERROR);
	ne->down = *C;
	ne->right = NULL;
	*C = ne;
	ne->num = 1;
	PNode node = IniNode(&(ne->right));
	node->data = x;
	counter[abs(x)].count += 1;   //更新计数器里的数据
	if(x>0){
		counter[abs(x)].positive += 1;
	}
	else{
		counter[abs(x)].negative += 1;
	}
	node = IniNode(&(node->next));
	return OK;
}

int StartSAT(char FileName[])
{
	PHead C = NULL;
	int re;
	clock_t start_t, end_t;
	double total_t;
	FileReader(&C, FileName);                                    //读取.cnf文件 
	start_t = clock();                                           //开始计时 
	re = DPLLSolver(&C);                                         //进行DPLL 
	end_t = clock();                                             //结束计时 
	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000; //计时换算
	SaveSATFile(re, total_t, FileName);                          //将DPLL结果存入到.res文件中 
	return OK;
}


//re是用来判断DPLL是否正确进行
status SaveSATFile(int re, double t, char filename[])
{
	int i=0;
	for(i=0; filename[i]!='\n'; i++){//修改拓展名 
		if(filename[i]=='.'){
			filename[i+1] = 'r';
			filename[i+2] = 'e';
			filename[i+3] = 's';
			break;
		}
	}
	FILE *fp;
	fp = fopen(filename, "w");
	if(!fp) exit(-1);
	if(re==TRUE){
		fprintf(fp, "s 1\n");
		fprintf(fp, "v: ");
		for(i=1; counter[i].data!=0 && i<MAXN; i++){
			if(counter[i].value==1){
				fprintf(fp, "%d ", i);
			}
			else if(counter[i].value==-1){
				fprintf(fp, "%d ", -i);
			}
		}
		fprintf(fp, "\n");
	}
	else{
		fprintf(fp, "s 0\n");
	}
	fprintf(fp, "t %f", t);
	fclose(fp);
	return OK;
}
