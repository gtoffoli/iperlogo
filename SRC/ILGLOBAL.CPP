// ilglobal.cpp : global utility function, mainly for use by C modules
//

#include "stdafx.h"

extern "C"
int MessBox (char *s)
{
	CString msg = s;
	return AfxMessageBox (msg, MB_OK, 0);
}


