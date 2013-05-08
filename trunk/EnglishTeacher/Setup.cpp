#include "StdAfx.h"
#include "Setup.h"
#include <string>

CSetup	gclsSetup;

CSetup::CSetup(void) : m_iRepeatPeriod(10)
	, m_bAutoSpeek(false)
	, m_bAutomatic(true)
	, m_bRandom(false)
	, m_bDirect(true)
	, m_iKoreanShowTime(0)
	, m_iLastIndex(0)
	, m_bShowEnglishFirst(true)
	, m_bShowSetup(false)
	, m_eShowMethod(E_SHOW_BOTH)
{
	m_strFileName.Format( _T("%s\\%s"), HGetProgramDirectory(), SETUP_FILENAME );
}

CSetup::~CSetup(void)
{
}

bool CSetup::GetSetup()
{
	FILE	* fd;
	char	szBuf[1024*8], szKey[1024], szValue[1024*8];
	int		i, iValuePos;
	CString	strValue;

	USES_CONVERSION;

	fd = fopen( T2A(m_strFileName), "r" );
	if( fd == NULL ) return false;

	memset( szBuf, 0, sizeof(szBuf) );
	while( fgets( szBuf, sizeof(szBuf)-1, fd ) )
	{
		iValuePos = 0;
		memset( szKey, 0, sizeof(szKey) );
		memset( szValue, 0, sizeof(szValue) );

		for( i = 0; szBuf[i]; ++i )
		{
			if( szBuf[i] == '\n' || szBuf[i] == '\r' )
			{
				if( strlen(szKey) > 0 )
				{
					memcpy( szValue, szBuf + iValuePos, i - iValuePos );
				}
				break;
			}

			if( szBuf[i] == '=' )
			{
				memcpy( szKey, szBuf, i );
				iValuePos = i + 1;
			}
		}

		if( strlen(szKey) > 0 && strlen( szValue ) == 0 )
		{
			memcpy( szValue, szBuf + iValuePos, i - iValuePos );
		}

		if( strlen(szKey) > 0 )
		{
			HTrim( szKey );
			HTrim( szValue );

			if( !strcmp( szKey, REPEAT_PERIOD_KEY ) )
			{
				m_iRepeatPeriod = atoi(szValue);
			}
			else if( !strcmp( szKey, KOREAN_SHOW_TIME ) )
			{
				m_iKoreanShowTime = atoi(szValue);
			}
			else if( !strcmp( szKey, LAST_INDEX ) )
			{
				m_iLastIndex = atoi(szValue);
			}
			else if( !strcmp( szKey, AUTO_SPEEK_KEY ) )
			{
				if( !strcmp( szValue, "true" ) )
				{
					m_bAutoSpeek = true;
				}
				else
				{
					m_bAutoSpeek = false;
				}
			}
			else if( !strcmp( szKey, DATA_FILENAME ) )
			{
				m_strDataFileName = szValue;
			}
			else if( !strcmp( szKey, EXECUTION_METHOD ) )
			{
				if( !strcmp( szValue, EXECUTION_AUTOMATIC ) )
				{
					m_bAutomatic = true;
				}
				else
				{
					m_bAutomatic = false;
				}
			}
			else if( !strcmp( szKey, EXECUTION_RANDOM ) )
			{
				if( !strcmp( szValue, "true" ) )
				{
					m_bRandom = true;
				}
				else
				{
					m_bRandom = false;
				}
			}
			else if( !strcmp( szKey, CONTENTS_ORDER ) )
			{
				if( !strcmp( szValue, CONTENTS_DIRECT ) )
				{
					m_bDirect = true;
				}
				else
				{
					m_bDirect = false;
				}
			}
			else if( !strcmp( szKey, SHOW_ORDER ) )
			{
				if( !strcmp( szValue, ENGLISH_TO_KOREAN ) )
				{
					m_bShowEnglishFirst = true;
				}
				else
				{
					m_bShowEnglishFirst = false;
				}
			}
			else if( !strcmp( szKey, SHOW_SETUP ) )
			{
				if( !strcmp( szValue, "true" ) )
				{
					m_bShowSetup = true;
				}
				else
				{
					m_bShowSetup = false;
				}
			}
			else if( !strcmp( szKey, VOICE_SETUP ) )
			{
				m_strVoiceSetup = szValue;
			}
			else if( !strcmp( szKey, ENGLISH_FONT ) )
			{
				m_strEnglishFont = szValue;
			}
			else if( !strcmp( szKey, KOREAN_FONT ) )
			{
				m_strKoreanFont = szValue;
			}
			else if( !strcmp( szKey, LANGUAGE_LIST ) )
			{
				SetLanguageList( szValue );
			}
			else if( !strcmp( szKey, LANGUAGE_SETUP ) )
			{
				m_strLanguageSetup = szValue;
			}
			else if( !strcmp( szKey, SHOW_METHOD ) )
			{
				strValue = szValue;
				SetShowMethodString( strValue );
			}
		}
	}
	fclose( fd );

	if( m_iRepeatPeriod < 1 )
	{
		m_iRepeatPeriod = 10;
	}

	return true;
}

