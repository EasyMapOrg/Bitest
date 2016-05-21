#pragma once
#include <vector>
#include "SORDATA.h"
#ifdef _CLASSINDLL
#define CLASSINDLL_CLASS_DECL __declspec(dllexport)
#else
#define CLASSINDLL_CLASS_DECL __declspec(dllimport)
#endif

#define FILE_SUCCESS 1//文件正确解析
#define ERROR_EMPTY_NAME 2//文件名为空
#define ERROR_FILE_NOTFOUND 3//未能打开文件
#define ERROR_CREATE_FILE 4//未能创建文件
#define ERROR_FORMAT 5//文件格式化失败
///////////////////////////////////////////////////////////////
//readSorFile/saveSorFile返回值说明：
//ERROR_EMPTY_NAME: 文件名为空
//ERROR_FILE_NOT_FOUND: 未能打开文件
//ERROR_CREATE_FILE:未能创建文件
/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//使用方法//wcq2012.07.24
//保存:
//m_sorAssist.convert(m_testResultInfo,m_sorDATA);
//m_sorfileBase.saveSorFile(_T("e:\\a.sor"),m_sorDATA);
//
//读取：
//if(m_sorfileBase.readSorFile(temp,m_sorDATA))
//{
//	m_sorAssist.convert(m_sorDATA, m_testResultInfo);
//}
//其中：m_sorAssist为CSorAssist变量,m_sorDATA为SORDATA变量,m_testResultInfo为TestResultInfo变量
///////////////////////////////////////////////////////////////////
class CSorFile
{
//public:
//	CSorFile(void);
//	~CSorFile(void);
public:
	virtual INT readSorFile(CString filePathAndName,SORDATA& sorData);//wcq2012.07.23
	virtual INT readSorFile(BYTE *pData, int nLength, SORDATA& sorData); //lzy2014.11.11直接读内存sor文件
	virtual INT saveSorFile(CString filePathAndName,SORDATA& sorData);//wcq2012.07.23

public:
	virtual void Release();
};
