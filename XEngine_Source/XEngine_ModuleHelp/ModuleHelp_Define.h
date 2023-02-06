﻿#pragma once
/********************************************************************
//    Created:     2022/03/04  13:37:09
//    File Name:   D:\XEngine_APIService\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Define.h
//    File Path:   D:\XEngine_APIService\XEngine_Source\XEngine_ModuleHelp
//    File Base:   ModuleHelp_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出帮助模块定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       导出回调函数
//////////////////////////////////////////////////////////////////////////
typedef void(CALLBACK* CALLBACK_APISERVICE_MODULE_HELP_P2PCLIENT)(XENGINE_P2XPPEER_PROTOCOL* pSt_P2PProtocol, LPVOID lParam);
//套接字测试回调:测试句柄，测试的服务器地址，测试的端口，当前测试次数，失败次数,成功次数,自定义参数
typedef void(CALLBACK* CALLBACK_APISERVICE_MODULE_HELP_SOCKETTEST)(XNETHANDLE xhToken, LPCSTR lpszAddr, int nPort, __int64x nNumber, __int64x nFailed, __int64x nSuccess, LPVOID lParam);
//////////////////////////////////////////////////////////////////////////
//                       导出数据结构
//////////////////////////////////////////////////////////////////////////
//计时器
typedef struct 
{
	__int64x dwUserTime;                                                  //用户存在时间
	__int64x dwPacketTime;                                                //包时间,拿到包后的存活时间
	__int64x dwKeepAlive;                                                 //保活计时器时间,经过网络时间
	unsigned short int usTTL : 1;                                         //路由数
}P2XP_TIMER_INFOMATION, * LPP2XP_TIMER_INFOMATION;
//一个节点信息
typedef struct 
{
	XENGINE_P2XPPEER_PROTOCOL st_PeerAddr;                                //P2P通信的时候使用的地址（客户方使用）
	P2XP_TIMER_INFOMATION st_PeerTimer;                                   //节点时间信息
	BOOL bIsLogin;
}XENGINE_P2XP_PEERINFO, * LPNETENGINE_P2XP_PEERINFO;
//重复连接测试结构体
typedef struct 
{
	CHAR tszAddr[64];                                                     //要测试的IP地址
	int nPort;                                                            //端口号码
	//单位毫秒
	int nConnectCount;                                                    //要测试的客户端个数
	int nConnectTest;                                                     //每个客户端测试的次数,-1表示不限制
	int nContWaitTime;                                                    //连接后等待多长时间关闭/处理一次休息时间，毫秒
	int nCloseWaitContTime;                                               //关闭后等待多长时间连接/套接字操作超时时间
}MODULEHELP_SOCKETTEST_RECONNECT, * LPMODULEHELP_SOCKETTEST_RECONNECT;
//数据包压力测试
typedef struct 
{
	MODULEHELP_SOCKETTEST_RECONNECT st_REConnect;
	int nSDLen;                                                           //发送数据包大小，如果ptszSDBuffer的值为NULL，那么这个值表示测试端允许发送的大小，否则表示缓冲区大小
	int nRVLen;                                                           //接收端大小,表示ptszRVBuffer缓冲区大小,ptszRVBuffer的值匹配接受数据才表示成功,如果ptszRVBuffer为NULL,不做数据验证只接受.
	CHAR tszSDBuffer[4096];                                               //如果你的服务器有特殊数据测试，请填写这个参数，否则测试端将自定义数据发送,内存需要由用户管理
	CHAR tszRVBuffer[4096];
}MODULEHELP_SOCKETTEST_DATAS, * LPMODULEHELP_SOCKETTEST_DATAS;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleHelp_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         导出的身份证帮助函数                         */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_IDCard_CheckBirth
函数功能：检查身份证号生日
 参数.一：pSt_IDInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要检查的身份结构
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_IDCard_CheckBirth(XENGINE_IDCARDINFO* pSt_IDInfo);
/********************************************************************
函数名称：ModuleHelp_IDCard_CheckBirth
函数功能：检查身份证的校验码是否正确
 参数.一：pSt_IDInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要检查的身份结构
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_IDCard_CheckSum(XENGINE_IDCARDINFO* pSt_IDInfo);
/************************************************************************/
/*                         导出的语言转换帮助函数                       */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_Language_ConvertZh
函数功能：中文语言转换函数
 参数.一：lpszJsonFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入转换的配置文件
 参数.一：lpszSourceStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要转换的字符串
 参数.一：ptszDestStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出转换后的字符串
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_Language_ConvertZh(LPCTSTR lpszJsonFile, LPCTSTR lpszSourceStr, TCHAR* ptszDestStr);
/********************************************************************
函数名称：ModuleHelp_Translation_Convert
函数功能：翻译类型转换
 参数.一：enTranslationType
  In/Out：In
  类型：枚举型
  可空：N
  意思：输入要转换的类型
 参数.二：ptszTranslationType
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出转换后的类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_Translation_Convert(ENUM_XENGINE_APISERVICE_TRANSLATION_TYPE enTranslationType, TCHAR* ptszTranslationType);
/************************************************************************/
/*                       节点管理器导出函数                             */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_P2PClient_Init
函数功能：初始化P2P客户端
 参数.一：nTimeout
  In/Out：In
  类型：整数型
  可空：N
  意思：输入超时时间,秒
 参数.二：fpCall_P2PClient
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：P2P客户端导出信息
 参数.三：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_Init(int nTimeout, CALLBACK_APISERVICE_MODULE_HELP_P2PCLIENT fpCall_P2PClient, LPVOID lParam = NULL);
