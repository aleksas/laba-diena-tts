// redakDoc.h : interface of the CRedakDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REDAKDOC_H__9258AD43_C29C_11D2_8544_0080AD40D45B__INCLUDED_)
#define AFX_REDAKDOC_H__9258AD43_C29C_11D2_8544_0080AD40D45B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CRedakDoc : public CDocument
{
protected: // create from serialization only
	CRedakDoc();
	DECLARE_DYNCREATE(CRedakDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRedakDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRedakDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRedakDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REDAKDOC_H__9258AD43_C29C_11D2_8544_0080AD40D45B__INCLUDED_)
