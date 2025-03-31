#include<stdlib.h>
#include"GL/glaux.h"
#include<GL/glut.h>
#pragma comment(lib,"glaux")
#include<math.h>
#include<map>
#include<string>
#include<iostream>
#include"thing.h"
using namespace std;

//绘制部分
//撤回
bool step = false;
int num;//撤回线条的图层号

//图层
int Layer = 0, layer = 0;//图层数量，当前图层号
bool* open = (bool*)calloc(sizeof(bool), 1);//图层是否打开
float r = 0, g = 0, b = 0;
int p0[2];//记录点（起点）

//工具栏
int tools = 1;
bool pen = false;//1
bool eraser = false;//2
int pointx = 0, pointy = 0;//起始定点
bool pic = false;//3
bool line = false;//4

struct point {
	float r = 0;
	float g = 0;
	float b = 0;
	int p1[2] = { 0,0 };//后继点
};

map<int, map<string, point*>>layers;

//展示部分
bool showing = false;//是否打开
bool moving = false;
bool uplooking = false;
GLuint basic[8];//墙纸和地板
int wall = 0, fl = 1;

//摆放
struct pos {
	int x = 400;
	int y = 81;
	int z = 400;
	int look = 0;
	int angle = 0;
};

GLuint bed[6];
GLuint box[7];
GLuint pot[2];
GLuint blanket[3];
GLuint table[3];
GLuint chair[4];

int thing = 0;//选中
bool visible[8] = { 0 };//是否已摆放
map<int, pos*>things;

void picture();
void Init();
void Reshape(int w, int h);
//绘制
void Button(int p);
void Screen();
void Minpaint(int n);
void Repaint();
void Paint(int x, int y);
void Eraser(int x, int y);
void Cube(int x, int y);
void Lines(int x, int y);
//摆放
void Uplook();
void Show(int x);
void Put(int x, int y, int z);
void Reput();
//控制
void mouse(int button, int state, int x, int y);
void move(int x, int y);
void keyboard(unsigned char key, int x, int y);
//菜单
void menu(int data);
void menu_layers(int data);
void menu_tool(int data);
void menu_thing(int data);

