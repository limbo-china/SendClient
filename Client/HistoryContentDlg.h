#pragma once
class CHistoryContentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoryContentDlg)

public:
	CHistoryContentDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CHistoryContentDlg(const CString &content, CWnd* pParent = NULL);
	virtual ~CHistoryContentDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTORYCONTENT_DIALOG };
#endif

private:
	DECLARE_MESSAGE_MAP()
	CString content;
public:
	CClientApp *app = (CClientApp *)AfxGetApp();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//void OnClickedStaticClosehistorycontent();
};
