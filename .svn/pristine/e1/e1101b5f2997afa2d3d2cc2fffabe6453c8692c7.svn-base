/** $Id$ $DateTime$
 *  @file  tcrPlayer.h
 *  @brief tcrPlayer模块的导出头文件
 *  @version 0.0.1
 *  @since 0.0.1
 *  @author engineer<engineer@tcay.com.cn>
 *  @date 2009-07-02    Created it
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

#ifndef __TC_PLAYER_H__
#define __TC_PLAYER_H__


#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC 
#endif

#ifdef NVD_PLAYER_EXPORTS
#define NVD_PLAYER_API EXTERNC __declspec(dllexport)	
#else	// else NVD_PLAYER_EXPORTS
#define NVD_PLAYER_API EXTERNC __declspec(dllimport)
#endif	// end of NVD_PLAYER_EXPORTS

/// 播放器句柄
typedef void* PlayerHandle;

/// 叠加信息句柄
typedef void* InfoHandle;

/// 播放对象
enum
{
	PLAYER_SOURCE_REAL		= 0,
	PLAYER_SOURCE_RECORD	= 1,
	PLAYER_SOURCE_FILE		= 2,
  PLAYER_SOURCE_PICTURE_EVENT		= 3,
  PLAYER_SOURCE_PICTURE_CLIP		= 4
};

/// 播放模式, 按位使用
enum 
{
	PLAYER_MODE_NORMAL		= 0x00,
	PLAYER_MODE_PERFRAME	= 0x01,
	PLAYER_MODE_IFRAME		= 0x02,
};

/// 返回值定义
enum
{
	PLAYER_ERR_NOIMPL		= -1,
	PLAYER_OK						= 0,

	PLAYER_ERR_BASE			= 0x00030000,
	PLAYER_ERR_PARA			= (PLAYER_ERR_BASE + 1),
	PLAYER_ERR_SYSTEM		= (PLAYER_ERR_BASE + 2),
	PLAYER_ERR_INVALID_HANDLE	=	(PLAYER_ERR_BASE + 3),
	PLAYER_ERR_FAILED		= (PLAYER_ERR_BASE + 4),
	PLAYER_ERR_FRAMEHDR	= (PLAYER_ERR_BASE + 5),
	PLAYER_ERR_FRAMEDATA =(PLAYER_ERR_BASE + 5),
	PLAYER_ERR_NO_SPACE =	(PLAYER_ERR_BASE + 6),
	PLAYER_ERR_NOTPLAY	=	(PLAYER_ERR_BASE + 7),
};

enum
{
  DECDATA_VIDEO = 1, 
  DECDATA_AUDIO,
};

struct NvdDecFrameInfo
{
  unsigned int sequence;				//帧序号,各通道独立,音视频独立
  unsigned int type;					  //数据类型:1视频,2音频
  unsigned int frameRate;			//编码时产生的图像帧率(若为音频此参数无意义)
  unsigned int timestamp;				//时标信息,单位秒;
  unsigned int tick;          //时间信息，单位为us


  unsigned char audioLen;   //音频长度
  unsigned char *pAudio;   ////音频指针

  //视频帧信息
  unsigned int videoType;		//是否I帧(对解码后数据无意义，恒为零，暂不用)
  unsigned int width;				//画面宽,单位像素(若为音频此参数无意义)
  unsigned int height;			//画面高,单位像素(若为音频此参数无意义)

  unsigned int strideY;      //视频亮度跨度
  unsigned int strideU;      //视频色度跨度
  unsigned int strideV;      //

  unsigned char* pY;      //视频Y分量
  unsigned char* pU;      //视频U分量
  unsigned char* pV;      //视频V分量

};


//
typedef void (CALLBACK *DecDataRoutine)( NvdDecFrameInfo *pInfo, void* pUser );

NVD_PLAYER_API int NvdPlayerCreate( PlayerHandle* phPlayer, int nPlaySource, int nBufferFrameScale );
NVD_PLAYER_API int NvdPlayerDestory( PlayerHandle hPlayer );
NVD_PLAYER_API int NvdPlayerInit( PlayerHandle hPlayer, HWND hPlayeWnd, int sizeVideoFifo, 
																unsigned int nChannels, unsigned int nSamplesPerSec, unsigned int nBitsPerSample, 
																void (*pfNotifyRoutine)( void*, int ), void* pRoutinePara );
NVD_PLAYER_API int NvdPlayerReset( PlayerHandle hPlayer, bool bIsForward );
NVD_PLAYER_API int NvdPlayerGetPosition( PlayerHandle hPlayer, __int64* ms );
NVD_PLAYER_API int NvdPlayerInputFrame( PlayerHandle hPlayer, void* pBuff, int nSize );
NVD_PLAYER_API int NvdPlayerPlay( PlayerHandle hPlayer, int nPlayMode );
NVD_PLAYER_API int NvdPlayerPause( PlayerHandle hPlayer );
NVD_PLAYER_API int NvdPlayerStop( PlayerHandle hPlayer );
NVD_PLAYER_API int NvdPlayerSetRate( PlayerHandle hPlayer, int nRate, int nScale );
NVD_PLAYER_API int NvdPlayerGetRate( PlayerHandle hPlayer, int* pnRate, int* pnScale );
NVD_PLAYER_API int NvdPlayerSound( PlayerHandle hPlayer );
// 与Render相关的功能
NVD_PLAYER_API void NvdPlayerSetDrawCallback( PlayerHandle hPlayer, 
void (*pfDrawRoutine)( void*, HDC hDC, unsigned int nWidth, unsigned int nHeight ), void* pDrawPara );
NVD_PLAYER_API int NvdPlayerSavePicture( PlayerHandle hPlayer, const char* szFileName);
NVD_PLAYER_API int NvdPlayerSavePictureEx( PlayerHandle hPlayer, const char* szFileName, int nImgType=0/*0 bitmap,1 jpg*/ );
NVD_PLAYER_API int NvdPlayerRefresh( PlayerHandle hPlayer );
NVD_PLAYER_API void NvdPlayerShowDateTime( PlayerHandle hPlayer, bool bShow );
NVD_PLAYER_API void NvdPlayerSetDateTimeColor( PlayerHandle hPlayer, unsigned int rgbColor );
NVD_PLAYER_API void NvdPlayerSetDateTimeCoordinate( PlayerHandle hPlayer, unsigned int nX, unsigned int nY );
NVD_PLAYER_API int NvdPlayerAddExtraInfo( PlayerHandle hPlayer, InfoHandle* phInfo, const char* szText,
																				unsigned int rgbColor, unsigned int nX, unsigned int nY );
