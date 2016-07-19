// UploadInfo.cpp : ʵ���ļ�
//

#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#include "stdafx.h"
#include "Client.h"
#include "UploadInfo.h"
#include "afxdialogex.h"
#include "Connection.h"
#include "Session.h"
#include "Config.h"
#include "LoginDlg.h"
#include "Register.h"
#include "RegVerify.h"
#include "ConfigDlg.h"
#include "HitoryInfoDlg.h"
#include "UploadAboutDlg.h"
#include "ChgPwsdDlg.h"
#include "CheckTokenStatus.h"
#include "io.h"

// CUploadInfo �Ի���

char g_sDataServerIP[20];
#define DEFAULT_CONFIG_FILE "config/system.cfg"
#define DEFAULT_LOG_FILE "log/upload.inf"
#define MD5_FILE "md5/MD5.txt"


IMPLEMENT_DYNAMIC(CUploadInfo, CDialogEx)

CUploadInfo::CUploadInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UPLOADINFO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

CUploadInfo::~CUploadInfo()
{

}

void CUploadInfo::CreateTray()
{
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDI_ICON1;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_SHOWTASK;
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	wcscpy(m_nid.szTip, _T("���ݵ��빤��v1.1"));
	Shell_NotifyIcon(NIM_ADD, &m_nid);
}

void CUploadInfo::ToTray()
{
	ShowWindow(SW_HIDE);
	m_isShown = false;
}

void CUploadInfo::PopTrayMenu()
{
	LPPOINT lpoint = new tagPOINT;
	::GetCursorPos(lpoint); // �õ����λ��
	CMenu menu;
	menu.CreatePopupMenu(); // ����һ������ʽ�˵�
	menu.AppendMenu(MF_STRING, IDM_TRAYSEND, _T("��������"));
	menu.AppendMenu(MF_STRING, IDM_TRAYCONFIG, _T("ϵͳ����"));
	menu.AppendMenu(MF_STRING, IDM_TRAYMODIFYPSWD, _T("�޸�����"));
	menu.AppendMenu(MF_SEPARATOR, IDM_TRAYSEPERATOR1, _T(""));
	menu.AppendMenu(MF_STRING, IDM_TRAYABOUTBOX, _T("ϵͳ��Ϣ"));
	menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�˳�"));
	SetForegroundWindow();//////////////������һ��͹رղ��˲˵�������һ��Ҫ�ŵ�����
	menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
	HMENU hmenu = menu.Detach();
	menu.DestroyMenu();
	delete lpoint;
}

LRESULT CUploadInfo::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDI_ICON1)
		return 1;
	switch (lParam)
	{
		case WM_RBUTTONDOWN: // �Ҽ�����ʱ�����˵�
		{
			PopTrayMenu();
		}
		break;
		case WM_LBUTTONDBLCLK: // ˫������Ĵ���
		{
			if (!m_isShown)
				ShowWindow(SW_SHOW);
			else
				ShowWindow(SW_HIDE);
			m_isShown = !m_isShown;
		}
		break;
	}
	return 0;
}

void CUploadInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO_SUBJECT, m_Subject);
	DDX_Control(pDX, IDC_INFO_CONTENT, m_Content);
	DDX_Control(pDX, IDC_COMBO_DATASOURCE, m_cbDataSource);
	DDX_Control(pDX, IDC_EDIT_URL, m_cURL);
	DDX_Control(pDX, IDC_STATIC_URL, m_sURL);
}
static UINT WINAPI IsConnected(CUploadInfo* dlg)
{
	SocketOperation conn;
	if (conn.Connect(g_sDataServerIP, session.m_uDataServerPort) != 0)
	{
		dlg->MessageBox(_T("�޷�������������"));
	}
	conn.Close();
	return 0;
}

