/***************************************************************************
      File Name:	PSemu.cpp
   File Created:	Monday, October 6th 2003
      Copyright:	(C) 2003 by Darko Matesic
          Email:	darkman@eunet.yu
 ***************************************************************************/

#include "stdafx.h"
                            
const  unsigned char version  = 1;
const  unsigned char revision = 2;
const  unsigned char build    = 0;
static char *libraryName      = LIBRARY_NAME;
static char *PluginAuthor     = "Darko Matesic";

//==========================================================================
// 
//==========================================================================

char * CALLBACK PSEgetLibName(void)
{
	return libraryName;
}

//==========================================================================
// 
//==========================================================================

unsigned long CALLBACK PSEgetLibType(void)
{
	return 2;
}

//==========================================================================
// 
//==========================================================================

unsigned long CALLBACK PSEgetLibVersion(void)
{
	return version<<16 | revision<<8 | build;
}