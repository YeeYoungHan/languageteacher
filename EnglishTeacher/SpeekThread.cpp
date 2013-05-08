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
