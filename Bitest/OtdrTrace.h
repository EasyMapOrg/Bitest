#pragma once
#include <math.h>
#include <vector>
#include "Util.h"
#include "global.h"
#include "afxcmn.h"

///////////////////////////////////////////////////////////
//CPoint ���ݵ�
class CElementPoint//
{

public:
	double x, y;
	CElementPoint(){ x=y=0;}
	CElementPoint(double c1, double c2)
	{
		x = c1;
		y = c2;    
	}//eif
	CElementPoint& operator=(const CElementPoint& pt)
	{
		x = pt.x;  y = pt.y;
		return *this;
	}//eif
	CElementPoint (const CElementPoint& pt)
	{
		*this = pt;
	}//eif
};
///////////////////////////////////////////////////////////
//ע��
class CGraphAnnotation 
{
public:
	CString   m_Caption;//�ı�
	COLORREF m_Color, m_BkColor;//��ɫ
	BOOL m_bHorizontal;//�Ƿ�ˮƽ
	BOOL m_bVisible;//�Ƿ�ɼ�
	CElementPoint place;//λ��

	CGraphAnnotation () 
	{
		m_Caption = _T("");
		m_Color = RGB(255,255,255);
		m_BkColor = RGB(120,120,120);
		m_bHorizontal = TRUE;
		m_bVisible = TRUE;
		place = CElementPoint(0.5,0.5);
	}//eif
};

///////////////////////////////////////////////////////////
//����
class CGraphElement
{
public:
	CGraphElement()
	{
		bIsPlotAvailable = FALSE;
		m_LineColor = RGB(0,0,0);
		m_bShow = TRUE;
		//m_strName.Format(_T("Element-%d"),id);
		m_pData = NULL;
		m_pPointsToDraw = NULL;
		m_nOffsetY = 0;
		m_fOffsetX = 0;
		m_nPointsCount = 0;
	}//eif
	struct TraceEvent
	{
		INT m_nIndex;
		FLOAT m_distance;
	};
	INT  DistanceToIndex(FLOAT fDistance);//���ݾ���ת��Ϊ��Ӧ�����ݵ��±�,һ������ȷ��ÿ�λ�ͼ����ʼ�㼰������
	//void DrawCurve(CDC* pDC, CRect rect);//��������	
	//void DrawCurve2Way(CDC* pDC, CRect rect);//��������, ˫�������
	void DrawCurve(CDC* pDC, CRect rect,int nIndexCurve);
	void DrawCurve2Way(CDC* pDC, CRect rect,int nIndexCurve);
	void DrawEvents(CDC* pDC, CRect rect);//�����¼���	
    void DrawEvents2Way(CDC* pDC, CRect rect); //�����¼���,˫�����
	BOOL m_bShow;//�Ƿ���ʾ
	BOOL bIsPlotAvailable;//�Ƿ�ɻ�
	BOOL m_bShowEvent;//�Ƿ���ʾ�¼��б�
	BOOL m_bSelected;//�Ƿ���ѡ������
	COLORREF m_LineColor;//������ɫ
	CString m_strName;//����
	INT m_nPointsCount;//�ܹ��ж��ٸ����ݵ�,��ǰ����ʵ�ʵ���
	//INT m_nPointsCountVirtual;//�������ߵ�������Ҫ��Ϊ�˶���������ʾ�������������Ϊ׼���㵱ǰ����Ӧ���ж��ٸ���(�ȵ�ǰʵ�ʵĵ�����)ʱ���õ��ĳ��ȵ�����������߳���
	INT m_nOffsetY;//y��ƫ����
	FLOAT m_fLength;//����
	FLOAT m_fOffsetX;//x��ƫ����
	static FLOAT m_fRangeX[2];//���浱ǰ��ͼʱ��С������,���x�ᣬ��Զ�������
	static FLOAT m_fRangeY[2];//���浱ǰ��ͼʱy�᷶Χ����Զ�������
	static FLOAT m_fMaxDistance;//����룬��Զ�������
	FLOAT m_fDataSpacing;//���Ϊ��������,������������
	FLOAT* m_pData;//�㼯��
	CPoint* m_pPointsToDraw;//
	std::vector<TraceEvent> m_vAllEvents;

