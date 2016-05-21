#ifndef _DATASTRUCT_H_
#define _DATASTRUCT_H_


#include <vector>
//#include "EndGabor.h"
#include "GaborResult.h"
#include "global.h"

struct EventInfo //�¼�����Ϣ�ṹ��
{
	unsigned int index;//λ����������ĵڼ�����
	unsigned int nEventType;//�¼����ͣ�0�����¼���1�Ƿ����¼�
	float fLocation;//�¼�λ��,(C/(2nf))*index,���ݵڼ������õ�����//2011.11.24
	float fEventloss;//���//2011.11.24
	float fEventHigh;////�¼��㷴��dB//2011.11.24
	float fLineK;////����б��//2011.11.24
	float fTotalLoss;////����ġ��ۼ����//2011.11.24
	float fReflect;//����ֵ
	float fTrailEnd;////β��λ��
	char  cEventType;// event code �ڶ�������  S:��ʼ�¼� E:����ĩ�˼������¼���A���û��Լӣ� F:������в��ң�O:����������Χ�Ĺ���ĩ�� D: wcq2012.06.11
};

struct EventInfoList//�¼�����Ϣ�б�
{
	~EventInfoList()
	{
		vEventInfo.clear();
	}
	std::vector<EventInfo> vEventInfo; //��������յ��¼�����
};
//struct DataScales//wcq20111101
//{
//	short scaleFactor;//����2�ֽ�
//	std::vector<unsigned short> vData;//
//};
//std::vector<DataScales> vAllData;//��������
struct TestResultInfo 
{
	TestResultInfo()//wcq 2011.11.1
	{
		IsMaxLength = FALSE;
		IsLowerStandard = FALSE;
		IsLoadFromFile = TRUE;
		DataType = 8888; //�������ͣ� ������8888��������4444
		TestMode = 2222; //����ģʽ��ƽ������2222��
		DataSerialNum = 0; //���ݰ����
		pDataPoints = NULL; //������ֵ ������1000����ֵ
		pDataCurve = NULL; //����ȡ�������������
		ScaleFactor = 1;//����ϵ����ÿ�����ݵ㶼�˸�ϵ��
		YOffset = 0;//��ֱ����ƫ�ƣ�����ƽ��
		StartPoint = 0;  //���ߺ���������
		MaxPoint = 0;    //���ߺ�������������ֵ
		FullPointCount = 0;//��������ʱΪ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��wcq2011.09.16
		FullPointCountMin = 0;//����δ����,Ϊ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��,FullPointCountΪ�����ı�����wcq2011.09.21
		LastZoomPointCount = 0;//��������ʱ���һ�����Ź����ٵ㣬��ǰ������ŵĵ�ĸ�����һ��һ����wcq2011.09.21
		DataSpacing = 0;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������
		Refraction = 0.0f; //������
		ChainLoss = 0.0f; //����ģ�������1000��
		ChainAttenu = 0.0f; //��˥����������1000��
		MaxTestDistance = 0.0f; //��������������
		DataPointsNum = 0; //���������
		Frequency = 0; //Ƶ��
		Range = 0; //������Χ
		PulseWidth = 0; //������
		WaveLength = 0; //���Ĳ���
		MeasureTiem = 0; //����ʱ��
		ChainLength = 0; //����
		ptDrawData = NULL; //��ͼ�����,ÿ�����Ӧһ����ͼ����ָ��,ԭ������ΪCOTDRPage����m_ptRecord wcq2011.09.21

		//
		Atten = 0.0f;//˥��
		Average = 0.0f;//ƽ��
		SampleCount = 0;//������
		FusionLoss = 0.0f;//�۽����
		ReturnLoss = 0.0f;//�ز����

		//��������
		Relay = 0;//�̵���//wcq2011.12.06
		Joint1 = 0;//ƴ�ӵ�1//wcq2011.12.05
		Joint2 = 0;//ƴ�ӵ�2//wcq2011.12.05
		Length = 0; //���߳���(���˳���)
		ThumbDrawXCount = 0;//����ͼ����ʱx����ֵ���ֵ�������Ϊ����ͼ������
		dataCurveCount = 0;//pDataCurve�����,��һ��ΪCOUNT_POINT_MAX_MIN��wcq2011.11.23
		curveColor = RGB(0,0,255); //������ɫ
		pThumbData = NULL;//����ͼ����
//��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ wcq2013.04.26
		pDataPointsSmooth = NULL;

		BackscatterCoefficient = 0;//ɢ��ϵ�� wcq2012.10.30
	    LossThreshold = 0;//˥������ wcq2012.10.30
	    ReflectanceThreshold = 0;//�������� wcq2012.10.30
	    EndOfFiberThreshold = 0;//����ĩ������ wcq2012.10.30
	}
	~TestResultInfo()//wcq 2011.11.1
	{
		if (NULL != pDataPoints)
		{
			delete[] pDataPoints;
			pDataPoints = NULL;
		}
		if (NULL != pDataCurve)
		{
			delete[] pDataCurve;
			pDataCurve = NULL;
		}
		if (NULL != ptDrawData)
		{
			//delete[] ptDrawData;
			ptDrawData = NULL;
		}
		if (NULL != pThumbData)
		{
			delete[] pThumbData;
			pThumbData = NULL;
		}
		

	//��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������ wcq2013.04.26
		//m_TestResult.pDataPointsSmooth=NULL;
		if (NULL != pDataPointsSmooth)
		{
			delete[] pDataPointsSmooth;
			pDataPointsSmooth=NULL;
		}

		EventList.vEventInfo.clear();
	}
	BOOL IsMaxLength; //�Ƿ��������
	BOOL IsLowerStandard;//wcq2011.09.26������ZOOM_VERTICAL_STANDARD�Ƚ϶�Ϊtrue��false.������ֱ��������ʱȷ���������
	BOOL IsLoadFromFile;//�Ƿ���ļ���������������//wcq2011.10.19
	WORD DataType; //�������ͣ� ������8888��������4444
	WORD TestMode; //����ģʽ��ƽ������2222��
	WORD DataSerialNum; //���ݰ����
	WORD* pDataPoints; //������ֵ ������1000����ֵ
	CPoint* pDataCurve; //����ȡ�������������
	short ScaleFactor;//����ϵ����ÿ�����ݵ㶼�˸�ϵ��
	int YOffset;//��ֱ����ƫ�ƣ�����ƽ��
	int StartPoint;  //���ߺ���������
	int MaxPoint;    //���ߺ�������������ֵ
	int FullPointCount;//��������ʱΪ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��wcq2011.09.16
	int FullPointCountMin;//����δ����,Ϊ���ٸ���ʱ����ռ��������ͼ����,����ȷ�����߿��,FullPointCountΪ�����ı�����wcq2011.09.21
	int LastZoomPointCount;//��������ʱ���һ�����Ź����ٵ㣬��ǰ������ŵĵ�ĸ�����һ��һ����wcq2011.09.21
	float DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19���Ϊ��������
	float Refraction; //������1310nm
	float Refraction1550; //������1550nm//zll 2012.9.18
	float ChainLoss; //����ģ�������1000��
	float ChainAttenu; //��˥����������1000��
	float MaxTestDistance; //��������������
	unsigned long int DataPointsNum; //���������
	float Frequency; //Ƶ��
	long int Range; //������Χ
	long int PulseWidth; //������
	long int WaveLength; //���Ĳ���
	long int MeasureTiem; //����ʱ��
	long int ChainLength; //����
	CPoint* ptDrawData; //��ͼ�����,ÿ�����Ӧһ����ͼ����ָ��,ԭ������ΪCOTDRPage����m_ptRecord wcq2011.09.21
	EventInfoList EventList;//�¼��б�
//��ʾʱ��ƽ���������ݣ�����ʱ��ʵ������
	WORD* pDataPointsSmooth; //������ֵ ������1000����ֵ,ԭʼ���ݾ���ƽ������������


