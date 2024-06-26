﻿#pragma once
/********************************************************************
//    Created:     2023/01/10  11:39:12
//    File Name:   D:\XEngine_APIService\XEngine_Source\XEngine_PluginModule\ModulePlugin_Phone\PluginPhone_Define.h
//    File Path:   D:\XEngine_APIService\XEngine_Source\XEngine_PluginModule\ModulePlugin_Phone
//    File Base:   PluginPhone_Define
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     电话号码查询导出
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       导出函数定义
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	XCHAR tszProvincer[64];     //省/自治区/直辖市
	XCHAR tszCity[64];          //市/区     
	__int64x nPhoneNumber;      //电话号码
	int nAreaCode;              //区号
}XENGINE_PHONEINFO;
//////////////////////////////////////////////////////////////////////////
//                       导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG PluginCore_GetLastError();
/*********************************************************************************
*                                导出函数定义                                    *
*********************************************************************************/
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
extern "C" bool PluginCore_Init(XPVOID lParam = NULL);
/********************************************************************
函数名称：PluginCore_UnInit
函数功能：卸载插件
返回值
  类型：无
  意思：
备注：
*********************************************************************/
extern "C" void PluginCore_UnInit();
/********************************************************************
函数名称：PluginCore_Call
函数功能：调用插件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool PluginCore_Call(XCHAR * **pppHDRList, int nListCount, int* pInt_HTTPCode, XCHAR * ptszMsgBuffer, int* pInt_MsgLen, LPCXSTR lpszMsgBuffer = NULL, int nMsgLen = 0);