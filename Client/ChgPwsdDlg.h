#pragma once


// CChgPwsdDlg 对话框

class CChgPwsdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChgPwsdDlg)

public:
	CChgPwsdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChgPwsdDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHGPSWD_DIALOG };
#endif

protected:
	CK_SLOT_ID_PTR m_pSlotList;
	CK_VOID_PTR m_pApplication;
	CK_ULONG m_ulSlotId;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg
		CStringA UTF16toUTF8(const CStringW & utf16);
	void OnClickedButtonChgpswdok();
	afx_msg void OnClickedButtonChgpswdcancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	bool ConnectKey();
	void ViewErrorMsg(CK_RV rv);
};
