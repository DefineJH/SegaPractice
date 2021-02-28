#include "GameLib/Framework.h"
#include "State.h"
#include "Image.h"
#include "File.h"
State::State(const char* stageData, int size) : tempFile("nimotsuKunImage2.dds") {

	prevInputW = false;
	prevInputA = false;
	prevInputS = false;
	prevInputD = false;

	setSize(stageData, size);
	mObjects.setSize(mWidth, mHeight);
	mGoalFlags.setSize(mWidth, mHeight);
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			mObjects(x,y) = OBJ_WALL; //궇귏궯궫븫빁궼빮
			mGoalFlags(x, y) = false; //긕?깑궣귗궶궋
		}
	}
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		Object t;
		bool goalFlag = false;
		switch (stageData[i]) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK; goalFlag = true; break;
		case '.': t = OBJ_SPACE; goalFlag = true; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN; goalFlag = true; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //둂뛱룉뿚
		default: t = OBJ_UNKNOWN; break;
		}
		if (t != OBJ_UNKNOWN) { //뭢귞궶궋빒럻궶귞뼰럨궥귡궻궳궞궻if빒궕궇귡
			mObjects(x, y) = t; //룕궖뜛귒
			mGoalFlags(x, y) = goalFlag; //긕?깑륃뺪
			++x;
		}
	}

	mImage = new Image(tempFile);
}

State::~State() {
	delete mImage;
	mImage = 0;
}

void State::setSize(const char* stageData, int size) {
	mWidth = mHeight = 0; //룊딖돸
	//뙸띪댧뭫
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; ++i) {
		switch (stageData[i]) {
		case '#': case ' ': case 'o': case 'O':
		case '.': case 'p': case 'P':
			++x;
			break;
		case '\n':
			++y;
			//띍묈뭠뛛륷
			mWidth = (mWidth > x) ? mWidth : x;
			mHeight = (mHeight > y) ? mHeight : y;
			x = 0;
			break;
		}
	}
}

void State::draw() const {
	using namespace GameLib;
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			Object o = mObjects(x, y);
			bool goalFlag = mGoalFlags(x, y);
			if (o != OBJ_WALL) {
				if (goalFlag) {
					drawCell(x, y, IMAGE_ID_GOAL);
				}
				else {
					drawCell(x, y, IMAGE_ID_SPACE);
				}
			}
			ImageID id = IMAGE_ID_SPACE;
			switch (o) {
			case OBJ_WALL: id = IMAGE_ID_WALL; break;
			case OBJ_BLOCK: id = IMAGE_ID_BLOCK; break;
			case OBJ_MAN: id = IMAGE_ID_PLAYER; break;
			}
			if (id != IMAGE_ID_SPACE) { 
				drawCell(x, y, id);
			}
		}
	}
}

void State::drawCell(int x, int y, ImageID id) const {
	
	int offset;
	switch (id)
	{
	case State::IMAGE_ID_PLAYER:
		offset = id;
		break;
	case State::IMAGE_ID_WALL:
		offset = id;
		break;
	case State::IMAGE_ID_BLOCK:
		offset = id;
		break;
	case State::IMAGE_ID_BLOCK_ON_GOAL:
		offset = 2;
		break;
	case State::IMAGE_ID_GOAL:
		offset = 3;
		break;
	case State::IMAGE_ID_SPACE:
		offset = 4;
		break;
	default:
		break;
	}
	mImage->Draw(x * 32, y * 32, offset * 32, 0, 32, 32);
}


void State::update() {
	//댷벍뜼빁궸빾듂
	int dx = 0;
	int dy = 0;

	using namespace GameLib;
	auto f = Framework::instance();
	bool inputW = f.isKeyOn('w');
	bool inputA = f.isKeyOn('a');
	bool inputS = f.isKeyOn('s');
	bool inputD = f.isKeyOn('d');

	//밑에서 prevInput을 설정하기 때문에 떼지 않으면 특정 구문에 진입할 수 없다
	if (inputW && (!prevInputW))
	{
		dy = -1;
	}
	else if (inputA && (!prevInputA))
	{
		dx = -1;
	}
	else if (inputD && (!prevInputD))
	{
		dx = 1;
	}
	else if (inputS && (!prevInputS))
	{
		dy = 1;
	}

	prevInputA = inputA;
	prevInputW = inputW;
	prevInputD = inputD;
	prevInputS = inputS;



	//뭒궋빾릶뼹귩궰궚귡갃
	int w = mWidth;
	int h = mHeight;
	Array2D< Object >& o = mObjects;
	//릐띆뷭귩뙚랊
	int x = -1;
	int y = -1;
	bool found = false;
	for (y = 0; y < mHeight; ++y) {
		for (x = 0; x < mWidth; ++x) {
			if (o(x, y) == OBJ_MAN) {
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
	}
	//댷벍
	//댷벍뚣띆뷭
	int tx = x + dx;
	int ty = y + dy;
	//띆뷭궻띍묈띍룷?긃긞긏갃둖귢궲궋귢궽븉떀됀
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	//A.궩궻뺴뛀궕뗴뵏귏궫궼긕?깑갃릐궕댷벍갃
	if (o(tx, ty) == OBJ_SPACE) {
		o(tx, ty) = OBJ_MAN;
		o(x, y) = OBJ_SPACE;
		//B.궩궻뺴뛀궕뵠갃궩궻뺴뛀궻렅궻?긚궕뗴뵏귏궫궼긕?깑궳궇귢궽댷벍갃
	}
	else if (o(tx, ty) == OBJ_BLOCK) {
		//2?긚먩궕붝댪볙궔?긃긞긏
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { //돓궧궶궋
			return;
		}
		if (o(tx2, ty2) == OBJ_SPACE) {
			//룈렅볺귢뫶궑
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_MAN;
			o(x, y) = OBJ_SPACE;
		}
	}
}

//긳깓긞긏궻궴궞귣궻goalFlag궕덇궰궳귖false궶귞
//귏궬긏깏귺궢궲궶궋
bool State::hasCleared() const {
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			if (mObjects(x, y) == OBJ_BLOCK) {
				if (mGoalFlags(x, y) == false) {
					return false;
				}
			}
		}
	}
	return true;
}
