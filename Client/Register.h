#pragma once

// Register 对话框

class CRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CRegister)

public:
	CRegister(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegister();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER_DIALOG };
#endif

private:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//BOOL CheckResult(int nResult);
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg
	//	CStringW UTF8toUTF16(const CStringA & utf8);
	void OnBnClickedMfcbuttonregister();
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	CClientApp *app = (CClientApp *)AfxGetApp();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedButtonExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedStaticRegisterabout();
	afx_msg void OnClickedStaticRegisterclose();
	afx_msg void OnStnClickedStaticRegistercaption();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
