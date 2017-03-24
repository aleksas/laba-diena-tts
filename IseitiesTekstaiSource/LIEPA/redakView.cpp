// redakView.cpp : implementation of the CRedakView class
//
#include "stdafx.h"
#include "redak.h"

#include "MainFrm.h"
#include "redakDoc.h"
#include "redakView.h"
#include "LithUSS.h"
#include "raw2wav.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////
// CRedakView

IMPLEMENT_DYNCREATE(CRedakView, CEditView)

BEGIN_MESSAGE_MAP(CRedakView, CEditView)
	//{{AFX_MSG_MAP(CRedakView)
	ON_COMMAND(ID_SEL_SKAITYTI, OnSelSuzinoti)
	ON_COMMAND(ID_SEL_PAUSE, OnSelPause)
	ON_COMMAND(ID_SEL_STOP, OnSelStop)
	ON_UPDATE_COMMAND_UI(ID_SEL_SKAITYTI, OnUpdateSelSkaityti)
	ON_UPDATE_COMMAND_UI(ID_SEL_PAUSE, OnUpdateSelPause)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

char *katvardai[4] = {".\\Regina\\", ".\\Aiste\\", ".\\Edvardas\\", ".\\Vladas\\"};
int greicio_koeficientas;
int tono_koeficientas;

char* getLUSSErrorMessages(int);

//////////////////////////////////////////////////////////////////
// CRedakView construction/destruction
CRedakView::CRedakView()
{
NeedWord=FALSE;
balsas=0; //0-Regina, 1-Aiste 2-Edvardas 3-Vladas
dwBufLen=5000000;
evsize = 10000;
evarr = (event*)malloc(evsize * sizeof(event));

largebufsize = 5000000;
largebuf = (short*)malloc(largebufsize * sizeof(short)); //sintezuotas signalas

//----------------------------------------------------
if(loadLUSS("LithUSS.dll")==FALSE)
	{MessageBox("Klaida pakraunant LithUSS.dll");exit(EXIT_FAILURE);}

int res;
if((res = initLUSS(".\\", katvardai[balsas])) < 0)
	{MessageBox(getLUSSErrorMessages(res));exit(EXIT_FAILURE);}

if((hData=GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, dwBufLen))==NULL)
	{MessageBox("Nepavyko isskirti atminties");exit(EXIT_FAILURE);}

cff=NULL;
}

//-----------------------------------------------------------------
CRedakView::~CRedakView()
{
if(waveOutReset(hwo)!=MMSYSERR_NOERROR)
{MessageBox("Nepavyko nutildyti wave audio irenginio");exit(EXIT_FAILURE);}

if(waveOutClose(hwo)!=MMSYSERR_NOERROR)
{MessageBox("Nepavyko uzdaryti wave audio irenginio");exit(EXIT_FAILURE);}

if(GlobalFree(hData)!=NULL)
{MessageBox("Nepavyko atlaisvinti atminties");exit(EXIT_FAILURE);}

free(evarr);
free(largebuf);
unloadLUSS();
}
//------------------------------------------------------------------
BOOL CRedakView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

///////////////////////////////////////////////////////////////////
// CRedakView drawing

void CRedakView::OnDraw(CDC* pDC)
{
	CRedakDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

//////////////////////////////////////////////////////////////////
// CRedakView printing

BOOL CRedakView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CRedakView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CRedakView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

//////////////////////////////////////////////////////////////////
// CRedakView diagnostics

#ifdef _DEBUG
void CRedakView::AssertValid() const
{
	CEditView::AssertValid();
}

void CRedakView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CRedakDoc* CRedakView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRedakDoc)));
	return (CRedakDoc*)m_pDocument;
}
#endif //_DEBUG

////////////////////////////////////////////////////////////
// CRedakView message handlers
void CRedakView::OnSelPause() 
{
NeedWord=FALSE;
GetEditCtrl().SetSel(nuo+evarr[ikie].charOffset,iki);
}

