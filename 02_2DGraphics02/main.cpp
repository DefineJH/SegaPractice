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

// 게임의 데이터 읽기 위해서 쓰는 함수
void readFile(char** buffer, int* size, const char* filename);
// 게임의 메인 루프에 사용
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
		//seekg로 end까지 이동 \0까지
		in.seekg(0, ifstream::end);
		//teelg는 현재 포인터가 가리키는 위치 알려줌
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

//unsigned char* 를 이용해 p번지에서 unsigned int를 받는다
unsigned getUnsigned(const char *p)
{
	const unsigned char* up;
	//가리키는 위치는 동일
	up = reinterpret_cast<const unsigned char*>(p);
	//첫번째 char를 받아옴
	unsigned ret = up[0];
	//2번쨰 char..
	ret |= (up[1] << 8);
	ret |= (up[2] << 16);
	ret |= (up[3] << 24);
	//4개의 char가 합쳐져 unsigned가 된다.
	//DDS는 리틀 엔디언 방식을 사용하는 파일이므로 첫번째 바이트가 1자리 다음 바이트가 256자리....로 이루어져있다.

	return ret;
}


