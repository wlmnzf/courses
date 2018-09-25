
/*********************包含链接的库文件*******************************************************************************************/
#pragma comment(lib, "legacy_stdio_definitions.lib ")
#pragma comment( lib, "opengl32.lib" )				
#pragma comment( lib, "glu32.lib" )	
#pragma comment( lib, "glut32.lib")
#pragma comment( lib, "glew32.lib")
#pragma comment( lib, "glaux.lib")
#pragma comment( lib, "vfw32.lib" )	
/********************************************************************************************************************************/

#include <windows.h>		// Windows的头文件
#include "glew.h"			// 包含最新的gl.h,glu.h库
//#include "glut.h"			// 包含OpenGL实用库
/***********************************新添的代码***********************************************************************************/
#include	<stdio.h>		// 标准输入/输出库的头文件
#include	"glaux.h"		// GLaux库的头文件
#include	"Alpha_Beta.h"
char chInput[512];


Alpha_Beta Chess_AB;

GLfloat		xrot;			// X 旋转量
GLfloat		yrot;			// Y 旋转量
GLfloat		zrot;			// Z 旋转量

GLfloat		xrate;

#define		BLACK	1
#define		WHITE	2
#define		CHESS	0
GLuint		texture[3];		// 存储一个纹理
/********************************************************************************************************************************/
HDC			hDC=NULL;		// 窗口着色描述表句柄
HGLRC		hRC=NULL;		// OpenGL渲染描述表句柄
HWND		hWnd=NULL;		// 保存我们的窗口句柄
HINSTANCE	hInstance;		// 保存程序的实例

bool	keys[256];			// 保存键盘按键的数组
bool	active=TRUE;		// 窗口的活动标志，缺省为TRUE
bool	fullscreen=FALSE;	// 全屏标志缺省，缺省设定成全屏模式

GLfloat		rtri;			// 用于三角形的角度
GLfloat		rquad;			// 用于四边形的角度

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// WndProc的定义
Move User_Move;
bool User_Down;





const char * szChessName = "Chess";






void GetPosition(int &x,int &y,GLfloat z)
{
	GLdouble    modelview[16];
    GLdouble    projection[16];
    GLint       viewport[4];
    glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev (GL_PROJECTION_MATRIX, projection);
    glGetIntegerv (GL_VIEWPORT, viewport);

	GLdouble world_x, world_y, world_z;
	GLdouble world_x1,world_y1,world_z1;
	gluUnProject( (GLdouble) x, (GLdouble) y, 0.0, 
                    modelview, projection, viewport, 
                    &world_x, &world_y, &world_z); 

	gluUnProject( (GLdouble) x, (GLdouble) y, 1.0, 
                    modelview, projection, viewport, 
                    &world_x1, &world_y1, &world_z1);

	x = (world_x1 - world_x) * z;
	y = (world_y1 - world_y) * z;

}










/***********************************新添的代码***********************************************************************************/
AUX_RGBImageRec *LoadBMP(CHAR *Filename)				// 载入位图图象
{
	FILE *File=NULL;									// 文件句柄
	if (!Filename)										// 确保文件名已提供
	{
		return NULL;									// 如果没提供，返回 NULL
	}
	File=fopen(Filename,"r");							// 尝试打开文件
	if (File)											// 文件存在么?
	{
		fclose(File);									// 关闭句柄
		return auxDIBImageLoadA(Filename);				// 载入位图并返回指针
	}
	return NULL;										// 如果载入失败，返回 NULL
}