	//
	float Atten;//˥��
	float Average;//ƽ��
	int   SampleCount;//������
	float FusionLoss;//�۽����
	float ReturnLoss;//�ز����

	//��������
	WORD Relay;//�̵���//wcq2011.12.06
	WORD Joint1;//ƴ�ӵ�1//wcq2011.12.05
	WORD Joint2;//ƴ�ӵ�2//wcq2011.12.05
	float Length; //���߳���(���˳���)
	unsigned int ThumbDrawXCount;//����ͼ����ʱx����ֵ���ֵ�������Ϊ����ͼ������
	int dataCurveCount;//pDataCurve�����,��һ��ΪCOUNT_POINT_MAX_MIN��wcq2011.11.23
	COLORREF curveColor; //������ɫ
	CPoint*  pThumbData;//����ͼ����

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//������� wcq2012.10.30
	float BackscatterCoefficient;//ɢ��ϵ�� wcq2012.10.30
	float LossThreshold;//˥������ wcq2012.10.30
	float ReflectanceThreshold;//�������� wcq2012.10.30
	float EndOfFiberThreshold;//����ĩ������ wcq2012.10.30
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	Gabor_Result m_GaborResult;//�¼��б�
	CString strTestTime;//ghq 2016.5.3 ����ʱ��
	CString strFileName;//ghq 2016.5.3 �ļ���
};

