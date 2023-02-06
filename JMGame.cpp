#include "JMGame.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

//윈도우 api 함수
void JMGame::setCursorView(bool visible) {
    CONSOLE_CURSOR_INFO cursor = { 1,visible };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

}

void JMGame::gotoxy(int x, int y) {
    COORD Pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);

}





JMGame::JMGame()
{

}

JMGame::~JMGame()
{


}

void JMGame::play()
{
    /*EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
    DrawMenuBar(GetConsoleWindow());*/
    //비정상적인 종료를 막기 위한 종료키 제거 코드입니다.
    //혹시 몰라서 넣음.

    menu();

}

void JMGame::menu()
{


    int menunum = 99;

    while (true) {
        int keycode = 0;
        system("cls");
        while (true) {
            gotoxy(x, y);
            cout << "학번 : 201911209 이름: 장 민" << endl;

            cout << "1)새 게임" << "\t" << "2) 이어하기" << "\t" << "3) 종료" << endl;

            gotoxy(x1, y1);
            setCursorView(false);
            if (_kbhit()) {
                int keycode = _getch();
                if (keycode == 224) {
                    system("cls");
                    keycode = _getch();
                    if (x1 < 0 || 16 < x1) {
                        x1 = 0;
                        y1 = 2;
                    }
                    if (y1 < 1 || 5 < y1) {
                        x1 = 0;
                        y1 = 2;
                    }
                    //커서가 메뉴판 범위를 벗어나지 않도록 설정
                    switch (keycode) {
                    case 75: x1 -= 16; break;
                    case 77: x1 += 16; break;
                        //바로 메뉴 숫자에 커서가 이동하도록 조절


                    }
                    gotoxy(x1, y1);
                    cout << "^" << endl;
                }
                else if (keycode == 13) {
                    //엔터 입력 시 그 위치 실행
                    menunum = x1;
                }


                if (menunum == 0 || menunum == 16 || menunum == 32) {
                    break;
                }


            }

        }
        if (menunum == 0) {//새 게임 선택
            menunum = 99;
            rungame = 0;
            startgame();


        }
        else if (menunum == 16) {//이어하기 선택
            loadnlist();
            if (nlist.size() == 0) {
                //nlist의 크기가 0이면 게임이 종료되었거나 게임이 제대로 시작되지 않았으니 실행 안하도록함.
                rungame = 1;
            }
            menunum = 99;
            if (rungame != 0) {
                cout << "이미 끝난 게임입니다! " << endl;
                cin.clear();
                cin.ignore(1024, '\n');
                system("pause");

            }
            else {
                //rungame이 0이면 게임 실행
                continue_game();

            }
        }
        else if (menunum == 32) {//종료 선택
            menunum = 99;
            cout << "바이" << endl;
            savenlist();

            //메모리누수방지를 위한 처리들
            Userpan.clear();
            Compan.clear();
            replicauser.clear();
            replicacom.clear();
            nlist.clear();
            Userpan.shrink_to_fit();
            Compan.shrink_to_fit();
            replicauser.shrink_to_fit();
            replicacom.shrink_to_fit();
            nlist.shrink_to_fit();


            break;
        }
        else if (menunum < 1 || 3 < menunum) {
            cout << "학번 : 201911209 이름 : 장 민" << endl;
            cout << "잘못입력했습니다. 다시 입력 : " << endl;
            cin.clear();
            cin.ignore(1024, '\n');

        }
        else {
            cout << "학번 : 201911209 이름 : 장 민" << endl;
            cout << "잘못입력했습니다. 다시 입력 : " << endl;
            cin.clear();
            cin.ignore(1024, '\n');
        }

    }





}




void JMGame::startgame()//새 게임 시작
{
    Bingopan();
    //빙고판을 동적할당받아 생성해주는 함수
    ubingo = 0;
    cbingo = 0;
    continue_game();




}

void JMGame::continue_game()//이어하기 및 본격적인 게임 수행해주는 함수
{



    do {
        system("cls");


        loadpanuser();
        loadpancom();

        showpan();




        if (rungame == 0) {
            usertime();
            //user 선택에서 user가 빙고나오면 rungame이 1이 됨
        }

        if (rungame == 0 && gomenu == false) {
            comtime();
            //com 선택에서 user가 빙고나오면 rungame이 1이 됨
        }
        if (rungame != 0) {

            //누군가가 이겨서 rungame이 0이 아니면 리플레이 수행
            replay();
            break;
        }
        if (gomenu) {//ESC키를 누르면 menu로 돌아가도록
            gomenu = false;

            break;
        }







    } while (true);


}

