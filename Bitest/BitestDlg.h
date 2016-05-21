// BitestDlg.h : ͷ�ļ�
//

#pragma once
#include "GrandwayReg.h"
#include "VARIABLE.h"
#include "Util.h"
#include "DATASTRUCT.h"
#include "SORDATA.h"
#include "afxwin.h"
#include "OtdrTrace.h"
#include "afxcmn.h"
#include "global.h"
#include "PageOpen.h"
#include "PageSetup.h"
#include "GaborDllUtil.h"

	//void DRWAPRINTER(CDC *pDC,CPrintInfo *pInfo,void *pVoid);
// CBitestDlg �Ի���
class CBitestDlg : public CDialog
{
// ����
public:
	CBitestDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CBitestDlg();
// �Ի�������
	enum { IDD = IDD_BITEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnOpenFile();
	afx_msg void OnRadioShow(UINT uID);    //select show type

//�Զ���
private:
	afx_msg void OnButtonHelp();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedButtonOrig();
	afx_msg void OnBnClickedButtonZoomin();
	afx_msg void OnBnClickedButtonZoomout();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedLang();
	afx_msg void OnChs();
	afx_msg void OnEng();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnPrint();
	afx_msg void OnPreview();
	afx_msg void OnBnClickedButtonZoominH();
	afx_msg void OnBnClickedButtonZoomoutH();
	afx_msg void OnBnClickedButtonZoominV();
	afx_msg void OnBnClickedButtonZoomoutV();
	afx_msg void OnSetup();
	afx_msg void OnBnClickedRadioAb();
	afx_msg void OnBnClickedRadioAorb();
	afx_msg void OnBnClickedAddEvent();
	afx_msg void OnBnClickedDelEvent();
	afx_msg void OnBnClickedCheckCursorab();

	LRESULT OnBeginPrinting(WPARAM wParam,LPARAM lParam);
	LRESULT OnEndPrinting(WPARAM wParam,LPARAM lParam);
	BOOL SaveLangSetting();
	INT GetZoomType();

	void resize();
	void SetEvents(TestResultInfo& testResultInfo, CGraphElement* graphElement);
	void OnOpenFileOk();
	void InitList();//�����б��ʼ��
	void InitList2Way();//˫������¼��б��ʼ��
	void OnBnClickedButtonZoomAny(INT nIndex);
	void OnInitPositon();
	void OnInitBtnIcon();
	void LoadLangSetting();
	void SetLocaleString();
	//�������,��c/2nf��ʽ����.xpoint:��λ�ü�1��Frequency:Ƶ�ʣ� Refraction:������
	float GetDistance(unsigned int xpoint, FLOAT Frequency, FLOAT Refraction);
	//����һ������,��sorDataת��Ϊ������ʾ������
	void DealFullData(OTDR_ALL_UPLOAD_DATA* allData, SORDATA* sorData, ShowType myType);
	//��ȡ�¼�������
	CString GetEventType(UINT iType, char chType);	
	//��ʾһ������
	void ShowTrace(ShowType myType);
	//��ʾ�б�
	void ShowEvents();
	//��ʾ�б�,˫�����е��¼�
	void ShowEvents2Way();	
	//������ͼ��������
	void SetCoordinate(CoordinateCoef xyCoef);
	//˫����Ժ󣬴���һ���¼�����Ϣ���γ�˫������¼�
	void GetEvent2Way(TestResultInfo *pInfo2Way,      //˫������¼�
					  TestResultInfo *infoA2B,     //A->B�����¼�
					  TestResultInfo *infoB2A);    //B->A�����¼�

	//��ʾ����
	void ShowOnceData(POTDR_ALL_UPLOAD_DATA pOtdrAllUploadData);
	//��ʾ˫������
	void ShowBidirectional();
	//�����¼��б�
	void FreshEventList();
	//������������
	void CreateAboutDlg();
    void CreateOpenFileDlg();
	void CreateSetupDlg();
	void SetABType(INT nType);
	void GaborResultToEvent(TestResultInfo* pCurveData);
	float Get_XCoord(unsigned int xpoint,TestResultInfo* pData);
	void EventToGaborResult(TestResultInfo* pCurveData);
    void GetEvent2WayIndex(TestResultInfo *infoA2B, TestResultInfo *infoB2A,INT nPoints,INT &nDelIndex1,INT &nDelIndex2);
	INT GetEvent2WayIndex2(INT nIndex);
public:
	//���ݴ���������ʾ�����ߣ��Լ��¼�����Ϣ
	void DataProcess(OTDR_ALL_UPLOAD_DATA* allData, TestResultInfo* testInfo, ShowType myType);
	void OnDown(UINT nID);
	void OnUp(UINT nID);
	void SetFileText(CString strFileName,INT nIndex);
	void OnSetQuestInfo(PrintQuest printQuest);
	void ChangeABStatus(BOOL bSelected);
	void OnResetEventList();
	void ResetEventButtonStatus();	
private:
	POINT    m_ptMin;
	CButton m_btnFile;
	CButton m_btnPrint;
	CButton m_btnLang;
	CButton m_btnSetup;
	CButton m_btnAbout;
	CButton m_btnExit;

	CButton m_btnOriginal;
	CButton m_btnZoomin;
	CButton m_btnZoomout;
	CButton m_btnZoomInH;
	CButton m_btnZoomOutH;
	CButton m_btnZoomInV;
	CButton m_btnZoomOutV;

	CRect m_rtCurve;
	CRect m_rtEvent;

	CRect m_rtBtnFile;
	CRect m_rtBtnPrint;
	CRect m_rtBtnLang;
	CRect m_rtBtnSetup;
	CRect m_rtBtnAbout;
	CRect m_rtBtnExit;

	CRect m_rtFile1;
	CRect m_rtFile2;

	CRect m_rtBtnReset;
	CRect m_rtBtnZoominB;
	CRect m_rtBtnZoomoutB;
	CRect m_rtBtnZoominH;
	CRect m_rtBtnZoomoutH;
	CRect m_rtBtnZoominV;
	CRect m_rtBtnZoomoutV;

	CRect m_rtBtnAddevent;
	CRect m_rtBtnDelevent;

	CRect m_rtRadioA2B;
	CRect m_rtRadioB2A;
	CRect m_rtRadioBoth;

	CRect m_rtCheckAB;
	CRect m_rtRadioAandB;
	CRect m_rtRadioAorB;

	BOOL m_bZoomin;
	BOOL m_bZoomout;
	BOOL m_bZoominH;
	BOOL m_bZoomoutH;
	BOOL m_bZoominV;
	BOOL m_bZoomoutV;

	BOOL	m_bPrintData;
	BOOL	m_bPrintImage;
	BOOL	m_bPrintHead;
	UINT m_cxWidth;
	UINT m_cxOffset;
	UINT m_nLinesPerPage;
	UINT m_cyPrinter;
	CFont		m_ListFont;
	CFont		m_fontPrinter;
	CButton m_radioAB;
	CButton m_radioBA;
	CButton m_radioBoth;
	CStatic m_txtFile1;
	CStatic m_txtFile2;

	CToolTipCtrl m_Mytip;
	CEventList m_listEvent;
	CImageList m_listImage;
	CPageOpen  *m_pdlgOpen;
	CPageSetup  *m_pdlgSetup;

	CGrandwayReg  m_reg;//ע��������
	OTDR_ALL_UPLOAD_DATA m_allData[3];  //0��ʾa->b, 1��ʾb->a, 2��ʾ˫������
	CString  m_strPath[3];  //�����ļ�·��, 0ΪA2B, 1ΪB2A, 2Ϊ˫������
	COtdrTrace m_otdrTrace;// �������ߵĿؼ�
	CUtil m_util;
	int m_iRadioShow;	// ��ʾ��ģʽ
	CoordinateCoef m_xyCoef;//��������
	//CEndGabor m_endgabor;
	CSorAssist m_sorAssist;
	CGaborDllUtil g_gaborDllUtil;//ghq.2016.05.12
public:
	//�����������
	TestResultInfo m_testResult[3];
	PrintQuest m_printQuest;
	BOOL m_bCursorAB;
	INT m_nSelectCursor; //Seleced��־,1ѡ��A��2ѡ��B, 4ѡ��AB
};