NVD_PLAYER_API void NvdPlayerDelExtraInfo( PlayerHandle hPlayer, InfoHandle hInfo );
NVD_PLAYER_API void NvdPlayerSetExtraInfoColor( PlayerHandle hPlayer, InfoHandle hInfo, unsigned int rgbColor );
NVD_PLAYER_API void NvdPlayerSetExtraInfoCoordinate( PlayerHandle hPlayer, InfoHandle hInfo, unsigned int nX, unsigned int nY );
NVD_PLAYER_API void NvdPlayerShowIAInfo( PlayerHandle hPlayer, BOOL bShow ); //设置是否显示智能信息
NVD_PLAYER_API BOOL NvdPlayerIAInfoShow( PlayerHandle hPlayer );             //是否显示智能信息
NVD_PLAYER_API bool NvdPlayerHasAiInfo(PlayerHandle hPlayer);
NVD_PLAYER_API void NvdSetPlayerShowAiLang(PlayerHandle hPlayer, int nLanguage);//设置智能叠加信息的显示语言

NVD_PLAYER_API void NvdPlayerSetDecCallback( PlayerHandle hPlayer, DecDataRoutine pCallback, void* pUser );

//到指定的时间开始播放，只对远程回放和本地文件播放有效
NVD_PLAYER_API bool NvdPlayerWaitForTime( PlayerHandle hPlayer, __int64 ms, int timeout );
NVD_PLAYER_API int NvdPlayerSetFluence( PlayerHandle hPlayer,int frameChacNum );

#endif	// end of __TC_PLAYER_H__

