// FaceDetialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "FaceDetialDlg.h"


// FaceDetialDlg dialog

IMPLEMENT_DYNAMIC(FaceDetialDlg, CDialog)

FaceDetialDlg::FaceDetialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FaceDetialDlg::IDD, pParent)
{
    id = 0;
}

FaceDetialDlg::~FaceDetialDlg()
{
	if (photoDesc)
	{
		free(photoDesc);
	}
}

void FaceDetialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FACE_DETAIL_NAME, m_CtrlNewName);
}


BEGIN_MESSAGE_MAP(FaceDetialDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK_DETAIL_MODIFY, &FaceDetialDlg::OnBnClickedDetailModify)
END_MESSAGE_MAP()


// FaceDetialDlg message handlers

BOOL FaceDetialDlg::ShowImage(CDC* pDC,CString strPath, int x, int y)  
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

void FaceDetialDlg::setParam(NvdHandle hDev)
{
	m_hDev = hDev;
}

void FaceDetialDlg::setFaceId(unsigned int inputId)
{
	id = inputId;
}


BOOL FaceDetialDlg::OnInitDialog()
{
	int ret = 0;
    FacePhotoDescStr descStr;

	CDialog::OnInitDialog();

    photoDesc = (FaceNetPhotoDescVector *)malloc(sizeof(FaceNetPhotoDescVector));
	ret = NvdSmartGetOnePhoto(m_hDev, id, photoDesc);
	if (ret)
	{
		MessageBox(_T("获取人脸信息出错"), _T("DEV"), MB_ICONERROR);		
        return FALSE;
	}

	NvdSmartConvertFaceDesc(m_hDev, &photoDesc->desc, &descStr);
	SetDlgItemText(IDC_EDIT_FACE_DETAIL_NAME, descStr.name);
	SetDlgItemText(IDC_STATIC_DETAIL_GENDER, descStr.gender);
	SetDlgItemText(IDC_STATIC_DETAIL_BIRTH, descStr.birth);
	SetDlgItemText(IDC_STATIC_DETAIL_PROV, descStr.province);
	SetDlgItemText(IDC_STATIC_DETAIL_CITY, descStr.city);
	SetDlgItemText(IDC_STATIC_DETAIL_CERTTYPE, descStr.certType);
	SetDlgItemText(IDC_STATIC_DETAIL_CERTNO, descStr.certNumber);
	//SetDlgItemText(IDC_STATIC_DETAIL_MEMO, descStr.res);

	CFile file;
	CString facePath;
    TCHAR szFilePath[256 + 1]; 

	GetModuleFileName(NULL, szFilePath, MAX_PATH); 
    (_tcsrchr(szFilePath, _T('\\')))[0] = 0;
	strcat(szFilePath, "\\face.jpg");
	facePath.Format(_T(szFilePath));
	file.Open(facePath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
	file.Write(photoDesc->photo, photoDesc->photoSize);
	file.Close();

	CDC *pDC = NULL;  
	pDC = GetDlgItem(IDC_STATIC_FACE_DETAIL_PHOTO)->GetDC();  
	ShowImage(pDC,facePath, 0, 0);  
	ReleaseDC(pDC); // 记得释放资源，不然会导致内存泄露  

	UpdateData(FALSE);

	return TRUE;
}

void FaceDetialDlg::OnBnClickedDetailModify()
{
	// TODO: Add your control notification handler code here
    CString value;
	int ret = 0;
    FaceNetPhotoDescVector *photoDesc = (FaceNetPhotoDescVector *)malloc(sizeof(FaceNetPhotoDescVector));

	ret = NvdSmartGetOnePhoto(m_hDev, id, photoDesc);
	if (ret)
	{
		MessageBox(_T("获取人脸信息出错"), _T("DEV"), MB_ICONERROR);
		free(photoDesc);
        return;
	}

	/* Demo程序里仅仅演示了如何修改姓名字段 */
	m_CtrlNewName.GetWindowTextA(value);
	strcpy(photoDesc->desc.name, value);

	ret = NvdSmartModifyOnePhoto(m_hDev, &photoDesc->desc);
	if (ret)
	{
		MessageBox(_T("Error"), _T("DEV"), MB_ICONERROR);
		free(photoDesc);
        return;
	}

	free(photoDesc);
    return;
}

void FaceDetialDlg::OnPaint()
{
#if 1
	if (photoDesc)
	{
	CFile file;
	CString facePath;
    TCHAR szFilePath[256 + 1]; 

	GetModuleFileName(NULL, szFilePath, MAX_PATH); 
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	strcat(szFilePath, "\\face.jpg");
	facePath.Format(_T(szFilePath));
	file.Open(facePath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
	file.Write(photoDesc->photo, photoDesc->photoSize);
	file.Close();

	CDC *pDC = NULL;  
	pDC = GetDlgItem(IDC_STATIC_FACE_DETAIL_PHOTO)->GetDC();  
	ShowImage(pDC,facePath, 0, 0);  
	ReleaseDC(pDC); // 记得释放资源，不然会导致内存泄露  
	}
#endif
	CDialog::OnPaint();

}
