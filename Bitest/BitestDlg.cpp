// BitestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Bitest.h"
#include "BitestDlg.h"
#include "PageOpen.h"
#if MY_PRINT_TEST
#include "MyFrame.h"
#else
#include "PrintFrame.h"
#endif
#include "SorDllUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//ghq 2016.4.29 debug��release���ò�ͬ��Lib��
/*
#ifdef _DEBUG
#pragma comment(lib,"..\\debug\\SOR.lib")
#pragma comment(lib,"..\\debug\\GABOR.lib")
#else
#pragma comment(lib,"..\\release\\SOR.lib")
#pragma comment(lib,"..\\release\\GABOR.lib")
#endif
*/

//SorFileArray g_sorFileArray;

CSorDllUtil g_sorUtil;
CString str_OkorCancel=_T("");
CString str_max_curve=_T("");
CString str_open_file_error=_T("");
CString str_open_data_invalid=_T("");
extern LANG g_iLang;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()


// CBitestDlg �Ի���




CBitestDlg::CBitestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBitestDlg::IDD, pParent)
	, m_iRadioShow(0)
	,m_pdlgOpen(NULL)
	,m_pdlgSetup(NULL)
	, m_bCursorAB(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nSelectCursor = CURSOR_SELECT::CURSOR_A; //Ĭ��ѡ��A
	for(int i = 0; i < 3; i++)
	   m_strPath[i] = _T("");
}

CBitestDlg::~CBitestDlg()
{
	//����ɾ������
	if (!m_pdlgOpen)
	{
		delete m_pdlgOpen;
		m_pdlgOpen = NULL;
	}
	if (!m_pdlgSetup)
	{
		delete m_pdlgSetup;
		m_pdlgSetup = NULL;
	}
	for(int i = 0; i < 3; i++)
	{
		if (m_allData[i].PData)
		{
            delete[] m_allData[i].PData;
			m_allData[i].PData = NULL;
		}

		m_testResult[i].EventList.vEventInfo.clear();
	}
}

void CBitestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TRACE, m_otdrTrace);
	DDX_Control(pDX, IDC_LIST_EVENT, m_listEvent);
	DDX_Radio(pDX, IDC_RADIO_A2B, m_iRadioShow);
	DDX_Control(pDX, IDC_BUTTON_ORIG, m_btnOriginal);
	DDX_Control(pDX, IDC_BUTTON_ZOOMIN, m_btnZoomin);
	DDX_Control(pDX, IDC_BUTTON_ZOOMOUT, m_btnZoomout);
	DDX_Control(pDX, IDC_BUTTON_ZOOMIN_H, m_btnZoomInH);
	DDX_Control(pDX, IDC_BUTTON_ZOOMOUT_H, m_btnZoomOutH);
	DDX_Control(pDX, IDC_BUTTON_ZOOMIN_V, m_btnZoomInV);
	DDX_Control(pDX, IDC_BUTTON_ZOOMOUT_V, m_btnZoomOutV);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btnFile);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BUTTON_LANG, m_btnLang);
	DDX_Control(pDX, IDC_BUTTON_ABOUT, m_btnAbout);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_RADIO_A2B, m_radioAB);
	DDX_Control(pDX, IDC_RADIO_B2A, m_radioBA);
	DDX_Control(pDX, IDC_RADIO_BIDIRECTIONAL, m_radioBoth);
	DDX_Control(pDX, IDC_STATIC_FILE1, m_txtFile1);
	DDX_Control(pDX, IDC_STATIC_FILE2, m_txtFile2);
	DDX_Control(pDX, IDC_BUTTON_SETUP, m_btnSetup);
	DDX_Check(pDX, IDC_CHECK_CURSORAB, m_bCursorAB);
}

BEGIN_MESSAGE_MAP(CBitestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CBitestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CBitestDlg::OnOpenFile)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_A2B, IDC_RADIO_BIDIRECTIONAL, OnRadioShow)   //��ͬradio���Ʋ�ͬ����ʾ��ʽ
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CBitestDlg::OnButtonHelp)
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_ORIG, &CBitestDlg::OnBnClickedButtonOrig)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN, &CBitestDlg::OnBnClickedButtonZoomin)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT, &CBitestDlg::OnBnClickedButtonZoomout)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_LANG, &CBitestDlg::OnBnClickedLang)
	ON_COMMAND(ID_CHS, &CBitestDlg::OnChs)
	ON_COMMAND(ID_ENG, &CBitestDlg::OnEng)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CBitestDlg::OnBnClickedButtonPrint)
	ON_COMMAND(ID_PRINT, &CBitestDlg::OnPrint)
	ON_COMMAND(ID_PREVIEW, &CBitestDlg::OnPreview)
	//	ON_MESSAGE(WM_BEGIN_PRINTING,OnBeginPrinting)
	//ON_MESSAGE(WM_END_PRINTING,OnEndPrinting)
	//ON_MESSAGE(WM_MY_PRINT,OnMyPrint)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN_H, &CBitestDlg::OnBnClickedButtonZoominH)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT_H, &CBitestDlg::OnBnClickedButtonZoomoutH)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN_V, &CBitestDlg::OnBnClickedButtonZoominV)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT_V, &CBitestDlg::OnBnClickedButtonZoomoutV)
//	ON_WM_MOUSEMOVE()
    ON_BN_CLICKED(IDC_BUTTON_SETUP, &CBitestDlg::OnSetup)
    ON_BN_CLICKED(IDC_CHECK_CURSORAB, &CBitestDlg::OnBnClickedCheckCursorab)
    ON_BN_CLICKED(IDC_RADIO_AB, &CBitestDlg::OnBnClickedRadioAb)
    ON_BN_CLICKED(IDC_RADIO_AORB, &CBitestDlg::OnBnClickedRadioAorb)
	ON_BN_CLICKED(IDC_BUTTON_ADD_EVENT, &CBitestDlg::OnBnClickedAddEvent)
	ON_BN_CLICKED(IDC_BUTTON_DEL_EVENT, &CBitestDlg::OnBnClickedDelEvent)
END_MESSAGE_MAP()


// CBitestDlg ��Ϣ�������

BOOL CBitestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CreateAboutDlg();

	//���ô˶Ի����ͼ��.��Ӧ�ó��������ڲ��ǶԻ���ʱ,��ܽ��Զ�ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//��ʼ���¼��б�
	//m_pImageList = new CImageList();
	m_listImage.Create(16,16,ILC_COLOR32 | ILC_MASK, 1, 0);//��ʾ�ļ��л��ļ�
	m_listImage.Add(AfxGetApp()->LoadIcon(IDI_EVENT_NOREFLECT));
	m_listImage.Add(AfxGetApp()->LoadIcon(IDI_EVENT_REFLECT));
	m_listEvent.SetImageList(&m_listImage, LVSIL_SMALL);
	
	InitList();
	
	//ghq ��ȡԭʼ���ڴ�С
	CRect rect;    
	GetClientRect(&rect);     //ȡ�ͻ�����С  
	m_ptMin.x=rect.right-rect.left;
	m_ptMin.y=rect.bottom-rect.top;

	//���ذ�ťͼ��
	OnInitBtnIcon();
	//��ʼ��λ����Ϣ
	OnInitPositon();

	m_otdrTrace.m_dlgParent = this;
	m_listEvent.m_dlgParent = this;

	//��������
	CreateOpenFileDlg();
	CreateSetupDlg();

	//gabor��SORDLL��ʼ��
    g_gaborDllUtil.Init();
	g_sorUtil.Init();

	//������
//	m_otdrTrace.InitScrollInfo();

	//��ʾ
	m_Mytip.Create(this);

	//�Ŵ���С��ť״̬
	m_bZoomin = FALSE;
	m_bZoomout = FALSE;
	m_bZoominH = FALSE;
	m_bZoomoutH = FALSE;
	m_bZoominV = FALSE;
	m_bZoomoutV = FALSE;

	//����
	g_iLang = CHS;
	LoadLangSetting();
	SetLocaleString();

	//��ʼ�����AB
	GetDlgItem(IDC_RADIO_AB)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO_AORB))->SetCheck(BST_CHECKED);
    GetDlgItem(IDC_RADIO_AORB)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BUTTON_ADD_EVENT)->EnableWindow(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CBitestDlg::OnInitPositon()
{
	m_rtCurve.SetRect(RECT_CURVE_LEFT,RECT_CURVE_TOP,
		RECT_CURVE_LEFT+RECT_CURVE_WIDTH,RECT_CURVE_TOP+RECT_CURVE_HEIGHT);
	m_rtEvent.SetRect(RECT_EVENT_LEFT,RECT_EVENT_TOP,
		RECT_EVENT_LEFT+RECT_EVENT_WIDTH,RECT_EVENT_TOP+RECT_EVENT_HEIGHT);

	m_rtBtnFile.SetRect(BUTTON_FILE_LEFT,TOP_BUTTON_TOP,
		BUTTON_FILE_LEFT+TOP_BUTTON_WIDTH,TOP_BUTTON_TOP+TOP_BUTTON_HEIGHT);
	m_rtBtnPrint.SetRect(BUTTON_PRINT_LEFT,TOP_BUTTON_TOP,
		BUTTON_PRINT_LEFT+TOP_BUTTON_WIDTH,TOP_BUTTON_TOP+TOP_BUTTON_HEIGHT);
	m_rtBtnLang.SetRect(BUTTON_LANG_LEFT,TOP_BUTTON_TOP,
		BUTTON_LANG_LEFT+TOP_BUTTON_WIDTH,TOP_BUTTON_TOP+TOP_BUTTON_HEIGHT);
	m_rtBtnSetup.SetRect(BUTTON_SETUP_LEFT,TOP_BUTTON_TOP,
		BUTTON_SETUP_LEFT+TOP_BUTTON_WIDTH,TOP_BUTTON_TOP+TOP_BUTTON_HEIGHT);
	m_rtBtnAbout.SetRect(BUTTON_ABOUT_LEFT,TOP_BUTTON_TOP,
		BUTTON_ABOUT_LEFT+TOP_BUTTON_WIDTH,TOP_BUTTON_TOP+TOP_BUTTON_HEIGHT);
	m_rtBtnExit.SetRect(BUTTON_EXIT_LEFT,TOP_BUTTON_TOP,
		BUTTON_EXIT_LEFT+TOP_BUTTON_WIDTH,TOP_BUTTON_TOP+TOP_BUTTON_HEIGHT);

	m_rtFile1.SetRect(TEXT_FILE1_LEFT,TEXT_FILE1_TOP,
		TEXT_FILE1_LEFT+TEXT_WIDTH,TEXT_FILE1_TOP+TEXT_HEIGHT);
	m_rtFile2.SetRect(TEXT_FILE2_LEFT,TEXT_FILE2_TOP,
		TEXT_FILE2_LEFT+TEXT_WIDTH,TEXT_FILE2_TOP+TEXT_HEIGHT);

	m_rtBtnReset.SetRect(BUTTON_RESET_LEFT,BUTTON_RESET_TOP,
		BUTTON_RESET_LEFT+LOWER_BUTTON_WIDTH,BUTTON_RESET_TOP+LOWER_BUTTON_HEIGHT);
	m_rtBtnZoominB.SetRect(BUTTON_ZOOMIN_BOTH_LEFT,BUTTON_ZOOMIN_BOTH_TOP,
		BUTTON_ZOOMIN_BOTH_LEFT+LOWER_BUTTON_WIDTH,BUTTON_ZOOMIN_BOTH_TOP+LOWER_BUTTON_HEIGHT);
	m_rtBtnZoomoutB.SetRect(BUTTON_ZOOMOUT_BOTH_LEFT,BUTTON_ZOOMOUT_BOTH_TOP,
		BUTTON_ZOOMOUT_BOTH_LEFT+LOWER_BUTTON_WIDTH,BUTTON_ZOOMOUT_BOTH_TOP+LOWER_BUTTON_HEIGHT);
	m_rtBtnZoominH.SetRect(BUTTON_ZOOMIN_H_LEFT,BUTTON_ZOOMIN_H_TOP,
		BUTTON_ZOOMIN_H_LEFT+LOWER_BUTTON_WIDTH,BUTTON_ZOOMIN_H_TOP+LOWER_BUTTON_HEIGHT);
	m_rtBtnZoomoutH.SetRect(BUTTON_ZOOMOUT_H_LEFT,BUTTON_ZOOMOUT_H_TOP,
		BUTTON_ZOOMOUT_H_LEFT+LOWER_BUTTON_WIDTH,BUTTON_ZOOMOUT_H_TOP+LOWER_BUTTON_HEIGHT);
	m_rtBtnZoominV.SetRect(BUTTON_ZOOMIN_V_LEFT,BUTTON_ZOOMIN_V_TOP,
		BUTTON_ZOOMIN_V_LEFT+LOWER_BUTTON_WIDTH,BUTTON_ZOOMIN_V_TOP+LOWER_BUTTON_HEIGHT);
	m_rtBtnZoomoutV.SetRect(BUTTON_ZOOMOUT_V_LEFT,BUTTON_ZOOMOUT_V_TOP,
		BUTTON_ZOOMOUT_V_LEFT+LOWER_BUTTON_WIDTH,BUTTON_ZOOMOUT_V_TOP+LOWER_BUTTON_HEIGHT);

	m_rtBtnAddevent.SetRect(BUTTON_ADDEVETNT_LEFT,BUTTON_ADDEVETNT_TOP,
		BUTTON_ADDEVETNT_LEFT+LOWER_BUTTON_WIDTH,BUTTON_ADDEVETNT_TOP+LOWER_BUTTON_HEIGHT);
	m_rtBtnDelevent.SetRect(BUTTON_DELEVETNT_LEFT,BUTTON_DELEVETNT_TOP,
		BUTTON_DELEVETNT_LEFT+LOWER_BUTTON_WIDTH,BUTTON_DELEVETNT_TOP+LOWER_BUTTON_HEIGHT);

	m_rtCheckAB.SetRect(CHECK_CURSORAB_LEFT,CHECK_CURSORAB_TOP,
		CHECK_CURSORAB_LEFT+RADIO_WIDTH,CHECK_CURSORAB_TOP+RADIO_HEIGHT);
	m_rtRadioAandB.SetRect(RADIO_AANDB_LEFT,RADIO_AANDB_TOP,
		RADIO_AANDB_LEFT+RADIO_WIDTH,RADIO_AANDB_TOP+RADIO_HEIGHT);
	m_rtRadioAorB.SetRect(RADIO_AORB_LEFT,RADIO_AORB_TOP,
		RADIO_AORB_LEFT+RADIO_WIDTH,RADIO_AORB_TOP+RADIO_HEIGHT);

	m_rtRadioA2B.SetRect(RADIO_AB_LEFT,RADIO_AB_TOP,
		RADIO_AB_LEFT+RADIO_WIDTH+30,RADIO_AB_TOP+RADIO_HEIGHT);
	m_rtRadioB2A.SetRect(RADIO_BA_LEFT,RADIO_BA_TOP,
		RADIO_BA_LEFT+RADIO_WIDTH+30,RADIO_BA_TOP+RADIO_HEIGHT);
	m_rtRadioBoth.SetRect(RADIO_BOTH_LEFT,RADIO_BOTH_TOP,
		RADIO_BOTH_LEFT+RADIO_WIDTH+30,RADIO_BOTH_TOP+RADIO_HEIGHT);

	m_otdrTrace.MoveWindow(&m_rtCurve);
	m_listEvent.MoveWindow(&m_rtEvent);

	m_btnFile.MoveWindow(&m_rtBtnFile);
	m_btnPrint.MoveWindow(&m_rtBtnPrint);
	m_btnLang.MoveWindow(&m_rtBtnLang);
	m_btnSetup.MoveWindow(&m_rtBtnSetup);
	m_btnAbout.MoveWindow(&m_rtBtnAbout);
	m_btnExit.MoveWindow(&m_rtBtnExit);

	m_txtFile1.MoveWindow(&m_rtFile1);
	m_txtFile2.MoveWindow(&m_rtFile2);

	m_btnOriginal.MoveWindow(&m_rtBtnReset);
	m_btnZoomin.MoveWindow(&m_rtBtnZoominB);
	m_btnZoomout.MoveWindow(&m_rtBtnZoomoutB);
	m_btnZoomInH.MoveWindow(&m_rtBtnZoominH);
	m_btnZoomOutH.MoveWindow(&m_rtBtnZoomoutH);
	m_btnZoomInV.MoveWindow(&m_rtBtnZoominV);
	m_btnZoomOutV.MoveWindow(&m_rtBtnZoomoutV);

    GetDlgItem(IDC_BUTTON_ADD_EVENT)->MoveWindow(&m_rtBtnAddevent);
	GetDlgItem(IDC_BUTTON_DEL_EVENT)->MoveWindow(&m_rtBtnDelevent);

	GetDlgItem(IDC_CHECK_CURSORAB)->MoveWindow(&m_rtCheckAB);
    GetDlgItem(IDC_RADIO_AB)->MoveWindow(&m_rtRadioAandB);
	GetDlgItem(IDC_RADIO_AORB)->MoveWindow(&m_rtRadioAorB);

    m_radioAB.MoveWindow(&m_rtRadioA2B);
    m_radioBA.MoveWindow(&m_rtRadioB2A);
    m_radioBoth.MoveWindow(&m_rtRadioBoth);

	m_bCursorAB = FALSE;
}

void CBitestDlg::OnInitBtnIcon()
{
	if (m_btnOriginal.GetIcon() == NULL)
         m_btnOriginal.SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_ORIGINAL)));
	if (m_btnZoomin.GetIcon() == NULL)
         m_btnZoomin.SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_ZOOMIN)));
	if (m_btnZoomout.GetIcon() == NULL)
         m_btnZoomout.SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_ZOOMOUT)));
	if (m_btnZoomInH.GetIcon() == NULL)
         m_btnZoomInH.SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_ZOOMIN_H)));
	if (m_btnZoomOutH.GetIcon() == NULL)
         m_btnZoomOutH.SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_ZOOMOUT_H)));
	if (m_btnZoomInV.GetIcon() == NULL)
         m_btnZoomInV.SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_ZOOMIN_V)));
	if (m_btnZoomOutV.GetIcon() == NULL)
         m_btnZoomOutV.SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_ZOOMOUT_V)));
	if (((CButton *)GetDlgItem(IDC_BUTTON_ADD_EVENT))->GetIcon() == NULL)
         ((CButton *)GetDlgItem(IDC_BUTTON_ADD_EVENT))->SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_ADDEVENT)));
	if (((CButton *)GetDlgItem(IDC_BUTTON_DEL_EVENT))->GetIcon() == NULL)
         ((CButton *)GetDlgItem(IDC_BUTTON_DEL_EVENT))->SetIcon(::LoadIcon(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_DELEVENT)));
}
void CBitestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBitestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{		
		CDialog::OnPaint();
	}

	GetDlgItem(IDC_BUTTON_TRACE)->GetWindowRect(m_xyCoef.rcWnd);
	ScreenToClient(m_xyCoef.rcWnd);
	//SetCoordinate(m_xyCoef);

	TRACE("xmax = %f; xmin= %f\n", m_xyCoef.fXmax, m_xyCoef.fXmin);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CBitestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CBitestDlg::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if(VK_RETURN == nKey || VK_ESCAPE == nKey || VK_SPACE == nKey)
			return TRUE;
	}  
	if(WM_MOUSEMOVE == pMsg->message)
	{
        m_Mytip.RelayEvent(pMsg);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//exit
void CBitestDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//����ɾ������
	for(int i = 0; i < 3; i++)
	{
		if(m_allData[i].PData)
		{
            delete[] m_allData[i].PData;
		    m_allData[i].PData = NULL;
		}
	}
	
	OnCancel();
}

//��SOR�ļ������ڷ���
void CBitestDlg::OnOpenFile()
{
	if (m_pdlgOpen)
	{
		m_pdlgOpen->ShowWindow(SW_SHOW);
		m_pdlgOpen->CenterWindow();
	}
//	if(m_pdlgOpen->DoModal() == IDOK)
//	{
      //DealFullData(&m_allData[0], &dlg.m_sorDATA[0], SHOW_A2B);
	  //DealFullData(&m_allData[1], &dlg.m_sorDATA[1], SHOW_B2A);
		//m_testResult[0] = dlg.m_testResInfo[0];
		//m_testResult[1] = dlg.m_testResInfo[1];
		//m_testResult[2] = dlg.m_testResInfo[2];

		//int itest = 0;
//	}
//	else
//	{
		//MessageBox(_T("Open files failed!"));
//	}	
}

void CBitestDlg::OnOpenFileOk()
{
	//���ݳ�ʼ��
	if (NULL != m_allData[0].PData)
	{
		delete[] m_allData[0].PData;
		m_allData[0].PData = NULL;
		m_allData[0].DataPointsNum = 0;
	}
	if (NULL != m_allData[1].PData)
	{
		delete[] m_allData[1].PData;
		m_allData[1].PData = NULL;
		m_allData[1].DataPointsNum = 0;
	}		

	DataProcess(&m_allData[0], &m_testResult[0], SHOW_A2B);
	DataProcess(&m_allData[1], &m_testResult[1], SHOW_B2A);

	ShowTrace((ShowType)m_iRadioShow);
	if(m_testResult[0].DataPointsNum>0||m_testResult[1].DataPointsNum>0)
	    (CButton *)GetDlgItem(IDC_BUTTON_PRINT)->EnableWindow(TRUE);
    m_otdrTrace.ZoomReset();//
}

//���ݴ���������ʾ�����ߣ��Լ��¼�����Ϣ
void CBitestDlg::DataProcess(OTDR_ALL_UPLOAD_DATA* allData, TestResultInfo* testInfo, ShowType myType)
{
	if(testInfo->Length == 0)  //���û������ֱ�ӷ���
		return;

	int oldDatapointsNume = allData->DataPointsNum;	

	UINT iNewCount = testInfo->DataPointsNum;  //�µ����ݵ����

	allData->SampleFrequence = testInfo->Frequency;//sorData->Frequency;  //����Ƶ��
	allData->TestRange = testInfo->Range; //���Է�Χ
	allData->PulseWidth = testInfo->PulseWidth;  //����
	allData->WaveLength = testInfo->WaveLength;  //����
	allData->Refract = testInfo->Refraction;  //������
	allData->LinkLength = testInfo->Length;   //���˳���
	allData->FiberEnd = testInfo->EndOfFiberThreshold;  //ĩ������
	allData->Reflect = testInfo->ReflectanceThreshold;  //��������
	allData->ScatterCoefficient = testInfo->BackscatterCoefficient;  //����ɢ��ϵ������
	//ghq 2016.4.14
	allData->DataSpacing = testInfo->DataSpacing;//���Ϊ��������,������������
	allData->Length = testInfo->Length;//

	//���ݳ�ʼ��
	if (iNewCount > oldDatapointsNume)
	{
		if (NULL != allData->PData)
		{
			delete[] allData->PData;
			allData->PData = NULL;
		}
		allData->PData = new WORD[iNewCount];
	}

	for(UINT i = 0; i < iNewCount; i++)
	{
		switch(myType)
		{
		case 0:
			allData->PData[i] = testInfo->pDataPoints[i];   //A->B			
			break;

		case 1:
			//allData->PData[i] = testInfo->pDataPoints[iNewCount - 1 - i];   //B->A, ��������ʾ
			allData->PData[i] = testInfo->pDataPoints[i];
			break;

		case 2:
			break;

		}
	}

	allData->DataPointsNum = iNewCount;

	m_xyCoef.fXmax = testInfo->Length / 1000.0;  //km
}


//˫����Ժ󣬴���һ���¼�����Ϣ���γ�˫������¼�
void CBitestDlg::GetEvent2Way(TestResultInfo *pInfo2Way, TestResultInfo *infoA2B, TestResultInfo *infoB2A)
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

	int iDiff = 0;   //λ�ò������С������Ϊ��һ���¼�
	int iCount = 0;
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
	UINT uiLen =  pInfo2Way->EventList.vEventInfo.size();
	if(uiLen>1)
	{
		it = pInfo2Way->EventList.vEventInfo.begin() + 1;
		//it->
		pInfo2Way->EventList.vEventInfo.erase(it);  //ɾ���ڶ���Ԫ�أ�����һ�����ߵ��׸��¼�
		if(uiLen>3)
		{
		    it = pInfo2Way->EventList.vEventInfo.end() - 2;
		    pInfo2Way->EventList.vEventInfo.erase(it);  //ɾ�����ڶ���Ԫ�أ����ڶ������ߵ����һ���¼�
		}
	}


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