BEGIN_MESSAGE_MAP(CUploadInfo, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CUploadInfo::OnClickedSendButton)
	ON_BN_CLICKED(IDM_TRAYSEND, &CUploadInfo::OnClickedSendMenu)
	ON_BN_CLICKED(IDM_TRAYCONFIG, &CUploadInfo::OnClickedConfigMenu)
	ON_BN_CLICKED(IDM_TRAYMODIFYPSWD, &CUploadInfo::OnClickedModifyPswdMenu)
	ON_BN_CLICKED(IDM_TRAYABOUTBOX, &CUploadInfo::OnClickedAboutMenu)
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_INFO_CONTENT, &CUploadInfo::OnChangeInfoContent)
	ON_STN_CLICKED(IDC_STATIC_CLOSE, &CUploadInfo::OnClickedStaticClose)
	ON_BN_CLICKED(IDC_BUTTON_SHOWHISTORY, &CUploadInfo::OnClickedButtonShowhistory)
	ON_STN_CLICKED(IDC_STATIC_UPLOADMIN, &CUploadInfo::OnClickedStaticUploadmin)
	ON_STN_CLICKED(IDC_STATIC_UPLOADABOUT, &CUploadInfo::OnClickedStaticUploadabout)
	ON_EN_CHANGE(IDC_INFO_SUBJECT, &CUploadInfo::OnEnChangeInfoSubject)
	ON_EN_CHANGE(IDC_EDIT_URL, &CUploadInfo::OnEnChangeEditUrl)
END_MESSAGE_MAP()


// CUploadInfo ��Ϣ�������
//
//void CUploadInfo::ConvertGBKToUtf8(CString &strGBK)
//{
//	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL, 0);
//	wchar_t * wszUtf8 = new wchar_t[len];
//	memset(wszUtf8, 0, len);
//	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, wszUtf8, len);
//
//	len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
//	char *szUtf8 = new char[len + 1];
//	memset(szUtf8, 0, len + 1);
//	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL, NULL);
//
//	strGBK = szUtf8;
//	delete[] szUtf8;
//	delete[] wszUtf8;
//}


void CUploadInfo::ClearALlEdit()
{
	m_Subject.SetSel(0, -1);
	m_Subject.Clear();
	m_Content.SetSel(0, -1);
	m_Content.Clear();
	m_cURL.SetSel(0, -1);
	m_cURL.Clear();
}


CStringA CUploadInfo::UTF16toUTF8(const CStringW& utf16)
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

CStringW CUploadInfo::UTF8toUTF16(const CStringA& utf8)
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


int CUploadInfo::DoUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sSubject;
	CString sContent;
	CString sURL;
	m_Subject.GetWindowTextW(sSubject);
	//m_Subject.GetWindowTextA(sSubject);
	m_Content.GetWindowTextW(sContent);
	m_cURL.GetWindowTextW(sURL);
	if (sSubject.Trim() == "" || sSubject.GetLength() == 0)
	{
		MessageBox(_T("���ⲻ��Ϊ��"));
		GetDlgItem(IDC_INFO_SUBJECT)->SetFocus();
		return -1;
	}
	if (sContent.Trim() == "" || sContent.GetLength() == 0)
	{
		MessageBox(_T("���ݲ���Ϊ��"));
		GetDlgItem(IDC_INFO_CONTENT)->SetFocus();
		return -1;
	}
	SocketOperation conn;
	int ret = -2;
	USES_CONVERSION;
	if (conn.Connect(g_sDataServerIP, session.m_uDataServerPort) == 0)
	{
		CString tmBuffer;
		
		CStringA sName(session.m_sUsrInfo);
		CString sTime;
		
		CString sStartToken = _T("%&");
		CString sEndToekn = _T("\n&%\n");
		CString utf16Name = UTF8toUTF16(sName);
		CString szDataSource;
		m_cbDataSource.GetWindowText(szDataSource);
		int nDataSource = -1;
		if (szDataSource == _T("������"))
			nDataSource = 231;
		else if (szDataSource == _T("ʮ�߾�"))
			nDataSource = 230;
		CString TPItem;
		TPItem.Format(_T("\nTP:%d"), nDataSource);
		CString STItem = _T("\nST:");
		CString CTItem = _T("\nCT:");
		CString TXItem = _T("\nTX:");
		CString AUItem = _T("\nAU:");
		CString MIItem = _T("\nMI:");
		
		//sContent.Replace(_T("\r\n"), _T("\t"));
		
		CTime tmSCan = CTime::GetCurrentTime();
		sTime = tmSCan.Format("%Y-%m-%d %H:%M:%S");
		//ConvertGBKToUtf8(sSubject);
		//ConvertGBKToUtf8(sContent);
		tmBuffer = sStartToken + TPItem + STItem + sSubject + AUItem + utf16Name + MIItem + sURL + CTItem + sTime + TXItem + sContent + sEndToekn;
		
		
		CStringA utf8Str = UTF16toUTF8(tmBuffer);
		int tag = htonl(2);
		int nLength = utf8Str.GetLength() + 1;
		char *sndBuffer = (char*)calloc(1, nLength + 4 + 4);

		int cur = 0;
		memcpy(sndBuffer + cur, &tag, sizeof(tag));
		cur += sizeof(tag);
		nLength = htonl(nLength);
		memcpy(sndBuffer + cur, &nLength, sizeof(nLength));
		cur += sizeof(nLength);
		
		char* ok = (LPSTR)(LPCSTR)utf8Str;
		memcpy(sndBuffer + cur, ok, strlen(ok));
		cur += strlen(ok);


		//MessageBox(tmBuffer);


		/*FILE *fp = NULL;
		fopen_s(&fp, "out.txt", "w");
		fprintf_s(fp, "%s\t %s\t%d %d\n*************\n", utf8Str, sndBuffer + 8, utf8Str.GetLength(), cur);
		fclose(fp);*/
		
		if (send(conn.m_sock, sndBuffer, cur + 1, 0) < 0)
		{
			free(sndBuffer);
			MessageBox(_T("����ʧ�ܣ�����ϵͳ����"));
		}
		else
		{
			free(sndBuffer);
			MessageBox(_T("����ɹ�"));
			ret = 0;
		}
	}
	else
	{
		MessageBox(_T("�޷�������������"));
	}
	conn.Close();
	return ret;
}