void JMGame::controlbingo()//누가 이겼는지 판단.
{
    loadpanuser();
    loadpancom();

    ubingo = checkBingo(Userpan);//유저의 모든 빙고 수
    cbingo = checkBingo(Compan);//컴퓨터의 모든 빙고 수

    if (ubingo > cbingo) {//유저 승리
        system("cls");
        showpan();

        cout << "유저 빙고!" << endl;
        cout << "유저 빙고 : " << ubingo << ", 컴퓨터 빙고 : " << cbingo << endl;
        rungame++;
        system("pause");

    }
    else if (cbingo > ubingo) {//컴퓨터 승리
        system("cls");
        showpan();

        cout << "컴퓨터 빙고!" << endl;
        cout << "유저 빙고 : " << ubingo << ", 컴퓨터 빙고 : " << cbingo << endl;
        rungame++;
        system("pause");

    }
    else if (cbingo == pansize * 2 + 2 && ubingo == pansize * 2 + 2) {//무승부
        system("cls");
        showpan();

        cout << "무승부!!!" << endl;
        cout << "유저 빙고 : " << ubingo << ", 컴퓨터 빙고 : " << cbingo << endl;
        rungame++;
        system("pause");

    }

}

void JMGame::usertime()//유저가 선택
{
    int keycode = 0;

    while (true) {
        gotoxy(8 * (pansize), y);

        cout << "숫자를 입력하세요" << endl;
        gotoxy(8 * (pansize)+3, y + 1);
        cout << "메뉴로 복귀(선택) > ESC키 입력";
        gotoxy(8 * (pansize)+3, y + 2);
        cout<<"메뉴 복귀 후 종료 선택시 저장" << endl;


        gotoxy(x1, y2);

        setCursorView(false);
        if (_kbhit()) {

            int keycode = _getch();
            if (keycode == 224) {
                system("cls");
                showpan();
                keycode = _getch();

                switch (keycode) {
                case 75: x1 -= 8; break;
                case 77: x1 += 8; break;
                case 72: y2 -= 2; break;
                case 80: y2 += 2; break;


                }
                //판사이즈, 커서의 범위를 제한 하기 위한 제어문
                if (x1 < 0 || 8 * (pansize - 1) + 1 < x1) {//2 * (4 * pansize - 3) < x1
                    x1 = 0;
                    y2 = 3;
                }
                if (y2 < 3 || 2 * (pansize + 1) < y2) {//(pansize * 2) < y1
                    x1 = 0;
                    y2 = 3;
                }
            }
            else if (keycode == 27) {
                //ESC입력 시 메뉴로 돌아가도록
                gomenu = true;
                savenlist();
                break;
            }
            else if (keycode == 13) {
                num = 999;
                num = Userpan[(y2 / 2) - 1][x1 / 8];
                if (num == 0 || num == 1999) {
                    //유저의 입력 값이 이미 선택한 값일 경우를 제어
                    cout << " 다시 입력하세요." << endl;
                }
                else if (0 < num && num <= pansize * pansize) {
                    //유저의 값을 저장, 유저의 저장 값은 0, 컴퓨터의 저장값은 1999(장민의 출생년도)
                    loadpanuser();
                    for (int i = 0; i < pansize; i++) {
                        for (int j = 0; j < pansize; j++) {
                            if (Userpan[i][j] == num) {
                                Userpan[i][j] = 0;
                            }
                        }
                    }
                    nlist.push_back(num);
                    savenlist();
                    //유저 입력 값을 nlist에 저장

                    loadpancom();
                    for (int i = 0; i < pansize; i++) {
                        for (int j = 0; j < pansize; j++) {
                            if (Compan[i][j] == num) {
                                Compan[i][j] = 0;
                            }
                        }
                    }



                    savepanuser();
                    savepancom();
                    controlbingo();
                    break;
                }
                else {

                    cout << " 다시 입력하세요." << endl;
                }
            }
            gotoxy(x1, y2);
            cout << "^" << endl;



        }

    }


}

