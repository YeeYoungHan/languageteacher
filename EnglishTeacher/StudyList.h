#pragma once

class CStudyData
{
public:
	CString	m_strFirst;
	CString m_strSecond;
};

class CStudyList
{
public:
	CStudyList(void);
	~CStudyList(void);
	bool ParseFile( const TCHAR * pszFileName );
	bool GetNext( CStudyData & clsData, bool bRandom );
	bool GetPrev( CStudyData & clsData, bool bRandom );
	bool GetRandom( CStudyData & clsData );
	int GetSize();
	int GetIndex();

private:
	CList< CStudyData, CStudyData& > m_clsList;
	POSITION	m_pos;
	int				m_iIndex;
	bool			m_bNext;
};