	//ghq 2016.4.12
	INT m_iSelEventLine;
	BOOL SelectLine( const CPoint  pt,CRect rtCurve, INT iSelMode);
	INT GetSelectLine( );
	void SetSelectLine(INT iSelEventLine );
	void DrawScale(CDC *pDC,CRect rect,INT nType);
	INT GetSelectEventPos();
};

// COtdrTrace
class CBitestDlg;
class COtdrTrace : public CButton
{
	DECLARE_DYNAMIC(COtdrTrace)

public:
	COtdrTrace();
	virtual ~COtdrTrace();
public:
	struct CursorAB
	{
		CursorAB()
		{
			m_nPos = 0;
			m_fPos = 0;
			m_colorLine = RGB(0,0,255);//��ѡ��A�����ʱBΪm_colorLine��ѡ��B�����ʱAΪm_colorLine
			m_colorLabel = RGB(255,255,255);//A��B�����Աߵ��ı������¼������Աߵ��ı�
		}
		INT m_nPos;//λ�ã������Ļ����������
		FLOAT m_fPos;//λ�ã�����km,�������λ��
		CString m_strAOrB;//"A"/"B"
		CString m_strDistance;//���پ��룬��100km���������ʾ���ַ���
		CString m_strDB;// ���� AB����dBֵ
		COLORREF m_colorLine;//������ɫ
		COLORREF m_colorLabel;//�������ı�����A 100km��ɫ
	}m_cursorA,m_cursorB,m_cursorAminusB,m_cursorEvent;//
public:
	void InitPaint(CDC* pDC);//��ʼ����ͼ�õı�ˢ��
	void DoDraw(CDC* pDC);
	void DrawMouseSeleRect(CDC* pDc);//��������϶�����
	void DrawCurves(CDC* pDC);//������������	
	void DrawCurve2Way(CDC* pDC);//����˫���������
	void CalculateRect();//�����ͼ����
	void SetRange(float xmin, float xmax, float ymin, float ymax, BOOL bSetScrollInof = TRUE);//���û�ͼ���ޣ���Ҫ�ػ�
	void ReleaseAllData();//�����������2013.08.14