void picture()
{
	//墙纸和地板
	AUX_RGBImageRec* TextureImage1[8];
	memset(TextureImage1, 0, sizeof(void*) * 8);//初始化并分配内存
	TextureImage1[0] = auxDIBImageLoad(TEXT("wall.bmp"));//载入图片文件
	TextureImage1[1] = auxDIBImageLoad(TEXT("floor.bmp"));
	TextureImage1[2] = auxDIBImageLoad(TEXT("paper1.bmp"));
	TextureImage1[3] = auxDIBImageLoad(TEXT("paper2.bmp"));
	TextureImage1[4] = auxDIBImageLoad(TEXT("paper3.bmp"));
	TextureImage1[5] = auxDIBImageLoad(TEXT("paper4.bmp"));
	TextureImage1[6] = auxDIBImageLoad(TEXT("paper5.bmp"));
	TextureImage1[7] = auxDIBImageLoad(TEXT("paper6.bmp"));
	for (int i = 0; i < 8; i++)
	{
		if (TextureImage1[i])//纹理是否存在
		{
			glGenTextures(1, &basic[i]);//创建纹理
			glBindTexture(GL_TEXTURE_2D, basic[i]);//绑定纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage1[i]->sizeX, TextureImage1[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage1[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage1[i]);//释放内存
		}
		else
			printf("error");
	}

	//床
	AUX_RGBImageRec* TextureImage2[6];
	memset(TextureImage2, 0, sizeof(void*) * 6);
	TextureImage2[0] = auxDIBImageLoad(TEXT("bed1.bmp"));
	TextureImage2[1] = auxDIBImageLoad(TEXT("bed1-2.bmp"));
	TextureImage2[2] = auxDIBImageLoad(TEXT("bed2.bmp"));
	TextureImage2[3] = auxDIBImageLoad(TEXT("bed2-2.bmp"));
	TextureImage2[4] = auxDIBImageLoad(TEXT("bed3.bmp"));
	TextureImage2[5] = auxDIBImageLoad(TEXT("bed3-2.bmp"));
	for (int i = 0; i < 6; i++)
	{
		if (TextureImage2[i])//纹理是否存在
		{
			glGenTextures(1, &bed[i]);//创建纹理
			glBindTexture(GL_TEXTURE_2D, bed[i]);//绑定纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage2[i]->sizeX, TextureImage2[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage2[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage2[i]);//释放内存
		}
		else
			printf("error");
	}

	//柜子
	AUX_RGBImageRec* TextureImage3[7];
	memset(TextureImage3, 0, sizeof(void*) * 7);//初始化并分配内存
	TextureImage3[0] = auxDIBImageLoad(TEXT("box1.bmp"));//载入图片文件
	TextureImage3[1] = auxDIBImageLoad(TEXT("box1-2.bmp"));
	TextureImage3[2] = auxDIBImageLoad(TEXT("box2.bmp"));
	TextureImage3[3] = auxDIBImageLoad(TEXT("box2-2.bmp"));
	TextureImage3[4] = auxDIBImageLoad(TEXT("box2-3.bmp"));
	TextureImage3[5] = auxDIBImageLoad(TEXT("box3.bmp"));
	TextureImage3[6] = auxDIBImageLoad(TEXT("box3-2.bmp"));
	for (int i = 0; i < 7; i++)
	{
		if (TextureImage3[i])//纹理是否存在
		{
			glGenTextures(1, &box[i]);//创建纹理
			glBindTexture(GL_TEXTURE_2D, box[i]);//绑定纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage3[i]->sizeX, TextureImage3[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage3[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage3[i]);//释放内存
		}
		else
			printf("error");
	}

	//花瓶
	AUX_RGBImageRec* TextureImage4[2];
	memset(TextureImage4, 0, sizeof(void*) * 2);//初始化并分配内存
	TextureImage4[0] = auxDIBImageLoad(TEXT("pot1.bmp"));//载入图片文件
	TextureImage4[1] = auxDIBImageLoad(TEXT("pot1-2.bmp"));
	for (int i = 0; i < 2; i++)
	{
		if (TextureImage4[i])//纹理是否存在
		{
			glGenTextures(1, &pot[i]);//创建纹理
			glBindTexture(GL_TEXTURE_2D, pot[i]);//绑定纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage4[i]->sizeX, TextureImage4[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage4[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage4[i]);//释放内存
		}
		else
			printf("error");
	}

	//桌子
	AUX_RGBImageRec* TextureImage6[3];
	memset(TextureImage6, 0, sizeof(void*) * 3);
	TextureImage6[0] = auxDIBImageLoad(TEXT("table1.bmp"));
	TextureImage6[1] = auxDIBImageLoad(TEXT("table2.bmp"));
	TextureImage6[2] = auxDIBImageLoad(TEXT("table3.bmp"));
	for (int i = 0; i < 3; i++)
	{
		if (TextureImage6[i])//纹理是否存在
		{
			glGenTextures(1, &table[i]);//创建纹理
			glBindTexture(GL_TEXTURE_2D, table[i]);//绑定纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage6[i]->sizeX, TextureImage6[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage6[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage6[i]);//释放内存
		}
		else
			printf("error");
	}

	//地毯
	AUX_RGBImageRec* TextureImage5[3];
	memset(TextureImage5, 0, sizeof(void*) * 3);
	TextureImage5[0] = auxDIBImageLoad(TEXT("blanket1.bmp"));
	TextureImage5[1] = auxDIBImageLoad(TEXT("blanket2.bmp"));
	TextureImage5[2] = auxDIBImageLoad(TEXT("blanket3.bmp"));
	for (int i = 0; i < 3; i++)
	{
		if (TextureImage5[i])//纹理是否存在
		{
			glGenTextures(1, &blanket[i]);//创建纹理
			glBindTexture(GL_TEXTURE_2D, blanket[i]);//绑定纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage5[i]->sizeX, TextureImage5[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage5[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage5[i]);//释放内存
		}
		else
			printf("error");
	}

	//椅子
	AUX_RGBImageRec* TextureImage7[4];
	memset(TextureImage7, 0, sizeof(void*) * 4);
	TextureImage7[0] = auxDIBImageLoad(TEXT("chair1.bmp"));
	TextureImage7[1] = auxDIBImageLoad(TEXT("chair1-2.bmp"));
	TextureImage7[2] = auxDIBImageLoad(TEXT("chair2.bmp"));
	TextureImage7[3] = auxDIBImageLoad(TEXT("chair3.bmp"));
	for (int i = 0; i < 4; i++)
	{
		if (TextureImage7[i])//纹理是否存在
		{
			glGenTextures(1, &chair[i]);//创建纹理
			glBindTexture(GL_TEXTURE_2D, chair[i]);//绑定纹理
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage7[i]->sizeX, TextureImage7[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage7[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage7[i]);//释放内存
		}
		else
			printf("error");
	}
}

void Init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	picture();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//回复原始坐标系环境，原点为（0,0）

	glOrtho(-w, w, -h, h, -4000, 4000);//左，右，下，上，后，前
	glMatrixMode(GL_MODELVIEW);
	glTranslated(-w, -h, 0);
	glScaled(2, 2, 1);
	gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Button(int p)
{
	glColor3f(0.5, 0.5, 0.5);
	glVertex2f(p, 50);
	glVertex2f(p + 10, 50);
}

//页面布局
void Screen()
{
	glScissor(0, 0, 950, 70);//只重绘颜色选区部分
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	glLineWidth(2);
	glLineStipple(2, 0x0F0F);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(0, 80); glVertex2f(950, 80);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glLineWidth(10);
	glBegin(GL_LINES);

	glColor3f(r / 225, g / 225, b / 225);//color
	glVertex2f(50, 50); glVertex2f(100, 50);

	glColor3f(0.0, 0.0, 0.0);//red
	glVertex2f(150, 50);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(375, 50);
	Button(150 + r);

	glColor3f(0.0, 0.0, 0.0);//green
	glVertex2f(425, 50);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(650, 50);
	Button(425 + g);

	glColor3f(0.0, 0.0, 0.0);//blue
	glVertex2f(700, 50);
	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(925, 50);
	Button(700 + b);
	glEnd();

	glLineWidth(1);//小窗口
	glColor3f(0.0, 0.0, 0.0);
	char see[] = "See";
	char unsee[] = "Un";
	for (int i = 1; i <= Layer; i++)
	{
		if (open[i])
		{
			for (int j = 0; j < sizeof(see); j++)
			{
				glRasterPos2f(885 + 7 * j, 881 - 110 * i);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, see[j]);
				glFlush();
			}
		}
		else
		{
			for (int j = 0; j < sizeof(unsee); j++)
			{
				glRasterPos2f(885 + 7 * j, 881 - 110 * i);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, unsee[j]);
				glFlush();
			}
		}
	}

	glBegin(GL_LINES);
	for (int i = 1; i <= Layer; i++)
	{
		glVertex2i(850, 800 - 110 * i); glVertex2i(850, 872 - 110 * i);
		glVertex2i(850, 872 - 110 * i); glVertex2i(935, 872 - 110 * i);
		glVertex2i(935, 872 - 110 * i); glVertex2i(935, 800 - 110 * i);
		glVertex2i(935, 800 - 110 * i); glVertex2i(850, 800 - 110 * i);
	}
	glEnd();

	glFlush();
}

