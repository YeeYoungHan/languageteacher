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

#pragma once

#define SETUP_FILENAME				_T("EnglishTeacher.ini")
#define REPEAT_PERIOD_KEY			"RepeatPeriod"
#define AUTO_SPEEK_KEY				"AutoSpeek"
#define DATA_FILENAME					"DataFileName"
#define EXECUTION_METHOD			"ExecutionMethod"
#define EXECUTION_RANDOM			"ExecutionRandom"
#define CONTENTS_ORDER				"ContentsOrder"
#define KOREAN_SHOW_TIME			"KoreanShowTime"
#define LAST_INDEX						"LastIndex"
#define VOICE_SETUP						"VoiceSetup"
#define SHOW_ORDER						"ShowOrder"
#define SHOW_SETUP						"ShowSetup"
#define ENGLISH_FONT					"EnglishFont"
#define KOREAN_FONT						"KoreanFont"
#define LANGUAGE_LIST					"LanguageList"
#define LANGUAGE_SETUP				"LauguageSetup"
#define SHOW_METHOD						"ShowMethod"
#define NEW_LINE							"\r\n"

#define EXECUTION_AUTOMATIC		"Automatic"
#define EXECUTION_MANUAL			"Manual"
#define EXECUTION_MANUAL_RANDOM	"Manual-Random"

#define CONTENTS_DIRECT				"Direct"
#define CONTENTS_INDIRECT			"Opposite"

#define ENGLISH_TO_KOREAN			"English -> Korean"
#define KOREAN_TO_ENGLISH			"Korean -> English"

#define SHOW_BOTH								"Both"
#define SHOW_FIRST_SPEAK_SECOND	"Lang -> Speak -> Lang"
#define SHOW_FIRST_SECOND				"Lang -> Lang"

enum EShowMethod
{
	E_SHOW_BOTH = 0,
	E_SHOW_FIRST_SPEAK_SECOND,
	E_SHOW_FIRST_SECOND
};

class CLanguageVoice
{
public:
	CString	m_strLanguage;
	CString	m_strVoice;
};

class CSetup
{
public:
	CSetup(void);
	~CSetup(void);

	bool GetSetup();
	bool PutSetup();
	bool IsSet();

	int		m_iRepeatPeriod;
	int		m_iKoreanShowTime;
	int		m_iLastIndex;
	bool	m_bAutoSpeek;
	bool	m_bAutomatic;
	bool	m_bRandom;
	bool	m_bDirect;
	bool	m_bShowEnglishFirst;		// Show Order
	bool	m_bShowSetup;
	CString	m_strDataFileName;
	CString m_strVoiceSetup;
	CString m_strLanguageSetup;
	CList< CLanguageVoice > m_clsLanguageVoiceList;
	EShowMethod	m_eShowMethod;

	bool GetEnglishFont( LOGFONT * psttFont );
	bool GetKoreanFont( LOGFONT * psttFont );
	void SetEnglishFont( LOGFONT * psttFont );
	void SetKoreanFont( LOGFONT * psttFont );
	void GetLanguageVoice( CString & strLanguage, CString & strVoice );
	void GetShowMethodString( CString & strShowMethod );
	void SetShowMethodString( CString & strShowMethod );

private:
	CString	m_strFileName;
	CString m_strEnglishFont;
	CString m_strKoreanFont;

	void SetFont( int iType, const char * pszBuf, LOGFONT * psttFont );
	void StringToFont( CString & strFont, LOGFONT * psttFont );
	void FontToString( LOGFONT * psttFont, CString & strFont );
	void SetLanguageList( const char * pszValue );
	void GetLanguageList( CString & strValue );
};

extern CSetup	gclsSetup;
