#pragma once

#define VERSION_SOFTWARE _T("2.00")//����汾

#define CONFIG_FILE       _T("\\config\\config.ini")
#define CHS_FILE         _T("\\config\\Multi_ch.ini")
#define ENG_FILE         _T("\\config\\Multi_eng.ini")

#define SET_LOSS_MAX 60    //y����ʾ����DB
#define SCALE_BIG   8    //������̶ȳ��߶εĳ���
#define SCALE_SMALL  4   //������̶ȶ��߶εĳ���
#define SCALE_MAX   6    //����ϵ����Χ
#define SCALE_MIN   1   //����ϵ����Χ
#define SCALE_STEP   5   //����ϵ����Χ
#define  OTDR_EVENT_NUM_MAX 100//�¼������

const unsigned C = 299792458 ; //����еĹ���m
const unsigned C_zll = 299792458 ; //����еĹ���m

#define  EVENT_POINT_HEIGHT 30//�����߻����¼���ʱ���һ����ֱ�ߣ���ֵΪ���߸߶�
#define  EVENT_POINT_NUM_HEIGHT 20//�����߻����¼���ʱ���һ����ֱ�ߣ���ֵΪ��ֱ���·���1,2..��ֱ����λ��
#define POINT2FLOATX(x) m_fRangeX[0] + (FLOAT)( (x - m_rtCurve.left)   * m_fResolutionX)
#define POINT2FLOATY(y) m_fRangeY[0] + (FLOAT)( (m_rtCurve.bottom - y) * m_fResolutionY)
#define FLOAT2POINTX(x) m_rtCurve.left + (x - m_fRangeX[0]) * m_rtCurve.Width() / (m_fRangeX[1] - m_fRangeX[0])//���ݾ���õ�X����
#define FLOAT2RATIO(x) (x-m_fRangeX[0]) / (m_fRangeX[1] - m_fRangeX[0])//xjf 2014.03.03  ��ȡ����ϵ��

#define COLOR_BK RGB(0, 0, 0)  //���߻������򱳾���ɫ
#define COLOR_A2B RGB(255, 255, 0)  //A2B�ߵ���ɫ
#define COLOR_B2A RGB(0, 255, 0)  //B2A�ߵ���ɫ
#define COLOR_NOSELECT_EVENT RGB(0,0,255)  //δѡ���¼�����ɫ
#define COLOR_SELECT_EVENT RGB(255,0,0)  //ѡ���¼�����ɫ
#define COLOR_EVENT_RECT RGB(128,64,64)  //�¼����ֱ߿����ɫ
#define COLOR_EVENT_NUM RGB(48,48,48)  //�¼������ı�����ɫ
#define COLOR_SCALE_TEXT RGB(180,180,180)  //�̶����ֵ���ɫ
#define COLOR_SCALE_BK RGB(0,0,0)  //�̶ȱ�������ɫ
#define COLOR_GRID_BK RGB(180,180,180)   //դ�����ɫ
#define OTDR_TRACE_CURSOR_AB_MARGIN_TOP 30//���AB��ֱ�����붥����Զ
#define OTDR_TRACE_CURSOR_AB_VALID_SELE 6//��������֮����Ϊѡ����A/B��
#define OTDR_TRACE_CURSOR_AB_COLOR_SELE RGB(255,0,0)//A/B��ѡ����ɫ

#define DIVISOR_TOKM  1000;//

#define OTDR_TRACE_SELNEAR  3
//ghq 2016.4.15
#define OTDR_TRACE_DEFAULT_ZOOM_PERCENT 0.02//2%Ĭ�����ű��� 
#define OTDR_TRACE_Y_MIN 0
#define COUNT_GRIDLINE_HORIZONE 10//��ͼ�á�ˮƽ������//wcq2011.12.19
#define COUNT_GRIDLINE_VERTICAL 8//��ͼ�á���ֱ������//wcq2011.12.19