//小窗口
void Minpaint(int n)
{
	glScissor(850, 800 - 111 * n, 100, 110);//只重绘小图层
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	
	glPushMatrix();
	glTranslatef(850, 792 - 110 * n, 0);
	glScalef(0.1, 0.1, 1);
	int p2[2], p;
	map<string, point*>lay = layers[n];
	for (auto i = lay.begin(); i != lay.end(); ++i)
	{
		p = (i->first).find("-");
		p2[0] = stoi((i->first).substr(0, p));
		p2[1] = stoi((i->first).substr(p + 1));
		glColor3f((i->second)->r / 225, (i->second)->g / 225, (i->second)->b / 225);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2i((i->second)->p1[0], (i->second)->p1[1]); glVertex2i(p2[0], p2[1]);
		glEnd();
	}
	glPopMatrix();
	
	glFlush();
	glutPostRedisplay();
}

//重绘
void Repaint()
{
	glScissor(0, 70, 850, 730);//只重绘画图部分
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	int p2[2], p;
	for (int j = Layer; j > 0; j--)
	{
		if (open[j] == 0)
			continue;
		map<string, point*>lay = layers[j];
		for (auto i = lay.begin(); i != lay.end(); ++i)
		{
			p = (i->first).find("-");
			p2[0] = stoi((i->first).substr(0, p));
			p2[1] = stoi((i->first).substr(p + 1));
			glColor3f((i->second)->r / 225, (i->second)->g / 225, (i->second)->b / 225);
			glLineWidth(2);
			glBegin(GL_LINES);
			glVertex3i((i->second)->p1[0], (i->second)->p1[1], 0); glVertex3i(p2[0], p2[1], 0);
			glEnd();
		}
	}
	glFlush();
}

