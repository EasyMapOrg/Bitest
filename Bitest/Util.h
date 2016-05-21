#pragma once
#include "GrandwayReg.h"
#include "locale.h"//
/***********************************************************
��:Util
wcq 090730
���ܣ������࣬���ڵõ�FHP2ϵ�й⹦�ʼƻ�FH2S��Դ�Ĵ������Ƶ�
ע�⣺������Ҫ����GrandwayComm��GrandwayReg��
*************************************************************/

/*******************************************************************************/
/* ��λ�������                                                                     */
/*****************************************************************************/
#define UNIT_SHOW_KM _T("m")//AB��λ�õ�ѡ����"Km"ʱ��ʾΪ"m"
#define UNIT_SHOW_KFEET _T("ft")//AB��λ�õ�ѡ����"kfeet"ʱ��ʾΪ"m"
#define UNIT_SHOW_MILE _T("mi")//AB��λ�õ�ѡ����"miles"ʱ��ʾΪ"m"

class CUtil
{
public:
	CUtil(void);
public:
	~CUtil(void);
public:
	CString m_newFhpCommName;//FHSϵ�й�Դ��������
	CString m_standFhpCommName;//��׼��������
	CGrandwayReg m_reg;
public:
	CString GetTimeWaste(LONG longbegintime,LONG longendtime);//��ʱ
	CString getCurTimeStr(BOOL bContainHour=TRUE);//�õ���ǰʱ����ַ�����ʾ��bContainHour���Ƿ����ʱ:��:��
	//
	static INT splitString(CString str,char split,CStringArray& strArray);//�ֽ��ַ���
	CString getAddSerial(CString str,INT last);//�����Զ���1
	BOOL isInArray(CStringArray& strArray,CString commname);
	//
	char* old_locale;
	void changeLocaleToChs();//��ȡ�ı��ļ�ʱΪʱstdiofile���ܴ���������Ҫ��ת������
	void recoverLoacle();//ǰ�溯���ķ�����

	//
	static CString getExePath();//��ȡ�ļ�����·��
	static BOOL isFileExist(CString filename);//�鿴�ļ��Ƿ����
	void openFolder(CString filepath);//���ļ���

	//
	void setListToReport(CListCtrl &list);//�޸�list��ʽΪ������ʽ
	void setListToReportCheck(CListCtrl &list);//�޸�list��ʽΪ������ʽ

	//
	CString getPath(HWND hwndOwner);//�ļ���ѡ���

	//
	BOOL isFloat(FLOAT f);//�ж�һ���������Ƿ�Ϊ�����ĸ�����
	//����������Դ����Ϊ�ļ�
	BOOL saveBinResourceAsFile(CString saveAsFileName,UINT resID,CString folder=_T(""),BOOL bSubPathOfExe=TRUE);

	
	////��ע������ʼ��������ѡ��(check.radio)
	void initMultiSelect(CDialog* dlg,INT startID,INT totalItem,CString regKey);//��ע������ʼ��������ѡ��(check.radio)
	//����������ѡ��ע�����(check.radio)
	void saveMultiSelect(CDialog* dlg,INT startID,INT totalItem,CString regKey);//����������ѡ��ע�����(check.radio)
	//�������ӵ��ı������ݵ�ע���
	void saveMultiEdit(CDialog* dlg,INT startID,INT totalItem,CString regKey);//�������ӵ��ı������ݵ�ע���
	//��ע������ʼ���������ı�������
	void initMultiEdit(CDialog* dlg,INT startID,INT totalItem,CString regKey,CString defaultValues=_T(""));//��ע������ʼ���������ı�������
	INT getMultiSelect(CDialog* dlg,UINT startID,INT totalItem);//�õ����������checkbox,radioѡ�����ĸ�������ƫ�Ƶ�һ��λ��
	//������ѡ���¼��еõ�ѡ����ı�
	CString getComboSelText(CComboBox* comboBox);//������ѡ���¼��еõ�ѡ����ı�
	
	////���ı���floatֵ
	void setEditFloat(CDialog* dlg,UINT ID,FLOAT f,INT precision=2);
	//
	void ReStart(BOOL bNormal);//��������
	//
	CString pCharToCString(char* pchar);//char*ת��ΪCString
	CString pTCharToCString(TCHAR* pchar);//TCHAR*ת��ΪCString
	INT CStringToPChar(CString str, char* pchar);//CStringת��Ϊchar*
	float KmeterToOther(float values,CString strUnit);//ǧ��ת��ΪKfeet �� mile
	CString ReadLangString(const CString& strSection,const CString strKey);
	CString GetExePath();//��ȡ�ļ�����·��
	CString GetLangFilePath();
	CString ReadConfigInfo(const CString& strSection, const CString strKey);
	bool WriteConfigInfo(const CString& strSection, const CString& strKey, const CString& strVal);

};