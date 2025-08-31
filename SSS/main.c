//@main.c
#include"Global.h"
#include "FileReader.c"
#include "DPLLSolver.c"
#include "Sudoku.c"
int main()
{
    int op = 1, choice = 1;                                                         //菜单界面选项变量的声明，初始化为1来进入循环
	int holes = 0;                                                                  //数独的挖洞数 
    int pd, re, uni = 111, flag = 1, flagg = 1,i, j, k;                 //用于条件判断或者循环 
    PHead C = NULL, temp1 = NULL;                                     //声明并初始化链表头结点
    char FileName[200];
    char SudokuFileName[200] = "sudu.cnf";
    while(op){                                                               //进入一级菜单选择Sat或Sudoku 
    	system("cls");                                                       //刷屏 
	    printf("\n\n");
	    printf("       Welcome to My System!\n");
	    printf(" -------------------------------\n");
	    printf(" |   1. Sat     |   2. Sudoku  |\n");
	    printf(" -------------------------------\n");
	    printf(" |   0. Exit    |              |\n");
	    printf(" -------------------------------\n");
	    printf(" 请输入你的选择[0~2]：");
	    scanf("%d", &op);
	    switch(op){
	    	case 1:                                                          //op=1，准备执行Sat 
	    		printf("请输入文件名：\n");
	    		scanf("%s", FileName);                                       //获取CNF文件名 
	    		if((pd=StartSAT(FileName))==OK){                             //开始基于DPLL算法执行Sat 
	    			printf("成功！请进入对应文件中查看！\n\n");
	    			printf("请按回车键回到主菜单！\n");
				}
	    		getchar();getchar();
				break;
			case 2:
				printf("请输入挖洞数：\n");                                 //op=2，准备执行Sudoku 
				scanf("%d", &holes);                                        //获取挖洞数 
				for(i=0; i<COL; i++){                                       //将初盘二维数组全部初始化为0 
					for(j=0; j<ROW; j++){
						player[i][j] = 0;
					}
				}
				CreateSudokuToFile(SudokuFileName, holes);                  //创建初始棋盘，并写入CNF文件 
				FileReader(&C, SudokuFileName);                             //读取生成的数独CNF文件，生成链表
				print(player);                                              //打印棋盘 
				printf("\n\n");                                             //打印二级菜单，选择answer或者play 
				printf("        Welcome to My Sudoku!\n");          
			    printf(" -------------------------------------\n");
			    printf(" |   1. Answer     |   2. Play       |\n");
			    printf(" -------------------------------------\n");
			    printf(" |   3. Go Back    |   0. Exit       |\n");
			    printf(" -------------------------------------\n");
			    printf(" 请输入你的选择[0~3]：");
			    scanf("%d", &op);
			    while(op && op!=3){
			    	switch(op){
				    	case 1:
				    		re = EntrySUDOKU(&C);                           //对数独CNF文件进行读取，DPLL求解 
				    		pd = SaveSudokuFile(re, SudokuFileName);        //将数独求解结果写到.res文件 
				    		if(pd==OK){
								print(player);
								printf("请按回车键进入主菜单！\n");
							}
							else{
								printf("%d",re);
								printf("该数独棋盘无解！\n");
							}
							getchar();getchar();
							system("cls");
							printf("\n");
							printf("        Welcome to My Sudoku!\n");
						    printf(" -------------------------------------\n");
						    printf(" |   1. Answer     |   2. Play       |\n");
						    printf(" -------------------------------------\n");
						    printf(" |   3. Go Back    |   0. Exit       |\n");
						    printf(" -------------------------------------\n");
						    printf(" 请输入你的选择[0~3]：");
						    scanf("%d", &op);
							getchar();
							break;
						case 2:
							while(choice==1 && holes>0){
								if(flagg){
									flagg = 0;
									system("cls");
								}
								if(flag) print(player);
								printf("请按顺序输入行号、列号、数字（如ijk表示在数独第i行第j列填入k）：\n");
								while(1){
									scanf("%d", &uni);
									i = uni / 100;
									j = uni % 100 / 10;
									k = uni % 10;
									if(i<=9 && i>=1 && j<=9 && j>=1 && k<=9 && k>=1 && player[i-1][j-1]==0) break;
									else{
										printf("输入不合法/该位置已填上数字，请重新输入！\n\n");
										printf("请按顺序输入行号、列号、数字（如ijk表示在数独第i行第j列填入k）：\n");
									} 
								}
								system("cls");
								temp1 = Duplication(&C);
								AddUniClause(uni, &temp1);
								pd = EntrySUDOKU(&temp1);
								if(pd==OK){
									AddUniClause(uni, &C);
									player[i-1][j-1] = k;
									holes--;
									flag = 0;
									print(player);
									if(holes==0){
										system("cls");
										print(player);
										getchar();
										break;
									}
									else printf("正确！\n继续请按'1'，退出请按'0'：\n");
									while(1){
										scanf("%d", &choice);
										if(choice==0 || choice==1) break;
									} 
								}
								else{
									flag = 1;
									printf("错误！\n重新输入请按'1'，退出请按'0'：\n");
									while(1){
										scanf("%d", &choice);
										if(choice==0 || choice==1) break;
									} 
								}
							}
							if(choice==0) op = 3;
							if(choice==1){
								op = 3;
								printf("Congratulations! You have done it!\n请按回车键回到主菜单！\n\n");
								getchar();
								break;
							} 
							getchar();
							break;
						case 3:
							getchar();getchar();
							break;
						case 0:
							break;
						default:
							printf("输入非法！请按回车键重新选择！\n");
							getchar();getchar();
							break;
					}//end of switch(interior)
				}//end of while(interior)
			case 0:
				break;
			default:
				printf("输入非法！请按回车键重新选择！\n");
				getchar();getchar();
				break;
		}//end of switch
    }//end of while
    printf("欢迎下次再使用本系统！\n");
    return 0;
}//end of main