/********************************************************************
函数名称：ModuleHelp_P2PClient_Destory
函数功能：销毁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_Destory();
/********************************************************************
函数名称：ModuleHelp_P2PClient_Add
函数功能：添加一个客户到一个节点信息
 参数.一：pSt_PeerInfo
  In/Out：In
  类型：结构体指针
  可空：N
  意思：要添加的节点信息
返回值
  类型：逻辑型
  意思：是否添加成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_Add(XENGINE_P2XP_PEERINFO * pSt_PeerInfo);
/********************************************************************
函数名称：ModuleHelp_P2PClient_Get
函数功能：获取P2P节点对应信息
 参数.一：pSt_P2PProtocol
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要查询的地址
 参数.二：pSt_PeerInfo
  In/Out：Out
  类型：数据结构指针
  可空：Y
  意思：如果为空将不导出对应的数据
返回值
  类型：逻辑型
  意思：是否查找成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_Get(XENGINE_P2XPPEER_PROTOCOL * pSt_P2PProtocol, XENGINE_P2XP_PEERINFO * pSt_PeerInfo = NULL);
/********************************************************************
函数名称：ModuleHelp_P2PClient_GetLan
函数功能：获取局域网IP地址列表
 参数.一：pSt_P2PProtocol
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要查找地址信息
 参数.二：pppSt_P2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：导出局域网IP地址列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出地址列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：参数二需要调用基础库的BaseLib_OperatorMemory_Free函数进行内存释放
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_GetLan(XENGINE_P2XPPEER_PROTOCOL * pSt_P2PProtocol, XENGINE_P2XPPEER_PROTOCOL * **pppSt_P2XPClient, int* pInt_ListCount);
/********************************************************************
函数名称：ModuleHelp_P2PClient_GetLList
函数功能：获取同一公网下所有局域网主IP列表
 参数.一：lpszPubAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要获取的IP地址
 参数.二：pppszP2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出内网主IP地址列表
 参数.三：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：参数二需要调用基础库的BaseLib_OperatorMemory_Free函数进行内存释放
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_GetLList(LPCSTR lpszPubAddr, CHAR * **pppszP2XPClient, int* pInt_ListCount);
/********************************************************************
函数名称：ModuleHelp_P2PClient_GetWList
函数功能：获取所有连接到的公网地址列表
 参数.一：pppszP2XPClient
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出地址列表
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_GetWList(CHAR * **pppszP2XPClient, int* pInt_ListCount);
/********************************************************************
函数名称：ModuleHelp_P2PClient_Delete
函数功能：删除一个指定的节点
 参数.一：pSt_P2PProtocol
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要删除的节点地址
返回值
  类型：逻辑型
  意思：是否成功删除
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_Delete(XENGINE_P2XPPEER_PROTOCOL * pSt_P2PProtocol);
/********************************************************************
函数名称：ModuleHelp_P2PClient_DelAll
函数功能：清空节点列表数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_DelAll();
/********************************************************************
函数名称：ModuleHelp_P2PClient_Heart
函数功能：触发一次心跳
 参数.一：pSt_P2PProtocol
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要操作的节点地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_P2PClient_Heart(XENGINE_P2XPPEER_PROTOCOL* pSt_P2PProtocol);
/************************************************************************/
/*                       分布式锁导出函数                               */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_Locker_Create
函数功能：创建锁
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出创建的锁句柄,并且会打开这个锁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_Locker_Create(XNETHANDLE* pxhToken);
/********************************************************************
函数名称：ModuleHelp_Locker_OPen
函数功能：打开一个锁
 参数.一：xhToken
  In/Out：In
  类型：锁句柄
  可空：N
  意思：输入要操作的锁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_Locker_OPen(XNETHANDLE xhToken);
/********************************************************************
函数名称：ModuleHelp_Locker_ReadLock
函数功能：读加锁
 参数.一：xhToken
  In/Out：In
  类型：锁句柄
  可空：N
  意思：输入要操作的锁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_Locker_ReadLock(XNETHANDLE xhToken);
/********************************************************************
函数名称：ModuleHelp_Locker_ReadUNLock
函数功能：读解锁
 参数.一：xhToken
  In/Out：In
  类型：锁句柄
  可空：N
  意思：输入要操作的锁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_Locker_ReadUNLock(XNETHANDLE xhToken);
/********************************************************************
函数名称：ModuleHelp_Locker_WriteLock
函数功能：写加锁
 参数.一：xhToken
  In/Out：In
  类型：锁句柄
  可空：N
  意思：输入要操作的锁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_Locker_WriteLock(XNETHANDLE xhToken);
/********************************************************************
函数名称：ModuleHelp_Locker_WriteUNLock
函数功能：写解锁
 参数.一：xhToken
  In/Out：In
  类型：锁句柄
  可空：N
  意思：输入要操作的锁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_Locker_WriteUNLock(XNETHANDLE xhToken);
/********************************************************************
函数名称：ModuleHelp_Locker_Close
函数功能：关闭锁
 参数.一：xhToken
  In/Out：In
  类型：锁句柄
  可空：N
  意思：输入要操作的锁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_Locker_Close(XNETHANDLE xhToken);
/************************************************************************/
/*                       二维码导出函数                                 */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_QRCode_QREncodecFile
函数功能：二维码生成器
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要生成的图片位置
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要编码的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_QRCode_QREncodecFile(LPCTSTR lpszFileName, LPCTSTR lpszMsgBuffer);
/********************************************************************
函数名称：ModuleHelp_QRCode_QREncodecMemory
函数功能：编码数据为二维码格式PNG图片
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要编码的数据
 参数.二：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出编码的数据
 参数.三：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出数据大小
  意思：输出数据大小
 参数.四：lpszFmt
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入文件格式.png
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_QRCode_QREncodecMemory(LPCTSTR lpszMsgBuffer, TCHAR* ptszMsgBuffer, int* pInt_MsgLen, LPCTSTR lpszFmt);
/********************************************************************
函数名称：ModuleHelp_QRCode_QRDecodecFile
函数功能：从文件解码二维码
 参数.一：lpszFileName
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的图片地址
 参数.二：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出识别到的内容
 参数.三：lpszDetectProto
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：学习模型地址,参考示例
 参数.四：lpszDetectModel
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：学习模型地址,参考示例
 参数.五：lpszSrProto
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：学习模型地址,参考示例
 参数.六：lpszSrModel
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：学习模型地址,参考示例
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_QRCode_QRDecodecFile(LPCTSTR lpszFileName, TCHAR* ptszMsgBuffer, LPCTSTR lpszDetectProto, LPCTSTR lpszDetectModel, LPCTSTR lpszSrProto, LPCTSTR lpszSrModel);
/********************************************************************
函数名称：ModuleHelp_QRCode_QRDecodecMemory
函数功能：从内存解析二维码
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的图片缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.三：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出识别到的内容
 参数.四：lpszDetectProto
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：学习模型地址,参考示例
 参数.五：lpszDetectModel
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：学习模型地址,参考示例
 参数.六：lpszSrProto
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：学习模型地址,参考示例
 参数.七：lpszSrModel
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：学习模型地址,参考示例
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_QRCode_QRDecodecMemory(LPCSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszMsgBuffer, LPCTSTR lpszDetectProto, LPCTSTR lpszDetectModel, LPCTSTR lpszSrProto, LPCTSTR lpszSrModel);
/************************************************************************/
/*                       网络测试导出函数                               */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_SocketTest_StartConnect
函数功能：测试连接
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：导出句柄
 参数.二：pSt_ReConnect
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要测试的属性
 参数.三：fpCall_ReConnect
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：测试回调信息
 参数.四：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：句柄型
  意思：成功返回连接的句柄,失败返回NULL
