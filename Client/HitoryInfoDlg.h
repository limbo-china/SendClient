#pragma once
#include "afxcmn.h"


// CHitoryInfoDlg 对话框

class CHitoryInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHitoryInfoDlg)

public:
	CHitoryInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHitoryInfoDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTORYINFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStringW UTF8toUTF16(const CStringA & utf8);
	void LoadHistory();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedStaticClosehistory();
	afx_msg void OnClickList(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_tbShowInfo;
	afx_msg void OnLvnItemchangedTableHistoryinfo(NMHDR *pNMHDR, LRESULT *pResult);
};
