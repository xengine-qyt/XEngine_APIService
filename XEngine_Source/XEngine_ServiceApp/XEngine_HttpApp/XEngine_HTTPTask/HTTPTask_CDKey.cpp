﻿#include "../XEngine_Hdr.h"

BOOL XEngine_HTTPTask_CDKey(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int unOperatorCode, LPCTSTR lpszPass)
{
	int nSDLen = 4096;
	int nRVLen = 4096;
	TCHAR tszSDBuffer[4096];
	TCHAR tszRVBuffer[4096];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;    //发送给客户端的参数

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = TRUE; //收到回复后就关闭
	if (0 == unOperatorCode)
	{
		XENGINE_AUTHORIZE_LOCAL st_Authorize;
		memset(&st_Authorize, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));

		ModuleProtocol_Parse_CDKey(lpszMsgBuffer, nMsgLen, &st_Authorize);
		if (!Authorize_Local_WriteMemory(tszRVBuffer,&nRVLen, &st_Authorize))
		{
			st_HDRParam.nHttpCode = 500;
			RfcComponents_HttpServer_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("客户端:%s,请求创建CDKEY协议失败,服务器内部错误：%lX"), lpszClientAddr, Authorize_GetLastError());
			return FALSE;
		}
		if (NULL == lpszPass)
		{
			RfcComponents_HttpServer_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		}
		else
		{
			UCHAR tszCodecBuffer[4096];
			memset(tszCodecBuffer, '\0', sizeof(tszCodecBuffer));

			OPenSsl_XCrypto_Encoder(tszRVBuffer, &nMsgLen, tszCodecBuffer, lpszPass);
			RfcComponents_HttpServer_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam, (LPCTSTR)tszCodecBuffer, nMsgLen);
		}
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("客户端:%s,请求创建CDKEY成功,APP名:%s,APP版本:%s"), lpszClientAddr, st_Authorize.st_AuthAppInfo.tszAppName, st_Authorize.st_AuthAppInfo.tszAppVer);
	}
	else if (1 == unOperatorCode)
	{
	}
	else
	{
		st_HDRParam.nHttpCode = 404;
		RfcComponents_HttpServer_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("客户端:%s,请求了一条未知的子协议：%lX"), lpszClientAddr, unOperatorCode);
	}
	return TRUE;
}