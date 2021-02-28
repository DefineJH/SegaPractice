#include "GameLib/Framework.h"
#include "State.h"
#include "File.h"

State* gState = 0;
using namespace GameLib;

void MainLoop();

File file("stageData.txt");


namespace GameLib
{
	void Framework::update()
	{
		MainLoop();
	}
}

void MainLoop()
{
	if ( Framework::instance().isEndRequested() ){
		if ( gState ){
			delete gState;
			gState = 0;
		}
		return;
	}
	if ( !gState ){ 
		if ( !( file.GetFileBuffer() ) ){ 
			cout << "stage file could not be read." << endl;
			return;
		}
		gState = new State( file.GetFileBuffer(), file.GetFileSize() );
		gState->draw();
		return; 
	}
	bool cleared = false;
	if ( gState->hasCleared() ){
		cleared = true;
	}
	cout << "a:left s:right w:up s:down. command?" << endl; 
	
	if ( Framework::instance().isKeyOn('q') ){
		delete gState;
		gState = 0;
		Framework::instance().requestEnd();
		return;
	}
	gState->update();
	gState->draw();

	if ( cleared ){
		cout << "Congratulation! you win." << endl;
		delete gState;
		gState = 0;
	}
}