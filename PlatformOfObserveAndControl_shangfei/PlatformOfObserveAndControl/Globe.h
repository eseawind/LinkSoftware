#pragma once
#include "stdafx.h"

#define xdelete(x) if(x){delete(x);x=NULL;}
#define xdeletes(x) if(x){delete[](x);x=NULL;}

#define _CComboGetCurSel(x)		((CComboBox*)GetDlgItem(x))->GetCurSel()
#define _CComboGetText(x,str)	((CComboBox*)GetDlgItem(x))->GetWindowText()
#define _CEditGetText(x,str)	((CEdit*)GetDlgItem(x))->GetWindowText(str)
#define _CComboSetCurSel(x,i)	((CComboBox*)GetDlgItem(x))->SetCurSel(i)
#define _CEditSetText(x,str)	((CEdit*)GetDlgItem(x))->SetWindowText(str)

enum//���ھ��ö��
{
	HWND_Main,//���ܷ������
	HWND_Master,//����խ������
	HWND_Controlled,//�ɻ�խ������
	HWND_BroadGrd,//����������
	HWND_BroadPlane,//�ɻ��������
	HWND_Statistics,//��ʾ����

	//����ӵĴ������ڴ�֮ǰ���
	HWND_Number,//��������
};