HBRUSH CUploadInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CUploadInfo::InitSession()
{
	GetConfigValue(DEFAULT_CONFIG_FILE, "DataServer", session.m_sDataServerIP);
	
	char ValueBuffer[256] = { 0 };
	GetConfigValue(DEFAULT_CONFIG_FILE, "DataServerPort", ValueBuffer);
	session.m_uDataServerPort = atoi(ValueBuffer);
	memset(ValueBuffer, 0, 256);
	StartListenThread();
}

void CUploadInfo::InitUI()
{
	//���ڷ�� 
	SetWindowText(_T("���ݵ��빤��v1.1"));
	SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE)&~(WS_CAPTION));
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE)&WS_EX_TOOLWINDOW);

	//���δ��� 
	ISonicWndEffect* a_pEffect = GetSonicUI()->CreateWndEffect();
	a_pEffect->Attach(m_hWnd);
	a_pEffect->SetBkColor(RGB(209, 232, 244));
	a_pEffect->SetLayeredAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY);
	a_pEffect->EnableWholeDrag(TRUE);

	//����λ�� 
	::SetWindowPos(m_hWnd, NULL, 0, 0, 416, 484+40, SWP_NOMOVE);
	::SetWindowPos(GetDlgItem(IDC_STATIC_MAINCAPTION)->m_hWnd, NULL, 22, 6, 120, 16, SWP_NOZORDER);

	::SetWindowPos(GetDlgItem(IDC_INFO_SUBJECT)->m_hWnd, NULL, 8, 120, 401, 23, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_INFO_CONTENT)->m_hWnd, NULL, 8, 217, 401, 267, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_EDIT_URL)->m_hWnd, NULL, 8, 165, 401, 23, SWP_NOZORDER);

	::SetWindowPos(GetDlgItem(IDC_STATIC_SUBJECT)->m_hWnd, NULL, 8, 101, 30, 16, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_CONTENT)->m_hWnd, NULL, 8, 190, 30, 16, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_URL)->m_hWnd, NULL, 8, 145, 30, 16, SWP_NOZORDER);



	SetDlgItemText(IDC_STATIC_USRID, app->userid);
	SetDlgItemText(IDC_STATIC_USRNAME, app->username);
	::SetWindowPos(GetDlgItem(IDC_STATIC_USRID)->m_hWnd, NULL, 83, 30, 180, 16, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_USRNAME)->m_hWnd, NULL,83, 50, 120, 16, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_DATASOURCE)->m_hWnd, NULL, 83, 77, 80, 16, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_COMBO_DATASOURCE)->m_hWnd, NULL, 153, 75, 120, 16, SWP_NOZORDER);

	::SetWindowPos(GetDlgItem(IDC_STATIC_CLOSE)->m_hWnd, NULL, 390, 6, 16, 14, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_UPLOADMIN)->m_hWnd, NULL, 374, 6, 16, 14, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_UPLOADABOUT)->m_hWnd, NULL, 358, 6, 16, 14, SWP_NOZORDER);

	hBitmap = (HBITMAP)::LoadImage(NULL, _T("res/in.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	hBitmap1 = (HBITMAP)::LoadImage(NULL, _T("res/history.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	((CButton*)GetDlgItem(IDC_BUTTON_SEND))->SetBitmap(hBitmap);
	((CButton*)GetDlgItem(IDC_BUTTON_SHOWHISTORY))->SetBitmap(hBitmap1);

	::SetWindowPos(GetDlgItem(IDC_BUTTON_SEND)->m_hWnd, NULL, 252, 493, 53, 23, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_BUTTON_SHOWHISTORY)->m_hWnd, NULL, 90, 493, 53, 23, SWP_NOZORDER);

	m_Subject.ShowScrollBar(SB_VERT, 0);
	m_Content.ShowScrollBar(SB_VERT, 0);
	m_cbDataSource.AddString(_T("������"));
	m_cbDataSource.AddString(_T("ʮ�߾�"));
	m_cbDataSource.SetCurSel(0);

	CreateTray();
}