void CRedakView::OnSelStop() 
{
if(NeedWord==FALSE) {GetEditCtrl().GetSel(nuo,iki);}
NeedWord=FALSE;
nuo=iki;
GetEditCtrl().SetSel(nuo,iki);
}

void CRedakView::OnUpdateSelSkaityti(CCmdUI* pCmdUI) 
{
if(NeedWord==FALSE)
{pCmdUI->Enable(TRUE);}
else
{pCmdUI->Enable(FALSE);}
}

void CRedakView::OnUpdateSelPause(CCmdUI* pCmdUI) 
{
if(NeedWord==TRUE)
{pCmdUI->Enable(TRUE);}
else
{pCmdUI->Enable(FALSE);}
}

//-------------------------------------------------------------
void CRedakView::OnSelSuzinoti() 
{
GetEditCtrl().GetSel(nuo,iki);
GetSelectedText(eilute);
ilg=strlen(eilute);
while((ilg==0)&&(nuo<iki))
	{
	nuo+=ilg+2;
    if(nuo>iki) nuo=iki;
    GetEditCtrl().SetSel(nuo,iki);
    GetSelectedText(eilute);
    ilg=strlen(eilute);
	}

GetEditCtrl().SetSel(nuo,nuo+ilg);
GetSelectedText(Zodis);
ikie=0;
evsizet=1;
Skaityti(Zodis);
}

//-------------------------------------------------------------------
void CRedakView::Skaityti(CString Zodis)
{
char tekstas[10000];

strcpy(tekstas,Zodis);

if(strlen(tekstas)==0)
	{NeedWord=FALSE;}
else //if(strlen(tekstas)>0)
	{
	if(ikie>=evsizet-1)
		{
		if(cff==NULL) cff = (CMainFrame*)GetActiveWindow();
		cff->OnGreitis();
		cff->OnTonas();
		largebufsizet = largebufsize;
		evsizet = evsize;
		for(int i1=0; i1<largebufsizet; i1++) largebuf[i1] = 0;
		int hr = synthesizeWholeText(tekstas, largebuf, &largebufsizet, evarr, &evsizet, greicio_koeficientas, tono_koeficientas);
		if(hr<0)
			{MessageBox("Klaida funkcijoje synthesizeWholeText");exit(EXIT_FAILURE);}
		nuoe=0;
		}
	else
		{
		nuoe=ikie;
		}

	whdr.lpData=(LPSTR)GlobalLock(hData);
	if(whdr.lpData==NULL)
		{MessageBox("Nepavyko apsaugoti atminties");exit(EXIT_FAILURE);}
	whdr.dwFlags=0;                 // papr. irasys WHDR_PREPARED ar WHDR_DONE

	ikie=nuoe+1;
	if(evsizet>0)
		{
		while((ikie < evsizet) && ((evarr[ikie].Id > 1) || (evarr[nuoe].charOffset == evarr[ikie].charOffset))) ikie++;
		if(ikie >= evsizet) ikie = evsizet-1;
		GetEditCtrl().SetSel(nuo+evarr[nuoe].charOffset,nuo+evarr[ikie].charOffset);

		memcpy(whdr.lpData, &largebuf[evarr[nuoe].signOffset], (evarr[ikie].signOffset-evarr[nuoe].signOffset)*sizeof(short));
		whdr.dwBufferLength=(evarr[ikie].signOffset-evarr[nuoe].signOffset)*sizeof(short);
		}
	else
		{
		memcpy(whdr.lpData, largebuf, 2*sizeof(short));
		whdr.dwBufferLength=2*sizeof(short);
		}

	if(waveOutPrepareHeader(hwo, &whdr, sizeof(WAVEHDR))!=MMSYSERR_NOERROR)
		{MessageBox("Nepavyko paruosti wave antrastes");exit(EXIT_FAILURE);}

	if(waveOutWrite(hwo, &whdr, sizeof(WAVEHDR))!=MMSYSERR_NOERROR)
		{MessageBox("Nepavyko isvesti wave signalo");exit(EXIT_FAILURE);}

	NeedWord=TRUE;
	}
}

