/** $Id$ $DateTime$
*  @file  DvrSdkType.h
*  @brief dvr sdk 所使用的类型的声明文件
*  @version 0.0.1
*  @since 0.0.1
*  @author engineer<engineer@tcay.com.cn>
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

#ifndef USE_NVD_SDK_TYPE_HEADER
#define USE_NVD_SDK_TYPE_HEADER

#pragma warning (disable: 4200)

/**********************************************************************************/
// 句柄定义
/// DEV 句柄
typedef void*	NvdHandle;
/// 现场流句柄
typedef void*	RealHandle;
/// 远程回放句柄
typedef void*	PlayHandle;
///主动注册句柄
typedef void*   SmartRegClientHandle;

#pragma pack( push, 1 )

enum
{
	NVD_PROTOCAL_TCP			= 1,
};

/**********************************************************************************/
// 返回值定义
enum
{
	/// 1.对于没有实现的方法或属性设置返回值均为 -1
	NVD_ERR_NOIMPL					= (-1),
	/// 2.函数执行成功，返回值均为 0
	NVD_OK									= (0),
	/// 3.对于小于0x10000均为dvr错误码

	/// 5.本地错误码：从0x20000开始
	NVD_ERR_BASE						= 0x00020000,

	/// 5.1一般性错误
	NVD_ERR_ASSERT					= (NVD_ERR_BASE + 0x1),		///< 出现了绝不应该出现的情况，基本上是SDK的问题
	NVD_ERR_MALLOCMEMORY		= (NVD_ERR_BASE + 0x2),		///< 分配内存出错，系统错误
	NVD_ERR_CREATE_EVENT		= (NVD_ERR_BASE + 0x3),		///< 创建Event句柄出错，系统错误
	NVD_ERR_CREATE_THREAD		= (NVD_ERR_BASE + 0x4),		///< 创建线程出错，系统错误
	NVD_ERR_PARA						= (NVD_ERR_BASE + 0x5),		///< 参数错误
	NVD_ERR_SDK_NO_INIT			= (NVD_ERR_BASE + 0x6),		///< SDK没有初始化
	NVD_ERR_NVD_NO_LOGIN		= (NVD_ERR_BASE + 0x7),		///< 还没有登录
	NVD_ERR_NVD_INVALID_HANDLE	= (NVD_ERR_BASE + 0x8),	///< 无效的NvdHandle句柄
	NVD_ERR_COMMON					= (NVD_ERR_BASE + 0x9),		///< 通用错误

	/// 5.2网络错误
	NVD_ERR_ERR_TRANS_OPEN	= (NVD_ERR_BASE + 0x21),  ///<打开传输流错误
	NVD_ERR_NET_DISCONNECT	= (NVD_ERR_BASE + 0x22),  ///<网络断开
  NVD_ERR_HELLO           = (NVD_ERR_BASE + 0x23),  ///<网络断开

	/// 5.3报警错误
	NVD_ERR_ALARM_DISABLE		= (NVD_ERR_BASE + 0x101),	///< 报警功能没有打开
	NVD_ERR_ENABLE_ALARM		= (NVD_ERR_BASE + 0x102),	///< 启动报警功能时出错
	NVD_ERR_NOMORE_ALARM    = (NVD_ERR_BASE + 0x103),	///< 没有更多的报警信息了

	/// 5.4现场错误
	NVD_ERR_REAL_INVALID_HANDLE	= (NVD_ERR_BASE + 0x201),	///< 无效的现场句柄

	/// 5.5对讲错误
	NVD_ERR_SPEECH_INVALID_HANDLE = (NVD_ERR_BASE + 0x301),	///< 无效的对讲句柄

	/// 5.6 本地回放错误
	NVD_ERR_LOCALPLAY_INVALID_HANDLE = (NVD_ERR_BASE + 0x401),	///< 无效的本地回放句柄
	NVD_ERR_LOCALPLAY_INVALID_FNAME = (NVD_ERR_BASE + 0x402),	  ///< 无效的本地回放句柄
	NVD_ERR_LOCALPLAY_NO_OPEN = (NVD_ERR_BASE + 0x403),	        ///< 本地回放，还没有打开

