// VoiceLanguageListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EnglishTeacher.h"
#include "VoiceLanguageListDlg.h"


// CVoiceLanguageListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVoiceLanguageListDlg, CDialog)

CVoiceLanguageListDlg::CVoiceLanguageListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVoiceLanguageListDlg::IDD, pParent)
	, m_strLanguage(_T(""))
{

}

CVoiceLanguageListDlg::~CVoiceLanguageListDlg()
{
}

void CVoiceLanguageListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANGUAGE_LIST, m_clsLanguageList);
	DDX_Text(pDX, IDC_LANGUAGE, m_strLanguage);
	DDX_Control(pDX, IDC_TTS, m_clsTts);
}


BEGIN_MESSAGE_MAP(CVoiceLanguageListDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD, &CVoiceLanguageListDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &CVoiceLanguageListDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDOK, &CVoiceLanguageListDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &CVoiceLanguageListDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CVoiceLanguageListDlg 메시지 처리기입니다.

BOOL CVoiceLanguageListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_clsLanguageList.InsertColumn( 0, _T("Language"), 0, 150 );
	m_clsLanguageList.InsertColumn( 1, _T("Voice"), 0, 180 );

	CStringList	clsTtsList;
	CString			strTts;
	POSITION		pos;
	CLanguageVoice	clsLanguageVoice;
	int		iIndex = 0;
	
	for( pos = gclsSetup.m_clsLanguageVoiceList.GetHeadPosition(); pos; )
	{
		clsLanguageVoice = gclsSetup.m_clsLanguageVoiceList.GetNext( pos );

		m_clsLanguageList.InsertItem( iIndex, clsLanguageVoice.m_strLanguage );
		m_clsLanguageList.SetItemText( iIndex, 1, clsLanguageVoice.m_strVoice );
		++iIndex;
	}

	GetVoiceList( clsTtsList );

	for( pos = clsTtsList.GetHeadPosition(); pos; )
	{
		strTts = clsTtsList.GetNext( pos );
		m_clsTts.AddString( strTts );
	}
	m_clsTts.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CVoiceLanguageListDlg::OnBnClickedAdd()
{
	UpdateData(TRUE);

	if( m_strLanguage.IsEmpty() )
	{
		MessageBox( _T("Please insert language name"), _T("Input Data Not found"), MB_OK | MB_ICONINFORMATION );
		return;
	}

	int iIndex = m_clsTts.GetCurSel();
	CString strTts;
	m_clsTts.GetLBText( iIndex, strTts );

	iIndex = m_clsLanguageList.GetItemCount();
	m_clsLanguageList.InsertItem( iIndex, m_strLanguage );
	m_clsLanguageList.SetItemText( iIndex, 1, strTts );

	m_strLanguage = "";
	UpdateData( FALSE );
}

void CVoiceLanguageListDlg::OnBnClickedDelete()
{
	POSITION pos = m_clsLanguageList.GetFirstSelectedItemPosition();
	if( pos == NULL )
	{
		MessageBox( _T("Please select language to delete"), _T("Information"), MB_OK | MB_ICONINFORMATION );
		return;
	}

	int iIndex = m_clsLanguageList.GetNextSelectedItem( pos );
	m_clsLanguageList.DeleteItem( iIndex );
}

void CVoiceLanguageListDlg::OnBnClickedOk()
{
	gclsSetup.m_clsLanguageVoiceList.RemoveAll();

	int	i, iCount = m_clsLanguageList.GetItemCount();
	
	for( i = 0; i < iCount; ++i )
	{
		CLanguageVoice	clsLanguageVoice;

		clsLanguageVoice.m_strLanguage = m_clsLanguageList.GetItemText( i, 0 );
		clsLanguageVoice.m_strVoice = m_clsLanguageList.GetItemText( i, 1 );

		gclsSetup.m_clsLanguageVoiceList.AddTail( clsLanguageVoice );
	}

	OnOK();
}

void CVoiceLanguageListDlg::OnBnClickedCancel()
{
	OnCancel();
}
