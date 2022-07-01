// 亚马逊棋（Amazons）
//正式版 

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include<cstring>
#include<cmath>

#define GRIDSIZE 8
#define OBSTACLE 2000
#define grid_black 1000
#define grid_white -1000
#define KINGMOVE 9
#define QUEENMOVE 3

using namespace std;

int currBotColor; // 我所执子颜色（1000为黑，-1000为白，棋盘状态亦同）
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // 先x后y，记录棋盘状态
int tking[GRIDSIZE][GRIDSIZE];
int tking2[GRIDSIZE][GRIDSIZE];
int tqueen[GRIDSIZE][GRIDSIZE]; 
int tqueen2[GRIDSIZE][GRIDSIZE]; 
int dx[] = { -1,-1,-1,0,0,1,1,1 };
int dy[] = { -1,0,1,-1,1,-1,0,1 };
int turnID;//回合数 
double f[5000];//判别函数值 
double advan=-0.2;//先手优势
double f1[30]={0.1080,0.1080,0.1235,0.1332,0.1400,0.1468,0.1565,0.1720,0.1949,0.2217,0.2476,0.2680,0.2800,0.2884,0.3000,0.3208,0.3535,0.4000,0.4613,0.5350,0.6181,0.7075,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000,0.8000};
double f2[30]={0.3940,0.3940,0.3826,0.3753,0.3700,0.3647,0.3574,0.3460,0.3294,0.3098,0.2903,0.2740,0.2631,0.2559,0.2500,0.2430,0.2334,0.2200,0.2020,0.1800,0.1550,0.1280,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000,0.1000};
double f3[30]={0.1160,0.1160,0.1224,0.1267,0.1300,0.1333,0.1376,0.1440,0.1531,0.1640,0.1754,0.1860,0.1944,0.1995,0.2000,0.1950,0.1849,0.1700,0.1510,0.1287,0.1038,0.0773,0.0500,0.0500,0.0500,0.0500,0.0500,0.0500,0.0500,0.0500};
double f4[30]={0.1160,0.1160,0.1224,0.1267,0.1300,0.1333,0.1376,0.1440,0.1531,0.1640,0.1754,0.1860,0.1944,0.1995,0.2000,0.1950,0.1849,0.1700,0.1510,0.1287,0.1038,0.0773,0.0500,0.0500,0.0500,0.0500,0.0500,0.0500,0.0500,0.0500};

inline bool inMap(int x, int y); 
bool ProcStep(int x0, int y0, int x1, int y1, int x2, int y2, int color, bool check_only);
int king();
int king2();
int queen();
int queen2();
double t1();
double t2();
double p1();
double p2();
//double m();

int main()
{
	int x0, y0, x1, y1, x2, y2;

	// 初始化棋盘
	gridInfo[0][(GRIDSIZE - 1) / 3] = gridInfo[(GRIDSIZE - 1) / 3][0]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
		= gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
	gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;


	cin >> turnID;
	// 读入到当前回合为止，自己和对手的所有行动，从而把局面恢复到当前回合
	currBotColor = grid_white; // 先假设自己是白方
	for (int i = 0; i < turnID; i++)
	{
		// 根据这些输入输出逐渐恢复状态到当前回合

		// 首先是对手行动
		cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
		if (x0 == -1)
			currBotColor = grid_black; // 第一回合收到坐标是-1, -1，说明我是黑方
		else
			ProcStep(x0, y0, x1, y1, x2, y2, -currBotColor, false); // 模拟对方落子

																	// 然后是自己当时的行动
																	// 对手行动总比自己行动多一个
		if (i < turnID - 1)
		{
			cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
			if (x0 >= 0)
				ProcStep(x0, y0, x1, y1, x2, y2, currBotColor, false); // 模拟己方落子
		}
	}
	
/*	king();
	king2();
	queen();
	queen2();
	cout<<m()<<endl;*/
	
	// 做出决策（你只需修改以下部分）

	// 这里枚举了所有可能的下法，以便之后随机用……

	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];
	int posCount = 0, choice;
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if(gridInfo[i][j]==currBotColor){
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))
								break;
							if (gridInfo[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;
							if (ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, true))
							{
								gridInfo[i][j] = 0;
								gridInfo[xx][yy] = currBotColor;
								gridInfo[xxx][yyy] = OBSTACLE;
								
								beginPos[posCount][0] = i;
								beginPos[posCount][1] = j;
								possiblePos[posCount][0] = xx;
								possiblePos[posCount][1] = yy;
								obstaclePos[posCount][0] = xxx;
								obstaclePos[posCount][1] = yyy;
								king();
								king2();
								queen();
								queen2();
								f[posCount]=t1()*f1[turnID-1]+t2()*f2[turnID-1]+p1()*f3[turnID-1]+p2()*f4[turnID-1];
								
								gridInfo[xxx][yyy] = 0;
								gridInfo[xx][yy] = 0;
								gridInfo[i][j] = currBotColor;
								posCount++;
							}
						}

					}
				}
			} 
			}
		}
	}

	int startX, startY, resultX, resultY, obstacleX, obstacleY;
	if (posCount > 0)
	{
		double x=-100;
		for(int i=0;i<posCount;i++)
		{
			if(f[i]>x)x=f[i];
		}
		//cout<<x<<endl;
		for(int i=0;i<posCount;i++)
		{
			if(f[i]==x)
			{
				choice=i;
				break;
			}
		}
		startX = beginPos[choice][0];
		startY = beginPos[choice][1];
		resultX = possiblePos[choice][0];
		resultY = possiblePos[choice][1];
		obstacleX = obstaclePos[choice][0];
		obstacleY = obstaclePos[choice][1];
	}
	else
	{
		startX = -1;
		startY = -1;
		resultX = -1;
		resultY = -1;
		obstacleX = -1;
		obstacleY = -1;
	}

	// 决策结束，输出结果（你只需修改以上部分）
	cout << startX << ' ' << startY << ' ' << resultX << ' ' << resultY << ' ' << obstacleX << ' ' << obstacleY << endl;
	return 0;
}





