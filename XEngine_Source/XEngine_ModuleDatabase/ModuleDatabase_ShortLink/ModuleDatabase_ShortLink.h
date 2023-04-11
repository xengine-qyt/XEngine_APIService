﻿#pragma once
/********************************************************************
//    Created:     2023/04/11  10:36:06
//    File Name:   D:\XEngine_APIService\XEngine_Source\XEngine_ModuleDatabase\ModuleDatabase_ShortLink\ModuleDatabase_ShortLink.h
//    File Path:   D:\XEngine_APIService\XEngine_Source\XEngine_ModuleDatabase\ModuleDatabase_ShortLink
//    File Base:   ModuleDatabase_ShortLink
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     短连接数据库管理器
//    History:
*********************************************************************/


class CModuleDatabase_ShortLink
{
public:
	CModuleDatabase_ShortLink();
	~CModuleDatabase_ShortLink();
public:
	XBOOL ModuleDatabase_ShortLink_Init(DATABASE_MYSQL_CONNECTINFO* pSt_DBConnector);
	XBOOL ModuleDatabase_ShortLink_Destory();
	XBOOL ModuleDatabase_ShortLink_Insert(XENGINE_SHORTLINK* pSt_SLinkInfo);
	XBOOL ModuleDatabase_ShortLink_Query(XENGINE_SHORTLINK* pSt_SLinkInfo);
	XBOOL ModuleDatabase_ShortLink_Delete(XENGINE_SHORTLINK* pSt_SLinkInfo);
protected:
private:
	XNETHANDLE xhDBSQL;
};