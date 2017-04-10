// redak.h : main header file for the REDAK application
//

#if !defined(AFX_REDAK_H__9258AD3D_C29C_11D2_8544_0080AD40D45B__INCLUDED_)
#define AFX_REDAK_H__9258AD3D_C29C_11D2_8544_0080AD40D45B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRedakApp:
// See redak.cpp for the implementation of this class
//

class CRedakApp : public CWinApp
{
public:
	CRedakApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRedakApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRedakApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REDAK_H__9258AD3D_C29C_11D2_8544_0080AD40D45B__INCLUDED_)
