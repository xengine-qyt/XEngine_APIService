﻿#pragma once
/********************************************************************
//    Created:     2022/02/28  10:16:35
//    File Name:   D:\XEngine_IPInfo\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_Error.h
//    File Path:   D:\XEngine_IPInfo\XEngine_Source\XEngine_ModuleDatabase
//    File Base:   ModuleDB_Error
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     数据库错误导出
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                        导出的IP数据库错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_APISERVICE_MODULE_DATABASE_IPINFO_PARAMENT 0xB0001           //初始化失败.参数错误
#define ERROR_APISERVICE_MODULE_DATABASE_IPINFO_NOTFOUND 0xB0002           //没有找到
//////////////////////////////////////////////////////////////////////////
//                        导出的身份证数据库错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_APISERVICE_MODULE_DATABASE_IDCARD_PARAMENT 0xB1001           //初始化失败.参数错误
#define ERROR_APISERVICE_MODULE_DATABASE_IDCARD_NOTFOUND 0xB1002           //没有找到
//////////////////////////////////////////////////////////////////////////
//                        导出的银行卡数据库错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_APISERVICE_MODULE_DATABASE_BANK_PARAMENT 0xB2001            //参数错误
#define ERROR_APISERVICE_MODULE_DATABASE_BANK_NOTFOUND 0xB2002            //没有找到
//////////////////////////////////////////////////////////////////////////
//                        行政邮编错误
//////////////////////////////////////////////////////////////////////////
#define ERROR_APISERVICE_MODULE_DATABASE_ZIPCODE_NOTFOUND 0xB3001         //没有找到
#define ERROR_APISERVICE_MODULE_DATABASE_ZIPCODE_PARAMENT 0xB3002         //参数错误
#define ERROR_APISERVICE_MODULE_DATABASE_ZIPCODE_NOTSET 0xB3003           //没有设置查询参数