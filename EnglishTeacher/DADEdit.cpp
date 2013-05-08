// DADEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DADEdit.h"
#include "resource.h"
#include "EnglishTeacherDlg.h"
// CDADEdit

IMPLEMENT_DYNAMIC(CDADEdit, CEdit)

CDADEdit::CDADEdit()
{

}

CDADEdit::~CDADEdit()
{
}


BEGIN_MESSAGE_MAP(CDADEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

// CDADEdit 메시지 처리기입니다.

int CDADEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->dwExStyle |= WS_EX_ACCEPTFILES;

	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDADEdit::OnDropFiles(HDROP hDropInfo)
{
	TCHAR   szFilePath[MAX_PATH]; 
	UINT    cFiles;

	cFiles = ::DragQueryFile( hDropInfo, (UINT)-1, NULL, 0);   

	if( cFiles >= 1 )
	{        
		::DragQueryFile( hDropInfo, 0, szFilePath, sizeof(szFilePath));  

		m_strFilePath = szFilePath;
		SetWindowText( m_strFilePath );

		CEnglishTeacherDlg * pclsDlg = (CEnglishTeacherDlg *)GetParent();
		if( pclsDlg )
		{
			pclsDlg->m_strFileName = m_strFilePath;
			pclsDlg->OnNewFile();
		}
	}      
}

void CDADEdit::SetFilePath( const TCHAR * pszFilePath )
{
	m_strFilePath = pszFilePath;
}

const TCHAR * CDADEdit::GetFilePath()
{
	return m_strFilePath;
}
