#include "StdAfx.h"
#include "StudyList.h"
#include "StdioFileEx.h"

CStudyList::CStudyList(void)
{
	m_pos = NULL;
	m_iIndex = -1;
}

CStudyList::~CStudyList(void)
{
}

/** '\n' 으로 종료되는 문자열을 '\r\n' 로 종료되도록 수정한다. */
static void LineEnd( char * szBuf, int iLen )
{
	if( (iLen > 1 && szBuf[iLen-2] != '\r') || iLen == 1 )
	{
		szBuf[iLen-1] = '\r';
		szBuf[iLen] = '\n';
		szBuf[iLen+1] = '\0';
	}
}

// QQQ: Unicode 파일을 읽을 수 있어야 한다.
bool CStudyList::ParseFile( const TCHAR * pszFileName )
{
	CStdioFileEx	clsFile;
	CString				strText, strFirst, strSecond;
	CStudyData	clsData;

	m_clsList.RemoveAll();
	m_pos = NULL;

	if( !clsFile.Open( pszFileName, CFile::modeRead | CFile::typeText ) )
	{
		return false;
	}

	while( 1 )
	{
		strFirst = "";
		strSecond = "";

		while( 1 )
		{
			if( clsFile.ReadString( strText ) == FALSE ) goto FUNC_END;
			if( strText.GetLength() > 0 && strText.GetAt(0) != _T(';') ) break;
		}

		strFirst = strText;

		while( 1 )
		{
			if( clsFile.ReadString( strText ) == FALSE ) goto FUNC_END;

			if( strText.GetLength() == 0 ) break;

			strSecond.Append( strText );
			strSecond.Append( _T("\r\n") );
		}

		clsData.m_strFirst = strFirst;
		clsData.m_strSecond = strSecond;

		m_clsList.AddTail( clsData );
	}

FUNC_END:
	if( strFirst.GetLength() > 0 )
	{
		clsData.m_strFirst = strFirst;
		clsData.m_strSecond = strSecond;

		m_clsList.AddTail( clsData );
	}

	clsFile.Close();

	return true;
}

/*
bool CStudyList::ParseFile( const TCHAR * pszFileName )
{
	FILE	* psttFd;
	char	szBuf[8192];
	int		iLen;
	CString	strFirst, strSecond;
	CStudyData	clsData;

	m_clsList.RemoveAll();
	m_pos = NULL;

	USES_CONVERSION;

	psttFd = fopen( T2A(pszFileName), "r" );
	if( psttFd == NULL ) return false;

	// QQQ: Unicode 파일인지 검사한다.

	while( 1 )
	{
		strFirst = "";
		strSecond = "";

		while( 1 )
		{
			memset( szBuf, 0, sizeof(szBuf) );
			if( fgets( szBuf, sizeof(szBuf)-1, psttFd ) == NULL )	goto FUNC_END;
			iLen = (int)strlen( szBuf );
			if( iLen > 0 && szBuf[0] != ';' && szBuf[0] != '\r' && szBuf[0] != '\n' ) break;
		}

		LineEnd( szBuf, iLen );
		strFirst = szBuf;

		while( 1 )
		{
			memset( szBuf, 0, sizeof(szBuf) );
			if( fgets( szBuf, sizeof(szBuf)-1, psttFd ) == NULL )	goto FUNC_END;
			iLen = (int)strlen(szBuf);

			if( iLen > 0 && ( szBuf[0] == '\n' || szBuf[0] == '\r' ) ) break;
			if( iLen == 0 ) break;

			LineEnd( szBuf, iLen );
			strSecond.Append( A2T(szBuf) );
		}

		clsData.m_strFirst = strFirst;
		clsData.m_strSecond = strSecond;

		m_clsList.AddTail( clsData );
	}

FUNC_END:
	if( strFirst.GetLength() > 0 )
	{
		clsData.m_strFirst = strFirst;
		clsData.m_strSecond = strSecond;

		m_clsList.AddTail( clsData );
	}

	fclose( psttFd );
	
	return true;
}
*/

bool CStudyList::GetNext( CStudyData & clsData, bool bRandom )
{
	if( bRandom )
	{
		return GetRandom( clsData );
	}
	else
	{
		if( m_pos == NULL )
		{
NEXT_START:
			m_pos = m_clsList.GetHeadPosition();
			m_iIndex = -1;
			m_bNext = true;
		}

		if( m_bNext )
		{
			clsData = m_clsList.GetNext( m_pos );
		}
		else
		{
			// prev 위치에서 next 위치로 변경하기 위해서는 3번의 next 가 필요하다.
			// ==> 왜 그럴까??
			m_bNext = true;
			m_clsList.GetNext( m_pos );
			if( m_pos == NULL ) goto NEXT_START;
			m_clsList.GetNext( m_pos );
			if( m_pos == NULL ) goto NEXT_START;
			clsData = m_clsList.GetNext( m_pos );
		}
		++m_iIndex;
	}

	return true;
}

bool CStudyList::GetPrev( CStudyData & clsData, bool bRandom )
{
	if( bRandom )
	{
		return GetRandom( clsData );
	}
	else
	{
		if( m_pos == NULL )
		{
PREV_START:
			m_pos = m_clsList.GetTailPosition();
			m_iIndex = (int)m_clsList.GetCount();
			m_bNext = false;
		}

		if( !m_bNext )
		{
			clsData = m_clsList.GetPrev( m_pos );
		}
		else
		{
			// next 위치에서 prev 위치로 변경하기 위해서는 3번의 prev 가 필요하다.
			// ==> 왜 그럴까?
			m_bNext = false;
			m_clsList.GetPrev( m_pos );
			if( m_pos == NULL ) goto PREV_START;
			m_clsList.GetPrev( m_pos );
			if( m_pos == NULL ) goto PREV_START;
			clsData = m_clsList.GetPrev( m_pos );
		}
		--m_iIndex;
	}

	return true;
}

bool CStudyList::GetRandom( CStudyData & clsData )
{
	int iIndex = m_iIndex, iCount;

	iCount = (int)m_clsList.GetCount();
	if( iCount == 0 ) return false;

	while( iIndex == m_iIndex )
	{
		iIndex = rand() % iCount;
	}

	m_iIndex = iIndex;
	m_pos = m_clsList.FindIndex( m_iIndex );

	clsData = m_clsList.GetAt( m_pos );

	return true;
}

int CStudyList::GetSize()
{
	return (int)m_clsList.GetCount();
}

int CStudyList::GetIndex()
{
	if( m_iIndex < 0 ) return 0;

	return m_iIndex;
}
