#pragma once
#include "stdafx.h"

#define xdelete(x) if(x){delete(x);x=NULL;}
#define xdeletes(x) if(x){delete[](x);x=NULL;}

#define _CComboGetCurSel(x)		((CComboBox*)GetDlgItem(x))->GetCurSel()
#define _CComboGetText(x,str)	((CComboBox*)GetDlgItem(x))->GetWindowText()
#define _CEditGetText(x,str)	((CEdit*)GetDlgItem(x))->GetWindowText(str)
#define _CComboSetCurSel(x,i)	((CComboBox*)GetDlgItem(x))->SetCurSel(i)
#define _CEditSetText(x,str)	((CEdit*)GetDlgItem(x))->SetWindowText(str)

enum//窗口句柄枚举
{
	HWND_Main,//性能分析软件
	HWND_Master,//地面窄带主控
	HWND_Controlled,//飞机窄带主控
	HWND_BroadGrd,//地面宽带主控
	HWND_BroadPlane,//飞机宽带主控
	HWND_Statistics,//显示窗口

	//新添加的窗口请在此之前添加
	HWND_Number,//窗口数量
};