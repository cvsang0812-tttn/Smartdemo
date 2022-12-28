 /** $Id$ $DateTime$
 *  @file  NvdSdk.h
 *  @brief NvrSdk接口的声明文件
 *  @version 0.0.1
 *  @since 0.0.1
 *  @author engineer<engineer@tcay.com.cn>
 *  @date 2009-06-15    Created it
 */
/******************************************************************************
*@note
    Copyright 2014, BeiJing TCAY Corporation, Limited
                        ALL RIGHTS RESERVED
Permission is hereby granted to licensees of BeiJing TCAY, Inc. products
to use or abstract this computer program for the sole purpose of implementing
a product based on BeiJing TCAY, Inc. products. No other rights to
reproduce, use, or disseminate this computer program,whether in part or  in
whole, are granted. BeiJing TCAY, Inc. makes no representation or
warranties with respect to the performance of this computer program, and
specifically disclaims any responsibility for any damages, special or
consequential, connected with the use of this program.
For details, see http://www.tcay.com.cn/
******************************************************************************/

#ifndef USE_NVD_SDK_HEADER
#define USE_NVD_SDK_HEADER

#include <windows.h>
#include <winsock2.h>
#include "NvdPlayer.h"
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC 
#endif

#ifdef NVD_SDK_EXPORTS
#define NVD_SDK_API EXTERNC __declspec(dllexport)	
#else	// else BSRDVRSDK_EXPORTS
#define NVD_SDK_API EXTERNC __declspec(dllimport)
#endif	// end BSRDVRSDK_EXPORTS


/**********************************************************************************/
// 系统管理
NVD_SDK_API int NvdSdkInit();
NVD_SDK_API int NvdSdkDeInit();
NVD_SDK_API char *NvdSdkGetVersion();
NVD_SDK_API int NvdSdkSetRegCallback( unsigned short port, int (*pfRegRoutine)( void*, SmartRegClientHandle, SmartRegClientInfo ), void* pRegPara );
NVD_SDK_API int NvdCreate( const char* ip, unsigned short nCmdPort, unsigned short nDataPort, const char* szUserName, const char* szPasswd, NvdHandle* pDvr );
NVD_SDK_API int NvdCreateBySocket( SmartRegClientHandle regHandle, NvdHandle* pNvd, unsigned short nDataPort );
NVD_SDK_API int NvdDestory( NvdHandle hNvd );
NVD_SDK_API int NvdSetReconnect( NvdHandle hNvd, bool bIsReconn );
NVD_SDK_API int NvdSetNotifyWnd( NvdHandle hNvd, HWND hNotifyWnd, int nMsgId );
NVD_SDK_API int NvdLogin( NvdHandle hNvd, const char* szUserName, const char* szPasswd, int msTimeout );
NVD_SDK_API int NvdLogout( NvdHandle hNvd );
NVD_SDK_API bool NvdIsLogin( NvdHandle hNvd );
NVD_SDK_API int NvdSysVersion( NvdHandle hNvd, SystemVersionInfo* pVersion);
NVD_SDK_API int NvdSysHello( NvdHandle hNvd, HelloRes *res);

NVD_SDK_API int NvdRealOpen( NvdHandle hNvd, RealOpenPara* pPara, HWND hPlayWnd, HWND hNotifyWnd, unsigned int msgId, RealHandle* phReal );
NVD_SDK_API int NvdRealStart( RealHandle hReal );
NVD_SDK_API int NvdRealStop( RealHandle hReal );
NVD_SDK_API int NvdRealClose( RealHandle hReal );
NVD_SDK_API int NvdPlaySound( PlayHandle hPlay );
NVD_SDK_API int NvdStopSound( PlayHandle hPlay );
NVD_SDK_API int NvdRealSetDataCallback( RealHandle hReal, int (*pfDataRoutine)( void*, void*, int ), void* pPara );


//智能
NVD_SDK_API int NvdSmartRecogNotifyCallbackSet(NvdHandle hNvd, TCSmartRecogNotify callback, void *data);
NVD_SDK_API int NvdSmartAddNotifyCallbackSet(NvdHandle hNvd, TCSmartAddNotify callback, void *data);
NVD_SDK_API int NvdSmartDelNotifyCallbackSet(NvdHandle hNvd, TCSmartDelNotify callback, void *data);
NVD_SDK_API int NvdSmartDpuVersionGet(NvdHandle hNvd, DpuResQueryDPU *lpRecv);
NVD_SDK_API int NvdSmartRecogParamGet(NvdHandle hNvd, CmFaceSmartFrameInfo *lpRecv);
NVD_SDK_API int NvdSmartRecogParamSet(NvdHandle hNvd, CmFaceSmartFrameInfo *lpSend);
NVD_SDK_API int NvdSmartGetOnePhoto(NvdHandle hNvd, unsigned int id, FaceNetPhotoDescVector *lpRecv);
NVD_SDK_API int NvdSmartSyncOnePhoto(NvdHandle hNvd, struct FaceNetPhotoDescVector *faceInfo);
NVD_SDK_API int NvdSmartAddOnePhoto(NvdHandle hNvd, FaceNetPhotoDesc *photoDesc, FaceVector *photoVector);
NVD_SDK_API int NvdSmartDelOnePhoto(NvdHandle hNvd, unsigned int id);
NVD_SDK_API int NvdSmartModifyOnePhoto(NvdHandle hNvd, FacePhotoDesc  *desc);
NVD_SDK_API int NvdSmartDelAllFaceLibs(NvdHandle hNvd);
NVD_SDK_API int NvdSmartDelAllRecogResults(NvdHandle hNvd);
NVD_SDK_API int NvdSmartQueryFaceLib(NvdHandle hNvd, FaceQueryReq  *cond, int *count, FacePhotoDesc **faces);
NVD_SDK_API int NvdSmartQueryRecogResult(NvdHandle hNvd, FaceRecogSearchCond  *cond, void *result, unsigned int length);
NVD_SDK_API int NvdSmartGetSceneSnapByTime(NvdHandle hNvd, unsigned long long pts64, void *snap, unsigned int maxLength);
NVD_SDK_API int NvdSmartSetBatchAddPhotoSwitch(NvdHandle hNvd, unsigned int batchAddSwitch);
NVD_SDK_API int NvdSmartGetProvinceList(NvdHandle hNvd, char **pProvince, int *count);
NVD_SDK_API int NvdSmartGetCityList(NvdHandle hNvd, int *pId, char **pCity, int *count);
NVD_SDK_API int NvdSmartSmartBatchImportFaceLib(NvdHandle hNvd, char *csvPath, char *failItems, int *totalCount, int *successCount);
NVD_SDK_API int NvdSmartSmartBatchImportGetProgress(NvdHandle hNvd, int *totalCount, int *successCount, int *failCount);
NVD_SDK_API int NvdSmartConvertFaceDesc(NvdHandle hNvd, FacePhotoDesc *desc, FacePhotoDescStr *descStr);
NVD_SDK_API int NvdSmartGetAlarmLinkage(NvdHandle hNvd, SmartAlarmLinkageCfg *pParam);
NVD_SDK_API int NvdSmartSetAlarmLinkage(NvdHandle hNvd, SmartAlarmLinkageCfg *pParam);

#endif	// end of USE_NVD_SDK_HEADER
