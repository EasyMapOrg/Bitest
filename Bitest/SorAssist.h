#pragma once
//#include "CurveData.h"
#include "OtdrTrace.h"
//#include "EndGabor.h"
#include "GaborResult.h"
#include "SORDATA.h"
#include "DATASTRUCT.h"
#include "math.h"

//sor������
//��Ҫ�ǽ���sor.dll���ò���ת��
//wcq2012.07.23
class CSorAssist
{
public:
	CSorAssist(void);
	~CSorAssist(void);
public:
	void convert(SORDATA sorData, TestResultInfo& testResult);//SORDATAת��ΪTestResultInfo
	void convert(TestResultInfo* testResult, SORDATA& sorData);//TestResultInfoת��ΪSORDATA,zll 2012.8.20�޸�TestResultInfoΪTestResultInfo*
	//void AddSordataToOtdrTrace(SORDATA sorData, COtdrTrace* pOtdrTrace, COLORREF colorLine=RGB(0, 0, 255));//��sor�ļ�����������ת��Ϊotdrtrace���� zll2013.04.23
	void AddSordataToOtdrTrace(SORDATA sorData, COtdrTrace* pOtdrTrace, WORD* pDataSmooth=NULL, COLORREF colorLine=RGB(0, 0, 255));//��sor�ļ�����������ת��Ϊotdrtrace���� zll2013.04.23
	void AddSordataToOtdrTrace(TestResultInfo* pTestResultInfo, COtdrTrace* pOtdrTrace, BOOL bShowOnlyOne=FALSE, COLORREF colorLine=RGB(0, 0, 255) ,WORD* pDataSmooth=NULL);//��sor�ļ�����������ת��Ϊotdrtrace����//wcq2016.01.26
	void SetEvents(TestResultInfo& testResultInfo, CGraphElement* graphElement);//���¼��б�ת��Ϊ������ʾ�õ��¼��б�zll2013.04.23
	void CloseCurvedata(int m_listItem,SORDATA sorData, COtdrTrace* pOtdrTrace, COLORREF colorLine=RGB(0, 0, 255));//��sor�ļ�����������ת��Ϊotdrtrace���� zll2013.04.23
	void ReEventAutoSearch(TestResultInfo& testResultInfo, SORDATA sorData, COtdrTrace* pOtdrTrace, WORD* pDataSmooth, COLORREF colorLine,int m_listItem);//zll 2013.6.18 ִ���Զ�����ʱ����sor�ļ�����������ת��Ϊotdrtrace����
	int GetTraceYmax(SORDATA &sorData);  //��ȡһ�����ߵ����ֵ��lzy2014.06.24
	FLOAT GetUserOffset(SORDATA &sorData);//�õ��������˳���//wcq2015.12.07
public:
	//CEndGabor m_endgabor;//zll 2013.8.19//�����ΪCGaborDllUtil g_gaborDllUtil cwcq2016.04.25
};
