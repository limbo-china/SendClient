// ChgPwsdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ChgPwsdDlg.h"
#include "afxdialogex.h"
#include "commonfunc.h"
#include <string>


// CChgPwsdDlg 对话框

IMPLEMENT_DYNAMIC(CChgPwsdDlg, CDialogEx)

CChgPwsdDlg::CChgPwsdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHGPSWD_DIALOG, pParent)
{
	m_pSlotList = NULL_PTR;
	m_pApplication = new char[255];
	ZeroMemory(m_pApplication, 255);
	strcpy((char*)m_pApplication, "InitToken App");
	//lstrcpy((char*)m_pApplication, "InitToken App");
}

CChgPwsdDlg::~CChgPwsdDlg()
{
	delete[] m_pApplication;
	if (m_pSlotList)
	{
		delete[] m_pSlotList;
		m_pSlotList = NULL_PTR;
	}
}

void CChgPwsdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChgPwsdDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHGPSWDOK, &CChgPwsdDlg::OnClickedButtonChgpswdok)
	ON_BN_CLICKED(IDC_BUTTON_CHGPSWDCANCEL, &CChgPwsdDlg::OnClickedButtonChgpswdcancel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CChgPwsdDlg 消息处理程序


CStringA CChgPwsdDlg::UTF16toUTF8(const CStringW& utf16)
{
	CStringA utf8;
	int len = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0);
	if (len > 1)
	{
		char *ptr = utf8.GetBuffer(len - 1);
		if (ptr) WideCharToMultiByte(CP_UTF8, 0, utf16, -1, ptr, len, 0, 0);
		utf8.ReleaseBuffer();
	}
	return utf8;
}

void CChgPwsdDlg::OnClickedButtonChgpswdok()
{
	// TODO: 在此添加控件通知处理程序代码
	CString m_strOldUserPIN, m_strUserPIN, m_strUserPINConfirm;
	GetDlgItemText(IDC_EDIT_OLDPSWD, m_strOldUserPIN);
	GetDlgItemText(IDC_EDIT_NEWPSWD, m_strUserPIN);
	GetDlgItemText(IDC_EDIT_CONFIRMPSWD, m_strUserPINConfirm);
	
	if (m_strOldUserPIN.Trim() == "")
	{
		MessageBox(_T("请输入旧密码!"));
		GetDlgItem(IDC_EDIT_OLDPSWD)->SetFocus();
		return;
	}

	if (m_strUserPIN.GetLength() < 8)
	{
		MessageBox(_T("密码长度不能少于8位!"));
		GetDlgItem(IDC_EDIT_NEWPSWD)->SetFocus();
		return;
	}

	if (m_strUserPINConfirm != m_strUserPIN)
	{
		MessageBox(_T("两次输入密码不一致"));
		GetDlgItem(IDC_EDIT_NEWPSWD)->SetFocus();
		return;
	}

	CK_RV rv = CKR_OK;
	//rv = C_Initialize(NULL_PTR);
	if (CKR_OK != rv)
	{
		AfxMessageBox(_T("Can't Load ePassNG PKCS#11 Library!"), MB_OK | MB_ICONERROR);
		//ViewErrorMsg(rv);
		C_Finalize(NULL_PTR);
		return;
	}
	if (!ConnectKey())
	{
		AfxMessageBox(_T("无法连接USBKey！"), MB_OK | MB_ICONERROR);
		//C_Finalize(NULL_PTR);
		return;
	}
	CK_SESSION_HANDLE hSession = NULL_PTR;

	rv = C_OpenSession(m_ulSlotId,
		CKF_RW_SESSION | CKF_SERIAL_SESSION,
		&m_pApplication, NULL_PTR, &hSession);
	if (CKR_OK != rv)
	{
		C_Logout(hSession);
		C_CloseSession(hSession);
		ViewErrorMsg(rv);
		//C_Finalize(NULL_PTR);
		return;
	}
	CStringA t1 = UTF16toUTF8(m_strOldUserPIN);
	CStringA t2 = UTF16toUTF8(m_strUserPIN);
	rv = RunC_SetPIN(hSession, (CK_BYTE_PTR)(LPSTR)(LPCSTR)(t1), t1.GetLength(), (CK_BYTE_PTR)(LPSTR)(LPCSTR)(t2), t2.GetLength());
	/*FILE *fp = NULL;
	fopen_s(&fp, "out.txt", "w");
	fprintf_s(fp, "%s\t %s\t%d %d\n*************\n", t1, t2, m_strOldUserPIN.GetLength(), m_strUserPIN.GetLength());
	fclose(fp);*/

	if (CKR_OK != rv)
	{
		C_Logout(hSession);
		C_CloseSession(hSession);
		ViewErrorMsg(rv);
		//C_Finalize(NULL_PTR);
		return;
	}
	C_Logout(hSession);
	C_CloseSession(hSession);
	AfxMessageBox(_T("密码修改成功"), MB_OK);
	//C_Finalize(NULL_PTR);
	OnOK();
}


void CChgPwsdDlg::OnClickedButtonChgpswdcancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


BOOL CChgPwsdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ISonicWndEffect* a_pEffect = GetSonicUI()->CreateWndEffect();
	a_pEffect->Attach(m_hWnd);
	a_pEffect->SetBkColor(RGB(209, 232, 244));
	a_pEffect->SetLayeredAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY);
	a_pEffect->EnableWholeDrag(TRUE);
	::SetWindowPos(GetDlgItem(IDC_STATIC_CHGPSWDCAPTION)->m_hWnd, NULL, 22, 6, 120, 16, SWP_NOZORDER);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChgPwsdDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CImage imgLogo;
	CRect rect;
	rect.SetRect(5, 6, 20, 21);
	imgLogo.Load(_T("res//Client.ico"));
	imgLogo.Draw(dc.m_hDC, rect);
	imgLogo.Destroy();
}


HBRUSH CChgPwsdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


bool CChgPwsdDlg::ConnectKey()
{
	CK_RV rv;
	CK_ULONG ulCount = 0;
	rv = C_GetSlotList(TRUE, NULL_PTR, &ulCount);
	if (CKR_OK != rv)
	{
		AfxMessageBox(_T("Can't Acquire information of Token."), MB_OK | MB_ICONERROR);
		return false;
	}
	if (0 >= ulCount)
	{
		AfxMessageBox(_T("Can't connect to Token ,Make sure you have inserted Token."), MB_OK | MB_ICONERROR);
		return false;
	}

	m_pSlotList = (CK_SLOT_ID_PTR)new CK_SLOT_ID[ulCount];
	if (!m_pSlotList)
	{
		AfxMessageBox(_T("Can't allocate enough memory!"), MB_OK | MB_ICONERROR);
		return false;
	}

	rv = C_GetSlotList(TRUE, m_pSlotList, &ulCount);
	if (CKR_OK != rv)
	{
		AfxMessageBox(_T("Can't Acquire information of Token."), MB_OK | MB_ICONERROR);
		return false;
	}
	if (0 >= ulCount)
	{
		AfxMessageBox(_T("Can't connect to Token,Make sure you have inserted Token."), MB_OK | MB_ICONERROR);
		return false;
	}
	m_ulSlotId = m_pSlotList[0];
	return true;
}

void CChgPwsdDlg::ViewErrorMsg(CK_RV rv)
{
	CString	strMsg = _T("Initialize token failed!");
	char szTmp[20] = { 0 };
	sprintf(szTmp, " [0x%08X]", rv);
	strMsg += szTmp;
	AfxMessageBox(strMsg, MB_OK | MB_ICONERROR);
}