//画笔
void Paint(int x, int y)
{
	glColor3f(r / 225, g / 225, b / 225);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2i(p0[0], p0[1]); glVertex2i(x, 800 - y);
	glEnd();
	glFlush();

	string s = to_string(p0[0]) + "-" + to_string(p0[1]);
	point* p = new point;
	p->r = r, p->g = g, p->b = b;
	p->p1[0] = x, p->p1[1] = 800 - y;
	layers[layer][s] = p;
	Minpaint(layer);
}

//橡皮擦
void Eraser(int x, int y)
{
	float k = float(p0[1] - (800 - y)) / (p0[0] - x);
	int ps[2], pe[2], p;
	map<string, point*>lay = layers[layer];
	for (auto i = lay.begin(); i != lay.end(); ++i)
	{
		p = (i->first).find("-");
		ps[0] = stoi((i->first).substr(0, p));
		ps[1] = stoi((i->first).substr(p + 1));
		pe[0] = i->second->p1[0];
		pe[1] = i->second->p1[1];
		if (((ps[0] >= x) ? 1 : 0) != ((ps[0] >= p0[0]) ? 1 : 0) && ((ps[1] >= (800 - y)) ? 1 : 0) != ((ps[1] >= p0[1]) ? 1 : 0)) {}//ps是否在范围内
		else if (((pe[0] >= x) ? 1 : 0) != ((pe[0] >= p0[0]) ? 1 : 0) && ((pe[1] >= (800 - y)) ? 1 : 0) != ((pe[1] >= p0[1]) ? 1 : 0)) {}//pe是否在范围内
		else
			continue;
		if (((k < float(p0[1] - ps[1]) / (p0[0] - ps[0])) ? 1 : 0) != ((k < float(p0[1] - pe[1]) / (p0[0] - pe[0])) ? 1 : 0))//判断是否有交点
		{
			layers[layer].erase(i->first);
		}
	}
	Repaint();
	Screen();
}

//矩形
void Cube(int x, int y)
{
	Repaint();
	Screen();
	glColor3f(r / 225, g / 225, b / 225);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2i(pointx, pointy); glVertex2i(pointx, 800 - y);
	glVertex2i(pointx, 800 - y); glVertex2i(x, 800 - y);
	glVertex2i(x, 800 - y); glVertex2i(x, pointy);
	glVertex2i(x, pointy); glVertex2i(pointx, pointy);
	glEnd();
	glFlush();
}

