// Server01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Server01.h"


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


int a[100][100];
int luot = 0; // 0: player 1, 1: player 2
struct ToaDo {
	int x;
	int y;
};
ToaDo mang[4];

int thang();

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
			CSocket ServerSocket;
			// Tao socket cho server, dang ky port la 8888, giao thuc TCP
			if (ServerSocket.Create(8888, SOCK_STREAM, NULL) == 0) //SOCK_STREAM or SOCK_DGRAM.
			{
				cout << "Khoi tao that bai !!!" << endl;
				cout << ServerSocket.GetLastError();
				return FALSE;
			}
			else
			{
				cout << "Server khoi tao thanh cong !!!" << endl;
				if (ServerSocket.Listen(1) == FALSE)
				{
					cout << "Khong the lang nghe tren port nay !!!" << endl;
					ServerSocket.Close();
					return FALSE;
				}
			}

			CSocket Connector[2];
		
			for (int i = 0; i < 2; i++) {
				// Khoi tao mot socket de duy tri viec ket noi va trao doi du lieu
				cout << "Dang cho Client thu " << i + 1 << " ket noi" << endl;
				if (ServerSocket.Accept(Connector[i]))
				{
					cout << "Client thu" << i + 1 << " da ket noi !!!" << endl << endl;
				}
			}

			char num = 0;
			Connector[0].Send(&num, sizeof(char), 0);
			num = 1;
			Connector[1].Send(&num, sizeof(char), 0);

			// Reset array
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < 100; j++) {
					a[i][j] = 0;
				}
			}

			do
			{
				bool tieptuc = false;
				
				char sWin[10];
				strcpy_s(sWin, "YOU WIN!!");
				char sLose[10];
				strcpy_s(sLose, "YOU LOSE!");

				int winner = thang();
				if (winner == 0) {
					tieptuc = true;
				}
				Connector[0].Send(&tieptuc, sizeof(bool), 0);
				Connector[1].Send(&tieptuc, sizeof(bool), 0);
				if (winner) {
					Connector[winner-1].Send(&sWin, 10, 0);
					Connector[1-(winner-1)].Send(&sLose, 10, 0);
					break;
				}
				// Tiếp tục chơi
				bool luotcuaban = true;
				Connector[luot].Send(&luotcuaban, sizeof(bool), 0);
				luotcuaban = false;
				Connector[1-luot].Send(&luotcuaban, sizeof(bool), 0);

				char ch;
				Connector[luot].Receive(&ch, sizeof(char), 0);
				Connector[1-luot].Send(&ch, sizeof(char), 0);

				if (ch == CH) {
					bool ok;
					Connector[1 - luot].Receive(&ok, sizeof(bool), 0);
					Connector[luot].Send(&ok, sizeof(bool), 0);
					if (ok)
					{
						_getch();
						break;
					}
				}
				else if (ch == XT) {
					break;
				}
				else if (ch == ESC) {
					break;
				}
				else if (ch == OK) {
					int x; int y;
					Connector[luot].Receive(&x, sizeof(int), 0);
					Connector[luot].Receive(&y, sizeof(int), 0);

					bool bErr;
					// Neu ma da co gia tri	
					if (a[x][y] == 0) {
						bErr = false;
					}
					else {
						bErr = true;
					}
					
					Connector[luot].Send(&bErr, sizeof(bool), 0);
					// luc nay da chon o khong co gia tri

					Connector[1 - luot].Send(&x, sizeof(int), 0);
					Connector[1 - luot].Send(&y, sizeof(int), 0);

					a[x][y] = luot+1;
					luot = 1 - luot;

					
				}


			} while (true);
		
			Connector[0].Close();
			Connector[1].Close();
			ServerSocket.Close();
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

