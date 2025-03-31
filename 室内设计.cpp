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

//���Ʋ���
//����
bool step = false;
int num;//����������ͼ���

//ͼ��
int Layer = 0, layer = 0;//ͼ����������ǰͼ���
bool* open = (bool*)calloc(sizeof(bool), 1);//ͼ���Ƿ��
float r = 0, g = 0, b = 0;
int p0[2];//��¼�㣨��㣩

//������
int tools = 1;
bool pen = false;//1
bool eraser = false;//2
int pointx = 0, pointy = 0;//��ʼ����
bool pic = false;//3
bool line = false;//4

struct point {
	float r = 0;
	float g = 0;
	float b = 0;
	int p1[2] = { 0,0 };//��̵�
};

map<int, map<string, point*>>layers;

//չʾ����
bool showing = false;//�Ƿ��
bool moving = false;
bool uplooking = false;
GLuint basic[8];//ǽֽ�͵ذ�
int wall = 0, fl = 1;

//�ڷ�
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

int thing = 0;//ѡ��
bool visible[8] = { 0 };//�Ƿ��Ѱڷ�
map<int, pos*>things;

void picture();
void Init();
void Reshape(int w, int h);
//����
void Button(int p);
void Screen();
void Minpaint(int n);
void Repaint();
void Paint(int x, int y);
void Eraser(int x, int y);
void Cube(int x, int y);
void Lines(int x, int y);
//�ڷ�
void Uplook();
void Show(int x);
void Put(int x, int y, int z);
void Reput();
//����
void mouse(int button, int state, int x, int y);
void move(int x, int y);
void keyboard(unsigned char key, int x, int y);
//�˵�
void menu(int data);
void menu_layers(int data);
void menu_tool(int data);
void menu_thing(int data);

