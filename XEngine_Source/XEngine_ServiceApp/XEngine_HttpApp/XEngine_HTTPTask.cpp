﻿#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/01/20  14:44:17
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_HttpApp\XEngine_HTTPTask.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_HttpApp
//    File Base:   XEngine_HTTPTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP任务处理代码
//    History:
*********************************************************************/
XHTHREAD CALLBACK HTTPTask_TastPost_Thread(XPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!HttpProtocol_Server_EventWaitEx(xhHTTPPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		HttpProtocol_Server_GetPoolEx(xhHTTPPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
		//先循环客户端
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                                    //客户端发送的数据大小,不包括头
				int nHDRCount = 0;
				XCHAR* ptszMsgBuffer = NULL;                         //客户端发送的数据
				XCHAR** pptszListHdr;
				RFCCOMPONENTS_HTTP_REQPARAM st_HTTPReqparam;        //客户端的请求参数

				memset(&st_HTTPReqparam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
				//得到一个指定客户端的完整数据包
				if (HttpProtocol_Server_GetMemoryEx(xhHTTPPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPReqparam, &pptszListHdr, &nHDRCount))
				{
					//在另外一个函数里面处理数据
					HTTPTask_TastPost_Handle(&st_HTTPReqparam, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &pptszListHdr, nHDRCount);
					//释放内存
					BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
					BaseLib_OperatorMemory_Free((XPPPMEM)&pptszListHdr, nHDRCount);
				}
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
bool HTTPTask_TastPost_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszRVBuffer, int nRVLen, XCHAR*** ppptszHDRList, int nHDRCount)
{
	int nMsgLen = 4096;
	LPCXSTR lpszMethodPost = _X("POST");
	LPCXSTR lpszMethodGet = _X("GET");
	XCHAR tszMsgBuffer[4096];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;    //发送给客户端的参数

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭

	XCHAR** pptszList;
	XCHAR tszUrlName[128];
	int nListCount = 0;

	memset(tszUrlName, '\0', sizeof(tszUrlName));
	//得到URL参数个数
	HttpProtocol_ServerHelp_GetParament(pSt_HTTPParam->tszHttpUri, &pptszList, &nListCount, tszUrlName);
	if (nListCount < 1)
	{
		//是不是代理转发
		if (HTTPTask_TaskPost_SLProxy(lpszClientAddr, pSt_HTTPParam->tszHttpUri, ppptszHDRList, nHDRCount))
		{
			return true;
		}
		st_HDRParam.nHttpCode = 404;
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}
	XCHAR tszKey[MAX_PATH];
	XCHAR tszValue[MAX_PATH];
	LPCXSTR lpszFuncName = _X("api");
	LPCXSTR lpszParamFuncKey = _X("function");
	LPCXSTR lpszParamName = _X("params1");
	//get
	LPCXSTR lpszParamIDCard = _X("id");
	LPCXSTR lpszParamBank = _X("bank");
	LPCXSTR lpszParamTranslation = _X("translation");
	LPCXSTR lpszParamLocker = _X("lock");
	LPCXSTR lpszParamReload = _X("reload");
	//post
	LPCXSTR lpszParamP2PClient = _X("p2p");
	LPCXSTR lpszParamZIPCode = _X("zipcode");
	LPCXSTR lpszParamXLog = _X("log");
	LPCXSTR lpszParamQRCode = _X("qrcode");
	LPCXSTR lpszParamSocket = _X("socket");
	LPCXSTR lpszParamDTest = _X("dtest");
	LPCXSTR lpszParamShortLink = _X("slink");
	LPCXSTR lpszParamWordFilter = _X("wordfilter");
	LPCXSTR lpszParamBack = _X("back");
	LPCXSTR lpszParamImage = _X("image");
	LPCXSTR lpszParamDeamon = _X("deamon");

	memset(tszKey, '\0', MAX_PATH);
	memset(tszValue, '\0', MAX_PATH);

	if (0 != _tcsxnicmp(lpszFuncName, tszUrlName, _tcsxlen(lpszFuncName)))
	{
		st_HDRParam.nHttpCode = 404;
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}
	//获得函数名
	BaseLib_OperatorString_GetKeyValue(pptszList[0], "=", tszKey, tszValue);
	if (0 != _tcsxnicmp(lpszParamFuncKey, tszKey, _tcsxlen(lpszParamFuncKey)))
	{
		st_HDRParam.nHttpCode = 404;
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}
	//首先处理插件
	int nPluginType = 0;
	if (ModulePlugin_Loader_Find(tszValue, &nPluginType))
	{
		XEngine_PluginTask_Handle(tszValue, lpszClientAddr, lpszRVBuffer, nRVLen, &pptszList, nListCount, nPluginType);
		return true;
	}
	if (0 == _tcsxnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPost)))
	{
		if (0 == _tcsxnicmp(lpszParamP2PClient, tszValue, _tcsxlen(lpszParamP2PClient)))
		{
			//是不是P2P
			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsxnicmp(lpszParamName, tszKey, _tcsxlen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return false;
			}
			HTTPTask_TastPost_P2PClient(lpszClientAddr, lpszRVBuffer, nRVLen, _ttxoi(tszValue));
		}
		else if (0 == _tcsxnicmp(lpszParamZIPCode, tszValue, _tcsxlen(lpszParamZIPCode)))
		{
			//邮政信息:http://app.xyry.org:5501/api?function=zipcode&params1=0
			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsxnicmp(lpszParamName, tszKey, _tcsxlen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return false;
			}
			HTTPTask_TastPost_PostCode(lpszClientAddr, lpszRVBuffer, nRVLen, _ttxoi(tszValue));
		}
		else if (0 == _tcsxnicmp(lpszParamXLog, tszValue, _tcsxlen(lpszParamXLog)))
		{
			//日志信息:http://app.xyry.org:5501/api?function=log&params1=0
			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsxnicmp(lpszParamName, tszKey, _tcsxlen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return false;
			}
			HTTPTask_TastPost_LogInfo(lpszClientAddr, lpszRVBuffer, nRVLen, _ttxoi(tszValue));
		}
		else if (0 == _tcsxnicmp(lpszParamQRCode, tszValue, _tcsxlen(lpszParamQRCode)))
		{
			//二维码生成:http://app.xyry.org:5501/api?function=qrcode&params1=0 或者 1
			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsxnicmp(lpszParamName, tszKey, _tcsxlen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return false;
			}
			HTTPTask_TaskPost_QRCode(lpszClientAddr, lpszRVBuffer, nRVLen, _ttxoi(tszValue));
		}
		else if (0 == _tcsxnicmp(lpszParamSocket, tszValue, _tcsxlen(lpszParamSocket)))
		{
			//网络测试:http://app.xyry.org:5501/api?function=socket&params1=0 或者 1
			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsxnicmp(lpszParamName, tszKey, _tcsxlen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return false;
			}
			HTTPTask_TastPost_SocketTest(lpszClientAddr, lpszRVBuffer, nRVLen, _ttxoi(tszValue));
		}
		else if (0 == _tcsxnicmp(lpszParamDTest, tszValue, _tcsxlen(lpszParamDTest)))
		{
			//数据测试:http://app.xyry.org:5501/api?function=dtest&params1=0 或者 1
			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsxnicmp(lpszParamName, tszKey, _tcsxlen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return false;
			}
			HTTPTask_TastPost_DTest(lpszClientAddr, lpszRVBuffer, nRVLen, _ttxoi(tszValue));
		}
		else if (0 == _tcsxnicmp(lpszParamShortLink, tszValue, _tcsxlen(lpszParamShortLink)))
		{
			//短连接:http://app.xyry.org:5501/api?function=slink&params1=0 
			XCHAR tszType[64];
			memset(tszType, '\0', sizeof(tszType));

			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszType);
			HTTPTask_TaskPost_ShortLink(lpszClientAddr, lpszRVBuffer, nRVLen, _ttxoi(tszType));
		}
		else if (0 == _tcsxnicmp(lpszParamWordFilter, tszValue, _tcsxlen(lpszParamWordFilter)))
		{
			//敏感词:http://app.xyry.org:5501/api?function=wordfilter&params1=0 
			XCHAR tszType[64];
			memset(tszType, '\0', sizeof(tszType));

			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszType);
			HTTPTask_TastPost_WordFilter(lpszClientAddr, lpszRVBuffer, nRVLen, _ttxoi(tszType));
		}
		else if (0 == _tcsxnicmp(lpszParamBack, tszValue, _tcsxlen(lpszParamBack)))
		{
			//后台管理接口:http://app.xyry.org:5501/api?function=back&params1=0
			XCHAR tszType[64];
			memset(tszType, '\0', sizeof(tszType));

			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszType);
			HTTPTask_TaskPost_BackService(lpszClientAddr, lpszRVBuffer, nRVLen, _ttxoi(tszType));
		}
		else if (0 == _tcsxnicmp(lpszParamImage, tszValue, _tcsxlen(lpszParamImage)))
		{
			//图像处理接口:http://app.xyry.org:5501/api?function=image&params1=0
			HTTPTask_TaskPost_Image(lpszClientAddr, lpszRVBuffer, nRVLen, &pptszList, nListCount);
		}
		else if (0 == _tcsxnicmp(lpszParamDeamon, tszValue, _tcsxlen(lpszParamDeamon)))
		{
			//守护进程接口:http://app.xyry.org:5501/api?function=deamon&params1=0
			HTTPTask_TaskPost_Deamon(lpszClientAddr, lpszRVBuffer, nRVLen);
			}
		else
		{
			st_HDRParam.nHttpCode = 404;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的请求不支持:%s，内容:\r\n%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri, lpszRVBuffer);
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodGet)))
	{
		if (0 == _tcsxnicmp(lpszParamReload, tszValue, _tcsxlen(lpszParamReload)))
		{
			//是不是配置重载
			memset(tszKey, '\0', sizeof(tszKey));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			HTTPTask_TaskGet_Reload(lpszClientAddr, tszValue);
		}
		else if (0 == _tcsxnicmp(lpszParamIDCard, tszValue, _tcsxlen(lpszParamIDCard)))
		{
			//是不是身份证查询
			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsxnicmp(lpszParamName, tszKey, _tcsxlen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return false;
			}
			HTTPTask_TaskGet_IDCard(lpszClientAddr, tszValue);
		}
		else if (0 == _tcsxnicmp(lpszParamBank, tszValue, _tcsxlen(lpszParamBank)))
		{
			//是不是银行卡信息
			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsxnicmp(lpszParamName, tszKey, _tcsxlen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return false;
			}
			HTTPTask_TaskGet_BankInfo(lpszClientAddr, tszValue);
		}
		else if (0 == _tcsxnicmp(lpszParamTranslation, tszValue, _tcsxlen(lpszParamTranslation)))
		{
			//是不是翻译
			XCHAR tszMSGBuffer[2048] = {};
			XCHAR tszSrcBuffer[64] = {};
			XCHAR tszDstBuffer[64] = {};

			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszMSGBuffer);
			BaseLib_OperatorString_GetKeyValue(pptszList[2], "=", tszKey, tszSrcBuffer);
			BaseLib_OperatorString_GetKeyValue(pptszList[3], "=", tszKey, tszDstBuffer);
			HTTPTask_TaskGet_Translation(lpszClientAddr, tszMSGBuffer, tszSrcBuffer, tszDstBuffer);
		}
		else if (0 == _tcsxnicmp(lpszParamLocker, tszValue, _tcsxlen(lpszParamLocker)))
		{
			//是不是分布式锁
			XCHAR tszLockToken[128];
			XCHAR tszLockType[64];
			
			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszLockToken, '\0', sizeof(tszLockToken));
			memset(tszLockType, '\0', sizeof(tszLockType));

			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszLockToken);
			BaseLib_OperatorString_GetKeyValue(pptszList[2], "=", tszKey, tszLockType);
			HTTPTask_TaskGet_Locker(lpszClientAddr, _ttxoll(tszLockToken), (ENUM_XENGINE_APISERVICE_LOCKER_TYPE)_ttxoi(tszLockType));
		}
		else
		{
			st_HDRParam.nHttpCode = 404;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送的请求不支持:%s，内容:\r\n%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri, lpszRVBuffer);
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,协议错误"), lpszClientAddr);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
	return true;
}