// 判断是否在地图内
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}

// 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int x0, int y0, int x1, int y1, int x2, int y2, int color, bool check_only)
{
	if ((!inMap(x0, y0)) || (!inMap(x1, y1)) || (!inMap(x2, y2)))
		return false;
	if (gridInfo[x0][y0] != color || gridInfo[x1][y1] != 0)
		return false;
	if ((gridInfo[x2][y2] != 0) && !(x2 == x0 && y2 == y0))
		return false;
	if (!check_only)
	{
		gridInfo[x0][y0] = 0;
		gridInfo[x1][y1] = color;
		gridInfo[x2][y2] = OBSTACLE;
	}
	return true;
}

//己方王移最少步数
int king() 
{
	memset(tking,0,sizeof(tking));
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			tking[i][j]=gridInfo[i][j];
		}
	}
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(tking[i][j]==currBotColor)
			{
				for (int k = 0; k < 8; ++k)
				{
					if(inMap(i+dx[k],j+dy[k])&&tking[i+dx[k]][j+dy[k]]==0)tking[i+dx[k]][j+dy[k]]=1;
				}
			}
		}
	}
	for (int l = 2; l<=KINGMOVE; ++l) {
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(tking[i][j]==l-1)
			{
				for (int k = 0; k < 8; ++k)
				{
					if(inMap(i+dx[k],j+dy[k])&&tking[i+dx[k]][j+dy[k]]==0)tking[i+dx[k]][j+dy[k]]=l;
				}
			}
		}
	}
	}
	return 0;
}

//敌方王移最少步数
int king2() 
{
	memset(tking2,0,sizeof(tking2));
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			tking2[i][j]=gridInfo[i][j];
		}
	}
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(tking2[i][j]==-currBotColor)
			{
				for (int k = 0; k < 8; ++k)
				{
					if(inMap(i+dx[k],j+dy[k])&&tking2[i+dx[k]][j+dy[k]]==0)tking2[i+dx[k]][j+dy[k]]=1;
				}
			}
		}
	}
	for (int l = 2; l<=KINGMOVE; ++l) {
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(tking2[i][j]==l-1)
			{
				for (int k = 0; k < 8; ++k)
				{
					if(inMap(i+dx[k],j+dy[k])&&tking2[i+dx[k]][j+dy[k]]==0)tking2[i+dx[k]][j+dy[k]]=l;
				}
			}
		}
	}
	}
	return 0;
}