void picture()
{
	//ǽֽ�͵ذ�
	AUX_RGBImageRec* TextureImage1[8];
	memset(TextureImage1, 0, sizeof(void*) * 8);//��ʼ���������ڴ�
	TextureImage1[0] = auxDIBImageLoad(TEXT("wall.bmp"));//����ͼƬ�ļ�
	TextureImage1[1] = auxDIBImageLoad(TEXT("floor.bmp"));
	TextureImage1[2] = auxDIBImageLoad(TEXT("paper1.bmp"));
	TextureImage1[3] = auxDIBImageLoad(TEXT("paper2.bmp"));
	TextureImage1[4] = auxDIBImageLoad(TEXT("paper3.bmp"));
	TextureImage1[5] = auxDIBImageLoad(TEXT("paper4.bmp"));
	TextureImage1[6] = auxDIBImageLoad(TEXT("paper5.bmp"));
	TextureImage1[7] = auxDIBImageLoad(TEXT("paper6.bmp"));
	for (int i = 0; i < 8; i++)
	{
		if (TextureImage1[i])//�����Ƿ����
		{
			glGenTextures(1, &basic[i]);//��������
			glBindTexture(GL_TEXTURE_2D, basic[i]);//������
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage1[i]->sizeX, TextureImage1[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage1[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage1[i]);//�ͷ��ڴ�
		}
		else
			printf("error");
	}

	//��
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
		if (TextureImage2[i])//�����Ƿ����
		{
			glGenTextures(1, &bed[i]);//��������
			glBindTexture(GL_TEXTURE_2D, bed[i]);//������
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage2[i]->sizeX, TextureImage2[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage2[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage2[i]);//�ͷ��ڴ�
		}
		else
			printf("error");
	}

	//����
	AUX_RGBImageRec* TextureImage3[7];
	memset(TextureImage3, 0, sizeof(void*) * 7);//��ʼ���������ڴ�
	TextureImage3[0] = auxDIBImageLoad(TEXT("box1.bmp"));//����ͼƬ�ļ�
	TextureImage3[1] = auxDIBImageLoad(TEXT("box1-2.bmp"));
	TextureImage3[2] = auxDIBImageLoad(TEXT("box2.bmp"));
	TextureImage3[3] = auxDIBImageLoad(TEXT("box2-2.bmp"));
	TextureImage3[4] = auxDIBImageLoad(TEXT("box2-3.bmp"));
	TextureImage3[5] = auxDIBImageLoad(TEXT("box3.bmp"));
	TextureImage3[6] = auxDIBImageLoad(TEXT("box3-2.bmp"));
	for (int i = 0; i < 7; i++)
	{
		if (TextureImage3[i])//�����Ƿ����
		{
			glGenTextures(1, &box[i]);//��������
			glBindTexture(GL_TEXTURE_2D, box[i]);//������
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage3[i]->sizeX, TextureImage3[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage3[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage3[i]);//�ͷ��ڴ�
		}
		else
			printf("error");
	}

	//��ƿ
	AUX_RGBImageRec* TextureImage4[2];
	memset(TextureImage4, 0, sizeof(void*) * 2);//��ʼ���������ڴ�
	TextureImage4[0] = auxDIBImageLoad(TEXT("pot1.bmp"));//����ͼƬ�ļ�
	TextureImage4[1] = auxDIBImageLoad(TEXT("pot1-2.bmp"));
	for (int i = 0; i < 2; i++)
	{
		if (TextureImage4[i])//�����Ƿ����
		{
			glGenTextures(1, &pot[i]);//��������
			glBindTexture(GL_TEXTURE_2D, pot[i]);//������
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage4[i]->sizeX, TextureImage4[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage4[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage4[i]);//�ͷ��ڴ�
		}
		else
			printf("error");
	}

	//����
	AUX_RGBImageRec* TextureImage6[3];
	memset(TextureImage6, 0, sizeof(void*) * 3);
	TextureImage6[0] = auxDIBImageLoad(TEXT("table1.bmp"));
	TextureImage6[1] = auxDIBImageLoad(TEXT("table2.bmp"));
	TextureImage6[2] = auxDIBImageLoad(TEXT("table3.bmp"));
	for (int i = 0; i < 3; i++)
	{
		if (TextureImage6[i])//�����Ƿ����
		{
			glGenTextures(1, &table[i]);//��������
			glBindTexture(GL_TEXTURE_2D, table[i]);//������
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage6[i]->sizeX, TextureImage6[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage6[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage6[i]);//�ͷ��ڴ�
		}
		else
			printf("error");
	}

	//��̺
	AUX_RGBImageRec* TextureImage5[3];
	memset(TextureImage5, 0, sizeof(void*) * 3);
	TextureImage5[0] = auxDIBImageLoad(TEXT("blanket1.bmp"));
	TextureImage5[1] = auxDIBImageLoad(TEXT("blanket2.bmp"));
	TextureImage5[2] = auxDIBImageLoad(TEXT("blanket3.bmp"));
	for (int i = 0; i < 3; i++)
	{
		if (TextureImage5[i])//�����Ƿ����
		{
			glGenTextures(1, &blanket[i]);//��������
			glBindTexture(GL_TEXTURE_2D, blanket[i]);//������
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage5[i]->sizeX, TextureImage5[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage5[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage5[i]);//�ͷ��ڴ�
		}
		else
			printf("error");
	}

	//����
	AUX_RGBImageRec* TextureImage7[4];
	memset(TextureImage7, 0, sizeof(void*) * 4);
	TextureImage7[0] = auxDIBImageLoad(TEXT("chair1.bmp"));
	TextureImage7[1] = auxDIBImageLoad(TEXT("chair1-2.bmp"));
	TextureImage7[2] = auxDIBImageLoad(TEXT("chair2.bmp"));
	TextureImage7[3] = auxDIBImageLoad(TEXT("chair3.bmp"));
	for (int i = 0; i < 4; i++)
	{
		if (TextureImage7[i])//�����Ƿ����
		{
			glGenTextures(1, &chair[i]);//��������
			glBindTexture(GL_TEXTURE_2D, chair[i]);//������
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage7[i]->sizeX, TextureImage7[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage7[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			free(TextureImage7[i]);//�ͷ��ڴ�
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
	glLoadIdentity();//�ظ�ԭʼ����ϵ������ԭ��Ϊ��0,0��

	glOrtho(-w, w, -h, h, -4000, 4000);//���ң��£��ϣ���ǰ
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

//ҳ�沼��
void Screen()
{
	glScissor(0, 0, 950, 70);//ֻ�ػ���ɫѡ������
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

	glLineWidth(1);//С����
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

//С����
void Minpaint(int n)
{
	glScissor(850, 800 - 111 * n, 100, 110);//ֻ�ػ�Сͼ��
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

//�ػ�
void Repaint()
{
	glScissor(0, 70, 850, 730);//ֻ�ػ滭ͼ����
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

//����
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

//��Ƥ��
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
		if (((ps[0] >= x) ? 1 : 0) != ((ps[0] >= p0[0]) ? 1 : 0) && ((ps[1] >= (800 - y)) ? 1 : 0) != ((ps[1] >= p0[1]) ? 1 : 0)) {}//ps�Ƿ��ڷ�Χ��
		else if (((pe[0] >= x) ? 1 : 0) != ((pe[0] >= p0[0]) ? 1 : 0) && ((pe[1] >= (800 - y)) ? 1 : 0) != ((pe[1] >= p0[1]) ? 1 : 0)) {}//pe�Ƿ��ڷ�Χ��
		else
			continue;
		if (((k < float(p0[1] - ps[1]) / (p0[0] - ps[0])) ? 1 : 0) != ((k < float(p0[1] - pe[1]) / (p0[0] - pe[0])) ? 1 : 0))//�ж��Ƿ��н���
		{
			layers[layer].erase(i->first);
		}
	}
	Repaint();
	Screen();
}

//����
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

//ֱ��
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

//����ͼ
void Uplook()
{
	glScissor(738, 0, 212, 212);//ֻ�ػ滭ͼ����
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	glPushMatrix();
	glLoadIdentity();
	gluLookAt(0, 1, 0, 0, 0, 0, 0, 0, -1);
	glTranslatef(525, 0, 375);
	glScalef(0.5, 0.5, 0.5);
	glEnable(GL_BACK);
	glEnable(GL_CULL_FACE);//���òü�
	glEnable(GL_TEXTURE_2D);//����2D������
	glBindTexture(GL_TEXTURE_2D, basic[fl]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 0, 0);//����
	glTexCoord2f(1.0, 0.0); glVertex3f(0, 0, 850);//����
	glTexCoord2f(1.0, 1.0); glVertex3f(850, 0, 850);//����
	glTexCoord2f(0.0, 1.0); glVertex3f(850, 0, 0);//����
	glEnd();
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	Reput();
	glPopMatrix();
}

//����չʾ
void Show(int x)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRotatef(float(x - pointx) / 200, 0, 1, 0);

	glEnable(GL_TEXTURE_2D);//����2D������
	glBindTexture(GL_TEXTURE_2D, basic[wall]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, 80, 0);//����
	glTexCoord2f(1.0, 0.0); glVertex3f(850, 80, 0);//����
	glTexCoord2f(1.0, 1.0); glVertex3f(850, 800, 0);//����
	glTexCoord2f(0.0, 1.0); glVertex3f(0, 800, 0);//����

	glTexCoord2f(0.0, 0.0); glVertex3f(0, 80, 850);//����
	glTexCoord2f(1.0, 0.0); glVertex3f(0, 80, 0);//����
	glTexCoord2f(1.0, 1.0); glVertex3f(0, 800, 0);//����
	glTexCoord2f(0.0, 1.0); glVertex3f(0, 800, 850);//����
	glEnd();

	if (fl > 5)
	{
		glBindTexture(GL_TEXTURE_2D, basic[fl]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0, 80, 0);//����
		glTexCoord2f(5.0, 0.0); glVertex3f(0, 80, 850);//����
		glTexCoord2f(5.0, 5.0); glVertex3f(850, 80, 850);//����
		glTexCoord2f(0.0, 5.0); glVertex3f(850, 80, 0);//����
		glEnd();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, basic[fl]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0, 80, 0);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(0, 80, 850);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(850, 80, 850);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(850, 80, 0);//����
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

//�ڷ�
void Put(int x, int y, int z)//�ײ��е�λ��
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

//����
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
			pic = false;//����ͼ�δ���map
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
			line = false;//����ͼ�δ���map
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
			p0[0] = x, p0[1] = 800 - y;//��¼ǰ�õ�
		}
		else if (eraser)
		{
			int a = x >= p0[0] ? 1 : -1;
			int b = y >= p0[1] ? 1 : -1;
			Eraser(x + a * 5, y - b * 5);//����Χ��Ч������
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
	case 'e'://�л��Ҿ���ʽ
	case 'E':
		if (thing > 1 && thing != 4)
		{
			things[thing]->look = (things[thing]->look + 1) % 3;
		}
		break;
	case 'b'://�л�ǽֽ��ʽ
	case 'B':
		wall = (wall + 1) % 8;
		if (wall == 1)
			wall++;
		break;
	case 'f'://�л��ذ���ʽ
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

//ͼ�����
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
		glutAddMenuEntry("ɾ��ͼ��", Layer * 2 + 1);
		Repaint();
		Screen();
	}
	else if (data % 2 == 0)//ż��ѡ��ͼ��
	{
		layer = data / 2;
	}
	else//��1�������ɾ��ͼ��
	{
		for (int i = (data - 1) / 2; i < Layer; i++)//��i���ͼ��map��ǰǨ��
		{
			swap(layers[i], layers[i + 1]);
			open[i] = open[i + 1];
		}
		glutRemoveMenuItem(Layer * 2);
		glutRemoveMenuItem(Layer * 2);
		//������ݣ��ػ���ͼ
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

//����ѡȡ
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

//�ڷ�����
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
		if (thing == data)//��������
		{
			visible[data] = false;
			things.erase(data);
			thing = 0;
			Show(pointx);
		}
		else if (thing != data && visible[data] == false)//���ΰڷ�
		{
			thing = data;
			visible[data] = true;
			pos* p = new pos;
			things[data] = p;
			Put(400, 81, 400);
		}
		else//ѡ������
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
	glutCreateWindow("�������");

	Init();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Screen);
	glutMouseFunc(mouse);
	glutMotionFunc(move);
	glutKeyboardFunc(keyboard);

	int mlayer = glutCreateMenu(menu_layers);
	glutAddMenuEntry("�½�ͼ��", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	int tool = glutCreateMenu(menu_tool);
	glutAddMenuEntry("��", 1);
	glutAddMenuEntry("��Ƥ", 2);
	glutAddMenuEntry("����", 3);
	glutAddMenuEntry("ֱ��", 4);
	glutAddMenuEntry("�������", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	int thing = glutCreateMenu(menu_thing);
	glutAddMenuEntry("���ӷ���", 1);
	glutAddMenuEntry("��", 2);
	glutAddMenuEntry("����", 3);
	glutAddMenuEntry("��ֲ", 4);
	glutAddMenuEntry("��̺", 5);
	glutAddMenuEntry("����", 6);
	glutAddMenuEntry("����", 7);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutCreateMenu(menu);
	glutAddSubMenu("ͼ��", mlayer);
	glutAddSubMenu("����", tool);
	glutAddSubMenu("�Ҿ�", thing);
	glutAddMenuEntry("�������", 4);
	glutAddMenuEntry("ǽֽ����", 5);
	glutAddMenuEntry("��һ��", 6);
	glutAddMenuEntry("��һ��", 7);
	glutAddMenuEntry("�˳�", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}