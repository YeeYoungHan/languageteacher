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
