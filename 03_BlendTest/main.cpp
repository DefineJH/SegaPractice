#include "GameLib/Framework.h"
#include "Image.h"
#include "File.h"

using namespace GameLib;

void MainLoop();

namespace GameLib
{
	void Framework::update()
	{
		MainLoop();
	}
}


File* file1 = 0;
File* file2 = 0;
Image* img1 = 0;
Image* img2 = 0;

void MainLoop()
{

	if (!file1)
	{
		file1 = new File("background.dds");
	}
	if (!file2)
	{
		file2 = new File("background.dds");
	}
	if (!img1)
	{
		img1 = new Image(*file1);
	}
	if (!img2)
	{
		img2 = new Image(*file2);
	}

	img1->Draw(0, 0, 0, 0, 128, 128);
	img2->Draw(0, 0, 0, 0, 128, 128);
}