// SmartDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "SmartDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI 
helloThread( LPVOID lpvoid )
{
	CSmartDemoDlg* pDemoDlg = (CSmartDemoDlg* )lpvoid;
	if ( pDemoDlg == NULL )
	{
		return 0;
	}

	pDemoDlg->hello_Thread();	

	return 0;
}

void recogCallback(FaceNetRecogNotifyMsg *pMsg, int nLen, void *data)
{
	CSmartDemoDlg *pDlg;
	CFile file;
	CString snapPath;
	CString matchPath;
	CString str;
	int nRet;
	FaceNetPhotoDescVector *lpRecv;

    TCHAR szFilePath[256 + 1]; 
    GetModuleFileName(NULL, szFilePath, MAX_PATH); 

	pDlg = (CSmartDemoDlg *)data;
	
	//snapPath.Format(_T(".\\snap.jpg"));
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	strcat(szFilePath, "\\snap.jpg");
	snapPath.Format(_T(szFilePath));
	nRet = file.Open(snapPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
	if (0 == nRet)
	{
		return;
	}
	file.Write(((char *)pMsg)+sizeof(FaceNetRecogNotifyMsg), pMsg->snapJpgSize);
	file.Close();

	lpRecv = (FaceNetPhotoDescVector *)malloc(sizeof(FaceNetPhotoDescVector));
	nRet = NvdSmartGetOnePhoto(pDlg->m_hDev, pMsg->matchId[0], lpRecv);
	if (nRet != NVD_OK)
	{
		free(lpRecv);
		return;
	}
	//matchPath.Format(_T(".\\match.jpg"));
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	strcat(szFilePath, "\\match.jpg");
	matchPath.Format(_T(szFilePath));

	nRet = file.Open(matchPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
	if (0 == nRet)
	{
		return;
	}
	file.Write(lpRecv->photo, lpRecv->photoSize);
	file.Close();

	free(lpRecv);

	CDC *pDC=NULL;  
	pDC=pDlg->GetDlgItem(IDC_PIC_SNAP)->GetDC();  
	pDlg->ShowImage(pDC,snapPath,0,0);  
	pDlg->ReleaseDC(pDC); // 记得释放资源，不然会导致内存泄露  

	pDC=pDlg->GetDlgItem(IDC_PIC_STD)->GetDC();  
	pDlg->ShowImage(pDC,matchPath,0,0);  
	pDlg->ReleaseDC(pDC); // 记得释放资源，不然会导致内存泄露

	str.Format(_T("%d%c"), pMsg->similarity[0], '%');
	pDlg->m_ctrlSimilarity.SetWindowText(str);
	str.Format(_T("%d"), pMsg->trackId);//人脸ID
}

void addCallback(FaceNetAddNotifyMsg *pMsg, int nLen, void *data)
{

}

void delCallback(unsigned int *pId, void *data)
{

}

// CSmartDemoDlg 对话框

CSmartDemoDlg::CSmartDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSmartDemoDlg::IDD, pParent)
	, m_UserName(_T(""))
	, m_PassWord(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	sVdcp = INVALID_SOCKET;
	sFaceCmd = INVALID_SOCKET;
	sFaceNotify = INVALID_SOCKET;
	m_RegFlag = 0;
}

void CSmartDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_Ip);
	DDX_Text(pDX, IDC_EDIT1, m_UserName);
	DDX_Text(pDX, IDC_EDIT2, m_PassWord);
	DDX_Control(pDX, ID_LOGIN, m_LoginBTN);
	DDX_Control(pDX, ID_LOGOUT, m_ExitBTN);
	DDX_Control(pDX, IDC_VIDEO_WIN, m_ctrlVideoWin);
	DDX_Control(pDX, IDC_BUTTON_OPENVIDEO, m_PlayVideBTN);
	DDX_Control(pDX, IDC_BUTTON_CLOSEVIDEO, m_CloseVideoBTN);
	DDX_Control(pDX, IDC_BUTTON_PARAM, m_ParamBTN);
	DDX_Control(pDX, IDC_STATIC_PER, m_ctrlSimilarity);
	DDX_Control(pDX, IDC_BUTTON_REGISTER, m_RegBTN);
	DDX_Control(pDX, IDC_ANH, m_ANH);
}

