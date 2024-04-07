﻿#include "../XEngine_Hdr.h"

bool HTTPTask_TastPost_Machine(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nType)
{
	int nSDLen = 0;
	int nRVLen = 0;
	XCHAR tszSDBuffer[4096];
	XCHAR tszRVBuffer[4096];
	XENGINE_MACHINEINFO st_MachineInfo = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};    //发送给客户端的参数

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭
	ModuleProtocol_Parse_Machine(lpszMsgBuffer, nMsgLen, &st_MachineInfo);

	if (!st_ServiceConfig.st_XSql.bEnable)
	{
		ModuleProtocol_Packet_Common(tszRVBuffer, &nRVLen, 501, _X("function is closed"));
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求的信息收集操作功能已经被服务器关闭,无法继续"), lpszClientAddr);
		return false;
	}
	if (0 == nType)
	{
		if (ModuleDatabase_Machine_Query(&st_MachineInfo))
		{
			st_MachineInfo.nTimeNumber++;
			ModuleDatabase_Machine_UPDate(&st_MachineInfo);
		}
		else
		{
			ModuleDatabase_Machine_Insert(&st_MachineInfo);
		}
		ModuleProtocol_Packet_Common(tszRVBuffer, &nRVLen);
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求插入一条信息收集数据成功,机器名:%s,,机器编码:%s"), lpszClientAddr, st_MachineInfo.tszMachineName, st_MachineInfo.tszMachineCode);
	}
	else if (1 == nType)
	{
		ModuleDatabase_Machine_Delete(&st_MachineInfo);
		ModuleProtocol_Packet_Common(tszRVBuffer, &nRVLen);
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求删除一条信息收集数据成功,机器名:%s,机器编码:%s"), lpszClientAddr, st_MachineInfo.tszMachineCode, st_MachineInfo.tszMachineCode);
	}
	return true;
}