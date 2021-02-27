#include "File.h"
#include <fstream>
#include <cassert>

File::File(const char* fileName)
{
	buf = nullptr;
	size = 0;
	ReadFile(fileName);
}


File::File()
{

}

const char* File::GetFileBuffer() const
{
	assert(buf);
	return buf;
}


void File::ReadFile(const char* fileName)
{
	using namespace std;

	ifstream open(fileName, ios::binary);

	if (open)
	{
		open.seekg(0, ifstream::end);
		size = static_cast<int>(open.tellg());
		buf = new char[size];
		open.seekg(0, ifstream::beg);
		open.read(buf, size);
	}
}
