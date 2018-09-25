#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define WIN 1
#define MAN 1
#define AI 2
#define NOT 0
#define ROW 15 
#define COL 15
int map[ROW][COL];//数组下标为0or15保存边界
void showmap();
void sbai(int* ,int* );
void move(int,int*,int*);
void manvsai();
int  model(int,int ,int );
int win(int ,int );
int money(int ,int );
void hxai(int*,int*);
void scan(int *row);
void scan(int *row,int*col)
{
int n=0;
int m=0;
n= scanf("%d",row);
m= scanf("%d",col);
while (!n&&!m)
{
  scanf("%*[^\n]");
  scanf("%*c");
  printf("输入0~14的数字作为坐标------请重新输入:");
  n = scanf("%d",row);
  m= scanf("%d",col);
}
	scanf("%*[^\n]");
	scanf("%*c");

}

void manvsai()
{	
	int row,col;
	int res;
	printf("五子棋人机对战\n");
	printf("===============\n");
	showmap();
	while(1)
	{
		printf("请输入坐标(如想在第一行的第二格下子,输入：1 2）:\n");	
		scan(&row,&col);	
		while(map[row][col]!=NOT||row<0||row>14||col<0||col>14)
		{
			printf("输入0~14的数字作为坐标，且不能再重复点下子,请重新输入下子坐标:\n");
			scan(&row,&col);
		}
		map[row][col]=MAN;//下子坐标
		res=win(row,col);
		if(res==WIN){ 
		showmap();
		printf("you win!\n");
		break;
					}//判断输赢
		hxai(&row,&col);
		map[row][col]=AI;
		res=win(row,col);
		if(res==WIN)
		{
		showmap();
		printf("computer win!\n");
		break;
		}
		showmap();
	}
}
void showmap()
{
		int i,j;
		char a[ROW][COL][4];//存棋子
		for(i=0;i<ROW;i++)
		{
		for(j=0;j<COL;j++)
		{
			if(map[i][j]==MAN){strcpy(a[i][j],"口\0");}//口是人类
			if(map[i][j]==AI){strcpy(a[i][j],"田\0");}//田是AI
			if(map[i][j]==NOT){strcpy(a[i][j],"  \0");}//空格代表没有棋子
		} 
		}
			printf("	| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11| 12| 13| 14|\n");
			printf("	_____________________________________________________________\n");
			for(i=0,j=0;i<ROW;i++,j++)
			
			{
				printf("%d	| %s| %s| %s| %s| %s| %s| %s| %s| %s| %s| %s| %s| %s| %s| %s|\n",
				j,a[i][0],a[i][1],a[i][2],a[i][3],a[i][4],a[i][5],a[i][6],
				a[i][7],a[i][8],a[i][9],a[i][10],a[i][11],a[i][12],a[i][13],a[i][14]
						);
				printf("	-------------------------------------------------------------\n");
			}	
}
void move(int n,int* row,int* col)
{
	if(0==n){*row=*row-1;}//上,顺时针旋转
	if(1==n){*row=*row-1;*col=*col+1;}
	if(2==n){*col=*col+1;}	
	if(3==n){*row=*row+1;*col=*col+1;}	
	if(4==n){*row=*row+1;}	
	if(5==n){*row=*row+1;*col=*col-1;}	
	if(6==n){*col=*col-1;}	
	if(7==n){*row=*row-1;*col=*col-1;}	
}
int win(int row,int col)
{
	int n=0;
	int i=row,j=col;
	int count=1;//计数器，下棋处本来就有一颗～
	int body = map[row][col];//下的那一颗是谁的？
	if(body==NOT)return 0;//如果此点没下子,直接退出
	while(n<8)
	{
		count=1;
		i=row;j=col;
		while(row>=0&&row<=14&&col>=0&&col<=14)//n方向不是边界
		{
			move(n,&i,&j);
			if(map[i][j]==body)
			{
				count++;
				if(count==5)return WIN;//当计数达到5颗，返回win
			}//如果下一颗是自己的子,计数
			else break;//如果是空或地方，退出这个循环
		}
			n++;//换方向判断
	}
	return 0;
}
int model(int n,int row,int col)
{
	int body;
			move(n,&row,&col);//看看n方向上 下一颗是什么
	      if(row==-1||row==15||col==-1||col==15) return 2;//如果下一颗是边界的话，直接返回2
			body=map[row][col];
			if(body==AI) //如果是自己的子，那么目前_田……
			{
				move(n,&row,&col);//再看看下一颗
				if(row==-1||row==15||col==-1||col==15) return 105;//_田|和_口一样
				body=map[row][col];//是谁的
				if(body==MAN)//如果是人类的或是边界那么n方向上就是棋型_田口 号码105
						return 105;
				if(body==NOT)return 101;//如果是空，那就是棋型_田_ 号码101
				if(body==AI)//如果是自己的
				{	
						move(n,&row,&col);//再看看下一个
						if(row==-1||row==15||col==-1||col==15) return 106;//_田田|
				body=map[row][col];//是谁的
						if(body==MAN)return 106;//如果是人类的子，那就是_田田口 号码为106
						if(body==NOT) return 102;//如果是空，那就是_田田_号码102
						if(body==AI)//如果是自己的
						{
							move(n,&row,&col);//再看看下一个	
							if(row==-1||row==15||col==-1||col==15) return 107;//_田田田|
							body=map[row][col];
							if(body==MAN) return 107; //如果是人的。那就是_田田田口_号码为107	
							if(body==NOT) return 103;//那就是_田田田_，号码为103
							if(body==AI)//再看。。。
							{
								move(n,&row,&col);
								if(row==-1||row==15||col==-1||col==15) return 108;//_田田田田|
								body=map[row][col];
								if(body==MAN) return 108;//如果是人，那就是_田田田田口，号码为108
								if(body==NOT) return 104;//_田田田田_
								if(body==AI)  return WIN;//终于结束了
							}
						}

				}
			}
			if(body==MAN) //如果是人类的子,那就是_口……
			{
				move(n,&row,&col);//再看看下一个是什么
				if(row==-1||row==15||col==-1||col==15) return 205;//_口|
				body=map[row][col];
				if(body==AI) return 205;//如果是自己的，那就是_口田，棋型号为205
				if(body==NOT) return 201;//棋型号为201 _口_
				if(body==MAN) //再继续看
				{
					move(n,&row,&col);
					if(row==-1||row==15||col==-1||col==15) return 206;//_口口|
					body=map[row][col];
					if(body==AI) return 206;//如果是自己的，那就是_口口田，棋型号，206
					if(body==NOT) return 202;//_口口_，棋型号 202
					if(body==MAN)//继续。= =
					{
						move(n,&row,&col);
						if(row==-1||row==15||col==-1||col==15) return 207;//_口口口|
						body=map[row][col];
						if(body==AI) return 207;//_口口口田,棋型号207
						if(body==NOT) return 203;//_口口口_，棋型号203
						if(body==MAN) //继续...
						{
							move(n,&row,&col);
							if(row==-1||row==15||col==-1||col==15) return 208;//_口口口口|
							body=map[row][col];
							if(body==AI) return 208;//_口口口口田 208
							if(body==NOT) return 204;//_口口口口_ 204
							if(body==MAN) return WIN;
						}
					}
				}
		
			}
			if(body==NOT)	//如果是空子
			{
				move(n,&row,&col);
				if(row==-1||row==15||col==-1||col==15) return 0;//__|
				body=map[row][col];
				if(body==NOT) return 0;//不用看是什么棋型了，没有意义
				if(body==MAN)
				{
					 move(n,&row,&col);//再看看下一个是什么
					 if(row==-1||row==15||col==-1||col==15) return 215;//__口|
	             body=map[row][col];
	             if(body==AI) return 215;//是__口田，棋型号为215
	             if(body==NOT) return 211;//棋型号为211 __口_
	             if(body==MAN) //再继续看
	             {
	                move(n,&row,&col);
						 if(row==-1||row==15||col==-1||col==15) return 216;//__口口|
	                body=map[row][col];
	                if(body==AI) return 216;//是__口口田，棋型号，216
	                if(body==NOT) return 212;//__口口_，棋型号 212
	                if(body==MAN)//继续。= =
	                {
	                   move(n,&row,&col);
							 if(row==-1||row==15||col==-1||col==15) return 217;//__口口口|
	                   body=map[row][col];
	                   if(body==AI) return 217;//__口口口田,棋型号217
	                   if(body==NOT) return 213;//__口口口_，棋型号213
	                   if(body==MAN) //继续...
	                   {
	                      move(n,&row,&col);
								 if(row==-1||row==15||col==-1||col==15) return 228;//__口口口口|
	                      body=map[row][col];
	                      if(body==AI) return 228;//__口口口口田 218
	                      if(body==NOT) return 214;//__口口口口_ 214
	                      if(body==MAN) return WIN;
	                   }
	                }
	             }

				}
				if(body==AI)
				{
					move(n,&row,&col);//再看看下一颗
					 if(row==-1||row==15||col==-1||col==15) return 115;//__田|
	             body=map[row][col];//是谁的
	             if(body==MAN) return 115;//那么n方向上就是棋型__田口
	             if(body==NOT)return 111;//棋型__田_ 
	             if(body==AI)//如果是自己的
	             {
	                   move(n,&row,&col);//再看看下一个
							 if(row==-1||row==15||col==-1||col==15) return 116;//__田田|
	                   body=map[row][col];
	                   if(body==MAN)return 116;//是__田田口 
	                   if(body==NOT) return 112;//是__田田_
	                   if(body==AI)//如果是自己的
	                   {
	                      move(n,&row,&col);//再看看下一个 
								 if(row==-1||row==15||col==-1||col==15) return 117;//__田田田|
	                      body=map[row][col];
	                      if(body==MAN) return 117; //__田田田口_
	                      if(body==NOT) return 113;//__田田田_
	                      if(body==AI)//再看。
	                      {
	                         move(n,&row,&col);
									 if(row==-1||row==15||col==-1||col==15) return 118;//_田田田田|
	                         body=map[row][col];
	                         if(body==MAN) return 118;//__田田田田口
	                         if(body==NOT) return 114;//__田田田田_
	                         if(body==AI)  return WIN;//终于结束了
	                      }
	                   }
	 
	            }

				}
				
			}
		return 0;
}
 
