// PageSetup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Bitest.h"
#include "PageSetup.h"
#include "afxdialogex.h"
#include "BitestDlg.h"


// CPageSetup �Ի���

IMPLEMENT_DYNAMIC(CPageSetup, CDialog)

CPageSetup::CPageSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CPageSetup::IDD, pParent)
{

}

CPageSetup::~CPageSetup()
{
}

void CPageSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPageSetup, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_OK, &CPageSetup::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_CANCEL, &CPageSetup::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_CLEAR, &CPageSetup::OnBnClickedSetupClear)
END_MESSAGE_MAP()


// CPageSetup ��Ϣ�������
void CPageSetup::SetLocaleString()
{
	CString strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_INFO_SETUP"));
	SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_JOB_INFO"));
	GetDlgItem(IDC_SETUP_JOB_INFO)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_ID"));
	GetDlgItem(IDC_SETUP_QUESTID)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_CONTRACTOR"));
	GetDlgItem(IDC_SETUP_CONTRACTOR)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_USER"));
	GetDlgItem(IDC_SETUP_CUSTOMER)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_PLAYER"));
	GetDlgItem(IDC_SETUP_OPERATOR)->SetWindowText(strTitle);
	//strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_FILE"));
	//GetDlgItem(IDC_SETUP_FILE)->SetWindowText(strTitle);
	//strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_DATE"));
	//GetDlgItem(IDC_SETUP_TESTTIME)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_COMMENT"));
	GetDlgItem(IDC_SETUP_COMMENT)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("PrintInfo"),_T("QUEST_CLEAR"));
	GetDlgItem(IDC_BUTTON_SETUP_CLEAR)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_BUTTON_OK"));
	GetDlgItem(IDC_BUTTON_SETUP_OK)->SetWindowText(strTitle);
	strTitle = m_util.ReadLangString(_T("MainDlg"),_T("IDS_BUTTON_CANCEL"));
	GetDlgItem(IDC_BUTTON_SETUP_CANCEL)->SetWindowText(strTitle);
}

void CPageSetup::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBitestDlg *pDlg = (CBitestDlg*)(AfxGetApp()->GetMainWnd());

	//��ȡ������Ϣ
	OnGetQuestInfo();
	//д�����ļ�
	OnWriteQuestConfigFile();

	OnOK();
}

void CPageSetup::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CPageSetup::OnBnClickedSetupClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CEdit *)GetDlgItem(IDC_EDIT_QUESTID))->SetSel(0,-1);
	((CEdit *)GetDlgItem(IDC_EDIT_QUESTID))->Clear();
	((CEdit *)GetDlgItem(IDC_EDIT_CONTRACTOR))->SetSel(0,-1);
	((CEdit *)GetDlgItem(IDC_EDIT_CONTRACTOR))->Clear();
	((CEdit *)GetDlgItem(IDC_EDIT_CUSTOMER))->SetSel(0,-1);
	((CEdit *)GetDlgItem(IDC_EDIT_CUSTOMER))->Clear();
	((CEdit *)GetDlgItem(IDC_EDIT_OPERATOR))->SetSel(0,-1);
	((CEdit *)GetDlgItem(IDC_EDIT_OPERATOR))->Clear();
	((CEdit *)GetDlgItem(IDC_EDIT_COMMENT))->SetSel(0,-1);
	((CEdit *)GetDlgItem(IDC_EDIT_COMMENT))->Clear();

	UpdateData(FALSE);
}

/****************************************************************
��������:             OnGetQuestInfo()                                                                               
����˵��:             ��ȡ������Ϣ                                                                         
��ڲ���:             ��                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-09                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CPageSetup::OnGetQuestInfo()
{
	GetDlgItem(IDC_EDIT_QUESTID)->GetWindowText(m_printQuest.QuestID);
	GetDlgItem(IDC_EDIT_CONTRACTOR)->GetWindowText(m_printQuest.Contractor);
	GetDlgItem(IDC_EDIT_CUSTOMER)->GetWindowText(m_printQuest.Customer);
	GetDlgItem(IDC_EDIT_OPERATOR)->GetWindowText(m_printQuest.Player);
	GetDlgItem(IDC_EDIT_COMMENT)->GetWindowText(m_printQuest.Comment);

	m_dlgParent->OnSetQuestInfo(m_printQuest);
}

BOOL CPageSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//�������ļ���ȡ
	OnReadQuestConfigFile();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/****************************************************************
��������:             OnReadQuestConfigFile()                                                                               
����˵��:             ��ȡ������Ϣ�����ļ�                                                                       
��ڲ���:             ��                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-10                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CPageSetup::OnReadQuestConfigFile()
{
	CString strTemp;
	strTemp = m_util.ReadConfigInfo(_T("questinfo"), _T("QuestID"));
	GetDlgItem(IDC_EDIT_QUESTID)->SetWindowText(strTemp);
	strTemp = m_util.ReadConfigInfo(_T("questinfo"), _T("Contractor"));
	GetDlgItem(IDC_EDIT_CONTRACTOR)->SetWindowText(strTemp);
	strTemp = m_util.ReadConfigInfo(_T("questinfo"), _T("Customer"));
	GetDlgItem(IDC_EDIT_CUSTOMER)->SetWindowText(strTemp);
	strTemp = m_util.ReadConfigInfo(_T("questinfo"), _T("Player"));
	GetDlgItem(IDC_EDIT_OPERATOR)->SetWindowText(strTemp);
	strTemp = m_util.ReadConfigInfo(_T("questinfo"), _T("Comment"));
	GetDlgItem(IDC_EDIT_COMMENT)->SetWindowText(strTemp);
}

/****************************************************************
��������:             OnWriteQuestConfigFile()                                                                               
����˵��:             д������Ϣ�����ļ�                                                                       
��ڲ���:             ��                                                                   
���ڲ���:             ��                                                                                        
����ʱ��:             2016-05-10                                                                  
Write By:             ghq                                                                                          
****************************************************************/
void CPageSetup::OnWriteQuestConfigFile()
{
	m_util.WriteConfigInfo(_T("questinfo"), _T("QuestID"),m_printQuest.QuestID);
	m_util.WriteConfigInfo(_T("questinfo"), _T("Contractor"),m_printQuest.Contractor);
	m_util.WriteConfigInfo(_T("questinfo"), _T("Customer"),m_printQuest.Customer);
	m_util.WriteConfigInfo(_T("questinfo"), _T("Player"),m_printQuest.Player);
	m_util.WriteConfigInfo(_T("questinfo"), _T("Comment"),m_printQuest.Comment);
}