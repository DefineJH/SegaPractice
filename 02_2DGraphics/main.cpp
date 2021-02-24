#include "GameLib/Framework.h"
#include <fstream>
#include <algorithm>

using namespace std;
using namespace GameLib;


struct ImgFormat
{
	unsigned height;
	unsigned width;
	unsigned* buf;
};

// 게임의 데이터 읽기 위해서 쓰는 함수
void readFile(char** buffer, int* size, const char* filename);
// 게임의 메인 루프에 사용
void mainLoop();

void DrawCells(int x, int y, unsigned int color);



void DrawCells(int x, int y, unsigned int color)
{
	unsigned int* vram = Framework::instance().videoMemory();
	unsigned int windowWidth = Framework::instance().width();

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			// * 16은 다음 픽셀들을 찍을 때 오프셋 역할을 함
			vram[(y * 16 + j)*windowWidth + (x * 16 + i)] = color;
		}
	}
}

void readFile(char** buffer, int* size, const char* filename)
{
	ifstream in(filename, ifstream::binary);
	if (!in)
	{
		*buffer = 0;
		*size = 0;
	}
	else
	{
		//seekg로 end까지 이동 \0까지
		in.seekg(0, ifstream::end);
		//teelg는 현재 포인터가 가리키는 위치 알려줌
		*size = static_cast<int>(in.tellg());
		in.seekg(0, ifstream::beg);
		*buffer = new char[*size];
		in.read(*buffer, *size);
	}
}

template< class T > class Array2D {
public:
	Array2D() : mArray(0) {}
	~Array2D() {
		delete[] mArray;
		mArray = 0;  //?귽깛?궸0귩볺귢귡궻궼긏긜궸궢귝궎갃
	}
	void setSize(int size0, int size1) {
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[size0 * size1];
	}
	T& operator()(int index0, int index1) {
		return mArray[index1 * mSize0 + index0];
	}
	const T& operator()(int index0, int index1) const {
		return mArray[index1 * mSize0 + index0];
	}
private:
	T* mArray;
	int mSize0;
	int mSize1;
};

class State {
public:
	State(const char* stageData, int size);
	void update(char input);
	void draw() const;
	bool hasCleared() const;
private:
	enum Object {
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	//파일에서 읽어온 데이터로 현재 맵의 크기 알아내는 함수
	void setSize(const char* stageData, int size);

	int mWidth;
	int mHeight;
	Array2D< Object > mObjects;
	Array2D< bool > mGoalFlags;
};

State::State(const char* stageData, int size)
{
	setSize(stageData, size);
	//봹쀱둴뺎
	mObjects.setSize(mWidth, mHeight);
	mGoalFlags.setSize(mWidth, mHeight);
	//룊딖뭠궳뻹귕궴궘
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			mObjects(x, y) = OBJ_WALL; //궇귏궯궫븫빁궼빮
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
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break;
		default: t = OBJ_UNKNOWN; break;
		}
		if (t != OBJ_UNKNOWN) { 
			mObjects(x, y) = t; 
			mGoalFlags(x, y) = goalFlag; 
			++x;
		}
	}
}

void State::update(char input)
{
	//이동하는 위치에 대해 나타냄
	int dx = 0;
	int dy = 0;
	switch (input)
	{
	case 'a':
		dx = -1; break;
	case 'd':
		dx = 1; break;
	case 'w':
		dy = -1; break;
	case 's':
		dy = 1; break;
	}

	int w = mWidth;
	int h = mHeight;
	Array2D< Object >& o = mObjects;

	int x, y;
	x = y = -1;
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

	int tx = x + dx;
	int ty = y + dy;

	if (tx < 0 || ty < 0 || tx >= w || ty >= h)
	{
		return;
	}


	//만약 이동예상위치가 빈칸이면
	if (o(tx, ty) == OBJ_SPACE)
	{
		//이동 위치는 MAN, 이동한 위치는 공백
		o(tx, ty) = OBJ_MAN;
		o(x, y) = OBJ_SPACE;
	}
	//만약 이동하려는 위치가 블록 혹은 이미 골에 들어간 블록이라면
	else if (o(tx,ty) == OBJ_BLOCK)
	{
		//이동위치의 2칸 앞 검색
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		//2칸 앞이 벽을 넘어가는지
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
		{
			return;
		}
		//2칸앞이 빈 공간일 때면
		if (o(tx2, ty2) == OBJ_SPACE)
		{
			o(tx2, ty2) = OBJ_BLOCK;
			o(tx, ty) = OBJ_MAN;
			o(tx, ty) = OBJ_SPACE;
		}
	}
}

void State::draw() const
{
	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			Object o = mObjects(x, y);
			bool goalFlag = mGoalFlags(x, y);
			unsigned int color = 0;
			if (goalFlag) {
				switch (o) {
				case OBJ_SPACE: cout << '.'; color = 0x0000ff; break;
				case OBJ_WALL: cout << '#'; color = 0xffffff; break;
				case OBJ_BLOCK: cout << 'O'; color = 0xff00ff; break;
				case OBJ_MAN: cout << 'P'; color = 0x00ffff; break;
				}
			}
			else {
				switch (o) {
				case OBJ_SPACE: cout << ' '; color = 0x00000; break;
				case OBJ_WALL: cout << '#'; color = 0xffffff; break;
				case OBJ_BLOCK: cout << 'o'; color = 0xff0000; break;
				case OBJ_MAN: cout << 'p'; color = 0x00ff00; break;
				}
				
			}
			//점 그리기
			DrawCells(x, y, color);
			
		}
		cout << endl;
	}

}

bool State::hasCleared() const
{
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

void State::setSize(const char* stageData, int size)
{
	mWidth = mHeight = 0; 
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
			mWidth = max(mWidth, x);
			mHeight = max(mHeight, y);
			x = 0;
			break;
		}
	}
}

State* gState = 0;



namespace GameLib
{
	void Framework::update()
	{
		mainLoop();
	}
}

void mainLoop()
{
	//gState 가 nullptr이면
	if (!gState)
	{
		const char* fileName = "stageData.txt";
		char* stageData = nullptr;
		int fileSize;
		readFile(&stageData, &fileSize, fileName);

		//stageData 구성 미완료시
		if (!stageData)
		{
			cout << "stage file couldn't read" << endl;
			return;
		}

		gState = new State(stageData, fileSize);

		delete[] stageData;
		stageData = 0;

		gState->draw();
		return;
	}

	bool cleared = 0;
	if (gState->hasCleared())
	{
		cleared = 1;
	}
	cout << "a:left s:right w:up z:down. command?" << endl; 

	char input;
	cin >> input;
	gState->update(input);
	gState->draw();

	if (cleared) {
		cout << "Congratulation! you win." << endl;
		delete gState;
		gState = 0;
	}



	if (input == 'q') {
		Framework::instance().requestEnd();
	}
	if (Framework::instance().isEndRequested()) {
		if (gState) {
			delete gState;
			gState = 0;
		}
		return;
	}
}