备注：链接测试函数，链接一次后就关闭
*********************************************************************/
extern "C" BOOL ModuleHelp_SocketTest_StartConnect(XNETHANDLE* pxhToken, MODULEHELP_SOCKETTEST_RECONNECT* pSt_ReConnect, CALLBACK_APISERVICE_MODULE_HELP_SOCKETTEST fpCall_ReConnect, LPVOID lParam = NULL);
/********************************************************************
函数名称：ModuleHelp_SocketTest_GetConnect
函数功能：获取是否在处理中
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入操作的句柄
 参数.二：pbRun
  In/Out：Out
  类型：逻辑型指针
  可空：N
  意思：输出是否在运行
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_SocketTest_GetConnect(XNETHANDLE xhToken, BOOL* pbRun);
/********************************************************************
函数名称：ModuleHelp_SocketTest_StopConnect
函数功能：停止短连接测试
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入测试的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_SocketTest_StopConnect(XNETHANDLE xhToken);
/********************************************************************
函数名称：ModuleHelp_SocketTest_StartDatas
函数功能：数据包测试函数
 参数.一：pxhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输出创建的句柄
 参数.二：pSt_SocketDatas
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：要测试的属性
 参数.三：fpCall_DataTest
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：测试回调信息
 参数.四：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_SocketTest_StartDatas(XNETHANDLE* pxhToken, MODULEHELP_SOCKETTEST_DATAS* pSt_SocketDatas, CALLBACK_APISERVICE_MODULE_HELP_SOCKETTEST fpCall_DataTest, BOOL bTCP = TRUE, LPVOID lParam = NULL);
/********************************************************************
函数名称：ModuleHelp_SocketTest_GetDatas
函数功能：获取是否在处理中
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入操作的句柄
 参数.二：pbRun
  In/Out：Out
  类型：逻辑型指针
  可空：N
  意思：输出是否在运行
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleHelp_SocketTest_GetDatas(XNETHANDLE xhToken, BOOL* pbRun);
/************************************************************************
函数名称：ModuleHelp_SocketTest_DatasStop
函数功能：停止大数据包测试
  参数.一：xhToken
   In/Out：In
   类型：句柄
   可空：N
   意思：输入测试的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
************************************************************************/
extern "C" BOOL ModuleHelp_SocketTest_StopDatas(XNETHANDLE xhToken);