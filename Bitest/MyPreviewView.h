#include <afxpriv.h>
// CMyPreviewView ��ͼ
#include "MyFrame.h"
#include "MyView.h"

class CMyPreviewView : public CPreviewView
{
	DECLARE_DYNCREATE(CMyPreviewView)

protected:
	CMyPreviewView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMyPreviewView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPreviewClose();
	afx_msg void OnPreviewPrint();
	virtual void OnDraw(CDC* /*pDC*/);

	CMyView * m_pView;
};