/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include "stdafx.h"
#include "EnglishTeacher.h"
#include "EnglishTeacherDlg.h"
#include "VoiceLanguageListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strProgramVersion;
	CString m_strAuthor;
	afx_msg void OnStnClickedAuthor();
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strProgramVersion(_T(""))
, m_strAuthor(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PROGRAM_VERSION, m_strProgramVersion);
	DDX_Text(pDX, IDC_AUTHOR, m_strAuthor);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_STN_CLICKED(IDC_AUTHOR, &CAboutDlg::OnStnClickedAuthor)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	m_strProgramVersion.Format( _T("SipPhone Version %s"), PROGRAM_VERSION );
	m_strAuthor.Format( _T("by yee young han\r\n")
		_T(" - websearch@naver.com\r\n")
		_T(" - http://blog.naver.com/websearch\r\n\r\n")
		_T("build %s %s"), __DATE__, __TIME__ );

	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAboutDlg::OnStnClickedAuthor()
{
	ShellExecute( m_hWnd, _T("open"), _T("iexplore"), _T("http://blog.naver.com/websearch"), NULL, SW_SHOW );
}

// CEnglishTeacherDlg dialog

CEnglishTeacherDlg::CEnglishTeacherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnglishTeacherDlg::IDD, pParent)
	, m_strKorean(_T(""))
	, m_strEnglish(_T(""))
	, m_strFileName(_T(""))
	, m_iShowPeriod(0)
	, m_iKoreanShowTime(0)
	, m_bAuthSpeek(FALSE)
	, m_iTotalCount(0)
	, m_iCurrentIndex(0)
	, m_strEnglishFont(_T(""))
	, m_strKoreanFont(_T(""))
{
	m_hAccelTable = NULL;
	m_iStudyCount = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEnglishTeacherDlg::~CEnglishTeacherDlg()
{
}

void CEnglishTeacherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_KOREAN, m_strKorean);
	DDX_Text(pDX, IDC_ENGLISH, m_strEnglish);
	DDX_Text(pDX, IDC_FILENAME, m_strFileName);
	DDX_Control(pDX, IDC_FILENAME, m_clsFileName);
	DDX_Control(pDX, IDC_EXECUTION_METHOD2, m_clsExecutionMethod);
	DDX_Control(pDX, IDC_CONTENTS_ORDER2, m_clsContentsOrder);
	DDX_Control(pDX, IDC_SHOW_ORDER2, m_clsShowOrder);
	DDX_Text(pDX, IDC_REPEAT_PERIOD2, m_iShowPeriod);
	DDX_Text(pDX, IDC_KOREAN_SHOW_TIME2, m_iKoreanShowTime);
	DDX_Control(pDX, IDC_VOICE2, m_clsVoice);
	DDX_Check(pDX, IDC_AUTO_SPEEK2, m_bAuthSpeek);
	DDX_Control(pDX, IDC_SHOW_SETUP, m_btnShowSetup);
	DDX_Text(pDX, IDC_TOTAL_COUNT, m_iTotalCount);
	DDX_Text(pDX, IDC_CURRENT_INDEX, m_iCurrentIndex);
	DDX_Control(pDX, IDC_ENGLISH, m_edtEnglish);
	DDX_Control(pDX, IDC_KOREAN, m_edtKorean);
	DDX_Text(pDX, IDC_ENGLISH_FONT, m_strEnglishFont);
	DDX_Text(pDX, IDC_KOREAN_FONT, m_strKoreanFont);
	DDX_Control(pDX, IDC_VOICE_LANGUAGE, m_clsVoiceLanguage);
	DDX_Control(pDX, IDC_REPEAT_PERIOD2, m_edtRepeatPeriod);
	DDX_Control(pDX, IDC_KOREAN_SHOW_TIME2, m_edtKoreanShowTime);
	DDX_Control(pDX, IDC_SHOW_METHOD, m_clsShowMethod);
}

