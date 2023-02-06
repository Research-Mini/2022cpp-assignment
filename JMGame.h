#pragma once

#include <string>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;

class JMGame
{
private:
	//윈도우 api 적용, 입력값 저장을 위한 변수들
	int x = 0;
	int y = 0;
	int beforenum = 0;
	int num = 0;
	int num2 = 0;
	int x1 = 0;
	int y1 = 2;
	int y2 = 3;
	int pansize = 0;//판의 크기 저장
	bool gomenu = false;//ESC키 입력 여부 판단
	//게임을 진행을 위한 벡터들(2차원 배열)
	vector<vector<int>> Userpan;

	vector<vector<int>> Compan;
	//파일들
	string filename = "userpan.txt";
	string filename2 = "compan.txt";
	string filename3 = "replica.txt";
	string filename4 = "save.txt";
	//빙고 개수, 누가 이겼는지 확인을 위한 변수들
	int uwin = 0;
	int cwin = 0;

	int ubingo = 0;
	int cbingo = 0;

	int rungame = 0;
	//리플레이를 위한 벡터들.
	vector<vector<int>> replicauser;//Userpan 저장본
	vector<vector<int>> replicacom;//Compan 저장본
	vector<int> nlist;//사용자가 입력한 것을 저장하는 nlist.

public:
	JMGame();
	~JMGame();
	void setCursorView(bool visible);
	void gotoxy(int x, int y);
	void play();
	void menu();//메뉴함수


	void startgame();//새 게임 선택 시 수행
	void continue_game();//게임은 본격적으로 진행해주는 함수, 이어하기 선택 시에도 수행
	void controlbingo();//승패여부
	void usertime();//유저가 입력할 때 수행하는 함수
	void comtime();//컴퓨터가 입력할 때 수행하는 함수

	int checkBingo(vector<vector<int>>(&arr));
	//빙고 개수를 구하는 함수

	void Bingopan();//새 빙고판을 만들어주는 함수
	void savepanuser();//Userpan 저장해주는 함수
	void savepancom();//Compan 저장해주는 함수
	void savereplica();//replicauser,replicacom 저장
	void savenlist();//nlist를 save.txt에 저장
	void showpan();//빙고판을 출력해주는 함수
	void loadpanuser();//userpan.txt를 읽어와서 Userpan에 저장해주는 함수
	void loadpancom();//compan.txt를 읽어와서 Compan에 저장해주는 함수
	void loadreplica();//replica.txt를 읽어와서 replicauser와 replicacom에 각각 저장해주는 함수
	void loadnlist();//save.txt를 읽어와서 nlist 에 저장해주는 함수
	void replay();//리플레이 추가 구현을 위한 함수

};

