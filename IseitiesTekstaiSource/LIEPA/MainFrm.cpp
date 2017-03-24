// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "redak.h"

#include "MainFrm.h"
#include "LithUSS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(ID_COMBO, OnCombo)
	ON_COMMAND(ID_TONAS, OnTonas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CRect rect;
	int nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_COMBO);
	m_wndToolBar.SetButtonInfo(nIndex, ID_COMBO, TBBS_SEPARATOR, 205);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.bottom = rect.top +250;
	if(!m_comboBox.Create(CBS_DROPDOWNLIST | WS_VISIBLE |
		WS_TABSTOP | WS_VSCROLL, rect, &m_wndToolBar, ID_COMBO))
	{
		TRACE0("Failed to create combobox\n");
		return -1;      // fail to create
	}
	
	m_comboBox.AddString("Regina");		//0
	m_comboBox.AddString("Aistë");		//1
	m_comboBox.AddString("Edvardas");	//2
	m_comboBox.AddString("Vladas");		//3
	m_comboBox.SetCurSel(0);

	CRect rect2;
	int nIndex2 = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_GREITIS);
	m_wndToolBar.SetButtonInfo(nIndex2, ID_GREITIS, TBBS_SEPARATOR, 45);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex2, &rect2);
	rect2.top = 1;
	rect2.bottom = rect2.top +90;
	if(!m_editGreitis.Create(ES_RIGHT | WS_VISIBLE |
		WS_TABSTOP, rect2, &m_wndToolBar, ID_GREITIS))
	{
		TRACE0("Failed to create editGreitis\n");
		return -1;      // fail to create
	}
	m_editGreitis.SetWindowText("100");

	CRect rect3;
	int nIndex3 = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_TONAS);
	m_wndToolBar.SetButtonInfo(nIndex3, ID_TONAS, TBBS_SEPARATOR, 45);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex3, &rect3);
	rect3.top = 1;
	rect3.bottom = rect3.top +90;
	if(!m_editTonas.Create(ES_RIGHT | WS_VISIBLE |
		WS_TABSTOP, rect3, &m_wndToolBar, ID_TONAS))
	{
		TRACE0("Failed to create editTonas\n");
		return -1;      // fail to create
	}
	m_editTonas.SetWindowText("100");

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

extern char *katvardai[4];
char* getLUSSErrorMessages(int);

void CMainFrame::OnCombo() 
{
	MessageBox("Keiciamas balsas. Prasau palaukti");

	CString s;
	m_wndStatusBar.GetPaneText(0, s);
	m_wndStatusBar.SetPaneText(0, "Keièiamas balsas", TRUE);

	int res;
	if((res = initLUSS(".\\", katvardai[m_comboBox.GetCurSel()])) < 0)
		{MessageBox(getLUSSErrorMessages(res));exit(EXIT_FAILURE);}

	m_wndStatusBar.SetPaneText(0, s, TRUE);
}

extern int greicio_koeficientas;
extern int tono_koeficientas;

void CMainFrame::OnGreitis() 
{
	CString s;
	int ggg; char cc;
	m_editGreitis.GetWindowText(s);
	int jj = sscanf((LPCTSTR)s, "%d", &ggg, &cc);
	if(jj != 1) 
		{
		ggg = 100;
		m_editGreitis.SetWindowText("100");
		}
	greicio_koeficientas = ggg;
}

void CMainFrame::OnTonas() 
{
	CString s;
	int ttt; char cc;
	m_editTonas.GetWindowText(s);
	int jj = sscanf((LPCTSTR)s, "%d", &ttt, &cc);
	if(jj != 1) 
		{
		ttt = 100;
		m_editTonas.SetWindowText("100");
		}
	tono_koeficientas = ttt;
}
