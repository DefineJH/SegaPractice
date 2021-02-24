#include <iostream>
#include <fstream>
using namespace std;

//g for Global variable
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};

void initalize(Object* state, int w, int h, const char* stageData);
void Draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);


int main()
{
	//열거형도 하나의 형이므로 new 사용하여 생성가능
	Object* state = new Object[gStageWidth * gStageHeight];
	initalize(state, gStageWidth, gStageHeight, gStageData);

	while (1)
	{
		Draw(state, gStageWidth, gStageHeight);
		if (checkClear(state, gStageWidth, gStageHeight))
		{
			break;
		}
		cout << "press w : up, a : left , s : down, d : right...... and press enter : " ;
		char input;
		cin >> input;
		update(state, input, gStageWidth, gStageHeight);
	}

	cout << "end";
	//배열이므로 delete[]
	delete[] state;
	state = nullptr;
	return 0;
}

void initalize(Object* state, int w, int h, const char* stageData)
{
	const char const * d = stageData;
	int x = 0, y = 0;

	while (*d != '\0')
	{
		Object t;
		switch (*d)
		{
		case '#':
			t = OBJ_WALL; break;
		case ' ':
			t = OBJ_SPACE; break;
		case '.':
			t = OBJ_GOAL; break;
		case 'o':
			t = OBJ_BLOCK; break;
		case 'O':
			t = OBJ_BLOCK_ON_GOAL; break;
		case 'p':
			t = OBJ_MAN; break;
		case 'P':
			t = OBJ_MAN_ON_GOAL; break;
		case '\n':
			x = 0;
			++y;
			t = OBJ_UNKNOWN;
			break;
		default:
			t = OBJ_UNKNOWN; break;
		}
		//포인터의 ++ 기능은 iterator의 역할
		++d;
		if (t != OBJ_UNKNOWN)
		{
			state[y * w + x] = t;
			x++;
		}
	}
}

bool checkClear(const Object* state, int w, int h)
{
	int count = 0;
	for (int i = 0; i < w*h; i++)
	{
		if (state[i] == Object::OBJ_BLOCK_ON_GOAL)
			count++;
	}
	return (count == 2) ? true : false;
}


void Draw(const Object* state, int w, int h)
{
	const char font[] = { ' ', '#', '.','o','O','p','P' };
	// i = y, j = x
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			Object o = state[i * w + j];
			//Object 는 열거형이고 이는 0,1,2...순서를 가진다
			cout << font[o];
		}
		cout << endl;
	}
}


void update(Object* state, char input, int w, int h)
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

	//플레이어 위치 검색
	int playerPos = 0;
	for (; playerPos < w*h; playerPos++)
	{
		if (state[playerPos] == OBJ_MAN || state[playerPos] == OBJ_MAN_ON_GOAL)
			break;
	}
	int x = playerPos % w;
	int y = playerPos / w;

	int tx = x + dx;
	int ty = y + dy;

	if (tx < 0 || ty < 0 || tx >= w || ty >= h)
	{
		return;
	}

	int p = y * w + x; // 현재위치
	int tp = ty * w + tx; //이동예상위치

	//만약 이동예상위치가 빈칸 혹은 골이면 이동
	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	//만약 이동하려는 위치가 블록 혹은 이미 골에 들어간 블록이라면
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
	{
		//이동위치의 2칸 앞 검색
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		//2칸 앞이 벽을 넘어가는지
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h)
		{
			return;
		}
		int tp2 = (ty + dy) * w + (tx + dx);
		//2칸앞이 골이거나 빈 공간일 때면
		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
		{
			state[tp2] = (state[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}

}

