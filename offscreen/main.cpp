/******************************************************************************
 *
 *       Filename:  main.cpp
 *
 *    Description:  test opengl
 *    g++ main.cpp -o firstOpenGlApp -lglut -lGLU -lGL
 *
 *        Version:  1.0
 *        Created:  2021年09月05日 10时47分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  yangkun (yk)
 *          Email:  xyyangkun@163.com
 *        Company:  yangkun.com
 *
 *****************************************************************************/
#include <GL/glut.h>
#include <stdio.h>


#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>


// bmp头
#define BM 19778                    // The ASCII code for BM
#define sizeofFILE 14
#define sizeofINFO 40
#pragma pack(1)
typedef struct _tagBMP_FILEHEADER
{ 
unsigned short bfType;        //2Bytes，必须为"BM"，即0x424D 才是Windows位图文件
unsigned int   bfSize;         //4Bytes，整个BMP文件的大小
unsigned short bfReserved1;  //2Bytes，保留，为0
unsigned short bfReserved2;  //2Bytes，保留，为0
unsigned int   bfOffBits;     //4Bytes，文件起始位置到图像像素数据的字节偏移量
} BMP_FILEHEADER;
 
typedef struct _tagBMP_INFOHEADER
{
unsigned int     biSize;    //4Bytes，INFOHEADER结构体大小，存在其他版本I NFOHEADER，用作区分
unsigned int     biWidth;    //4Bytes，图像宽度（以像素为单位）
unsigned int     biHeight;    //4Bytes，图像高度，+：图像存储顺序为Bottom2Top，-：Top2Bottom
unsigned short   biPlanes;    //2Bytes，图像数据平面，BMP存储RGB数据，因此总为1
unsigned short   biBitCount;         //2Bytes，图像像素位数
unsigned int     biCompression;     //4Bytes，0：不压缩，1：RLE8，2：RLE4
unsigned int     biSizeImage;       //4Bytes，4字节对齐的图像数据大小
unsigned int     biXPelsPerMeter;   //4 Bytes，用象素/米表示的水平分辨率
unsigned int     biYPelsPerMeter;   //4 Bytes，用象素/米表示的垂直分辨率
unsigned int     biClrUsed;          //4 Bytes，实际使用的调色板索引数，0：使用所有的调色板索引
unsigned int     biClrImportant;     //4 Bytes，重要的调色板索引数，0：所有的调色板索引都重要
}BMP_INFOHEADER;
#pragma pack()

int w = 400;
int h = 300;


void *pixel_data;

int bmp_write()
{
	// 打开文件
	int fd = open("test.bmp", O_CREAT|O_WRONLY, 0644);
	if(-1 == fd)
	{
		printf("ERROR to open file:%s\n", strerror(errno));
		return -1;
	}
	BMP_FILEHEADER head = {0};
	BMP_INFOHEADER info = {0};
	head.bfType = BM;
	// 640*480 bmp 1555
	head.bfSize = sizeofFILE + sizeofINFO + w*h*3;
	head.bfOffBits = sizeofFILE + sizeofINFO;

	info.biSize = sizeofINFO;
	info.biWidth = w;
	info.biHeight = h;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biSizeImage = w*h*3;
	info.biXPelsPerMeter = w;
	info.biYPelsPerMeter = h;

	// 写文件
	write(fd, &head, sizeof(head));
	write(fd, &info, sizeof(info));
	write(fd, pixel_data, w*h*3);

	close(fd);
}

void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.5, 0.0, 0.5);

		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.5, 0.0, 0.0);

		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0, 0.5, 0.0);

        glColor3f(1.0f, 1.0f, 1.0f); glVertex3f(0.0, 0.0, 0.5);
    glEnd();

	glRasterPos2i(1, 1); 

	//glPixelZoom(-0.5, -0.5);


	glCopyPixels(w/2, h/2, w/2, h/2, GL_COLOR);
	glCopyPixels(0, 0, w, h, GL_COLOR);

	// 完成绘制 
	glutSwapBuffers(); 

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); 
	glReadPixels(0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixel_data);

	bmp_write();
    glFlush();

	// 退出
	exit(0);
}

int main(int argc, char** argv)
{

	// 初始化内存
	pixel_data = malloc(w*h*3);
	if(!pixel_data) {
		printf("ERROR to malloc\n");
		return -1;
	}

    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("off screen");
    glutDisplayFunc(displayMe);
    glutMainLoop();

	if(pixel_data)
		free(pixel_data);
    return 0;
}
