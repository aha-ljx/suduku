//@Sudoku.c
#include "Global.h"

status CreateSudokuToFile(char SudokuFile[], int holes)
{
    int sudoku[ROW][COL]={0};       //声明数独终盘二维数组，并全部初始化为0 
    int play[ROW][COL]={0};         //数独初盘二维数组，并全部初始化为0 
    CreateSudoku(sudoku);           //生成数独终盘 
    CreatePlay(sudoku, play, holes);//生成初盘
    int i, j;
    for(i=0; i<COL; i++){
    	for(j=0; j<ROW; j++){
    		player[i][j] = play[i][j];
		}
	}
    
    //把数独初盘写进CNF文件
	TranToCNF(play, holes, SudokuFile);
	return OK;
}

void CreateSudoku(int rows[][COL])
{
	FirstRow(rows[0]);    //生成数独第一行 
	OtherRows(rows, 1, 0);//递归生成其他行 
	return;
}


/**
* @param   rows（用于存储数独棋盘的二维数组），i（第i行，从0开始数），j（第j列，从0开始数） 
* @return  status（函数执行状态） 
* @brief   利用递归给第i行第j列位置赋上一个数字，直至生成整个数独棋盘 
* @detail  check[COL+1]，值为WA时表示下标数字已经被同行或同列或同宫使用过，值为AC表示未使用过
* @todo     
*/
status OtherRows(int rows[][COL], int i, int j){
	if(i<ROW && j<COL){
		int x, y, k;
		int check[COL+1] = {AC};                  
		for(x=0; x<i; x++) check[rows[x][j]] = WA;//标记同列已使用过的数字 
		for(x=0; x<j; x++) check[rows[i][x]] = WA;//标记同行已使用过的数字 
		//标记同宫使用过的数字
		int bx=i/3,by=j/3;
		for(x= bx*3;x <= i;x++){
			if(x == i){
				for(y = by*3;y<= j;y++){
					check[rows[x][y]] = WA;
				}
			}
			else{
				for(y = by*3;y<= by*3+2;y++){
					check[rows[x][y]] = WA;
				}
			}
		}
		//标记撇对角线使用过的数字
		if(i+j == 8){
			for(x=0;x<i;x++){
				check[rows[x][8-x]] = WA;
			}
		}
		//标记百分号区域使用过的数字
		if(i>=1 && i<=3 && j>=1 && j<=3){
			for(x= 1;x <= i;x++){
			if(x == i){
				for(y = 1;y<= j;y++){
					check[rows[x][y]] = WA;
				}
			}
			else{
				for(y = 1;y<= 3;y++){
					check[rows[x][y]] = WA;
				}
			}
		}
		}
		if(i>=5 && i<=7 && j>=5 && j<=7){
			for(x= 5;x <= i;x++){
			if(x == i){
				for(y = 5;y<= j;y++){
					check[rows[x][y]] = WA;
				}
			}
			else{
				for(y = 5;y<= 7;y++){
					check[rows[x][y]] = WA;
				}
			}
		}
		}

		int flag = 0;
		for(k=1; k<=COL && flag==0; k++){
			if(check[k]==AC){
				flag = 1;
				rows[i][j] = k;
				if(j==COL-1 && i!=ROW-1){//如果ij是一行的最后一列且不是最后一行，递归进入下一行 
					if(OtherRows(rows, i+1, 0)==AC) return AC;
					else flag = 0;
				}
				else if(j!=COL-1){//如果不是最后一列，递归进入下一列 
					if(OtherRows(rows, i, j+1)==AC) return AC;
					else flag = 0;
				}
			}
		}
		if(flag==0){
			rows[i][j] = 0;
			return WA;
		}
	}
	return AC;
} 


