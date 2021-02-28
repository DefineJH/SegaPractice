#include "Image.h"
#include "GameLib/Framework.h"
#include "File.h"
#include <cassert>
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
			unsigned* dst = &screen[(posY + y) * windowWidth + (posX + x)];
			unsigned src = data[(y + srcStrY) * this->width + (x + srcStrX)];
			
			unsigned srcA = (src & 0xff000000) >> 24;
			unsigned srcB = (src & 0x00ff0000) >> 16;
			unsigned srcG = (src & 0x0000ff00) >> 8;
			unsigned srcR = (src & 0x000000ff);

			unsigned dstB = (*dst & 0x00ff0000) >> 16;
			unsigned dstG = (*dst & 0x0000ff00) >> 8;
			unsigned dstR = (*dst & 0x000000ff);

			unsigned r = (srcR - dstR)* srcA / 255 + dstR;
			unsigned g = (srcG - dstG)* srcA / 255 + dstG;
			unsigned b = (srcB - dstB)* srcA / 255 + dstB;

			*dst = (r << 16) | (g << 8) | b;
		}
	}
}

unsigned* Image::GetData()
{
	assert(data);
	return data;
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