//����һ������,��sorDataת��Ϊ������ʾ������
void CBitestDlg::DealFullData(OTDR_ALL_UPLOAD_DATA* allData, SORDATA* sorData, ShowType myType)
{	
	if(! sorData->Length)  //���û������ֱ�ӷ���
		return;

	int oldDatapointsNume = allData->DataPointsNum;	
	
	UINT iNewCount = (UINT)sorData->vAllData[0].vData.size();  //�µ����ݵ����

	if(!iNewCount)  //���û������
		return;

	//m_sorDATA[0] = dlg.m_sorDATA[0];
	//m_sorDATA[1] = dlg.m_sorDATA[1];

	allData->SampleFrequence = sorData->Frequency;  //����Ƶ��
	allData->TestRange = sorData->Range; //���Է�Χ
	allData->PulseWidth = sorData->PulseWidth;  //����
	allData->WaveLength = sorData->WaveLength;  //����
	allData->Refract = sorData->Refraction;  //������
	allData->LinkLength = sorData->Length;   //���˳���
	allData->FiberEnd = sorData->EndOfFiberThreshold;  //ĩ������
	allData->Reflect = sorData->ReflectanceThreshold;  //��������
    allData->ScatterCoefficient = sorData->BackscatterCoefficient;  //����ɢ��ϵ������

	//���ݳ�ʼ��
	if (iNewCount > oldDatapointsNume)
	{
		if (NULL != allData->PData)
		{
			delete[] allData->PData;
			allData->PData = NULL;
		}
		allData->PData = new WORD[iNewCount];
	}

	for(UINT i = 0; i < iNewCount; i++)
	{
		switch(myType)
		{
			case 0:
				allData->PData[i] = sorData->vAllData[0].vData[i];   //A->B
				break;

			case 1:
				allData->PData[i] = sorData->vAllData[0].vData[iNewCount - 1 - i];   //B->A, ��������ʾ
				//allData->PData[i] = sorData->vAllData[0].vData[i]; 
				break;

			case 2:
				break;
		}
	}

	allData->DataPointsNum = iNewCount;

	m_xyCoef.fXmax = sorData->Length;  //����������

	//�¼��б�
 //   allData->VEventInfo.clear();
	//UINT iEvents = (UINT)sorData->vEvent.size();

	//int x = 0;/////////////////////////////////////////////////

	/*m_allUploadData.VEventInfo.clear();
	char* pEvent = pChar + sizeof(FRAME_HEADER) + 17*4 + m_allUploadData.DataPointsNum*sizeof(WORD);
	UINT eventCount = ((UINT*)pEvent)[0];
	for(int i=0; i<eventCount; i++)
	{
		OTDR_ALL_UPLOAD_DATA::ENVENT_INFO eventInfo;
		memcpy(&eventInfo, pEvent + 4 + i * sizeof(OTDR_ALL_UPLOAD_DATA::ENVENT_INFO), sizeof(OTDR_ALL_UPLOAD_DATA::ENVENT_INFO));
		m_allUploadData.VEventInfo.push_back(eventInfo);    //����¼�
	}*/
}

//��ʾһ������
void CBitestDlg::ShowTrace(ShowType myType)
{
	UpdateData();

	switch(m_iRadioShow)
	{
	case 0:
		InitList();
		ShowOnceData(&m_allData[0]);
		break;
	case 1:
		InitList();
		ShowOnceData(&m_allData[1]);
		break;
	case 2:
		InitList2Way();
		ShowBidirectional();        
		break;
	}
}


//��ʾ����
void CBitestDlg::ShowOnceData(POTDR_ALL_UPLOAD_DATA pOtdrAllUploadData)
{
	m_otdrTrace.ReleaseAllData();//���ԭ������

	std::vector<FLOAT> record;//

	if (pOtdrAllUploadData->DataPointsNum <= 0)
	{
		return;
	}

	////INT nPointsCount = (m_onceData.nCountToRead - OTDR_CLIENT_LENGTH_CHAR - OTDR_CLIENT_LENGTH_LENGTH) / 2;
	INT nPointsCount = pOtdrAllUploadData->DataPointsNum;
	if (nPointsCount <= 0)
	{
		FreshEventList();
		return;
	}
	//WORD* pWord = (WORD*)(m_onceData.pData + OTDR_CLIENT_LENGTH_CHAR + OTDR_CLIENT_LENGTH_LENGTH);
	WORD* pWord = pOtdrAllUploadData->PData;
	for (int j=0; j<nPointsCount; j++)
	{
		FLOAT fTemp = (65535-(int)pWord[j])/DIVISOR_TOKM;
		record.push_back(fTemp);
	}//efor

	FLOAT fLength = (FLOAT)pOtdrAllUploadData->TestRange;
	FLOAT fDataspacing = (FLOAT)pOtdrAllUploadData->TestRange/pOtdrAllUploadData->DataPointsNum;
	
	m_xyCoef.fXmax = m_testResult[m_iRadioShow].Length / DIVISOR_TOKM;   //km
	m_xyCoef.fXmin = 0;  //km
	
	CGraphElement element;
	element.m_nPointsCount = nPointsCount;

	if(m_iRadioShow == 0)
		element.m_LineColor = COLOR_A2B;
	else
		element.m_LineColor = COLOR_B2A;

	element.m_pData = new FLOAT[element.m_nPointsCount];
	INT nCount = element.m_nPointsCount;

	memcpy(element.m_pData, &record[0], nCount * sizeof(FLOAT));  //ȡ������
	//memcpy(element.m_pData, &record[0], nCount * sizeof(FLOAT));  //ȡ������	
	
	//ghq 2016.4.15
	SetEvents(m_testResult[m_iRadioShow], &element);

	element.m_fRangeX[0]=0;
	element.m_fRangeX[1]=pOtdrAllUploadData->Length;;//nPointsCount;//ghq 2016.4.15
	element.m_fRangeY[0]=0;
	element.m_fRangeY[1]= OTDR_TRACE_Y_MAX; //65.535;
	element.m_fDataSpacing = pOtdrAllUploadData->DataSpacing;//ghq 2016.4.14
	element.m_fLength = pOtdrAllUploadData->Length;////ghq 2016.4.14
	element.m_fMaxDistance = pOtdrAllUploadData->Length;////ghq 2016.4.16
	m_otdrTrace.m_vAllData.clear();
	m_otdrTrace.m_vAllData.push_back(element);
	m_otdrTrace.SetMaxDistance(element.m_fMaxDistance); //ghq 2016.5.5 ����������������
	m_otdrTrace.SetRange(0, element.m_fLength, 0, OTDR_TRACE_Y_MAX); //65.63);
	m_otdrTrace.DrawCurves(&m_otdrTrace.m_dcCurve);

	m_otdrTrace.Invalidate(FALSE);
	//ShowParams(pOtdrAllUploadData);
	//FreshEventList(pOtdrAllUploadData);

	ShowEvents();

	record.clear();

//	Invalidate(TRUE);
}

//��ʾ˫������
void CBitestDlg::ShowBidirectional()
{
	m_otdrTrace.ReleaseAllData();//���ԭ������

	std::vector<FLOAT> recordAB;//
	std::vector<FLOAT> recordBA;

	if (!(&m_allData[0]) || !(&m_allData[1]))
	{
		return;
	}

	INT nPointsCountAB = m_allData[0].DataPointsNum;  //A��B�����ݵ���
	INT nPointsCountBA = m_allData[1].DataPointsNum;  //B��A�����ݵ���

	if (nPointsCountAB <= 0 || nPointsCountBA <= 0)  //����û�оͷ���
	{
		m_listEvent.DeleteAllItems();
		return;
	}

	UINT iEventLast = m_testResult[1].EventList.vEventInfo.size() - 1;  //��ȡB2A�����һ���¼���λ��
	INT nPointsStart = m_testResult[1].DataPointsNum - m_testResult[1].EventList.vEventInfo[iEventLast].index;  //��ȡA2B�ߵ���ʼλ��
	INT nPointsTotal = m_testResult[0].DataPointsNum>m_testResult[1].DataPointsNum?m_testResult[0].DataPointsNum:m_testResult[1].DataPointsNum + nPointsStart;   //��������
	FLOAT fLengthTotal = m_testResult[0].Length + m_testResult[1].Length;

	m_xyCoef.fXmax = m_testResult[0].Length / DIVISOR_TOKM;   //km
	m_xyCoef.fXmin = (m_testResult[1].EventList.vEventInfo[iEventLast].fLocation  - m_testResult[1].Length) / 1000.0;  //km

	WORD* pWordAB = m_allData[0].PData;
	WORD* pWordBA = m_allData[1].PData;

	//������A2B�ߵ�����
	for(int j = 0; j < nPointsStart; j++) //A2B��ǰ��һ��Ϊ0ֵ
	{
		recordAB.push_back(0);
	}
	for(int j = nPointsStart; j < nPointsTotal; j++)  //A2B�ߺ���һ������ΪOTDR���⵽������
	{
		recordAB.push_back((65535-(int)pWordAB[j - nPointsStart])/1000.0);
	}

	//������B2A�ߵ�����
	for(int j = 0; j < nPointsCountBA; j++) //A2B��ǰ��һ��Ϊ0ֵ
	{
		recordBA.push_back((65535-(int)pWordBA[nPointsCountBA - j])/1000.0);  //����ǰ��һ��
		
	}
	for(int j = nPointsCountBA; j < nPointsTotal; j++)  //A2B�ߺ���һ������ΪOTDR���⵽������
	{
		recordBA.push_back(0);
	}

	//��ȡ˫����Ե��¼�
	GetEvent2Way(&m_testResult[2], &m_testResult[0], &m_testResult[1]);

	//A->B���߸�ֵ
	CGraphElement elementAB;
	elementAB.m_nPointsCount = nPointsTotal;//nPointsCountAB;
	elementAB.m_LineColor = COLOR_A2B;  //RGB(255, 0, 0);
	elementAB.m_pData = new float[elementAB.m_nPointsCount];	
	int nCount = elementAB.m_nPointsCount;
	memcpy(elementAB.m_pData, &recordAB[0], nCount * sizeof(FLOAT));  //ȡ������
	elementAB.m_fRangeX[0] = 0;//m_testResult[0].Length / 1000.0
	elementAB.m_fRangeX[1] = abs(m_testResult[0].Length>m_testResult[0].Length?m_testResult[0].Length:m_testResult[1].Length-(m_testResult[1].EventList.vEventInfo[iEventLast].fLocation  - m_testResult[1].Length));//nPointsCountAB;//(m_testResult[1].EventList.vEventInfo[iEventLast].fLocation  - m_testResult[1].Length) / 1000.0
	elementAB.m_fRangeY[0]=0;
	elementAB.m_fRangeY[1]=OTDR_TRACE_Y_MAX_2WAY;

	elementAB.m_fDataSpacing = m_testResult[0].DataSpacing;//ghq 2016.4.15
	elementAB.m_fLength = m_testResult[0].Length;////ghq 2016.4.15
	elementAB.m_fMaxDistance = elementAB.m_fRangeX[1];////ghq 2016.4.15
	elementAB.m_fOffsetX = m_testResult[1].EventList.vEventInfo[iEventLast].fLocation  - m_testResult[1].Length;

	//B->A���߸�ֵ
	CGraphElement elementBA;
	elementBA.m_nPointsCount = nPointsTotal;
	elementBA.m_LineColor = COLOR_B2A; //RGB(0, 0, 255);
	elementBA.m_pData = new float[elementBA.m_nPointsCount];
	nCount = elementBA.m_nPointsCount;
	memcpy(elementBA.m_pData, &recordBA[0], nCount * sizeof(FLOAT));  //ȡ������

	elementBA.m_fDataSpacing = m_testResult[1].DataSpacing;//ghq 2016.4.15
	elementBA.m_fLength = m_testResult[1].Length;////ghq 2016.4.15
	elementBA.m_fOffsetX = m_testResult[1].EventList.vEventInfo[iEventLast].fLocation  - m_testResult[1].Length;
    SetEvents(m_testResult[m_iRadioShow], &elementBA);
	
	m_otdrTrace.m_vAllData.clear();
	m_otdrTrace.m_vAllData.push_back(elementAB);
	m_otdrTrace.m_vAllData.push_back(elementBA);
	m_otdrTrace.SetMaxDistance(elementBA.m_fRangeX[1]); //ghq 2016.5.5 ����������������
	m_otdrTrace.SetRange(elementBA.m_fRangeX[0],elementBA.m_fRangeX[1], 0, OTDR_TRACE_Y_MAX_2WAY); //65.63);

	m_otdrTrace.DrawCurve2Way(&m_otdrTrace.m_dcCurve);

	m_otdrTrace.Invalidate(FALSE);

	ShowEvents2Way();

	recordAB.clear();
	recordBA.clear();

//	Invalidate(TRUE);
}


