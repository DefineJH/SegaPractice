#pragma once
class File
{
public:
	File();
	File(const char* fileName);
	const char* GetFileBuffer() const;
	inline const int GetFileSize() const { return size; }
private:
	void ReadFile(const char* fileName);
private:
	char* buf;
	int size;
};