//�����ϴ����ݲ���
typedef struct _OTDRAllUploadData
{
	_OTDRAllUploadData()
	{
		DataPointsNum = 0;
		PData = NULL;
	}
	~_OTDRAllUploadData()
	{
		if(PData)
		{
			delete []PData;
		    PData = NULL;
		}
	}
	INT GetLength()
	{
		return 16 + DataPointsNum*2+1 + VEventInfo.size()*6*4+1;//16:������������;DataPointsNum+1:��������;VEventInfo.size()*6+1:�¼��б�
	}//eif
	//��������
	UINT SampleFrequence;//����Ƶ��
	UINT TestRange;//������Χ
	UINT PulseWidth;//����
	UINT WaveLength;//�����Ⲩ��
	UINT TestTime;//����ʱ��
	FLOAT Refract;//������
	FLOAT LinkLength;//����
	FLOAT LinkLoss;//�����
	FLOAT LinkAttenuation;//��˥��
	FLOAT Loss;//˥��
	FLOAT FiberEnd;//ĩ������
	UINT TestMode;//OTDR����ģʽ OtdrMode
	UINT AutoTest;//���Է�ʽ 0:�Զ�,1:�ֶ�
	FLOAT Reflect;//��������
	FLOAT Slope;//б������
	FLOAT ScatterCoefficient;//ɢ��ϵ������
	//16
	//ghq 2016.4.14
	FLOAT DataSpacing;//���Ϊ��������,������������
	FLOAT Length;//����

	//���Խ������
	UINT DataPointsNum;//���ݵ����
	WORD* PData;//����

	//�¼�����Ϣ
	UINT EventNum;//�¼������
	struct ENVENT_INFO
	{
		UINT Index;//�¼����λ�������ϴ�����������
		UINT EventType;//�¼�����,0/1/2/3 ��Ӧ ��ʼ/����/�Ƿ���/����
		FLOAT Reflect;//����
		FLOAT Loss;//�������
		FLOAT Slope;//ƽ��˥��ϵ��
		FLOAT TotalLoss;//�¼���౻����·��ʼ�����·�����
	};
	std::vector<ENVENT_INFO> VEventInfo;
}OTDR_ALL_UPLOAD_DATA,*POTDR_ALL_UPLOAD_DATA;

//����ṹ�屣�����������Ҫ������
typedef struct CoordinateCoef
{
	CoordinateCoef()
	{
		iHorizontal = 6;
		iVertical = 10;
		fXmin = 0;
		fXmax = 80;
		fYmin = 0;
		fYmax = OTDR_TRACE_Y_MAX;
	}
	
	CRect rcWnd;  //���ƴ��ڵĳߴ�
	UINT iHorizontal;  //ˮƽ�ߵ�����
	UINT iVertical;    //��ֱ�ߵ�����
	float fXmin;       //X����Сֵ
	float fXmax;       //X�����ֵ
	float fYmin;       //Y����Сֵ
	float fYmax;       //Y�����ֵ
};

typedef struct PrintQuest 
{	
	CString strTitle;//�������
	CString strID;//������
	CString strContractor; //�а���
	CString strCustomer;  //�û�
	CString strSorFileName; //�ļ���
	CString strTestDate;   //��������
	CString strPlayer;   //����Ա
	CString strComment;

	//value
	CString QuestID;    //������
	CString Contractor; //�а���
	CString Customer;  //�û�
	CString SorFileName; //�ļ���
	CString TestDate;   //��������
	CString Player;   //����Ա
	CString Comment;
};

