﻿#include "../XEngine_Hdr.h"

bool HTTPTask_TaskPost_BackService(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nType)
{
	int nSDLen = 0;
	int nRVLen = 0;
	int nBSType = 0;
	XCHAR* ptszRVBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XCHAR* ptszSDBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XCHAR tszSrcBuffer[MAX_PATH];
	XCHAR tszDstBuffer[MAX_PATH];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;    //发送给客户端的参数

	memset(ptszRVBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	memset(ptszSDBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	
	memset(tszSrcBuffer, '\0', sizeof(tszSrcBuffer));
	memset(tszDstBuffer, '\0', sizeof(tszDstBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭

	if (st_ServiceConfig.st_XVerifcation.bBackService)
	{
		XCHAR tszUserName[MAX_PATH];
		XCHAR tszUserPass[MAX_PATH];

		memset(tszUserName, '\0', sizeof(tszUserName));
		memset(tszUserPass, '\0', sizeof(tszUserPass));

		ModuleProtocol_Parse_Verifcation(lpszMsgBuffer, nMsgLen, tszUserName, tszUserPass);

		if (0 != _tcsxnicmp(st_ServiceConfig.st_XVerifcation.tszUserName, tszUserName, _tcsxlen(st_ServiceConfig.st_XVerifcation.tszUserName)))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求后台协议失败,用户验证失败,用户名错误,提供的用户名:%s"), lpszClientAddr, tszUserName);
			return false;
		}
		if (0 != _tcsxnicmp(st_ServiceConfig.st_XVerifcation.tszUserPass, tszUserPass, _tcsxlen(st_ServiceConfig.st_XVerifcation.tszUserPass)))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求后台协议失败,解析协议失败,错误码:%lX"), lpszClientAddr, tszUserPass);
			return false;
		}
	}
	if (!ModuleProtocol_Parse_BackService(lpszMsgBuffer, nMsgLen, tszSrcBuffer, tszDstBuffer, &nBSType))
	{
		st_HDRParam.nHttpCode = 400;
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求后台协议失败,解析协议失败,错误码:%lX"), lpszClientAddr, ModuleProtocol_GetLastError());
		return false;
	}
	//执行任务
	switch (nType)
	{
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DOWNFILE:
	{
		XHANDLE xhTask = APIClient_File_Create(tszSrcBuffer, tszDstBuffer);
		if (NULL == xhTask)
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,下载任务处理失败,请求下载地址:%s,下载路径:%s,错误码:%lX"), lpszClientAddr, tszSrcBuffer, tszDstBuffer, APIClient_GetLastError());
			return false;
		}
		while (true)
		{
			NETHELP_FILEINFO st_TaskInfo;
			memset(&st_TaskInfo, '\0', sizeof(NETHELP_FILEINFO));

			if (APIClient_File_Query(xhTask, &st_TaskInfo))
			{
				if ((ENUM_NETHELP_APICLIENT_FILE_STATUS_INIT != st_TaskInfo.en_DownStatus) || (ENUM_NETHELP_APICLIENT_FILE_STATUS_DOWNLOADDING != st_TaskInfo.en_DownStatus))
				{
					break;
				}
			}
		}
		APIClient_File_Delete(xhTask);
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:下载任务处理成功,下载地址:%s,保存地址:%s"),lpszClientAddr, tszSrcBuffer, tszDstBuffer);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEFILE:
		if (-1 == _xtremove(tszSrcBuffer))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s:删除文件:%s,任务处理失败,错误码:%d"), lpszClientAddr, tszSrcBuffer, errno);
			return false;
		}
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:删除文件处理成功,删除的文件:%s"), lpszClientAddr, tszSrcBuffer);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_DELETEDIR:
		if (!SystemApi_File_DeleteMutilFolder(tszSrcBuffer))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s:删除文件夹:%s 任务处理失败,错误码:%lX"), lpszClientAddr, tszSrcBuffer, SystemApi_GetLastError());
			return false;
		}
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:删除文件夹处理成功,删除的文件夹:%s"), lpszClientAddr, tszSrcBuffer);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_UPFILE:
	{
		XHANDLE xhTask = APIClient_File_Create(tszSrcBuffer, tszDstBuffer, false);
		if (NULL == xhTask)
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s:FTP上传任务处理失败,上传的文件:%s,上传的地址:%s,错误码:%lX"), lpszClientAddr, tszSrcBuffer, tszDstBuffer, APIClient_GetLastError());
			return false;
		}
		while (true)
		{
			NETHELP_FILEINFO st_TaskInfo;
			memset(&st_TaskInfo, '\0', sizeof(NETHELP_FILEINFO));

			if (APIClient_File_Query(xhTask, &st_TaskInfo))
			{
				if (ENUM_NETHELP_APICLIENT_FILE_STATUS_DOWNLOADDING != st_TaskInfo.en_DownStatus)
				{
					break;
				}
			}
		}
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		APIClient_File_Delete(xhTask);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:上传文件处理成功,上传的文件:%s,上传的地址:%s"), lpszClientAddr, tszSrcBuffer, tszDstBuffer);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_GETLIST:
	{
		int nListCount = 0;
		XCHAR** ppszFileList;

		if (!SystemApi_File_EnumFile(tszSrcBuffer, &ppszFileList, &nListCount))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s:请求文件列表:%s 失败,错误码:%lX"), lpszClientAddr, tszSrcBuffer, SystemApi_GetLastError());
			return false;
		}
		ModuleProtocol_Packet_ListFile(ptszRVBuffer, &nRVLen, &ppszFileList, nListCount);
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam, ptszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppszFileList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:获取文件列表成功,回复个数:%d"), lpszClientAddr, nListCount);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_EXEC:
	{
		XLONG dwProcessID = 0;
		if (!SystemApi_Process_CreateProcess(&dwProcessID, tszSrcBuffer, NULL, nBSType))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s:请求创建进程:%s 失败,错误码:%lX"), lpszClientAddr, tszSrcBuffer, SystemApi_GetLastError());
			return false;
		}
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:请求创建进程成功,进程:%s,PID:%d"), lpszClientAddr, tszSrcBuffer, dwProcessID);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_STOPPROCESS:
	{
		if (!SystemApi_Process_Stop(NULL, nBSType))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s:请求停止进程ID:%d 失败,错误码:%lX"), lpszClientAddr, nBSType, SystemApi_GetLastError());
			return false;
		}
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:请求停止进程成功,进程ID:%ld"), nBSType);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_SHUTDOWN:
	{
		if (!SystemApi_System_Shutdown(nBSType))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s:请求关机失败,错误码:%lX"), lpszClientAddr, SystemApi_GetLastError());
			return false;
		}
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:请求关闭系统成功,关闭方式:%ld"), lpszClientAddr, nBSType);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_ECMD:
		if (-1 == system(tszSrcBuffer))
		{
			st_HDRParam.nHttpCode = 400;
			HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s:请求执行命令:%s 失败,错误码:%lX"), lpszClientAddr, tszSrcBuffer, SystemApi_GetLastError());
			return false;
		}
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:请求执行命令成功,执行的命令:%s"), lpszClientAddr, tszSrcBuffer);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_REPORT:
	{
		if (0 == nBSType)
		{
			ModuleProtocol_Packet_HardWare(ptszRVBuffer, &nRVLen);
		}
		else if (1 == nBSType)
		{
			ModuleProtocol_Packet_SoftWare(ptszRVBuffer, &nRVLen);
		}
		else if (2 == nBSType)
		{
			int nACount = 0;
			int nVCount = 0;
			AVHELP_DEVICEINFO** ppSt_AudioList;
			AVHELP_DEVICEINFO** ppSt_VideoList;

			AVHelp_Device_EnumDevice(&ppSt_AudioList, &ppSt_VideoList, &nACount, &nVCount);
			ModuleProtocol_Packet_EnumDevice(ptszRVBuffer, &nRVLen, &ppSt_AudioList, &ppSt_VideoList, nACount, nVCount);
			BaseLib_OperatorMemory_Free((void***)&ppSt_AudioList, nACount);
			BaseLib_OperatorMemory_Free((void***)&ppSt_VideoList, nVCount);
		}
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam, ptszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s:请求上报信息成功,上报类型:%d"), lpszClientAddr, nBSType);
	}
	break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_BS_NOTHINGTODO:
		break;
	default:
		st_HDRParam.nHttpCode = 400;
		HttpProtocol_Server_SendMsgEx(xhHTTPPacket, ptszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s:请求的操作码不支持,操作码:%d"), lpszClientAddr, nType);
		return false;
	}
	free(ptszRVBuffer);
	free(ptszSDBuffer);
	ptszSDBuffer = NULL;
	ptszRVBuffer = NULL;
	return true;
}