int thang() {
	int Thang = 1; // thang . 0 la chua thang
	for (int i = 1; i <= 50; i++) {
		for (int j = 1; j <= 50; j++) {

			if ((a[i][j] == a[i + 1][j]) && (a[i][j] == a[i + 2][j]) && (a[i][j] == a[i + 3][j])/*&&(a[i+4][j]==a[i][j])*/ && (a[i][j] != 0)) {
				Thang = 1;
				for (int h = 0; h <= 50; h++) {
					if (i - h <= 1) {
						break;
					}
					else {
						if ((a[i - h][j] != a[i][j]) && (a[i - h][j] != 0)) {
							Thang = 0;
							break;
						}
					}
				}
				for (int h = 0; h <= 50; h++) {
					if (i + h >= 50) {
						break;
					}
					else {
						if ((a[i + h + 3][j] != a[i][j]) && (a[i + h + 3][j] != 0)) {
							Thang = 0;
							break;
						}
					}
				}
				if (Thang == 1) {
					for (int k = 0; k < 4; k++) {
						mang[k].y = j;
						mang[k].x = i + k;
					}
					return a[i][j];
				}
			}
			if ((a[i][j] == a[i][j + 1]) && (a[i][j] == a[i][j + 2]) && (a[i][j] == a[i][j + 3])/*&&(a[i][j]==a[i][j+4])*/ && (a[i][j] != 0)) {
				Thang = 1;
				for (int h = 0; h <= 50; h++) {
					if (j + h >= 50) {
						break;
					}
					else {
						if ((a[i][j + 3 + h] != 0) && (a[i][j + 3 + h] != a[i][j])) Thang = 0;
					}
				}
				for (int h = 0; h <= 50; h++) {
					if (j - h <= 1) {
						break;
					}
					else {
						if ((a[i][j - h] != 0) && (a[i][j - h] != a[i][j])) Thang = 0;
					}
				}
				if (Thang == 1) {
					for (int k = 0; k < 4; k++) {
						mang[k].x = i;
						mang[k].y = j + k;
					}
					return a[i][j];
				}
			}
			if ((a[i][j] == a[i + 1][j + 1]) && (a[i][j] == a[i + 2][j + 2]) && (a[i][j] == a[i + 3][j + 3])/*&&(a[i][j]==a[i+4][j+4])*/ && (a[i][j] != 0)) {
				Thang = 1;
				for (int h = 0; h <= 50; h++) {
					if ((i + h >= 50) || (j + h) >= 30) break;
					else {
						if ((a[i + h + 3][j + h + 3] != a[i][j]) && (a[i + h + 3][j + h + 3] != 0)) Thang = 0;
					}
				}
				for (int h = 0; h <= 50; h++) {
					if ((i - h <= 1) || (j - h) <= 1) break;
					else {
						if ((a[i - h][j - h] != a[i][j]) && (a[i - h][j - h] != 0)) Thang = 0;
					}
				}
				if (Thang == 1) {
					for (int k = 0; k < 4; k++) {
						mang[k].x = i + k;
						mang[k].y = j + k;
					}
					return a[i][j];
				}
			}
			if ((a[i][j] == a[i - 1][j + 1]) && (a[i][j] == a[i - 2][j + 2]) && (a[i][j] == a[i - 3][j + 3])/*&&(a[i][j]==a[i-4][j-4])*/ && (a[i][j] != 0)) {
				Thang = 1;
				for (int h = 1; h <= 50; h++) {
					if ((i + h >= 50) || (j - h) <= 1) break;
					else {
						if ((a[i + h][j - h] != a[i][j]) && (a[i + h][j - h] != 0)) Thang = 0;
					}
				}
				for (int h = 0; h <= 50; h++) {
					if ((i - h <= 1) || (j + h) >= 50) break;
					else {
						if ((a[i - 4 - h][j + h + 4] != a[i][j]) && (a[i - h - 4][j + h + 4] != 0)) Thang = 0;
					}
				}
				if (Thang == 1) {
					for (int k = 0; k < 4; k++) {
						mang[k].x = i - k;
						mang[k].y = j + k;
					}
					return a[i][j];
				}
			};
		}
	}

	return 0;
}
