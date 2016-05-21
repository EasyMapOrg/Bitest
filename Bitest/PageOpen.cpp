// PageOpen.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Bitest.h"
#include "PageOpen.h"
#include "BitestDlg.h"

// CPageOpen �Ի���

extern CSorDllUtil g_sorUtil;

IMPLEMENT_DYNAMIC(CPageOpen, CDialog)

CPageOpen::CPageOpen(CWnd* pParent /*=NULL*/)
	: CDialog(CPageOpen::IDD, pParent)
{
	//��ʼ�����ݳ���Ϊ��,Ϊ�Ժ����Ƿ������ݵ��ж�
	for(int i = 0; i < 3; i++)
		m_sorDATA[i].Length = 0;
}

CPageOpen::~CPageOpen()
{
}

void CPageOpen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageOpen, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_OPEN_A, &CPageOpen::OnOpenA)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_OPEN_B, &CPageOpen::OnOpenB)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_OPEN_OK, &CPageOpen::OnOpenOk)
	ON_BN_CLICKED(IDC_BUTTON_PAGE_OPEN_CANCEL, &CPageOpen::OnOpenCancel)
END_MESSAGE_MAP()


// CPageOpen ��Ϣ�������

//��A��B�Ĳ����ļ�
void CPageOpen::OnOpenA()
{
	CFileDialog dlg(TRUE,_T("*.SOR"),NULL,OFN_HIDEREADONLY,_T("SOR (*.SOR)|*.SOR||"));

	CString path;
	//path = m_reg.readKey(REPORT_RPT_PATH_REG);
	path = m_util.ReadConfigInfo(_T("FileOpen"), _T("Path1"));
	if(""==path)//�״δ����ߣ���·��ָ�������װ���ڵ�·�� zll 2012.10.30
	{
		path= m_util.getExePath()+REPORT_RPT_DEFAULT_PATH;
	}
	dlg.m_ofn.lpstrInitialDir=path;
	if(dlg.DoModal() != IDOK)
		return;

	if (path!=dlg.m_ofn.lpstrFile)
//		m_reg.writeKey(REPORT_RPT_PATH_REG,dlg.m_ofn.lpstrFile);//�����װ·���뵱ǰ�����ߵ�·����ͬ���򱣴浱ǰ·��
	    m_util.WriteConfigInfo(_T("FileOpen"), _T("Path1"), dlg.m_ofn.lpstrFile);
	else
//		m_reg.writeKey(REPORT_RPT_PATH_REG,path);////�����װ·���뵱ǰ�����ߵ�·����ͬ���򱣴������װ·��
        m_util.WriteConfigInfo(_T("FileOpen"), _T("Path1"), path);

	CString strFilePath=dlg.GetPathName();//·�����ļ���
	CString strFileName=dlg.GetFileName();//�õ��ļ���
	m_util.WriteConfigInfo(_T("FileOpen"), _T("Name1"), strFileName);
	GetDlgItem(IDC_EDIT_PAGE_OPEN_A)->SetWindowText(strFilePath);
	CBitestDlg *pDlg = (CBitestDlg*)(AfxGetApp()->GetMainWnd());
	pDlg->m_testResult[0].strFileName = strFileName;
	//��ȡ�ļ� ghq 2016.5.4 ����SOR���ļ�
	//CSorFile sorFile;
	INT nRes = 0;
	//TestResultInfo* pTestResultInfo;

	//ghq 2016.5.6 �����ļ�·������
	pDlg->SetFileText(strFilePath,0);

	try
	{
		nRes = g_sorUtil.readSorFile(strFilePath, m_sorDATA[0]);
		Sleep(100);
		//m_sorAssist.convert(m_sorDATA[0], m_testResInfo[0]);

		//int x = 0;///////////////////////////////////////////////////////////////////////////////
		//m_sorDrawStatic.EventToGaborResult(g_sorFileArray.sorFileList[index].pTestResultInfo);

		//pTestResultInfo=g_sorFileArray.sorFileList[index].pTestResultInfo;
		//zll,2012.9.13 ����DataSpacing�ļ���

		//g_sorFileArray.sorFileList[index].pTestResultInfo->DataSpacing = m_sorDrawStatic.Get_XCoord2nf(1,g_sorFileArray.sorFileList[index].pTestResultInfo);
	}
	catch (CException* e)
	{
		nRes = 0;
	}
}

