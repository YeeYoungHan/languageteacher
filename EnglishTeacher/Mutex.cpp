/*----------------------------------------------------------------------------
 * Mutex.cpp
 *----------------------------------------------------------------------------
 * programmer : yee young han ( websearch@naver.com )
 *----------------------------------------------------------------------------
 * start date : 2008/03/06
 *----------------------------------------------------------------------------
 * comment    : mutex 기능을 수행하는 클래스 정의
 *----------------------------------------------------------------------------
 */

#include "stdafx.h"
#include "Mutex.h"

#ifdef DEAD_LOCK_TEST
static bool bIsFirst = false;
#ifdef WIN32
CRITICAL_SECTION CMutex::m_sttClassMutex;
#else
pthread_mutex_t	 CMutex::m_sttClassMutex;
#endif
#endif

CMutex::CMutex( const char * pszName )
{
#ifdef WIN32
	InitializeCriticalSection( &m_sttMutex );
#else
	pthread_mutex_init( &m_sttMutex, NULL );
#endif

#ifdef DEAD_LOCK_TEST
	if( bIsFirst == false )
	{
#ifdef WIN32
		InitializeCriticalSection( &m_sttClassMutex );
#else
		pthread_mutex_init( &m_sttClassMutex, NULL );
#endif

		bIsFirst = true;
	}

	if( pszName )
	{
		m_strName = pszName;
	}
#endif
}

CMutex::~CMutex()
{
#ifdef WIN32
	DeleteCriticalSection( &m_sttMutex );
#else
	pthread_mutex_destroy( &m_sttMutex );
#endif
}

bool CMutex::acquire()
{
#ifdef DEAD_LOCK_TEST
	LOCK_LIST::const_iterator	it;
	unsigned int	iId;

#ifdef WIN32
	iId = GetCurrentThreadId();
#else
	iId = (unsigned int)pthread_self();
#endif

#ifdef WIN32
	EnterCriticalSection( &m_sttClassMutex );
#else
	pthread_mutex_lock( &m_sttClassMutex );
#endif
	for( it = m_clsThreadMap.begin(); it != m_clsThreadMap.end(); ++it )
	{
		if( iId == *it )
		{
			printf( "dead lock (%s)\n", m_strName.c_str() );
			char	* pszName = NULL;

			sprintf( pszName, "test me........................................" );
			break;
		}
	}

	m_clsThreadMap.push_back( iId );
#ifdef WIN32
	LeaveCriticalSection( &m_sttClassMutex );
#else
	pthread_mutex_unlock( &m_sttClassMutex );
#endif

#endif

#ifdef WIN32
	EnterCriticalSection( &m_sttMutex );
#else
	if( pthread_mutex_lock( &m_sttMutex ) != 0 )
	{
		return false;
	}
#endif

	return true;
}

bool CMutex::release()
{
#ifdef WIN32
	LeaveCriticalSection( &m_sttMutex );
#else
	if( pthread_mutex_unlock( &m_sttMutex ) != 0 )
	{
		return false;
	}
#endif

#ifdef DEAD_LOCK_TEST
	LOCK_LIST::iterator	it;
	unsigned int	iId;

#ifdef WIN32
	iId = GetCurrentThreadId();
#else
	iId = (unsigned int)pthread_self();
#endif

#ifdef WIN32
	EnterCriticalSection( &m_sttClassMutex );
#else
	pthread_mutex_lock( &m_sttClassMutex );
#endif
	for( it = m_clsThreadMap.begin(); it != m_clsThreadMap.end(); ++it )
	{
		if( iId == *it )
		{
			m_clsThreadMap.erase( it );
			break;
		}
	}

#ifdef WIN32
	LeaveCriticalSection( &m_sttClassMutex );
#else
	pthread_mutex_unlock( &m_sttClassMutex );
#endif
#endif

	return true;
}

CMutexSignal::CMutexSignal( const char * pszName ) : CMutex( pszName )
{
#ifdef WIN32
#else
	pthread_cond_init( &m_sttCond, NULL );
#endif
}

CMutexSignal::~CMutexSignal()
{
#ifdef WIN32
#else
	pthread_cond_destroy( &m_sttCond );
#endif
}

bool CMutexSignal::wait()
{
#ifdef WIN32
	Sleep(1000);
#else
	int n = pthread_cond_wait( &m_sttCond, &m_sttMutex );
	if( n != 0 )
	{
		return false;
	}
#endif

	return true;
}

bool CMutexSignal::signal()
{
#ifdef WIN32
#else
	int n = pthread_cond_signal( &m_sttCond );
	if( n != 0 )
	{
		return false;
	}
#endif

	return true;
}

bool CMutexSignal::broadcast()
{
#ifdef WIN32
#else
	int n = pthread_cond_broadcast( &m_sttCond );
	if( n != 0 )
	{
		return false;
	}
#endif

	return true;
}
