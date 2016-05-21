// MyView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Bitest.h"
#include "MyView.h"
#include "MyPreviewView.h"
#include "global.h"
#include <math.h>
#include "BitestDlg.h"
// CMyView

extern LANG g_iLang;
extern INT g_nTraceYmax;
extern CursorInfo g_cursorInfo;


IMPLEMENT_DYNCREATE(CMyView, CScrollView)

CMyView::CMyView()
{
	m_nMapMode = MM_TEXT;
	m_bPrint = TRUE;
	m_bPrintCurve = TRUE;
	m_bPrintHead = TRUE;
	m_stPrintDetail.curSelection = 0;
	m_stPrintDetail.totalCurveNumber = 0;
	InitPrintText();
}

CMyView::~CMyView()
{
	//����ɾ������
	for(INT i = 0; i < 3; i++)
	{
		m_stPrintDetail.stTestResultInfo[i].EventList.vEventInfo.clear();
	}
}

BEGIN_MESSAGE_MAP(CMyView, CScrollView)
//	ON_COMMAND(ID_PRINT, &CMyView::OnPrint)
END_MESSAGE_MAP()


// CMyView ��ͼ

void CMyView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CMyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���

	DrawPic(pDC);
}

// CMyView ���

#ifdef _DEBUG
void CMyView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyView ��Ϣ�������
void CMyView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: �ڴ����ר�ô����/����û���
	pDC->SetMapMode(MM_ANISOTROPIC); 
	//ת������ӳ�䷽ʽ 
	CSize   size=CSize(800,   560); 
	pDC->SetWindowExt(size);             //ȷ�����ڴ�С 
	//�õ�ʵ���豸ÿ�߼�Ӣ����������� 
	INT   xLogPixPerInch=pDC->GetDeviceCaps(LOGPIXELSX);   
	INT   yLogPixPerInch=pDC->GetDeviceCaps(LOGPIXELSY); 
	//�õ��豸������߼�����ı��� 
	long   xExt=(long)size.cx*xLogPixPerInch/96; 
	long   yExt=(long)size.cy*yLogPixPerInch/96; 
	pDC->SetViewportExt(xExt,yExt);
	CScrollView::OnPrepareDC(pDC, pInfo);
}

BOOL CALLBACK _AfxMyPreviewCloseProc(CFrameWnd* pFrameWnd)
{
	ASSERT_VALID(pFrameWnd);
	CMyPreviewView* pView = (CMyPreviewView*) pFrameWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT_KINDOF(CPreviewView, pView);
	pView->OnPreviewClose();
	return FALSE;
}

void CMyView::OnFilePrintPreview(void)
{
	CPrintPreviewState* pState = new CPrintPreviewState;
	pState->lpfnCloseProc =_AfxMyPreviewCloseProc;//���ô�ӡԤ�����ڹر�ʱ�ĵ��ú�����
	if(!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this,RUNTIME_CLASS(CMyPreviewView), pState))
	{
		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(_T("really??"));
		delete pState;
	}
}

void CMyView::DrawPic(CDC* pDC)//�Զ��廭ͼ���� 
{
	//CBrush ColorBrush(RGB(100,100,0));
//	pDC->SelectObject(&ColorBrush);
	//pDC->Ellipse(0,0,1000,1000);
}

void CMyView::OnMyPrint()
{
	GetParent()->ShowWindow(SW_SHOWMINIMIZED);
	m_bPrint = TRUE;
	CView::OnFilePrint();
}

void CMyView::OnMyPrintPreview()
{
	GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
	m_bPrint=FALSE;
	CView::OnFilePrintPreview();
}

BOOL CMyView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	//��ӡ�������������򣬴�ӡ��������ú�� lzy2014.8.28
	//INT nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);	
	LPDEVMODE lpDevMode;
	PRINTDLG pd = pInfo->m_pPD->m_pd;
	if(AfxGetApp()->GetPrinterDeviceDefaults(&pd))
	{
		lpDevMode = (LPDEVMODE)GlobalLock(pd.hDevMode);
		if(lpDevMode)
		{
			lpDevMode->dmPaperSize = DMPAPER_A4;
			if((!m_nCurveNumber ) || (2 == m_nCurveNumber) || m_bPrintCurve ) //��������,4������
			{
				lpDevMode->dmOrientation = DMORIENT_PORTRAIT; //�����ӡ
			}
			else  //�������
			{
				lpDevMode->dmOrientation = DMORIENT_LANDSCAPE; //�����ӡ
			}			
		}
		GlobalUnlock(pd.hDevMode);
	}	
	
	if(DoPreparePrinting(pInfo))
		return TRUE;
	else
	{
     //   GetParent()->DestroyWindow();
		return FALSE;
	}
}

void CMyView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnBeginPrinting(pDC, pInfo);
	INT iEventNum = 0;
	CMyFrame *pFrame =(CMyFrame *)GetParent();
	//pFrame->m_pMainDlg->SendMessage(WM_BEGIN_PRINTING,(WPARAM) pDC, (LPARAM) pInfo);

	InitFont(pDC);

	TEXTMETRIC tm;
	CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
	pDC->GetTextMetrics (&tm);
	m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
	CSize size = pDC->GetTextExtent (_T ("---------1---------2---------3---------4---------5---------6---------7---------8---"), 81);
	pDC->SelectObject (pOldFont);
	m_cxWidth = size.cx;

	m_nLinesPerPage = (pDC->GetDeviceCaps (VERTRES) -
		(m_cyPrinter * (3 + (2 * PRINTMARGIN)))) / m_cyPrinter;

	if(m_stPrintDetail.stTestResultInfo[0].DataPointsNum>0&&m_stPrintDetail.stTestResultInfo[1].DataPointsNum>0)
	{
        GetEvent2Way(&m_stPrintDetail.stTestResultInfo[2], &m_stPrintDetail.stTestResultInfo[0], &m_stPrintDetail.stTestResultInfo[1]);
		iEventNum = m_stPrintDetail.stTestResultInfo[2].EventList.vEventInfo.size();
	}
	else if(m_stPrintDetail.stTestResultInfo[0].DataPointsNum>0&&m_stPrintDetail.stTestResultInfo[1].DataPointsNum<=0)
	    iEventNum = m_stPrintDetail.stTestResultInfo[0].EventList.vEventInfo.size();
	else
	    iEventNum = m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo.size();

	INT maxPage=iEventNum>EVENT_LIST_PRINT_MAX?2:1;//����ҳ��

	pInfo->SetMaxPage (maxPage);


	m_cxOffset = (pDC->GetDeviceCaps (HORZRES) - size.cx) / 2;
	//���ô�ӡ����
	m_rectDraw.SetRect(0,0,pDC->GetDeviceCaps (HORZRES), pDC->GetDeviceCaps (VERTRES));
}

void CMyView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnEndPrinting(pDC, pInfo);

	CMyFrame *pFrame =(CMyFrame *)GetParent();
	//pFrame->m_pMainDlg->SendMessage(WM_END_PRINTING,(WPARAM) pDC, (LPARAM) pInfo);
	//ghq 2016.5.3 print end but not close maindlg
	//if(m_bPrint)		//print not preview
	//	GetParent()->DestroyWindow();

	if(m_fontPrinter.m_hObject!=NULL)
		m_fontPrinter.DeleteObject ();

	if(m_dataFont.m_hObject!=NULL)
		m_dataFont.DeleteObject();

	if(m_FontVR.m_hObject!=NULL)
		m_FontVR.DeleteObject();
}

void CMyView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CMyFrame *pFrame =(CMyFrame *)GetParent();
	//pFrame->m_pMainDlg->SendMessage(WM_MY_PRINT,(WPARAM) pDC, (LPARAM) pInfo) ;
	if(m_bPrintHead)
		PrintPageHeader(pDC,pInfo->m_nCurPage,  pInfo);
	InnerDraw(pDC,pInfo->m_nCurPage);
}

void CMyView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);

	GetParent()->DestroyWindow();
	//�ָ��������
	//MAIN->ShowReport(TRUE);
	//��ӡ
	//OnMyPrint();//zll 2013.6.26
}

void CMyView::PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo)
{
	CFont *pOldFont;
	CFont fontHeader;
	fontHeader.CreatePointFont(100,_T("FixedSys"),pDC);//
	//fontHeader.CreatePointFont(100,_T("����"),pDC);//
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader));
	INT nPosY,nPosX;
	nPosY=m_cyPrinter;
	nPosX=m_cxOffset;
	pDC->TextOut(nPosX,nPosY,CString(_T("")));
	//CString str;
	//str.Format(_T("�� %d ҳ / �� %d ҳ"),nPageNumber,pInfo->GetMaxPage());
	//CSize size=pDC->GetTextExtent(str);
	//nPosX=m_cxOffset+m_cxWidth-size.cx;
	//pDC->TextOut(nPosX,nPosY,str);
	//pDC->MoveTo(m_cxOffset,1.5*m_cyPrinter);
	//pDC->LineTo(m_cxOffset+m_cxWidth,1.5*m_cyPrinter);
	pDC->SelectObject(pOldFont);
	fontHeader.DeleteObject();
}