	void SetVerScoll(); //������ֱ������
	void SetHorScroll();//����ˮƽ������
	void InitScrollInfo();//��ʼ��������
	void SetMyScrollInfo(float xmin, float xmax, float ymin, float ymax);//���ù�������Χ
	void SetScrollRangeX();//����ˮƽ����Χ
	void SetMaxDistance(FLOAT fMaxDistance);
	void SetMaxTraceY(INT nShowType);
	//ghq 2016.5.10 ����AB���
	void OnInitAB(void);
	void DrawLineAB(CDC* pDC);
	BOOL IsInCurRangeX(FLOAT fDistance);
	BOOL IsInCurRangeY(FLOAT fDistance);
	void DrawLineABDetail(CDC* pDC, CursorAB* pCursorAB);
	void SetCurveCursorData();
	void CursorABOnMove(CPoint pointMoving);
	void CreateMyFont(CDC* pDC, CFont& font, CString fontName);
	void OnShowAB();
	INT GetABSelectState(CPoint pointClick);
	void ShowCatch(CDC *pDC, CPoint pt);
	void SetCurrentTypeAB(INT nABType);
	void SetABSelStatus(INT nType);
	INT GetCurrentCursorIndex();
	//e
public:
	CRect m_rtMouseSelect;//���ѡ������cwcq
	CRect m_rtClient;//
	CRect m_rtCurve; //���߻�ͼ�����С
	BOOL m_bLeftButtonDown;
	BOOL m_bInitPaint;//�Ƿ��ѽ��й���ʼ����ˢ��
	INT m_nCurMode;//ģʽ
	CDC m_dcGrid;//������DC
	CDC m_dcCurve;//��ͼDC�ֱ���
	CBitmap m_bmpGrid;
	CBitmap m_bmpCurve;
	FLOAT m_fResolutionY, m_fResolutionX;//(��Ļÿһ����ռ���ٸ����ݵ�/ÿһ���ض�Ӧ���߶೤����)
	FLOAT m_fRangeY[2], m_fRangeX[2], m_fBiRangeX[2];//�����Сֵ
	FLOAT m_fOldRangeX[2], m_fOldRangeY[2];//ԭ���������Сֵ,������С
	std::vector<CGraphElement> m_vAllData;//��������
	//ghq 2016.5.10 ����AB���
	BOOL m_bShowLineAB;//�Ƿ���ʾA/B���
	BOOL m_bDrawGrid;//�Ƿ���Ƹ�����(GridLine)//zll 2013.4.22
	INT  m_nCurABSele;//��ǰ�Ƿ�ѡ��AB�꣬���ѡ�У�ѡ����A����B
	INT  m_nABSele;//��ǰA����B��Ϊ��������
	INT  m_nCurveIndex;//��ǰѡ�������±�
	INT  m_nABType;//���ѡ��ģʽ����ǰֻѡһ�����(A/B)��ͬʱѡ����(AB,�϶�һ����һ��Ҳ�ᶯ)
	//e
	INT m_nCount;
public:
	CFont m_font;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	//ghq 2016.4.12
	INT m_iSelEvent;
	CUtil m_util;
	CBitestDlg *m_dlgParent;
	FLOAT m_fMaxDistance;//����룬��Զ�������
	INT m_iSelMode;//��ǰѡ����/˫��ģʽ
    CPoint m_ptMouse;  //��������λ��lzy2014.9.9
	int m_nSelectCursor; //Seleced��־,1ѡ��A��2ѡ��B, 4ѡ��AB
	int m_nPointA;//A�ߵ�λ�õ㣬������ ������Ϊ�޷�����,  //ʼ��������������Ϊ׼wcq
	int m_nPointB;//B�ߵ�λ�õ㣬������  //ʼ��������������Ϊ׼wcq
	//������
	int m_nHorzPos;
	FLOAT m_nVertPos;
	int m_nHorzMaxSize;
	FLOAT m_nVertMaxSize;
	int m_nHorzPageSize;
	FLOAT m_nVertPageSize;
	int m_nHorzOneStep;
	FLOAT m_nVertOneStep;
	int m_nOneStep;
	//
	BOOL m_bShowScale;  //��ʾ����lzy2014.9.9
	WORD m_wdGridH;  //ˮƽ��ָ���
	WORD m_wdGridV;  //��ֱ��ָ���

	afx_msg void OnSize(UINT nType, int cx, int cy);
	//ghq 2016.4.15
	void ZoomInHorizonForBig(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);//ˮƽ�Ŵ� fPercent�Ŵ������Ĭ��2%
	void ZoomOutHorizonForSmall(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);//ˮƽ��С fPercent�Ŵ������Ĭ��2%
	void ZoomInVertialForBig(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);//��ֱ�Ŵ� fPercent�Ŵ������Ĭ��2%
	void ZoomOutVertialForSmall(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);//��ֱ��С fPercent�Ŵ������Ĭ��2%
	void ZoomInBothForBig(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);
	void ZoomOutBothForSmall(FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT);
	void ZoomReset();//��ԭ��ˮƽ��ֱ����Ϊ
	void ZoomInClickForBig(INT nPointX=0, INT nPointY=0, FLOAT fPercent = OTDR_TRACE_DEFAULT_ZOOM_PERCENT * 5);//�����Ŵ�Ĭ�ϷŴ�10%
	FLOAT GetCenterOffset();
	BOOL IsCurrentTypeAB();
	FLOAT GetYDB(FLOAT fPos);
	FLOAT GetMaxDistance();
	void SetRadioType(INT iSelMode);
	void RefreshCurve();
	void MoveToEvent();
	void DrawGrid(CDC *pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

class CEventList :
	public CListCtrl
{
public:
	CEventList(void);
	~CEventList(void);
public:
	CBitestDlg *m_dlgParent;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};