//-----------------------------------------------------------------
LRESULT CRedakView::WindowProc(UINT Message, WPARAM hwo, LPARAM whdr) 
{
if(Message==MM_WOM_DONE)
 {
 if(waveOutUnprepareHeader((HWAVEOUT)hwo, (WAVEHDR*)whdr, sizeof(WAVEHDR))!=MMSYSERR_NOERROR)
 {MessageBox("Nepavyko atstatyti antraste");exit(EXIT_FAILURE);}
 
 if((GlobalUnlock(hData)!=NULL)||(GetLastError()!=NO_ERROR))
 {MessageBox("Nepavyko nuimti atminties apsaugos");exit(EXIT_FAILURE);}

 if(NeedWord==TRUE)
   {
	if(nuo>=iki) {NeedWord=FALSE;}
	else
	{
	 	if(ikie>=evsizet-1)
			{
	do
		{
		nuo+=ilg+2;
	    if(nuo>iki) nuo=iki;
	    GetEditCtrl().SetSel(nuo,iki);
	    GetSelectedText(eilute);
	    ilg=strlen(eilute);
		}
	while((ilg==0)&&(nuo<iki));
	
	GetEditCtrl().SetSel(nuo,nuo+ilg);
    GetSelectedText(Zodis);
			}
    Skaityti(Zodis);
	}
   }
 }
else
 return CEditView::WindowProc(Message, hwo, whdr);

return 0L;
}
//----------------------------------------------------------------
BOOL CRedakView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
BOOL RetVal;
if((RetVal=CWnd::Create(lpszClassName, lpszWindowName,
                            dwStyle, rect, pParentWnd, nID, pContext))==0) return 0;
else
{
WAVEFORMATEX wf;

wf.wFormatTag=WAVE_FORMAT_PCM;     // ==1
wf.nChannels=1;                    // 1-mono, 2-stereo
wf.nSamplesPerSec=22050;			// diskretizavimo daznis
wf.wBitsPerSample=16;
wf.nBlockAlign=wf.wBitsPerSample / 8 * wf.nChannels;
wf.nAvgBytesPerSec=wf.nSamplesPerSec * wf.nBlockAlign;
wf.cbSize=0;                       // papildomos inf. nereikia

if(waveOutOpen(&hwo, WAVE_MAPPER, &wf, (DWORD)m_hWnd, 0,
               WAVE_ALLOWSYNC | CALLBACK_WINDOW)!=MMSYSERR_NOERROR)
{MessageBox("Nepavyko atidaryti wave audio irenginio");exit(EXIT_FAILURE);}

return RetVal;
}	
}

//-------------------------------------------------------------------------------------------
void CRedakView::OnFileSave() 
{
	MessageBox("Vyksta áraðymas");

long ib=0;
int ie;
int n, ilg;
FILE *fr;
int hr=0;

char tekstas[10000];
tekstas[0]=0;

do
	{
	GetEditCtrl().GetSel(nuo,iki);
	GetSelectedText(eilute);
	ilg=strlen(eilute);
	if(ilg==0) ilg=1;
	nuo+=ilg;
	GetEditCtrl().SetSel(nuo,iki);

	strcat(tekstas,eilute);
}
while(nuo<iki);

if(strlen(tekstas)>0)
	{
	if(cff==NULL) cff = (CMainFrame*)GetActiveWindow();
	cff->OnGreitis();
	cff->OnTonas();
	ib=largebufsize;
	ie=evsize;
	for(int i2=0; i2<ib; i2++) largebuf[i2] = 0;
	hr = synthesizeWholeText(tekstas, largebuf, &ib, evarr, &ie, greicio_koeficientas, tono_koeficientas);

	if(hr<0)
		{MessageBox("Klaida funkcijoje synthesizeWholeText");exit(EXIT_FAILURE);}
	}

if((hr==0) && (ib>0))
	{
	if((fr = fopen("rez.wav", "wb"))==0)
		{MessageBox("Nepavyko atidaryti wav failo");exit(EXIT_FAILURE);}

	n=raw2wav(ib, fr, largebuf);
	fclose(fr);
	}
}

