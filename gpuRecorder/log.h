/***************************************************************************
      File Name:	Log.h
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"

#ifndef _LOG_H_
#define _LOG_H_

// disable compiler message:
//          warning C4002: too many actual parameters for macro 'LogAppend'
#pragma warning(disable: 4002)

#define LOG_ENABLED

#ifdef _DEBUG
	#ifdef LOG_ENABLED
		void LogStart();
		void LogAppend(const char *, ...);
		void LogEnd();
	#else
		#define LogStart(...) do { } while(0)
		#define LogAppend(...) do { } while(0)
		#define LogEnd(...) do { } while(0)
	#endif
#else
	#define LogStart(...) do { } while(0)
	#define LogAppend(...) do { } while(0)
	#define LogEnd(...) do { } while(0)
#endif

#endif