//������ͼ��������
void CBitestDlg::SetCoordinate(CoordinateCoef xyCoef)
{
	CClientDC dc(this);
	CRect rcText;
	CString strTemp;

	//����y���ϵĶ���
	INT iyBegin = xyCoef.rcWnd.bottom;
	FLOAT fyStep = (FLOAT)xyCoef.rcWnd.Height() / xyCoef.iHorizontal; 
	INT ixEnd = xyCoef.rcWnd.left;
	for(UINT i = 0; i <= xyCoef.iHorizontal ; i++)
	{
		dc.MoveTo(ixEnd - SCALE_BIG, iyBegin - i * fyStep);
		dc.LineTo(ixEnd, iyBegin - i * fyStep);

		rcText.left = ixEnd - 50;
		rcText.right = ixEnd - 10;
		rcText.top = iyBegin - i * fyStep - 7;
		rcText.bottom = rcText.top + 20;

		//strTemp.Format(_T("%.1f"), xyCoef.fYmax * (xyCoef.iHorizontal - i) / xyCoef.iHorizontal);
		strTemp.Format(_T("%.1f"), xyCoef.fYmin - (xyCoef.fYmax - xyCoef.fYmin) * i / xyCoef.iHorizontal);

		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(strTemp, rcText, DT_CENTER);

		if(i < xyCoef.iHorizontal)
		{
			for(int j = 0; j < SCALE_STEP; j++)  //���滹��С����
			{
				dc.MoveTo(ixEnd - SCALE_SMALL, iyBegin - i * fyStep - j * fyStep / 4);
				dc.LineTo(ixEnd, iyBegin - i * fyStep - j * fyStep / 4);

				if(j>0&&j<SCALE_STEP-1)
				{
					rcText.left = ixEnd - 40;
					rcText.right = ixEnd - 10;
					rcText.top = iyBegin - i * fyStep - 7 - j * fyStep / 4;
					rcText.bottom = rcText.top + 20;

					strTemp.Format(_T("%.1f"), xyCoef.fYmin - (xyCoef.fYmax - xyCoef.fYmin) * i / xyCoef.iHorizontal - 10.0/(SCALE_STEP-1)*j);

					dc.SetBkMode(TRANSPARENT);
					dc.DrawText(strTemp, rcText, DT_CENTER);			
			    }
			}
		}
	}

	//����x���ϵĶ���
	INT ixBegin = xyCoef.rcWnd.left;
	FLOAT fxStep = (FLOAT)xyCoef.rcWnd.Width() / xyCoef.iVertical;
	INT iyTop = xyCoef.rcWnd.bottom;
    for(UINT i = 0; i <= xyCoef.iVertical; i++)
	{
		dc.MoveTo(ixBegin + i * fxStep, iyTop);
		dc.LineTo(ixBegin + i * fxStep, iyTop + SCALE_BIG);

		rcText.left = ixBegin + i * fxStep - 25;
		rcText.right = rcText.left + 50;
		rcText.top = iyTop + 10;
		rcText.bottom = rcText.top + 20;

		//strTemp.Format(_T("%.1f"), xyCoef.fXmax * i / xyCoef.iVertical);
		strTemp.Format(_T("%.1f"), xyCoef.fXmin + (xyCoef.fXmax - xyCoef.fXmin) * i / xyCoef.iVertical);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(strTemp, rcText, DT_CENTER);

		if(i < xyCoef.iVertical)
		{
			for(int j = 0; j < SCALE_STEP; j++)
			{
				dc.MoveTo(ixBegin + i * fxStep + j * fxStep / 4,  iyTop);
				dc.LineTo(ixBegin + i * fxStep + j * fxStep / 4, iyTop + SCALE_SMALL);

				if(j>0&&j<SCALE_STEP-1)
				{
					rcText.left = ixBegin + i * fxStep - 25 + j * fxStep / 4;
					rcText.right = rcText.left + 50;
					rcText.top = iyTop + 5;
					rcText.bottom = rcText.top + 15;
					float ftemp = 8.0/(SCALE_STEP-1)*j;
					ftemp = ftemp + xyCoef.fXmin + (xyCoef.fXmax - xyCoef.fXmin) * i / xyCoef.iVertical;

					strTemp.Format(_T("%.1f"), xyCoef.fXmin + (xyCoef.fXmax - xyCoef.fXmin) * i / xyCoef.iVertical + 8.0/(SCALE_STEP-1)*j);

					dc.SetBkMode(TRANSPARENT);
					dc.DrawText(strTemp, rcText, DT_CENTER);			
			    }
			}
		}
	}
}


//��ʾ�б�
void CBitestDlg::ShowEvents()
{
	//��ʼ�����б�	
	//InitList();
	
	std::vector<EventInfo> myEvents = m_testResult[m_iRadioShow].EventList.vEventInfo;
	//UINT iEvents = myEvents.vEventInfo.size();

	float fDistance = 0;  //��λkm
	CString strTemp;
	//int item = 0;
	//LVCOLUMN lvc;

	for(std::vector<EventInfo>::size_type i = 0; i < myEvents.size(); i++)
	{		
		//��һ���¼����⴦��һ��
		if(i == 0)   
		{
			m_listEvent.InsertItem(i, 0);			

			//strTemp.Format(_T("%d"), myEvents[i].nEventType);
			//strTemp = GetEventType(myEvents[i].nEventType, myEvents[i].cEventType);
            strTemp.Format(_T("%d"), i+1);
			m_listEvent.SetItemText(i, 1, strTemp);  //�¼����
			m_listEvent.SetItemText(i, 2, _T("0.000"));
			m_listEvent.SetItemText(i, 3, _T("0.000"));
			m_listEvent.SetItemText(i, 4, _T("--"));
			m_listEvent.SetItemText(i, 5, _T("--"));
			m_listEvent.SetItemText(i, 6, _T("--"));
			m_listEvent.SetItemText(i, 7, _T("--"));

			fDistance = 0;
		}
		else
		{
			m_listEvent.InsertItem(i, 0);

			//strTemp.Format(_T("%d"), myEvents[i].nEventType);
			//strTemp = GetEventType(myEvents[i].nEventType, myEvents[i].cEventType);
			strTemp.Format(_T("%d"), i+1);
			m_listEvent.SetItemText(i, 1, strTemp);

			strTemp.Format(_T("%.3f"), myEvents[i].fLocation / 1000.0);   //Distance km
			m_listEvent.SetItemText(i, 2, strTemp);

			strTemp.Format(_T("%.3f"), (myEvents[i].fLocation - fDistance)/ 1000.0);  //Segment km
			m_listEvent.SetItemText(i, 3, strTemp);   
			fDistance = myEvents[i].fLocation;

			strTemp.Format(_T("%.3f"), myEvents[i].fEventloss);  //loss
			m_listEvent.SetItemText(i, 4, strTemp); 

			strTemp.Format(_T("%.3f"), myEvents[i].fTotalLoss);  //Total loss
			m_listEvent.SetItemText(i, 5, strTemp);

			strTemp.Format(_T("%.3f"), myEvents[i].fLineK);  //slope
			m_listEvent.SetItemText(i, 6, strTemp);

			strTemp.Format(_T("%.3f"), myEvents[i].fEventHigh);  //Reflection
			m_listEvent.SetItemText(i, 7, strTemp);
		}

		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT| LVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;     // get all state flags
		lvi.cchTextMax = 50;
		lvi.pszText = (LPWSTR)(LPCTSTR)strTemp;
		//����
		if (0 == myEvents[i].nEventType)
		{
			lvi.iImage = OTDR_EVENT_TYPE_NOREFLECT;//0����Ƿ����¼�����
		}
		else if (1 == myEvents[i].nEventType)
		{
			lvi.iImage = OTDR_EVENT_TYPE_REFLECT;//1�������¼�����
		}
		//strTemp.Format(_T("%d"), i+1);
		strTemp = "";
		
        m_listEvent.SetItem(&lvi);
		//���
		if (_T('S')==myEvents[i].cEventType)
		{
			m_listEvent.SetItemText(i, 0, _T("(S)")+strTemp);
		}
		else if (_T('E')==myEvents[i].cEventType)
		{
			m_listEvent.SetItemText(i, 0, _T("(E)")+strTemp);
		}
		else if (_T('A')==myEvents[i].cEventType)
		{
			m_listEvent.SetItemText(i, 0, _T("(A)")+strTemp);
		}
		else if (_T('F')==myEvents[i].cEventType)
		{
			m_listEvent.SetItemText(i, 0, _T("(F)")+strTemp);
		}
		else if (_T('O')==myEvents[i].cEventType)
		{
			m_listEvent.SetItemText(i, 0, _T("(O)")+strTemp);
		}
		else
		{
			m_listEvent.SetItemText(i, 0, _T("    ")+strTemp);
		}
	}
}

//��ȡ�¼�������
CString CBitestDlg::GetEventType(UINT iType, char chType)
{
	CString strType; 

	strType.Format(_T("%d-%c"), iType, chType);

	return strType;
}
//��ʾ�б�,˫�����е��¼�
void CBitestDlg::ShowEvents2Way()
{
	//��ʼ���б�
	//InitList2Way();

	//���»�ȡA-B�ߵ��¼�����
	std::vector<EventInfo> vAB;
	UINT uiEvents = m_testResult[1].EventList.vEventInfo.size();  //B->A�ߵ����¼���
	UINT uiOffset = m_testResult[1].DataPointsNum - m_testResult[1].EventList.vEventInfo[uiEvents - 1].index;   //�ܵ���-����¼�������
	for(UINT i = 0; i != m_testResult[0].EventList.vEventInfo.size(); i++ ) //�Ȱѵ�һ���ߵ��¼�����
	{
		vAB.push_back(m_testResult[0].EventList.vEventInfo[i]);
		vAB[i].index += uiOffset;  //�����ݵ�ƽ��һ��
	}

	//���»�ȡB-A�ߵ��¼�����
	std::vector<EventInfo *> vBA;
	float fOffset = m_testResult[1].Range -  m_testResult[1].EventList.vEventInfo[uiEvents -1].fLocation;   //�ܾ���-����¼���λ��
	for(UINT i = 0; i != uiEvents; i++ )  //�ٰѵڶ����ߵ����ݷ�����ʱ����
	{
		EventInfo *pEvent = new EventInfo();
		*pEvent = m_testResult[1].EventList.vEventInfo[uiEvents -1 - i];
		pEvent->fLocation = m_testResult[1].EventList.vEventInfo[uiEvents -1].fLocation -  m_testResult[1].EventList.vEventInfo[uiEvents -1 - i].fLocation;  //��һ���¼�Ϊ0
		pEvent->index = m_testResult[1].DataPointsNum - m_testResult[1].EventList.vEventInfo[uiEvents - 1 - i].index;	 //�����ˣ���һ���¼���Ϊ0	      

		//pInfo2Way->EventList.vEventInfo.push_back(*pEvent);
		vBA.push_back(pEvent);
	}
	
	CString strTemp;
	float fLossAvg = 0;
    //int iTwo = 0;  //ͬһ�¼����������϶���
	
	//std::vector<EventInfo>::iterator itEvent = m_testResult[2].EventList.vEventInfo.begin();
	//for(;itEvent != m_testResult[2].EventList.vEventInfo.end(); itEvent++)
	for(std::vector<EventInfo>::size_type i=0; i!=m_testResult[2].EventList.vEventInfo.size();++i)
	{		
		fLossAvg = 0;
		//iTwo = 0;
		strTemp.Format(_T("%d"), i+1);
		m_listEvent.InsertItem(i, strTemp);  //������Ϊ�п�Ϊ1��������ʵδ��ʾ

		m_listEvent.SetItemText(i, 1, strTemp);

		strTemp.Format(_T("%.3f"), m_testResult[2].EventList.vEventInfo[i].fLocation / 1000.0);
		m_listEvent.SetItemText(i, 2, strTemp);

		strTemp.Format(_T("%.3f"), m_testResult[2].EventList.vEventInfo[i].fTotalLoss);
		m_listEvent.SetItemText(i, 6, strTemp);

		strTemp.Format(_T("%.3f"), m_testResult[2].EventList.vEventInfo[i].fLineK);
		m_listEvent.SetItemText(i, 7, strTemp);

		strTemp.Format(_T("%.3f"), m_testResult[2].EventList.vEventInfo[i].fEventHigh);
		m_listEvent.SetItemText(i, 8, strTemp);

		strTemp.Format(_T("%.3f"), 0.0);
		//���»�ȡA->B LOSS
		for(std::vector<EventInfo>::size_type j=0; j!=vAB.size();j++)
		{
			if(abs(1.0 * vAB[j].index - m_testResult[2].EventList.vEventInfo[i].index) <= 5)
			{
				strTemp.Format(_T("%.3f"), vAB[j].fEventloss);
				m_listEvent.SetItemText(i, 3, strTemp);
				fLossAvg = vAB[j].fEventloss;
				//iTwo ++;
				break;
			}
		}
		m_listEvent.SetItemText(i, 3, strTemp);

        strTemp.Format(_T("%.3f"), 0.0);
		//���»�ȡB->A LOSS
		for(std::vector<EventInfo>::size_type j=0; j!=vBA.size();j++)
		{
			if(abs(1.0 * vBA[j]->index - m_testResult[2].EventList.vEventInfo[i].index) <= 5)
			{
				strTemp.Format(_T("%.3f"), vBA[j]->fEventloss);
				m_listEvent.SetItemText(i, 4, strTemp);
				fLossAvg += vBA[j]->fEventloss;
				//iTwo ++;
				break;
			}
		}
		m_listEvent.SetItemText(i, 4, strTemp);

		//���¸���ƽ�����
		//if( 2 == iTwo)
		strTemp.Format(_T("%.3f"), fLossAvg / 2.0);
		//else
			//strTemp.Format(_T("%.3f"), fLossAvg );
		m_listEvent.SetItemText(i, 5, strTemp);

		//strTemp.Format(_T("%.3f"), myEvents[i].fLocation / 1000.0);   //Distance km
		//m_listEvent.SetItemText(i, 2, strTemp);
	}

	std::vector<EventInfo *>::iterator it_Event;
	for(it_Event = vBA.begin(); it_Event != vBA.end(); it_Event++)
	{
		delete *it_Event;
		*it_Event = NULL;
	}
	vBA.clear();
	vAB.clear();
	//�����¼�������
	/*UINT iEventsA2B = m_testResult[0].EventList.vEventInfo.size();
	UINT iEventsB2A = m_testResult[1].EventList.vEventInfo.size();
	std::vector<EventInfo> myEvents = m_testResult[0].EventList.vEventInfo;   //A2B�ߵ��¼�
	for(int i = 0; i < iEventsB2A; i++)
	{
		myEvents.push_back(m_testResult[1].EventList.vEventInfo[i]);
	}
	
	float fDistance = 0;  //��λkm
	CString strTemp;

	for(std::vector<EventInfo>::size_type i = 0; i < myEvents.size(); i++)
	{		
		//��һ���¼����⴦��һ��
		if(i == 0)   
		{
			strTemp.Format(_T("%d"), i+1);
			m_listEvent.InsertItem(i, strTemp);
			//strTemp.Format(_T("%d"), myEvents[i].nEventType);
			strTemp = GetEventType(myEvents[i].nEventType, myEvents[i].cEventType);
            m_listEvent.SetItemText(i, 1, strTemp);
			m_listEvent.SetItemText(i, 2, _T("0.000"));
			m_listEvent.SetItemText(i, 3, _T("0.000"));
			m_listEvent.SetItemText(i, 4, _T("--"));
			m_listEvent.SetItemText(i, 5, _T("--"));
			m_listEvent.SetItemText(i, 6, _T("--"));
			m_listEvent.SetItemText(i, 7, _T("--"));

			fDistance = 0;
		}
		else
		{
			strTemp.Format(_T("%d"), i+1);
			m_listEvent.InsertItem(i, strTemp);

			strTemp = GetEventType(myEvents[i].nEventType, myEvents[i].cEventType);  //Event type
			m_listEvent.SetItemText(i, 1, strTemp);

			strTemp.Format(_T("%.3f"), myEvents[i].fLocation / 1000.0);   //Distance km
			m_listEvent.SetItemText(i, 2, strTemp);

			strTemp.Format(_T("%.3f"), (myEvents[i].fLocation - fDistance)/ 1000.0);  //Segment km
			m_listEvent.SetItemText(i, 3, strTemp);   
			fDistance = myEvents[i].fLocation;

			strTemp.Format(_T("%.3f"), myEvents[i].fEventloss);  //loss
			m_listEvent.SetItemText(i, 4, strTemp); 

			strTemp.Format(_T("%.3f"), myEvents[i].fTotalLoss);  //Total loss
			m_listEvent.SetItemText(i, 5, strTemp);

			strTemp.Format(_T("%.3f"), myEvents[i].fLineK);  //slope
			m_listEvent.SetItemText(i, 6, strTemp);

			strTemp.Format(_T("%.3f"), myEvents[i].fEventHigh);  //Reflection
			m_listEvent.SetItemText(i, 7, strTemp);
		}		
	}*/
}

