
/** 설정 항목을 보여주거나 가려준다. */
void CEnglishTeacherDlg::OnBnClickedShowSetup()
{
	RECT	rt;

	GetWindowRect( &rt );

	if( gclsSetup.m_bShowSetup == false )
	{
		rt.bottom += EXPEND_HEIGHT;
		gclsSetup.m_bShowSetup = true;
		m_btnShowSetup.SetWindowText( HIDE_SETUP_TEXT );
	}
	else
	{
		rt.bottom -= EXPEND_HEIGHT;
		gclsSetup.m_bShowSetup = false;
		m_btnShowSetup.SetWindowText( SHOW_SETUP_TEXT );
	}
	MoveWindow( &rt );
	gclsSetup.PutSetup();
}

/** 설정 항목을 초기화시켜준다. */
void CEnglishTeacherDlg::InitDialogSetup()
{
	if( gclsSetup.m_bShowSetup == false )
	{
		RECT	rt;

		GetWindowRect( &rt );
		rt.bottom -= EXPEND_HEIGHT;
		MoveWindow( &rt );
		m_btnShowSetup.SetWindowText( SHOW_SETUP_TEXT );
	}
	else
	{
		m_btnShowSetup.SetWindowText( HIDE_SETUP_TEXT );
	}

	AddStringToComboBox( m_clsExecutionMethod, EXECUTION_AUTOMATIC );
	AddStringToComboBox( m_clsExecutionMethod, EXECUTION_MANUAL );
	AddStringToComboBox( m_clsExecutionMethod, EXECUTION_MANUAL_RANDOM );
	if( gclsSetup.m_bAutomatic )
	{
		m_clsExecutionMethod.SetCurSel(0);
	}
	else
	{
		if( gclsSetup.m_bRandom )
		{
			m_clsExecutionMethod.SetCurSel(2);
		}
		else
		{
			m_clsExecutionMethod.SetCurSel(1);
		}
	}

	AddStringToComboBox( m_clsContentsOrder, CONTENTS_DIRECT );
	AddStringToComboBox( m_clsContentsOrder, CONTENTS_INDIRECT );
	if( gclsSetup.m_bDirect )
	{
		m_clsContentsOrder.SetCurSel(0);
	}
	else
	{
		m_clsContentsOrder.SetCurSel(1);
	}

	AddStringToComboBox( m_clsShowOrder, ENGLISH_TO_KOREAN );
	AddStringToComboBox( m_clsShowOrder, KOREAN_TO_ENGLISH );
	if( gclsSetup.m_bShowEnglishFirst )
	{
		m_clsShowOrder.SetCurSel(0);
	}
	else
	{
		m_clsShowOrder.SetCurSel(1);
	}

	CStringList clsList;
	int	iIndex = 0, iPos = 0;

	GetVoiceList( clsList );
	
	for( POSITION pos = clsList.GetHeadPosition(); pos; )
	{
		CString strVoice = clsList.GetNext(pos);

		m_clsVoice.AddString( strVoice );
		if( !_memicmp( gclsSetup.m_strVoiceSetup, strVoice, strVoice.GetLength() ) )
		{
			iPos = iIndex;
		}
		++iIndex;
	}
	m_clsVoice.SetCurSel( iPos );

	LOGFONT	sttFont;

	if( gclsSetup.GetEnglishFont( &sttFont ) )
	{
		m_strEnglishFont.Format( _T("%s"), sttFont.lfFaceName );
		m_clsEnglishFont.CreateFontIndirect( &sttFont );
		m_edtEnglish.SetFont( &m_clsEnglishFont );
	}

	if( gclsSetup.GetKoreanFont( &sttFont ) )
	{
		m_strKoreanFont.Format( _T("%s"), sttFont.lfFaceName );
		m_clsKoreaFont.CreateFontIndirect( &sttFont );
		m_edtKorean.SetFont( &m_clsKoreaFont );
	}

	InitDialogLanguageSetup();

	AddStringToComboBox( m_clsShowMethod, SHOW_BOTH );
	AddStringToComboBox( m_clsShowMethod, SHOW_FIRST_SPEAK_SECOND );
	AddStringToComboBox( m_clsShowMethod, SHOW_FIRST_SECOND );

	CString	strShowMethod, strText;
	gclsSetup.GetShowMethodString( strShowMethod );
	for( int i = 0; i < m_clsShowMethod.GetCount(); ++i )
	{
		m_clsShowMethod.GetLBText( i, strText );
		if( !strText.Compare( strShowMethod ) )
		{
			m_clsShowMethod.SetCurSel( i );
			break;
		}
	}

	UpdateData(FALSE);

	EnableChildWindow();
}

