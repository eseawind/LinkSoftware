
// ControllerOnServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
// CControllerOnServerApp:
// �йش����ʵ�֣������ ControllerOnServer.cpp
//
class WinpCapHelper;
class CControllerOnServerApp : public CWinApp
{
public:
	CControllerOnServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CControllerOnServerApp theApp;
extern WinpCapHelper* caper;