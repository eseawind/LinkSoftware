// SelectNetDeviceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlatformOfObserveAndControl.h"
#include "SelectNetDeviceDlg.h"
#include "afxdialogex.h"

#include "Globe.h"
#include "WinpCapHelper.h"

// CSelectNetDeviceDlg �Ի���

IMPLEMENT_DYNAMIC(CSelectNetDeviceDlg, CDialogEx)

CSelectNetDeviceDlg::CSelectNetDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectNetDeviceDlg::IDD, pParent)
{

}

CSelectNetDeviceDlg::~CSelectNetDeviceDlg()
{
}

void CSelectNetDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NetDevs, m_ListCtrlNetDevs);
	DDX_Control(pDX, IDC_LIST_NetDevs2, m_ListNetDev2);
}


BEGIN_MESSAGE_MAP(CSelectNetDeviceDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CSelectNetDeviceDlg::OnBnClickedButtonOk)
END_MESSAGE_MAP()


// CSelectNetDeviceDlg ��Ϣ�������


BOOL CSelectNetDeviceDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ListCtrlNetDevs.SetExtendedStyle(m_ListCtrlNetDevs.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	m_ListNetDev2.SetExtendedStyle(m_ListCtrlNetDevs.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	CString tempStr;
	m_ListCtrlNetDevs.InsertColumn(0,_T("�������"),LVCFMT_LEFT,20);
	m_ListCtrlNetDevs.InsertColumn(1,_T("��������"),LVCFMT_LEFT,200);
	m_ListCtrlNetDevs.InsertColumn(2,_T("��������"),LVCFMT_LEFT,200);
	m_ListNetDev2.InsertColumn(0,_T("�������"),LVCFMT_LEFT,20);
	m_ListNetDev2.InsertColumn(1,_T("��������"),LVCFMT_LEFT,200);
	m_ListNetDev2.InsertColumn(2,_T("��������"),LVCFMT_LEFT,200);
	pcap_if_t* pAdapter=WinpCapHelper::getAllAdapters();
	for(int i=0;pAdapter!=NULL;pAdapter=pAdapter->next,i++)
	{
		tempStr.Format(_T("%d"),i);
		m_ListCtrlNetDevs.InsertItem(i,tempStr);
		m_ListNetDev2.InsertItem(i,tempStr);
		tempStr=pAdapter->name;
		m_ListCtrlNetDevs.SetItemText(i,1,tempStr);
		m_ListNetDev2.SetItemText(i,1,tempStr);
		tempStr=pAdapter->description;
		m_ListCtrlNetDevs.SetItemText(i,2,tempStr);
		m_ListNetDev2.SetItemText(i,2,tempStr);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSelectNetDeviceDlg::OnBnClickedButtonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int devID[2];
	devID[0]=m_ListCtrlNetDevs.GetSelectionMark();
	devID[1]=m_ListNetDev2.GetSelectionMark();
	
	if(devID[0]>=0&&devID[1]>=0)
	{
		::SendMessage(GetParent()->GetSafeHwnd(),WM_USER_HAVESELECTEDDEVICE,NULL,(LPARAM)devID);
		SendMessage(WM_CLOSE);
	}
	else
	{
		AfxMessageBox(_T("��ѡ������"));
	}
}
