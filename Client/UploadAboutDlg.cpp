// UploadAboutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "UploadAboutDlg.h"
#include "afxdialogex.h"


// CUploadAboutDlg 对话框

IMPLEMENT_DYNAMIC(CUploadAboutDlg, CDialogEx)

CUploadAboutDlg::CUploadAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{

}

CUploadAboutDlg::~CUploadAboutDlg()
{
}

void CUploadAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUploadAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUploadAboutDlg 消息处理程序