	/// 5.7 
	NVD_ERR_CREATE_FILE			= (NVD_ERR_BASE + 0x501), ///<创建文件失败
	NVD_ERR_NET							= (NVD_ERR_BASE + 0x901), ///<网络错误
	NVD_VDCP_BUSY						= (NVD_ERR_BASE + 0x902), ///<命令通道忙
	NVD_VDCP_TIMEOUT				= (NVD_ERR_BASE + 0x903), ///<vdcp命令执行超时
	NVD_VDCP_NETERR					= (NVD_ERR_BASE + 0x904), ///<vdcp网络错误
	NVD_ERR_INIT						= (NVD_ERR_BASE + 0x905), ///<初始化错误
	NVD_ERR_NET_INIT				= (NVD_ERR_BASE + 0x906), ///<网络初始化错误
};

struct HelloRes
{
	int		product;					/**< 产品ID */
	unsigned char	macid[8];
	char	vdcpVersion;
	char	res[3];
};

/** @addtogroup grReal
*  @{
*/
/// 现场流打开参数
struct RealOpenPara
{
	char channel;     ///.通道号，1-16表示1-16通道。 
	char subStream;   /// 0表示主码流，1表示子码流。
	char transProc;   /// 传输协议: 1, TCP，4RTP
	char res;   /// 
	int  ip;
};

/// 帧的类型定义
enum
{
	VDFP_FRAME_TYPE_AUDIO		= 1,
	VDFP_FRAME_TYPE_VIDEO		= 2,
	VDFP_FRAME_TYPE_AI_FACE_DETECT  = 4,  //face detection frame, for face position rectangle overlap
	VDFP_FRAME_TYPE_AI_FACE_RECOG   = 5,  //face recog frame, for people name overlap

};

struct FrameHeader
{
  unsigned int mark;			  /**< 帧头的前导标识，用于帧头定位，固定值为0xBF9D1FDB */
  unsigned char type;			  /**< 帧类型标识符 1：音频 2：视频 3：语音 4: 音视频智能分析信息 9：消息通告 10: 命令请求与应答*/
  char channel;						  /**< 通道号  取值范围（-127~128）*/
  unsigned short device;	  /**< 设备出厂序列号 */
  unsigned int length;		  /**< 本帧数据(data)总长度，不包括28 个字节的帧头 */
  unsigned int sequence;	  /**< 该类型帧在该通道(channel)的序号，各类型独立计数，从系统启动后开始计数 */
  unsigned int timestamp;	  /**< 该帧捕获的系统时间，为time_t 标准时间 */
  unsigned int tick;			  /**< 该帧捕获的系统参考时钟计数(单位为微秒) */
  unsigned char codec;		
  unsigned char resolution;	/**< 分辨率 */
  unsigned char frameRate;
  unsigned char frameType;
};

struct FaceNetRecogNotifyMsg
{
    unsigned long long   pts64;
    float            vector[512];       /* 抓拍照片的特征值 */
    unsigned int     snapJpgSize;            
    unsigned int     matchId[5];        /* 0 for invalid */
    unsigned char    similarity[5];     /* 人脸相似度评分，0~100。*/
	unsigned char    trackId;
    unsigned char    res[2];
};

struct FacePhotoDesc
{
    char               name[32];    /* 姓名*/
    unsigned char      gender;      /* 性别*/
    unsigned char      certType;    /* 证件类型*/
    char               certNumber[30];  /* 证件号*/
    unsigned short     birthYear;   /* 出生-  年*/
    unsigned char      birthMon;    /* 出生-  月*/
    unsigned char      birthDay;    /* 出生-  日*/
    unsigned char      province;    /* 省*/
    unsigned char      city;        /* 市*/
    char               addr[62];    
    unsigned char      faceLibIndex; /*  人脸库编号*/
    unsigned char      res1[3];     /* 保留*/
    unsigned int       id;          /* 人脸ID*/
    unsigned char      res2[52];    /* 保留*/
};