//直线
void Lines(int x, int y)
{
	Repaint();
	Screen();
	glColor3f(r / 225, g / 225, b / 225);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2i(pointx, pointy); glVertex2i(x, 800 - y);
	glEnd();
	glFlush();
}

//俯视图
void Uplook()
{
	glScissor(738, 0, 212, 212);//只重绘画图部分
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	glPushMatrix();
	glLoadIdentity();
	gluLookAt(0, 1, 0, 0, 0, 0, 0, 0, -1);
	glTranslatef(525, 0, 375);
	glScalef(0.5, 0.5, 0.5);
	glEnable(GL_BACK);
	glEnable(GL_CULL_FACE);//启用裁剪
	glEnable(GL_TEXTURE_2D);//开启2D纹理功能
	glBindTexture(GL_TEXTURE_2D, basic[fl]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 0);//左上
	glTexCoord2f(1.0, 0.0); glVertex3f(0, 0, 850);//右上
	glTexCoord2f(1.0, 1.0); glVertex3f(850, 0, 850);//右下
	glTexCoord2f(0.0, 1.0); glVertex3f(850, 0, 0);//左下
	glEnd();
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	Reput();
	glPopMatrix();
}

//立体展示
void Show(int x)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRotatef(float(x - pointx) / 200, 0, 1, 0);

	glEnable(GL_TEXTURE_2D);//开启2D纹理功能
	glBindTexture(GL_TEXTURE_2D, basic[wall]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 80, 0);//左上
	glTexCoord2f(1.0, 0.0); glVertex3f(850, 80, 0);//右上
	glTexCoord2f(1.0, 1.0); glVertex3f(850, 800, 0);//右下
	glTexCoord2f(0.0, 1.0); glVertex3f(0, 800, 0);//左下

	glTexCoord2f(0.0, 0.0); glVertex3f(0, 80, 850);//左上
	glTexCoord2f(1.0, 0.0); glVertex3f(0, 80, 0);//右上
	glTexCoord2f(1.0, 1.0); glVertex3f(0, 800, 0);//右下
	glTexCoord2f(0.0, 1.0); glVertex3f(0, 800, 850);//左下
	glEnd();

	if (fl > 5)
	{
		glBindTexture(GL_TEXTURE_2D, basic[fl]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0, 80, 0);//左上
		glTexCoord2f(5.0, 0.0); glVertex3f(0, 80, 850);//右上
		glTexCoord2f(5.0, 5.0); glVertex3f(850, 80, 850);//右下
		glTexCoord2f(0.0, 5.0); glVertex3f(850, 80, 0);//左下
		glEnd();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, basic[fl]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0, 80, 0);//左上
		glTexCoord2f(1.0, 0.0); glVertex3f(0, 80, 850);//右上
		glTexCoord2f(1.0, 1.0); glVertex3f(850, 80, 850);//右下
		glTexCoord2f(0.0, 1.0); glVertex3f(850, 80, 0);//左下
		glEnd();
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	int p2[2], p;
	for (int j = Layer; j > 0; j--)
	{
		if (open[j] == 0)
			continue;
		map<string, point*>lay = layers[j];
		for (auto i = lay.begin(); i != lay.end(); ++i)
		{
			p = (i->first).find("-");
			p2[0] = stoi((i->first).substr(0, p));
			p2[1] = stoi((i->first).substr(p + 1));
			glColor3f((i->second)->r / 225, (i->second)->g / 225, (i->second)->b / 225);
			glLineWidth(2);
			glBegin(GL_LINES);
			glVertex3i((i->second)->p1[0], (i->second)->p1[1], 1); glVertex3i(p2[0], p2[1], 1);
			glVertex3i(1, (i->second)->p1[1], 840 - (i->second)->p1[0]); glVertex3i(1, p2[1], 840 - p2[0]);
			glEnd();
		}
	}

	glEnable(GL_BACK);
	glEnable(GL_CULL_FACE);
	Reput();

	if (uplooking)
		Uplook();
	glFlush();
}