//�±����õĺ��������������ݡ�lzy 2014.8.11
void CMyView::DrawPrintContent(CDC* pDC, CRect rectDraw, INT nCurSel, BOOL bSingle)
{
	//����
	INT nXoffset = rectDraw.left;
	INT nYoffset = rectDraw.top;
	INT nVspace = rectDraw.Height() / 64;
	INT nCurHeight = nYoffset+ nVspace * 3;
	INT nHleftspace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	INT nHmidspace = rectDraw.Width() / 20;
	INT nHinterval = nVspace / 4;   //���ο�ֱ���
	INT nHMargin = nHleftspace / 2;   //���ο����������Ե�ľ���
	INT nHSubMargin = nHleftspace / 2;  //����Ŀ��������
	INT n1 = nXoffset + nHleftspace + nHMargin;  //��ߵ�һ�����ʼX����
	INT n2 = n1 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //�м������ʼX����
	INT n3 = n2 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //���ұ������ʼX����
	INT nBottom = 0;   //������ο���±�������
	CRect textRect;
	CString strText , strTemp;	

	//����	
	CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
	CSize sizeText = pDC->GetTextExtent(m_stPrintDetail.strTitle);
	textRect.SetRect(nXoffset + rectDraw.Width() / 2 - sizeText.cx / 2, nCurHeight-nVspace ,nXoffset + rectDraw.Width() / 2 + sizeText.cx / 2 ,  nCurHeight-nVspace + sizeText.cy );
	pDC->DrawText(m_stPrintDetail.strTitle, textRect, DT_CENTER);  //д����̧ͷ
	nBottom = textRect.bottom;
	
	//����д��һ��	
	{
		pDC->SelectObject (&m_dataFont);
		nCurHeight = nBottom + nVspace; 		
		sizeText = pDC->GetTextExtent(m_stPrintDetail.strLogo);
		nBottom = nCurHeight + sizeText.cy;
		textRect.SetRect(nXoffset + nHleftspace, nCurHeight, nXoffset + nHleftspace + sizeText.cx, nCurHeight + sizeText.cy);
		if(m_bPrintLogo)
			pDC->DrawText(m_stPrintDetail.strLogo, textRect, DT_LEFT);  //д����汾����ά��Ȩ
		CTime tm = CTime::GetCurrentTime();
		CString strCurTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
		sizeText = pDC->GetTextExtent(strCurTime);
		textRect.SetRect(nXoffset + rectDraw.Width() - nHleftspace - sizeText.cx ,nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nCurHeight + sizeText.cy);
		pDC->DrawText(strCurTime, textRect, DT_LEFT);  //д��ǰ��ӡ��ʱ��
	}
	
	TEXTMETRIC  tm;
	pDC->GetTextMetrics(&tm);
	INT nCharWidth = tm.tmMaxCharWidth;  //����ַ����
	strText = _T("Test char height ��������");
	sizeText = pDC->GetTextExtent(strText);
	INT nCharHeight = sizeText.cy;    //��������ĸ߶�
	INT nLineInterval = nCharHeight / 2 ;  //0.5���о�
    //����д������Ϣ����
	if(m_bPrintQuest)
	{
		DrawQuestInfo(pDC,rectDraw,nBottom,nCurSel);
	}
	
    //���»�������Ϣ
	if(m_bPrintConfig)
	{
		DrawConfig(pDC,rectDraw,nBottom,nCurSel);
#if 0
		nCurHeight = nBottom + nVspace / 2;	
		nBottom = nCurHeight + nCharHeight * 7 + nLineInterval * 8;
		pDC->Rectangle(nXoffset + nHleftspace, nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nBottom);
		pDC->SelectObject(&m_fontGroup);
		//strText = _T(" Configuration info ");
		strText = _T(" ") + m_stPrintDetail.stPrintConfig.strTitle + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy / 2, nXoffset +(rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy / 2);
		pDC->DrawText(strText, textRect, DT_LEFT);
		pDC->SelectObject(&m_dataFont);	
		nCurHeight += nLineInterval;
		//strText = _T("Wavelength(nm): ") + m_stPrintDetail.stPrintConfig.Wavelength;  //дwavelength
		strText = m_stPrintDetail.stPrintConfig.strWavelength + _T("(nm): ") + m_stPrintDetail.stPrintConfig.Wavelength;  //дwavelength
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);		
		//strText = _T("Range(m): ") + m_stPrintDetail.stPrintConfig.Range;  //дRange
		strText = m_stPrintDetail.stPrintConfig.strRange + _T("(m): ") + m_stPrintDetail.stPrintConfig.Range;  //дRange
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 , nCurHeight, n2 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Backscatter coeff(dB): ") + m_stPrintDetail.stPrintConfig.BackScatter;  //дBackscatter coeff 3
		strText = m_stPrintDetail.stPrintConfig.strBackScatter + _T("(dB): ") + m_stPrintDetail.stPrintConfig.BackScatter;  //дBackscatter coeff 3
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Pulse(ns): ") + m_stPrintDetail.stPrintConfig.Pulse;  //дPulse
		strText = m_stPrintDetail.stPrintConfig.strPulse + _T("(ns): ") + m_stPrintDetail.stPrintConfig.Pulse;//дPULSE
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Average: ") + m_stPrintDetail.stPrintConfig.Average;  //дAverage counts
		strText = m_stPrintDetail.stPrintConfig.strAverage + _T(": ") + m_stPrintDetail.stPrintConfig.Average;  //дAverage counts  
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 , nCurHeight, n2 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Loss threshold(dB): ") + m_stPrintDetail.stPrintConfig.LossThreshold;  //дLossThreshold 3
		strText = m_stPrintDetail.stPrintConfig.strLossThreshold + _T("(dB): ") + m_stPrintDetail.stPrintConfig.LossThreshold;  //дLossThreshold 3
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		
		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("ORIGIN");  //дORIGIN 
		strText = m_stPrintDetail.stPrintConfig.strOrigin;//д��ʼ��
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
        //strText = _T("END");  //дEND 2
		strText = m_stPrintDetail.stPrintConfig.strEnd;  //д������
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2, nCurHeight, n2 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Reflection threshold(dB): ") + m_stPrintDetail.stPrintConfig.RefThreshold;  //дReflection threshold
		strText = m_stPrintDetail.stPrintConfig.strRefThreshold + _T("(dB): ") + m_stPrintDetail.stPrintConfig.RefThreshold;  //дReflection threshold
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Origin location: ") + m_stPrintDetail.stPrintConfig.OriLocation;  //дOrigin location 1
		strText = m_stPrintDetail.stPrintConfig.strOriLocation + _T(": ") + m_stPrintDetail.stPrintConfig.OriLocation;  //дOrigin location 1
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 + nHSubMargin, nCurHeight, n1 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End location: ") + m_stPrintDetail.stPrintConfig.EndLocation;  //д End location 2
		strText = m_stPrintDetail.stPrintConfig.strEndLocation + _T(": ") + m_stPrintDetail.stPrintConfig.EndLocation;  //д End location 2
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 + nHSubMargin , nCurHeight, n2 + nHSubMargin + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End threshold(dB): ") + m_stPrintDetail.stPrintConfig.EndThreshold;  //дFiber end threshold
		strText = m_stPrintDetail.stPrintConfig.strEndThreshold + _T("(dB): ") + m_stPrintDetail.stPrintConfig.EndThreshold;  //дFiber end threshold
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Origin cable: ") + m_stPrintDetail.stPrintConfig.OriCable;  //дOrigin Cable
		strText = m_stPrintDetail.stPrintConfig.strOriCable + _T(": ") + m_stPrintDetail.stPrintConfig.OriCable;  //дOrigin Cable
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 + nHSubMargin, nCurHeight, n1 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End cable: ") + m_stPrintDetail.stPrintConfig.EndCable;  //дEnd Cable
		strText = m_stPrintDetail.stPrintConfig.strEndCable + _T(": ") + m_stPrintDetail.stPrintConfig.EndCable;  //дEnd Cable
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 + nHSubMargin , nCurHeight, n2 + nHSubMargin + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Index: ") + m_stPrintDetail.stPrintConfig.Index;  //д ������
		strText = m_stPrintDetail.stPrintConfig.strIndex + _T(": ") + m_stPrintDetail.stPrintConfig.Index;  //д ������
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3, nCurHeight, n3 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Origin fiber: ") + m_stPrintDetail.stPrintConfig.OriFiber;  //дOrigin Fiber
		strText = m_stPrintDetail.stPrintConfig.strOriFiber + _T(": ") + m_stPrintDetail.stPrintConfig.OriFiber;  //дOrigin Fiber
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 + nHSubMargin, nCurHeight, n1 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End fiber: ") + m_stPrintDetail.stPrintConfig.EndFiber;  //д End Fiber
		strText = m_stPrintDetail.stPrintConfig.strEndFiber + _T(": ") + m_stPrintDetail.stPrintConfig.EndFiber;  //д End Fiber
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 + nHSubMargin, nCurHeight, n2 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Origin color: ") + m_stPrintDetail.stPrintConfig.OriColor;  //дOrigin Color
		strText = m_stPrintDetail.stPrintConfig.strOriColor + _T(": ") + m_stPrintDetail.stPrintConfig.OriColor;  //дOrigin Color		
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 + nHSubMargin, nCurHeight, n1 + nHSubMargin + sizeText.cx, nCurHeight +sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End color: ") + m_stPrintDetail.stPrintConfig.EndColor;  //д End color
		strText = m_stPrintDetail.stPrintConfig.strEndColor + _T(": ") + m_stPrintDetail.stPrintConfig.EndColor;  //д End color		
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 + nHSubMargin, nCurHeight, n2 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);		
#endif
	}

#if 1
	//���»�Comment
	if(m_bPrintComment && bSingle)
	{
		//TEXTMETRIC  tm;
		//pDC->GetTextMetrics(&tm);
		INT cxChar = tm.tmAveCharWidth * 1.5;
		INT cyChar = tm.tmHeight;
		INT nOneLine = (rectDraw.Width() - nHleftspace * 2 - nHMargin * 2) / cxChar;  //ÿ�ж����ַ�
		INT nchars = m_stPrintDetail.stPrintQuest.Comment.GetLength();
		sizeText = pDC->GetTextExtent( m_stPrintDetail.stPrintQuest.Comment);
		INT nLines = sizeText.cx / (rectDraw.Width() - n1 * 2 );  //���ж�����
		INT nRemain = sizeText.cx % (rectDraw.Width() - n1 * 2 ); //����
		nLines = nRemain > 0 ? ++nLines : nLines;
		
		nCurHeight = nBottom + nVspace / 2;
		nBottom = nCurHeight + nCharHeight * nLines + nLineInterval * (nLines + 1);
		pDC->Rectangle(nXoffset + nHleftspace, nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nBottom);
		pDC->SelectObject(&m_fontGroup);
		//strText = _T(" Comment ");
		strText = _T(" ") + m_stPrintDetail.stPrintQuest.strComment + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy / 2, nXoffset + (rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy / 2);
		pDC->DrawText(strText, textRect, DT_LEFT);
		pDC->SelectObject(&m_dataFont);
		strTemp = m_stPrintDetail.stPrintQuest.Comment;
		INT nCharCount = 0; //��ʾ���ַ��ĸ���
		strText.Empty();
		for(INT i=0; i<nchars; i++)
		{
			strText += strTemp.GetAt(i);
			sizeText = pDC->GetTextExtent(strText);
			if((sizeText.cx >= rectDraw.Width() - (n1-nXoffset) * 2 ) || i == nchars - 1)  //дһ��
			{
				nCurHeight += nLineInterval;
                textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight  + sizeText.cy);
                pDC->DrawText(strText, textRect, DT_LEFT); 
                nCurHeight += nCharHeight;
				strText.Empty();
			}
		}
    }
#endif

	//����
	if(m_bPrintTrace)
	{
		nCurHeight = nBottom + nVspace / 2;	
		nBottom = nCurHeight + nVspace * 24;
		pDC->SelectObject(&m_fontGroup);
		//strText = _T(" Trace ");
		strText = _T(" ") + m_stPrintDetail.stPrintTrace.strTitle + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		pDC->Rectangle(nXoffset + nHleftspace, nCurHeight , nXoffset + rectDraw.Width() - nHleftspace, nBottom);
		textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy /2, nXoffset + (rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy /2);
		pDC->DrawText(strText, textRect, DT_LEFT);
		pDC->SelectObject(&m_dataFont);
		
		//�����
		nCurHeight += nHMargin + nLineInterval;
		CRect curveRect;
		curveRect.SetRect(nXoffset + nHleftspace*1.5 + nHmidspace/2, nCurHeight, nXoffset + rectDraw.Width() - 1.5 * nHleftspace - nHmidspace/2, nBottom - nVspace * 1.5);
		DrawTrace(pDC, curveRect, nCurSel);
	}

#if 0
	//���߲���	
	if(m_bPrintTrace && bSingle)
	{
		pDC->SelectObject(&m_dataFont);
		strText = _T("A: ") + m_stPrintDetail.stPrintTrace.LocationA; // + _T(" m");  //дA��λ��
		sizeText = pDC->GetTextExtent(strText);
		nCurHeight = nBottom + nVspace / 4;
		nBottom = nCurHeight + nLineInterval * 4  + nCharHeight * 3 ;		
		pDC->Rectangle(nXoffset + nHleftspace, nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nBottom);        
		
		nCurHeight += nLineInterval;
		textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		strText = _T("B: ") + m_stPrintDetail.stPrintTrace.LocationB; //+ _T(" m");;  //дB��λ��
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 , nCurHeight, n2 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		strText = _T("A-B: ") + m_stPrintDetail.stPrintTrace.DistanceAB + _T("km");;  //дAB�����
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		nCurHeight = textRect.bottom + nLineInterval;
		strText = _T("    ") + m_stPrintDetail.stPrintTrace.LossA; //+ _T(" dB");;  //дA�����
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 , nCurHeight, n1 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		strText = _T("    ") + m_stPrintDetail.stPrintTrace.LossB; //+ _T(" dB");;  //дB�����
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 , nCurHeight, n2 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		strText = _T("        ") + m_stPrintDetail.stPrintTrace.LossAB + _T("dB");;  //дAB����Ĳ�
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		nCurHeight = textRect.bottom + nLineInterval;		
		strText = _T("        ") + m_stPrintDetail.stPrintTrace.SlopeAB + _T("dB/km");;  //дAB�����б��
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
	}
#endif

	//�¼��б�
	if(m_bPrintEvent)
	{
		nCurHeight = nBottom + nVspace / 2;
		//�����¼��б��ͷ
		DrawEventHead(pDC, nXoffset + nHleftspace, nCurHeight + nLineInterval, rectDraw,nCurSel);
		//�����¼���
		if(nCurSel != SHOW_ALL) //Ϊ�����ֵ�����������ߣ��˴���Ϊ�����Զ������߱������ʾ�¼���lzy2014.8.30
			DrawEventList(pDC, nXoffset + nHleftspace, nCurHeight + nLineInterval, nLineInterval , rectDraw, m_stPrintDetail.curSelection);
		else
			DrawEventList2Way(pDC, nXoffset + nHleftspace, nCurHeight + nLineInterval, nLineInterval , rectDraw, nCurSel);

        
		//�����ο�
		pDC->SelectObject(&m_dataFont);
		//strText = _T(" Event info ");
		strText = _T(" ") + m_stPrintDetail.stPrintEvent.strTitle + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		nBottom =  nCurHeight + (m_stPrintDetail.stPrintEvent.EventCount + 1) * (nLineInterval + nCharHeight) + nLineInterval;
		textRect.SetRect(nXoffset + nHleftspace , nCurHeight,nXoffset + rectDraw.Width() - nHleftspace, nBottom);
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CBrush *pOldBrush = pDC->SelectObject(pBrush);
		pDC->Rectangle(textRect);  //�����ο�
		pDC->SelectObject(pOldBrush);
        
		//дtitle
		pDC->SelectObject(&m_fontGroup);
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy /2, nXoffset+(rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy /2);
		pDC->DrawText(strText, textRect, DT_LEFT);

		//������
		for(INT i = 1; i<m_stPrintDetail.stPrintEvent.EventCount + 1; i++)
		{		
			pDC->MoveTo(nXoffset + nHleftspace , nCurHeight + nLineInterval / 2 + i * (nLineInterval + nCharHeight));
			pDC->LineTo(rectDraw.right - nHleftspace, nCurHeight + nLineInterval / 2 + i * (nLineInterval + nCharHeight));
		}
	}	

	pDC->SelectObject (pOldFont);
}

//���ƴ��������ߣ�����AB���� lzy2014.8.26
void CMyView::DrawTrace(CDC *pDC, CRect rc, INT nCurSel, float fontRatio)
{	
	float fLength=0;
	float fOffsetX=0;

	InitFont(pDC);  
	pDC->SelectObject(&m_dataFont);
	pDC->Rectangle(&rc);//�����

	DrawGridLine(pDC, rc);//����դ�� 

	if(nCurSel!=SHOW_ALL)
	{
        fLength=m_stPrintDetail.stTestResultInfo->Length;
	    DrawScale(pDC, rc, 0, fLength);//���ƿ̶�
	    DrawCurve(pDC, rc, nCurSel);//��������
		DrawEvent(pDC, rc, nCurSel);//�����¼�
	}
	else
	{
  	    //��ȡ˫����Ե��¼�
	    //GetEvent2Way(&m_stPrintDetail.stTestResultInfo[2], &m_stPrintDetail.stTestResultInfo[0], &m_stPrintDetail.stTestResultInfo[1]);
		DrawCurve2Way(pDC, rc, nCurSel);
        fLength=m_stPrintDetail.stTestResultInfo[2].Length;
		fOffsetX=m_stPrintDetail.stTestResultInfo[2].YOffset;	
	    DrawScale2Way(pDC, rc, fOffsetX, fLength);
	}
	//���ƹ��AB
	DrawCursorAB(pDC, rc);  //lzy2014.7.23
}