void JMGame::comtime()//컴퓨터의 선택시 수행하는 함수
{
    //컴퓨터의 승리를 위한 알고리즘을 구현하기 위한 코드.
    //동그라미(빈, 꽉찬 전부)가 가장 많은 줄을 찾아서 
    //빙고를 만들 확률이 가장 높은 줄의 값을 선택하도록 함.
    loadpancom();
    int iline = 0;//가장 확률이 높은 줄을 선택하기 위한 변수

    int izerocount = 0;//지금 확인 중인 동그라미 개수를 저장
    int isavecount = 0;//지금까지 확인 한 동그라미 개수 중 가장 큰 값 저장
    int n = 0;
    //가로
    for (int i = 0; i < pansize; i++) {
        izerocount = 0;
        for (int j = 0; j < pansize; j++) {
            if (Compan[i][j] == 0 || Compan[i][j] == 1999) {
                //저장 값이 0이나 1999면 동그라미 이므로 zerocount를 ++한다.
                ++izerocount;
            }

        }


        //밑의 조건을 만족하면 라인 교체
        if (izerocount < pansize && isavecount < izerocount) {
            //izerocount==pansize면 이미 빙고 만들어진 것이니 제외
            //zero카운트가 가장 크면 라인, save카운트교체.
            iline = i;//i번째 가로가 확률이 높다.
            isavecount = izerocount;

            //n=0이 가로 줄
        }
    }

    //위의 값과 비교해서 가장 많은 라인을 구한다.
    for (int i = 0; i < pansize; i++) {
        izerocount = 0;
        for (int j = 0; j < pansize; j++) {
            if (Compan[j][i] == 0 || Compan[j][i] == 1999) {
                ++izerocount;
            }

            if (izerocount < pansize && isavecount < izerocount) {
                //세로라인은 pansize~pansize
                iline = j + pansize;//라인을 찾고 구별하기 위해서 가로와는 다른 값을 저장
                isavecount = izerocount;
            }
        }
    }
    //대각선 1
    izerocount = 0;
    for (int i = 0; i < pansize; i++) {
        if (Compan[i][i] == 0 || Compan[i][i] == 1999) {
            ++izerocount;
        }

        if (izerocount < pansize && isavecount < izerocount) {
            //세로라인은 pansize~pansize
            iline = pansize * pansize + 1;//라인을 찾고 구별하기 위해서 가로와는 다른 값을 저장
            isavecount = izerocount;
        }

    }
    //대각선2
    izerocount = 0;
    for (int i = 0; i < pansize; i++) {
        if (Compan[i][pansize - i - 1] == 0 || Compan[i][pansize - i - 1] == 1999) {
            ++izerocount;
        }
        if (izerocount < pansize && isavecount < izerocount) {
            //세로라인은 pansize~pansize
            iline = pansize * pansize + 2;//라인을 찾고 구별하기 위해서 가로와는 다른 값을 저장
            isavecount = izerocount;
        }
    }
    //모든 라인 조사후 iline에 가능성이 가장 높은 줄 번호가 저장.
    //줄의 0이 아닌 수 중 하나를 선택하도록 한다.
    //가로
    if (iline < pansize) {
        //가로줄 iline 0~pansize-1
        for (int i = 0; i < pansize; i++) {
            if (Compan[iline][i] != 0 && Compan[iline][i] != 1999) {
                num2 = Compan[iline][i];
                break;
            }

        }
    }
    else if (iline < pansize * 2) {
        //가로줄 제외하고 세로줄 선택
        for (int i = 0; i < pansize; i++) {
            if (Compan[i][iline - pansize] != 0 && Compan[i][iline - pansize] != 1999) {
                num2 = Compan[i][iline - pansize];
                break;
            }

        }
    }
    else if (iline == pansize * pansize + 1) {
        //대각선1의 경우
        for (int i = 0; i < pansize; i++) {
            if (Compan[i][i] != 0 && Compan[i][i] != 1999) {
                num2 = Compan[i][i];
                break;
            }

        }
    }
    else if (iline == pansize * pansize + 2) {
        //대각선2의 경우
        for (int i = 0; i < pansize; i++) {
            if (Compan[i][pansize - i - 1] != 0 && Compan[i][pansize - i - 1] != 1999) {
                num2 = Compan[i][pansize - i - 1];
                break;
            }

        }
    }
    if (num2 == beforenum) {
        for (int i = 0; i < pansize; i++) {
            for (int j = 0; j < pansize; j++) {
                if (Compan[i][j] != 0 && Compan[i][j] != 1999) {
                    num2 = Compan[i][j];

                }
            }
        }
    }


    //위의 알고리즘을 통해 정해진 num2와 같은 값을 찾아 1999저장
    loadpanuser();
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            if (Userpan[i][j] == num2) {
                Userpan[i][j] = 1999;
            }
        }
    }

    loadpancom();
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            if (Compan[i][j] == num2) {
                Compan[i][j] = 1999;
            }
        }
    }
    //nlist에도 저장
    beforenum = num2;
    nlist.push_back(num2);
    savenlist();
    savepanuser();
    savepancom();
    controlbingo();

}

