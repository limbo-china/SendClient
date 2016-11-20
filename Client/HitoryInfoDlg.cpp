// HitoryInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "HitoryInfoDlg.h"
#include "afxdialogex.h"
#include "HistoryContentDlg.h"

#define DEFAULT_LOG_FILE "log/upload.inf"

// CHitoryInfoDlg 对话框

IMPLEMENT_DYNAMIC(CHitoryInfoDlg, CDialogEx)

CHitoryInfoDlg::CHitoryInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HISTORYINFO_DIALOG, pParent)
{

}

CHitoryInfoDlg::~CHitoryInfoDlg()
{
}

void CHitoryInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABLE_HISTORYINFO, m_tbShowInfo);
}


BEGIN_MESSAGE_MAP(CHitoryInfoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_CLOSEHISTORY, &CHitoryInfoDlg::OnClickedStaticClosehistory)
	ON_NOTIFY(NM_DBLCLK, IDC_TABLE_HISTORYINFO, OnClickList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TABLE_HISTORYINFO, &CHitoryInfoDlg::OnLvnItemchangedTableHistoryinfo)
END_MESSAGE_MAP()




// CHitoryInfoDlg 消息处理程序

CStringW CHitoryInfoDlg::UTF8toUTF16(const CStringA& utf8)
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

void CHitoryInfoDlg::LoadHistory()
{
	FILE *fp = NULL;
	char line_buf[512] = { 0 };
	CString time;
	CStringA subject;
	int status;
	CString szStatus;
	CStringA content;
	CStringA url;
	CString szNum;
	int node_cnt = 0;

	fopen_s(&fp, DEFAULT_LOG_FILE, "r+");
	if (!fp)
	{
		return;
	}

	while (!feof(fp))
	{
		fgets(line_buf, 512, fp);
		time = line_buf;
		if(time == "" || line_buf[0] == '\n')
			continue;
		fgets(line_buf, 512, fp);
		status = atoi(line_buf);
		if(!isdigit(line_buf[0]))
			continue;
		szStatus.Format(_T("%s"), status == 0 ? _T("成功") : _T("失败"));
		fgets(line_buf, 512, fp);
		subject = line_buf;
		if (subject == "" || line_buf[0] == '\n')
			continue;
		fgets(line_buf, 512, fp);
		url = line_buf;
		if (url == "" || line_buf[0] == '\n')
			continue;
		content = "";
		while (!feof(fp))
		{
			fgets(line_buf, 512, fp);
			if (!strcmp(line_buf, "###################\n"))
				break;
			//line_buf[strlen(line_buf) - 1] = 0;
			content += line_buf;
		}
		CString uft16Content = UTF8toUTF16(content);
		CString uft16Subject = UTF8toUTF16(subject);
		CString uft16Url = UTF8toUTF16(url);
		szNum.Format(_T("%d"), node_cnt + 1);
		m_tbShowInfo.InsertItem(node_cnt, szNum);
		m_tbShowInfo.SetItemText(node_cnt, 1, time);
		m_tbShowInfo.SetItemText(node_cnt, 2, uft16Subject);
		m_tbShowInfo.SetItemText(node_cnt, 3, uft16Url);
		m_tbShowInfo.SetItemText(node_cnt, 4, uft16Content);
		m_tbShowInfo.SetItemText(node_cnt, 5, szStatus);
		//fgets(line_buf, 512, fp);
		
		node_cnt++;
	}
	fclose(fp);
}


BOOL CHitoryInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ISonicWndEffect* a_pEffect = GetSonicUI()->CreateWndEffect();
	a_pEffect->Attach(m_hWnd);
	a_pEffect->SetBkColor(RGB(209, 232, 244));
	a_pEffect->SetLayeredAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY);
	a_pEffect->EnableWholeDrag(TRUE);
	::SetWindowPos(GetDlgItem(IDC_STATIC_HISTORYCAPTION)->m_hWnd, NULL, 22, 6, 120, 16, SWP_NOZORDER);
	::SetWindowPos(GetDlgItem(IDC_STATIC_CLOSEHISTORY)->m_hWnd, NULL, 440, 6, 16, 14, SWP_NOZORDER);

	DWORD dwStyle = m_tbShowInfo.GetExtendedStyle();

	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_tbShowInfo.SetExtendedStyle(dwStyle);
	m_tbShowInfo.InsertColumn(0, _T(""), LVCFMT_LEFT, 40);
	m_tbShowInfo.InsertColumn(1, _T("时间"), LVCFMT_LEFT, 120);
	m_tbShowInfo.InsertColumn(2, _T("标题"), LVCFMT_LEFT, 100);
	m_tbShowInfo.InsertColumn(3, _T("链接"), LVCFMT_LEFT, 100);
	m_tbShowInfo.InsertColumn(4, _T("内容"), LVCFMT_LEFT, 140);
	m_tbShowInfo.InsertColumn(5, _T("状态"), LVCFMT_LEFT, 61);

	LoadHistory();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CHitoryInfoDlg::OnPaint()
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

	rect.SetRect(440, 6, 456, 20);
	imgLogo.Load(_T("res//close.png"));
	imgLogo.Draw(dc.m_hDC, rect);
	imgLogo.Destroy();
}


HBRUSH CHitoryInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CHitoryInfoDlg::OnClickedStaticClosehistory()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
void CHitoryInfoDlg::OnClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		CString time = m_tbShowInfo.GetItemText(pNMListView->iItem, 1);
		CString subject = m_tbShowInfo.GetItemText(pNMListView->iItem, 2);
		CString url = m_tbShowInfo.GetItemText(pNMListView->iItem, 3);
		CString content = m_tbShowInfo.GetItemText(pNMListView->iItem, 4);
		CString status = m_tbShowInfo.GetItemText(pNMListView->iItem, 5);

		CHistoryContentDlg  Dlg(_T("时间: ")+time+"\r\n\r\n主题: "+subject+"\r\n\r\n链接: "+
			url+"\r\n\r\n内容: "+content+"\r\n\r\n状态: "+status);
		Dlg.DoModal();
		
	}
	*pResult = 0;
}

void CHitoryInfoDlg::OnLvnItemchangedTableHistoryinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