//�����б��ʼ��
void CBitestDlg::InitList()
{
	INT iColumn = m_listEvent.GetHeaderCtrl()->GetItemCount();
	INT iListWidth = 0;
	CRect rc;
	CString strTitle;

	m_listEvent.DeleteAllItems();
	for(int i = 0; i < iColumn; i++)
	{
		m_listEvent.DeleteColumn(0);
	}

	// ���³�ʼ���¼��б�		
	m_util.setListToReport(m_listEvent);	
	//�����б����� ghq 2016.4.20

	m_listEvent.GetWindowRect(rc);
	iListWidth = rc.Width() / 8 - 2;

	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_TYPE"));
	m_listEvent.InsertColumn(0, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_ORDER"));
	m_listEvent.InsertColumn(1, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_DIST"));
	m_listEvent.InsertColumn(2, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_SEGMENT"));
	m_listEvent.InsertColumn(3, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_LOSS"));
	m_listEvent.InsertColumn(4,strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_TOTAL_LOSS"));
	m_listEvent.InsertColumn(5, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_SLOPE"));
	m_listEvent.InsertColumn(6, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_REFLECTION"));
	m_listEvent.InsertColumn(7, strTitle, LVCFMT_CENTER, iListWidth);
	//���½�����1����2��λ��
	CHeaderCtrl *pmyHeadCtrl = m_listEvent.GetHeaderCtrl();
	int nCount = pmyHeadCtrl->GetItemCount();
	LPINT pnOrder = (LPINT)malloc(nCount * sizeof(int));
	ASSERT(pnOrder != NULL);
	pmyHeadCtrl->GetOrderArray(pnOrder, nCount);
	int nTemp = pnOrder[0];
	pnOrder[0] = pnOrder[1];
	pnOrder[1] = nTemp;
	pmyHeadCtrl->SetOrderArray(nCount, pnOrder);
	free(pnOrder);
}

//˫������¼��б��ʼ��
void CBitestDlg::InitList2Way()
{
    INT iListWidth = 0;
	INT iColumn = m_listEvent.GetHeaderCtrl()->GetItemCount();
	CRect rc;
	CString strTitle;

	m_listEvent.DeleteAllItems();
	for(INT i = 0; i < iColumn; i++)
	{
		m_listEvent.DeleteColumn(0);
	}
	
	// ���³�ʼ���¼��б�
	m_util.setListToReport(m_listEvent);	
	m_listEvent.GetWindowRect(rc);
	iListWidth = rc.Width() / 8 - 2;
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_ORDER"));
	m_listEvent.InsertColumn(0, strTitle, LVCFMT_CENTER, 1);
	m_listEvent.InsertColumn(1, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_DIST"));
	m_listEvent.InsertColumn(2, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_LOSS_AB"));
	m_listEvent.InsertColumn(3, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_LOSS_BA"));
	m_listEvent.InsertColumn(4, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_AVG_LOSS"));
	m_listEvent.InsertColumn(5, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_TOTAL_LOSS"));
	m_listEvent.InsertColumn(6, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_SLOPE"));
	m_listEvent.InsertColumn(7, strTitle, LVCFMT_CENTER, iListWidth);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_EVENT_LIST_EVENT_MAX_REFLECTION"));
	m_listEvent.InsertColumn(8, strTitle, LVCFMT_CENTER, iListWidth);

	/*
    m_listEvent.InsertColumn(0, _T("Event Order"), LVCFMT_CENTER, 1);
	m_listEvent.InsertColumn(1, _T("Event Order"), LVCFMT_CENTER, iListWidth);
	m_listEvent.InsertColumn(2, _T("Distance(km)"), LVCFMT_CENTER, iListWidth);
	m_listEvent.InsertColumn(3, _T("A->B Loss(dB)"), LVCFMT_CENTER, iListWidth);
	m_listEvent.InsertColumn(4, _T("B->A Loss(dB)"), LVCFMT_CENTER, iListWidth);
	m_listEvent.InsertColumn(5, _T("Avg Loss(dB)"), LVCFMT_CENTER, iListWidth);
	m_listEvent.InsertColumn(6, _T("Total Loss(dB)"), LVCFMT_CENTER, iListWidth);
	m_listEvent.InsertColumn(7, _T("Slope(dB/km)"), LVCFMT_CENTER, iListWidth);
	m_listEvent.InsertColumn(8, _T("Max Reflect(dB)"), LVCFMT_CENTER, iListWidth);
	*/
   //m_listEvent.DeleteColumn(0);

	//���½�����1����2��λ��
	/*CHeaderCtrl *pmyHeadCtrl = m_listEvent.GetHeaderCtrl();
	int nCount = pmyHeadCtrl->GetItemCount();
	LPINT pnOrder = (LPINT)malloc(nCount * sizeof(int));
	ASSERT(pnOrder != NULL);
	pmyHeadCtrl->GetOrderArray(pnOrder, nCount);
	int nTemp = pnOrder[0];
	pnOrder[0] = pnOrder[1];
	pnOrder[1] = nTemp;
	pmyHeadCtrl->SetOrderArray(nCount, pnOrder);
	free(pnOrder);*/
}

//�����¼��б�
void CBitestDlg::FreshEventList()
{
	m_listEvent.DeleteAllItems();//wcq2012.07.25
	std::vector<EventInfo> ve = m_testResult[m_otdrTrace.m_nCurveIndex].EventList.vEventInfo;////wcq2012.09.03
	int nSize=ve.size();
	//
	CString temp;
	float preDistance=0;
	float ftemp=0;//
	BOOL bKm = TRUE;//
	BOOL bFeet = FALSE;//
	BOOL bPreciseMeter = (m_testResult[m_otdrTrace.m_nCurveIndex].Frequency <= 100);////
	FLOAT fDataspcing = m_testResult[m_otdrTrace.m_nCurveIndex].Range*1.0/m_testResult[m_otdrTrace.m_nCurveIndex].DataPointsNum;
	FLOAT Frequence = (FLOAT)m_testResult[m_otdrTrace.m_nCurveIndex].Frequency;
	FLOAT Refraction = m_testResult[m_otdrTrace.m_nCurveIndex].Refraction;
	for (int i=0;i<nSize;i++)//
	{
		temp.Format(_T("%d"),i+1);
		m_listEvent.InsertItem(i,temp);

		temp.Format(_T("%d"),ve[i].nEventType);
		m_listEvent.SetItemText(i, 1, temp);//

		if (0 == i)//��һ���¼����붼Ϊ0//wcq2012.07.09
		{
			temp = _T("0.000");//����
			m_listEvent.SetItemText(i,2,temp);
			m_listEvent.SetItemText(i,3,temp);
			preDistance = 0;
		}
		else
		{
			if (bKm)
			{
				if (bPreciseMeter)
				{
					temp.Format(_T("%.3f"),GetDistance(ve[i].index , Frequence, Refraction)/1000);//����
					m_listEvent.SetItemText(i,2,temp);

					temp.Format(_T("%.3f"),(GetDistance(ve[i].index , Frequence, Refraction)-preDistance)/1000);//��,���¼�������
					m_listEvent.SetItemText(i,3,temp);
					preDistance = GetDistance(ve[i].index + 1, Frequence, Refraction);
				}
				else
				{
					temp.Format(_T("%.5f"),GetDistance(ve[i].index , Frequence, Refraction)/1000);//����
					m_listEvent.SetItemText(i,2,temp);

					temp.Format(_T("%.5f"),( GetDistance(ve[i].index , Frequence, Refraction) - preDistance)/1000);//��,���¼�������
					m_listEvent.SetItemText(i,3,temp);
					preDistance = GetDistance(ve[i].index + 1, Frequence, Refraction);
				}
			}

		}//eif

		if (0 == ve[i].fEventloss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
		{
			temp.Format(_T("--"));//�����
		}
		else
		{
			temp.Format(_T("%.3f"),ve[i].fEventloss);//���
		}
		//temp.Format(_T("%.3f"),ve[i].fEventloss);//��� //cwcq2012.07.09
		m_listEvent.SetItemText(i,4,temp);

		if (0 == ve[i].fTotalLoss)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
		{
			temp.Format(_T("--"));//�����
		}
		else
		{
			temp.Format(_T("%.3f"),ve[i].fTotalLoss);//�����
		}
		m_listEvent.SetItemText(i,5,temp);

		//if (fabs(ve[i].fLineK)>0.3)//cwcq2012.05.09
		if (fabs(ve[i].fLineK)>0.5)//wcq2012.05.09
		{
			temp.Format(_T("--"));
		}
		else if (0 == ve[i].fLineK)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
		{
			temp.Format(_T("--"));
		}
		else
		{
			temp.Format(_T("%.3f"),ve[i].fLineK);//б��
		}
		m_listEvent.SetItemText(i,6,temp);

		if (0 == ve[i].fReflect)//����
		{
			m_listEvent.SetItemText(i,7,_T("--"));
		}
		else if (0 == ve[i].fReflect)//Ϊ0ʱ��ʾ"-"wcq2012.07.09
		{
			m_listEvent.SetItemText(i,7,_T("--"));
		}
		else
		{
			temp.Format(_T("%.3f"),ve[i].fReflect);//-52 + 20log10(����/1us)-2fEventHigh;����1310:-50;1550:-52;log10:��10Ϊ�ס�.�Ժ�Ҫ����Ӧ�޸�wcq2011.11.30
			m_listEvent.SetItemText(i,7,temp);
		}
	}//efor
}

//�������,��c/2nf��ʽ����.xpoint:��λ�ü�1��Frequency:Ƶ�ʣ� Refraction:������
float CBitestDlg::GetDistance(unsigned int xpoint, FLOAT Frequency, FLOAT Refraction)
{
	FLOAT ftemp = 2 * Frequency *  Refraction * 1000000;//
	if (0 == ftemp)//
	{
		return 0;
	}//eif
	return C / (ftemp/*MHz*/) * xpoint;
}

void CBitestDlg::OnRadioShow(UINT uID)
{
	UINT index = uID - IDC_RADIO_A2B;

	if(index >= 0 && index <= 2)
	{
		m_otdrTrace.SetRadioType(index);
		ShowTrace((ShowType)index);
	}
	m_otdrTrace.m_nABSele = OTDRTRACE_AB_SELE_NONE;
	//m_otdrTrace.m_nCurveIndex = (index==2)?1:0;
	//m_otdrTrace.RefreshCurve();
}


void CBitestDlg::OnButtonHelp()
{
	CAboutDlg dlg;
	dlg.DoModal();
	//this->SendMessage(WM_WSPPRINTPREVIEW,NULL,NULL); 
}

void CBitestDlg::OnBnClickedLang()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu,*pMenu;
	menu.LoadMenuW(IDR_MENU_LANG);
	CRect rtMenu;
	GetDlgItem(IDC_BUTTON_LANG)->GetWindowRect(&rtMenu);
	pMenu = menu.GetSubMenu(0);
	CString strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_MENU_CHS"));
    pMenu->ModifyMenu(ID_CHS,MF_POPUP,ID_CHS,strTitle);
    strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_MENU_ENG"));
    pMenu->ModifyMenu(ID_ENG,MF_POPUP,ID_ENG,strTitle);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN,rtMenu.right,rtMenu.top,this);
}

void CBitestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)
	{
		resize();
		if(m_otdrTrace&&m_listEvent)
		ShowTrace((ShowType)m_iRadioShow);
	}
}

//ghq 2016.4.12
void CBitestDlg::resize()
{
	float fsp[2];
	POINT ptMax; //��ȡ���ڶԻ���Ĵ�С
	CRect recta;    
	GetClientRect(&recta);     //ȡ�ͻ�����С  
	ptMax.x=recta.right-recta.left;
	ptMax.y=recta.bottom-recta.top;
	fsp[0]=(float)ptMax.x/m_ptMin.x;
	fsp[1]=(float)ptMax.y/m_ptMin.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //���Ͻ�
	CPoint OldBRPoint,BRPoint; //���½�
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ�  
	while(hwndChild)    
	{    
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID
		GetDlgItem(woc)->GetWindowRect(Rect);  
		ScreenToClient(Rect);  
		OldTLPoint = Rect.TopLeft();  
		TLPoint.x = long(OldTLPoint.x*fsp[0]);  
		TLPoint.y = long(OldTLPoint.y*fsp[1]);  
		OldBRPoint = Rect.BottomRight();  
		BRPoint.x = long(OldBRPoint.x *fsp[0]);  
		BRPoint.y = long(OldBRPoint.y *fsp[1]);  
		Rect.SetRect(TLPoint,BRPoint);  
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);
		if(woc == IDC_LIST_EVENT)
		{
			for(INT i=1;i<9;i++)
			    m_listEvent.SetColumnWidth(i, Rect.Width()/8);  
		}
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);    
	}
	m_ptMin=ptMax;
}

BOOL CBitestDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//m_otdrTrace.SetFocus();
	INT nType = GetZoomType();
	if(zDelta>0)
	{
		switch(nType)
		{
		case ZOOM_IN_BOTH:
		case ZOOM_OUT_BOTH:
            m_otdrTrace.ZoomInBothForBig();//
			break;
		case ZOOM_IN_H:
        case ZOOM_OUT_H:
            m_otdrTrace.ZoomInHorizonForBig();//
			break;
		case ZOOM_IN_V:
		case ZOOM_OUT_V:
            m_otdrTrace.ZoomInVertialForBig();//
			break;
		}
	}
	else
	{
		switch(nType)
		{
		case ZOOM_IN_BOTH:
		case ZOOM_OUT_BOTH:
            m_otdrTrace.ZoomOutBothForSmall();//;
			break;
		case ZOOM_IN_H:
        case ZOOM_OUT_H:
            m_otdrTrace.ZoomOutHorizonForSmall();//
			break;
		case ZOOM_IN_V:
		case ZOOM_OUT_V:
            m_otdrTrace.ZoomOutVertialForSmall();//
			break;
		}
	}

	//SetCoordinate(m_xyCoef);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CBitestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnLButtonDown(nFlags, point);
}

void CBitestDlg::OnDown(UINT nID)
{
	switch(nID)
	{
		case IDC_BUTTON_TRACE:
			//m_listEvent.SetFocus();
			//m_listEvent.SetItemState(m_otdrTrace.m_iSelEvent,LVIS_SELECTED,LVIS_SELECTED);//LVIS_FOCUSED
			//m_listEvent.EnsureVisible(m_otdrTrace.m_iSelEvent,FALSE);
			//m_otdrTrace.MoveToEvent();
			m_otdrTrace.RefreshCurve();
			break;
		case IDC_LIST_EVENT:
			{
			INT iSelEventLine = 0;
		    for(int i = 0; i < m_listEvent.GetItemCount(); i++)
			{
				if(m_listEvent.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
				{
					m_otdrTrace.m_nABSele = OTDRTRACE_AB_SELE_EVENT;
					iSelEventLine = i;
					break;
				}
			}
	        for(UINT i = 0; i < m_otdrTrace.m_vAllData.size(); i++)
			{
				m_otdrTrace.m_vAllData[i].SetSelectLine(iSelEventLine);
			}
			m_otdrTrace.MoveToEvent();
            m_otdrTrace.RefreshCurve();
			break;
			}
 		case IDD_PAGE_OPEN_FILE:
			EventToGaborResult( &m_testResult[0] ); 
	        EventToGaborResult( &m_testResult[1] ); 
            OnOpenFileOk();
			m_otdrTrace.OnInitAB();
			break;
	}
//	ResetEventButtonStatus();
}

void CBitestDlg::OnUp(UINT nID)
{
	switch(nID)
	{
		case IDC_BUTTON_TRACE:
			m_listEvent.SetFocus();
			m_listEvent.SetItemState(m_otdrTrace.m_iSelEvent,LVIS_SELECTED,LVIS_SELECTED|LVIS_FOCUSED);
			m_listEvent.EnsureVisible(m_otdrTrace.m_iSelEvent,FALSE);
			//m_otdrTrace.RefreshCurve();
			break;
	}
}

//���¼��б�ת��Ϊ������ʾ�õ��¼��б�//zll2013.4.23
void CBitestDlg::SetEvents(TestResultInfo& testResultInfo, CGraphElement* graphElement)
{
	graphElement->m_vAllEvents.clear();
	CGraphElement::TraceEvent te;
	float fTemp;
	for (UINT i=0; i< testResultInfo.EventList.vEventInfo.size(); i++)
	{
		fTemp = testResultInfo.DataSpacing * testResultInfo.EventList.vEventInfo[i].index;
		te.m_nIndex = testResultInfo.EventList.vEventInfo[i].index;
		te.m_distance = testResultInfo.EventList.vEventInfo[i].fLocation;
		graphElement->m_vAllEvents.push_back(te);
	}
}

BOOL CBitestDlg::SaveLangSetting()
{
	CString strLang;
	strLang.Format(_T("%d"), (INT)g_iLang);
	return WritePrivateProfileString (_T("grandway"), _T("Language"), strLang, m_util.GetExePath()+CONFIG_FILE);
}

void CBitestDlg::LoadLangSetting()
{
	CString strLang;
//	LPWSTR lpStr = (LPWSTR)(LPCTSTR)strLang; 
	::GetPrivateProfileString(_T("grandway"),_T("Language"),NULL,(LPWSTR)(LPCTSTR)strLang,MAX_PATH,m_util.GetExePath()+CONFIG_FILE);
	g_iLang = (LANG)_ttoi(strLang);
	return ;
}

void CBitestDlg::OnChs()
{
	// TODO: �ڴ���������������
	g_iLang = CHS;

	SetLocaleString();
		//������������
	SaveLangSetting();
//	GetDlgItem(IDC_BUTTON_LANG)->GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_CHS,MF_BYCOMMAND|MF_CHECKED);
}

void CBitestDlg::OnEng()
{
	// TODO: �ڴ���������������
	g_iLang = ENG;

	SetLocaleString();
		//������������
	SaveLangSetting();
}

void CBitestDlg::SetLocaleString()
{
	//����������
	CString strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_TITLE"));
	SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_BUTTON_OPEN"));
	GetDlgItem(IDC_BUTTON_OPEN)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_BUTTON_PRINT"));
	GetDlgItem(IDC_BUTTON_PRINT)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_BUTTON_LANGUAGE"));
	GetDlgItem(IDC_BUTTON_LANG)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_BUTTON_SETUP"));
	GetDlgItem(IDC_BUTTON_SETUP)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_BUTTON_ABOUT"));
	GetDlgItem(IDC_BUTTON_ABOUT)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_BUTTON_EXIT"));
	GetDlgItem(IDC_BUTTON_EXIT)->SetWindowText(strTitle);

    strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_CHECK_AB"));
	GetDlgItem(IDC_CHECK_CURSORAB)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_RADIO_AANDB"));
	GetDlgItem(IDC_RADIO_AB)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_RADIO_AORB"));
	GetDlgItem(IDC_RADIO_AORB)->SetWindowText(strTitle);

	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_RADIO_SHOWAB"));
	GetDlgItem(IDC_RADIO_A2B)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_RADIO_SHOWBA"));
	GetDlgItem(IDC_RADIO_B2A)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_RADIO_SHOWBI"));
	GetDlgItem(IDC_RADIO_BIDIRECTIONAL)->SetWindowText(strTitle);

	//����������������
	m_pdlgOpen->SetLocaleString();
	m_pdlgSetup->SetLocaleString();

	//�¼��б�����
	if(m_iRadioShow == SHOW_ALL)
	{
		InitList2Way();
		ShowEvents2Way();
	}
	else
	{
	    InitList();
	    ShowEvents();
	}
}

//�������ļ�ҳ��
void CBitestDlg::CreateOpenFileDlg()
{
	if (!m_pdlgOpen)
	{
		m_pdlgOpen = new CPageOpen();
		m_pdlgOpen->Create(IDD_PAGE_OPEN_FILE, this);
		m_pdlgOpen->m_dlgParent = this;
	}
}

//��������ҳ��
void CBitestDlg::CreateAboutDlg()
{
		// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
}

void CBitestDlg::OnBnClickedButtonPrint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu,*pMenu;
	menu.LoadMenuW(IDR_MENU_PRINT);
	CRect rtMenu;
	GetDlgItem(IDC_BUTTON_PRINT)->GetWindowRect(&rtMenu);
	pMenu = menu.GetSubMenu(0);
	CString strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_MENU_PRINT"));
	pMenu->ModifyMenu(ID_PRINT,MF_POPUP,ID_PRINT,strTitle);
    strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_MENU_PREVIEW"));
	pMenu->ModifyMenu(ID_PREVIEW,MF_POPUP,ID_PREVIEW,strTitle);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN,rtMenu.right,rtMenu.top,this);
}

void CBitestDlg::OnPrint()
{
	// TODO: �ڴ���������������
#if MY_PRINT_TEST
	CMyFrame* pFrame = new CMyFrame(this,0);
//	pFrame->Create(NULL,_T("Curve Print"),WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));
	pFrame->m_pView->OnMyPrint();
#else
	CPrintFrame *pFrame = new CPrintFrame;
	pFrame->m_pMainDlg = this;
	pFrame->Create(NULL,NULL,WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));
	pFrame->m_pView->OnMyPrint();
#endif
}


void CBitestDlg::OnPreview()
{
	// TODO: �ڴ���������������
#if MY_PRINT_TEST
	if (m_pdlgSetup)
	{
		m_pdlgSetup->OnReadQuestConfigFile();
		m_pdlgSetup->OnGetQuestInfo();
	}

    CMyFrame* pFrame = new CMyFrame(this,1);
//	pFrame->Create(NULL,_T("Curve Print Preview"),WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));

#else
	CPrintFrame *pFrame = new CPrintFrame;
	pFrame->m_pMainDlg = this;
	pFrame->Create(NULL,_T("aaa"),WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));
	pFrame->m_pView->OnMyPrintPreview();
#endif
}

void CBitestDlg::OnBnClickedButtonOrig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnBnClickedButtonZoomAny(ZOOM_RESET);
	m_otdrTrace.ZoomReset();//
}

void CBitestDlg::OnBnClickedButtonZoomin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonZoomAny(ZOOM_IN_BOTH);
	m_otdrTrace.ZoomInBothForBig();//

}

void CBitestDlg::OnBnClickedButtonZoomout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonZoomAny(ZOOM_OUT_BOTH);
	m_otdrTrace.ZoomOutBothForSmall();
}

void CBitestDlg::OnBnClickedButtonZoominH()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonZoomAny(ZOOM_IN_H);
	m_otdrTrace.ZoomInHorizonForBig();
}