//��ʼ������ lzy2014.9.3
void CMyView::InitFont(CDC *pDC)
{
	m_fontPrinter.DeleteObject();
	m_dataFont.DeleteObject();
	m_fontGroup.DeleteObject();
	m_FontVR.DeleteObject();

	//��ȡ��ӡ�߼��ߴ� lzy2014.8.15
	INT nLogicX = pDC->GetDeviceCaps(LOGPIXELSX);
	INT nLogicY = pDC->GetDeviceCaps(LOGPIXELSY);
	//��ȡ�����ӡ�ߴ�
	INT nPhysicalX = pDC->GetDeviceCaps(PHYSICALWIDTH);
	INT nPhysicalY = pDC->GetDeviceCaps(PHYSICALHEIGHT);
	//CString str;
	//str.Format(L"lx = %d;ly=%d;px=%d;py=%d", nLogicX, nLogicY, nPhysicalX, nPhysicalY);
	//	AfxMessageBox(str);
	float fScale = nLogicX / 300.0;
	INT nCurveNumber = _wtoi(m_stPrintDetail.WaveList);  //��ǰѡ��ÿҳ��ӡ��������
	if(nCurveNumber > 4 || nCurveNumber < 0 || m_bPrintCurve)
		nCurveNumber = 0;

	if(m_fontPrinter.m_hObject==NULL)
	{
		m_fontPrinter.CreateFont(54 * fScale * FONTSIZE_RATIO[nCurveNumber], 0, 0, 0, FW_BOLD,//��������2700
			0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH,_T("Arial")); 
	}
	
	if(m_dataFont.m_hObject==NULL)
	{
		m_dataFont.CreateFont(36 * fScale * FONTSIZE_RATIO[nCurveNumber], 0, 0, 0, FW_NORMAL,//��������2700
			0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH,_T("Arial")); 
	}
	//m_dataFont.CreatePointFont(80,_T("����"),pDC);
	if(m_fontGroup.m_hObject == NULL)
	{
		m_fontGroup.CreateFont(36 * fScale * FONTSIZE_RATIO[nCurveNumber], 0, 0, 0, FW_BOLD,//��������2700
			0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH,_T("Arial")); 
	}
	//m_fontGroup.CreatePointFont(70,_T("����"),pDC);
	if(m_FontVR.m_hObject==NULL)
		m_FontVR.CreateFont(32 * fScale * FONTSIZE_RATIO[nCurveNumber], 0, 900, 900, FW_NORMAL,//��������2700
		0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH,_T("Arial"));	
}

//���ƿ̶�lzy2014.7.17
void CMyView::DrawScale(CDC *pDC, CRect &rect, float fYmax, float fXmax)
{
	if (m_stPrintDetail.stWaveConfig.ShowGrid)
	{
		//float flo_Length = fXmax;
		INT ndx = rect.Width () / COOR_SPLIT_COUNT_H;
		INT ndy = rect.Height () / COOR_SPLIT_COUNT_V;
		CString strText;
		CSize sizeText;
		CRect rectText;

		INT iYmin = 0;//M_FMAXDB0;
		INT iYmax = M_FMAXDB0 - M_FSTARTDB0;
		INT itemp;
		INT bottom = rect.bottom;
		INT left = rect.left;
		INT iOffset = min(10, rect.Height() / 20);
				
		//������̶�
		rectText.top = rect.bottom + iOffset;		
		for ( INT i = 0; i< COOR_SPLIT_COUNT_H; i++)
		{			
			itemp = left + ndx * i;
			strText.Format(_T("%.1f"), 0.001 * fXmax * i / COOR_SPLIT_COUNT_H);
			sizeText = pDC->GetTextExtent(strText);//�õ��ı������
			rectText.left = itemp - sizeText.cx / 2;
			rectText.right = rectText.left + sizeText.cx;
			rectText.bottom = rectText.top + sizeText.cy;			
			pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);			
		}
		strText = _T("km");
		sizeText = pDC->GetTextExtent(strText);//�õ��ı������
		rectText.right = rect.right - 10;
		rectText.left = rectText.right - sizeText.cx;		
		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);

		//������̶�
		//pDC->SetTextAlign(TA_LEFT | TA_TOP);
		strText = _T("0000");
		sizeText = pDC->GetTextExtent(strText);//�õ��ı������
		INT iText = sizeText.cx;  //���ֿ��
		for (INT i=1; i<COOR_SPLIT_COUNT_V; i++)
		{
			//��Y��̶�
			if(iYmax >= 0 && iYmax <= 100)
			{
				rectText.left = rect.left - iText - 5;		

				for(INT i = 0; i < COOR_SPLIT_COUNT_V; i++)
				{
					itemp = bottom - ndy * i;
					strText.Format(_T("%.1f"), (M_FMAXDB0 - M_FSTARTDB0) * i / COUNT_GRIDLINE_VERTICAL);
					sizeText = pDC->GetTextExtent(strText);//�õ��ı������
					rectText.top = itemp - sizeText.cy / 2;
					rectText.bottom = rectText.top + sizeText.cy;
					rectText.right = rectText.left + sizeText.cx;
					pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
				}

				strText = _T("dB");
				sizeText = pDC->GetTextExtent(strText);//�õ��ı������
				rectText.top = rect.top + 5;
				rectText.bottom = rectText.top + sizeText.cy;
				rectText.right = rectText.left + sizeText.cx;
				pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
			}
		}
	}
}

//ghq.2016.4.26
void CMyView::DrawScale2Way(CDC *pDC, CRect &rect, float fXmin, float fXmax)
{
	if (m_stPrintDetail.stWaveConfig.ShowGrid)
	{
		//float flo_Length = fXmax;
		INT ndx = rect.Width () / COOR_SPLIT_COUNT_H;
		INT ndy = rect.Height () / COOR_SPLIT_COUNT_V;
		CString strText;
		CSize sizeText;
		CRect rectText;

		INT iYmin = 0;//M_FMAXDB0;
		INT iYmax = M_FMAXDB0 - M_FSTARTDB0;
		INT itemp;
		INT bottom = rect.bottom;
		INT left = rect.left;
		INT iOffset = min(10, rect.Height() / 20);
				
		//������̶�
		rectText.top = rect.bottom + iOffset;		
		for ( INT i = 0; i< COOR_SPLIT_COUNT_H; i++)
		{			
			itemp = left + ndx * i;
			strText.Format(_T("%.1f"), 0.001 * fXmax * i / COOR_SPLIT_COUNT_H -0.001 * fXmin);
			sizeText = pDC->GetTextExtent(strText);//�õ��ı������
			rectText.left = itemp - sizeText.cx / 2;
			rectText.right = rectText.left + sizeText.cx;
			rectText.bottom = rectText.top + sizeText.cy;			
			pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);			
		}
		strText = _T("km");
		sizeText = pDC->GetTextExtent(strText);//�õ��ı������
		rectText.right = rect.right - 10;
		rectText.left = rectText.right - sizeText.cx;		
		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);

		//������̶�
		//pDC->SetTextAlign(TA_LEFT | TA_TOP);
		strText = _T("0000");
		sizeText = pDC->GetTextExtent(strText);//�õ��ı������
		INT iText = sizeText.cx;  //���ֿ��
		for (INT i=1; i<COOR_SPLIT_COUNT_V; i++)
		{
			//��Y��̶�
			if(iYmax >= 0 && iYmax <= 100)
			{
				rectText.left = rect.left - iText - 5;		

				for(INT i = 0; i < COOR_SPLIT_COUNT_V; i++)
				{
					itemp = bottom - ndy * i;
					strText.Format(_T("%.1f"), (M_FMAXDB0 - M_FSTARTDB0) * i / COUNT_GRIDLINE_VERTICAL);
					sizeText = pDC->GetTextExtent(strText);//�õ��ı������
					rectText.top = itemp - sizeText.cy / 2;
					rectText.bottom = rectText.top + sizeText.cy;
					rectText.right = rectText.left + sizeText.cx;
					pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
				}

				strText = _T("dB");
				sizeText = pDC->GetTextExtent(strText);//�õ��ı������
				rectText.top = rect.top + 5;
				rectText.bottom = rectText.top + sizeText.cy;
				rectText.right = rectText.left + sizeText.cx;
				pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
			}
		}
	}
}

void CMyView::DrawEventHead(CDC* pDC, INT nLeft, INT nTop, CRect rectDraw,INT nCurSel)//�����¼��б�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	//���Ʊ�����
	LV_COLUMN lvColumn;
	TCHAR uin[50];
	lvColumn.pszText=uin;
	lvColumn.cchTextMax=50;
	lvColumn.mask=LVCF_TEXT;
	CString header=_T("");
	INT nLen = 0;//�õ�����
	INT nVspace = 0;
	INT nLeftSpace = 0;
	INT nHspace = 0;  //lzy2014.8.12
	CRect textRect;

	if(nCurSel == SHOW_ALL)
	{
		nLen = EVENT_LIST_COL;//�õ�����
	}
	else
	{
		nLen = EVENT_LIST_COL-1;//�õ�����
	}

	m_stPrintDetail.stPrintEvent.EventColumn = nLen;   //��������
	nVspace = rectDraw.Height() / 42;
	nLeftSpace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	nHspace = ( rectDraw.Width() - 2 * nLeftSpace)  / nLen;  //lzy2014.8.12
	for(INT i=0;i<nLen;i++)
	{
		textRect.left = nLeft + i * nHspace;
		textRect.top = nTop;
		textRect.right = nLeft + (i+1) * nHspace;
		textRect.bottom = nTop + nVspace;
		//if(m_dlgParent->m_pEventFormView->m_Eventlist.GetColumn(i,&lvColumn))

		if(nCurSel == SHOW_ALL)
		    header=strListHead2Way[i];
		else
			header=strListHead[i];
		pDC->DrawText(header, &textRect,DT_CENTER);//����
		TRACE(_T("left=%d;right=%d;offset=%d\n"), textRect.left, textRect.right, nLeft);
	}
	pDC->SelectObject (pOldFont);
}