//摆放
void Put(int x, int y, int z)//底部中点位置
{
	switch (thing)
	{
	case 2:
		Bed(x, y, z, things[thing]->look, things[thing]->angle, bed);
		break;
	case 3:
		Box(x, y, z, things[thing]->look, things[thing]->angle, box);
		break;
	case 4:
		Pot(x, y, z, things[thing]->look, things[thing]->angle, pot);
		break;
	case 5:
		Blanket(x, y, z, things[thing]->look, things[thing]->angle, blanket);
		break;
	case 6:
		Table(x, y, z, things[thing]->look, things[thing]->angle, table);
		break;
	case 7:
		Chair(x, y, z, things[thing]->look, things[thing]->angle, chair);
		break;
	default:
		break;
	}
}

//重现
void Reput()
{
	for (auto i = things.begin(); i != things.end(); i++)
	{
		switch (i->first)
		{
		case 2:
			Bed(i->second->x, i->second->y, i->second->z, i->second->look, i->second->angle, bed);
			break;
		case 3:
			Box(i->second->x, i->second->y, i->second->z, i->second->look, i->second->angle, box);
			break;
		case 4:
			Pot(i->second->x, i->second->y, i->second->z, i->second->look, i->second->angle, pot);
			break;
		case 5:
			Blanket(i->second->x, i->second->y, i->second->z, i->second->look, i->second->angle, blanket);
			break;
		case 6:
			Table(i->second->x, i->second->y, i->second->z, i->second->look, i->second->angle, table);
			break;
		case 7:
			Chair(i->second->x, i->second->y, i->second->z, i->second->look, i->second->angle, chair);
			break;
		default:
			break;
		}
	}
}

void mouse(int button, int state, int x, int y)
{
	if (showing)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (uplooking && x > 738 && y > 588 && thing > 0)
			{
				things[thing]->x = (x - 738) * 850 / 212;
				things[thing]->z = (y - 588) * 850 / 212;
				Show(pointx);
			}
			else
			{
				pointx = x;
				moving = true;
			}
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			moving = false;
		}
		return;
	}
	if (layer > 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (x > 885 && x < 915)
		{
			for (int i = 1; i <= Layer; i++)
			{
				if ((800 - y) > (880 - 110 * i) && (800 - y) < (900 - 110 * i))
				{
					open[i] = !open[i];
					Minpaint(i);
					return;
				}
			}
		}
		layers[0].clear();
		num = layer;
		layers[0].insert(layers[layer].begin(), layers[layer].end());
		step = true;
		if (x > 0 && x < 840 && y > 0 && y < 720)
		{
			switch (tools)
			{
			case 1:
				p0[0] = x, p0[1] = 800 - y;
				pen = true;
				break;
			case 2:
				p0[0] = x, p0[1] = 800 - y;
				eraser = true;
				break;
			case 3:
				pointx = x, pointy = 800 - y;
				pic = true;
				break;
			case 4:
				pointx = x, pointy = 800 - y;
				line = true;
				break;
			case 5:
				for (int i = 81; i < 800; i++)
				{
					string s = "0-" + to_string(i);
					point* p = new point;
					p->r = r, p->g = g, p->b = b;
					p->p1[0] = 839, p->p1[1] = i;
					layers[layer][s] = p;
				}
				break;
			default:
				break;
			}
		}
	}
	else if (layer > 0 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		switch (tools)
		{
		case 1:
			pen = false;
			break;
		case 2:
			eraser = false;
			break;
		case 3:
			pic = false;//最终图形存入map
			if (x > 0 && x < 840 && y > 0 && y < 720)
			{
				for (int i = 0; i < 4; i++)
				{
					int lp[4][2] = { {pointx,pointy},{pointx, 800 - y},{x, 800 - y},{x, pointy} };
					string s = to_string(lp[i][0]) + "-" + to_string(lp[i][1]);
					point* p = new point;
					p->r = r, p->g = g, p->b = b;
					p->p1[0] = lp[(i + 1) % 4][0], p->p1[1] = lp[(i + 1) % 4][1];
					layers[layer][s] = p;
				}
			}
			break;
		case 4:
			line = false;//最终图形存入map
			if (x > 0 && x < 840 && y > 0 && y < 720)
			{
				for (int i = 0; i < 1; i++)
				{
					string s = to_string(pointx) + "-" + to_string(pointy);
					point* p = new point;
					p->r = r, p->g = g, p->b = b;
					p->p1[0] = x, p->p1[1] = 800 - y;
					layers[layer][s] = p;
				}
			}
			break;
		default:
			break;
		}
	}
	Minpaint(layer);
	Repaint();
	Screen();
}