bool CSetup::PutSetup()
{
	FILE	* fd;
	CString	strLanguageList, strShowMethod;

	USES_CONVERSION;

	fd = fopen( T2A(m_strFileName), "w" );
	if( fd == NULL ) return false;

	fprintf( fd, "%s = %d%s", REPEAT_PERIOD_KEY, m_iRepeatPeriod, NEW_LINE );
	fprintf( fd, "%s = %d%s", KOREAN_SHOW_TIME, m_iKoreanShowTime, NEW_LINE );
	fprintf( fd, "%s = %d%s", LAST_INDEX, m_iLastIndex, NEW_LINE );
	fprintf( fd, "%s = %s%s", AUTO_SPEEK_KEY, m_bAutoSpeek ? "true" : "false", NEW_LINE );
	fprintf( fd, "%s = %s%s", DATA_FILENAME, T2A(m_strDataFileName), NEW_LINE );
	fprintf( fd, "%s = %s%s", EXECUTION_METHOD, m_bAutomatic ? EXECUTION_AUTOMATIC : EXECUTION_MANUAL, NEW_LINE );
	fprintf( fd, "%s = %s%s", EXECUTION_RANDOM, m_bRandom ? "true" : "false", NEW_LINE );
	fprintf( fd, "%s = %s%s", CONTENTS_ORDER, m_bDirect ? CONTENTS_DIRECT : CONTENTS_INDIRECT, NEW_LINE );
	fprintf( fd, "%s = %s%s", SHOW_ORDER, m_bShowEnglishFirst ? ENGLISH_TO_KOREAN : KOREAN_TO_ENGLISH, NEW_LINE );
	fprintf( fd, "%s = %s%s", SHOW_SETUP, m_bShowSetup ? "true" : "false", NEW_LINE );
	fprintf( fd, "%s = %s%s", VOICE_SETUP, T2A(m_strVoiceSetup), NEW_LINE );
	fprintf( fd, "%s = %s%s", ENGLISH_FONT, T2A(m_strEnglishFont), NEW_LINE );
	fprintf( fd, "%s = %s%s", KOREAN_FONT, T2A(m_strKoreanFont), NEW_LINE );

	GetLanguageList( strLanguageList );
	fprintf( fd, "%s = %s%s", LANGUAGE_LIST, T2A(strLanguageList), NEW_LINE );
	fprintf( fd, "%s = %s%s", LANGUAGE_SETUP, T2A(m_strLanguageSetup), NEW_LINE );

	GetShowMethodString( strShowMethod );
	fprintf( fd, "%s = %s%s", SHOW_METHOD, T2A(strShowMethod), NEW_LINE );

	fclose( fd );

	return true;
}

bool CSetup::IsSet()
{
	return true;
}

bool CSetup::GetEnglishFont( LOGFONT * psttFont )
{
	if( m_strEnglishFont.IsEmpty() ) return false;

	StringToFont( m_strEnglishFont, psttFont );

	return true;
}

bool CSetup::GetKoreanFont( LOGFONT * psttFont )
{
	if( m_strKoreanFont.IsEmpty() ) return false;

	StringToFont( m_strKoreanFont, psttFont );

	return true;
}

void CSetup::SetEnglishFont( LOGFONT * psttFont )
{
	FontToString( psttFont, m_strEnglishFont );
}

void CSetup::SetKoreanFont( LOGFONT * psttFont )
{
	FontToString( psttFont, m_strKoreanFont );
}

void CSetup::SetFont( int iType, const char * pszBuf, LOGFONT * psttFont )
{
	switch( iType )
	{
	case 0:		psttFont->lfHeight = atoi( pszBuf );	break;
	case 1:		psttFont->lfWidth = atoi( pszBuf );	break;
	case 2:		psttFont->lfEscapement = atoi( pszBuf );	break;
	case 3:		psttFont->lfOrientation = atoi( pszBuf );	break;
	case 4:		psttFont->lfWeight = atoi( pszBuf );	break;
	case 5:		psttFont->lfItalic = atoi( pszBuf );	break;
	case 6:		psttFont->lfUnderline = atoi( pszBuf );	break;
	case 7:		psttFont->lfStrikeOut = atoi( pszBuf );	break;
	case 8:		psttFont->lfCharSet = atoi( pszBuf );	break;
	case 9:		psttFont->lfOutPrecision = atoi( pszBuf );	break;
	case 10:	psttFont->lfClipPrecision = atoi( pszBuf );	break;
	case 11:	psttFont->lfQuality = atoi( pszBuf );	break;
	case 12:	psttFont->lfPitchAndFamily = atoi( pszBuf );	break;
	case 13:
		{
			USES_CONVERSION;

			_tcscpy( psttFont->lfFaceName, A2T( pszBuf ) );
			break;
		}
	}
}

