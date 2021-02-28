#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "Array2D.h"
#include "File.h"
class Image; 

class State {
public:
	State(const char* stageData, int size);
	~State();
	void update();
	void draw() const;
	bool hasCleared() const;
private:
	class Object
	{
		enum Type {
			OBJ_SPACE,
			OBJ_WALL,
			OBJ_BLOCK,
			OBJ_MAN,

			OBJ_UNKNOWN,
		};

		Type mType;
		bool bGoalFlag;
		int iMoveX;
		int iMoveY;
	};
	enum ImageID {
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_BLOCK_ON_GOAL,
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};
	void setSize(const char* stageData, int size);
	void drawCell(int x, int y, ImageID) const;
	File tempFile;

	int mWidth;
	int mHeight;

	bool prevInputW;
	bool prevInputA;
	bool prevInputS;
	bool prevInputD;


	Array2D<Object> mObjects;
	Array2D< bool > mGoalFlags;
	Image* mImage; //‰æ‘œ
};

#endif