int JMGame::checkBingo(vector<vector<int>>(&arr))
{
    loadpanuser();
    loadpancom();
    int check = 0;// 빙고 체크!
    int x, y;
    int sum = 0;//빙고의 개수를 다 더해서 저장하는 변수

    //x축 확인
    for (y = 0; y < pansize; y++) {
        check = 0;
        for (x = 0; x < pansize; x++) {
            check += arr[y][x];
        }
        for (int i = 0; i <= pansize; i++) {
            if (check == 1999 * i) {
                sum++;

            }
        }
    }
    //y축 확인
    for (x = 0; x < pansize; x++) {
        check = 0;
        for (y = 0; y < pansize; y++) {
            check += arr[y][x];
        }
        for (int i = 0; i <= pansize; i++) {
            if (check == 1999 * i) {
                sum++;

            }
        }

    }

    //대각선1
    check = 0;
    //대각선은 한 줄이니까 for문밖에서 초기화
    for (x = 0; x < pansize; x++) {
        check += arr[x][x];
    }
    for (int i = 0; i <= pansize; i++) {
        if (check == 1999 * i) {
            sum++;

        }
    }


    //대각선2
    check = 0;
    for (x = 0; x < pansize; x++) {
        check += arr[x][pansize - x - 1];
    }
    for (int i = 0; i <= pansize; i++) {
        if (check == 1999 * i) {
            sum++;

        }
    }

    return sum;


}




//게임 시작하면 빙고판 만들어줌.
void JMGame::Bingopan()
{
    ubingo = 0;
    cbingo = 0;
    int pannum;
    system("cls");
    while (true) {
        system("cls");
        cout << "학번 : 201911209 이름 : 장 민" << endl;
        cout << "빙고판의 크기를 입력하세요(3에서 9사이의 수를 입력) : " << endl;
        cin >> pannum;
        pansize = pannum;
        if (pansize < 3 || pansize>9) {


            cout << "크기를 다시 입력하세요" << endl;
            system("pause");
            cin.clear();
            cin.ignore(INT_MAX, '\n');

        }
        else if (3 <= pansize && pansize <= 9) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            break;
        }
        else {

            cout << "크기를 다시 입력하세요" << endl;
            system("pause");
            cin.clear();
            cin.ignore(INT_MAX, '\n');

        }

    }
    //크기 입력 받고서 빙고판 동적할당.

    Userpan.assign(pansize, vector<int>(pansize, 0));
    Compan.assign(pansize, vector<int>(pansize, 0));
    //일단 숫자 순서대로 넣기
    int insertnum = 1;
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            Userpan[i][j] = insertnum++;

        }

    }
    int insertnum2 = 1;
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            Compan[i][j] = insertnum2++;
        }
    }
    //빙고안의 숫자 섞기.
    srand((unsigned int)time(0));
    for (int i = 0; i < 100; i++) {
        int rnum1 = rand() % pansize;//User 층 섞기
        int rnum2 = rand() % pansize;//User 방 섞기
        int rnum3 = rand() % pansize;//com 층 섞기
        int rnum4 = rand() % pansize;//com 방 섞기
        int temp;

        temp = Userpan[rnum1][rnum2];
        Userpan[rnum1][rnum2] = Userpan[rnum2][rnum1];
        Userpan[rnum2][rnum1] = temp;

        temp = Compan[rnum3][rnum4];
        Compan[rnum3][rnum4] = Compan[rnum4][rnum3];
        Compan[rnum4][rnum3] = temp;



    }
    //다 섞었는데 비교했더니 똑같은 게 있으면 그것도 처리
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            if (Userpan[i][j] == Compan[i][j]) {
                if (0 == j) {
                    int temp;
                    temp = Compan[i][j];
                    Compan[i][j] = Compan[i][j + 1];
                    Compan[i][j + 1] = temp;
                }
                if (0 < j) {
                    if (j <= pansize - 2) {
                        int temp;
                        temp = Compan[i][j];
                        Compan[i][j] = Compan[i][j - 1];
                        Compan[i][j - 1] = temp;
                    }
                    else if (j == pansize - 1) {
                        int temp;
                        temp = Compan[i][j];
                        Compan[i][j] = Compan[i][j - 1];
                        Compan[i][j - 1] = temp;
                    }
                }


            }

        }
    }
    //초기값의 빙고판들을 레플리카들에도 저장하기
    //리플레이를 위해서 필요함.
    replicauser.assign(pansize, vector<int>(pansize, 0));
    replicacom.assign(pansize, vector<int>(pansize, 0));
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            replicauser[i][j] = Userpan[i][j];
        }
    }
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            replicacom[i][j] = Compan[i][j];
        }
    }
    savepanuser();
    savepancom();
    savereplica();

}