void CBitestDlg::OnBnClickedButtonZoomoutH()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonZoomAny(ZOOM_OUT_H);
	m_otdrTrace.ZoomOutHorizonForSmall();
}


void CBitestDlg::OnBnClickedButtonZoominV()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonZoomAny(ZOOM_IN_V);
	m_otdrTrace.ZoomInVertialForBig();
}


void CBitestDlg::OnBnClickedButtonZoomoutV()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedButtonZoomAny(ZOOM_OUT_V);
	m_otdrTrace.ZoomOutVertialForSmall();
}

void CBitestDlg::OnBnClickedButtonZoomAny(INT nIndex)
{
	switch(nIndex)
	{
	case ZOOM_IN_BOTH:
		//if(m_bZoomin)
		//    m_bZoomin = FALSE;
	  //  else
		{
		    m_bZoomin = TRUE;
			m_bZoomout = FALSE;
			m_bZoominH = FALSE;
			m_bZoomoutH = FALSE;
			m_bZoominV = FALSE;
			m_bZoomoutV = FALSE;
		}
		break;
	case ZOOM_OUT_BOTH:
		//if(m_bZoomout)
		//    m_bZoomout = FALSE;
	   // else
		{
		    m_bZoomout = TRUE;
	        m_bZoomin = FALSE;
			m_bZoominH = FALSE;
			m_bZoomoutH = FALSE;
			m_bZoominV = FALSE;
			m_bZoomoutV = FALSE;
		}
		break;
	case ZOOM_IN_H:
		//if(m_bZoominH)
		//    m_bZoominH = FALSE;
	  //  else
		{
			m_bZoominH = TRUE;
	        m_bZoomin = FALSE;
			m_bZoomout = FALSE;
			m_bZoomoutH = FALSE;
			m_bZoominV = FALSE;
			m_bZoomoutV = FALSE;
		}
		break;
	case ZOOM_OUT_H:
		//if(m_bZoomoutH)
	//	    m_bZoomoutH = FALSE;
	  //  else
		{
			m_bZoomoutH = TRUE;
	        m_bZoomin = FALSE;
			m_bZoomout = FALSE;
			m_bZoominH = FALSE;
			m_bZoominV = FALSE;
			m_bZoomoutV = FALSE;
		}
		break;
	case ZOOM_IN_V:
	//	if(m_bZoominV)
	//	    m_bZoominV = FALSE;
	 //   else
		{
			m_bZoominV = TRUE;
	        m_bZoomin = FALSE;
			m_bZoomout = FALSE;
			m_bZoominH = FALSE;
			m_bZoomoutH = FALSE;
			m_bZoomoutV = FALSE;
		}
		break;
	case ZOOM_OUT_V:
//		if(m_bZoomoutV)
	//	    m_bZoomoutV = FALSE;
	//    else
		{
			m_bZoomoutV = TRUE;
	        m_bZoomin = FALSE;
			m_bZoomout = FALSE;
			m_bZoominH = FALSE;
			m_bZoomoutH = FALSE;
			m_bZoominV = FALSE;
		}
		break;
	default:
			m_bZoomoutV = FALSE;
	        m_bZoomin = FALSE;
			m_bZoomout = FALSE;
			m_bZoominH = FALSE;
			m_bZoomoutH = FALSE;
			m_bZoominV = FALSE;
			break;
	}
}