#define LINE_BOTTOM_OFFSET_X   8   //�¼����ַ���ƫ��
#define LINE_BOTTOM_WIDTH      16   //�¼����ַ����
#define LINE_BOTTOM_HEIGHT     18   //�¼����ַ����
#define LINE_TOP               -10  //�¼���������ƫ��
#define LINE_BOTTOM            20

#define RETURN_OK               0
#define RETURN_ERROR            -1


#define RECT_CURVE_LEFT        110   //������ͼ��������31
#define RECT_CURVE_TOP         68   //
#define RECT_CURVE_WIDTH       769  //848
#define RECT_CURVE_HEIGHT      245  //200

#define RECT_EVENT_LEFT        110   //�¼���ͼ��������
#define RECT_EVENT_TOP         335   //
#define RECT_EVENT_WIDTH       769  //
#define RECT_EVENT_HEIGHT      200

#define BUTTON_FILE_LEFT        30   //���ļ���ť����
#define BUTTON_PRINT_LEFT       110   //
#define BUTTON_LANG_LEFT        190   //
#define BUTTON_SETUP_LEFT       270   //
#define BUTTON_ABOUT_LEFT       350   //
#define BUTTON_EXIT_LEFT        430   //

#define TEXT_FILE1_LEFT         530   //
#define TEXT_FILE1_TOP          20   //
#define TEXT_FILE2_LEFT         530   //
#define TEXT_FILE2_TOP          40   //

#define BUTTON_ZOOMIN_V_LEFT        30   //
#define BUTTON_ZOOMIN_V_TOP        68   //
#define BUTTON_ZOOMOUT_V_LEFT        30   //
#define BUTTON_ZOOMOUT_V_TOP        108   //
#define BUTTON_ZOOMIN_BOTH_LEFT        70   //
#define BUTTON_ZOOMIN_BOTH_TOP        68   //
#define BUTTON_ZOOMOUT_BOTH_LEFT        70   //
#define BUTTON_ZOOMOUT_BOTH_TOP        108   //
#define BUTTON_ZOOMIN_H_LEFT          30   //
#define BUTTON_ZOOMIN_H_TOP          148   //
#define BUTTON_ZOOMOUT_H_LEFT        70   //
#define BUTTON_ZOOMOUT_H_TOP        148   //

#define BUTTON_RESET_LEFT         30   //��ԭ��ť����
#define BUTTON_RESET_TOP          188   //��ԭ��ť����

#define BUTTON_ADDEVETNT_LEFT         30   //����¼���ť����
#define BUTTON_ADDEVETNT_TOP          228   //
#define BUTTON_DELEVETNT_LEFT         70   //ɾ���¼���ť����
#define BUTTON_DELEVETNT_TOP          228   //

#define CHECK_CURSORAB_LEFT        30   //
#define CHECK_CURSORAB_TOP        273   //
#define RADIO_AANDB_LEFT              30   //
#define RADIO_AANDB_TOP              (CHECK_CURSORAB_TOP+25)   //
#define RADIO_AORB_LEFT            70   //
#define RADIO_AORB_TOP               (CHECK_CURSORAB_TOP+25)   //

#define RADIO_AB_LEFT        30   //
#define RADIO_AB_TOP         340   //
#define RADIO_BA_LEFT        30   //
#define RADIO_BA_TOP         370   //
#define RADIO_BOTH_LEFT      30   //
#define RADIO_BOTH_TOP       400   //

#define TOP_BUTTON_TOP          15  //
#define TOP_BUTTON_WIDTH        75  //
#define TOP_BUTTON_HEIGHT       40
#define LOWER_BUTTON_WIDTH      36  //
#define LOWER_BUTTON_HEIGHT     36
#define RADIO_WIDTH             40  //
#define RADIO_HEIGHT            15
#define TEXT_WIDTH              500  //
#define TEXT_HEIGHT             20

#define  M_FSTARTDB0 -65.63//-65.53
#define  M_FMAXDB0   -5.63//-9.23
#define  M_FSTARTDB1 -58.3//����������Сֵ�� > ZOOM_VERTICAL_STANDARD 30*1000ʱʹ��
#define  M_FMAXDB1   -8.3//��-1��Ϊ-8.3
#define  CLEAR_DATAPOINTS_NUM 1//���²��Ժ��������ʱ����Ӧ������pDataPointsNumֵ����Ϊ0���׵��³�0,��ʾ���.wcq2011.12.23