BOOL CUploadInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	 //TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	int acc = access(MD5_FILE, 0);
	if (acc != 0) 
	{
		MessageBox(_T("�뼤��"));
		CRegister rgdlg;
		if (rgdlg.DoModal() != IDOK)
			SendMessage(WM_CLOSE);
		else {
			if (!session.m_bLogin)
			{
				CLoginDlg ldlg;
				if (ldlg.DoModal() != IDOK)
					SendMessage(WM_CLOSE);
				else
				{
					session.m_bLogin = true;
					InitSession();
					strcpy_s(g_sDataServerIP, session.m_sDataServerIP);
				}
			}
		}
	}
	else 
	{
		FILE *fp = NULL;
		fopen_s(&fp, MD5_FILE, "r+");
		char md[33];
		char ch; int cur = 0;
		do
		{
			ch = fgetc(fp);
			md[cur++] = ch;
		} while (cur<32 && ch != EOF);
		md[cur] = 0;
		fclose(fp);
		CRegVerify rv;
		if (rv.IsRegLocal(md))
		{
			MessageBox(_T("�Ѽ�����¼"));
			if (!session.m_bLogin)
			{
				CLoginDlg ldlg; 
				if (ldlg.DoModal() != IDOK)
					SendMessage(WM_CLOSE);
				else
				{
					session.m_bLogin = true;
					InitSession();
					strcpy_s(g_sDataServerIP, session.m_sDataServerIP);
				}
			}
		}
		else 
		{
			MessageBox(_T("�Ǳ��������룬�����¼���"));
			CRegister rgdlg;
			if (rgdlg.DoModal() != IDOK)
				SendMessage(WM_CLOSE);
			else {
				if (!session.m_bLogin)
				{
					CLoginDlg ldlg;
					if (ldlg.DoModal() != IDOK)
						SendMessage(WM_CLOSE);
					else
					{
						session.m_bLogin = true;
						InitSession();
						strcpy_s(g_sDataServerIP, session.m_sDataServerIP);
					}
				}
			}
		}	
	}
	
	InitUI();
	
	m_isShown = true;
	m_bIfLogin = true;

	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IsConnected, this, 0, NULL);
	CloseHandle(hThread);	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CUploadInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CImage imgLogo;
	CRect rect(8, 33, 68, 93);
	imgLogo.Load(_T("res//logo.png"));
	imgLogo.Draw(dc.m_hDC, rect);
	imgLogo.Destroy();

	rect.SetRect(5, 6, 20, 21);
	imgLogo.Load(_T("res//Client.ico"));
	imgLogo.Draw(dc.m_hDC, rect);
	imgLogo.Destroy();

	rect.SetRect(390, 6, 406, 20);
	imgLogo.Load(_T("res//close.png"));
	imgLogo.Draw(dc.m_hDC, rect);
	imgLogo.Destroy();

	rect.SetRect(374, 6, 390, 20);
	imgLogo.Load(_T("res//min.png"));
	imgLogo.Draw(dc.m_hDC, rect);
	imgLogo.Destroy();

	dc.SetBkColor(RGB(209, 232, 244));
	dc.TextOut(362, 6, _T("?"));
}