INT CBitestDlg::GetZoomType()
{
	INT nType = 0;

	if(m_bZoomin)
	    nType = ZOOM_IN_BOTH;
	else if(m_bZoomout)
		nType = ZOOM_OUT_BOTH;
	else if(m_bZoominH)
		nType = ZOOM_IN_H;
	else if(m_bZoomoutH)
		nType = ZOOM_OUT_H;
	else if(m_bZoominV)
		nType = ZOOM_IN_V;
	else if(m_bZoomoutV)
		nType = ZOOM_OUT_V;
	else
		nType = ZOOM_RESET;

	return nType;
}

void CBitestDlg::SetFileText(CString strFileName,INT nIndex)
{
	CDC *pDC = GetDC(); // Get device context
	if(nIndex == 0)
	{
		m_Mytip.AddTool(GetDlgItem(IDC_STATIC_FILE1),strFileName);
		CString strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_PATH1"));
		strFileName = strTitle + strFileName;
		CSize size = pDC->GetTextExtent (strFileName, strFileName.GetLength());
		if(size.cx > MAX_FILE_PATH)
		{
			int nPos;
			CString strTemp;
	        nPos=strFileName.ReverseFind('\\');
			strTemp=strFileName.Right(strFileName.GetLength()-nPos);
			nPos=strFileName.Find('\\');
			strFileName = strFileName.Left(nPos+1);
			strFileName = strFileName + _T("...")+strTemp;
		}
		GetDlgItem(IDC_STATIC_FILE1)->SetWindowText(strFileName);
	}
	else
	{
		m_Mytip.AddTool(GetDlgItem(IDC_STATIC_FILE2),strFileName);
		CString strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_MAIN_DLG_PATH2"));
		strFileName = strTitle + strFileName;
		CSize size = pDC->GetTextExtent (strFileName, strFileName.GetLength());
		if(size.cx > MAX_FILE_PATH)
		{
			int nPos;
			CString strTemp;
	        nPos=strFileName.ReverseFind('\\');
			strTemp=strFileName.Right(strFileName.GetLength()-nPos);
			nPos=strFileName.Find('\\');
			strFileName = strFileName.Left(nPos+1);
			strFileName = strFileName + _T("...")+strTemp;
		}
		GetDlgItem(IDC_STATIC_FILE2)->SetWindowText(strFileName);
	}
	m_Mytip.SetDelayTime(200);
	m_Mytip.SetTipTextColor(RGB(0,0,255));
	m_Mytip.SetTipBkColor(RGB(255,255,255));
	m_Mytip.Activate(TRUE);
	
}

void CBitestDlg::OnSetup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pdlgSetup)
	{
		m_pdlgSetup->ShowWindow(SW_SHOW);
		m_pdlgSetup->CenterWindow();
		m_pdlgSetup->OnReadQuestConfigFile();
	}
}
/****************************************************************
��������:             CreateOpenFileDlg()                                                                               
����˵��:             �������ļ�ҳ��                                                                         
��ڲ���:             ��                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-06                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CBitestDlg::CreateSetupDlg()
{
	if (!m_pdlgSetup)
	{
		m_pdlgSetup = new CPageSetup();
		m_pdlgSetup->Create(IDD_PAGE_SETUP, this);
		m_pdlgSetup->m_dlgParent = this;
	}
}
/****************************************************************
��������:             OnSetQuestInfo()                                                                               
����˵��:             ����������Ϣ                                                                         
��ڲ���:             ��                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-09                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CBitestDlg::OnSetQuestInfo(PrintQuest printQuest)
{
	m_printQuest.QuestID = printQuest.QuestID;
	m_printQuest.Contractor = printQuest.Contractor;
	m_printQuest.Customer = printQuest.Customer;
	m_printQuest.Player = printQuest.Player;
	m_printQuest.Comment = printQuest.Comment;
}

void CBitestDlg::OnBnClickedCheckCursorab()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(!m_bCursorAB)
	{
		ChangeABStatus(m_bCursorAB);
		m_otdrTrace.m_bShowLineAB = TRUE;	       //����ΪTRUE, Ȼ����OnShowAB�лᴦ���FALSE
		m_otdrTrace.OnShowAB();
		m_otdrTrace.Invalidate(FALSE);
	}
	else
	{
		ChangeABStatus(m_bCursorAB);
		m_otdrTrace.m_bShowLineAB = FALSE;
		m_otdrTrace.OnShowAB();
		m_otdrTrace.Invalidate(FALSE);
	}
	SetABType(OTDRTRACE_AB_TYPE_SINGLE);
//	ResetEventButtonStatus();
}

//�ı�AB���йصĿؼ���Ϣ   lzy 2015.2.10
void CBitestDlg::ChangeABStatus(BOOL bSelected)
{
	if(!bSelected)
	{
		m_bCursorAB = FALSE;
		GetDlgItem(IDC_RADIO_AB)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_AORB)->EnableWindow(FALSE);
		m_otdrTrace.SetABSelStatus(OTDRTRACE_AB_SELE_DISABLE);
	}
	else
	{
		m_bCursorAB = TRUE;
		GetDlgItem(IDC_RADIO_AB)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_AORB)->EnableWindow(TRUE);
		m_otdrTrace.SetABSelStatus(OTDRTRACE_AB_SELE_NONE);
	}

	UpdateData(FALSE);
}

void CBitestDlg::SetABType(INT nType)
{
	m_otdrTrace.SetCurrentTypeAB(nType);
}

void CBitestDlg::OnBnClickedRadioAb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetABType(OTDRTRACE_AB_SELE_BOTH);
}


void CBitestDlg::OnBnClickedRadioAorb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetABType(OTDRTRACE_AB_TYPE_SINGLE);
}

void CBitestDlg::OnResetEventList()
{
	m_listEvent.SetItemState(m_otdrTrace.m_iSelEvent,0,-1);
}

void CBitestDlg::OnBnClickedAddEvent()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_otdrTrace.AddEvent();
	TestResultInfo* pData = &m_testResult[0];
	if (0==pData->EventList.vEventInfo.size()
		||m_otdrTrace.m_vAllData.size()<=0)//zll 2013.6.19 ��ֹ����
	{
		return;
	}
	int nSelectPoint = 0;
	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A)
	{
		nSelectPoint = m_otdrTrace.GetCurrentCursorIndex();
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B)
	{
		nSelectPoint = m_otdrTrace.GetCurrentCursorIndex();
	}
	else
		return;

	int waveNum = pData->WaveLength; 
	__try
	{
	/*	pData->m_GaborResult = m_endgabor.insertEvent(nSelectPoint, //��Ҫ������¼�������ݵ���
			waveNum, //���߲���
			pData->PulseWidth, //��������
			pData->pDataPoints, //���Ե�����
			pData->DataPointsNum, //����
			pData->m_GaborResult,
			pData->Refraction,//������zll
			pData->Frequency,
			pData->BackscatterCoefficient);//g_sorFileArray.eventAnalConfig.BackScatter����ɢ��ϵ��
*/
		pData->m_GaborResult = g_gaborDllUtil.insertEvent(nSelectPoint, //��Ҫ������¼�������ݵ���
			waveNum, //���߲���
			pData->PulseWidth, //��������
			pData->pDataPoints, //���Ե�����
			pData->DataPointsNum, //����
			pData->m_GaborResult,
			pData->Refraction,//������zll
			pData->Frequency,
			pData->BackscatterCoefficient);//g_sorFileArray.eventAnalConfig.BackScatter����ɢ��ϵ��

	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}

	GaborResultToEvent(pData);
	m_sorAssist.SetEvents(*pData,&m_otdrTrace.m_vAllData[m_otdrTrace.m_nCurveIndex]);//zll 2013.7.12 ע��g_sorFileArray.curSelection �ǵ�ǰ����

	//�ػ�
	if(m_otdrTrace.m_iSelMode == SHOW_ALL)
	{
		//��ȡ˫����Ե��¼�
	    GetEvent2Way(&m_testResult[2], &m_testResult[0], &m_testResult[1]);
		SetEvents(m_testResult[m_iRadioShow], &m_otdrTrace.m_vAllData[1]);
	    //m_otdrTrace.DrawCurve2Way(&m_otdrTrace.m_dcCurve);
		//ShowTrace(SHOW_ALL);
	}
	else
	{
		//m_otdrTrace.DrawCurves(&m_otdrTrace.m_dcCurve);
	    //InvalidateRect(m_otdrTrace.m_rtCurve);
	}
	ShowTrace((ShowType)m_iRadioShow);
	//FreshEventList();//ˢ���¼��б�
	//����ѡ����
	m_listEvent.SetItemState(m_listEvent.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED);

	//m_EventAdd.SetTooltipText(strAddEvent);
}


