#pragma once


// CUploadAboutDlg �Ի���

class CUploadAboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUploadAboutDlg)

public:
	CUploadAboutDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUploadAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
