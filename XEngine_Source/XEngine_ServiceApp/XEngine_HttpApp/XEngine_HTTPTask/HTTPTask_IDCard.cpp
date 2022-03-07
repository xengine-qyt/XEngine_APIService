﻿#include "../XEngine_Hdr.h"

BOOL XEngine_HTTPTask_IDCard(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nType)
{
	int nMsgLen = 4096;
	int nPktLen = 4096;
	TCHAR tszMsgBuffer[4096];
	TCHAR tszPktBuffer[4096];
	XENGINE_IDCARDINFO st_IDCardInfo;
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;    //发送给客户端的参数

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(tszPktBuffer, '\0', sizeof(tszPktBuffer));
	memset(&st_IDCardInfo, '\0', sizeof(XENGINE_IDCARDINFO));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = TRUE; //收到回复后就关闭

	if (!ModuleProtocol_Parse_IDCard(lpszMsgBuffer, &st_IDCardInfo))
	{
		st_HDRParam.nHttpCode = 406;
		RfcComponents_HttpServer_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求的身份证编号不正确:%s"), lpszClientAddr, lpszMsgBuffer);
		return FALSE;
	}
	//验证身份证是否正确
	if (!ModuleHelp_IDCard_CheckBirth(&st_IDCardInfo))
	{
		st_HDRParam.nHttpCode = 406;
		RfcComponents_HttpServer_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求的身份证验证生日失败:%s"), lpszClientAddr, lpszMsgBuffer);
		return FALSE;
	}
	if (!ModuleHelp_IDCard_CheckSum(&st_IDCardInfo))
	{
		st_HDRParam.nHttpCode = 406;
		RfcComponents_HttpServer_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求的身份证验证校验码失败:%s"), lpszClientAddr, lpszMsgBuffer);
		return FALSE;
	}
	RfcComponents_HttpServer_SendMsgEx(xhHTTPPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam, tszPktBuffer, nPktLen);
	XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求身份证号码验证与信息查询成功,号码:%s"), lpszClientAddr, lpszMsgBuffer);
	return TRUE;
}