struct FacePhotoDescStr
{
    char               name[32];        /* 姓名*/
    char               gender[16];      /* 性别*/
    char               certType[32];    /* 证件类型*/
    char               certNumber[30];  /* 证件号*/
    char               birth[16];       /* 出生日期*/
    char               province[16];    /* 省*/
    char               city[16];        /* 市*/
    char               addr[64];        /* 地址*/
    char               res[64];         /* 保留*/
};

struct FaceNetAddNotifyMsg
{
    FacePhotoDesc   desc;
    unsigned int            photoSize;
};

typedef void (*TCSmartRecogNotify)(FaceNetRecogNotifyMsg *pMsg, int nLen, void *data);
typedef void (*TCSmartAddNotify)(FaceNetAddNotifyMsg *pMsg, int nLen, void *data);
typedef void (*TCSmartDelNotify)(unsigned int *pId, void *data);

//#define DPU_COMM_STRUCT_V5
#define DPU_COMM_STRUCT_V2

struct DpuResQueryDPU
{
    unsigned char    software_version[3];
    unsigned char    hardware_version;
    unsigned int     FPGA_version;    // hex display
    unsigned int     release_date[3]; // year, month, day
#if (defined(DPU_COMM_STRUCT_V2) || defined(DPU_COMM_STRUCT_V5))
	unsigned int	uboot_version;
	unsigned int	kernel_version;
	unsigned int	fs_version;
	unsigned char	interface_doc_version[3];
	unsigned char    extra_length[29];
#else
    unsigned char    extra_length[44];
#endif
};

#if defined(DPU_COMM_STRUCT_V5)
struct CmFaceSmartFrameInfo
{
    unsigned short rawImageWidth;               // default:1920
    unsigned short rawImageHeight;              // default:1080
    unsigned short ROILeft;                     // detection region, default:0
    unsigned short ROITop;                      // detection region, default:0
    unsigned short ROIRight;                    // detection region, default:1920
    unsigned short ROIBottom;                   // detection region, default:1080
    unsigned short minFace;                     // min rect size
    unsigned short maxFace;                     // max rect size
    unsigned long long maxTimestampDelay;       // microseconds
    unsigned char  maxTrackingNumber;           // default:255
    unsigned char  detThreshold;                // default:90
    unsigned char  qualityThreshold;            // default:50
    unsigned char  similarityThreshold;         // default:83
    unsigned char  recogPolicy;                 // priority function
    unsigned char  firstCaptureInterval;      
    unsigned char  repeatedCaptureInterval;  
    unsigned char  reserved[33];
    unsigned char  res2[132 - 64]; 
};
#elif defined(DPU_COMM_STRUCT_V2)
struct CmFaceSmartFrameInfo
{
	unsigned short rawImageWidth;               // default:1920
    unsigned short rawImageHeight;              // default:1080
    unsigned short ROILeft;                     // detection region, default:0
    unsigned short ROITop;                      // detection region, default:0
    unsigned short ROIRight;                    // detection region, default:1920
    unsigned short ROIBottom;                   // detection region, default:1080
    unsigned short minFace;                     // min rect size
    unsigned short maxFace;                     // max rect size
    unsigned char  maxTrackingNumber;
	unsigned char  maxRecogNumber;
    unsigned char  detThreshold;                // default:90
    unsigned char  clarityThreshold;            // default:90
    unsigned char  shadeThreshold;              // unused now
    unsigned char  frontalThreshold;            // default:90
    unsigned char  lightThreshold;              // unused now
    unsigned char  similarityThreshold;         // unused in our module
    unsigned char  qualityPolicy;
    unsigned char  recogInterval;               // single object's recog period in seconds
    unsigned char  recogGap;                    // single object's waiting period in seconds
    unsigned char  reserved[37];
    unsigned char  res2[132 - 64]; 
};
#else
struct CmFaceSmartFrameInfo
{
    char speed;             /**< 目标生成次数 */
    char snapCount;         /**< 抓拍次数*/
    char policy;            /**< 灵敏度*/ 
    char snapInterval;      /**< 抓拍间隔*/
	char snapThreshold;     /**< 抓拍阈值*/	
	char compThreshold;     /**< 比对阈值*/
	char res[126];          /**< 保留值*/
};
#endif