void CSetup::StringToFont( CString & strFont, LOGFONT * psttFont )
{
	USES_CONVERSION;

	const char * pszFont = T2A(strFont);
	int	i, j, iType, iLen = (int)strlen( pszFont );
	char	szBuf[1024];

	memset( szBuf, 0, sizeof(szBuf) );
	for( i = 0, j = 0, iType = 0; i < iLen; ++i )
	{
		if( pszFont[i] == '\r' || pszFont[i] == '\n' )
		{
			SetFont( iType, szBuf, psttFont );
			j = 0;
			break;
		}
		else if( pszFont[i] == '|' )
		{
			SetFont( iType, szBuf, psttFont );
			++iType;
			j = 0;
			memset( szBuf, 0, sizeof(szBuf) );
		}
		else
		{
			szBuf[j] = pszFont[i];
			++j;
		}
	}

	if( j > 0 )
	{
		SetFont( iType, szBuf, psttFont );
	}
}

void CSetup::FontToString( LOGFONT * psttFont, CString & strFont )
{
	strFont.Format( _T("%ld|%ld|%ld|%ld|%ld|%d|%d|%d|%d|%d|%d|%d|%d|%s")
		, psttFont->lfHeight
		, psttFont->lfWidth
		, psttFont->lfEscapement
		, psttFont->lfOrientation
		, psttFont->lfWeight
		, psttFont->lfItalic
		, psttFont->lfUnderline
		, psttFont->lfStrikeOut
		, psttFont->lfCharSet
		, psttFont->lfOutPrecision
		, psttFont->lfClipPrecision
		, psttFont->lfQuality
		, psttFont->lfPitchAndFamily
		, psttFont->lfFaceName );
}

void CSetup::SetLanguageList( const char * pszValue )
{
	int	i, j, iType, iLen = (int)strlen( pszValue );
	char	szLanguage[1024], szTts[1024];

	memset( szLanguage, 0, sizeof(szLanguage) );
	memset( szTts, 0, sizeof(szTts) );

	iType = 0;
	for( i = 0, j = 0; i < iLen; ++i )
	{
		if( pszValue[i] == '|' )
		{
			if( iType == 0 )
			{
				++iType;
			}
			else
			{
				iType = 0;

				CLanguageVoice	clsLanguageVoice;

				clsLanguageVoice.m_strLanguage = szLanguage;
				clsLanguageVoice.m_strVoice = szTts;

				m_clsLanguageVoiceList.AddTail( clsLanguageVoice );

				memset( szLanguage, 0, sizeof(szLanguage) );
				memset( szTts, 0, sizeof(szTts) );
			}

			j = 0;
		}
		else if( iType == 0 )
		{
			szLanguage[j] = pszValue[i];
			++j;
		}
		else
		{
			szTts[j] = pszValue[i];
			++j;
		}
	}

	if( j > 0 )
	{
		CLanguageVoice	clsLanguageVoice;

		clsLanguageVoice.m_strLanguage = szLanguage;
		clsLanguageVoice.m_strVoice = szTts;

		m_clsLanguageVoiceList.AddTail( clsLanguageVoice );
	}
}

void CSetup::GetLanguageList( CString & strValue )
{
	CLanguageVoice	clsLanguageVoice;
	POSITION	pos;

	strValue.Empty();
	for( pos = m_clsLanguageVoiceList.GetHeadPosition(); pos; )
	{
		clsLanguageVoice = m_clsLanguageVoiceList.GetNext( pos );

		strValue.Append( clsLanguageVoice.m_strLanguage );
		strValue.Append( _T("|") );
		strValue.Append( clsLanguageVoice.m_strVoice );
		strValue.Append( _T("|") );
	}
}

void CSetup::GetLanguageVoice( CString & strLanguage, CString & strVoice )
{
	CLanguageVoice	clsLanguageVoice;
	POSITION	pos;

	strVoice.Empty();
	for( pos = m_clsLanguageVoiceList.GetHeadPosition(); pos; )
	{
		clsLanguageVoice = m_clsLanguageVoiceList.GetNext( pos );

		if( !strLanguage.Compare( clsLanguageVoice.m_strLanguage ) )
		{
			strVoice = clsLanguageVoice.m_strVoice;
			break;
		}
	}
}

void CSetup::GetShowMethodString( CString & strShowMethod )
{
	switch( m_eShowMethod )
	{
	case E_SHOW_BOTH:
		strShowMethod = SHOW_BOTH;
		break;
	case E_SHOW_FIRST_SPEAK_SECOND:
		strShowMethod = SHOW_FIRST_SPEAK_SECOND;
		break;
	case E_SHOW_FIRST_SECOND:
		strShowMethod = SHOW_FIRST_SECOND;
		break;
	default:
		strShowMethod = "";
		break;
	}
}

void CSetup::SetShowMethodString( CString & strShowMethod )
{
	char	szText[255];

	USES_CONVERSION;
	_snprintf( szText, sizeof(szText), "%s", T2A(strShowMethod) );

	if( !strcmp( szText, SHOW_FIRST_SPEAK_SECOND ) )
	{
		m_eShowMethod = E_SHOW_FIRST_SPEAK_SECOND;
	}
	else if( !strcmp( szText, SHOW_FIRST_SECOND ) )
	{
		m_eShowMethod = E_SHOW_FIRST_SECOND;
	}
	else
	{
		m_eShowMethod = E_SHOW_BOTH;
	}
}
