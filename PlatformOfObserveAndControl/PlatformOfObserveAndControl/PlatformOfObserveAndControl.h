
// PlatformOfObserveAndControl.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

class Node;
class WinpCapHelper;
class PhyHelper;
// CPlatformOfObserveAndControlApp:
// �йش����ʵ�֣������ PlatformOfObserveAndControl.cpp
//

class CPlatformOfObserveAndControlApp : public CWinApp
{
public:
	CPlatformOfObserveAndControlApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPlatformOfObserveAndControlApp theApp;
extern WinpCapHelper* serverCaper;
extern WinpCapHelper* protocolCaper;
extern PhyHelper* pcieSender;
extern PhyHelper* pcieReceiver;