BEGIN_MESSAGE_MAP(CSmartDemoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DEV_DISCONNECT, &CSmartDemoDlg::OnDisconnectMsg)
	ON_BN_CLICKED(ID_LOGOUT, &CSmartDemoDlg::OnBnClickedLogout)
	ON_BN_CLICKED(ID_LOGIN, &CSmartDemoDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_BUTTON_OPENVIDEO, &CSmartDemoDlg::OnBnClickedButtonOpenvideo)
	ON_BN_CLICKED(IDC_BUTTON_CLOSEVIDEO, &CSmartDemoDlg::OnBnClickedButtonClosevideo)
	ON_BN_CLICKED(IDC_BUTTON_PARAM, &CSmartDemoDlg::OnBnClickedButtonParam)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CSmartDemoDlg::OnBnClickedButtonRegister)
	ON_STN_CLICKED(IDC_STATIC_SDK_VERSION, &CSmartDemoDlg::OnStnClickedStaticSdkVersion)
	ON_STN_CLICKED(IDC_ANH, &CSmartDemoDlg::OnStnClickedAnh)
END_MESSAGE_MAP()


// CSmartDemoDlg 消息处理程序

BOOL CSmartDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//String pngPath = L"C:\Users\phong\Downloads\LOGO.png";
	CImageList pngImage;
	CBitmap pngBmp;
	CDC bmDC;
	CBitmap* pOldbmp;
	BITMAP  bi;
	UINT xPos = 255, yPos = 60;

	CClientDC dc(this);

	pngImage.CImageList(IDB_PNG2);
	// new code

	pngBmp.Attach(pngImage.Detach());

	bmDC.CreateCompatibleDC(&dc);

	pOldbmp = bmDC.SelectObject(&pngBmp);
	pngBmp.GetBitmap(&bi);
	dc.BitBlt(xPos, yPos, bi.bmWidth, bi.bmHeight, &bmDC, 0, 0, SRCCOPY);
	bmDC.SelectObject(pOldbmp);

	CBITMAP_.LoadBitmap(IDB_BITMAP1);
	m_ANH.SetBitmap((HBITMAP)CBITMAP_.Detach());
	UpdateData(FALSE);
	//SetDlgItemText(IDC_STATIC_SDK_VERSION, NvdSdkGetVersion());
	//NvdSdkInit();
	m_ctrlVideoDlg.Create(IDD_VIDEO_DIALOG, &m_ctrlVideoWin);

	CRect rc;
	m_ctrlVideoWin.GetClientRect(rc);
	m_ctrlVideoDlg.MoveWindow(&rc);
	m_ctrlVideoDlg.ShowWindow(SW_SHOW);

	m_LoginBTN.EnableWindow(TRUE);
	m_ExitBTN.EnableWindow(FALSE);
	m_PlayVideBTN.EnableWindow(FALSE);
	m_CloseVideoBTN.EnableWindow(FALSE);
	m_ParamBTN.EnableWindow(FALSE);

	//INIT_EASYSIZE;
    //ShowWindow(SW_MAXIMIZE);//对话框默认最大化弹出

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSmartDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSmartDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSmartDemoDlg::OnBnClickedLogout()
{
	// TODO: Add your control notification handler code here
	int nRet;
	if (0 == m_RegFlag)
	{
		if(!NvdIsLogin(m_hDev))
		{
			MessageBox(_T("Note"), _T("DEV"), MB_ICONERROR);
			EndDialog(0);
			return;
		}
	}

	m_ctrlVideoDlg.Stop(0);

	if (0 == m_RegFlag)
	{
		nRet = NvdLogout(m_hDev);
		if (nRet != NVD_OK)
		{
			MessageBox(_T("Note"), _T("DEV"), MB_ICONERROR);
			EndDialog(0);
			return;
		}
	}

	NvdDestory(m_hDev);

	m_LoginBTN.EnableWindow(TRUE);
	m_ExitBTN.EnableWindow(FALSE);
	m_PlayVideBTN.EnableWindow(FALSE);
	m_CloseVideoBTN.EnableWindow(FALSE);
	m_ParamBTN.EnableWindow(FALSE);
	SetDlgItemText(IDC_STATIC_IPC_VERSION, "");
	SetDlgItemText(IDC_STATIC_MODE_VERSION, "");
	m_hDev = NULL;
}

void CSmartDemoDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	DWORD ip;
	int nRet;

	UpdateData();
	m_Ip.GetAddress(ip);
	ip = htonl(ip);

	nRet = NvdCreate(inet_ntoa(*(struct in_addr*)(&ip)), 5231, 5320, m_UserName, m_PassWord, &m_hDev);
	if (nRet != 0 || m_hDev == NULL)
	{
		m_hDev = NULL;
		MessageBox(_T("Note"), _T("DEV"), MB_ICONERROR);
		return;
	}
	
	NvdSetReconnect(m_hDev, TRUE);
	NvdSetNotifyWnd(m_hDev, this->GetSafeHwnd(), WM_DEV_DISCONNECT);

	m_RegBTN.EnableWindow(FALSE);
	m_LoginBTN.EnableWindow(FALSE);
	m_ExitBTN.EnableWindow(TRUE);
	m_PlayVideBTN.EnableWindow(TRUE);
	m_CloseVideoBTN.EnableWindow(FALSE);
	m_ParamBTN.EnableWindow(TRUE);

	NvdSmartRecogNotifyCallbackSet(m_hDev, recogCallback, (void *)this);
	NvdSmartAddNotifyCallbackSet(m_hDev, addCallback, (void *)2);
	NvdSmartDelNotifyCallbackSet(m_hDev, delCallback, (void *)3);
	
	SystemVersionInfo pPara;
	NvdSysVersion(m_hDev, &pPara);
	SetDlgItemText(IDC_STATIC_IPC_VERSION, pPara.version);
	SetDlgItemText(IDC_STATIC_MODE_VERSION, pPara.model);
	m_RegFlag = 0;
}

BOOL CSmartDemoDlg::ShowImage(CDC* pDC,CString strPath, int x, int y)  
{  
     
    IPicture *pPic=NULL;
	//CComBSTR(strPath.GetBuffer())
    OleLoadPicturePath(strPath.AllocSysString(), (LPUNKNOWN)NULL, 0, 0, IID_IPicture,(LPVOID*)&pPic);  
    if (NULL==pPic)  
    {  
        return FALSE;  
    }  
  
    // 获取图像宽和高,注意这里的宽和高不是图像的分辨率  
    OLE_XSIZE_HIMETRIC hmWidth;    
    OLE_YSIZE_HIMETRIC hmHeight;    
    pPic->get_Width(&hmWidth);    
    pPic->get_Height(&hmHeight);    
  
    // 将图像宽度和高度单位转化为像素单位  
   //#define HIMETRIC_PER_INCH 2540  
   //int nPicWidth =  MulDiv(hmWidth, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSX),2540);  
   //int nPicHeight = MulDiv(hmHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),2540);  
  
   // 获取显示图片窗口的宽度和高度  
   RECT rtWnd;  
   pDC->GetWindow()->GetWindowRect(&rtWnd);  
   int iWndWidth=rtWnd.right-rtWnd.left;  
   int iWndHeight=rtWnd.bottom-rtWnd.top;  
  
   if(FAILED(pPic->Render(*pDC,x,y,iWndWidth,iWndHeight,0,hmHeight,hmWidth,-hmHeight,NULL)))    
   {  
       pPic->Release();  
       return false;  
   }  
  
   //记得释放资源，不然会导致内存泄露  
   pPic->Release();   
     
   return true;  
}  

