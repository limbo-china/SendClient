// ConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"

#define DEFAULT_IP_LIST "config/ip.list"
extern char g_sDataServerIP[20];

// CConfigDlg �Ի���

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONFIG_DIALOG, pParent)
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERVERLIST, m_cbServerList);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CConfigDlg::OnClickedButtonApply)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CONFIGCANCEL, &CConfigDlg::OnBnClickedButtonCancel)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CConfigDlg ��Ϣ�������


void CConfigDlg::InitServerList()
{
	m_sServerList.clear();
	FILE* fp = fopen(DEFAULT_IP_LIST, "r");
	if (fp == NULL)
	{
		MessageBox(_T("δ�ҵ������ļ�"));
		return;
	}
	char LineBuffer[256] = { 0 };
	while (fgets(LineBuffer, 256, fp) != NULL)
	{
		if (strlen(LineBuffer) < 4)
			continue;
		if (LineBuffer[0] == '#')
			continue;
		if (LineBuffer[strlen(LineBuffer) - 1] == 10)
		{
			LineBuffer[strlen(LineBuffer) - 1] = '\0';
		}
		USES_CONVERSION;
		CString tmp = A2T(LineBuffer);
		m_sServerList.insert(tmp);
		//m_cbServerList.AddString(LineBuffer);
		//MessageBox(_T(LineBuffer));
		memset(LineBuffer, 0, 256);
	}

	set<CString>::iterator iter = m_sServerList.begin();
	set<CString>::iterator iter_end = m_sServerList.end();
	for (; iter != iter_end; ++iter)
	{
		m_cbServerList.AddString(*iter);
	}

	m_cbServerList.SetCurSel(0);

	fclose(fp);
}

BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitServerList();
	USES_CONVERSION;
	m_cbServerList.SetWindowText(A2T(g_sDataServerIP));
	ISonicWndEffect* a_pEffect = GetSonicUI()->CreateWndEffect();
	a_pEffect->Attach(m_hWnd);
	a_pEffect->SetBkColor(RGB(209, 232, 244));
	a_pEffect->SetLayeredAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY);
	a_pEffect->EnableWholeDrag(TRUE);
	::SetWindowPos(GetDlgItem(IDC_STATIC_CONFIGCAPTION)->m_hWnd, NULL, 22, 6, 120, 16, SWP_NOZORDER);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

bool isIPAddress(const char *s)
{
	const char *pChar;
	bool rv = true;
	int tmp1, tmp2, tmp3, tmp4, i;

	while (1)
	{
		i = sscanf(s, "%d.%d.%d.%d", &tmp1, &tmp2, &tmp3, &tmp4);
		if (i != 4)
		{
			rv = false;
			break;
		}
		if ((tmp1 > 255) || (tmp2 > 255) || (tmp3 > 255) || (tmp4 > 255))
		{
			rv = false;
			break;
		}
		for (pChar = s; *pChar != 0; pChar++)
		{
			if ((*pChar != '.')
				&& ((*pChar < '0') || (*pChar > '9')))
			{
				rv = false;
				break;
			}
		}
		break;
	}

	return rv;
}

void CConfigDlg::OnClickedButtonApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	CString selIP;
	m_cbServerList.GetWindowText(selIP);
	/*m_cbServerList.GetLBText(m_cbServerList.GetCurSel(), selIP);*/
	if (!isIPAddress(T2A(selIP)))
	{
		MessageBox(_T("��������IP�����Ƿ�����"));
		return;
	}
	strcpy_s(g_sDataServerIP, T2A(selIP));
	//g_sDataServerIP = selIP;
	m_sServerList.insert(selIP);
	SaveServerList();
	OnOK();
}


void CConfigDlg::SaveServerList()
{
	FILE* fp = fopen(DEFAULT_IP_LIST, "w");
	set<CString>::iterator iter = m_sServerList.begin();
	set<CString>::iterator iter_end = m_sServerList.end();
	USES_CONVERSION;
	for (; iter != iter_end; ++iter)
	{
		CString tmp = *iter;
		fprintf(fp, "%s\n", T2A(tmp));
	}
	fclose(fp);
}


LRESULT CConfigDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(point) ? HTCAPTION : CDialogEx::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CConfigDlg::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CConfigDlg::OnCancel();
}


HBRUSH CConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CConfigDlg::OnPaint()
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
}