BEGIN_MESSAGE_MAP(CEnglishTeacherDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SELECT_FILE, &CEnglishTeacherDlg::OnBnClickedSelectFile)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SPEAK, &CEnglishTeacherDlg::OnBnClickedSpeak)
	ON_BN_CLICKED(IDC_PREV, &CEnglishTeacherDlg::OnBnClickedPrev)
	ON_BN_CLICKED(IDC_NEXT, &CEnglishTeacherDlg::OnBnClickedNext)
	ON_BN_CLICKED(IDC_INFO, &CEnglishTeacherDlg::OnBnClickedInfo)
	ON_BN_CLICKED(IDC_SHOW_SETUP, &CEnglishTeacherDlg::OnBnClickedShowSetup)
	ON_CBN_SELCHANGE(IDC_EXECUTION_METHOD2, &CEnglishTeacherDlg::OnCbnSelchangeExecutionMethod2)
	ON_CBN_SELCHANGE(IDC_CONTENTS_ORDER2, &CEnglishTeacherDlg::OnCbnSelchangeContentsOrder2)
	ON_CBN_SELCHANGE(IDC_SHOW_ORDER2, &CEnglishTeacherDlg::OnCbnSelchangeShowOrder2)
	ON_EN_CHANGE(IDC_REPEAT_PERIOD2, &CEnglishTeacherDlg::OnEnChangeRepeatPeriod2)
	ON_EN_CHANGE(IDC_KOREAN_SHOW_TIME2, &CEnglishTeacherDlg::OnEnChangeKoreanShowTime2)
	ON_CBN_SELCHANGE(IDC_VOICE2, &CEnglishTeacherDlg::OnCbnSelchangeVoice2)
	ON_BN_CLICKED(IDC_AUTO_SPEEK2, &CEnglishTeacherDlg::OnBnClickedAutoSpeek2)
	ON_BN_CLICKED(IDC_SELECT_ENGLISH_FONT, &CEnglishTeacherDlg::OnBnClickedSelectEnglishFont)
	ON_BN_CLICKED(IDC_SELECT_KOREAN_FONT2, &CEnglishTeacherDlg::OnBnClickedSelectKoreanFont2)
	ON_BN_CLICKED(IDC_SHOW_VOICE_LANGUAGE_LIST, &CEnglishTeacherDlg::OnBnClickedShowVoiceLanguageList)
	ON_CBN_SELCHANGE(IDC_VOICE_LANGUAGE, &CEnglishTeacherDlg::OnCbnSelchangeVoiceLanguage)
	ON_CBN_SELCHANGE(IDC_SHOW_METHOD, &CEnglishTeacherDlg::OnCbnSelchangeShowMethod)
END_MESSAGE_MAP()


// CEnglishTeacherDlg message handlers

BOOL CEnglishTeacherDlg::OnInitDialog()
{
	bool bGetSetup = gclsSetup.GetSetup();

	m_iShowPeriod = gclsSetup.m_iRepeatPeriod;
	m_iKoreanShowTime = gclsSetup.m_iKoreanShowTime;
	m_bAuthSpeek = gclsSetup.m_bAutoSpeek ? TRUE : FALSE;

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//VoiceSetup();
	StartSpeekThread();
	InitDialogSetup();

	if( bGetSetup )
	{
		if( gclsSetup.m_strDataFileName.IsEmpty() == false )
		{
			m_strFileName = gclsSetup.m_strDataFileName;
			UpdateData(FALSE);
			StartShowText();
		}
	}

	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1)); 

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEnglishTeacherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEnglishTeacherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEnglishTeacherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEnglishTeacherDlg::OnBnClickedSelectFile()
{
	CFileDialog	clsDlg( TRUE );

	if( clsDlg.DoModal() != IDOK ) return;

	m_strFileName = clsDlg.GetPathName();
	OnNewFile();
}

/** 파일에서 데이터를 읽어서 학습을 시작한다. */
bool CEnglishTeacherDlg::StartShowText()
{
	if( m_clsList.GetSize() > 0 )	StopTimer();

	if( m_clsList.ParseFile( m_strFileName ) == false )
	{
		CString	strMsg;

		strMsg.Format( _T("file(%s) open error(%d)"), m_strFileName, GetLastError() );
		MessageBox( strMsg, _T("Error"), MB_OK | MB_ICONERROR );
		return false;
	}

	if( m_clsList.GetSize() > 0 )
	{
		gclsSetup.m_strDataFileName = m_strFileName;
		gclsSetup.PutSetup();

		if( gclsSetup.m_iLastIndex > 0 )
		{
			CStudyData	clsData;

			for( int i = 0; i < gclsSetup.m_iLastIndex; ++i )
			{
				m_clsList.GetNext( clsData, gclsSetup.m_bRandom );
			}
		}

		m_iTotalCount = m_clsList.GetSize();
		OnBnClickedNext();

		if( gclsSetup.m_bAutomatic ) StartTimer();
	}

	return true;
}

