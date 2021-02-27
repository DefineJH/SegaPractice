#include "Image.h"
#include "GameLib/Framework.h"
#include "File.h"
#include <fstream>

Image::Image(File fileImage)
{
	const char* buf = fileImage.GetFileBuffer();

	height = static_cast<unsigned short>(GetUnsigned(&buf[12]));
	width = static_cast<unsigned short>(GetUnsigned(&buf[16]));

	data = new unsigned[width * height];

	for(int i = 0; i < height * width; i++)
	{
		data[i] = GetUnsigned(&buf[128 + i * 4]);
	}
}



void Image::Draw(unsigned posX, unsigned posY, unsigned srcStrX, unsigned srcStrY, unsigned width, unsigned height) const
{
	using namespace GameLib;
	unsigned* screen = Framework::instance().videoMemory();
	unsigned short windowWidth = Framework::instance().width();
	unsigned short windowHeight = Framework::instance().height();

	for (unsigned y = 0; y < height; y++)
	{
		for (unsigned x = 0; x < width; x++)
		{
			//ABGR
			unsigned src = data[(y + srcStrY) * this->width + (x + srcStrX)];
			//0xff000000과 &연산을 해서 A채널만 보존 후 끌어내려 정수로 만든다.
			if (src & 0x80000000)
			{
				screen[(posY + y) * windowWidth + (posX + x)] = src;
			}

		}
	}
}

unsigned Image::GetUnsigned(const char* p)
{
	unsigned val = 0;
	const unsigned char* up;
	up = reinterpret_cast<const unsigned char*>(p);

	//R
	val = up[0];
	//G
	val |= (up[1] << 8);
	//B
	val |= (up[2] << 16);
	//A
	val |= (up[3] << 24);

	return val;
}

