// StringSetSample.h : main header file for the STRINGSETSAMPLE application
//

#if !defined(AFX_STRINGSETSAMPLE_H__D6098EB3_D8B5_4435_B84D_4F42129F0BB7__INCLUDED_)
#define AFX_STRINGSETSAMPLE_H__D6098EB3_D8B5_4435_B84D_4F42129F0BB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CStringSetSampleApp:
// See StringSetSample.cpp for the implementation of this class
//

class CStringSetSampleApp : public CWinApp
{
public:
	CStringSetSampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStringSetSampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CStringSetSampleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRINGSETSAMPLE_H__D6098EB3_D8B5_4435_B84D_4F42129F0BB7__INCLUDED_)