//��B��A�Ĳ����ļ�
void CPageOpen::OnOpenB()
{
	CFileDialog dlg(TRUE,_T("*.SOR"),NULL,OFN_HIDEREADONLY,_T("SOR (*.SOR)|*.SOR||"));

	CString path;
	path = m_util.ReadConfigInfo(_T("FileOpen"), _T("Path2"));
	if(""==path)//�״δ����ߣ���·��ָ�������װ���ڵ�·�� zll 2012.10.30
	{
		path = m_util.getExePath()+REPORT_RPT_DEFAULT_PATH;
	}
	dlg.m_ofn.lpstrInitialDir=path;
	if(dlg.DoModal() != IDOK)
		return;

	if (path!=dlg.m_ofn.lpstrFile)
        m_util.WriteConfigInfo(_T("FileOpen"), _T("Path2"), dlg.m_ofn.lpstrFile);
	else
        m_util.WriteConfigInfo(_T("FileOpen"), _T("Path2"), path);

	CString strFilePath=dlg.GetPathName();//�õ��ļ���
	CString strFileName=dlg.GetFileName();//·�����ļ���
	m_util.WriteConfigInfo(_T("FileOpen"), _T("Name2"), strFileName);
	GetDlgItem(IDC_EDIT_PAGE_OPEN_B)->SetWindowText(dlg.GetPathName());
	CBitestDlg *pDlg = (CBitestDlg*)(AfxGetApp()->GetMainWnd());
	pDlg->m_testResult[1].strFileName = strFileName;
	//��ȡ�ļ� ghq 2016.5.4 ����SOR���ļ�
	//CSorFile sorFile;
	//CSorDllUtil sorUtil;
	INT nRes = 0;
	//TestResultInfo* pTestResultInfo;

	//ghq 2016.5.6 �����ļ�·������
	pDlg->SetFileText(strFilePath,1);

	try
	{
		nRes = g_sorUtil.readSorFile(strFilePath, m_sorDATA[1]);
		//m_sorAssist.convert(m_sorDATA[1], m_testResInfo[1]);

		//m_sorAssist.convert(m_sorDATA,*g_sorFileArray.sorFileList[index].pTestResultInfo);
		//m_sorDrawStatic.EventToGaborResult(g_sorFileArray.sorFileList[index].pTestResultInfo);

		//pTestResultInfo=g_sorFileArray.sorFileList[index].pTestResultInfo;
		//zll,2012.9.13 ����DataSpacing�ļ���

		//g_sorFileArray.sorFileList[index].pTestResultInfo->DataSpacing = m_sorDrawStatic.Get_XCoord2nf(1,g_sorFileArray.sorFileList[index].pTestResultInfo);
	}
	catch (CException* e)
	{
		nRes = 0;
	}
}

//�����ļ���ȷ��
void CPageOpen::OnOpenOk()
{
	CBitestDlg *pDlg = (CBitestDlg*)(AfxGetApp()->GetMainWnd());
	m_sorAssist.convert(m_sorDATA[0], pDlg->m_testResult[0]);
	m_sorAssist.convert(m_sorDATA[1], pDlg->m_testResult[1]);
	//m_sorAssist.convert(m_sorDATA[0], pDlg->m_testResult[2]);   //�����˫������ݳ�ʼ��Ϊa->b������
	m_dlgParent->OnDown(IDD_PAGE_OPEN_FILE);
	OnOK();
}

//ȡ��
void CPageOpen::OnOpenCancel()
{
	OnCancel();
}

void CPageOpen::SetLocaleString()
{
	CString strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_OPEN_FILE_TITLE"));
	SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_OPEN_FILE1_LABEL"));
	GetDlgItem(IDC_STATIC_AB)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_OPEN_FILE2_LABEL"));
	GetDlgItem(IDC_STATIC_BA)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_OPEN_FILE1_LABEL"));
	GetDlgItem(IDC_BUTTON_PAGE_OPEN_A)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_OPEN_FILE2_LABEL"));
	GetDlgItem(IDC_BUTTON_PAGE_OPEN_B)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_BUTTON_OK"));
	GetDlgItem(IDC_BUTTON_PAGE_OPEN_OK)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_BUTTON_CANCEL"));
	GetDlgItem(IDC_BUTTON_PAGE_OPEN_CANCEL)->SetWindowText(strTitle);
}

