// EnglishTeacherDlg.h : header file
//

#pragma once

#include "StudyList.h"
#include "DADEdit.h"
#include "afxwin.h"

#define EXPEND_HEIGHT	290
#define SHOW_SETUP_TEXT	_T("Show Setup")
#define HIDE_SETUP_TEXT	_T("Hide Setup")

// CEnglishTeacherDlg dialog
class CEnglishTeacherDlg : public CDialog
{
// Construction
public:
	CEnglishTeacherDlg(CWnd* pParent = NULL);	// standard constructor
	~CEnglishTeacherDlg();

// Dialog Data
	enum { IDD = IDD_ENGLISHTEACHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelectFile();
	CString m_strKorean;
	CString m_strEnglish;
	CString m_strFileName;
	CString m_strCurrentKorean;
	CString m_strCurrentEnglish;

	bool StartShowText();
	bool ShowData();
	bool ShowData( CString & strFirst, CString & strSecond );
	bool StartTimer();
	bool StopTimer();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSpeak();
protected:
	virtual void OnCancel();

	CStudyList	m_clsList;
public:
	afx_msg void OnBnClickedPrev();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedInfo();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnNewFile();

	HACCEL m_hAccelTable;
	CDADEdit m_clsFileName;

	CComboBox m_clsExecutionMethod;
	CComboBox m_clsContentsOrder;
	CComboBox m_clsShowOrder;
	int m_iShowPeriod;
	int m_iKoreanShowTime;
	CComboBox m_clsVoice;
	BOOL m_bAuthSpeek;

	afx_msg void OnBnClickedShowSetup();
	void InitDialogSetup();
	void InitDialogLanguageSetup();
	CButton m_btnShowSetup;
	afx_msg void OnCbnSelchangeExecutionMethod2();
	afx_msg void OnCbnSelchangeContentsOrder2();
	afx_msg void OnCbnSelchangeShowOrder2();
	afx_msg void OnEnChangeRepeatPeriod2();
	afx_msg void OnEnChangeKoreanShowTime2();
	afx_msg void OnCbnSelchangeVoice2();
	afx_msg void OnBnClickedAutoSpeek2();
	int m_iTotalCount;
	int m_iCurrentIndex;
	afx_msg void OnBnClickedSelectEnglishFont();
	afx_msg void OnBnClickedSelectKoreanFont2();
	CEdit m_edtEnglish;
	CEdit m_edtKorean;
	CString m_strEnglishFont;
	CString m_strKoreanFont;
	afx_msg void OnBnClickedShowVoiceLanguageList();
	CComboBox m_clsVoiceLanguage;
	afx_msg void OnCbnSelchangeVoiceLanguage();
	CEdit m_edtRepeatPeriod;
	CEdit m_edtKoreanShowTime;
	CComboBox m_clsShowMethod;

private:
	void EnableChildWindow();

	CStudyData	m_clsStudyData;
	int					m_iStudyCount;

	CFont				m_clsEnglishFont;
	CFont				m_clsKoreaFont;

public:
	afx_msg void OnCbnSelchangeShowMethod();
};