struct FaceVector
{
    unsigned int    id;             //取值1~100000
    float           vector[512];    //特征向量
};

struct FaceNetPhotoDesc
{
    FacePhotoDesc   desc;
    unsigned int            photoSize;
    unsigned char           photo[2*1024*1024];
};

struct FaceNetPhotoDescVector
{
    FacePhotoDesc   desc;
	FaceVector      vector;
    unsigned int            photoSize;
    unsigned char           photo[2*1024*1024];
};

enum
{
    FACE_PHOTO_QUERY_OP_CONTAIN,      //^^     // just for field in string type 
    FACE_PHOTO_QUERY_OP_EQUAL,        //==
    FACE_PHOTO_QUERY_OP_BIG,          //>
    FACE_PHOTO_QUERY_OP_LESS,         //<
    FACE_PHOTO_QUERY_OP_NOT_EQUAL,    //!=
    FACE_PHOTO_QUERY_OP_EQUAL_BIG,    //>=
    FACE_PHOTO_QUERY_OP_EQUAL_LESS    //<=
};

struct FaceCompValueString
{
	unsigned char comparison;
	unsigned char value[55];
};

struct FaceCompValueId
{
	unsigned char comparison;
	unsigned char value;
	unsigned char res[2];
};

struct FaceQueryReq
{
	FaceCompValueString name;
	FaceCompValueId gender;
	FaceCompValueId certType;
	FaceCompValueString certNumber;
	FaceCompValueString beginBirth;
	FaceCompValueString endBirth;
	FaceCompValueId province;
	FaceCompValueId city;
	FaceCompValueString res2;
	unsigned int   limit;
};

struct FaceRecogSearchCond
{
    unsigned long long   startPts64;
    unsigned long long   endPts64;
    unsigned int         limit;             /* max ??? */
    unsigned char        order;             /* 0:ascending order;  1: descending order. */
    unsigned char        isUsingThreshold;  /* 0: all recog results;  1: recog results higher than threshold. */
    unsigned char        isNeedJpgContent;  /* 0: just return path of jpg in result ;  1: return jpg file content in result */
    unsigned char        res[1];
};

struct FaceRecogSimiId
{
    unsigned int           matchId;        /* 0 for invalid */
    unsigned char          similarity;     /* 人脸相似度评分，0~100。*/
    unsigned char          res[3];
    FacePhotoDesc  desc;
};

struct FaceRecogTopNSimi
{
    unsigned long long   pts64;    
    FaceRecogSimiId      simiList[5];
    unsigned int         pathDetectJpgSize;
    char                 pathDetect[128];    /* path of detected face photo */
    unsigned char        res[4];
};

//版本信息
struct SystemVersionInfo	 
{
	int product;            //产品开发编号
	char model[32];					//销售型号
	char version[32];				//录像机版本
	char serial[32];        //产品系列号
	unsigned char macid[8]; //机器标示 <前2字节为0， 后6各字节为MAC地址>
};

#define WEEK_DATE 7
struct TimeArea
{
	unsigned int bTime;
	unsigned int eTime;
	unsigned char res[4];
};

struct SmartAlarmLinkageCfg
{
	char alarm1;
	char alarm2;
	char alarmSound;
	char res[1];
	TimeArea timeArea[WEEK_DATE];
};

struct SmartRegClientInfo
{
	unsigned char mac[6];
	unsigned char res1[2];
	char          ip[16];
	char          hostName[32];
	char          devId[64];
	char          res2[128];
};

#pragma pack( pop )

#endif	// end of USE_NVD_SDK_TYPE_HEADER