void sbai(int* row,int* col)
{
	srand(time(0));
	*row=rand()%15;
	*col=rand()%15;
	while(map[*row][*col]!=NOT)
	{
		*row=rand()%15;
		*col=rand()%15;
	}
	
}
int money(int row,int col)
{
	int bai,shi,ge;//棋型号的3位数
	int value[2][3][8]={100,3000,6000,100000,10,20,3000,100000,10,2500,0,0,10,25,2500,0,0,0,0,0,0,0,0,0,
	                	20,250,4000,24999,10,20,2000,24999,15,15,0,0,10,20,1999,0,0,0,0,0,0,0,0,0};
	//{101,102,103,100000,105,106,107,108,111,112,113,0,115,116,117,0,121,122,123,124,125,126,127,128,201,202 ,203,204,205,206,207,208,211,212,213,214,215,216,217,218,221,222,223,224,225,226,227,228}
	int n,fn;//从0开始，正向4个方向，反向4个方向
	int z;//正方向
	int f;//反方向
	int sum=0;//统计分析各棋型在一个点的联系，算出价值
	for(n=0,fn=4;n<4;n++,fn++)
	{
		z=model(n,row,col);//看看正方向的棋型
		f=model(fn,row,col);//反方向棋型
		//相反方向棋子的第一种情况
		if((z/10==10)&&(f/10==10)&&(z%10<=4)&&(f%10<=4))//如果正反都属于101~104之间的棋型,
		{
			//合并成一个方向的棋型
			bai=1;//bai=0代表百位是1
			shi=0;//shi=0代表十位是0
			if((z%10+f%10)>4) ge=4;//如果个位加起来大于4了，那就当做4
			else ge=z%10+f%10 ;//如果没大于4，直接相加
			sum+=value[bai-1][shi][ge-1];//合并和计算价值
		}
		//相反方向棋子的第二种情况
		else if((z/10==20)&&(f/10==20)&&(z%10<=4)&&(f%10<=4))//201~204之间
		{
				bai=2;//bai=0代表百位是1
	          shi=0;//shi=0代表十位是0
	          if((z%10+f%10)>4) ge=4;//如果个位加起来大于4了，那就当做4
	          else ge=z%10+f%10 ;//如果没大于4，直接相加
	          sum+=value[bai-1][shi][ge-1];//合并和计算价值

		}
		//相反方向棋子的第三种情况
		else if((z/10==10)&&(f/10==10)&&(z%10<=4)&&(f%10>4))//正方向属于101~104，反方向属于105~108
		{
			//合成一个方向的棋型
			bai = 1;
			shi = 0;
			if((z%10+f%10)>8)ge=8;
			else ge=z%10+f%10;
			sum+=value[bai-1][shi][ge-1];
		}
		//相反方向棋子的第四种情况
		else if((z/10==10)&&(f/10==10)&&(z%10>4)&&(f%10<=4))//正方向属于105~108，反方向属于101~104
		{
			//合成一个方向的棋型
			bai = 1;
			shi = 0;
			if((z%10+f%10)>8)ge=8;
			else ge=z%10+f%10;
			sum+=value[bai-1][shi][ge-1];
		}
		//相反方向棋子的第五种情况
		else if((z/10==20)&&(f/10==20)&&(z%10<=4)&&(f%10>4))//正方向属于201~204，反方向属于205~208
		{
			//合成一个方向的棋型
			bai = 2;
			shi = 0;
			if((z%10+f%10)>8)ge=8;
			else ge=z%10+f%10;
			sum+=value[bai-1][shi][ge-1];
		}
		else if((z/10==20)&&(f/10==20)&&(z%10>4)&&(f%10<=4))//正方向属于205~208，反方向属于201~204
		{
			//合成一个方向的棋型
			bai = 2;
			shi = 0;
			if((z%10+f%10)>8)ge=8;
			else ge=z%10+f%10;
			sum+=value[bai-1][shi][ge-1];
		}
			//其他
		else
			{
				if(z==0||z==2) sum+=0;//空棋型和边界棋型
				else
				{
					ge=z%10;
					shi=((z-(ge))/10)%10;
					bai=z/100;
					sum+=value[bai-1][shi][ge-1];
				}
				if(f==2||f==0) sum+=0;
				else
				{
					ge=f%10;
					shi=((f-(ge))/10)%10;
					bai=f/100;
					sum+=value[bai-1][shi][ge-1];
				}
			
	}	
	}
		return sum;
	
}
void hxai(int* row ,int* col)//带回坐标
{
		srand(time(0));
		int i,j;
		int value_max=0;
		int value=0;
		for(i=0;i<15;i++)
		{
			for(j=0;j<15;j++)
			{
				if(map[i][j]==NOT) 
					{
						value = money(i,j);
						if(value>value_max) 
						{	
									*row=i;
									*col=j;
									value_max=value;
						}
					}
					
			}
		}
}

int main()
{
	manvsai();
	return 0;
}