void move(int x, int y)
{
	if (moving && x > 0 && x < 950)
	{
		Show(x);
	}
	if (showing)return;
	if (y >= 735 && y <= 755)//rgb
	{
		if (x >= 150 && x <= 375)
		{
			r = x - 150;
		}
		if (x >= 425 && x <= 650)
		{
			g = x - 425;
		}
		if (x >= 700 && x <= 925)
		{
			b = x - 700;
		}
		glutPostRedisplay();
	}
	else if (x > 0 && x < 840 && y > 0 && y < 720)
	{
		if (pen)
		{
			Paint(x, y);
			p0[0] = x, p0[1] = 800 - y;//记录前置点
		}
		else if (eraser)
		{
			int a = x >= p0[0] ? 1 : -1;
			int b = y >= p0[1] ? 1 : -1;
			Eraser(x + a * 5, y - b * 5);//增大范围，效果更好
			p0[0] = x, p0[1] = 800 - y;
		}
		else if (pic)
		{
			Cube(x, y);
		}
		else if (line)
		{
			Lines(x, y);
		}
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (!showing)return;
	switch (key)
	{
	case 'a'://-x
	case 'A':
		if (thing > 0 && things[thing]->x > 0)
			things[thing]->x -= 10;
		break;
	case 'd'://+x
	case 'D':
		if (thing > 0 && things[thing]->x < 850)
			things[thing]->x += 10;
		break;
	case 'z'://-y
	case 'Z':
		if (thing > 0 && things[thing]->y > 81)
			things[thing]->y -= 10;
		break;
	case 'q'://+y
	case 'Q':
		if (thing > 0 && things[thing]->y < 800)
			things[thing]->y += 10;
		break;
	case 'w'://-z
	case 'W':
		if (thing > 0 && things[thing]->z > 0)
			things[thing]->z -= 10;
		break;
	case 's'://+z
	case 'S':
		if (thing > 0 && things[thing]->z < 850)
			things[thing]->z += 10;
		break;
	case 'r'://
	case 'R':
		if (thing > 0)
			things[thing]->angle += 90;
		break;
	case 'e'://切换家具样式
	case 'E':
		if (thing > 1 && thing != 4)
		{
			things[thing]->look = (things[thing]->look + 1) % 3;
		}
		break;
	case 'b'://切换墙纸样式
	case 'B':
		wall = (wall + 1) % 8;
		if (wall == 1)
			wall++;
		break;
	case 'f'://切换地板样式
	case 'F':
		fl = (fl + 1) % 8;
		break;
	default:
		break;
	}
	Show(pointx);
}

void menu(int data)
{
	switch (data)
	{
	case 4:
		showing = true;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glScalef(2, 2, 2);
		gluLookAt(1, 0.8, 1, 0, 0, 0, 0, 1, 0);
		pointx = 0;
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BACK);
		glEnable(GL_CULL_FACE);
		Show(0);
		break;
	case 5:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BACK);
		glDisable(GL_CULL_FACE);
		glPopMatrix();
		glPushMatrix();
		showing = false;
		break;
	case 6:
		if (step && !showing)
		{
			swap(layers[0], layers[num]);
			step = false;
		}
		break;
	case 7:
		if (!step && !showing)
		{
			swap(layers[0], layers[num]);
			step = true;
		}
		break;
	case 8:
		exit(0);
	default:
		break;
	}

	if (!showing)
	{
		Repaint();
		Screen();
		Minpaint(num);
	}
}

