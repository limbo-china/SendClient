// Register.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "LoginDlg.h"
#include "io.h"
#include "afxdialogex.h"
#include "Connection.h"
#include "Verify.h"
#include "string.h"
#include "Session.h"
#include "UploadAboutDlg.h"
#include "Register.h"
#include "RegVerify.h"
// CRegister 对话框BEGIN_MESSAGE_MAP(CRegister, CDialogEx)

IMPLEMENT_DYNAMIC(CRegister, CDialogEx)

#define MD5_FILE "md5/MD5.txt"

CRegister::CRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REGISTER_DIALOG, pParent)
{

}

CRegister::~CRegister()
{

}
BEGIN_MESSAGE_MAP(CRegister, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTONREGISTER, &CRegister::OnBnClickedMfcbuttonregister)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_EXIT2, &CRegister::OnBnClickedButtonExit)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_REGISTERABOUT, &CRegister::OnClickedStaticRegisterabout)
	ON_STN_CLICKED(IDC_STATIC_REGISTERCLOSE, &CRegister::OnClickedStaticRegisterclose)
	ON_STN_CLICKED(IDC_STATIC_REGISTERCAPTION, &CRegister::OnStnClickedStaticRegistercaption)
END_MESSAGE_MAP()

BOOL CRegister::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  在此添加额外的初始化
	ISonicWndEffect* a_pEffect = GetSonicUI()->CreateWndEffect();
	a_pEffect->Attach(m_hWnd);
	a_pEffect->SetBkColor(RGB(209, 232, 244));
	a_pEffect->SetLayeredAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY);
	a_pEffect->EnableWholeDrag(TRUE);

	::SetWindowPos(GetDlgItem(IDC_STATIC_REGISTERCAPTION)->m_hWnd, NULL, 22, 6, 120, 16, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_REGISTERCLOSE)->m_hWnd, NULL, 310, 6, 16, 14, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_REGISTERABOUT)->m_hWnd, NULL, 294, 6, 16, 14, SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRegister::OnPaint()
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

	rect.SetRect(310, 6, 326, 20);
	imgLogo.Load(_T("res//close.png"));
	imgLogo.Draw(dc.m_hDC, rect);
	imgLogo.Destroy();

	dc.SetBkColor(RGB(209, 232, 244));
	dc.TextOut(298, 6, _T("?"));
}
LRESULT CRegister::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialogEx::OnNcHitTest(point);
	return CDialogEx::OnNcHitTest(point);
}
HBRUSH CRegister::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CRegister::OnBnClickedMfcbuttonregister()
{
	// TODO: Add your control notification handler code here
	CString rcode = _T("");
	GetDlgItemText(IDC_INPUTREGISTERCODE, rcode);
	CRegVerify rv(rcode);
	if (rv.IsRegVaild(rcode))
	{
		if (rv.GetCpuByCmd())
		{
			//MessageBox(_T("cpu序列号获取成功"));
			char md[33];
			strcpy(md, rv.GetMd5());
			FILE *fp = NULL;
			fopen_s(&fp, MD5_FILE, "w+");
			fprintf_s(fp, "%s", md);
			fclose(fp);
			MessageBox(_T("激活成功，请登录"));
			CDialogEx::OnOK();
		}
		else
		{
			MessageBox(_T("本地主机序列号获取失败"));
		}
	}
	else
	{
		MessageBox(_T("激活码无效，请重新输入"));
		GetDlgItem(IDC_INPUTREGISTERCODE)->SetFocus();
	}
}
void CRegister::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代
	CDialogEx::OnCancel();
}
void CRegister::OnClickedStaticRegisterabout() {
	CUploadAboutDlg abtDlg;
	abtDlg.DoModal();
}
void CRegister::OnClickedStaticRegisterclose() {
	CRegister::OnCancel();
}
void CRegister::OnStnClickedStaticRegistercaption() {

}
BOOL CRegister::PreTranslateMessage(MSG* pMsg) {
	if (WM_KEYDOWN == pMsg->message && 13 == pMsg->wParam)
	{
		if (GetDlgItem(IDC_INPUTREGISTERCODE) == GetFocus())
		{
			OnBnClickedMfcbuttonregister();
			return FALSE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}