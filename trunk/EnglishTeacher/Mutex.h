/*----------------------------------------------------------------------------
 * Mutex.h
 *----------------------------------------------------------------------------
 * programmer : yee young han ( websearch@naver.com )
 *----------------------------------------------------------------------------
 * start date : 2008/03/06
 *----------------------------------------------------------------------------
 * comment    : mutex 기능을 수행하는 클래스 선언
 *----------------------------------------------------------------------------
 */

#ifndef _MUTEX_H_
#define _MUTEX_H_

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#ifdef _DEBUG
#define DEAD_LOCK_TEST
#endif

#ifdef DEAD_LOCK_TEST
#include <list>
#include <string>
typedef std::list< unsigned int >	LOCK_LIST;
#endif

/** @brief mutex 기능을 수행하는 클래스 */

class CMutex
{
public:
	CMutex( const char * pszName );
	~CMutex();
	
	bool acquire();
	bool release();

protected:
#ifdef WIN32
	CRITICAL_SECTION m_sttMutex;
#else
	pthread_mutex_t	 m_sttMutex;
#endif

#ifdef DEAD_LOCK_TEST
	LOCK_LIST		m_clsThreadMap;
	std::string	m_strName;

#ifdef WIN32
	static CRITICAL_SECTION m_sttClassMutex;
#else
	static pthread_mutex_t	 m_sttClassMutex;
#endif
#endif
};

class CMutexSignal : public CMutex
{
public:
	CMutexSignal( const char * pszName );
	~CMutexSignal();
	
	bool wait();
	bool signal();
	bool broadcast();

private:
#ifndef WIN32
	pthread_cond_t		m_sttCond;
#endif
};

#endif
