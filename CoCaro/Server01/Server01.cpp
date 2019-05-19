// Server01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "afxsock.h"
#include "Server01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

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
			//char msgclient[2][10];
			for (int i = 0; i < 2; i++) {
				// Khoi tao mot socket de duy tri viec ket noi va trao doi du lieu
				cout << "Dang cho Client thu " << i + 1 << " ket noi" << endl;
				if (ServerSocket.Accept(Connector[i]))
				{
					cout << "Client thu" << i + 1 << " da ket noi !!!" << endl << endl;
				}
			}
			for (int i = 0; i < 2; i++) {

				do
				{


				} while (true);
			}
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
