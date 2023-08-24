﻿#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"jsoncpp")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIClient")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Error.h>

//需要优先配置XEngine
//WINDOWS支持VS2022 x64 debug 编译调试
//linux::g++ -std=c++17 -Wall -g APPClient_WordFilter.cpp -o APPClient_WordFilter.exe -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIClient
//macos::g++ -std=c++17 -Wall -g APPClient_WordFilter.cpp -o APPClient_WordFilter.exe -lXEngine_BaseLib -lNetHelp_APIClient

LPCXSTR lpszTableName = _T("xengine");

int test_insert()
{
	int nLen = 0;
	int nCode = 0;
	LPCXSTR lpszAPIUrl = _T("http://127.0.0.1:5501/api?function=wordfilter&params1=0");

	Json::Value st_JsonRoot;
	Json::StreamWriterBuilder st_JsonBuilder;
	st_JsonRoot["tszWordsFrom"] = "root";
	st_JsonRoot["tszWordsTo"] = "user";
	st_JsonRoot["nLevel"] = 1;

	st_JsonBuilder["emitUTF8"] = true;

	XCHAR* ptszMsgBuffer = NULL;
	if (!APIClient_Http_Request(_T("POST"), lpszAPIUrl, Json::writeString(st_JsonBuilder, st_JsonRoot).c_str(), &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	printf("接受到数据,大小:%d,内容:%s\n", nLen, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}
int test_query()
{
	int nLen = 0;
	int nCode = 0;
	LPCXSTR lpszAPIUrl = _T("http://127.0.0.1:5501/api?function=wordfilter&params1=2");

	Json::Value st_JsonRoot;
	st_JsonRoot["tszWordsFrom"] = "root";

	XCHAR* ptszMsgBuffer = NULL;
	if (!APIClient_Http_Request(_T("POST"), lpszAPIUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	printf("接受到数据,大小:%d,内容:%s\n", nLen, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}
int test_delete()
{
	int nLen = 0;
	int nCode = 0;
	LPCXSTR lpszAPIUrl = _T("http://127.0.0.1:5501/api?function=wordfilter&params1=1");

	Json::Value st_JsonRoot;
	st_JsonRoot["tszWordsFrom"] = "root";

	XCHAR* ptszMsgBuffer = NULL;
	if (!APIClient_Http_Request(_T("POST"), lpszAPIUrl, st_JsonRoot.toStyledString().c_str(), &nCode, &ptszMsgBuffer, &nLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	printf("接受到数据,大小:%d,内容:%s\n", nLen, ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	test_insert();
	test_query();
	test_delete();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}