/** 언어 관련 선택 항목을 초기화시켜준다. */
void CEnglishTeacherDlg::InitDialogLanguageSetup()
{
	POSITION	pos;
	CLanguageVoice	clsLanguageVoice;
	int	i, iCount;
	CString	strText, strVoice;

	for( i = m_clsVoiceLanguage.GetCount() - 1; i >= 0; --i )
	{
		m_clsVoiceLanguage.DeleteString( i );
	}

	for( pos = gclsSetup.m_clsLanguageVoiceList.GetHeadPosition(); pos; )
	{
		clsLanguageVoice = gclsSetup.m_clsLanguageVoiceList.GetNext( pos );

		m_clsVoiceLanguage.AddString( clsLanguageVoice.m_strLanguage );
	}

	if( gclsSetup.m_strLanguageSetup.IsEmpty() == false )
	{
		iCount = m_clsVoiceLanguage.GetCount();
		for( i = 0; i < iCount; ++i )
		{
			m_clsVoiceLanguage.GetLBText( i, strText );

			if( !gclsSetup.m_strLanguageSetup.Compare( strText ) )
			{
				m_clsVoiceLanguage.SetCurSel( i );
				gclsSetup.GetLanguageVoice( strText, strVoice );
				break;
			}
		}
	}

	if( strVoice.IsEmpty() == false )
	{
		int iId = m_clsVoice.GetCurSel();
		
		m_clsVoice.GetLBText( iId, strText );

		if( strText.Compare( strVoice ) )
		{
			gclsSetup.m_strVoiceSetup = strVoice;

			iCount = m_clsVoice.GetCount();
			for( i = 0; i < iCount; ++i )
			{
				m_clsVoice.GetLBText( i, strText );
				if( !strText.Compare( strVoice ) )
				{
					m_clsVoice.SetCurSel( i );
					break;
				}
			}

			SetVoiceChange();
			gclsSetup.PutSetup();
		}
	}
}

void CEnglishTeacherDlg::OnCbnSelchangeExecutionMethod2()
{
	int iIndex = m_clsExecutionMethod.GetCurSel();
	CString strText;

	m_clsExecutionMethod.GetLBText( iIndex, strText );
	if( !StringCompare( strText, EXECUTION_AUTOMATIC ) )
	{
		gclsSetup.m_bAutomatic = true;
		StartTimer();
	}
	else if( !StringCompare( strText, EXECUTION_MANUAL_RANDOM ) )
	{
		gclsSetup.m_bAutomatic = false;
		gclsSetup.m_bRandom = true;
		StopTimer();
	}
	else
	{
		gclsSetup.m_bAutomatic = false;
		gclsSetup.m_bRandom = false;
		StopTimer();
	}

	EnableChildWindow();

	gclsSetup.PutSetup();
}

void CEnglishTeacherDlg::OnCbnSelchangeContentsOrder2()
{
	int iIndex = m_clsContentsOrder.GetCurSel();
	CString strText;

	m_clsContentsOrder.GetLBText( iIndex, strText );
	if( !StringCompare( strText, CONTENTS_DIRECT ) )
	{
		gclsSetup.m_bDirect = true;
	}
	else
	{
		gclsSetup.m_bDirect = false;
	}

	gclsSetup.PutSetup();
}

void CEnglishTeacherDlg::OnCbnSelchangeShowOrder2()
{
	int iIndex = m_clsShowOrder.GetCurSel();
	CString strText;

	m_clsShowOrder.GetLBText( iIndex, strText );
	if( !StringCompare( strText, ENGLISH_TO_KOREAN ) )
	{
		gclsSetup.m_bShowEnglishFirst = true;
	}
	else
	{
		gclsSetup.m_bShowEnglishFirst = false;
	}

	gclsSetup.PutSetup();
}

void CEnglishTeacherDlg::OnEnChangeRepeatPeriod2()
{
	UpdateData(TRUE);

	gclsSetup.m_iRepeatPeriod = m_iShowPeriod;
	gclsSetup.PutSetup();
}

void CEnglishTeacherDlg::OnEnChangeKoreanShowTime2()
{
	UpdateData(TRUE);

	gclsSetup.m_iKoreanShowTime = m_iKoreanShowTime;
	gclsSetup.PutSetup();
}

void CEnglishTeacherDlg::OnCbnSelchangeVoice2()
{
	int iId = m_clsVoice.GetCurSel();
	if( iId != -1 )
	{
		CString strText;

		m_clsVoice.GetLBText( iId, strText );
		gclsSetup.m_strVoiceSetup = strText;

		SetVoiceChange();
		gclsSetup.PutSetup();	
	}
}

