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
	file1 = new File("background.dds");
	file2 = new File("forground.dds");


	img1 = new Image(*file1);
	img2 = new Image(*file2);

	img1->Draw(0, 0, 0, 0, 128, 128);
	img2->Draw(0, 0, 0, 0, 128, 128);
}