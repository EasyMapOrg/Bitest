#pragma once

#include "Util.h"
#include "DATASTRUCT.h"
//#include "BitestDlg.h"

// CMyView ��ͼ
const float FONTSIZE_RATIO[5] = {1.0, 0.8, 0.6, 0.45, 0.4};  //�������ʱ�õ���������ű�����[0]��ʾ������[1]��ʾ2ͼ/ҳ��[2]��ʾ4ͼ/ҳ��[3]��ʾ6ͼ/ҳ��[4]��ʾ8ͼÿҳ

typedef struct PrintQuest 
{	
	CString strTitle;//�������
	CString strID;//������
	CString strContractor; //�а���
	CString strCustomer;  //�û�
	CString strSorFileName; //�ļ���
	CString strTestDate;   //��������
	CString strPlayer;   //����Ա

	//value
	CString QuestID;    //������
	CString Contractor; //�а���
	CString Customer;  //�û�
	CString SorFileName; //�ļ���
	CString TestDate;   //��������
	CString Player;   //����Ա
};

typedef struct PrintConfig 
{	
	//name
	CString strTitle;  //��������
	CString strWavelength;   //��������
	CString strPulse;    //����
	CString strOrigin;    //���
	CString strOriCable;   //�����º�
	CString strOriFiber;   //�����˺�
	CString strOriColor;   //��������ɫ

	CString strRange;    //����
	CString strAverage;  //ƽ���Ĵ���
	CString strEnd;     //�յ�
	CString strEndCable;   //�յ���º�
	CString strEndFiber;   //�յ���˺�
	CString strEndColor;   //�յ���ɫ
	CString strOriLocation;//���λ��
	CString strEndLocation;   //�յ�λ��

	CString strBackScatter;  //����ɢ��ϵ��
	CString strLossThreshold;  //˥������
	CString strRefThreshold;  //��������
	CString strEndThreshold;  //����ĩ������
	CString strIndex;    //����ʱ�õ�������
	CString strResolution;  //������
	CString strTitleAB;  //AB����
	CString strTitleBA;  //BA����
	//value
	CString OriCable;   //�����º�
	CString OriFiber;   //�����˺�
	CString OriColor;   //��������ɫ
	CString EndCable;   //�յ���º�
	CString EndFiber;   //�յ���˺�
	CString EndColor;   //�յ���ɫ
	CString OriLocation;//���λ��
	CString EndLocation;   //�յ�λ��
	CString Wavelength;   //��������
	CString Index;    //����ʱ�õ�������
	CString Pulse;    //����
	CString Range;    //����
	CString Average;  //ƽ���Ĵ���
	CString Resolution;  //������
	CString BackScatter;  //����ɢ��ϵ��
	CString LossThreshold;  //˥������
	CString RefThreshold;  //��������
	CString EndThreshold;  //����ĩ������
};

typedef struct PrintComment 
{
	CString strTitle; //ע�ͱ�������
	CString strComment;   //ע��
};

typedef struct PrintTrace
{	
	CString strTitle;  //�켣�߱���
	CString LocationA;   //A��λ��
	CString LossA;    //A�괦���
	CString LocationB;  //B��λ��
	CString LossB;  //B�괦���
	CString DistanceAB;  //ABλ�ü��
	CString LossAB;  //AB��Ĳ�
	CString SlopeAB;  //AB�����б��
};

struct WaveConfig 
{
	COLORREF ColorCurve1;  //����1��ɫ
	COLORREF ColorCurve2;  //����2��ɫ
	COLORREF ColorBack;  //������ɫ
	COLORREF ColorGrid;  //դ����ɫ
	COLORREF ColorEvent;  //�¼���ɫ
	COLORREF ColorCursor;  //�����ɫ
	BOOL ShowGrid; //�Ƿ���ʾ����
	BOOL ShowCursor; //�Ƿ���ʾ���
	BOOL ShowDoubleCursor; //�Ƿ���ʾ˫���
	BOOL MultiMode; //�Ƿ�Ϊ��������ʾģʽ
	//Unit Page
	int UnitDbFig; //Dt�̶ȵ�λ����5\4\3
	int UnitDistFig; //����̶ȵ�λ����
	CString DistUnit; //���뵥λ
};

typedef struct PrintEvent 
{	
	CString strTitle;
	int EventColumn;    //�¼��б�����
	int EventCount; //�¼�����
};

//���еĴ�ӡ��Ϣ
struct PrintDetail
{
	CString strTitle;
	CString strLogo;
	PrintQuest stPrintQuest;
	PrintComment stComment;
	PrintConfig stPrintConfig; //������Ϣ
	PrintTrace stPrintTrace;
	WaveConfig stWaveConfig;
	PrintEvent stPrintEvent;
	TestResultInfo stTestResultInfo[3];
	INT curSelection;//�ļ��б�ǰѡ���ĸ�
	INT totalCurveNumber;
	CString WaveList;
};


class CMyView : public CScrollView
{
	DECLARE_DYNCREATE(CMyView)

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
public:
//	afx_msg void OnPrint();
};


