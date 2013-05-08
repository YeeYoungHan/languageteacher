#pragma once


// CDADEdit

class CDADEdit : public CEdit
{
	DECLARE_DYNAMIC(CDADEdit)

public:
	CDADEdit();
	virtual ~CDADEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	void SetFilePath( const TCHAR * pszFilePath );

	const TCHAR * GetFilePath();

private:
	CString	m_strFilePath;
};


