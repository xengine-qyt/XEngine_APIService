﻿#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"jsoncpp")
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIClient")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <json/json.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>

//需要优先配置XEngine
//WINDOWS支持VS2022 x64 debug 编译调试
//linux::g++ -std=c++17 -Wall -g APPClient_ImageExample.cpp -o APPClient_ImageExample.exe -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIClient
//macos::g++ -std=c++17 -Wall -g APPClient_ImageExample.cpp -o APPClient_ImageExample.exe -lXEngine_BaseLib -lNetHelp_APIClient


bool APPClient_ImageExample_GetAttr(LPCXSTR lpszMsgBuffer, int nMsgLen, int* pInt_Width, int* pInt_Height)
{
	int nCode = 0;
	LPCXSTR lpszAPIUrl = _X("http://127.0.0.1:5501/api?function=image&params1=0");

	XCHAR* ptszMsgBuffer = NULL;
	if (!APIClient_Http_Request(_X("POST"), lpszAPIUrl, lpszMsgBuffer, &nCode, &ptszMsgBuffer, &nMsgLen))
	{
		printf("发送投递失败！\n");
		return 0;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(ptszMsgBuffer, ptszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		printf("json parse failed\n");
		return false;
	}
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	Json::Value st_JsonBase = st_JsonRoot["st_BaseInfo"];
	*pInt_Width = st_JsonBase["nWidth"].asInt();
	*pInt_Height = st_JsonBase["nHeigth"].asInt();
	return 0;
}
int test_imgzoom()
{
	LPCXSTR lpszFileDir = _X("D:\\Image\\*.png");

	int nListCount = 0;
	XCHAR** pptszListFile;

	SystemApi_File_EnumFile(lpszFileDir, &pptszListFile, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		int nCode = 0;
		int nWidth = 0;
		int nHeight = 0;
		XCHAR* ptszFileBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
		if (NULL == ptszFileBuffer)
		{
			return -1;
		}
		memset(ptszFileBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);

		FILE* pSt_File = _xtfopen(pptszListFile[i], _X("rb"));
		int nRet = (int)fread(ptszFileBuffer, 1, XENGINE_MEMORY_SIZE_MAX, pSt_File);
		fclose(pSt_File);
		APPClient_ImageExample_GetAttr(ptszFileBuffer, nRet, &nWidth, &nHeight);

		XCHAR tszAPIUrl[MAX_PATH];
		memset(tszAPIUrl, '\0', sizeof(tszAPIUrl));

		_xstprintf(tszAPIUrl, _X("http://127.0.0.1:5501/api?function=image&type=1&ext=png&width=%d&height=%d"), nWidth / 2, nHeight / 2);

		XCHAR* ptszMsgBuffer = NULL;
		if (!APIClient_Http_Request(_X("POST"), tszAPIUrl, ptszFileBuffer, &nCode, &ptszMsgBuffer, &nRet))
		{
			printf("发送投递失败！\n");
			return 0;
		}
		free(ptszFileBuffer);
		
		pSt_File = _xtfopen(pptszListFile[i], _X("wb"));
		fwrite(ptszMsgBuffer, 1, nRet, pSt_File);
		fclose(pSt_File);

		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}

	
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	test_imgzoom();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}