void JMGame::savepanuser()//Userpan 저장
{



    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "파일 저장 실패";
        return;
    }
    fout << pansize << endl;
    for (int k = 0; k < pansize; k++) {
        for (int p = 0; p < pansize; p++) {
            fout << Userpan[k][p] << " ";
        }
        fout << endl;
    }



}

void JMGame::savepancom()//Compan 저장
{



    ofstream fout(filename2);
    if (!fout.is_open()) {
        cerr << "파일 저장 실패";
        return;
    }
    fout << pansize << endl;

    for (int k = 0; k < pansize; k++) {
        for (int p = 0; p < pansize; p++) {
            fout << Compan[k][p] << " ";
        }
        fout << endl;
    }



}

void JMGame::savereplica()//replicauser,replicacom 저장
{
    ofstream fout(filename3);
    if (!fout.is_open()) {
        cerr << "파일 저장 실패";
        return;
    }
    fout << pansize << endl;

    for (int k = 0; k < pansize; k++) {
        for (int p = 0; p < pansize; p++) {
            fout << replicauser[k][p] << " ";
        }
        fout << endl;
    }
    for (int k = 0; k < pansize; k++) {
        for (int p = 0; p < pansize; p++) {
            fout << replicacom[k][p] << " ";
        }
        fout << endl;
    }
}

void JMGame::savenlist()//nlist 저장
{
    ofstream fout(filename4);
    if (!fout.is_open()) {
        cerr << "파일 저장 실패";
        return;
    }
    fout << nlist.size() << endl;

    for (auto it = nlist.begin(); it != nlist.end(); it++) {
        fout << *it << "\t";
    }
}

//빙고판 화면에 띄워주기.
void JMGame::showpan()
{

    gotoxy(x, y);
    cout << "201911209 장 민" << endl;

    loadpanuser();
    loadpancom();


    cout << "<    플레이어 빙고판    >" << endl;
    int k = 0;
    for (int i = 0; i < pansize; i++) {
        gotoxy(x, y + 2 + k);

        for (int j = 0; j < pansize; j++) {
            if (Userpan[i][j] == 0) {//0이면 유저 입력
                cout << "○" << "\t";
            }
            else if (Userpan[i][j] == 1999) {//1999면 컴퓨터 입력
                cout << "●" << "\t";
            }

            else {
                cout << Userpan[i][j] << "\t";
            }
        }
        k += 2;
        cout << endl;
    }
    k += 2;
    gotoxy(x, y + 1 + k);
    cout << "<    컴퓨터 빙고판    >" << endl;

    for (int i = 0; i < pansize; i++) {

        gotoxy(x, y + 2 + k);
        for (int j = 0; j < pansize; j++) {
            if (Compan[i][j] == 1999) {
                cout << "●" << "\t";
            }
            else if (Compan[i][j] == 0) {
                cout << "○" << "\t";
            }
            else {
                cout << Compan[i][j] << "\t";
            }
        }
        k += 2;
        cout << endl;
    }






}
//빙고판을 화면에 띄울 때나 이어하기를 위한 파일읽어 오는 함수
void JMGame::loadpanuser()//Userpan 읽어오기
{
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "파일 읽기 실패" << endl;

        return;
    }
    fin >> pansize;

    //잠깐 동안 저장했다가 넘겨주고 사라지는 temps1벡터
    vector<vector<int>> temps1;
    temps1.assign(pansize, vector<int>(pansize, 0));
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            fin >> temps1[i][j];
        }
    }
    temps1.swap(Userpan);
    temps1.clear();
    temps1.shrink_to_fit();


}

