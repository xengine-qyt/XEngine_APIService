﻿#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIClient")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Error.h>

//需要优先配置XEngine
//WINDOWS支持VS2022 x64 debug 编译调试
//linux::g++ -std=c++17 -Wall -g APPClient_ImageExample.cpp -o APPClient_ImageExample.exe -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIClient
//macos::g++ -std=c++17 -Wall -g APPClient_ImageExample.cpp -o APPClient_ImageExample.exe -lXEngine_BaseLib -lNetHelp_APIClient


int test_query()
{
	int nCode = 0;
	XCHAR* ptszFileBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	if (NULL == ptszFileBuffer)
	{
		return -1;
	}
	memset(ptszFileBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);

	LPCXSTR lpszAPIUrl = _X("http://127.0.0.1:5501/api?function=image&params1=0");

	FILE* pSt_File = _xtfopen(_X("D:\\XEngine_APIService\\XEngine_APPClient\\x64\\Debug\\1.png"), "rb");
	int nRet = fread(ptszFileBuffer, 1, XENGINE_MEMORY_SIZE_MAX, pSt_File);

	XCHAR* ptszMsgBuffer = NULL;
	if (!APIClient_Http_Request(_X("POST"), lpszAPIUrl, ptszFileBuffer, &nCode, &ptszMsgBuffer, &nRet))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	printf("接受到数据,大小:%d,内容:%s\n", nRet, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	free(ptszFileBuffer);
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	test_query();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}