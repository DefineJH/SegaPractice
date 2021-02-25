#include "GameLib/Framework.h"
#include <fstream>
#include <algorithm>

using namespace std;
using namespace GameLib;


struct ImgFormat
{
	unsigned height;
	unsigned width;
	unsigned* buf;
};

// ������ ������ �б� ���ؼ� ���� �Լ�
void readFile(char** buffer, int* size, const char* filename);
// ������ ���� ������ ���
void mainLoop();

void DrawCells(int x, int y, unsigned int color);

unsigned getUnsigned(const char *p);

ImgFormat readDDS(char * fileImage);

void DrawImage(const char* filePath);




ImgFormat readDDS(const char* filepath)
{
	char* ddsBuffer; int filesize;
	readFile(&ddsBuffer, &filesize, filepath);

	int height = getUnsigned(&ddsBuffer[12]);
	int width = getUnsigned(&ddsBuffer[16]);


	ImgFormat img;

	img.height = height;
	img.width = width;


	img.buf = new unsigned[height * width];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			img.buf[y * width + x] = getUnsigned(&ddsBuffer[128 + ( (y * width + x) * 4)]);
		}
	}

	return img;

}

void DrawImage(ImgFormat img)
{
	unsigned* vram = Framework::instance().videoMemory();

	unsigned windowWidth = Framework::instance().width();
	unsigned windowHeight = Framework::instance().height();
	
	for (unsigned y = 0; y < img.height; y++)
	{
		for (unsigned x = 0; x < img.width	; x++)
		{
			vram[y * windowWidth + x] = img.buf[y * img.width + x];
		}
	}

}


void readFile(char** buffer, int* size, const char* filename)
{
	ifstream in(filename, ifstream::binary);
	if (!in)
	{
		*buffer = 0;
		*size = 0;
	}
	else
	{
		//seekg�� end���� �̵� \0����
		in.seekg(0, ifstream::end);
		//teelg�� ���� �����Ͱ� ����Ű�� ��ġ �˷���
		*size = static_cast<int>(in.tellg());
		in.seekg(0, ifstream::beg);
		*buffer = new char[*size];
		in.read(*buffer, *size);
	}
}




namespace GameLib
{
	void Framework::update()
	{
		mainLoop();
	}
}

ImgFormat img;
bool gfirst = false;
void mainLoop()
{	
	if (!gfirst)
	{
		img = readDDS("bar.dds");
		gfirst = true;
	}
	DrawImage(img);

}

//unsigned char* �� �̿��� p�������� unsigned int�� �޴´�
unsigned getUnsigned(const char *p)
{
	const unsigned char* up;
	//����Ű�� ��ġ�� ����
	up = reinterpret_cast<const unsigned char*>(p);
	//ù��° char�� �޾ƿ�
	unsigned ret = up[0];
	//2���� char..
	ret |= (up[1] << 8);
	ret |= (up[2] << 16);
	ret |= (up[3] << 24);
	//4���� char�� ������ unsigned�� �ȴ�.
	//DDS�� ��Ʋ ����� ����� ����ϴ� �����̹Ƿ� ù��° ����Ʈ�� 1�ڸ� ���� ����Ʈ�� 256�ڸ�....�� �̷�����ִ�.

	return ret;
}


