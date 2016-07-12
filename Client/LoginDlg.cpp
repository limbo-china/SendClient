// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "Connection.h"
#include "Verify.h"
#include "string.h"
#include "Session.h"
#include "UploadAboutDlg.h"
// CLoginDlg �Ի���
//extern CClientSession session;

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ID_LOGIN, pParent)
{

}

CLoginDlg::~CLoginDlg()
{

}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CLoginDlg::CheckResult(int nResult)
{
	switch (nResult)
	{
	case IA_GENETAL:
		return true;
	case IA_SOCKET_SEND_ERROR:
	case IA_SOCKET_RECV_ERROR:
	case IA_UKEY_CONNECT_ERROR:
		MessageBox(_T("���Ӵ���"));
		return false;
	case IA_UKEY_LOGIN_ERROR:
		MessageBox(_T("PINֵ���Ϸ�"));
		return false;
	case IA_NOT_VAL_USR:
		MessageBox(_T("����USBKeyδע��"));
		return false;
	case IA_VERIFY_FAILER:
		MessageBox(_T("��֤ʧ��"));
		return false;
	case IA_INIT_ADEPTER_INFO_ERROR:
		MessageBox(_T("��ʼ��������Ϣ���� �Ƿ��Ѱ�װ����"));
	default:
		MessageBox(_T("�ڲ�����"));
		return false;
	}
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CLoginDlg::OnBnClickedLogin)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CLoginDlg::OnBnClickedButtonExit)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_LOGINABOUT, &CLoginDlg::OnClickedStaticLoginabout)
	ON_STN_CLICKED(IDC_STATIC_LOGINCLOSE, &CLoginDlg::OnClickedStaticLoginclose)
	ON_EN_CHANGE(IDC_INPUTPIN, &CLoginDlg::OnEnChangeInputpin)
	ON_STN_CLICKED(IDC_STATIC_LOGINCAPTION, &CLoginDlg::OnStnClickedStaticLogincaption)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������


CStringW CLoginDlg::UTF8toUTF16(const CStringA& utf8)
{
	CStringW utf16;
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	if (len>1)
	{
		wchar_t *ptr = utf16.GetBuffer(len - 1);
		if (ptr) MultiByteToWideChar(CP_UTF8, 0, utf8, -1, ptr, len);
		utf16.ReleaseBuffer();
	}
	return utf16;
}

void CLoginDlg::OnBnClickedLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString pswd = _T("");
	GetDlgItemText(IDC_INPUTPIN, pswd);

	Verify ck(-1);
	USES_CONVERSION;
	int ret = ck.cVerifyDataServer(T2A(pswd));
	if (CheckResult(ret))
	{
		memcpy(session.m_sUsrInfo, ck.GetUsrInfo(), strlen(ck.GetUsrInfo()));
		memcpy(session.m_sSerial, ck.GetSerial(), strlen(ck.GetSerial()));
		CStringA username = session.m_sUsrInfo;
		CStringA userid = session.m_sSerial;

		app->userid += _T("�û�ID��");
		app->userid += UTF8toUTF16(userid);
		app->username += _T("�û�����");
		app->username += UTF8toUTF16(username);

		CDialogEx::OnOK();
	}
	else
	{
		GetDlgItem(IDC_INPUTPIN)->SetFocus();
	}
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_tooltip.m_hWnd != NULL)
		m_tooltip.RelayEvent(pMsg);
	if (WM_KEYDOWN == pMsg->message && 13 == pMsg->wParam)
	{
		if (GetDlgItem(IDC_INPUTPIN) == GetFocus())
		{
			OnBnClickedLogin();
			return FALSE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CLoginDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialogEx::OnNcHitTest(point);
	return CDialogEx::OnNcHitTest(point);
}


void CLoginDlg::OnBnClickedButtonExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLoginDlg::OnCancel();
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ISonicWndEffect* a_pEffect = GetSonicUI()->CreateWndEffect();
	a_pEffect->Attach(m_hWnd);
	a_pEffect->SetBkColor(RGB(209, 232, 244));
	a_pEffect->SetLayeredAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY);
	a_pEffect->EnableWholeDrag(TRUE);

	::SetWindowPos(GetDlgItem(IDC_STATIC_LOGINCAPTION)->m_hWnd, NULL, 22, 6, 120, 16, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_LOGINCLOSE)->m_hWnd, NULL, 310, 6, 16, 14, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_LOGINABOUT)->m_hWnd, NULL, 294, 6, 16, 14, SWP_NOZORDER);

	m_tooltip.Create(GetDlgItem(IDC_INPUTPIN));
	m_tooltip.AddTool(GetDlgItem(IDC_INPUTPIN), _T("�������������뼰ʱ��ϵ����Ա"));
	//m_tooltip.SetTipTextColor(RGB(0, 0, 255));
	m_tooltip.SetDelayTime(150);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
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


HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CLoginDlg::OnClickedStaticLoginabout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUploadAboutDlg abtDlg;
	abtDlg.DoModal();
}


void CLoginDlg::OnClickedStaticLoginclose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLoginDlg::OnCancel();
}


void CLoginDlg::OnEnChangeInputpin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CLoginDlg::OnStnClickedStaticLogincaption()
{
	// TODO: Add your control notification handler code here
}
