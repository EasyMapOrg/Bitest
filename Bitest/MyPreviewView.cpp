// MyPreviewView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Bitest.h"
#include "MyPreviewView.h"


// CMyPreviewView

IMPLEMENT_DYNCREATE(CMyPreviewView, CPreviewView)

CMyPreviewView::CMyPreviewView()
{

}

CMyPreviewView::~CMyPreviewView()
{
}

BEGIN_MESSAGE_MAP(CMyPreviewView, CPreviewView)
	ON_COMMAND(AFX_ID_PREVIEW_CLOSE, &CMyPreviewView::OnPreviewClose)
	ON_COMMAND(AFX_ID_PREVIEW_PRINT, &CMyPreviewView::OnPreviewPrint)
END_MESSAGE_MAP()


// CMyPreviewView ��Ϣ�������
void CMyPreviewView::OnPreviewPrint()
{
	// TODO: �ڴ���������������
	//m_pPrintView->SendMessage(WM_COMMAND, ID_FILE_PRINT);
	CMyFrame* pf=(CMyFrame*)::AfxGetMainWnd();
	pf->m_pView->OnMyPrint();
}

void CMyPreviewView::OnPreviewClose()
{
	// TODO: �ڴ���������������
	CMyFrame* pf=(CMyFrame*)::AfxGetMainWnd();
	CWinApp *pApp=AfxGetApp();      
	pApp->m_pMainWnd=pf->m_pMainDlg;
	pf->DestroyWindow();
}

void CMyPreviewView::OnDraw(CDC* pDC)
{
	CPreviewView::OnDraw(pDC);	
	// TODO: �ڴ����ר�ô����/����û���
	m_pToolBar->PostMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);// ������������״̬����
}
