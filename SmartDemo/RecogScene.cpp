// RecogScene.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "RecogScene.h"


// RecogScene dialog

IMPLEMENT_DYNAMIC(RecogScene, CDialog)

RecogScene::RecogScene(CWnd* pParent /*=NULL*/)
	: CDialog(RecogScene::IDD, pParent)
{

}

RecogScene::~RecogScene()
{
}

void RecogScene::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RecogScene, CDialog)
		ON_WM_PAINT()
END_MESSAGE_MAP()


// RecogScene message handlers

BOOL RecogScene::ShowImage(CDC* pDC,CString strPath, int x, int y)  
{  
     
    IPicture *pPic=NULL;
	//CComBSTR(strPath.GetBuffer())
    OleLoadPicturePath(strPath.AllocSysString(), (LPUNKNOWN)NULL, 0, 0, IID_IPicture,(LPVOID*)&pPic);  
    if (NULL==pPic)  
    {  
        return FALSE;  
    }  
  
    // ��ȡͼ���͸�,ע������Ŀ�͸߲���ͼ��ķֱ���  
    OLE_XSIZE_HIMETRIC hmWidth;    
    OLE_YSIZE_HIMETRIC hmHeight;    
    pPic->get_Width(&hmWidth);    
    pPic->get_Height(&hmHeight);    
  
    // ��ͼ���Ⱥ͸߶ȵ�λת��Ϊ���ص�λ  
   //#define HIMETRIC_PER_INCH 2540  
   //int nPicWidth =  MulDiv(hmWidth, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSX),2540);  
   //int nPicHeight = MulDiv(hmHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),2540);  
  
   // ��ȡ��ʾͼƬ���ڵĿ�Ⱥ͸߶�  
   RECT rtWnd;  
   pDC->GetWindow()->GetWindowRect(&rtWnd);  
   int iWndWidth=rtWnd.right-rtWnd.left;  
   int iWndHeight=rtWnd.bottom-rtWnd.top;  
  
   if(FAILED(pPic->Render(*pDC,x,y,iWndWidth,iWndHeight,0,hmHeight,hmWidth,-hmHeight,NULL)))    
   {  
       pPic->Release();  
       return false;  
   }  
  
   //�ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶  
   pPic->Release();   
     
   return true;  
}  

BOOL RecogScene::setParam(NvdHandle hDev, unsigned long long pts64)
{
	int ret = 0;
	int size = 0;

	m_hDev = hDev;
    m_pts64 = pts64;

	ret = NvdSmartGetSceneSnapByTime(m_hDev, m_pts64, m_scene, sizeof(m_scene));
	if (ret)
	{
		MessageBox(_T("��ȡȫ��ͼ����"), _T("DEV"), MB_ICONERROR);
		return FALSE;
	}
	memcpy(&size, m_scene, sizeof(int));

	CFile file;
	CString scenePath;
	TCHAR szFilePath[256 + 1]; 

	GetModuleFileName(NULL, szFilePath, MAX_PATH); 
	(_tcsrchr(szFilePath, _T('\\')))[0] = 0;
	sprintf(szFilePath, "%s\\scene.jpg", szFilePath);
	scenePath.Format(_T(szFilePath));
	file.Open(scenePath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
	file.Write(m_scene + 4, size);
	file.Close();

	return TRUE;
}

void RecogScene::OnPaint()
{
	CString scenePath;
	TCHAR szFilePath[256 + 1];


	GetModuleFileName(NULL, szFilePath, MAX_PATH); 
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	strcat(szFilePath, "\\scene.jpg");
	scenePath.Format(_T(szFilePath));

	CDC *pDC = NULL;  
	pDC = GetDlgItem(IDC_STATIC_SCENE_JPG)->GetDC();  
	ShowImage(pDC,scenePath, 0, 0);  
	ReleaseDC(pDC); // �ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶  

	CDialog::OnPaint();

}