void JMGame::loadpancom()//Compan 읽어오기
{
    ifstream fin(filename2);
    if (!fin.is_open()) {
        cerr << "파일 읽기 실패" << endl;

        return;
    }
    fin >> pansize;
    //잠깐 동안 저장했다가 넘겨주고 사라지는 temps2벡터
    vector<vector<int>> temps2;
    temps2.assign(pansize, vector<int>(pansize, 0));
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            fin >> temps2[i][j];
        }
    }
    temps2.swap(Compan);
    temps2.clear();
    temps2.shrink_to_fit();


}

void JMGame::loadreplica()//레플리카 파일 읽어서 Userpan, Compan에 저장
{
    ifstream fin(filename3);
    if (!fin.is_open()) {
        cerr << "파일 읽기 실패" << endl;
        //return nullptr;
        return;
    }
    fin >> pansize;
    //잠깐 동안 저장했다가 넘겨주고 사라지는 temps벡터들
    vector<vector<int>> temps1;
    temps1.assign(pansize, vector<int>(pansize, 0));
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            fin >> temps1[i][j];
        }
    }
    temps1.swap(Userpan);
    temps1.clear();
    temps1.shrink_to_fit();
    savepanuser();

    vector<vector<int>> temps2;
    temps2.assign(pansize, vector<int>(pansize, 0));
    for (int i = 0; i < pansize; i++) {
        for (int j = 0; j < pansize; j++) {
            fin >> temps2[i][j];
        }
    }
    temps2.swap(Compan);
    temps2.clear();
    temps2.shrink_to_fit();
    savepancom();
}

void JMGame::loadnlist()//save.txt읽어서 nlist에 저장
{
    ifstream fin(filename4);
    if (!fin.is_open()) {
        cerr << "파일 읽기 실패" << endl;

        return;
    }
    int size;
    fin >> size;
    vector<int> temps3;
    temps3.assign(size, 0);
    for (int j = 0; j < size; j++) {
        fin >> temps3[j];
    }

    temps3.swap(nlist);
    temps3.clear();
    temps3.shrink_to_fit();
}

void JMGame::replay()//리플레이
{
    rungame = 0;
    int ncount = -1;
    loadreplica();
    //replica.txt에는 초기의 빙고판이 저장되어 있음
    loadnlist();
    int num3 = 9999;
    do {

        system("cls");




        showpan();
        //사용자 입력 대신에 nlist의 값을 얻어와서 그 값을 사용자 입력 값처럼 저장
        num3 = nlist[++ncount];
        loadpanuser();
        for (int i = 0; i < pansize; i++) {
            for (int j = 0; j < pansize; j++) {
                if (Userpan[i][j] == num3) {
                    Userpan[i][j] = 0;
                }
            }
        }


        loadpancom();
        for (int i = 0; i < pansize; i++) {
            for (int j = 0; j < pansize; j++) {
                if (Compan[i][j] == num3) {
                    Compan[i][j] = 0;
                }
            }
        }

        savepanuser();
        savepancom();
        system("cls");
        showpan();
        Sleep(500);// 시간 지연
        //system("pause");
        controlbingo();
        //이겼으면 rungame++
        if (rungame != 0) {
            ubingo = 0;
            cbingo = 0;
            break;
        }
        //컴퓨터 입력 대신에 nlist의 값을 얻어와서 그 값을 사용자 입력 값처럼 저장
        //괜히 알고리즘 돌리지 않고 바로 입력하도록 함. 시간이 아주 조금이라도 빨라짐
        num3 = nlist[++ncount];
        loadpanuser();
        for (int i = 0; i < pansize; i++) {
            for (int j = 0; j < pansize; j++) {
                if (Userpan[i][j] == num3) {
                    Userpan[i][j] = 1999;
                }
            }
        }

        loadpancom();
        for (int i = 0; i < pansize; i++) {
            for (int j = 0; j < pansize; j++) {
                if (Compan[i][j] == num3) {
                    Compan[i][j] = 1999;
                }
            }
        }


        savepanuser();
        savepancom();

        system("cls");
        showpan();
        Sleep(500);//시간 지연
        //system("pause");
        controlbingo();
        //이겼으면 rungame++
        if (rungame != 0) {
            ubingo = 0;
            cbingo = 0;
            break;
        }



    } while (true);

    nlist.clear();
    savenlist();
}


