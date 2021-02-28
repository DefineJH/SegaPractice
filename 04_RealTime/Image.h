#pragma once

class File;

class Image
{
public:
	Image(File fileImage);
	~Image() { delete[] data; }
	void Draw(unsigned posX, unsigned posY, unsigned srcStrX, unsigned srcStrY, unsigned srcDstX,unsigned srcDstY) const;
	unsigned* GetData();
private:
	unsigned GetUnsigned(const char* p);
private:
	unsigned short width;
	unsigned short height;
	unsigned* data;
};

