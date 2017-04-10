// redakDoc.cpp : implementation of the CRedakDoc class
//

#include "stdafx.h"
#include "redak.h"

#include "redakDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRedakDoc

IMPLEMENT_DYNCREATE(CRedakDoc, CDocument)

BEGIN_MESSAGE_MAP(CRedakDoc, CDocument)
	//{{AFX_MSG_MAP(CRedakDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRedakDoc construction/destruction

CRedakDoc::CRedakDoc()
{
	// TODO: add one-time construction code here

}

CRedakDoc::~CRedakDoc()
{
}

BOOL CRedakDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRedakDoc serialization

void CRedakDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CRedakDoc diagnostics

#ifdef _DEBUG
void CRedakDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRedakDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

