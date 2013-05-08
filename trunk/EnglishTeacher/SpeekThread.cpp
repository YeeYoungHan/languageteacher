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
#include "Mutex.h"

static bool gbIsStop = false;
static bool gbIsRunning = false;
static bool gbIsSpeeking = false;
static bool gbVoiceChange = true;

static CStringList gclsSpeekList;
static CMutex gclsMutex("SpeekList");

UINT SpeekThread( LPVOID pParam )
{
	CString strSpeekText;

	ISpVoice * pVoice = NULL;

  if( FAILED(CoInitialize(NULL)) )
	{
		AfxMessageBox(_T("Error to intiliaze COM"));
    goto FUNC_END;
	}

	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	if( !SUCCEEDED( hr ) )
	{
	  CoUninitialize();
		goto FUNC_END;
	}

	gbIsRunning = true;
	while( gbIsStop == false )
	{
		strSpeekText.Empty();
		gclsMutex.acquire();
		if( gclsSpeekList.GetCount() > 0 )
		{
			strSpeekText = gclsSpeekList.RemoveHead();
		}
		gclsMutex.release();

		if( strSpeekText.IsEmpty() == false )
		{
			gbIsSpeeking = true;
			if( gbVoiceChange )
			{
				VoiceSetup( gclsSetup.m_strVoiceSetup, pVoice );
				gbVoiceChange = false;
			}
			hr = pVoice->Speak(strSpeekText.AllocSysString(), 0, NULL);
			gbIsSpeeking = false;
		}
		Sleep(50);
	}

	pVoice->Release();
	pVoice = NULL;

  CoUninitialize();

FUNC_END:
	gbIsRunning = false;

	return 0;
}

void SetStopSpeekThread()
{
	gbIsStop = true;
}

bool IsStopSpeekThread()
{
	return !gbIsRunning;
}

bool IsSpeeking()
{
	return gbIsSpeeking;
}

void SetVoiceChange()
{
	gbVoiceChange = true;
}

void AddSpeekText( CString & strText )
{
	gclsMutex.acquire();
	gclsSpeekList.AddTail( strText );
	gclsMutex.release();
}

CWinThread * StartSpeekThread()
{
	return AfxBeginThread( SpeekThread, NULL );
}