//图层管理
void menu_layers(int data)
{
	if (data == 1)
	{
		Layer++;
		open = (bool*)realloc(open, sizeof(bool) * (Layer + 1));
		open[Layer] = true;
		layer = Layer;
		char s[2];
		snprintf(s, sizeof(s), "%d", Layer);
		glutAddMenuEntry(s, Layer * 2);
		glutAddMenuEntry("删除图层", Layer * 2 + 1);
		Repaint();
		Screen();
	}
	else if (data % 2 == 0)//偶数选中图层
	{
		layer = data / 2;
	}
	else//除1外的奇数删除图层
	{
		for (int i = (data - 1) / 2; i < Layer; i++)//在i后的图层map向前迁移
		{
			swap(layers[i], layers[i + 1]);
			open[i] = open[i + 1];
		}
		glutRemoveMenuItem(Layer * 2);
		glutRemoveMenuItem(Layer * 2);
		//清除数据，重绘视图
		open[Layer] = false;
		layers[Layer].clear();
		for (int i = 1; i <= Layer; i++)
			Minpaint(i);
		if (layer == Layer)
			layer--;
		Layer--;
		Repaint();
		Screen();
	}
}

//工具选取
void menu_tool(int data)
{
	switch (data)
	{
	case 1:
		tools = 1;
		break;
	case 2:
		tools = 2;
		break;
	case 3:
		tools = 3;
		break;
	case 4:
		tools = 4;
		break;
	case 5:
		tools = 5;
		break;
	default:
		break;
	}
}

//摆放设置
void menu_thing(int data)
{
	if (!showing)return;
	switch (data)
	{
	case 1:
		uplooking = !uplooking;
		Show(pointx);
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		if (thing == data)//连点两次
		{
			visible[data] = false;
			things.erase(data);
			thing = 0;
			Show(pointx);
		}
		else if (thing != data && visible[data] == false)//初次摆放
		{
			thing = data;
			visible[data] = true;
			pos* p = new pos;
			things[data] = p;
			Put(400, 81, 400);
		}
		else//选中已有
		{
			thing = data;
		}
		break;
	default:
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(950, 800);
	glutCreateWindow("室内设计");

	Init();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Screen);
	glutMouseFunc(mouse);
	glutMotionFunc(move);
	glutKeyboardFunc(keyboard);

	int mlayer = glutCreateMenu(menu_layers);
	glutAddMenuEntry("新建图层", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	int tool = glutCreateMenu(menu_tool);
	glutAddMenuEntry("笔", 1);
	glutAddMenuEntry("橡皮", 2);
	glutAddMenuEntry("矩形", 3);
	glutAddMenuEntry("直线", 4);
	glutAddMenuEntry("背景填充", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	int thing = glutCreateMenu(menu_thing);
	glutAddMenuEntry("俯视放置", 1);
	glutAddMenuEntry("床", 2);
	glutAddMenuEntry("柜子", 3);
	glutAddMenuEntry("绿植", 4);
	glutAddMenuEntry("地毯", 5);
	glutAddMenuEntry("桌子", 6);
	glutAddMenuEntry("椅子", 7);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutCreateMenu(menu);
	glutAddSubMenu("图层", mlayer);
	glutAddSubMenu("工具", tool);
	glutAddSubMenu("家具", thing);
	glutAddMenuEntry("房间设计", 4);
	glutAddMenuEntry("墙纸绘制", 5);
	glutAddMenuEntry("上一步", 6);
	glutAddMenuEntry("下一步", 7);
	glutAddMenuEntry("退出", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}