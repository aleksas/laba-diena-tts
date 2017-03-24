// redakView.h : interface of the CRedakView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REDAKVIEW_H__9258AD45_C29C_11D2_8544_0080AD40D45B__INCLUDED_)
#define AFX_REDAKVIEW_H__9258AD45_C29C_11D2_8544_0080AD40D45B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "mmsystem.h"
#include "LithUSS.h"

class CRedakView : public CEditView
{
protected: // create from serialization only
	CRedakView();
	DECLARE_DYNCREATE(CRedakView)

void Skaityti(CString Zodis);
LRESULT WindowProc(UINT Message, WPARAM hwo, LPARAM whdr);

HGLOBAL hData;
DWORD dwBufLen;
HWAVEOUT hwo;       // handle of wave output device
WAVEHDR whdr;

CMainFrame* cff;

int i,ilg,nuo,iki;
CString eilute;
BOOL NeedWord;

CString Zodis;

int balsas;

//struct event {short Id; short phonviz; int charOffset; long signOffset;}; 
int evsize;
int evsizet;
event *evarr;

long largebufsize;
long largebufsizet;
short *largebuf; //sintezuotas signalas

int nuoe, ikie;

// Attributes
public:
	CRedakDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRedakView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRedakView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRedakView)
	afx_msg void OnSelSuzinoti();
	afx_msg void OnSelPause();
	afx_msg void OnSelStop();
	afx_msg void OnUpdateSelSkaityti(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelPause(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnGreitis();
	afx_msg void OnTonas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in redakView.cpp
inline CRedakDoc* CRedakView::GetDocument()
   { return (CRedakDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REDAKVIEW_H__9258AD45_C29C_11D2_8544_0080AD40D45B__INCLUDED_)
