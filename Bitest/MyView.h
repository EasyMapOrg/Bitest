#pragma once

#include "Util.h"
#include "DATASTRUCT.h"
#include "global.h"

// CMyView ��ͼ

class CMyView : public CScrollView
{
	DECLARE_DYNCREATE(CMyView)

protected:

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);  
public:  
	virtual ~CMyView();
	CMyView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);  
	void OnFilePrintPreview(void);  
	void DrawPic(CDC* pDC);//�Զ��廭ͼ���� 

public:
	BOOL	m_bPrint;
	BOOL	m_bPrintCurve;
	CRect m_rectDraw;//���ƾ��������С

private:
	// Print 
	BOOL m_bPrintData;
	BOOL m_bPrintImage;
	BOOL m_bPrintHead;	
	UINT m_cxWidth;
	UINT m_cxOffset;
	UINT m_nLinesPerPage;
	UINT m_PageCount;
	UINT m_cyPrinter;
	CFont m_ListFont;
	CFont m_fontPrinter;
	CFont m_dataFont;
	CFont m_FontVR;
	CFont m_fontGroup;   //����������
public:
	BOOL m_bPrintLogo; //�Ƿ��ӡ��ά��Ȩ��Ϣ
	BOOL m_bPrintQuest;  //�Ƿ��ӡ����
	BOOL m_bPrintConfig; //�Ƿ��ӡ����
	BOOL m_bPrintComment;//�Ƿ��ӡע��
	BOOL m_bPrintTrace;//�Ƿ��ӡ����
	BOOL m_bPrintEvent;//�Ƿ��ӡ�¼�
	BOOL m_bShowGrid;//�Ƿ��ӡ�¼�

	//ghq 2016.4.21
	INT m_nCurveNumber;
	CUtil m_util;
	PrintDetail m_stPrintDetail;
	CString  strListHead[8];
    CString  strListHead2Way[8];
	//CBitestDlg *m_dlgParent; 
public:
	void OnMyPrint();
	void OnMyPrintPreview();
	void PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);  
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	void DrawGridLine(CDC* pDC, CRect& rect);//����դ��
	void GetPrintInfo(INT nCurSel); //��ȡ��ӡ��Ҫ�Ĳ���
	void DrawPrintContent(CDC* pDC, CRect rectDraw,INT nCurSel = 0, BOOL bSingle = TRUE); //���ƴ�ӡԤ������
	void DrawScale(CDC *pDC, CRect &rect, FLOAT fYmax = 0, FLOAT fXmax = 0);//���ƿ̶�
	void DrawCurve(CDC* pDC, CRect& rect, INT nCurveIndex);	
	void DrawTrace(CDC *pDC, CRect rc, INT nCurSel, FLOAT fontRatio = 1.0); //���ƴ���������
	void MemcpyResInfo(TestResultInfo *pResultInfo);
	void DrawConfig(CDC* pDC, CRect rectDraw,INT &nBottom,INT nCurSel);
	void MemcpyQuestInfo(PrintQuest *pQuestInfo);
	void DrawQuestInfo(CDC* pDC, CRect rectDraw,INT &nBottom,INT nCurSel);
	void DrawCursorAB(CDC* pDC, CRect& rect);
private:
	void InnerDraw(CDC* pDC,INT m_nCurPage); //�ڲ���ͼֻ��
	void DrawEventHead(CDC* pDC, INT nLeft, INT nTop, CRect rectDraw,INT nCurSel);//�����¼��б��ͷ
	void DrawEventList(CDC* pDC, INT nLeft, INT nTop, INT nVspace, CRect rectDraw, INT nCurSel);//�����¼��б�
    void DrawEventList2Way(CDC* pDC, INT nLeft, INT nTop , INT nVspace , CRect rectDraw, INT nCurSel);//����˫���¼��б�	
	void InitFont(CDC* pDC); //��ʼ������
	void InitPrintText();
	void DrawCurve2Way(CDC* pDC, CRect& rect, INT nCurveIndex);
	void DrawEvent(CDC* pDC, CRect& rect, INT nCurveIndex);
	void DrawEvent2Way(CDC* pDC, CRect& rect, INT nCurveIndex);
	void GetEvent2Way(TestResultInfo *pInfo2Way, TestResultInfo *infoA2B, TestResultInfo *infoB2A);
	void DrawScale2Way(CDC *pDC, CRect &rect, FLOAT fXmin, FLOAT fXmax);

	void DrawPrintContentPage2(CDC* pDC, CRect rectDraw, INT nCurSel, BOOL bSingle = TRUE);
	void DrawEventListPage2(CDC* pDC, INT nLeft, INT nTop, INT nVspace, CRect rectDraw, INT nCurSel);//�����¼��б�
	void DrawEventList2WayPage2(CDC* pDC, INT nLeft, INT nTop , INT nVspace , CRect rectDraw, INT nCurSel);//����˫���¼��б�	
	void GetEventInfo2Way(INT nIndex,CString &strText,INT nType);
public:
//	afx_msg void OnPrint();
};