int LoadGLTextures()									// 载入位图(调用上面的代码)并转换成纹理
{
	char buf_path[20] = "Data\\GoBang.bmp";
	char black_path[20] = "Data\\Black.bmp";
	char white_path[20] = "Data\\White.bmp";
	int Status=TRUE;									// 状态指示器
	AUX_RGBImageRec *TextureImage[3];					// 创建纹理的存储空间
	memset(TextureImage,0,sizeof(void *)*3);			// 将指针设为 NULL

	// 载入位图，检查有无错误，如果位图没找到则退出
	
	glGenTextures(3, &texture[0]);					// 创建纹理
	
		if(TextureImage[0]=LoadBMP(buf_path))
		{									// 将 Status 设为 TRUE

			// 使用来自位图数据生成 的典型纹理
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			// 生成纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// 线形滤波
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// 线形滤波
		}
		else
			Status=FALSE;
		//////////////////////
		if(TextureImage[1] = LoadBMP(black_path))
		{
			// 使用来自位图数据生成 的典型纹理
			glBindTexture(GL_TEXTURE_2D, texture[BLACK]);
			// 生成纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// 线形滤波
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// 线形滤波
		}
		else
			Status = FALSE;
		////////////////////////////
		if(TextureImage[2] = LoadBMP(white_path))
		{
			// 使用来自位图数据生成 的典型纹理
			glBindTexture(GL_TEXTURE_2D, texture[WHITE]);
			// 生成纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	// 线形滤波
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);	// 线形滤波
		}
		else
			Status = FALSE;

		//////
//
		//////
		if (TextureImage[0])								// 纹理是否存在
		{
			if (TextureImage[0]->data)						// 纹理图像是否存在
			{
				free(TextureImage[0]->data);				// 释放纹理图像占用的内存
			}
			free(TextureImage[0]);							// 释放图像结构
		}
		if (TextureImage[1])								// 纹理是否存在
		{
			if (TextureImage[1]->data)						// 纹理图像是否存在
			{
				free(TextureImage[1]->data);				// 释放纹理图像占用的内存
			}
			free(TextureImage[1]);							// 释放图像结构
		}
		if (TextureImage[2])								// 纹理是否存在
		{
			if (TextureImage[2]->data)						// 纹理图像是否存在
			{
				free(TextureImage[2]->data);				// 释放纹理图像占用的内存
			}
			free(TextureImage[2]);							// 释放图像结构
		}
	return Status;										// 返回 Status
}

/********************************************************************************************************************************/
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// 重置OpenGL窗口大小
{
	if (height==0)										// 防止被零除
	{
		height=1;										// 将Height设为1
	}

	glViewport(0,0,width,height);						// 重置当前的视口

	glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
	glLoadIdentity();									// 重置投影矩阵

	// 设置视口的大小
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// 选择模型观察矩阵
	glLoadIdentity();									// 重置模型观察矩阵
}

int InitGL(GLvoid)										// 此处开始对OpenGL进行所有设置
{
/***********************************新添的代码***********************************************************************************/
	if (!LoadGLTextures())								// 调用纹理载入子例程
	{
		return FALSE;									// 如果未能载入，返回FALSE
	}

	glEnable(GL_TEXTURE_2D);							// 启用纹理映射
/********************************************************************************************************************************/
	glShadeModel(GL_SMOOTH);							// 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// 黑色背景
	glClearDepth(1.0f);									// 设置深度缓存
	glEnable(GL_DEPTH_TEST);							// 启用深度测试
	glDepthFunc(GL_LEQUAL);								// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 告诉系统对透视进行修正

/*
	glColor4f(1.0f,1.0f,1.0f,1.0f);			// 全亮度， 50% Alpha 混合
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// 基于源象素alpha通道值的半透明混合函数

	glEnable(GL_BLEND);			// 打开混合
	glDisable(GL_DEPTH_TEST);	// 关闭深度测试
	*/
	return TRUE;										// 初始化 OK
}
#define CSQU 0.0637f


int Chess_Board[MAX_Y + 1][MAX_X + 1] = 
{
	{}
};
#define START 0.905f
#define Graph_Position_Z (- 4.0f)
int DrawGLScene(GLvoid)									// 从这里开始进行所有的绘制
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕和深度缓存
	glLoadIdentity();									// 重置当前的模型观察矩阵