typedef struct PrintConfig 
{	
	//name
	CString strTitle;  //��������
	CString strWavelength;   //��������
	CString strPulse;    //����
	CString strOrigin;    //���
	CString strOriCable;   //�����º�
	CString strOriFiber;   //�����˺�
	CString strOriColor;   //��������ɫ

	CString strRange;    //����
	CString strAverage;  //ƽ���Ĵ���
	CString strEnd;     //�յ�
	CString strEndCable;   //�յ���º�
	CString strEndFiber;   //�յ���˺�
	CString strEndColor;   //�յ���ɫ
	CString strOriLocation;//���λ��
	CString strEndLocation;   //�յ�λ��

	CString strBackScatter;  //����ɢ��ϵ��
	CString strLossThreshold;  //˥������
	CString strRefThreshold;  //��������
	CString strEndThreshold;  //����ĩ������
	CString strIndex;    //����ʱ�õ�������
	CString strResolution;  //������
	CString strTitleAB;  //AB����
	CString strTitleBA;  //BA����
	//value
	CString OriCable;   //�����º�
	CString OriFiber;   //�����˺�
	CString OriColor;   //��������ɫ
	CString EndCable;   //�յ���º�
	CString EndFiber;   //�յ���˺�
	CString EndColor;   //�յ���ɫ
	CString OriLocation;//���λ��
	CString EndLocation;   //�յ�λ��
	CString Wavelength;   //��������
	CString Index;    //����ʱ�õ�������
	CString Pulse;    //����
	CString Range;    //����
	CString Average;  //ƽ���Ĵ���
	CString Resolution;  //������
	CString BackScatter;  //����ɢ��ϵ��
	CString LossThreshold;  //˥������
	CString RefThreshold;  //��������
	CString EndThreshold;  //����ĩ������
};

typedef struct PrintTrace
{	
	CString strTitle;  //�켣�߱���
	CString LocationA;   //A��λ��
	CString LossA;    //A�괦���
	CString LocationB;  //B��λ��
	CString LossB;  //B�괦���
	CString DistanceAB;  //ABλ�ü��
	CString LossAB;  //AB��Ĳ�
	CString SlopeAB;  //AB�����б��
};

struct WaveConfig 
{
	COLORREF ColorCurve1;  //����1��ɫ
	COLORREF ColorCurve2;  //����2��ɫ
	COLORREF ColorBack;  //������ɫ
	COLORREF ColorGrid;  //դ����ɫ
	COLORREF ColorEvent;  //�¼���ɫ
	COLORREF ColorCursor;  //�����ɫ
	BOOL ShowGrid; //�Ƿ���ʾ����
	BOOL ShowCursor; //�Ƿ���ʾ���
	BOOL ShowDoubleCursor; //�Ƿ���ʾ˫���
	BOOL MultiMode; //�Ƿ�Ϊ��������ʾģʽ
	//Unit Page
	int UnitDbFig; //Dt�̶ȵ�λ����5\4\3
	int UnitDistFig; //����̶ȵ�λ����
	CString DistUnit; //���뵥λ
};

typedef struct PrintEvent 
{	
	CString strTitle;
	int EventColumn;    //�¼��б�����
	int EventCount; //�¼�����
};

//���еĴ�ӡ��Ϣ
struct PrintDetail
{
	CString strTitle;
	CString strLogo;
	PrintQuest stPrintQuest;
	PrintConfig stPrintConfig; //������Ϣ
	PrintTrace stPrintTrace;
	WaveConfig stWaveConfig;
	PrintEvent stPrintEvent;
	TestResultInfo stTestResultInfo[3];
	INT curSelection;//�ļ��б�ǰѡ���ĸ�
	INT totalCurveNumber;
	CString WaveList;
};

//����A\B���߻�������
struct CursorInfo
{
	BOOL bDrawAB; //�Ƿ����
	FLOAT fRatioA; //A����������еı���ϵ��	
	CString strDistanceA;
	CString strDBA;
	CString strA;
	

	FLOAT fRatioB;//B����������еı���ϵ��
	CString strDistanceB;
	CString strDBB;
	CString strB;
};

#endif   //_DATASTRUCT_H_
