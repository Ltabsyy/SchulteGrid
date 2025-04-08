#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <graphics.h>

int difficulty = 5;
int sideLength = 96;
int board[10][10];
int hardMode = 2;

#define heightOfChar sideLength*3/4
#define yOfChar sideLength/8

void DrawBlock(int r, int c, int n, int highlight)
{
	setcolor(BLACK);
	if(highlight) setfillcolor(GRAY);
	else setfillcolor(LIGHTGRAY);
	ege_fillrect(c*sideLength+sideLength/16, r*sideLength+sideLength/16, sideLength*7/8, sideLength*7/8);
	if(n <= 0);
	else if(n < 10) xyprintf(c*sideLength+sideLength*5/16, r*sideLength+yOfChar, "%d", n);
	else if(n < 100) xyprintf(c*sideLength+sideLength/8, r*sideLength+yOfChar, "%d", n);
	else xyprintf(c*sideLength-sideLength/16, r*sideLength+yOfChar, "%d", n);
}

void DrawSelection(int rm, int cm)
{
	int r, c;
	cleardevice();
	for(r=0; r<5; r++)
	{
		for(c=2; c<10; c++)
		{
			DrawBlock(r, c, c+1, (r == rm && c == cm));
		}
	}
	setfont(sideLength*5/8, 0, "Consolas");
	xyprintf(sideLength/16+sideLength/16, 0*sideLength+sideLength*3/16, "Lowest");
	xyprintf(sideLength*3/8+sideLength/16, 1*sideLength+sideLength*3/16, "Easy");
	xyprintf(sideLength/16+sideLength/16, 2*sideLength+sideLength*3/16, "Normal");
	xyprintf(sideLength*3/8+sideLength/16, 3*sideLength+sideLength*3/16, "Hard");
	xyprintf(sideLength*7/32+sideLength/16, 4*sideLength+sideLength*3/16, "Blind");
	setfont(sideLength*3/4, 0, "Consolas");
}

void DrawLineA(int x0, int y0, int r, int angle)//绘制时钟指针
{
	float rad;
	int x1, y1;
	rad = angle*PI/180;
	x1 = x0 + r*cos(rad);
	y1 = y0 + r*sin(rad);
	ege_line(x0, y0, x1, y1);
}

void DrawClock(int x0, int y0, int r)//绘制时钟
{
	int second, minute, hour;
	int t = time(0);
	// 获取时间
	second = t%60;
	minute = t/60;
	hour = minute/60+8;
	minute = minute%60;
	hour = hour%24;
	// 绘制时钟(r=20)
	setfillcolor(WHITE);
	ege_fillellipse(x0-r, y0-r, 2*r, 2*r);
	setlinewidth(r/10);
	setcolor(BLACK);
	//circle(x0, y0, r);
	ege_ellipse(x0-r, y0-r, 2*r, 2*r);
	//秒针
	setlinewidth(r/20);
	setcolor(RED);
	DrawLineA(x0, y0, r*4/5, 270+second*6);
	setcolor(BLACK);
	//分针
	setlinewidth(r/20);
	DrawLineA(x0, y0, r*3/4, 270+minute*6);
	//时针
	setlinewidth(r/10);
	DrawLineA(x0, y0, r/2, 270+hour%12*30+minute/12*6);//每12分钟跳一格
	//转轴
	setfillcolor(RED);
	ege_fillellipse(x0-r/10, y0-r/10, r/5, r/5);
}

