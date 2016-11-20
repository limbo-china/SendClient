
#include "stdafx.h"
#include "Client.h"
#include "io.h"
#include "afxdialogex.h"
#include "string.h"
#include "HistoryContentDlg.h"
// CRegister 对话框BEGIN_MESSAGE_MAP(CRegister, CDialogEx)

IMPLEMENT_DYNAMIC(CHistoryContentDlg, CDialogEx)

CHistoryContentDlg::CHistoryContentDlg(CWnd* pParent)
	: CDialogEx(IDD_HISTORYCONTENT_DIALOG, pParent)
{
	content = "";
}
CHistoryContentDlg::CHistoryContentDlg(const CString &con, CWnd* pParent) 
	: CDialogEx(IDD_HISTORYCONTENT_DIALOG, pParent) {

	content = con;
}
CHistoryContentDlg::~CHistoryContentDlg()
{

}
BEGIN_MESSAGE_MAP(CHistoryContentDlg, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//ON_STN_CLICKED(IDC_STATIC_CLOSEHISTORYCONTENT, &CHistoryContentDlg::OnClickedStaticClosehistorycontent)
END_MESSAGE_MAP()
BOOL CHistoryContentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  在此添加额外的初始化
	ISonicWndEffect* a_pEffect = GetSonicUI()->CreateWndEffect();
	a_pEffect->Attach(m_hWnd);
	a_pEffect->SetBkColor(RGB(209, 232, 244));
	a_pEffect->SetLayeredAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY);
	a_pEffect->EnableWholeDrag(TRUE);

	SetDlgItemText(IDC_HISTORYCONTENT_EDIT, content);
	UpdateData(FALSE);
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_HISTORYCONTENT_EDIT);
	pEdit->SetReadOnly(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
//void CHistoryContentDlg::OnClickedStaticClosehistorycontent()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CHistoryContentDlg::OnCancel();
//}
void CHistoryContentDlg::OnPaint()
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

	/*rect.SetRect(440, 6, 456, 20);
	imgLogo.Load(_T("res//close.png"));
	imgLogo.Draw(dc.m_hDC, rect);
	imgLogo.Destroy();*/
}
LRESULT CHistoryContentDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialogEx::OnNcHitTest(point);
	return CDialogEx::OnNcHitTest(point);
}
HBRUSH CHistoryContentDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(209, 232, 244));
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
BOOL CHistoryContentDlg::PreTranslateMessage(MSG* pMsg) {
	if (WM_KEYDOWN == pMsg->message && 13 == pMsg->wParam)
	{
		if (GetDlgItem(IDC_INPUTREGISTERCODE) == GetFocus())
		{
			CDialogEx::OnCancel();
			return FALSE;
		}
	}
	int ch = (int)pMsg->wParam;
	if (WM_KEYDOWN == pMsg->message)
	{
		if ((GetKeyState(VK_CONTROL) & 0xFF00) == 0xFF00)
		{
			if ('x' == ch || 'X' == ch)
				::SendMessage(this->GetFocus()->GetSafeHwnd(), WM_CUT, 0, 0);
			if ('C' == ch || 'c ' == ch)
				::SendMessage(this->GetFocus()->GetSafeHwnd(), WM_COPY, 0, 0);
			if ('v' == ch || 'V' == ch)
				::SendMessage(this->GetFocus()->GetSafeHwnd(), WM_PASTE, 0, 0);
			if ('a' == ch || 'A' == ch)
				::SendMessage(this->GetFocus()->GetSafeHwnd(), EM_SETSEL, 0, -1);
			if ('z' == ch || 'Z' == ch)
				::SendMessage(this->GetFocus()->GetSafeHwnd(), EM_REDO, 0, -1);
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}