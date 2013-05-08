#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CVoiceLanguageListDlg ��ȭ �����Դϴ�.

class CVoiceLanguageListDlg : public CDialog
{
	DECLARE_DYNAMIC(CVoiceLanguageListDlg)

public:
	CVoiceLanguageListDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CVoiceLanguageListDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VOICE_LANGUAGE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedOk();
	CListCtrl m_clsLanguageList;
	CString m_strLanguage;
	CComboBox m_clsTts;
	afx_msg void OnBnClickedCancel();
};