#define  EVENT_LIST_COL   8
#define  EVENT_LIST_PRINT_MAX   18

#define  MY_PRINT_TEST   1
#define  OTDR_TRACE_Y_MAX    60
#define  OTDR_TRACE_Y_MAX_2WAY    60
#define  OTDR_TRACE_Y_FACTOR 1000//y�������ΧΪ�����ͣ����Ŵ�һ��������nPageΪ0��������Ҫ�Ŵ�һ��������������Ϊ1000//zll 2013.4.24

#define  MAX_FILE_PATH       350   //
#define  MARGIN_LEFT_COUNT 18  //�������յĿ��ռ�ܿ�ȵļ���֮��
#define  PRINTMARGIN 2
#define  EVENT_2WAY_OFFSET   20

#define	WM_MY_PRINT			    (WM_USER+1003)
#define	WM_BEGIN_PRINTING		(WM_USER+1004)
#define	WM_END_PRINTING			(WM_USER+1005)

const float FONTSIZE_RATIO[5] = {1.0, 0.8, 0.6, 0.45, 0.4};  //�������ʱ�õ���������ű�����[0]��ʾ������[1]��ʾ2ͼ/ҳ��[2]��ʾ4ͼ/ҳ��[3]��ʾ6ͼ/ҳ��[4]��ʾ8ͼÿҳ


//����������ʾ���͵�ö��
typedef enum ShowType
{
	SHOW_A2B = 0,
	SHOW_B2A = 1,
	SHOW_ALL = 2
};

typedef enum ButtonType
{
	ZOOM_RESET = -1,
	ZOOM_IN_BOTH = 0,
	ZOOM_OUT_BOTH = 1,
	ZOOM_IN_H = 2,
	ZOOM_OUT_H = 3,
	ZOOM_IN_V = 4,
	ZOOM_OUT_V = 5,
};


enum OTDR_TRACE_AB_SELECT//���ABѡ��״̬
{
	OTDRTRACE_AB_SELE_DISABLE=-1,
	OTDRTRACE_AB_SELE_NONE=0,
	OTDRTRACE_AB_SELE_A,
	OTDRTRACE_AB_SELE_B,
	OTDRTRACE_AB_SELE_AB,
	OTDRTRACE_AB_SELE_EVENT,
};
enum OTDR_TRACE_AB_TYPE//���ABģʽ  
{
	OTDRTRACE_AB_TYPE_SINGLE = 0,//ѡһ��(A/B)
	OTDRTRACE_AB_SELE_BOTH//ABģʽ
};

enum OTDR_EVENT_TYPE//�¼����� 
{
	OTDR_EVENT_TYPE_NOREFLECT = 0,//ѡһ��(A/B)
	OTDR_EVENT_TYPE_REFLECT//ABģʽ
};

//ͼ�λ�������
enum CURSOR_SELECT : int
{
	CURSOR_NONE = 0,
	CURSOR_A = 1,		//ѡ��A
	CURSOR_B = 2,		//ѡ��B
	CURSOR_AB = 4,		//ѡ��AB
	CURSOR_EVENT = 8	//ѡ���¼�
};

enum CURVE_CURRECT_MODE
{
	CURVE_NONE=0,
	CURVE_GET_DATA,
	CURVE_SELECT,
	CURVE_ZOOM_OUT ,
	CURVE_ZOOM_OUT_V,
	CURVE_ZOOM_OUT_H,
	CURVE_ZOOM_IN,
	CURVE_ZOOM_IN_V,
	CURVE_ZOOM_IN_H,
	CURVE_MOVE,
	CURVE_MOVE_UP,
	CURVE_MOVE_DOWN,
	CURVE_MOVE_LEFT,
	CURVE_MOVE_RIGHT,
	CURVE_RESET
};

//support language
enum LANG : int
{
	CHS = 0,   //Chinese
	ENG = 1,   //English
};



