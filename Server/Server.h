
// Server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CServerApp:
// �йش����ʵ�֣������ Server.cpp

//#include "ServerDlg.h"

class CServerApp : public CWinApp
{
public:
	CServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()


//public:
//	CServerDlg* m_pServerDlg;

//����취һ
public:
	HWND m_hwndPic;
};

extern CServerApp theApp;