// Client01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Client01.h"
#include <afxsock.h>
#include <conio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

#define UP   119  // w
#define DOW  115  // s
#define LE   97   // a
#define RI   100  // d
#define OK   13   // enter
#define CH   104 // cầu hòa
#define XT   116 // xin thua
#define ESC  27

int x = 12;
int y = 6;
//int thaydoiluot = 0;
char mynum;
char signal[2] = {'x','o'};
char color[2] {10,12};

void txtColor(int color);
void khung();
void gotoxy(int x, int y);
void huongdan();


int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
			// Khoi tao Thu vien
			if (AfxSocketInit() == FALSE)
			{
				cout << "Khong the khoi tao Socket Libraray";
				return FALSE;
			}

			// Tao socket dau tien
			CSocket ClientSocket;
			ClientSocket.Create();

			if (ClientSocket.Connect(_T("localhost"),8888) != 0)
			{
				//cout << "Ket noi toi Server thanh cong !!!" << endl << endl;
				ClientSocket.Receive(&mynum, sizeof(char), 0);

				khung();
				gotoxy(x, y);
				
				// Playing

				while (true)
				{
					bool tieptuc;
					ClientSocket.Receive(&tieptuc, sizeof(bool), 0);
					if (!tieptuc)  // kết thúc
					{
						char sKetQua[10];
						ClientSocket.Receive(&sKetQua,10, 0);
						gotoxy(10, 1);
						cout << "                                                              ";
						gotoxy(10, 1);
						cout << sKetQua;
						break;
					}
					// tiếp tục
					// chờ lượt chơi
					bool luotcuatoi;
					ClientSocket.Receive(&luotcuatoi, sizeof(bool), 0);
					if (luotcuatoi)
					{
						gotoxy(15, 1);
						txtColor(11);
						cout << "Den luot nguoi ban -";
						txtColor(color[mynum]);
						cout << signal[mynum] << "                       ";
						gotoxy(x, y);
						dichuyen:
						char ch = _getch();						
						if (ch == CH) { 
							gotoxy(15, 1);
							cout << "Doi doi thu tra loi                        ";
							ClientSocket.Send(&ch, sizeof(char), 0);
							bool ok;
							ClientSocket.Receive(&ok, sizeof(bool), 0);
							if (ok)
							{
								gotoxy(15, 1);
								cout << "Doi thu dong y - HOA                   ";
								break;
							}
							else {
								gotoxy(15, 1);
								cout << "Doi thu khong dong y!                   ";
								gotoxy(15, 1);
								_getch();
								cout << "                                        ";
							}
						}
						else if (ch == XT) { 
							gotoxy(15, 0);
							cout << "Ban co muon xin thua c/k";
							char C = _getch();
							if ((C == 'c') || (C == 'C')) {
								ClientSocket.Send(&ch, sizeof(char), 0);
								exit(0);
							}
							else {
								gotoxy(15, 0);
								cout << "                                ";
							}
						}
						else if (ch == ESC) {
							gotoxy(15, 0);
							cout << "Ban co muon thoat c/k";
							char C = _getch();
							if ((C == 'c') || (C == 'C')) {
								ClientSocket.Send(&ch, sizeof(char), 0);
								exit(0);
							}
							else {
								gotoxy(15, 0);
								cout << "                                ";
							}
						}
						else if (ch == UP) {
							if (y <= 4) {
								y = 19;
								gotoxy(x, y);
								goto dichuyen;
							}
							else {
								y--;
								gotoxy(x, y);
								goto dichuyen;
							}
						}						
						else if (ch == DOW) {
							if (y >= 19) {
								y = 4;
								gotoxy(x, y);
								goto dichuyen;
							}
							else {
								y++;
								gotoxy(x, y);
								goto dichuyen;
							}
						}
						else if (ch == RI) {
							if (x >= 48) {
								x = 11;
								gotoxy(x, y);
								goto dichuyen;
							}
							else {
								x++;
								gotoxy(x, y);
								goto dichuyen;
							}
						}
						else if (ch == LE) {
							if (x <= 11) {
								x = 48;
								gotoxy(x, y);
								goto dichuyen;
							}
							else {
								x--;
								gotoxy(x, y);
								goto dichuyen;
							}
						}
						else if (ch == OK) {
							ClientSocket.Send(&ch, sizeof(char), 0);
							ClientSocket.Send(&x, sizeof(int), 0);
							ClientSocket.Send(&y, sizeof(int), 0);
							bool bErr;
							ClientSocket.Receive(&bErr, sizeof(bool), 0);
							if (bErr) // ô này đã được chọn
							{
								gotoxy(15, 1);
								txtColor(10);
								cout << "O nay khong trong. Ban hay chon o khac ...";
								_getch();
								gotoxy(15, 1);
								cout << "                                            ";
								gotoxy(x, y);
								goto dichuyen;
							}
							else {
								gotoxy(x, y);
								txtColor(color[mynum]);
								cout << signal[mynum];								
							}

						}
					}
					else {
						gotoxy(15, 1);
						txtColor(11);
						cout << "Den luot nguoi doi thu -";
						txtColor(color[1 - mynum]);
						cout << signal[1 - mynum] << "                   ";

						char ch;
						ClientSocket.Receive(&ch, sizeof(char), 0);

						if (ch == CH) {
							txtColor(10);
							gotoxy(15, 1);
							cout << "Doi thu cau hoa. Ban co dong y khong? c/k";
							bool ok = false;
							char C = _getch();
							if ((C == 'c') || (C == 'C')) {
								ok = true;
								gotoxy(15, 1);
								cout << "HOA                                      ";
								break;
							}							
							ClientSocket.Send(&ok, sizeof(bool), 0);


						}
						else if (ch == XT) {
							gotoxy(15, 1);
							txtColor(11);
							cout << "Doi thu bo cuoc - YOU WIN!           ";
							break;
						}
						else if (ch == ESC) {
							gotoxy(15, 1);
							txtColor(11);
							cout << "Doi thu bi mat ket noi - YOU WIN!  ";
							break;
						}
						else if (ch == OK) {							
							ClientSocket.Receive(&x, sizeof(int), 0);
							ClientSocket.Receive(&y, sizeof(int), 0);
							gotoxy(x, y);
							txtColor(color[1 - mynum]);
							cout << signal[1 - mynum];
						}
					}
				}

				_getch();
			}
			else
			{
				cout << "Khong the ket noi den Server !!!" << endl;
			}
			// Dong ket noi
			ClientSocket.Close();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}

void txtColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void khung() {
	int d = 1;
	txtColor(6);
	for (int i = 0; i < 40; i++) {
		gotoxy(10 + i, 3);
		cout << char(220);
		gotoxy(10 + i, 20);
		cout << char(223);
	}
	for (int i = 4; i <= 19; i++) {
		gotoxy(10, i);
		cout << char(219);
		gotoxy(49, i);
		cout << char(219);
	}
	int k = 1;
	for (int i = 11; i <= 48; i++) {
		for (int j = 4; j <= 19; j++) {
			if (i % 2 == 0) {
				if (j % 2 == 0) {
					gotoxy(i, j);
					txtColor(0);
					cout << char(219);
				}
				else {
					gotoxy(i, j);
					txtColor(7);
					cout << char(219);
				}
			}
			else {
				if (j % 2 == 0) {
					gotoxy(i, j);
					txtColor(7);
					cout << char(219);
				}
				else {
					gotoxy(i, j);
					txtColor(0);
					cout << char(219);
				}
			}
		}
	}
	txtColor(14);
	huongdan();
}

void huongdan() {
	txtColor(10);
	gotoxy(1, 0);
	txtColor(9);
	cout << "Esc - Thoat";
	txtColor(10);
	gotoxy(1, 3);
	cout << "a : " << "trai";
	gotoxy(1, 4);
	cout << "d : " << "phai";
	gotoxy(1, 5);
	cout << "w : " << "len";
	gotoxy(1, 6);
	cout << "s : " << "xuong";
	gotoxy(1, 8);
	txtColor(13);
	cout << "t : xin ";
	gotoxy(1, 9);
	txtColor(13);
	cout << "    thua";
	gotoxy(1, 10);
	txtColor(10);
	txtColor(13);
	cout << "h : cau ";
	gotoxy(1, 11);

	cout << "    hoa";
}

