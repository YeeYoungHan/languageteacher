#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CVoiceLanguageListDlg 대화 상자입니다.

class CVoiceLanguageListDlg : public CDialog
{
	DECLARE_DYNAMIC(CVoiceLanguageListDlg)

public:
	CVoiceLanguageListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVoiceLanguageListDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VOICE_LANGUAGE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
