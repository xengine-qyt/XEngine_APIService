﻿#include "pch.h"
#include "Plugin_Zodiac.h"
/********************************************************************
//    Created:     2022/04/21  15:50:22
//    File Name:   D:\XEngine_APIService\XEngine_Source\XEngine_PluginModule\ModulePlugin_Zodiac\Plugin_Zodiac\Plugin_Zodiac.cpp
//    File Path:   D:\XEngine_APIService\XEngine_Source\XEngine_PluginModule\ModulePlugin_Zodiac\Plugin_Zodiac
//    File Base:   Plugin_Zodiac
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     星座生肖
//    History:
*********************************************************************/
CPlugin_Zodiac::CPlugin_Zodiac()
{
	m_StrChinese[0] = _T("猴");
	m_StrChinese[1] = _T("鸡");
	m_StrChinese[2] = _T("狗");
	m_StrChinese[3] = _T("猪");
	m_StrChinese[4] = _T("鼠");
	m_StrChinese[5] = _T("牛");
	m_StrChinese[6] = _T("虎");
	m_StrChinese[7] = _T("兔");
	m_StrChinese[8] = _T("龙");
	m_StrChinese[9] = _T("蛇");
	m_StrChinese[10] = _T("马");
	m_StrChinese[11] = _T("羊");
}
CPlugin_Zodiac::~CPlugin_Zodiac()
{
}
//////////////////////////////////////////////////////////////////////////
//                       公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：PluginCore_Init
函数功能：初始化插件模块
 参数.一：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：N
  意思：自定义参数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CPlugin_Zodiac::PluginCore_Init(LPVOID lParam)
{
	Zodiac_IsErrorOccur = FALSE;

    return TRUE;
}
/********************************************************************
函数名称：PluginCore_UnInit
函数功能：卸载插件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CPlugin_Zodiac::PluginCore_UnInit()
{
	Zodiac_IsErrorOccur = FALSE;

	return TRUE;
}
/********************************************************************
函数名称：PluginCore_Call
函数功能：调用插件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CPlugin_Zodiac::PluginCore_Call(TCHAR*** pppHDRList, int nListCount, int* pInt_HTTPCode, TCHAR* ptszMsgBuffer, int* pInt_MsgLen)
{
	Zodiac_IsErrorOccur = FALSE;

	if ((NULL == pInt_HTTPCode) || (NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		Zodiac_IsErrorOccur = TRUE;
		Zodiac_dwErrorCode = ERROR_XENGINE_APISERVICE_PLUGIN_MODULE_ZODIAC_PARAMENT;
		return FALSE;
	}
	TCHAR tszKeyName[128];
	TCHAR tszValueName[128];
	TCHAR tszChineseZodiac[64];
	TCHAR tszEnglishZodiac[64];
	Json::Value st_JsonRoot;
	Json::Value st_JsonObject;
	Json::StreamWriterBuilder st_JsonBuilder;

	memset(tszKeyName, '\0', sizeof(tszKeyName));
	memset(tszValueName, '\0', sizeof(tszValueName));
	memset(tszChineseZodiac, '\0', sizeof(tszChineseZodiac));
	memset(tszEnglishZodiac, '\0', sizeof(tszEnglishZodiac));

	BaseLib_OperatorString_GetKeyValue((*pppHDRList)[1], "=", tszKeyName, tszValueName);
	Plugin_Zodiac_Chinese(tszValueName, tszChineseZodiac);
	Plugin_Zodiac_English(tszValueName, tszEnglishZodiac);

	st_JsonObject["tszDate"] = tszValueName;
	st_JsonObject["tszChineseZodiac"] = tszChineseZodiac;
	st_JsonObject["tszEnglishZodiac"] = tszEnglishZodiac;
	st_JsonRoot["data"] = st_JsonObject;
	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = "success";
	st_JsonBuilder["emitUTF8"] = true;

	*pInt_HTTPCode = 200;
	*pInt_MsgLen = Json::writeString(st_JsonBuilder, st_JsonRoot).length();
	memcpy(ptszMsgBuffer, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), *pInt_MsgLen);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                       保护函数
//////////////////////////////////////////////////////////////////////////
BOOL CPlugin_Zodiac::Plugin_Zodiac_Chinese(LPCTSTR lpszDate, TCHAR* ptszCZodiac)
{
	Zodiac_IsErrorOccur = FALSE;

	int nDate = _ttoi(lpszDate);
	int nYear = nDate / 10000;
	nYear = nYear % 12;

	_tcscpy(ptszCZodiac, m_StrChinese[nYear].c_str());
	return TRUE;
}
BOOL CPlugin_Zodiac::Plugin_Zodiac_English(LPCTSTR lpszDate, TCHAR* ptszEZodiac)
{
	Zodiac_IsErrorOccur = FALSE;

	int nMonth = 0;
	int nDay = 0;
	Algorithm_String_GetMemoryInt(lpszDate, 4, 5, &nMonth);
	Algorithm_String_GetMemoryInt(lpszDate, 6, 7, &nDay);

	string m_StrEnglish[12][2] =
	{
		{ "摩羯座","水瓶座" },//一月
		{ "水瓶座","双鱼座" },//二月
		{ "双鱼座","白羊座" },//三月
		{ "白羊座","金牛座" },//四月
		{ "金牛座","双子座" },//五月
		{ "双子座","巨蟹座" },//六月
		{ "巨蟹座","狮子座" },//七月
		{ "狮子座","处女座" },//八月
		{ "处女座","天秤座" },//九月
		{ "天秤座","天蝎座" },//十月
		{ "天蝎座","射手座" },//十一月
		{ "射手座","摩羯座" },//十一月
	};
	//每个月有两个星座，数组中的值对应每个月中两个星座的分割日期
	int nConstell[]{ 20,19,21,20,21,22,23,23,23,24,23,22 };
	/*第一维输出月份，第二维由生日的日期/每个月对应的分割日期（constell_dates）,
	   为0就表示一个月中的前一个星座，否则为后一个星座，
	比如生日为5月6号，则对应输出 constells[4][0] 对应金牛座*/
	_tcscpy(ptszEZodiac, m_StrEnglish[nMonth - 1][nDay / nConstell[nMonth]].c_str());
	return TRUE;
}