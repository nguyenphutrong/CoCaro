// Client01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "afxsock.h"
#include "Client01.h"

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
			CSocket ClientSocket;
			ClientSocket.Create();

			// 192.168.42.37

			if (ClientSocket.Connect(_T("localhost"),8888) != 0)
			{
				cout << "Ket noi toi Server thanh cong !!!" << endl << endl;


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