/***********************************新添的代码***********************************************************************************/
	glTranslatef(0.0f,0.0f,Graph_Position_Z);									// 移入屏幕 5 个单位
	glBindTexture(GL_TEXTURE_2D, texture[0]);						// 选择纹理
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// 纹理和四边形的左上

	glEnd();
	glTranslatef( - START,START,1.0f);
	for(int y = 1;y <= MAX_Y;y ++)
	{
		float dis = 0;
		for(int x = 1;x <= MAX_X;x ++)
		{
			if(Chess_Board[y][x])
			{
				if(Chess_Board[y][x] == BLACK)
					glBindTexture(GL_TEXTURE_2D, texture[BLACK]);
				else
					glBindTexture(GL_TEXTURE_2D, texture[WHITE]);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(-CSQU, -CSQU,  CSQU);	// 纹理和四边形的左下
					glTexCoord2f(1.0f, 0.0f); glVertex3f( CSQU, -CSQU,  CSQU);	// 纹理和四边形的右下
					glTexCoord2f(1.0f, 1.0f); glVertex3f( CSQU,  CSQU,  CSQU);	// 纹理和四边形的右上
					glTexCoord2f(0.0f, 1.0f); glVertex3f(-CSQU,  CSQU,  CSQU);	// 纹理和四边形的左上
				glEnd();
			}
			glTranslatef( 2 * CSQU + 0.0017,0.0f,0.0f);
			dis += 2 * CSQU + 0.0017;
		}
		glTranslatef(- dis ,- (CSQU * 2 + 0.0017),0.0f);
	}

	glTranslatef( 8.0f,0.0f,0.0f);

	glBindTexture(GL_TEXTURE_2D, texture[WHITE]);
	return TRUE;										// 一切 OK
}