void DrawBoard(int rm, int cm, int mstime, int right, int wrong)
{
	int r, c;
	cleardevice();
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			if(hardMode == 0)
			{
				if((board[r][c] >= right+1 && board[r][c] <= right+3) || right == difficulty*difficulty)
				{
					DrawBlock(r+1, c, board[r][c], (r == rm && c == cm));
				}
			}
			else if(hardMode == 1)
			{
				if(board[r][c] >= right+1 || right == difficulty*difficulty)
				{
					DrawBlock(r+1, c, board[r][c], (r == rm && c == cm));
				}
			}
			else if(hardMode == 4)
			{
				if(right == 0 || right == difficulty*difficulty)
				{
					DrawBlock(r+1, c, board[r][c], (r == rm && c == cm));
				}
				else
				{
					DrawBlock(r+1, c, 0, (r == rm && c == cm));
				}
			}
			else
			{
				DrawBlock(r+1, c, board[r][c], (r == rm && c == cm));
				//DrawBlock(r+1, c, r*difficulty+c+1, (r == rm && c == cm));
			}
		}
	}
	//用时
	DrawClock(sideLength/2, sideLength/2, sideLength*10/32);
	setcolor(BLACK);
	if(right == difficulty*difficulty)
	{
		if(mstime < 10*1000) xyprintf(sideLength, yOfChar, "%.1f", (float)mstime/1000);
		else xyprintf(sideLength, yOfChar, "%d", (mstime+500)/1000);
	}
	else
	{
		xyprintf(sideLength, yOfChar, "%d", mstime/1000);
	}
	//正确进度
	if(right == difficulty*difficulty)
	{
		setcolor(LIME);
		setfillcolor(LIME);
	}
	else
	{
		setcolor(GREEN);
		setfillcolor(GREEN);
	}
	setlinewidth(sideLength/16);
	ege_line(2*sideLength+sideLength/4, sideLength/2, 2*sideLength+sideLength/2, sideLength*3/4);
	ege_line(2*sideLength+sideLength/2, sideLength*3/4, 2*sideLength+sideLength*3/4, sideLength/4);
	ege_fillcircle(2*sideLength+sideLength/2, sideLength*3/4, sideLength/32);
	setcolor(BLACK);
	xyprintf(3*sideLength, yOfChar, "%d", right);
	//错误数
	setcolor(RED);
	setlinewidth(sideLength/16);
	ege_line(4*sideLength+sideLength/4, sideLength/4, 4*sideLength+sideLength*3/4, sideLength*3/4);
	ege_line(4*sideLength+sideLength/4, sideLength*3/4, 4*sideLength+sideLength*3/4, sideLength/4);
	setcolor(BLACK);
	xyprintf(5*sideLength, yOfChar, "%d", wrong);
}

void InitWindow(int mode)
{
	int screenHeight, screenWidth;
	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);
	if(EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) == 0)//无法获取显示屏分辨率
	{
		screenWidth = 1920;
		screenHeight = 1080;
	}
	else
	{
		screenWidth = dm.dmPelsWidth;
		screenHeight = dm.dmPelsHeight;
	}
	if(mode == 0)
	{
		if(screenHeight >= 2160) sideLength = 128;
		else if(screenHeight >= 1440) sideLength = 88;
		else if(screenHeight >= 1080) sideLength = 64;
		else sideLength = 48;
		setcaption("Schulte Grid");
		SetProcessDPIAware();
		initgraph(10*sideLength, 5*sideLength, INIT_RENDERMANUAL);
		setbkcolor(WHITE);
		setfont(heightOfChar, 0, "Consolas");
		setbkmode(TRANSPARENT);
		ege_enable_aa(true);
	}
	else
	{
		while(difficulty*sideLength > screenWidth || (difficulty+2)*sideLength > screenHeight)
		{
			sideLength -= 8;
		}
		if(sideLength < 24) sideLength = 24;
		if(difficulty < 6) resizewindow(6*sideLength, (difficulty+1)*sideLength);
		else resizewindow(difficulty*sideLength, (difficulty+1)*sideLength);
		setfont(heightOfChar, 0, "Consolas");
	}
}

void Resize(char mode)//调整显示大小
{
	if(mode == '+')//24-128时每格调整8，128+时32
	{
		if(sideLength >= 128) sideLength += 32;
		else sideLength += 4;
	}
	else if(mode == '-')
	{
		if(sideLength > 128) sideLength -= 32;
		else if(sideLength > 24) sideLength -= 4;
	}
}

int IsMousePosOutside()//鼠标在窗口边界外
{
	//EGE无法区分鼠标静止和鼠标在窗口边界外，调用WindowsAPI
	HWND hwnd = getHWnd();//获取绘图窗口句柄
	RECT rect;
	POINT point;
	GetWindowRect(hwnd, &rect);//获取窗口四角坐标
	GetCursorPos(&point);//获取鼠标屏幕坐标
	return (point.x < rect.left || point.x > rect.right || point.y < rect.top || point.y > rect.bottom);
	//ScreenToClient(hwnd, &point);//转换为窗口坐标
	//窗口大小rect.right-rect.left+1, rect.bottom-rect.top+1
	//return (point.x <= 0 || point.x > rect.right-rect.left || point.y <= 0 || point.y > rect.bottom-rect.top);
}

void InitBoard()
{
	int r, c, n;
	for(r=0; r<difficulty; r++)
	{
		for(c=0; c<difficulty; c++)
		{
			board[r][c] = 0;
		}
	}
	for(n=1; n<=difficulty*difficulty; )
	{
		r = rand() % difficulty;
		c = rand() % difficulty;
		if(board[r][c] == 0)
		{
			board[r][c] = n;
			n++;
		}
	}
}