/**
* @param   firstrow
* @return  void
* @brief  利用当前系统时间随机生成数独棋盘第一行 
*/
void FirstRow(int firstrow[])
{
	int i, j;
	srand((unsigned int)time(NULL));
	for(i=0; i<COL; i++){
		firstrow[i] = rand() % 9 + 1;
		j = 0;
		while(j<i){//保证新的赋值不等于已有的数字
			if(firstrow[j]==firstrow[i]){
				firstrow[i] = rand() % 9 + 1;
				j = 0;
			}
			else{
				j++;
			}
		}
	}
}

void CreatePlay(int a[][COL], int b[][COL], int numDigits)
{
    int i, j, k;
    srand((unsigned)time(NULL));//用当前系统时间生成随机数种子 
    for(i=0; i<ROW; i++)        //对数独终盘数组进行复制 
        for(j=0; j<COL; j++)
            b[i][j] = a[i][j];

    int c[numDigits][2];  //记录每个洞的行列坐标值
    int m, flag = 0;

    for(i=0; i<numDigits; i++){//随机选择位置 
        j = rand() % 9;
        k = rand() % 9;

        flag = 0;
        for(m=0; m<i; m++){         //检查是否挖了重复的洞
            if(j==c[m][0] && k==c[m][1]){
                flag = 1;
			}
		}
        if(flag==0){    //记录洞坐标
            b[j][k] = 0;//挖洞
            c[i][0] = j;
            c[i][1] = k;
        }
        else
            i--;
    }
}

/**
* @param   a(数独初盘二维数组), holes（挖洞数），SudokuFile（文件名） 
* @return  void
* @brief   把数独初盘写进CNF文件 
*/
status TranToCNF(int a[][COL],int holes, char SudokuFile[])
{
FILE *fp = fopen(SudokuFile, "w");
	if(!fp) exit(ERROR);
	fprintf(fp, "p cnf 729 %d\n", 6642+81+333+333+333-holes);
	int x, y, z;
	for(x=0; x<ROW; x++){//play盘的已知信息作为单子句加入子句集
		for(y=0; y<COL; y++){
			if(a[x][y]!=0){
				fprintf(fp, "%d 0\n", (x+1)*100 + (y+1)*10 + a[x][y]);
			}
		}
	} 
	for(x=1; x<=9; x++){//每个格子都可能填1-9的任意一个数字 
		for(y=1; y<=9; y++){
			for(z=1; z<=9; z++){
				fprintf(fp, "%d ", x*100 + y*10 + z);
			}
			fprintf(fp, "0\n");
		}
	}
	int i, j, k;
	int x2,y2;
	//列约束
	for(y=1; y<=9; y++){ 
		for(z=1; z<=9; z++){
			for(x=1; x<=8; x++){
				for(i=x+1; i<=9; i++){
					fprintf(fp, "%d ", opp(x*100 + y*10 +z));
					fprintf(fp, "%d 0\n", opp(i*100 + y*10 + z));
				}
			}
		}
	}
	
	//行约束 
	for(x=1; x<=9; x++){
		for(z=1; z<=9; z++){
			for(y=1; y<=8; y++){
				for(i=y+1; i<=9; i++){
					fprintf(fp, "%d ", opp(x*100 + y*10 +z));
					fprintf(fp, "%d 0\n", opp(x*100 + i*10 + z));
				}
			}
		}
	}
	
	//宫约束 
	for(z=1; z<=9; z++){
		for(i=0; i<=2; i++){
			for(j=0; j<=2; j++){
				for(x=1; x<=3; x++){
					for(y=1; y<=3; y++){
						for(k=y+1; k<=3; k++){
							fprintf(fp, "%d ", opp((3*i+x)*100 + (3*j+y)*10 + z));
							fprintf(fp, "%d 0\n", opp((3*i+x)*100 + (3*j+k)*10 + z));
						}
						
					}
				}
			}
		}
	}
	int m;
	for(z=1; z<=9; z++){
		for(i=0; i<=2; i++){
			for(j=0; y<=2; j++){
				for(x=1; x<=3; x++){
					for(y=1; y<=3; y++){
						for(k=x+1; k<=3; k++){
							for(m=1; m<=3; m++){
								fprintf(fp, "%d ", opp((3*i+x)*100 + (3*j+y)*10 + z));
								fprintf(fp, "%d 0\n", opp((3*i+k)*100 + (3*j+m)*10 + z));
							}
						}
					}
				}
			}
		}
	}
	//撇对角线约束
	for(z=1;z<=9;z++){
		for(x=1,y=9;x<=9;x++,y--){
			fprintf(fp,"%d ",x*100+y*10+z);
		}
		fprintf(fp,"0\n");
	}
	for(z=1;z<=9;z++){
		for(x=1,y=9;x<=8;x++,y--){
			for(i=x+1,j=10-i;i<=9;i++,j--){
				fprintf(fp,"%d ",opp(x*100+y*10+z));
				fprintf(fp,"%d 0\n",opp(i*100+j*10+z));
			}
		}
	}
	//上下窗口约束
	for(z=1;z<=9;z++){
		for(x=2;x<=4;x++){
			for(y=2;y<=4;y++){
				fprintf(fp,"%d ",x*100+y*10+z);
			}
		}
		fprintf(fp,"0\n");
	}
	for(z=1;z<=9;z++){
		for(x=2;x<=4;x++){
			for(y=2;y<=4;y++){
				int v1=x*100+y*10+z;
				for(x2=x;x2<=4;x2++){
					int y_start = (x2==x) ? y+1 : 2;
					for(y2=y_start;y2<=4;y2++){
						int v2=x2*100+y2*10+z;
						fprintf(fp,"%d %d 0\n",opp(v1),opp(v2));
					}
				}
			}
		}
	}
	for(z=1;z<=9;z++){
		for(x=6;x<=8;x++){
			for(y=6;y<=8;y++){
				fprintf(fp,"%d ",x*100+y*10+z);
			}
		}
		fprintf(fp,"0\n");
	}
    for(z=1;z<=9;z++){
		for(x=6;x<=8;x++){
			for(y=6;y<=8;y++){
				int v1=x*100+y*10+z;
				for(x2=x;x2<=8;x2++){
					int y_start = (x2==x) ? y+1 : 6;
					for(y2=y_start;y2<=8;y2++){
						int v2=x2*100+y2*10+z;
						fprintf(fp,"%d %d 0\n",opp(v1),opp(v2));
					}
				}
			}
		}
	}
	fclose(fp);
	return OK;
}