GLvoid KillGLWindow(GLvoid)								// 正常销毁窗口
{
	if (fullscreen)										// 我们处于全屏模式吗?
	{
		ChangeDisplaySettings(NULL,0);					// 是的话，切换回桌面
		ShowCursor(TRUE);								// 显示鼠标指针
	}

	if (hRC)											//我们拥有OpenGL描述表吗?
	{
		if (!wglMakeCurrent(NULL,NULL))					// 我们能否释放DC和RC描述表?
		{
			MessageBox(NULL,"释放DC或RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// 我们能否删除RC?
		{
			MessageBox(NULL,"释放RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// 将RC设为 NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// 我们能否释放 DC?
	{
		MessageBox(NULL,"释放DC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// 将 DC 设为 NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// 能否销毁窗口?
	{
		MessageBox(NULL,"释放窗口句柄失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// 将 hWnd 设为 NULL
	}

	if (!UnregisterClass("OpenG",hInstance))			// 能否注销类?
	{
		MessageBox(NULL,"不能注销窗口类。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// 将 hInstance 设为 NULL
	}
}

/*	这个函数创建我们OpenGL窗口，参数为：									*
 *	title			- 窗口标题												*
 *	width			- 窗口宽度												*
 *	height			- 窗口高度												*
 *	bits			- 颜色的位深（8/16/32）									*
 *	fullscreenflag	- 是否使用全屏模式，全屏模式(TRUE)，窗口模式(FALSE)		*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// 保存查找匹配的结果
	WNDCLASS	wc;						// 窗口类结构
	DWORD		dwExStyle;				// 扩展窗口风格
	DWORD		dwStyle;				// 窗口风格
	RECT		WindowRect;				// 取得矩形的左上角和右下角的坐标值
	WindowRect.left=(long)0;			// 将Left   设为 0
	WindowRect.right=(long)width;		// 将Right  设为要求的宽度
	WindowRect.top=(long)0;				// 将Top    设为 0
	WindowRect.bottom=(long)height;		// 将Bottom 设为要求的高度

	fullscreen=fullscreenflag;			// 设置全局全屏标志

	hInstance			= GetModuleHandle(NULL);				// 取得我们窗口的实例
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// 移动时重画，并为窗口取得DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc处理消息
	wc.cbClsExtra		= 0;									// 无额外窗口数据
	wc.cbWndExtra		= 0;									// 无额外窗口数据
	wc.hInstance		= hInstance;							// 设置实例
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// 装入缺省图标
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// 装入鼠标指针
	wc.hbrBackground	= NULL;									// GL不需要背景
	wc.lpszMenuName		= NULL;									// 不需要菜单
	wc.lpszClassName	= "OpenG";							// 设定类名字

	if (!RegisterClass(&wc))									// 尝试注册窗口类
	{
		MessageBox(NULL,"注册窗口失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// 退出并返回FALSE
	}
	

	if (fullscreen)												// 要尝试全屏模式吗?
	{
		DEVMODE dmScreenSettings;								// 设备模式
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// 确保内存清空为零
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Devmode 结构的大小
		dmScreenSettings.dmPelsWidth	= width;				// 所选屏幕宽度
		dmScreenSettings.dmPelsHeight	= height;				// 所选屏幕高度
		dmScreenSettings.dmBitsPerPel	= bits;					// 每象素所选的色彩深度
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// 若模式失败，提供两个选项：退出或在窗口内运行。
			if (MessageBox(NULL,"全屏模式在当前显卡上设置失败！\n使用窗口模式？","NeHe G",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				//如果用户选择窗口模式，变量fullscreen 的值变为FALSE,程序继续运行
				fullscreen=FALSE;		// 选择窗口模式(Fullscreen=FALSE)
			}
			else
			{
				//如果用户选择退出，弹出消息窗口告知用户程序将结束。并返回FALSE告诉程序窗口未能成功创建。程序退出。
				MessageBox(NULL,"程序将被关闭","错误",MB_OK|MB_ICONSTOP);
				return FALSE;									// 退出并返回 FALSE
			}
		}
	}
	ShowCursor(TRUE);
	if (fullscreen)												// 仍处于全屏模式吗?
	{
		dwExStyle=WS_EX_APPWINDOW;								// 扩展窗体风格
		dwStyle=WS_POPUP;										// 窗体风格
											// 隐藏鼠标指针
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// 扩展窗体风格
		dwStyle=WS_OVERLAPPEDWINDOW;							// 窗体风格
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// 调整窗口达到真正要求的大小


	// 创建窗口
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// 扩展窗体风格
								"OpenG",							// 类名字
								title,								// 窗口标题
								dwStyle |							// 必须的窗体风格属性
								WS_CLIPSIBLINGS |					// 必须的窗体风格属性
								WS_CLIPCHILDREN,					// 必须的窗体风格属性
								0, 0,								// 窗口位置
								WindowRect.right-WindowRect.left,	// 计算调整好的窗口宽度
								WindowRect.bottom-WindowRect.top,	// 计算调整好的窗口高度
								NULL,								// 无父窗口
								NULL,								// 无菜单
								hInstance,							// 实例
								NULL)))								// 不向WM_CREATE传递任何东东
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"窗口创建错误","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				//pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// 上述格式描述符的大小
		1,											// 版本号
		PFD_DRAW_TO_WINDOW |						// 格式支持窗口
		PFD_SUPPORT_OPENGL |						// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,							// 必须支持双缓冲
		PFD_TYPE_RGBA,								// 申请 RGBA 格式
		bits,										// 选定色彩深度
		0, 0, 0, 0, 0, 0,							// 忽略的色彩位
		0,											// 无Alpha缓存
		0,											// 忽略Shift Bit
		0,											// 无累加缓存
		0, 0, 0, 0,									// 忽略聚集位
		16,											// 16位 Z-缓存 (深度缓存) 
		0,											// 无蒙板缓存
		0,											// 无辅助缓存
		PFD_MAIN_PLANE,								// 主绘图层
		0,											// 不使用重叠层
		0, 0, 0										// 忽略层遮罩
	};
	
	if (!(hDC=GetDC(hWnd)))							// 取得设备描述表了么?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能创建一个窗口设备描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Windows 找到相应的象素格式了吗?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能创建一种相匹配的像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// 能够设置象素格式么?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能设置像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// 能否取得OpenGL渲染描述表?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能创建OpenGL渲染描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// 尝试激活着色描述表
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能激活当前的OpenGL渲然描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// 显示窗口
	SetForegroundWindow(hWnd);						// 略略提高优先级
	SetFocus(hWnd);									// 设置键盘的焦点至此窗口
	ReSizeGLScene(width, height);					// 设置透视 GL 屏幕

	if (!InitGL())									// 初始化新建的GL窗口
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"初始化失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	return TRUE;									// 成功
}

#define XP 177
#define YP 78

#define XPE 623 
#define YPE 524



#define MAXTIME_MS 2000
LRESULT CALLBACK WndProc(	HWND	hWnd,			// 窗口的句柄	
							UINT	uMsg,			// 窗口的消息
							WPARAM	wParam,			// 附加的消息内容
							LPARAM	lParam)			// 附加的消息内容
{
	static POINT CurPoint,PointNow;
	static int cx,cy;//间隔长度
	static RECT rect;

	switch (uMsg)									// 检查Windows消息
	{
		case WM_CREATE:
		{
			cx = (XPE - XP) / (MAX_X - 1);
			cy = (YPE - YP) / (MAX_Y - 1);
			Chess_AB.Reload();
		}
		case WM_ACTIVATE:							// 监视窗口激活消息
		{
			if (!HIWORD(wParam))					// 检查最小化状态
			{
				active=TRUE;						// 程序处于激活状态
			}
			else
			{
				active=FALSE;						// 程序不再激活
			}

			return 0;								// 返回消息循环
		}
		
		case WM_COMMAND:
		{
			if(WM_LBUTTONDOWN == wParam)
			{
				int y = lParam >> 8;
				int x = lParam & 0xff;
				if( x <= MAX_X && x > 0 && y <= MAX_Y && y >0 && !Chess_Board[y][x])
				{
					User_Move.x = x;
					User_Move.y = y;
					
					User_Move = Chess_AB.Search(User_Move);
					if(!Chess_Board[User_Move.y][User_Move.x])
						Chess_Board[User_Move.y][User_Move.x] = WHITE;
					
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			GetCursorPos(&CurPoint);
			ScreenToClient(hWnd,&CurPoint);

			CurPoint.x -= XP;
			CurPoint.y -= YP;

			int x = CurPoint.x / cx;
			int y = CurPoint.y / cy;

			if((CurPoint.x % cx) >= (cx/2))
				x ++;
			if((CurPoint.y % cy) >= (cy/2))
				y ++;
				
			x ++;
			y ++;
			
			User_Down = true;

			User_Move.x = x;
			User_Move.y = y;

			//char chInput[512];
			sprintf(chInput, "int:%d %d\n",y,x);
			OutputDebugString(chInput);


			Chess_Board[y][x] = BLACK;
			

			if(Chess_AB.IsKill(User_Move,BLACK))
			{
				MessageBox(hWnd,"我挂了","你强",MB_OK);
			}
			break;
		}
		case WM_SYSCOMMAND:							// 系统中断命令
		{
			switch (wParam)							// 检查系统调用
			{
				case SC_SCREENSAVE:					// 屏保要运行?
				case SC_MONITORPOWER:				// 显示器要进入节电模式?
				return 0;							// 阻止发生
			}
			break;									// 退出
		}

		case WM_CLOSE:								// 收到Close消息?
		{
			PostQuitMessage(0);						// 发出退出消息
			return 0;								// 返回
		}

		case WM_KEYDOWN:							// 有键按下么?
		{
			keys[wParam] = TRUE;					// 如果是，设为TRUE
			return 0;								// 返回
		}

		case WM_KEYUP:								// 有键放开么?
		{
			keys[wParam] = FALSE;					// 如果是，设为FALSE
			return 0;								// 返回
		}

		case WM_SIZE:								// 调整OpenGL窗口大小
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width,HiWord=Height
			GetWindowRect(hWnd,&rect);
			return 0;								// 返回
		}
	}

	// 向 DefWindowProc传递所有未处理的消息。
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// 当前窗口实例
					HINSTANCE	hPrevInstance,		// 前一个窗口实例
					LPSTR		lpCmdLine,			// 命令行参数
					int			nCmdShow)			// 窗口显示状态
{
	MSG		msg;									// Windowsx消息结构
	BOOL	done=FALSE;								// 用来退出循环的Bool 变量


	// 创建OpenGL窗口
//	if (!CreateGLWindow("NeHe's 3D实例",640,480,16,fullscreen))
/***********************************新添的代码***********************************************************************************/
	if (!CreateGLWindow("NeHe's 纹理实例",800,600,16,fullscreen))
/********************************************************************************************************************************/
	{
		return 0;									// 失败退出
	}

	while(!done)									// 保持循环直到 done=TRUE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// 有消息在等待吗?
		{
			if (msg.message==WM_QUIT)				// 收到退出消息?
			{
				done=TRUE;							// 是，则done=TRUE
			}
			else									// 不是，处理窗口消息
			{
				TranslateMessage(&msg);				// 翻译消息
				DispatchMessage(&msg);				// 发送消息
			}
		}
		else										// 如果没有消息
		{

			// 绘制场景。监视ESC键和来自DrawGLScene()的退出消息
			if (active)								// 程序激活的么?
			{
				if (keys[VK_ESCAPE])				// ESC 按下了么?
				{
					done=TRUE;						// ESC 发出退出信号
				}
				else								// 不是退出的时候，刷新屏幕
				{
					DrawGLScene();					// 绘制场景
					SwapBuffers(hDC);				// 交换缓存 (双缓存)
				}
			}

			if (keys[VK_F1])						// F1键按下了么?
			{
				keys[VK_F1]=FALSE;					// 若是，使对应的Key数组中的值为 FALSE
				KillGLWindow();						// 销毁当前的窗口
				fullscreen=!fullscreen;				// 切换 全屏 / 窗口 模式
				// 重建 OpenGL 窗口
//				if (!CreateGLWindow("NeHe's 3D实例",640,480,16,fullscreen))
/***********************************新添的代码***********************************************************************************/
				if (!CreateGLWindow("NeHe's 纹理实例",640,480,16, fullscreen))
/********************************************************************************************************************************/
				{
					return 0;						// 如果窗口未能创建，程序退出
				}
			}

			if (keys[VK_UP])
			{
				keys[VK_UP] = FALSE;
				xrate += 0.01;
			}
			
			
			if (User_Down)
			{
				User_Down = false;
				sprintf(chInput, "AI-before:%d %d\n", User_Move.y, User_Move.x);
				OutputDebugString(chInput);

				User_Move = Chess_AB.Search(User_Move);

				

			//	sprintf(chInput, "AI:%d %d\n", User_Move.y, User_Move.x);
			//	OutputDebugString(chInput);

				Chess_Board[User_Move.y][User_Move.x] = WHITE;

				int i;
				int j;
				int index = 0;
				for (i = 0; i < 17; i++,index++)
				{
					for (j = 0; j < 17; j++,index++)
					{
						sprintf(chInput+index, "%d\n",Chess_AB.Chess[i][j]);
					}
					sprintf(chInput + index, "\n");
				}

				//OutputDebugString(chInput);



				if(Chess_AB.IsKill(User_Move,WHITE))
				{
					MessageBox(hWnd,"你挂了","哈哈",MB_OK);
				}
			}

		}
	}

	// 关闭程序
	KillGLWindow();									// 销毁窗口
	return (msg.wParam);							// 退出程序
}