int main()
{
	int r, c, t0, t1, rightNumber, countWrong;
	mouse_msg mouseMsg;
	key_msg keyMsg;
	/*选择难度*/
	InitWindow(0);
	difficulty = 0;
	r = -1;
	c = -1;
	while(difficulty == 0)
	{
		DrawSelection(r, c);
		if(IsMousePosOutside())
		{
			r = -1;
			c = -1;
		}
		while(mousemsg())
		{
			mouseMsg = getmouse();
			r = mouseMsg.y / sideLength;
			c = mouseMsg.x / sideLength;
			if(mouseMsg.is_up())//选择难度
			{
				if(r >= 0 && r <= 4) hardMode = r;
				else hardMode = 2;
				if(c >= 0 && c < 10) difficulty = c+1;
			}
			if(mouseMsg.is_wheel() && keystate(key_control))//调整显示大小
			{
				if(mouseMsg.wheel > 0) Resize('+');
				else Resize('-');
				resizewindow(10*sideLength, 5*sideLength);
				setfont(heightOfChar, 0, "Consolas");//更新字体大小
				DrawSelection(r, c);
			}
		}
		while(kbmsg())
		{
			keyMsg = getkey();
			if(keyMsg.msg == key_msg_down)
			{
				if(keyMsg.key >= '3' && keyMsg.key <= '9')//选择难度
				{
					difficulty = keyMsg.key-'3'+3;
				}
				else if(keyMsg.key >= key_num3 && keyMsg.key <= key_num9)//小键盘识别
				{
					difficulty = keyMsg.key-key_num3+3;
				}
				else if(keyMsg.key == '0' || keyMsg.key == '1' || keyMsg.key == key_num0 || keyMsg.key == key_num1)
				{
					difficulty = 10;
				}
				else if(keyMsg.key == '2' || keyMsg.key == key_num2)
				{
					difficulty = 3;
				}
			}
		}
		delay_ms(50);
	}
	/*游戏*/
	srand(time(0));
	InitBoard();
	InitWindow(1);
	r = -1;
	c = -1;
	rightNumber = 1;
	countWrong = 0;
	t0 = clock();
	while(1)
	{
		if(rightNumber <= difficulty*difficulty) t1 = clock();
		DrawBoard(r, c, t1-t0, rightNumber-1, countWrong);
		if(IsMousePosOutside())
		{
			r = -1;
			c = -1;
		}
		while(mousemsg())
		{
			mouseMsg = getmouse();
			r = mouseMsg.y / sideLength - 1;
			c = mouseMsg.x / sideLength;
			if(mouseMsg.is_up())
			{
				if(rightNumber <= difficulty*difficulty && r>=0 && r<difficulty && c>=0 && c<difficulty)
				{
					if(board[r][c] == rightNumber)
					{
						rightNumber++;
						if(hardMode == 3 && rightNumber-1 != difficulty*difficulty) InitBoard();
					}
					else countWrong++;
				}
			}
			if(mouseMsg.is_wheel() && keystate(key_control))//调整显示大小
			{
				if(mouseMsg.wheel > 0) Resize('+');
				else Resize('-');
				if(difficulty < 6) resizewindow(6*sideLength, (difficulty+1)*sideLength);
				else resizewindow(difficulty*sideLength, (difficulty+1)*sideLength);
				setfont(heightOfChar, 0, "Consolas");//更新字体大小
				DrawBoard(r, c, t1-t0, rightNumber-1, countWrong);
			}
		}
		while(kbmsg())
		{
			keyMsg = getkey();
			if(keyMsg.msg == key_msg_down)
			{
				if(keyMsg.key == 'R')
				{
					srand(time(0));
					InitBoard();
					DrawBoard(-1, -1, t1-t0, rightNumber-1, countWrong);
					rightNumber = 1;
					countWrong = 0;
					t0 = clock();
				}
			}
		}
		delay_ms(50);
	}
	closegraph();
	return 0;
}

/*--------------------------------
更新日志：
SchulteGrid 0.2
——新增 在对应位置点击可进入1阶和2阶难度
——新增 显示3个方块的Lowest、打乱的Hard、盲玩的Blind难度
——优化 旧Easy移除，旧Normal为新Easy，旧Hard为新Normal
——优化 在非数字位置点击不再会增加错误数
-------------------------------*/
