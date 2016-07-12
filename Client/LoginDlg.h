#pragma once


// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN_DIALOG };
#endif

protected:
	CToolTipCtrl m_tooltip;
	CString m_IP;
	int m_nFlag;
	ISonicString* g_fgtPswd;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL CheckResult(int nResult);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		CStringW UTF8toUTF16(const CStringA & utf8);
	void OnBnClickedLogin();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CClientApp *app = (CClientApp *)AfxGetApp();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedButtonExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedStaticLoginabout();
	afx_msg void OnClickedStaticLoginclose();
	afx_msg void OnEnChangeInputpin();
	afx_msg void OnStnClickedStaticLogincaption();
};
