#pragma once
#include "afxwin.h"

#define WM_SHOWTASK WM_USER+1

// CUploadInfo �Ի���

class CUploadInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CUploadInfo)

public:
	CUploadInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUploadInfo();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UPLOADINFO_DIALOG };
#endif

private:
	HICON m_hIcon;
	NOTIFYICONDATA m_nid;
	void CreateTray();
	void ToTray();
	void PopTrayMenu();
	LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
private:
	bool m_isShown;
	ISonicImage* g_pImgBG;        //����ͼƬ 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Subject;
	CEdit m_Content;
	
	HBITMAP hBitmap;
	HBITMAP hBitmap1;

	CClientApp *app = (CClientApp *)AfxGetApp();

	void ClearALlEdit();
	CStringA UTF16toUTF8(const CStringW & utf16);
	CStringW UTF8toUTF16(const CStringA & utf8);
	int DoUpdate();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitSession();
	void InitUI();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void StartListenThread();
	afx_msg void OnClickedSendButton();
	afx_msg void OnClickedSendMenu();
	afx_msg void OnClickedConfigMenu();
	afx_msg void OnClickedAboutMenu();
	afx_msg void OnClickedModifyPswdMenu();
	afx_msg void OnDestroy();
	afx_msg void OnChangeInfoContent();
	afx_msg void OnClickedStaticClose();
	afx_msg void OnClickedButtonShowhistory();
	afx_msg void OnClickedStaticUploadmin();
	afx_msg void OnClickedStaticUploadabout();
	CComboBox m_cbDataSource;
	bool m_bIfLogin;
	CEdit m_cURL;
	CStatic m_sURL;
	afx_msg void OnEnChangeInfoSubject();
	afx_msg void OnEnChangeEditUrl();
	BOOL CUploadInfo::PreTranslateMessage(MSG* pMsg);
};