void CSmartDemoDlg::OnBnClickedButtonOpenvideo()
{
	// TODO: Add your control notification handler code here
	m_ctrlVideoDlg.OnPreview(m_hDev, 0);
	m_PlayVideBTN.EnableWindow(FALSE);
	m_CloseVideoBTN.EnableWindow(TRUE);
}

void CSmartDemoDlg::OnBnClickedButtonClosevideo()
{
	// TODO: Add your control notification handler code here
	m_ctrlVideoDlg.Stop(0);
	m_PlayVideBTN.EnableWindow(TRUE);
	m_CloseVideoBTN.EnableWindow(FALSE);
}

ParaDlg m_ctrlParaDlg;
void CSmartDemoDlg::OnBnClickedButtonParam()
{
	// TODO: Add your control notification handler code here
	m_ctrlParaDlg.setParam(m_hDev);
	
	INT_PTR ret = m_ctrlParaDlg.DoModal();
	
}

int RegCallback( void *pPara, SmartRegClientHandle regHandle, SmartRegClientInfo regInfo )
{
	CSmartDemoDlg *dlg = (CSmartDemoDlg *)pPara;
	if ( dlg == NULL )
	{
		return 0;
	}

	dlg->registerCallback(pPara, regHandle, regInfo);

	return 0;
}

void CSmartDemoDlg::registerCallback(void *pPara, SmartRegClientHandle regHandle, SmartRegClientInfo regInfo)
{
	int nRet;

	nRet = NvdCreateBySocket( regHandle, &m_hDev, 0 );
	if (nRet != 0 || m_hDev == NULL)
	{
		m_hDev = NULL;
		MessageBox(_T("Note"), _T("DEV"), MB_ICONERROR);
		return;
	}
#if 0
	NvdSetNotifyWnd(m_hDev, GetSafeHwnd(), WM_DEV_DISCONNECT);
#else
	HANDLE thread;
	thread = CreateThread( NULL, 64*1024, helloThread, this, 0, 0 );
	if ( NULL == thread )
	{
		return;
	}
#endif
	m_LoginBTN.EnableWindow(FALSE);
	m_ExitBTN.EnableWindow(TRUE);
	m_PlayVideBTN.EnableWindow(TRUE);
	m_CloseVideoBTN.EnableWindow(FALSE);
	m_ParamBTN.EnableWindow(TRUE);

	NvdSmartRecogNotifyCallbackSet(m_hDev, recogCallback, (void *)this);
	NvdSmartAddNotifyCallbackSet(m_hDev, addCallback, (void *)2);
	NvdSmartDelNotifyCallbackSet(m_hDev, delCallback, (void *)3);
		
	SystemVersionInfo sysParam;
	NvdSysVersion(m_hDev, &sysParam);
	SetDlgItemText(IDC_STATIC_IPC_VERSION, sysParam.version);
	SetDlgItemText(IDC_STATIC_MODE_VERSION, sysParam.model);
	m_RegFlag = 1;
}

void CSmartDemoDlg::OnBnClickedButtonRegister()
{
	// TODO: Add your control notification handler code here
	m_LoginBTN.EnableWindow(FALSE);
	NvdSdkSetRegCallback( 8888, RegCallback, this );
}

LRESULT CSmartDemoDlg::OnDisconnectMsg(WPARAM wParam, LPARAM lParam)
{
	OnBnClickedLogout();
	return LRESULT();
}

void CSmartDemoDlg::hello_Thread()
{
	bool exit = true;
	int ret;
	HelloRes res;
	int count = 0;

	do
	{
		ret = NvdSysHello(m_hDev, &res);
		if (ret != 0)
		{
			if (count >= 5)
			{
				exit = false;
			}
			else
			{
				count++;
			}
		}
		else
		{
			count = 0;
		}
		Sleep(5000);
	}while(exit);

	OnBnClickedLogout();
}

void CSmartDemoDlg::OnStnClickedStaticSdkVersion()
{
	// TODO: Add your control notification handler code here
}


void CSmartDemoDlg::OnStnClickedAnh()
{
	// TODO: Add your control notification handler code here
}
