#pragma once
#include "BitestDlg.h"
#include "MyView.h"

// CMyFrame ���

class CMyFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMyFrame)
public:
	CMyFrame();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMyFrame();

	CMyFrame(CBitestDlg* pWnd,INT nType);  
public:
	CBitestDlg *m_pMainDlg;//���Ի���  
	CMyView * m_pView;  
	INT m_iShowType;

protected:
	DECLARE_MESSAGE_MAP()

public:  
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	virtual void PostNcDestroy();
};


