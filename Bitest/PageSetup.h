#pragma once

#include "Util.h"
#include "SorAssist.h"
#include "afxwin.h"

// CPageSetup �Ի���

class CPageSetup : public CDialog
{
	DECLARE_DYNAMIC(CPageSetup)

public:
	CPageSetup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageSetup();

// �Ի�������
	enum { IDD = IDD_PAGE_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CUtil m_util;
	PrintQuest m_printQuest;
public:
	CBitestDlg *m_dlgParent;
private:
	void OnWriteQuestConfigFile();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSetupClear();
public:
	void SetLocaleString();
	void OnReadQuestConfigFile();
	void OnGetQuestInfo();

	virtual BOOL OnInitDialog();
};