void CEnglishTeacherDlg::OnCbnSelchangeVoiceLanguage()
{
	int iId = m_clsVoiceLanguage.GetCurSel();
	if( iId != -1 )
	{
		CString strText, strVoice;

		m_clsVoiceLanguage.GetLBText( iId, strText );
		gclsSetup.m_strLanguageSetup = strText;

		gclsSetup.GetLanguageVoice( gclsSetup.m_strLanguageSetup, strVoice );
		gclsSetup.m_strVoiceSetup = strVoice;

		int i, iCount = m_clsVoice.GetCount();
		for( i = 0; i < iCount; ++i )
		{
			m_clsVoice.GetLBText( i, strText );

			if( !strText.Compare( gclsSetup.m_strVoiceSetup ) )
			{
				m_clsVoice.SetCurSel( i );
				break;
			}
		}

		SetVoiceChange();
		gclsSetup.PutSetup();	
	}
}

void CEnglishTeacherDlg::OnBnClickedAutoSpeek2()
{
	UpdateData(TRUE);

	if( m_bAuthSpeek )
	{
		gclsSetup.m_bAutoSpeek = true;
	}
	else
	{
		gclsSetup.m_bAutoSpeek = false;
	}

	gclsSetup.PutSetup();
}

void CEnglishTeacherDlg::OnBnClickedSelectEnglishFont()
{
	LOGFONT	sttLogFont;

	if( gclsSetup.GetEnglishFont( &sttLogFont ) == false )
	{
		CFont * pclsFont = m_edtEnglish.GetFont();
		if( pclsFont )
		{
			pclsFont->GetLogFont( &sttLogFont );
		}
	}

	CFontDialog clsDlg( &sttLogFont );
	if( clsDlg.DoModal( ) == IDOK )
	{
		clsDlg.GetCurrentFont( &sttLogFont );

		m_clsEnglishFont.DeleteObject();
		m_clsEnglishFont.CreateFontIndirect( &sttLogFont );
		m_edtEnglish.SetFont( &m_clsEnglishFont );

		gclsSetup.SetEnglishFont( &sttLogFont );
		gclsSetup.PutSetup();

		m_strEnglishFont.Format( _T("%s"), sttLogFont.lfFaceName );
		UpdateData(FALSE);
	}
}

void CEnglishTeacherDlg::OnBnClickedSelectKoreanFont2()
{
	LOGFONT	sttLogFont;

	if( gclsSetup.GetKoreanFont( &sttLogFont ) == false )
	{
		CFont * pclsFont = m_edtKorean.GetFont();
		if( pclsFont )
		{
			pclsFont->GetLogFont( &sttLogFont );
		}
	}

	CFontDialog clsDlg( &sttLogFont );
	if( clsDlg.DoModal( ) == IDOK )
	{
		clsDlg.GetCurrentFont( &sttLogFont );

		m_clsKoreaFont.DeleteObject();
		m_clsKoreaFont.CreateFontIndirect( &sttLogFont );
		m_edtKorean.SetFont( &m_clsKoreaFont );

		gclsSetup.SetKoreanFont( &sttLogFont );
		gclsSetup.PutSetup();

		m_strKoreanFont.Format( _T("%s"), sttLogFont.lfFaceName );
		UpdateData(FALSE);
	}
}

void CEnglishTeacherDlg::OnBnClickedShowVoiceLanguageList()
{
	CVoiceLanguageListDlg	clsDlg;

	if( clsDlg.DoModal() == IDOK )
	{
		InitDialogLanguageSetup();
	}
}

void CEnglishTeacherDlg::EnableChildWindow()
{
	if( gclsSetup.m_bAutomatic )
	{
		m_edtRepeatPeriod.EnableWindow( TRUE );
		m_edtKoreanShowTime.EnableWindow( TRUE );
		m_clsShowMethod.EnableWindow( FALSE );
	}
	else
	{
		m_edtRepeatPeriod.EnableWindow( FALSE );
		m_edtKoreanShowTime.EnableWindow( FALSE );
		m_clsShowMethod.EnableWindow( TRUE );
	}
}

void CEnglishTeacherDlg::OnCbnSelchangeShowMethod()
{
	int iId = m_clsShowMethod.GetCurSel();
	if( iId != -1 )
	{
		CString strText;

		m_clsShowMethod.GetLBText( iId, strText );
		gclsSetup.SetShowMethodString( strText );
		gclsSetup.PutSetup();	
	}

}