void SaveUploadInfo(const char *subject, const char *content, const char *uptime, int state)
{
	FILE *fp = NULL;
	fopen_s(&fp, DEFAULT_LOG_FILE, "a+");
	fprintf_s(fp, "%s\n%d\n%s\n%s\n###################\n", uptime, state, subject, content);
	fclose(fp);
}

static UINT WINAPI ListenUKStatus(CUploadInfo* dlg)
{
	CCheckTokenStatus inf;
	inf.LoadLib();
	UINT status = 0;
	while (TRUE)
	{
		status = inf.GetStatus();
		if (status == 0)
		{
			dlg->m_bIfLogin = false;
			dlg->MessageBox(_T("Key�Ѱγ�,�����˳�"));
			ExitProcess(-1);
			//if (session.m_bIfOnline)
			//{
			//	//dlg->MessageBox("Key�Ѱγ�");

			//	dlg->OnBnClickedStopService();
			//	session.m_bIfOnline = false;
			//}
		}
		else if (status == 1)
		{
			dlg->m_bIfLogin = true;
			//if (!session.m_bIfOnline)
			//{
			//	if (::MessageBox(NULL, "�Ѳ���Key ȷ��Ҫ������", "����ȷ��", MB_YESNO) == IDYES)
			//	{
			//		dlg->OnBnClickedStartService();
			//	}
			//	else
			//	{
			//		session.m_bIfOnline = true;
			//	}
			//	//
			//	//session.m_bIfOnline = true;
			//}
			/*if (!dlg->m_show)
			{
			dlg->KeepWindowShown();
			}*/
		}
		Sleep(500);
	}

	return 0;
}

void CUploadInfo::StartListenThread()
{
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenUKStatus, this, 0, NULL);
	CloseHandle(hThread);
}

void CUploadInfo::OnClickedSendButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret = DoUpdate();
	if (ret == -1)
		return;
	CString subject;
	GetDlgItemText(IDC_INFO_SUBJECT, subject);

	CString content;
	GetDlgItemText(IDC_INFO_CONTENT, content);

	CString strDateTime;
	CTime c_time = CTime::GetCurrentTime();
	strDateTime = c_time.Format("%Y-%m-%d %H:%M:%S");
	//USES_CONVERSION;
	SaveUploadInfo(UTF16toUTF8(subject), UTF16toUTF8(content), UTF16toUTF8(strDateTime), ret == 0 ? 0 : 1);

	if(ret == 0)
		ClearALlEdit();
}


void CUploadInfo::OnClickedSendMenu()
{
	if (!m_isShown)
	{
		ShowWindow(SW_SHOW);
		m_isShown = !m_isShown;
	}
}


void CUploadInfo::OnClickedConfigMenu()
{
	CConfigDlg cdlg;
	cdlg.DoModal();
}

void CUploadInfo::OnClickedAboutMenu()
{
	CUploadAboutDlg abtDlg;
	abtDlg.DoModal();
}

void CUploadInfo::OnClickedModifyPswdMenu()
{
	CChgPwsdDlg itd;
	itd.DoModal();
}


void CUploadInfo::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}


void CUploadInfo::OnChangeInfoContent()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	int nLineNum = m_Content.GetLineCount();
	if (nLineNum < 15)
		m_Content.ShowScrollBar(SB_VERT, 0);
	else
		m_Content.ShowScrollBar(SB_VERT, 1);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CUploadInfo::OnClickedStaticClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ToTray();
}


void CUploadInfo::OnClickedButtonShowhistory()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CHitoryInfoDlg hiDlg;
	hiDlg.DoModal();
}


void CUploadInfo::OnClickedStaticUploadmin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_MINIMIZE);
}


void CUploadInfo::OnClickedStaticUploadabout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CUploadAboutDlg abtDlg;
	abtDlg.DoModal();
}


void CUploadInfo::OnEnChangeInfoSubject()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CUploadInfo::OnEnChangeEditUrl()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