//己方后移最少步数
int queen() 
{
	memset(tqueen,0,sizeof(tqueen));
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			tqueen[i][j]=gridInfo[i][j];
		}
	}
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(tqueen[i][j]==currBotColor)
			{
				for (int k = 0; k < 8; ++k)
				{
					for(int delta=1;delta<GRIDSIZE;++delta)
					{
						int x=i+delta*dx[k],y=j+delta*dy[k];
						if(inMap(x,y)&&(tqueen[x][y]==0||tqueen[x][y]==1))tqueen[x][y]=1;
						else break;
					}	
				}
			}
		}
	}
	for (int l = 2; l<=QUEENMOVE; ++l) {
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(tqueen[i][j]==l-1)
			{
				for (int k = 0; k < 8; ++k)
				{
					for(int delta=1;delta<GRIDSIZE;++delta)
					{
						int x=i+delta*dx[k],y=j+delta*dy[k];
						if(inMap(x,y)&&tqueen[x][y]>=0&&tqueen[x][y]<10)
						{
							if(tqueen[x][y]==0)tqueen[x][y]=l;
						}
						else break;
					}	
				}
			}
		}
	}
	}
	return 0;
}

//敌方后移最少步数
int queen2() 
{
	memset(tqueen2,0,sizeof(tqueen2));
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			tqueen2[i][j]=gridInfo[i][j];
		}
	}
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(tqueen2[i][j]==-currBotColor)
			{
				for (int k = 0; k < 8; ++k)
				{
					for(int delta=1;delta<GRIDSIZE;++delta)
					{
						int x=i+delta*dx[k],y=j+delta*dy[k];
						if(inMap(x,y)&&(tqueen2[x][y]==0||tqueen2[x][y]==1))tqueen2[x][y]=1;
						else break;
					}	
				}
			}
		}
	}
	for (int l = 2; l<=QUEENMOVE; ++l) {
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(tqueen2[i][j]==l-1)
			{
				for (int k = 0; k < 8; ++k)
				{
					for(int delta=1;delta<GRIDSIZE;++delta)
					{
						int x=i+delta*dx[k],y=j+delta*dy[k];
						if(inMap(x,y)&&tqueen2[x][y]>=0&&tqueen2[x][y]<10)
						{
							if(tqueen2[x][y]==0)tqueen2[x][y]=l;
						}
						else break;
					}	
				}
			}
		}
	}
	}
	return 0;
}

//后控制权判断函数 
double t1()
{
	double x=0;
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(gridInfo[i][j]==0)
			{
				if(tqueen[i][j]!=0&&tqueen2[i][j]!=0)
				{
					if(tqueen[i][j]<tqueen2[i][j])x=x+1;
					else if(tqueen[i][j]>tqueen2[i][j])x=x-1;
					else x=x+advan;
				}
				else
				{
					if(tqueen[i][j]==0)x=x-1;
					if(tqueen2[i][j]==0)x=x+1;
				}
			}
		}
	}
	return x;
}

//王控制权判断函数
double t2()
{
	double x=0;
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(gridInfo[i][j]==0)
			{
				if(tking[i][j]!=0&&tking2[i][j]!=0)
				{
					if(tking[i][j]<tking2[i][j])x=x+1;
					else if(tking[i][j]>tking2[i][j])x=x-1;
					else x=x+advan;
				}
				else
				{
					if(tking[i][j]==0)x=x-1;
					if(tking2[i][j]==0)x=x+1;
				}
			}
		}
	}
	return x;
}

//后地理优势判断函数
double p1()
{
	double x=0;
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(gridInfo[i][j]==0)
			{
				if(tqueen[i][j]!=0&&tqueen2[i][j]!=0)
				x=x+2*(pow(2.0,-1.0*tqueen[i][j])-pow(2.0,-1.0*tqueen2[i][j]));
				else
				{
					if(tqueen[i][j]!=0)x=x+2*pow(2.0,-1.0*tqueen[i][j]);
					if(tqueen2[i][j]!=0)x=x-2*pow(2.0,-1.0*tqueen[i][j]);
				}
			}
		}
	}
	return x;
}

//王地理优势判断函数
double p2()
{
	double x=0;
	for (int i = 0; i < GRIDSIZE; ++i) 
	{
		for (int j = 0; j < GRIDSIZE; ++j)
		{
			if(gridInfo[i][j]==0)
			{
				if(tking[i][j]!=0&&tking2[i][j]!=0)
				x=x+min(1.0,max(-1.0,(1.0*tking2[i][j]-tking[i][j])/6.0));
				else
				{
					if(tking[i][j]!=0)x=x+1;
					if(tking2[i][j]!=0)x=x-1;
				}
			}
		}
	}
	return x;
}

//灵活度判断函数 
/*
double m()
{
	
}*/