void CBitestDlg::OnBnClickedDelEvent()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_otdrTrace.RemoveEvent();
	POSITION pos = m_listEvent.GetFirstSelectedItemPosition();
	INT nIndex = m_listEvent.GetNextSelectedItem(pos);

    if(m_iRadioShow == SHOW_ALL)
	{
		INT nPoints = GetEvent2WayIndex2(nIndex);
		INT nDelIndex1 = -1;
		INT nDelIndex2 = -1;
		GetEvent2WayIndex(&m_testResult[0],&m_testResult[1],nPoints,nDelIndex1,nDelIndex2);
		TestResultInfo* pCurveData1 = &m_testResult[0];
		TestResultInfo* pCurveData2 = &m_testResult[1];

		if ((nDelIndex1 >= 0) && (nDelIndex1 < pCurveData1->m_GaborResult.Valid_Num))
		{
			int waveNum = (1550 == pCurveData1->WaveLength) ? 1:0;

			pCurveData1->m_GaborResult = g_gaborDllUtil.deleteEvent(nDelIndex1,
													waveNum,
													pCurveData1->PulseWidth,
													pCurveData1->pDataPoints,
													pCurveData1->m_GaborResult,
													pCurveData1->Refraction,//������zll
													pCurveData1->Frequency,
													0.0f);

			GaborResultToEvent(pCurveData1);////����dll�õ����ݺ�ת��Ϊ�¼��б� wcq2012.06.08
			m_sorAssist.SetEvents(*pCurveData1,&m_otdrTrace.m_vAllData[0]);//zll 2013.5.7
		}
		if ((nDelIndex2 >= 0) && (nDelIndex2 < pCurveData2->m_GaborResult.Valid_Num)&&
			(nDelIndex1 >= 0) && (nDelIndex1 < pCurveData1->m_GaborResult.Valid_Num))
		{
			int waveNum = (1550 == pCurveData2->WaveLength) ? 1:0;

			pCurveData2->m_GaborResult = g_gaborDllUtil.deleteEvent(nDelIndex2,
													waveNum,
													pCurveData2->PulseWidth,
													pCurveData2->pDataPoints,
													pCurveData2->m_GaborResult,
													pCurveData2->Refraction,//������zll
													pCurveData2->Frequency,
													0.0f);

			GaborResultToEvent(pCurveData2);////����dll�õ����ݺ�ת��Ϊ�¼��б� wcq2012.06.08
			m_sorAssist.SetEvents(*pCurveData2,&m_otdrTrace.m_vAllData[1]);//zll 2013.5.7
		}
	}
	else
	{
		TestResultInfo* pCurveData = &m_testResult[0];

		if ((nIndex < 0) || (nIndex > pCurveData->m_GaborResult.Valid_Num))
		{
			return;
		}

		int waveNum = (1550 == pCurveData->WaveLength) ? 1:0;
		__try{
			pCurveData->m_GaborResult = g_gaborDllUtil.deleteEvent(nIndex,
													waveNum,
													pCurveData->PulseWidth,
													pCurveData->pDataPoints,
													pCurveData->m_GaborResult,
													pCurveData->Refraction,//������zll
													pCurveData->Frequency,
													0.0f);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			return;
		}
		GaborResultToEvent(pCurveData);////����dll�õ����ݺ�ת��Ϊ�¼��б� wcq2012.06.08
		m_sorAssist.SetEvents(*pCurveData,&m_otdrTrace.m_vAllData[0]);//zll 2013.5.7
	}
	
	//�ػ�
	if(m_otdrTrace.m_iSelMode == SHOW_ALL)
	{
		//��ȡ˫����Ե��¼�
	    GetEvent2Way(&m_testResult[2], &m_testResult[0], &m_testResult[1]);
		SetEvents(m_testResult[m_iRadioShow], &m_otdrTrace.m_vAllData[1]);
	    //m_otdrTrace.DrawCurve2Way(&m_otdrTrace.m_dcCurve);
		//ShowTrace(SHOW_ALL);
	}
	else
	{
		//m_otdrTrace.DrawCurves(&m_otdrTrace.m_dcCurve);
	    //InvalidateRect(m_otdrTrace.m_rtCurve);
	}
	ShowTrace((ShowType)m_iRadioShow);
	//FreshEventList();//ˢ���¼��б�
	//����ѡ����
	m_listEvent.SetItemState(m_listEvent.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED);
	nIndex--;
	if (nIndex < 0)
	{
		nIndex = 0;
	}
	if (m_listEvent.GetItemCount() >= nIndex)
	{
		m_listEvent.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}

	//m_EventDelete.SetTooltipText(strDeleteEvent);
}

//����dll�õ����ݺ�ת��Ϊ�¼��б�
void CBitestDlg::GaborResultToEvent(TestResultInfo* pCurveData)
{
	pCurveData->EventList.vEventInfo.clear();
	EventInfo eventinfo;
	int len = pCurveData->m_GaborResult.Valid_Num;//wcq2012.06.07
	float fDisTwo=Get_XCoord(1,pCurveData);
	for(int i=0;i<len;i++)
	{
		if (0==fDisTwo)
		{
			int ff=0;
		}//eif
		eventinfo.index = pCurveData->m_GaborResult.Event_Array[i];//�±꣬�ڼ���
		eventinfo.nEventType = pCurveData->m_GaborResult.Reflect_Type[i];//�¼�����
		eventinfo.fLocation = fDisTwo * (pCurveData->m_GaborResult.Event_Array[i]+1);;//Get_XCoord(sGabor_Result..Event_Array[i],pCurveData);//�������룬��kmΪ��λ
		eventinfo.fEventloss = pCurveData->m_GaborResult.Event_Loss[i];
		eventinfo.fEventHigh = pCurveData->m_GaborResult.Event_Hight[i];
		eventinfo.fLineK = pCurveData->m_GaborResult.Line_k[i];// / fDisTwo;//2012.8.7 rzg�޸�
		eventinfo.fTotalLoss = pCurveData->m_GaborResult.Total_Loss[i];
		eventinfo.fReflect = pCurveData->m_GaborResult.Reflect;
		eventinfo.fTrailEnd = pCurveData->m_GaborResult.Trail_End;
		eventinfo.cEventType = pCurveData->m_GaborResult.Event_type[i];//event code
		pCurveData->EventList.vEventInfo.push_back(eventinfo);
	}//efor
}

float CBitestDlg::Get_XCoord(unsigned int xpoint,TestResultInfo* pData)
{
	//return xpoint * pData->DataSpacing * 1.0/100000000;//wcq2011.11.25
	//return C/(5000 * pData->Refraction * 1000000/*MHz*/) * xpoint
	return C_zll/( pData->Refraction)/pData->Frequency/1000000/2* xpoint;//2012.8.3 rzg�޸Ĺ�ʽ /1000��ʾ����Ϊ��KmΪ��λ
}

//���ļ���ȡ�¼��б��ת��Ϊʹ�õ�GaborResult�ṹ�� wcq2012.06.11
void CBitestDlg::EventToGaborResult(TestResultInfo* pCurveData)
{
	EventInfo eventinfo;
	int len = pCurveData->EventList.vEventInfo.size();//wcq2012.06.07
	if ((len < 0)  || (len > OTDR_EVENT_NUM_MAX))//��ֹ�������� wcq2012.07.16
	{
		return;
	}//eif
	pCurveData->m_GaborResult.Valid_Num = len;
	float fDisTwo=Get_XCoord(1,pCurveData);
	//float ddd=0;//zll 2012.12.26ע��
	for(int i=0;i<len;i++)
	{
		eventinfo = pCurveData->EventList.vEventInfo[i];
		//	pCurveData->m_GaborResult.Event_Array[i] = eventinfo.index;//�±꣬�ڼ���//???wcq2012.07.09

		//pCurveData->m_GaborResult.Reflect_Type[i] = !eventinfo.nEventType;//�¼�����//��׼����3�֣�����ֻ��ʾ���䡢�Ƿ�������cwcq2012.06.12//cwcq2012.07.09
		pCurveData->m_GaborResult.Reflect_Type[i] = eventinfo.nEventType;//�¼�����//��׼����3�֣�����ֻ��ʾ���䡢�Ƿ�������cwcq2012.06.12//wcq2012.07.09
		pCurveData->m_GaborResult.Event_Array[i] = eventinfo.index;//Get_XCoord(pCurveData->m_GaborResult..Event_Array[i],pCurveData);//�������룬��kmΪ��λ

		//�������� zll 2012.12.26ע��
		/*ddd=eventinfo.fLocation/fDisTwo;
		pCurveData->m_GaborResult.Event_Array[i]=ddd;*/

		pCurveData->m_GaborResult.Event_Loss[i] = eventinfo.fEventloss;
		pCurveData->m_GaborResult.Event_Hight[i] = eventinfo.fEventHigh ;
		//pCurveData->m_GaborResult.Line_k[i] =  fDisTwo * eventinfo.fLineK;
		pCurveData->m_GaborResult.Line_k[i] =  eventinfo.fLineK;//wcq2012.06.29 ?????
		pCurveData->m_GaborResult.Total_Loss[i] = eventinfo.fTotalLoss;
		pCurveData->m_GaborResult.Reflect = eventinfo.fReflect;
		pCurveData->m_GaborResult.Trail_End = eventinfo.fTrailEnd;
		pCurveData->m_GaborResult.Event_type[i] = eventinfo.cEventType;//wcq2012.07.09
	}//efor
}

void CBitestDlg::GetEvent2WayIndex(TestResultInfo *infoA2B, TestResultInfo *infoB2A,INT nPoints,INT &nDelIndex1,INT &nDelIndex2)
{
	std::vector<EventInfo> vEventInfo1;
	std::vector<EventInfo> vEventInfo2;

	UINT uiEvents = infoB2A->EventList.vEventInfo.size();  //B->A�ߵ����¼���

	UINT uiOffset = infoB2A->DataPointsNum - infoB2A->EventList.vEventInfo[uiEvents - 1].index;   //�ܵ���-����¼�������
	
	for(UINT i = 0; i != infoA2B->EventList.vEventInfo.size(); i++ ) //�Ȱѵ�һ���ߵ��¼�����
	{
		vEventInfo1.push_back(infoA2B->EventList.vEventInfo[i]);
		vEventInfo1[ i ].index += uiOffset;  //�����ݵ�ƽ��һ��
        if(abs(1.0 *vEventInfo1[ i ].index - nPoints) <= EVENT_2WAY_OFFSET)
		{
			nDelIndex1 = i;
			break;
		}
	}
	
	for(UINT i = 0; i != uiEvents; i++ )  //�ٰѵڶ����ߵ����ݷ�����ʱ����
	{		
		vEventInfo2.push_back(infoB2A->EventList.vEventInfo[i]);
		vEventInfo2[ i ].index = infoB2A->DataPointsNum - infoB2A->EventList.vEventInfo[i].index;	 //�����ˣ���һ���¼���Ϊ0	      
	}

	for(UINT i = 0; i != uiEvents; i++ )  //�ٰѵڶ����ߵ����ݷ�����ʱ����
	{		
        if(abs(1.0*vEventInfo2[ i ].index - nPoints) <= EVENT_2WAY_OFFSET)
		{
			nDelIndex2 = i;
			break;
		}
	}

	vEventInfo1.clear();
	vEventInfo2.clear();

	return;
}


INT CBitestDlg::GetEvent2WayIndex2(INT nIndex)
{
	GetEvent2Way(&m_testResult[2], &m_testResult[0], &m_testResult[1]);
	return m_testResult[2].EventList.vEventInfo[nIndex].index;
}

void CBitestDlg::ResetEventButtonStatus()
{
    if(m_otdrTrace.m_nCurABSele == OTDRTRACE_AB_SELE_A
		||m_otdrTrace.m_nCurABSele == OTDRTRACE_AB_SELE_B
		||m_otdrTrace.m_nCurABSele == OTDRTRACE_AB_SELE_AB)
	{
	    GetDlgItem(IDC_BUTTON_ADD_EVENT)->EnableWindow(TRUE);
	}
	else
	{
	    GetDlgItem(IDC_BUTTON_ADD_EVENT)->EnableWindow(FALSE);
	}
}