/** 국문/영문을 보여준다. */
bool CEnglishTeacherDlg::ShowData( CString & strFirst, CString & strSecond )
{
	if( gclsSetup.m_bAutomatic )
	{
		if( gclsSetup.m_bShowEnglishFirst )
		{
			if( gclsSetup.m_bDirect )
			{
				m_strEnglish = strFirst;
				if( gclsSetup.m_iKoreanShowTime == 0 )
				{
					m_strKorean = strSecond;
				}
				else
				{
					m_strKorean = "";
					m_strCurrentKorean = strSecond;
				}
			}
			else
			{
				m_strEnglish = strSecond;
				if( gclsSetup.m_iKoreanShowTime == 0 )
				{
					m_strKorean = strFirst;
				}
				else
				{
					m_strKorean = "";
					m_strCurrentKorean = strFirst;
				}
			}
		}
		else
		{
			if( gclsSetup.m_bDirect )
			{
				m_strKorean = strSecond;
				if( gclsSetup.m_iKoreanShowTime == 0 )
				{
					m_strEnglish = strFirst;
				}
				else
				{
					m_strEnglish = "";
					m_strCurrentEnglish = strFirst;
				}
			}
			else
			{
				m_strKorean = strFirst;
				if( gclsSetup.m_iKoreanShowTime == 0 )
				{
					m_strEnglish = strSecond;
				}
				else
				{
					m_strEnglish = "";
					m_strCurrentEnglish = strSecond;
				}
			}
		}

		m_iCurrentIndex = m_clsList.GetIndex() + 1;
		UpdateData( FALSE );

		if( gclsSetup.m_bAutoSpeek && ( gclsSetup.m_bShowEnglishFirst || gclsSetup.m_iKoreanShowTime == 0 ) )
		{
			AddSpeekText( m_strEnglish );
		}

		if( gclsSetup.m_iKoreanShowTime > 0 )
		{
			SetTimer( SHOW_KOREAN_TIMER, gclsSetup.m_iKoreanShowTime * 1000, NULL );
		}
	}
	else
	{
		if( gclsSetup.m_bDirect )
		{
			m_strEnglish = strFirst;
			m_strKorean = strSecond;
		}
		else
		{
			m_strEnglish = strSecond;
			m_strKorean = strFirst;
		}
		m_iCurrentIndex = m_clsList.GetIndex() + 1;
		UpdateData( FALSE );

		if( gclsSetup.m_bAutoSpeek )
		{
			AddSpeekText( m_strEnglish );
		}
	}

	return true;
}

bool CEnglishTeacherDlg::ShowData()
{
	switch( m_iStudyCount )
	{
	case 0:
		if( gclsSetup.m_bShowEnglishFirst )
		{
			m_strEnglish = m_clsStudyData.m_strFirst;
			m_strKorean = "";
		}
		else
		{
			m_strEnglish = "";
			m_strKorean = m_clsStudyData.m_strSecond;
		}
		m_iCurrentIndex = m_clsList.GetIndex() + 1;
		UpdateData( FALSE );

		if( gclsSetup.m_eShowMethod == E_SHOW_FIRST_SECOND )
		{
			++m_iStudyCount;
		}
		break;
	case 1:
		AddSpeekText( m_clsStudyData.m_strFirst );
		break;
	case 2:
		m_strEnglish = m_clsStudyData.m_strFirst;
		m_strKorean = m_clsStudyData.m_strSecond;
		UpdateData( FALSE );
		break;
	}

	++m_iStudyCount;
	if( m_iStudyCount == 3 ) m_iStudyCount = 0;

	return true;
}

/** 자동 학습 타이머를 시작한다. */
bool CEnglishTeacherDlg::StartTimer()
{
	if( m_clsList.GetSize() > 0 )
	{
		KillTimer( SHOW_TIMER );
		SetTimer( SHOW_TIMER, gclsSetup.m_iRepeatPeriod * 1000, NULL );
	}

	return true;
}

/** 자동 학습 타이머를 종료한다. */
bool CEnglishTeacherDlg::StopTimer()
{
	KillTimer( SHOW_TIMER );

	return true;
}

void CEnglishTeacherDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case SHOW_TIMER:
		OnBnClickedNext();
		break;
	case SHOW_KOREAN_TIMER:
		KillTimer( SHOW_KOREAN_TIMER );

		if( gclsSetup.m_bShowEnglishFirst )
		{
			m_strKorean = m_strCurrentKorean;
		}
		else
		{
			m_strEnglish = m_strCurrentEnglish;
			if( gclsSetup.m_bAutoSpeek )
			{
				AddSpeekText( m_strEnglish );
			}
		}
		
		UpdateData( FALSE );
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

/** 영어를 TTS 엔진에 입력하여서 음성 출력한다. */
void CEnglishTeacherDlg::OnBnClickedSpeak()
{
	UpdateData( TRUE );

	AddSpeekText( m_strEnglish );
}

/** Speek 쓰레드를 종료한 후, 대화상자를 닫는다. */
void CEnglishTeacherDlg::OnCancel()
{
	SetStopSpeekThread();

	gclsSetup.m_iLastIndex = m_clsList.GetIndex();
	gclsSetup.PutSetup();

	while( 1 )
	{
		if( IsStopSpeekThread() ) break;
		Sleep(50);
	}

	CDialog::OnCancel();
}

/** 이전 학습 자료를 보여준다. */
void CEnglishTeacherDlg::OnBnClickedPrev()
{
	if( m_clsList.GetSize() == 0 ) return;
	if( IsSpeeking() ) return;

	if( gclsSetup.m_bAutomatic == false && gclsSetup.m_eShowMethod != E_SHOW_BOTH )
	{
		if( m_iStudyCount == 0 )
		{
			CStudyData	clsData;

			if( m_clsList.GetPrev( clsData, gclsSetup.m_bRandom ) )
			{
				if( gclsSetup.m_bDirect )
				{
					m_clsStudyData.m_strFirst = clsData.m_strFirst;
					m_clsStudyData.m_strSecond = clsData.m_strSecond;
				}
				else
				{
					m_clsStudyData.m_strSecond = clsData.m_strFirst;
					m_clsStudyData.m_strFirst = clsData.m_strSecond;
				}
			}
		}

		ShowData();
	}
	else
	{
		CStudyData	clsData;

		if( m_clsList.GetPrev( clsData, gclsSetup.m_bRandom ) )
		{
			ShowData( clsData.m_strFirst, clsData.m_strSecond );
		}
	}
}

/** 다음 학습 자료를 보여준다. */
void CEnglishTeacherDlg::OnBnClickedNext()
{
	if( m_clsList.GetSize() == 0 ) return;
	if( IsSpeeking() ) return;

	if( gclsSetup.m_bAutomatic == false && gclsSetup.m_eShowMethod != E_SHOW_BOTH )
	{
		if( m_iStudyCount == 0 )
		{
			CStudyData	clsData;

			if( m_clsList.GetNext( clsData, gclsSetup.m_bRandom ) )
			{
				if( gclsSetup.m_bDirect )
				{
					m_clsStudyData.m_strFirst = clsData.m_strFirst;
					m_clsStudyData.m_strSecond = clsData.m_strSecond;
				}
				else
				{
					m_clsStudyData.m_strSecond = clsData.m_strFirst;
					m_clsStudyData.m_strFirst = clsData.m_strSecond;
				}
			}
		}

		ShowData();
	}
	else
	{
		CStudyData	clsData;

		if( m_clsList.GetNext( clsData, gclsSetup.m_bRandom ) )
		{
			ShowData( clsData.m_strFirst, clsData.m_strSecond );
		}
	}
}

void CEnglishTeacherDlg::OnBnClickedInfo()
{
	CAboutDlg	clsDlg;

	clsDlg.DoModal();
}

/**
 * @brief 
 * @param pMsg 
 * @returns 
 */
BOOL CEnglishTeacherDlg::PreTranslateMessage( MSG* pMsg )
{
   if( m_hAccelTable != NULL ) 
  { 
     if( TranslateAccelerator( m_hWnd, m_hAccelTable, pMsg ) ) 
        return TRUE; 
  } 

  return __super::PreTranslateMessage(pMsg);
}

void CEnglishTeacherDlg::OnNewFile()
{
	gclsSetup.m_iLastIndex = 0;
	UpdateData(FALSE);
	StartShowText();
}

#include "EnglishTeacherDlg.hpp"