/**
* @param  CHead **C（链表头结点指针的指针） 
* @return 函数执行结果 
* @brief  从主函数进入Sudoku的入口函数  
*/
status EntrySUDOKU(CHead **C)
{
	int re;
	//clock_t start_t, end_t;
	//double total_t;
	//start_t = clock();
	re = DPLLSolver(C);
	//end_t = clock();
	//total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;//计时换算
	/*if(re==OK){
		int i, j, k;
        for (i = 1; i <= 9; ++i){
            for (j = 1; j <= 9; ++j){
                for (k = 1; k <= 9; ++k) {
                    int var = i*100 + j*10 + k;
                    if (counter[var].value == 1) {   // 该变元为真
                        player[i-1][j-1] = k;        // 回填到棋盘
                        break;                       // 每个格子只会有一个真值
                    }
	}
		}
		}
}*/
    return re;
}




/**
* @param   a（数独棋盘的二维数组） 
* @return  void
* @brief  在终端上输出数独棋盘 
*/
void print(int a[][COL])
{
    int i, j;
    printf("     1  2  3    4  5  6    7  8  9\n");
    for(i=0; i<ROW; i++){
    	if(i%3==0) printf("   ----------------------------------\n");
    	printf("%d  ", i+1); 
        for(j=0; j<COL ; j++){
        	if(j%3==0) printf("| ");
        	if(a[i][j]!=0) printf("%d  ", a[i][j]);
        	else printf("_  ");
		}
		if(i%3==2) printf("|\n");
		else{
			printf("|\n");
        	printf("   |          |          |          |\n");
		}
    }
    printf("   ----------------------------------\n");
}
