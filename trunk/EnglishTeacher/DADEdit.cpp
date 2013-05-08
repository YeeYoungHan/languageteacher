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
