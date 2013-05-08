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

TCHAR * HGetProgramDirectory( )
{
	static	TCHAR	szDir[1024];

	if( _tcslen(szDir) == 0 )
	{
		int		i;
		HMODULE	hThis;

		hThis = GetModuleHandle( NULL );

		GetModuleFileName( hThis, szDir, sizeof(szDir));
		for( i = (int)_tcslen( szDir) - 1; i >= 0; i-- )
		{
			if( szDir[i] == _T('\\') ) 
			{
				szDir[i] = '\0';
				break;
			}
		}
	}

	return szDir;
}

void VoiceSetup( )
{
	HRESULT hr;

	ISpObjectTokenCategory * pCategory = NULL;
	IEnumSpObjectTokens * pEnum = NULL;
	ISpVoice * pVoice = NULL;
	ULONG	iCount;
	long iRate;
	CString strName;

  if( FAILED(CoInitialize(NULL)) )
	{
		AfxMessageBox(_T("Error to intiliaze COM"));
    return;
	}

	hr = CoCreateInstance(CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, IID_ISpObjectTokenCategory, (void **)&pCategory);
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pCategory->SetId( SPCAT_VOICES, TRUE );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;
	
	hr = pCategory->EnumTokens( NULL, NULL, &pEnum );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pEnum->GetCount( &iCount );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	while( SUCCEEDED( hr ) && iCount-- )
	{
		CComPtr<ISpObjectToken> pToken;
	
		hr = pEnum->Next( 1, &pToken, NULL );
		if( SUCCEEDED( hr ) )
		{
			TCHAR	* pszId;

			hr = pToken->GetId( &pszId );
			if( SUCCEEDED( hr ) )
			{
				int	i, iLen = (int)_tcslen(pszId);

				for( i = iLen - 1; i >= 0; --i )
				{
					if( pszId[i] == _T('\\') )
					{
						strName = pszId + i + 1;
						break;
					}
				}

				pVoice->GetRate( &iRate );
			}
		}
	}

FUNC_END:
	if( pVoice ) pVoice->Release();
	if( pEnum ) pEnum->Release();
	if( pCategory ) pCategory->Release();

	CoUninitialize();
}

void VoiceSetup( const TCHAR * pszVoice, ISpVoice * pVoice )
{
	if( pszVoice == NULL || _tcslen(pszVoice) == 0 ) return;
	if( pVoice == NULL ) return;

	HRESULT hr;

	ISpObjectTokenCategory * pCategory = NULL;
	IEnumSpObjectTokens * pEnum = NULL;
	ULONG	iCount;
	CString strName;

	hr = CoCreateInstance(CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, IID_ISpObjectTokenCategory, (void **)&pCategory);
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pCategory->SetId( SPCAT_VOICES, TRUE );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;
	
	hr = pCategory->EnumTokens( NULL, NULL, &pEnum );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pEnum->GetCount( &iCount );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	while( SUCCEEDED( hr ) && iCount-- )
	{
		CComPtr<ISpObjectToken> pToken;
	
		hr = pEnum->Next( 1, &pToken, NULL );
		if( SUCCEEDED( hr ) )
		{
			WCHAR	* pszId;

			hr = pToken->GetId( &pszId );
			if( SUCCEEDED( hr ) )
			{
				int	i, iLen = (int)_tcslen(pszId);

				for( i = iLen - 1; i >= 0; --i )
				{
					if( pszId[i] == _T('\\') )
					{
						strName = pszId + i + 1;
						break;
					}
				}

				if( !strName.Compare( pszVoice ) )
				{
					pVoice->SetVoice( pToken );
					break;
				}
			}
		}
	}

FUNC_END:
	if( pEnum ) pEnum->Release();
	if( pCategory ) pCategory->Release();
}

void GetVoiceList( CStringList & clsList )
{
	HRESULT hr;

	ISpObjectTokenCategory * pCategory = NULL;
	IEnumSpObjectTokens * pEnum = NULL;
	ULONG	iCount;
	CString strName;

  if( FAILED(CoInitialize(NULL)) )
	{
		AfxMessageBox(_T("Error to intiliaze COM"));
    return;
	}

	hr = CoCreateInstance(CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL, IID_ISpObjectTokenCategory, (void **)&pCategory);
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pCategory->SetId( SPCAT_VOICES, TRUE );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;
	
	hr = pCategory->EnumTokens( NULL, NULL, &pEnum );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	hr = pEnum->GetCount( &iCount );
	if( !SUCCEEDED( hr ) ) goto FUNC_END;

	while( SUCCEEDED( hr ) && iCount-- )
	{
		CComPtr<ISpObjectToken> pToken;
	
		hr = pEnum->Next( 1, &pToken, NULL );
		if( SUCCEEDED( hr ) )
		{
			WCHAR	* pszId;

			hr = pToken->GetId( &pszId );
			if( SUCCEEDED( hr ) )
			{
				int	i, iLen = (int)_tcslen(pszId);

				for( i = iLen - 1; i >= 0; --i )
				{
					if( pszId[i] == _T('\\') )
					{
						strName = pszId + i + 1;
						break;
					}
				}

				clsList.AddTail( strName );
			}
		}
	}

FUNC_END:
	if( pEnum ) pEnum->Release();
	if( pCategory ) pCategory->Release();

	CoUninitialize();
}

int StringCompare( CString & strFirst, const char * pszSecond )
{
	USES_CONVERSION;

	CString	strSecond = A2T(pszSecond);

	return strFirst.Compare( strSecond );
}

void AddStringToComboBox( CComboBox & clsComboBox, const char * pszText )
{
	USES_CONVERSION;

	clsComboBox.AddString( A2T(pszText) );
}

bool IsSpace( char c )
{
	if( c == ' ' || c == '\t' ) return true;

	return false;
}

char * HTrim( char *pstrInput )
{
	int		iLen, i;
	char	* p = NULL;

	iLen = (int)strlen( pstrInput );
	if( iLen == 0 ) return NULL;

	for( i = 0; i < iLen; i++ )
	{
		if( IsSpace(pstrInput[i]) ) p = &pstrInput[i];			
		else								  break;
	}

	if( i == iLen )
	{
		pstrInput[0] = '\0';
		return NULL;	
	}
	
	if( p )	strcpy( pstrInput, p+1 );

	iLen = (int)strlen( pstrInput );
	if( iLen == 0 ) return NULL;

	for( i = iLen - 1; i > 0; i-- )
	{
		if( IsSpace(pstrInput[i]) ) pstrInput[i] = '\0';
		else							 break;
	}
	
	return pstrInput;
}