/****************************************************************
��������:             DrawEventList()                                                                               
����˵��:             �������¼��б���                                                                             
��ڲ���:             nLeft ����㣬 nTop �����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-24                                                                  
Write By:             XJF                                                                                          
****************************************************************/
void CMyView::DrawEventList(CDC* pDC, INT nLeft, INT nTop , INT nVspace , CRect rectDraw, INT nCurSel)//�����¼��б�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	INT nLeftSpace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	INT nHspace = (rectDraw.Width() - 2 * nLeftSpace) / (EVENT_LIST_COL-1);  //lzy2014.8.12
	INT nStartLine = 1;

	float preDistance=0;

	//TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].stTestResultInfo;
	TestResultInfo* pData = &m_stPrintDetail.stTestResultInfo[nCurSel];  //lzy2014.8.30

	if (NULL == pData)
		return;

	CString strText=_T("Test Height");
	CSize sizeText = pDC->GetTextExtent(strText);
	INT nCharHeight = sizeText.cy;  //��ȡ����߶�
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//�ٶ�??
	INT nSize=0;
	    
	if(ve.size()>EVENT_LIST_PRINT_MAX)
		nSize = EVENT_LIST_PRINT_MAX;
	else
		nSize = ve.size();

	m_stPrintDetail.stPrintEvent.EventCount = nSize;   //��������
	for (INT i=0;i<nSize;i++)//
	{
		// ���
		{
			textRect.left = nLeft;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace; lzy 2014.8.15

			//����
			strText.Format(_T("%d"),i+1);
			if (g_iLang == CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" �Ƿ���");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" ����");;//1�������¼�����
				}
			}
			else if (g_iLang == ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1�������¼�����
				}
			}

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// �¼�����
		//{
		//	textRect.left = nLeft + nHspace;
		//	textRect.top = nTop + nStartLine * nVspace;
		//	textRect.right = nLeft + 2 * nHspace;
		//	textRect.bottom = nTop + (nStartLine+1) * nVspace;

		//	//����
		//	if (0 == ve[i].nEventType)
		//	{
		//		strText = _T("��ʼ�¼�");
		//	}
		//	else if (1 == ve[i].nEventType)
		//	{
		//		strText = _T("�����¼�");
		//	}
		//	else if (2 == ve[i].nEventType)
		//	{
		//		strText = _T("�Ƿ����¼�");
		//	}
		//	else if (3 == ve[i].nEventType)
		//	{
		//		strText = _T("�����¼�");
		//	}
		//	else if (4 == ve[i].nEventType)
		//	{
		//		strText = _T("�����¼�");
		//	}
		//	else
		//	{
		//		strText = _T("--");
		//	}
		//	pDC->DrawText(strText, &textRect,DT_CENTER);//����
		//}
		// ����
		{
			textRect.left = nLeft + nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 2 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//����
			}
			else
			{
				//if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ��
		{
			textRect.left = nLeft + 2 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 3 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			float a,b=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//��
			}
			else
			{
				//if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//��,���¼�������
			}
			if (0==i)
			{
				preDistance = 0.0f;
			}
			else
			{
				preDistance = ve[i].fLocation;
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ���
		{
			textRect.left = nLeft + 3 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 4 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fEventloss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"), ve[i].fEventloss);//���
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// �����
		{
			textRect.left = nLeft + 4 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 5 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fTotalLoss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//�����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// б��
		{
			textRect.left = nLeft + 5 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 6 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			//б��
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//Ϊ0ʱ��ʾ"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//б��
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ����
		{
			textRect.left = nLeft + 6 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 7 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].nEventType)//����
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		nStartLine++; //������1
	}
	pDC->SelectObject (pOldFont);
}
/****************************************************************
��������:             DrawEventList2Way()                                                                               
����˵��:             ��˫�������¼��б���                                                                             
��ڲ���:             nLeft ����㣬 nTop �����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-04-26                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::DrawEventList2Way(CDC* pDC, INT nLeft, INT nTop , INT nVspace , CRect rectDraw, INT nCurSel)//�����¼��б�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	INT nLeftSpace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	INT nHspace = (rectDraw.Width() - 2 * nLeftSpace) / EVENT_LIST_COL;  //lzy2014.8.12
	INT nStartLine = 1;

	float preDistance=0;
	float fLossAvg = 0;
	TestResultInfo* pData = &m_stPrintDetail.stTestResultInfo[nCurSel];  //lzy2014.8.30

	if (NULL == pData)
		return;

	CString strText=_T("Test Height");
	CSize sizeText = pDC->GetTextExtent(strText);
	INT nCharHeight = sizeText.cy;  //��ȡ����߶�
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//�ٶ�??
	INT nSize=0;
	    
	if(ve.size()>EVENT_LIST_PRINT_MAX)
		nSize = EVENT_LIST_PRINT_MAX;
	else
		nSize = ve.size();

	m_stPrintDetail.stPrintEvent.EventCount = nSize;   //��������

	for (INT i=0;i<nSize;i++)//
	{
		// ���
		{
			textRect.left = nLeft;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace; lzy 2014.8.15

			//����
			strText.Format(_T("%d"),i+1);
			if (g_iLang == CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" �Ƿ���");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" ����");;//1�������¼�����
				}
			}
			else if (g_iLang == ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1�������¼�����
				}
			}

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		{
			textRect.left = nLeft + nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 2 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//����
			}
			else
			{
				//if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
				strText.Format(_T("%.3f"),ve[i].fLocation/1000);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		//strText.Format(_T("%.3f"), 0.0);
		strText.Format(_T("--"));//Ϊ0ʱ��ʾ"-"

        //���»�ȡA->B LOSS
		textRect.left = nLeft + 2 * nHspace;
		textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
		textRect.right = nLeft + 3 * nHspace;
		textRect.bottom = textRect.top + nCharHeight;

		GetEventInfo2Way(i,strText,0);
		pDC->DrawText(strText, &textRect,DT_CENTER);//����

		//strText.Format(_T("%.3f"), 0.0);
		strText.Format(_T("--"));//Ϊ0ʱ��ʾ"-"

		//���»�ȡB->A LOSS
		textRect.left = nLeft + 3 * nHspace;
		textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
		textRect.right = nLeft + 4 * nHspace;
		textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;

		GetEventInfo2Way(i,strText,1);
		pDC->DrawText(strText, &textRect,DT_CENTER);//����

		//���¸���ƽ�����
		textRect.left = nLeft + 4 * nHspace;
		textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
		textRect.right = nLeft + 5 * nHspace;
		textRect.bottom = textRect.top + nCharHeight;

		GetEventInfo2Way(i,strText,2);
		pDC->DrawText(strText, &textRect,DT_CENTER);//����

		// �����
		{
			textRect.left = nLeft + 5 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 6 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fTotalLoss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//�����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// б��
		{
			textRect.left = nLeft + 6 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 7 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			//б��
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//Ϊ0ʱ��ʾ"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//б��
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ����
		{
			textRect.left = nLeft + 7 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 8 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].nEventType)//����
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		nStartLine++; //������1
	}
	pDC->SelectObject (pOldFont);
}

/****************************************************************
��������:             DrawCurve()                                                                               
����˵��:             ����������                                                                              
��ڲ���:             CDC                                                                                           
���ڲ���:             ��                                                                                        
����ʱ��:             2011-07-24                                                                  
Write By:             XJF                                                                                          
****************************************************************/
void CMyView::DrawCurve(CDC* pDC, CRect& rect, INT nCurveIndex)
{
	TestResultInfo* pData = &m_stPrintDetail.stTestResultInfo[nCurveIndex];
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//û�л����ݺ���
		return;

	//ת��Ϊ��ͼ��������
	if (NULL==pData->ptDrawData)
	{
		pData->ptDrawData=new CPoint[pData->DataPointsNum];
	}
	pData->StartPoint=0;
    pData->MaxPoint=pData->DataPointsNum;
	for(INT i = pData->StartPoint; i < pData->MaxPoint; i++)
	{
		pData->ptDrawData[i].x = INT(rect.left + rect.Width () * (i - pData->StartPoint)/(pData->MaxPoint - pData->StartPoint));
		//pData->ptDrawData[i].y = INT(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ 2013.05.12
		pData->ptDrawData[i].y = INT(rect.bottom - (((-pData->pDataPointsSmooth[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#else
		//pData->ptDrawData[i].y = INT(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height() - 150);//zll 2013.6.3 -150 ʹ������y�������ƶ�
		pData->ptDrawData[i].y = INT(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());//lzy2014.7.18

#endif
		if(pData->ptDrawData[i].x > rect.right)
			pData->ptDrawData[i].x = rect.right;
		if(pData->ptDrawData[i].y < rect.top)
			pData->ptDrawData[i].y = rect.top;
		if(pData->ptDrawData[i].y > rect.bottom)
			pData->ptDrawData[i].y = rect.bottom; 
	}

	//������
	pDC->Polyline(&pData->ptDrawData[pData->StartPoint], pData->MaxPoint - pData->StartPoint);
}
/****************************************************************
��������:             DrawEvent()                                                                               
����˵��:             ���¼�                                                                              
��ڲ���:             CDC                                                                                           
���ڲ���:             ��                                                                                        
����ʱ��:             2016-04-26                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::DrawEvent(CDC* pDC, CRect& rect, INT nCurveIndex)
{
	TestResultInfo* pData = &m_stPrintDetail.stTestResultInfo[nCurveIndex];
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//û�л����ݺ���
		return;

	//���¼������߼��·����ο� zll 2013.5.23 ��
	INT fX,fY;
	INT prevfX = 0;//��һ�ڵ��λ��
	INT offsetfx = 0; //���ڵ��ƫ��
	CRect rectTemp;
	rectTemp.SetRectEmpty();
	//TEXTMETRIC  tm;
	//pDC->GetTextMetrics(&tm);
	//INT nCharWidth = tm.tmMaxCharWidth;  //����ַ����
	for (UINT i=0; i<pData->EventList.vEventInfo.size(); i++)
	{		
		CRect rectLineBottom;
		CString temp;
		fX=INT(rect.left + pData->EventList.vEventInfo[i].fLocation*rect.Width()/pData->Length);
		//fY=INT(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(g_nTraceYmax-OTDR_TRACE_Y_MIN) - 110);//zll 2013.6.3 -110 ʹ�¼����ʾ��y�������ƶ�
		fY=INT(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(OTDR_TRACE_Y_MAX-OTDR_TRACE_Y_MIN));//lzy2014.7.23 ʹ�¼������ֲ���y���ƶ�

		if(i == 0)
		{
			prevfX = fX;
		}
		else
		{
			offsetfx = fX - prevfX;
			prevfX = fX;
		}
		//����ֱ��
		INT nLineH = rect.Height() / 10; //���߸߶�
		INT nSquare = nLineH / 5 ;  //С�����εı߳�
		INT nLineTop = nLineH / 6;  //����ʼ��λ��
		pDC->MoveTo(fX,fY- nLineTop);
		//pDC->LineTo(fX,fY+44+50);//zll 2013.6.3 +50 ʹ�¼����ʾ��y�������ƶ�
		pDC->LineTo(fX,fY+nLineH);//lzy2014.8.26 ʹ�¼����ʾ��y�������ƶ�
		//��ֱ�������������
		rectLineBottom.SetRect(fX - nSquare / 2,fY + nLineH,fX + nSquare / 2, fY + nLineH + nSquare);
		//rectLineBottom.SetRect(fX - 8-20,fY + 34+60,fX - 8 + 16+20, fY + 34 + 18+100);
		if (i>8)//��λ��
		{
			rectLineBottom.right += 2;
		}
		//pDC->Rectangle(&rectLineBottom);
		//����˥������Ծ��롢б��
		CString strEventLosss;
		strEventLosss.Format(_T("%.3fdB"), pData->EventList.vEventInfo[i].fEventloss);
		CString strLocation;
		CString strLink;
		//�����ı�
		float fValLocation = pData->EventList.vEventInfo[i].fLocation/DIVISOR_TOKM;
		float fValLink = pData->EventList.vEventInfo[i].fLineK;
		
		CFont* pOldPen = pDC->SelectObject(&m_FontVR);

		//����Ҫ��ʾ���ַ����߶�
		INT height = pDC->DrawText( strEventLosss, &rectLineBottom, DT_WORDBREAK | DT_CENTER | DT_CALCRECT );
		CSize sizeText = pDC->GetTextExtent(strEventLosss);
		//rectLineBottom.top += (rectLineBottom.Height() - height)/2;
		//���
		pDC->TextOut(fX - sizeText.cy, fY + nLineH +  rectLineBottom.Height() + sizeText.cx, strEventLosss);
		//pDC->TextOut(fX - 20, fY + rectLineBottom.Width() + 130, strEventLosss);
		//λ��
		sizeText = pDC->GetTextExtent(strLocation);
		pDC->TextOut(fX , fY + nLineH + rectLineBottom.Height() + sizeText.cx, strLocation);
		//б��
		//pDC->TextOut(fX + offsetfx / 2, fY + rectLineBottom.Width() + 300, strLink);//cwcq
		sizeText = pDC->GetTextExtent(strEventLosss);
		pDC->TextOut(fX - offsetfx / 2, fY + nLineH*2 + rectLineBottom.Height() + sizeText.cx, strLink);//lzy2014.8.26
		pDC->SelectObject(pOldPen);
	}
}
/****************************************************************
��������:             DrawCurve2Way()                                                                               
����˵��:             ��˫���������                                                                              
��ڲ���:             CDC                                                                                           
���ڲ���:             ��                                                                                        
����ʱ��:             2016-04-26                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::DrawCurve2Way(CDC* pDC, CRect& rect, INT nCurveIndex)
{
	UINT iEventLast = m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo.size() - 1;  //��ȡB2A�����һ���¼���λ��
	INT nPointsStart = m_stPrintDetail.stTestResultInfo[1].DataPointsNum - m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo[iEventLast].index;  //��ȡA2B�ߵ���ʼλ��
	INT nPointsTotal = m_stPrintDetail.stTestResultInfo[0].DataPointsNum>m_stPrintDetail.stTestResultInfo[1].DataPointsNum?m_stPrintDetail.stTestResultInfo[0].DataPointsNum:m_stPrintDetail.stTestResultInfo[1].DataPointsNum + nPointsStart;   //��������
	m_stPrintDetail.stTestResultInfo[2].YOffset = m_stPrintDetail.stTestResultInfo[1].Length - m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo[iEventLast].fLocation;
	FLOAT fLengthTotal = m_stPrintDetail.stTestResultInfo[0].Length + m_stPrintDetail.stTestResultInfo[2].YOffset;
	
	WORD* pWordAB = m_stPrintDetail.stTestResultInfo[0].pDataPoints;
	WORD* pWordBA = m_stPrintDetail.stTestResultInfo[1].pDataPoints;
	std::vector<WORD> recordAB;//
	std::vector<WORD> recordBA;
	INT nPointsCountAB = m_stPrintDetail.stTestResultInfo[0].DataPointsNum;  //A��B�����ݵ���
	INT nPointsCountBA = m_stPrintDetail.stTestResultInfo[1].DataPointsNum;  //B��A�����ݵ���
	m_stPrintDetail.stTestResultInfo[2].Length = fLengthTotal;
	//������A2B�ߵ�����
	for(INT j = 0; j < nPointsStart; j++) //A2B��ǰ��һ��Ϊ0ֵ
	{
		recordAB.push_back(0);
	}
	for(INT j = nPointsStart; j < nPointsTotal; j++)  //A2B�ߺ���һ������ΪOTDR���⵽������
	{
		recordAB.push_back(pWordAB[j - nPointsStart]);
	}

	//������B2A�ߵ�����
	for(INT j = 0; j < nPointsCountBA; j++) //A2B��ǰ��һ��Ϊ0ֵ
	{
		recordBA.push_back(pWordBA[nPointsCountBA - j]);  //����ǰ��һ��
	}
	for(INT j = nPointsCountBA; j < nPointsTotal; j++)  //A2B�ߺ���һ������ΪOTDR���⵽������
	{
		recordBA.push_back(0);
	}

	for(INT i=0;i<2;i++)
	{
		if(m_stPrintDetail.stTestResultInfo[2].pDataPoints)
		{
			delete []m_stPrintDetail.stTestResultInfo[2].pDataPoints;
			m_stPrintDetail.stTestResultInfo[2].pDataPoints = NULL;
		}
		m_stPrintDetail.stTestResultInfo[2].DataPointsNum = nPointsTotal;
		m_stPrintDetail.stTestResultInfo[2].pDataPoints = new WORD[nPointsTotal];
		if(i==0)
	        memcpy(m_stPrintDetail.stTestResultInfo[2].pDataPoints, &recordAB[0], nPointsTotal * sizeof(WORD));  //ȡ������
		else
	        memcpy(m_stPrintDetail.stTestResultInfo[2].pDataPoints, &recordBA[0], nPointsTotal * sizeof(WORD));  //ȡ������
		
		DrawCurve(pDC, rect, 2);
	}
	DrawEvent2Way(pDC, rect, nCurveIndex);

	for(INT i=0;i<2;i++)
	{
		if(m_stPrintDetail.stTestResultInfo[2].pDataPoints)
		{
			delete []m_stPrintDetail.stTestResultInfo[2].pDataPoints;
			m_stPrintDetail.stTestResultInfo[2].pDataPoints = NULL;
		}
	}

	//
	recordAB.clear();
	recordBA.clear();
}
/****************************************************************
��������:             DrawEvent2Way()                                                                               
����˵��:             ��˫�������¼�                                                                              
��ڲ���:             CDC                                                                                           
���ڲ���:             ��                                                                                        
����ʱ��:             2016-04-26                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::DrawEvent2Way(CDC* pDC, CRect& rect, INT nCurveIndex)
{
	TestResultInfo* pData = &m_stPrintDetail.stTestResultInfo[nCurveIndex];
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//û�л����ݺ���
		return;

	//���¼������߼��·����ο� zll 2013.5.23 ��
	INT fX,fY;
	INT prevfX = 0;//��һ�ڵ��λ��
	INT offsetfx = 0; //���ڵ��ƫ��
	CRect rectTemp;
	rectTemp.SetRectEmpty();

	for (UINT i=0; i<pData->EventList.vEventInfo.size(); i++)
	{		
		CRect rectLineBottom;
		CString temp;
		fX=INT(rect.left + (pData->YOffset+pData->EventList.vEventInfo[i].fLocation)*rect.Width()/pData->Length);
		fY=INT(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(OTDR_TRACE_Y_MAX-OTDR_TRACE_Y_MIN));//lzy2014.7.23 ʹ�¼������ֲ���y���ƶ�

		if(i == 0)
		{
			prevfX = fX;
		}
		else
		{
			offsetfx = fX - prevfX;
			prevfX = fX;
		}
		//����ֱ��
		INT iTop = rect.top + rect.Height() / 6;
	    INT iBottom = rect.top + rect.Height() *5/ 6;
		INT iText = rect.top + rect.Height() *3/ 4;

		INT nLineH = rect.Height() / 10; //���߸߶�
		INT nSquare = nLineH / 5 ;  //С�����εı߳�
		INT nLineTop = nLineH / 6;  //����ʼ��λ��
		//pDC->MoveTo(fX,fY- nLineTop);
		//pDC->LineTo(fX,fY+nLineH);
		pDC->MoveTo(fX,iTop);
		pDC->LineTo(fX,iBottom);
		//��ֱ�������������
		rectLineBottom.SetRect(fX - nSquare / 2,fY + nLineH,fX + nSquare / 2, fY + nLineH + nSquare);
		if (i>8)//��λ��
		{
			rectLineBottom.right += 2;
		}
		//����˥������Ծ��롢б��
		CString strEventLosss;
		GetEventInfo2Way(i,strEventLosss,2);
		//strEventLosss.Format(_T("%.3fdB"), pData->EventList.vEventInfo[i].fEventloss);
		//CString strLocation;
		//CString strLink;
		//�����ı�
		//float fValLocation = pData->EventList.vEventInfo[i].fLocation/DIVISOR_TOKM;
		//float fValLink = pData->EventList.vEventInfo[i].fLineK;
		
		CFont* pOldPen = pDC->SelectObject(&m_FontVR);

		//����Ҫ��ʾ���ַ����߶�
		INT height = pDC->DrawText( strEventLosss, &rectLineBottom, DT_WORDBREAK | DT_CENTER | DT_CALCRECT );
		CSize sizeText = pDC->GetTextExtent(strEventLosss);
		//���
		//pDC->TextOut(fX - sizeText.cy, fY + nLineH +  rectLineBottom.Height() + sizeText.cx, strEventLosss);
		pDC->TextOut(fX , iText, strEventLosss);
		//λ��
		//strLocation.Format(_T("%.3fdBkm"), fValLocation);
		//sizeText = pDC->GetTextExtent(strLocation);
		//pDC->TextOut(fX , fY + nLineH + rectLineBottom.Height() + sizeText.cx, strLocation);
        //pDC->TextOut(fX , iText, strLocation);
		//б��
		//sizeText = pDC->GetTextExtent(strEventLosss);
		//pDC->TextOut(fX - offsetfx / 2, fY + nLineH*2 + rectLineBottom.Height() + sizeText.cx, strLink);//lzy2014.8.26
		//pDC->TextOut(fX , iText, strLink);
		pDC->SelectObject(pOldPen);
	}
}

void CMyView::InnerDraw(CDC* pDC,INT nCurPage)
{
	INT nCurSel = 0;

	if (m_bPrintCurve) //��ӡ��ǰѡ������
	{
		if(m_stPrintDetail.stTestResultInfo[0].DataPointsNum>0&&m_stPrintDetail.stTestResultInfo[1].DataPointsNum>0)
		    nCurSel = SHOW_ALL;
		else if(m_stPrintDetail.stTestResultInfo[0].DataPointsNum>0&&m_stPrintDetail.stTestResultInfo[1].DataPointsNum<=0)
			nCurSel = SHOW_A2B;
		else
			nCurSel = SHOW_B2A;

		if(nCurPage == 1)//��һҳ
		    DrawPrintContent(pDC, m_rectDraw, nCurSel); 
		else
			DrawPrintContentPage2(pDC, m_rectDraw, nCurSel); 
		return;
	}
}

void CMyView::DrawGridLine(CDC* pDC, CRect& rect)
{
	if(!m_bShowGrid)//����ʾդ��
	{
		return;
	}//eif

	CPen penGridLine;
	//penGridLine.CreatePen (PS_DOT/*����*/,1, g_sorFileArray.waveConfig.ColorGrid);//��Ϊ�����cwcq2012.10.31
	penGridLine.CreatePen (PS_DOT/*����*/, 1, RGB(0xcc,0xcc,0xcc) );//wcq2012.10.31
	CPen* pOldPen = pDC->SelectObject (&penGridLine);

	if (m_bShowGrid)
	{
		//������̶�
		INT ndx = rect.Width () / COOR_SPLIT_COUNT_H;
		INT ndy = rect.Height () / COOR_SPLIT_COUNT_V;
		pDC->SelectObject (&penGridLine);
		for ( INT i = 1; i< COOR_SPLIT_COUNT_H; i++)
		{	
			//������̶�
			pDC->MoveTo(rect.left + ndx * i,rect.bottom );
			pDC->LineTo(rect.left + ndx * i,rect.top );
		}

		//������̶�
		pDC->SetTextAlign(TA_LEFT | TA_TOP);
		for (INT i=1; i<COOR_SPLIT_COUNT_V; i++)
		{
			pDC->MoveTo(rect.left , rect.bottom - ndy * i);
			pDC->LineTo(rect.right, rect.bottom - ndy * i);
		}
	}

	//�ָ�
	pDC->SelectObject(pOldPen);
	//�ͷ���Դ
	DeleteObject(penGridLine.GetSafeHandle());
}

void CMyView::InitPrintText()
{
	//
    m_stPrintDetail.strTitle=m_util.ReadLangString(_T("PrintInfo"),_T("PRINT_TITLE"));
	m_stPrintDetail.strLogo=m_util.ReadLangString(_T("PrintInfo"), _T("PRINT_LOGO"));
	m_stPrintDetail.stPrintQuest.strTitle=m_util.ReadLangString(_T("PrintInfo"), _T("QUEST_TITLE"));
	m_stPrintDetail.stPrintQuest.strID=m_util.ReadLangString(_T("PrintInfo"), _T("QUEST_ID"));
	m_stPrintDetail.stPrintQuest.strContractor=m_util.ReadLangString(_T("PrintInfo"), _T("QUEST_CONTRACTOR"));
	m_stPrintDetail.stPrintQuest.strCustomer=m_util.ReadLangString(_T("PrintInfo"), _T("QUEST_USER"));
	m_stPrintDetail.stPrintQuest.strSorFileName=m_util.ReadLangString(_T("PrintInfo"), _T("QUEST_FILE"));
	m_stPrintDetail.stPrintQuest.strTestDate=m_util.ReadLangString(_T("PrintInfo"), _T("QUEST_DATE"));
	m_stPrintDetail.stPrintQuest.strPlayer=m_util.ReadLangString(_T("PrintInfo"), _T("QUEST_PLAYER"));

	m_stPrintDetail.stPrintQuest.strComment=m_util.ReadLangString(_T("PrintInfo"), _T("COMMENT_TITLE"));

	m_stPrintDetail.stPrintConfig.strTitle=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_TITLE"));
	m_stPrintDetail.stPrintConfig.strWavelength=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_WAVELEN"));
	m_stPrintDetail.stPrintConfig.strPulse=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_PULSE"));
	m_stPrintDetail.stPrintConfig.strOrigin=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ORIGIN"));
	m_stPrintDetail.stPrintConfig.strOriLocation=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ORILOCATION"));
	m_stPrintDetail.stPrintConfig.strOriCable=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ORICABLE"));
	m_stPrintDetail.stPrintConfig.strOriFiber=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ORIFIBER"));
	m_stPrintDetail.stPrintConfig.strOriColor=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ORICOLOR"));

	m_stPrintDetail.stPrintConfig.strRange=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_DIST"));
	m_stPrintDetail.stPrintConfig.strAverage=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_AVG"));
	m_stPrintDetail.stPrintConfig.strEnd=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_END"));
	m_stPrintDetail.stPrintConfig.strEndLocation=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ENDLOCATION"));
	m_stPrintDetail.stPrintConfig.strEndCable=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ENDCABLE"));
	m_stPrintDetail.stPrintConfig.strEndFiber=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ENDFIBER"));
	m_stPrintDetail.stPrintConfig.strEndColor=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ENDCOLOR"));

	m_stPrintDetail.stPrintConfig.strBackScatter=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_RBS"));
	m_stPrintDetail.stPrintConfig.strLossThreshold=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_LOSSTH"));
	m_stPrintDetail.stPrintConfig.strRefThreshold=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_REFLECTH"));
	m_stPrintDetail.stPrintConfig.strEndThreshold=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_ENDFIBERTH"));
	m_stPrintDetail.stPrintConfig.strIndex=m_util.ReadLangString(_T("PrintInfo"), _T("CONFIG_INDEX"));
	m_stPrintDetail.stPrintTrace.strTitle=m_util.ReadLangString(_T("PrintInfo"), _T("TRACE_TITLE"));
	m_stPrintDetail.stPrintEvent.strTitle=m_util.ReadLangString(_T("PrintInfo"), _T("EVENT_TITLE"));
	m_stPrintDetail.stPrintConfig.strTitleAB=m_util.ReadLangString(_T("MainDlg"), _T("IDS_MAIN_DLG_RADIO_SHOWAB"));
	m_stPrintDetail.stPrintConfig.strTitleBA=m_util.ReadLangString(_T("MainDlg"), _T("IDS_MAIN_DLG_RADIO_SHOWBA"));


	strListHead[0]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_ORDER"));
//	strListHead[1]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_TYPE"));
	strListHead[1]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_DIST"));
	strListHead[2]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_SEGMENT"));
	strListHead[3]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_LOSS"));
	strListHead[4]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_TOTAL_LOSS"));
	strListHead[5]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_SLOPE"));
	strListHead[6]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_REFLECTION"));

	strListHead2Way[0]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_ORDER"));
	strListHead2Way[1]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_DIST"));
	strListHead2Way[2]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_LOSS_AB"));
	strListHead2Way[3]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_LOSS_BA"));
	strListHead2Way[4]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_AVG_LOSS"));
	strListHead2Way[5]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_TOTAL_LOSS"));
	strListHead2Way[6]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_SLOPE"));
	strListHead2Way[7]=m_util.ReadLangString(_T("MainDlg"), _T("IDS_EVENT_LIST_EVENT_MAX_REFLECTION"));
}

void CMyView::MemcpyResInfo(TestResultInfo *pResultInfo)
{
	INT nTemp = 0;

	for(INT i=0;i<3;i++)
	{
		if(pResultInfo->DataPointsNum<=0) 
		{
			pResultInfo++;
			continue;
		}

		nTemp = pResultInfo->EventList.vEventInfo.size();
		if (NULL == m_stPrintDetail.stTestResultInfo[i].pDataPoints)
		{
			m_stPrintDetail.stTestResultInfo[i].pDataPoints = new WORD[pResultInfo->DataPointsNum];
			memcpy(m_stPrintDetail.stTestResultInfo[i].pDataPoints,pResultInfo->pDataPoints,pResultInfo->DataPointsNum * 2);
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ wcq2013.04.26
			if (NULL == testResultInfo.pDataPointsSmooth)
			{
				testResultInfo.pDataPointsSmooth = new WORD[sorData.vAllData[0].nCount];
			}
#endif
		}
		else//��������
		{
			if (m_stPrintDetail.stTestResultInfo[i].DataPointsNum < pResultInfo->DataPointsNum)//�������ݳ��ȱȽ�Ҫ��ֵ����
			{
				delete[] m_stPrintDetail.stTestResultInfo[i].pDataPoints;
				m_stPrintDetail.stTestResultInfo[i].pDataPoints = new WORD[pResultInfo->DataPointsNum];
#ifdef COMPILE_SHOW_SMOOTH_DATA////��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ wcq2013.04.26
				delete[] testResultInfo.pDataPointsSmooth;
				testResultInfo.pDataPointsSmooth = new WORD[sorData.vAllData[0].nCount];
#endif
			}//eif
			memcpy(m_stPrintDetail.stTestResultInfo[i].pDataPoints,&pResultInfo->pDataPoints,pResultInfo->DataPointsNum * 2);
		}//eif
		m_stPrintDetail.stTestResultInfo[i].DataPointsNum = pResultInfo->DataPointsNum;

		//
		m_stPrintDetail.stTestResultInfo[i].Refraction = pResultInfo->Refraction;
		m_stPrintDetail.stTestResultInfo[i].PulseWidth = pResultInfo->PulseWidth;
		m_stPrintDetail.stTestResultInfo[i].WaveLength = pResultInfo->WaveLength;
		m_stPrintDetail.stTestResultInfo[i].Frequency = pResultInfo->Frequency;
		m_stPrintDetail.stTestResultInfo[i].DataSpacing = pResultInfo->DataSpacing;
		m_stPrintDetail.stTestResultInfo[i].Length = pResultInfo->Length;

		m_stPrintDetail.stTestResultInfo[i].EventList.vEventInfo.clear();//�¼��б������� wcq2012.08.13
		//nTemp = 50;
		for(INT j=0; j<nTemp; j++)
		{	
			EventInfo eventInfo;

			memcpy(&eventInfo,&pResultInfo->EventList.vEventInfo[j],sizeof(EventInfo));
			m_stPrintDetail.stTestResultInfo[i].EventList.vEventInfo.push_back(eventInfo);
		}//efor

		for(INT k=0;k<nTemp;k++)
		{
			m_stPrintDetail.stTestResultInfo[i].EventList.vEventInfo[k].fTotalLoss = pResultInfo->EventList.vEventInfo[k].fTotalLoss;
		}//efor
		m_stPrintDetail.stTestResultInfo[i].BackscatterCoefficient = pResultInfo->BackscatterCoefficient;
		m_stPrintDetail.stTestResultInfo[i].LossThreshold = pResultInfo->LossThreshold;
		m_stPrintDetail.stTestResultInfo[i].ReflectanceThreshold = pResultInfo->ReflectanceThreshold;
		m_stPrintDetail.stTestResultInfo[i].EndOfFiberThreshold = pResultInfo->EndOfFiberThreshold;
		m_stPrintDetail.stTestResultInfo[i].Range = pResultInfo->Range;//��������Ϊ������Χ����λ��m
		m_stPrintDetail.stTestResultInfo[i].strTestTime = pResultInfo->strTestTime;//��������Ϊ������Χ����λ��m
		m_stPrintDetail.stTestResultInfo[i].strFileName = pResultInfo->strFileName;
		pResultInfo++;
    }
}

void CMyView::GetEvent2Way(TestResultInfo *pInfo2Way, TestResultInfo *infoA2B, TestResultInfo *infoB2A)
{
	std::vector<EventInfo *> vEventInfo;

	pInfo2Way->EventList.vEventInfo.clear();

	UINT uiEvents = infoB2A->EventList.vEventInfo.size();  //B->A�ߵ����¼���

	UINT uiOffset = infoB2A->DataPointsNum - infoB2A->EventList.vEventInfo[uiEvents - 1].index;   //�ܵ���-����¼�������
	
	for(UINT i = 0; i != infoA2B->EventList.vEventInfo.size(); i++ ) //�Ȱѵ�һ���ߵ��¼�����
	{
		pInfo2Way->EventList.vEventInfo.push_back(infoA2B->EventList.vEventInfo[i]);
		pInfo2Way->EventList.vEventInfo[ i ].index += uiOffset;  //�����ݵ�ƽ��һ��
	}	
	
	float fOffset = infoB2A->Range -  infoB2A->EventList.vEventInfo[uiEvents -1].fLocation;   //�ܾ���-����¼���λ��
	for(UINT i = 0; i != uiEvents; i++ )  //�ٰѵڶ����ߵ����ݷ�����ʱ����
	{
		EventInfo *pEvent = new EventInfo();//&(infoB2A->EventList.vEventInfo[uiEvents - 1 - i]);//
		*pEvent = infoB2A->EventList.vEventInfo[uiEvents - 1 - i];
		pEvent->fLocation = infoB2A->EventList.vEventInfo[uiEvents -1].fLocation -  infoB2A->EventList.vEventInfo[uiEvents -1 - i].fLocation;  //��һ���¼�Ϊ0
		pEvent->index = infoB2A->DataPointsNum - infoB2A->EventList.vEventInfo[uiEvents - 1 - i].index;	 //�����ˣ���һ���¼���Ϊ0	      
		
		//pInfo2Way->EventList.vEventInfo.push_back(*pEvent);
		vEventInfo.push_back(pEvent);
	}

	INT iDiff = 0;   //λ�ò������С������Ϊ��һ���¼�
	INT iCount = 0;
	for(std::vector<EventInfo *>::iterator it = vEventInfo.begin(); it != vEventInfo.end(); ++it)  //ȫ����һ��
	{
		BOOL bSel = FALSE;
        //�����뵽ĳ��λ��
		for(std::vector<EventInfo>::iterator it2 = pInfo2Way->EventList.vEventInfo.begin(); it2 != pInfo2Way->EventList.vEventInfo.end(); ++it2)
		{
			iDiff = (*it)->index - ((EventInfo)*it2).index ;
			
			if(iDiff <= 0) //������������뵽ǰ��
			{
				pInfo2Way->EventList.vEventInfo.insert(it2, **it);
				bSel = TRUE;
				break;
			}
		}

		if(!bSel)
			pInfo2Way->EventList.vEventInfo.push_back(**it);
	}

	//���´���ϲ���ص��¼�
	std::vector<EventInfo>::iterator it;
/*	UINT uiLen =  pInfo2Way->EventList.vEventInfo.size();
	if(uiLen>1)
	{
		it = pInfo2Way->EventList.vEventInfo.begin() + 1;
		pInfo2Way->EventList.vEventInfo.erase(it);  //ɾ���ڶ���Ԫ�أ�����һ�����ߵ��׸��¼�
		if(uiLen>3)
		{
		    it = pInfo2Way->EventList.vEventInfo.end() - 2;
		    pInfo2Way->EventList.vEventInfo.erase(it);  //ɾ�����ڶ���Ԫ�أ����ڶ������ߵ����һ���¼�
		}
	}
*/
    UINT uiFirst = pInfo2Way->EventList.vEventInfo[0].index;
	UINT uiTemp;
	float fEventHigh = pInfo2Way->EventList.vEventInfo[0].fEventHigh;
	//EventInfo eventTemp = ((EventInfo)*it);
	for(it = pInfo2Way->EventList.vEventInfo.begin() + 1; it < pInfo2Way->EventList.vEventInfo.end() - 1; ++it)  //ѭ���ϲ��¼�
	{
		uiTemp = (*it).index;	
		if(abs(1.0 * uiTemp - uiFirst) <= EVENT_2WAY_OFFSET)
		{
			fEventHigh = max(fEventHigh, ((EventInfo)*it).fEventHigh);   //ȡ�����Ľϴ��ֵ����

			it = pInfo2Way->EventList.vEventInfo.erase(it);
			//(*it).fEventHigh = fEventHigh;
			(*(--it)).fEventHigh = fEventHigh;
			++it;
		}
		uiFirst =  (*it).index;
		//eventTemp = ((EventInfo)*it);
		fEventHigh = (*it).fEventHigh;
	}

	std::vector<EventInfo *>::iterator it_Event;
	for(it_Event = vEventInfo.begin(); it_Event != vEventInfo.end(); it_Event++)
	{
		delete *it_Event;
		*it_Event = NULL;
	}
	vEventInfo.clear();
}
/****************************************************************
��������:             DrawQuestInfo()                                                                               
����˵��:             ��������Ϣ                                                                        
��ڲ���:                                                                               
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-01                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::DrawQuestInfo(CDC* pDC, CRect rectDraw,INT &nBottom,INT nCurSel)
{
	INT nCurHeight=0;
	INT nXoffset = rectDraw.left;
	INT nYoffset = rectDraw.top;
	INT nVspace = rectDraw.Height() / 64;
	INT nHleftspace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	INT nHmidspace = rectDraw.Width() / 20;
	INT nHinterval = nVspace / 4;   //���ο�ֱ���
	INT nHMargin = nHleftspace / 2;   //���ο����������Ե�ľ���
	INT nHSubMargin = nHleftspace / 2;  //����Ŀ��������
	INT n1 = nXoffset + nHleftspace + nHMargin;  //��ߵ�һ�����ʼX����
//	INT n2 = n1 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //�м������ʼX����
	//INT n3 = n2 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //���ұ������ʼX����
	CRect textRect;

	TEXTMETRIC  tm;
	pDC->GetTextMetrics(&tm);
	INT nCharWidth = tm.tmMaxCharWidth;  //����ַ����
	CString strText = _T("Test char height ��������");
	CSize sizeText = pDC->GetTextExtent(strText);
	INT nCharHeight = sizeText.cy;    //��������ĸ߶�
	INT nLineInterval = nCharHeight / 2 ;  //0.5���о�

	INT iWidth = 0;
	INT iX = 0;
	INT iY = 0;
	INT iHeight = 0;

	nCurHeight = nBottom + nHinterval;
	nBottom = nCurHeight + nLineInterval * 4 + nCharHeight * 3;
	pDC->Rectangle(nXoffset + nHleftspace, nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nBottom);	
	pDC->SelectObject(&m_fontGroup);
	strText = _T(" ") + m_stPrintDetail.stPrintQuest.strTitle + _T(" ");
	sizeText = pDC->GetTextExtent(strText);
	textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy / 2, nXoffset + (rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy / 2);
	pDC->DrawText(strText, textRect, DT_CENTER);  //д������Ϣ����	
	pDC->SelectObject(&m_dataFont);
	nCurHeight += nLineInterval;
	strText = m_stPrintDetail.stPrintQuest.strID + _T(": ") + m_stPrintDetail.stPrintQuest.QuestID;  //д������
	sizeText = pDC->GetTextExtent(strText);
	textRect.SetRect(nXoffset + nHleftspace + nHMargin, nCurHeight, nXoffset + nHleftspace + sizeText.cx + nHMargin, nCurHeight + sizeText.cy);
	pDC->DrawText(strText, textRect, DT_LEFT);		
	//strText = _T("File: ") + m_stPrintDetail.stPrintQuest.SorFileName;  //д�ļ���
	if(nCurSel!=SHOW_ALL)
		strText = m_stPrintDetail.stPrintQuest.strSorFileName + _T(": ") + m_stPrintDetail.stTestResultInfo[nCurSel].strFileName;  //д�ļ���
	else
		strText = m_stPrintDetail.stPrintQuest.strSorFileName + _T(": ") + m_stPrintDetail.stTestResultInfo[0].strFileName + _T("  ") + m_stPrintDetail.stTestResultInfo[1].strFileName;  //д�ļ���
	sizeText = pDC->GetTextExtent(strText);
	//textRect.SetRect(m_rectDraw.Width() / 2  - nHMargin, nCurHeight, m_rectDraw.Width() / 2 + sizeText.cx - nHMargin, nCurHeight + sizeText.cy);
	textRect.SetRect(nXoffset + rectDraw.Width() / 2  - nHMargin, nCurHeight, 2*nXoffset + rectDraw.Width() - n1, nCurHeight + sizeText.cy);
	pDC->DrawText(strText, textRect, DT_LEFT);
	nCurHeight = textRect.bottom + nLineInterval;
	//strText = _T("Contractor: ") + m_stPrintDetail.stPrintQuest.Contractor;  //д�а���
	strText = m_stPrintDetail.stPrintQuest.strContractor + _T(": ") + m_stPrintDetail.stPrintQuest.Contractor;  //д�а���
	sizeText = pDC->GetTextExtent(strText);
	textRect.SetRect(nXoffset + nHleftspace + nHMargin, nCurHeight, nXoffset + nHleftspace + sizeText.cx + nHMargin, nCurHeight + sizeText.cy);
	pDC->DrawText(strText, textRect, DT_LEFT);
	//strText = _T("Test date: ") + m_stPrintDetail.stPrintQuest.TestDate;  //д����ʱ��
	if(nCurSel!=SHOW_ALL)
		strText = m_stPrintDetail.stPrintQuest.strTestDate + _T(": ") + m_stPrintDetail.stTestResultInfo[nCurSel].strTestTime;  //д����ʱ��
	else
		strText = m_stPrintDetail.stPrintQuest.strTestDate + _T(": ") + m_stPrintDetail.stTestResultInfo[0].strTestTime + _T("  ") + m_stPrintDetail.stTestResultInfo[1].strTestTime;  //д����ʱ��

	sizeText = pDC->GetTextExtent(strText);
	textRect.SetRect(nXoffset + rectDraw.Width() / 2 - nHMargin, nCurHeight, nXoffset + rectDraw.Width() / 2 + sizeText.cx - nHMargin, nCurHeight + sizeText.cy);
	pDC->DrawText(strText, textRect, DT_LEFT);
	nCurHeight = textRect.bottom + nLineInterval;
	strText = m_stPrintDetail.stPrintQuest.strCustomer + _T(": ") + m_stPrintDetail.stPrintQuest.Customer;  //�ͻ�
	sizeText = pDC->GetTextExtent(strText);
	textRect.SetRect(nXoffset + nHleftspace + nHMargin, nCurHeight, nXoffset + nHleftspace + sizeText.cx + nHMargin, nCurHeight + sizeText.cy);
	pDC->DrawText(strText, textRect, DT_LEFT);
	//strText = _T("Operator: ") + m_stPrintDetail.stPrintQuest.Operator;  //д����Ա
	strText = m_stPrintDetail.stPrintQuest.strPlayer + _T(": ") + m_stPrintDetail.stPrintQuest.Player;  //д����Ա
	sizeText = pDC->GetTextExtent(strText);
	textRect.SetRect(nXoffset + rectDraw.Width() / 2 - nHMargin, nCurHeight, nXoffset + rectDraw.Width() / 2 + sizeText.cx - nHMargin, nCurHeight + sizeText.cy);
	pDC->DrawText(strText, textRect, DT_LEFT);
}
/****************************************************************
��������:             DrawConfig()                                                                               
����˵��:             ��������Ϣ                                                                        
��ڲ���:                                                                               
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-01                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::DrawConfig(CDC* pDC, CRect rectDraw,INT &nBottom,INT nCurSel)
{
	INT nCurHeight=0;
	INT nXoffset = rectDraw.left;
	INT nYoffset = rectDraw.top;
	INT nVspace = rectDraw.Height() / 64;
	INT nHleftspace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	INT nHmidspace = rectDraw.Width() / 20;
	INT nHinterval = nVspace / 4;   //���ο�ֱ���
	INT nHMargin = nHleftspace / 2;   //���ο����������Ե�ľ���
	INT nHSubMargin = nHleftspace / 2;  //����Ŀ��������
	INT n1 = nXoffset + nHleftspace + nHMargin;  //��ߵ�һ�����ʼX����
//	INT n2 = n1 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //�м������ʼX����
	//INT n3 = n2 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //���ұ������ʼX����
	CRect textRect;

	TEXTMETRIC  tm;
	pDC->GetTextMetrics(&tm);
	INT nCharWidth = tm.tmMaxCharWidth;  //����ַ����
	CString strText = _T("Test char height ��������");
	CSize sizeText = pDC->GetTextExtent(strText);
	INT nCharHeight = sizeText.cy;    //��������ĸ߶�
	INT nLineInterval = nCharHeight / 2 ;  //0.5���о�

	INT iWidth = 0;
	INT iX = 0;
	INT iY = 0;
	INT iHeight = 0;

	nCurHeight = nBottom + nVspace / 2;	
	nBottom = nCurHeight + nCharHeight * 10 + nLineInterval * 11;
	pDC->Rectangle(nXoffset + nHleftspace, nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nBottom);
	pDC->SelectObject(&m_fontGroup);
	strText = _T(" ") + m_stPrintDetail.stPrintConfig.strTitle + _T(" ");
	sizeText = pDC->GetTextExtent(strText);
	textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy / 2, nXoffset +(rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy / 2);
	pDC->DrawText(strText, textRect, DT_LEFT);
	pDC->SelectObject(&m_dataFont);	
	nCurHeight += nLineInterval;
	iWidth = (nXoffset + rectDraw.Width() - nHleftspace)/3;

	iX = nXoffset + nHleftspace + iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(m_stPrintDetail.stPrintConfig.strTitleAB, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(m_stPrintDetail.stPrintConfig.strTitleBA, textRect, DT_LEFT);	
	//дwavelength
	nCurHeight += nLineInterval*2;
	strText = m_stPrintDetail.stPrintConfig.strWavelength + _T("(nm): ") ;  //дwavelength
	
	iX = nXoffset + nHleftspace;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(n1,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(strText, textRect, DT_LEFT);		

	iX += iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%d"),m_stPrintDetail.stTestResultInfo[0].WaveLength);
	pDC->DrawText(strText, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%d"),m_stPrintDetail.stTestResultInfo[1].WaveLength);
	pDC->DrawText(strText, textRect, DT_LEFT);
	//дRange
	strText = m_stPrintDetail.stPrintConfig.strRange + _T("(m): ");  //дRange
	nCurHeight = textRect.bottom + nLineInterval;	
	iWidth = (nXoffset + rectDraw.Width() - nHleftspace)/3;
	iX = nXoffset + nHleftspace;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(n1,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(strText, textRect, DT_LEFT);

	iX += iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[0].Length);
	pDC->DrawText(strText, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[1].Length);
	pDC->DrawText(strText, textRect, DT_LEFT);
	//дBackscatter coeff 3
	strText = m_stPrintDetail.stPrintConfig.strBackScatter + _T("(dB): ");  //дBackscatter coeff 3
	nCurHeight = textRect.bottom + nLineInterval;	
	iWidth = (nXoffset + rectDraw.Width() - nHleftspace)/3;
	iX = nXoffset + nHleftspace;
	iY = nCurHeight;
	iHeight = sizeText.cy;	
	textRect.SetRect(n1,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(strText, textRect, DT_LEFT);

	iX += iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[0].BackscatterCoefficient);
	pDC->DrawText(strText, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[1].BackscatterCoefficient);
	pDC->DrawText(strText, textRect, DT_LEFT);

	//дPulse
	strText = m_stPrintDetail.stPrintConfig.strPulse + _T("(ns): ");//дPULSE
	nCurHeight = textRect.bottom + nLineInterval;	
	iWidth = (nXoffset + rectDraw.Width() - nHleftspace)/3;
	iX = nXoffset + nHleftspace;
	iY = nCurHeight;
	iHeight = sizeText.cy;		
	textRect.SetRect(n1,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(strText, textRect, DT_LEFT);

	iX += iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%ld"),m_stPrintDetail.stTestResultInfo[0].PulseWidth);
	pDC->DrawText(strText, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%ld"),m_stPrintDetail.stTestResultInfo[1].PulseWidth);
	pDC->DrawText(strText, textRect, DT_LEFT);
	//дAverage counts
	strText = m_stPrintDetail.stPrintConfig.strAverage + _T(": ");  //дAverage counts  
	nCurHeight = textRect.bottom + nLineInterval;	
	iWidth = (nXoffset + rectDraw.Width() - nHleftspace)/3;
	iX = nXoffset + nHleftspace;
	iY = nCurHeight;
	iHeight = sizeText.cy;		
	textRect.SetRect(n1,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(strText, textRect, DT_LEFT);

	iX += iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[0].Average);
	pDC->DrawText(strText, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[1].Average);
	pDC->DrawText(strText, textRect, DT_LEFT);
	//дLossThreshold 3
	strText = m_stPrintDetail.stPrintConfig.strLossThreshold + _T("(dB): ");  //дLossThreshold 3
	nCurHeight = textRect.bottom + nLineInterval;	
	iWidth = (nXoffset + rectDraw.Width() - nHleftspace)/3;
	iX = nXoffset + nHleftspace;
	iY = nCurHeight;
	iHeight = sizeText.cy;		
	textRect.SetRect(n1,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(strText, textRect, DT_LEFT);

	iX += iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[0].LossThreshold);
	pDC->DrawText(strText, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[1].LossThreshold);
	pDC->DrawText(strText, textRect, DT_LEFT);

	//дReflection threshold
	strText = m_stPrintDetail.stPrintConfig.strRefThreshold + _T("(dB): ");  //дReflection threshold
	nCurHeight = textRect.bottom + nLineInterval;	
	iWidth = (nXoffset + rectDraw.Width() - nHleftspace)/3;
	iX = nXoffset + nHleftspace;
	iY = nCurHeight;
	iHeight = sizeText.cy;		
	textRect.SetRect(n1,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(strText, textRect, DT_LEFT);

	iX += iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[0].ReflectanceThreshold);
	pDC->DrawText(strText, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[1].ReflectanceThreshold);
	pDC->DrawText(strText, textRect, DT_LEFT);

	//дFiber end threshold
	strText = m_stPrintDetail.stPrintConfig.strEndThreshold + _T("(dB): ");  //дFiber end threshold
	nCurHeight = textRect.bottom + nLineInterval;	
	iWidth = (nXoffset + rectDraw.Width() - nHleftspace)/3;
	iX = nXoffset + nHleftspace;
	iY = nCurHeight;
	iHeight = sizeText.cy;		
	textRect.SetRect(n1,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(strText, textRect, DT_LEFT);

	iX += iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[0].EndOfFiberThreshold);
	pDC->DrawText(strText, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[1].EndOfFiberThreshold);
	pDC->DrawText(strText, textRect, DT_LEFT);

	//д ������
	strText = m_stPrintDetail.stPrintConfig.strIndex + _T(": ");  //д ������
	nCurHeight = textRect.bottom + nLineInterval;	
	iWidth = (nXoffset + rectDraw.Width() - nHleftspace)/3;
	iX = nXoffset + nHleftspace;
	iY = nCurHeight;
	iHeight = sizeText.cy;		
	textRect.SetRect(n1,iY,iX+iWidth,iY+iHeight);
	pDC->DrawText(strText, textRect, DT_LEFT);

	iX += iWidth;
	iY = nCurHeight;
	iHeight = sizeText.cy;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[0].Refraction);
	pDC->DrawText(strText, textRect, DT_LEFT);	

	iX += iWidth;
	textRect.SetRect(iX,iY,iX+iWidth,iY+iHeight);
	strText.Format(_T("%.3f"),m_stPrintDetail.stTestResultInfo[1].Refraction);
	pDC->DrawText(strText, textRect, DT_LEFT);
}
/****************************************************************
��������:             DrawPrintContentPage2()                                                                               
����˵��:             ��˫���������ݵڶ�ҳ                                                                            
��ڲ���:             nLeft ����㣬 nTop �����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-01                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::DrawPrintContentPage2(CDC* pDC, CRect rectDraw, INT nCurSel, BOOL bSingle)
{
	//����
	INT nXoffset = rectDraw.left;
	INT nYoffset = rectDraw.top;
	INT nVspace = rectDraw.Height() / 64;
	INT nCurHeight = nYoffset+ nVspace * 3;
	INT nHleftspace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	INT nHmidspace = rectDraw.Width() / 20;
	INT nHinterval = nVspace / 4;   //���ο�ֱ���
	INT nHMargin = nHleftspace / 2;   //���ο����������Ե�ľ���
	INT nHSubMargin = nHleftspace / 2;  //����Ŀ��������
	INT n1 = nXoffset + nHleftspace + nHMargin;  //��ߵ�һ�����ʼX����
	INT n2 = n1 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //�м������ʼX����
	INT n3 = n2 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //���ұ������ʼX����
	INT nBottom = 0;   //������ο���±�������
	CRect textRect;
	CString strText , strTemp;	
	CSize sizeText;	
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);

	TEXTMETRIC  tm;
	pDC->GetTextMetrics(&tm);
	INT nCharWidth = tm.tmMaxCharWidth;  //����ַ����
	strText = _T("Test char height ��������");
	sizeText = pDC->GetTextExtent(strText);
	INT nCharHeight = sizeText.cy;    //��������ĸ߶�
	INT nLineInterval = nCharHeight / 2 ;  //0.5���о�
	//����д��һ��	
	{
		pDC->SelectObject (&m_dataFont);
		nCurHeight = nBottom + nVspace; 		
		sizeText = pDC->GetTextExtent(m_stPrintDetail.strLogo);
		nBottom = nCurHeight + sizeText.cy;
		textRect.SetRect(nXoffset + nHleftspace, nCurHeight, nXoffset + nHleftspace + sizeText.cx, nCurHeight + sizeText.cy);
		if(m_bPrintLogo)
			pDC->DrawText(m_stPrintDetail.strLogo, textRect, DT_LEFT);  //д����汾����ά��Ȩ
		CTime tm = CTime::GetCurrentTime();
		CString strCurTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
		sizeText = pDC->GetTextExtent(strCurTime);
		textRect.SetRect(nXoffset + rectDraw.Width() - nHleftspace - sizeText.cx ,nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nCurHeight + sizeText.cy);
		pDC->DrawText(strCurTime, textRect, DT_LEFT);  //д��ǰ��ӡ��ʱ��
	}

	//�¼��б�
	if(m_bPrintEvent)
	{
		nCurHeight = nBottom + nVspace / 2;
		//�����¼��б��ͷ
		DrawEventHead(pDC, nXoffset + nHleftspace, nCurHeight + nLineInterval, rectDraw,nCurSel);
		//�����¼���
		if(nCurSel != SHOW_ALL) //Ϊ�����ֵ�����������ߣ��˴���Ϊ�����Զ������߱������ʾ�¼���lzy2014.8.30
			DrawEventListPage2(pDC, nXoffset + nHleftspace, nCurHeight + nLineInterval, nLineInterval , rectDraw, m_stPrintDetail.curSelection);
		else
			DrawEventList2WayPage2(pDC, nXoffset + nHleftspace, nCurHeight + nLineInterval, nLineInterval , rectDraw, nCurSel);

		//�����ο�
		pDC->SelectObject(&m_dataFont);
		//strText = _T(" Event info ");
		strText = _T(" ") + m_stPrintDetail.stPrintEvent.strTitle + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		nBottom =  nCurHeight + (m_stPrintDetail.stPrintEvent.EventCount + 1) * (nLineInterval + nCharHeight) + nLineInterval;
		textRect.SetRect(nXoffset + nHleftspace , nCurHeight,nXoffset + rectDraw.Width() - nHleftspace, nBottom);
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CBrush *pOldBrush = pDC->SelectObject(pBrush);
		pDC->Rectangle(textRect);  //�����ο�
		pDC->SelectObject(pOldBrush);

		//������
		for(INT i = 1; i<m_stPrintDetail.stPrintEvent.EventCount +1; i++)
		{		
			pDC->MoveTo(nXoffset + nHleftspace , nCurHeight + nLineInterval / 2 + i * (nLineInterval + nCharHeight));
			pDC->LineTo(rectDraw.right - nHleftspace, nCurHeight + nLineInterval / 2 + i * (nLineInterval + nCharHeight));
		}
	}	

	pDC->SelectObject (pOldFont);
}

/****************************************************************
��������:             DrawEventList2WayPage2()                                                                               
����˵��:             ��˫�������¼��б���ڶ�ҳ                                                                            
��ڲ���:             nLeft ����㣬 nTop �����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-01                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::DrawEventList2WayPage2(CDC* pDC, INT nLeft, INT nTop , INT nVspace , CRect rectDraw, INT nCurSel)//�����¼��б�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	INT nLeftSpace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	INT nHspace = (rectDraw.Width() - 2 * nLeftSpace) / EVENT_LIST_COL;  //lzy2014.8.12
	INT nStartLine = 1;

	float preDistance=0;
	float fLossAvg = 0;
	//TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].stTestResultInfo;
	TestResultInfo* pData = &m_stPrintDetail.stTestResultInfo[nCurSel];  //lzy2014.8.30

	if (NULL == pData)
		return;

	CString strText=_T("Test Height");
	CSize sizeText = pDC->GetTextExtent(strText);
	INT nCharHeight = sizeText.cy;  //��ȡ����߶�
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//�ٶ�??
	INT nSize=ve.size();

	if(ve.size()<=EVENT_LIST_PRINT_MAX)
		return;
	else
		nSize = ve.size();

	m_stPrintDetail.stPrintEvent.EventCount = nSize-EVENT_LIST_PRINT_MAX;   //��������

	for (INT i=EVENT_LIST_PRINT_MAX;i<nSize;i++)//
	{
		// ���
		{
			textRect.left = nLeft;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace; lzy 2014.8.15

			//����
			strText.Format(_T("%d"),i+1);
			if (g_iLang == CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" �Ƿ���");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" ����");;//1�������¼�����
				}
			}
			else if (g_iLang == ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1�������¼�����
				}
			}

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		{
			textRect.left = nLeft + nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 2 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//����
			}
			else
			{
				//if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		//strText.Format(_T("%.3f"), 0.0);
		strText.Format(_T("--"));//Ϊ0ʱ��ʾ"-"
//���»�ȡA->B LOSS
		textRect.left = nLeft + 2 * nHspace;
		textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
		textRect.right = nLeft + 3 * nHspace;
		textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;

		GetEventInfo2Way(i,strText,0);
		pDC->DrawText(strText, &textRect,DT_CENTER);//����

		//strText.Format(_T("%.3f"), 0.0);
		strText.Format(_T("--"));//Ϊ0ʱ��ʾ"-"

		//���»�ȡB->A LOSS
		textRect.left = nLeft + 3 * nHspace;
		textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
		textRect.right = nLeft + 4 * nHspace;
		textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;

		GetEventInfo2Way(i,strText,1);
		pDC->DrawText(strText, &textRect,DT_CENTER);//����

		//���¸���ƽ�����
		textRect.left = nLeft + 4 * nHspace;
		textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
		textRect.right = nLeft + 5 * nHspace;
		textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
		
		GetEventInfo2Way(i,strText,2);
		pDC->DrawText(strText, &textRect,DT_CENTER);//����

		// �����
		{
			textRect.left = nLeft + 5 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 6 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fTotalLoss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//�����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// б��
		{
			textRect.left = nLeft + 6 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 7 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			//б��
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//Ϊ0ʱ��ʾ"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//б��
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ����
		{
			textRect.left = nLeft + 7 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 8 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].nEventType)//����
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		nStartLine++; //������1
	}
	pDC->SelectObject (pOldFont);
}
/****************************************************************
��������:             DrawEventListPage2()                                                                               
����˵��:             �������¼��б���ڶ�ҳ                                                                            
��ڲ���:             nLeft ����㣬 nTop �����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-01                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::DrawEventListPage2(CDC* pDC, INT nLeft, INT nTop , INT nVspace , CRect rectDraw, INT nCurSel)//�����¼��б�
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	INT nLeftSpace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	INT nHspace = (rectDraw.Width() - 2 * nLeftSpace) / EVENT_LIST_COL;  //lzy2014.8.12
	INT nStartLine = 1;

	float preDistance=0;

	TestResultInfo* pData = &m_stPrintDetail.stTestResultInfo[nCurSel];  //lzy2014.8.30

	if (NULL == pData)
		return;

	CString strText=_T("Test Height");
	CSize sizeText = pDC->GetTextExtent(strText);
	INT nCharHeight = sizeText.cy;  //��ȡ����߶�
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//�ٶ�??
	INT nSize=ve.size();

	if(ve.size()<=EVENT_LIST_PRINT_MAX)
		return;
	else
		nSize = ve.size();

	m_stPrintDetail.stPrintEvent.EventCount = nSize-EVENT_LIST_PRINT_MAX;   //��������
	for (INT i=EVENT_LIST_PRINT_MAX;i<nSize;i++)//
	{
		// ���
		{
			textRect.left = nLeft;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace; lzy 2014.8.15

			//����
			strText.Format(_T("%d"),i+1);
			if (g_iLang == CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" �Ƿ���");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" ����");;//1�������¼�����
				}
			}
			else if (g_iLang == ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0����Ƿ����¼�����
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1�������¼�����
				}
			}

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		{
			textRect.left = nLeft + nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 2 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//����
			}
			else
			{
				//if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ��
		{
			textRect.left = nLeft + 2 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 3 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			float a,b=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//��
			}
			else
			{
				//if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//�����굥λΪkm
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//��,���¼�������
			}
			if (0==i)
			{
				preDistance = 0.0f;
			}
			else
			{
				preDistance = ve[i].fLocation;
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ���
		{
			textRect.left = nLeft + 3 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 4 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fEventloss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"), ve[i].fEventloss);//���
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// �����
		{
			textRect.left = nLeft + 4 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 5 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fTotalLoss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//�����
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//�����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// б��
		{
			textRect.left = nLeft + 5 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 6 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			//б��
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//Ϊ0ʱ��ʾ"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//б��
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}
		// ����
		{
			textRect.left = nLeft + 6 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 7 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].nEventType)//����
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//����
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//����
		}

		nStartLine++; //������1
	}
	pDC->SelectObject (pOldFont);
}

/****************************************************************
��������:             GetEventInfo2Way()                                                                               
����˵��:             ��ȡ˫���б�ʱ����Ϣ                                                                            
��ڲ���:             nIndex ������ strText ��ʾ����                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-03                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CMyView::GetEventInfo2Way(INT nIndex,CString &strText,INT nType)
{
	CString strTemp;
	float fLossAvg = 0;
	//���»�ȡA-B�ߵ��¼�����
	std::vector<EventInfo> vAB;
	UINT uiEvents = m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo.size();  //B->A�ߵ����¼���
	UINT uiOffset = m_stPrintDetail.stTestResultInfo[1].DataPointsNum - m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo[uiEvents - 1].index;   //�ܵ���-����¼�������
	for(UINT i = 0; i != m_stPrintDetail.stTestResultInfo[0].EventList.vEventInfo.size(); i++ ) //�Ȱѵ�һ���ߵ��¼�����
	{
		vAB.push_back(m_stPrintDetail.stTestResultInfo[0].EventList.vEventInfo[i]);
		vAB[i].index += uiOffset;  //�����ݵ�ƽ��һ��
	}

	//���»�ȡB-A�ߵ��¼�����
	std::vector<EventInfo *> vBA;
	float fOffset = m_stPrintDetail.stTestResultInfo[1].Range -  m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo[uiEvents -1].fLocation;   //�ܾ���-����¼���λ��
	for(UINT i = 0; i != uiEvents; i++ )  //�ٰѵڶ����ߵ����ݷ�����ʱ����
	{
		EventInfo *pEvent = new EventInfo();
		*pEvent = m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo[uiEvents -1 - i];
		pEvent->fLocation = m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo[uiEvents -1].fLocation -  m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo[uiEvents -1 - i].fLocation;  //��һ���¼�Ϊ0
		pEvent->index = m_stPrintDetail.stTestResultInfo[1].DataPointsNum - m_stPrintDetail.stTestResultInfo[1].EventList.vEventInfo[uiEvents - 1 - i].index;	 //�����ˣ���һ���¼���Ϊ0	      

		vBA.push_back(pEvent);
	}

	//strTemp.Format(_T("%.3f"), 0.0);
	strTemp.Format(_T("--"));//Ϊ0ʱ��ʾ"-"

	for(std::vector<EventInfo>::size_type j=0; j!=vAB.size();j++)
	{
		if(abs(1.0 * vAB[j].index - m_stPrintDetail.stTestResultInfo[2].EventList.vEventInfo[nIndex].index) <= 5)
		{
		    if (0 == vAB[j].fEventloss)//Ϊ0ʱ��ʾ"-"
		    {
				strTemp.Format(_T("--"));//�����
			}
			else
			{
				strTemp.Format(_T("%.3f"), vAB[j].fEventloss);
			}
			fLossAvg = vAB[j].fEventloss;
			break;
		}
	}

	if(nType == 0)
		strText = strTemp;

	//strTemp.Format(_T("%.3f"), 0.0);
	strTemp.Format(_T("--"));//Ϊ0ʱ��ʾ"-"

	//���»�ȡB->A LOSS
	for(std::vector<EventInfo>::size_type j=0; j!=vBA.size();j++)
	{
		if(abs(1.0 * vBA[j]->index - m_stPrintDetail.stTestResultInfo[2].EventList.vEventInfo[nIndex].index) <= 5)
		{
		    if (0 == vBA[j]->fEventloss)//Ϊ0ʱ��ʾ"-"
			{
				strTemp.Format(_T("--"));//�����
			}
			else
			{
				strTemp.Format(_T("%.3f"), vBA[j]->fEventloss);
			}
			fLossAvg += vBA[j]->fEventloss;
			break;
		}
	}

	if(nType == 1)
		strText = strTemp;

	//���¸���ƽ�����
	if (0 == (fLossAvg / 2.0))//Ϊ0ʱ��ʾ"-"
	{
		strTemp.Format(_T("--"));//�����
	}
	else
	{
		strTemp.Format(_T("%.3f"), fLossAvg / 2.0);
	}

	if(nType == 2)
		strText = strTemp;

	vAB.clear();
	vBA.clear();
}

void CMyView::MemcpyQuestInfo(PrintQuest *pQuestInfo)
{
	//
	m_stPrintDetail.stPrintQuest.QuestID = pQuestInfo->QuestID;
	m_stPrintDetail.stPrintQuest.Contractor = pQuestInfo->Contractor;
	m_stPrintDetail.stPrintQuest.Customer = pQuestInfo->Customer;
	m_stPrintDetail.stPrintQuest.Player = pQuestInfo->Player;
	m_stPrintDetail.stPrintQuest.Comment = pQuestInfo->Comment;
}

/***************************************************************
��������:             DrawCursorAB()                                                                               
����˵��:             �����AB                                                                              
��ڲ���:             CDC                                                                                           
���ڲ���:             ��                                                                                        
����ʱ��:             2014-03-03                                                                  
Write By:             XJF                                                                                          
****************************************************************/
void CMyView::DrawCursorAB(CDC* pDC, CRect& rect)
{
	if(!g_cursorInfo.bDrawAB)
		return;
	//����AB��
	INT nMarginTop = rect.Height() / 10;//lzy2014.8.26
	CPen linePen;
	linePen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	CPen *oldPen = (CPen*)pDC->SelectObject(&linePen);
	CRect rectHead;  //�������򣬰�ABȦ����lzy2014.7.18
	int nHead = 2;//lzy2014.7.18
	//A��
	if(g_cursorInfo.fRatioA > 0)
	{
		INT ndxA = rect.left + g_cursorInfo.fRatioA * rect.Width();
		pDC->MoveTo(ndxA, rect.top + nMarginTop);
		pDC->LineTo(ndxA, rect.bottom);
		CString strText;
		strText.Format(_T("%s"), g_cursorInfo.strA);//���뼰��Ĳ��ڴ˴���ʾlzy2014.7.18, g_cursorInfo.strDistanceA,g_cursorInfo.strDBA);
		CSize sizeText = pDC->GetTextExtent(strText);//�õ��ı������
		CRect rectText;
		rectText.top = rect.top + nMarginTop;
		rectText.bottom = rectText.top + sizeText.cy;		

		if(ndxA + sizeText.cx + 1 <= rect.right)//�ı�������ʾ�����ұ�,1���ı������߼��
		{
			rectText.left = ndxA + 1;
			rectText.right = ndxA + 1 + sizeText.cx;
		}
		else//�ƶ����ұߣ����ı���ʾ���£�����ı���ʾ�����
		{
			//rectText.left = ndxA - (sizeText.cx + 1);
			//rectText.right = ndxA - 1;
			
		}//eif

		rectText.left = ndxA - sizeText.cx / 2 + 1;
		rectText.right = rectText.left + sizeText.cx + 1;

		rectHead.SetRect(rectText.left - nHead, rectText.top - nHead / 2, rectText.right + nHead, rectText.bottom + nHead / 2); //lzy2014.7.18
		pDC->Rectangle(&rectHead);//lzy2014.7.18
		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
	}
	//B��
	if(g_cursorInfo.fRatioB > 0)
	{
		//nMarginTop += 80;
		INT ndxB = rect.left + g_cursorInfo.fRatioB * rect.Width();
		pDC->MoveTo(ndxB, rect.top + nMarginTop);
		pDC->LineTo(ndxB, rect.bottom);
		CString strText;
		strText.Format(_T("%s"), g_cursorInfo.strB);//���뼰��Ĳ��ڴ˴���ʾlzy2014.7.18, g_cursorInfo.strDistanceB,g_cursorInfo.strDBB);
		CSize sizeText = pDC->GetTextExtent(strText);//�õ��ı������
		CRect rectText;
		rectText.top = rect.top + nMarginTop;
		rectText.bottom = rectText.top + sizeText.cy;

		rectText.left = ndxB - sizeText.cx / 2 + 1;
		rectText.right = rectText.left + sizeText.cx + 1;

		rectHead.SetRect(rectText.left - nHead, rectText.top - nHead / 2, rectText.right + nHead, rectText.bottom + nHead / 2); //lzy2014.7.18
		pDC->Rectangle(&rectHead);//lzy2014.7.18
		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
	}

	pDC->SelectObject(oldPen);
	DeleteObject(&linePen);

	/*
	g_sorFileArray.reportConfig.Trace.LocationA = g_cursorInfo.strDistanceA;   //A��λ��
	g_sorFileArray.reportConfig.Trace.LossA = g_cursorInfo.strDBA;  //A�����
	g_sorFileArray.reportConfig.Trace.LocationB = g_cursorInfo.strDistanceB;   //B��λ��
	g_sorFileArray.reportConfig.Trace.LossB = g_cursorInfo.strDBB;  //B�����
	float fABdistance = _wtof(g_cursorInfo.strDistanceA.GetBuffer()) - _wtof(g_cursorInfo.strDistanceB.GetBuffer()); //AB���� 
	g_cursorInfo.strDistanceA.ReleaseBuffer();
	g_cursorInfo.strDistanceB.ReleaseBuffer();
	float fABLoss = abs(_wtof(g_cursorInfo.strDBA.GetBuffer()) - _wtof(g_cursorInfo.strDBB.GetBuffer())); 
	g_cursorInfo.strDistanceA.ReleaseBuffer();
	g_cursorInfo.strDistanceB.ReleaseBuffer();
	float fABslope = fABdistance !=0 ? fABLoss / fABdistance : 0;
	g_sorFileArray.reportConfig.Trace.DistanceAB.Format(_T("%.3f"),fABdistance); //AB��ľ���	
	g_sorFileArray.reportConfig.Trace.LossAB.Format(_T("%.2f"), fABLoss); //AB�������	
	g_sorFileArray.reportConfig.Trace.SlopeAB.Format(_T("%.3f"), fABslope); //AB�������б��
	*/
}
