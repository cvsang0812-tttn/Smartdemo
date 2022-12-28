// RecogDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "RecogDetailDlg.h"


// RecogDetailDlg dialog

IMPLEMENT_DYNAMIC(RecogDetailDlg, CDialog)

RecogDetailDlg::RecogDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RecogDetailDlg::IDD, pParent)
{
    m_recog = NULL;
	m_faceCount = 0;
	m_hasShowPic = 0;
}

RecogDetailDlg::~RecogDetailDlg()
{
	if (m_recog)
	{
		free(m_recog);
	}
}

void RecogDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RecogDetailDlg, CDialog)
		ON_WM_PAINT()
		ON_BN_CLICKED(IDC_BUTTON_VIEW_SCENE, &RecogDetailDlg::OnBnClickedButtonViewScene)
END_MESSAGE_MAP()


// RecogDetailDlg message handlers
BOOL RecogDetailDlg::ShowImage(CDC* pDC,CString strPath, int x, int y)  
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

void RecogDetailDlg::setParam(NvdHandle hDev, FaceRecogTopNSimi *recog)
{
	m_hDev = hDev;

	int length = sizeof(FaceRecogTopNSimi) + recog->pathDetectJpgSize;
	if (m_recog)
	{
		free(m_recog);
	}
	m_recog = (FaceRecogTopNSimi *)malloc(length);
	memcpy(m_recog, recog, length);
}


BOOL RecogDetailDlg::OnInitDialog()
{
	int ret = 0;
    FacePhotoDescStr descStr;

	CDialog::OnInitDialog();

	if (m_recog)
	{
		CTime snapTime(m_recog->pts64/1000000);
        CString value;
		value.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), 
			snapTime.GetYear(), snapTime.GetMonth(), snapTime.GetDay(), snapTime.GetHour(), snapTime.GetMinute(), snapTime.GetSecond());	//ץ��ʱ��
		SetDlgItemText(IDC_STATIC_RECOG_DETAIL_TIME, value);

		char cmpStr[16] = {0};
		sprintf(cmpStr, "%d%%", m_recog->simiList[0].similarity);
		SetDlgItemText(IDC_STATIC_RECOG_DETAIL_CMPVALUE, cmpStr);
		sprintf(cmpStr, "%d%%", m_recog->simiList[1].similarity);
		SetDlgItemText(IDC_STATIC_RECOG_DETAIL_FACE2_CMPVALUE, cmpStr);
		sprintf(cmpStr, "%d%%", m_recog->simiList[2].similarity);
		SetDlgItemText(IDC_STATIC_RECOG_DETAIL_FACE3_CMPVALUE, cmpStr);
		sprintf(cmpStr, "%d%%", m_recog->simiList[3].similarity);
		SetDlgItemText(IDC_STATIC_RECOG_DETAIL_FACE4_CMPVALUE, cmpStr);
		sprintf(cmpStr, "%d%%", m_recog->simiList[4].similarity);
		SetDlgItemText(IDC_STATIC_RECOG_DETAIL_FACE5_CMPVALUE, cmpStr);

		m_faceCount = 0;
		for (int i = 0; i < 5; i++)
		{
		  if (m_recog->simiList[i].matchId > 0)
		  {
			memset(&m_desc, 0, sizeof(m_desc));
			ret = NvdSmartGetOnePhoto(m_hDev, m_recog->simiList[i].matchId, &m_desc);
			if (ret)
			{
				MessageBox(_T("Note"), _T("DEV"), MB_ICONERROR);		
                return FALSE;
            }

			if (0 == i)
			{
				NvdSmartConvertFaceDesc(m_hDev, &m_desc.desc, &descStr);
				SetDlgItemText(IDC_STATIC_RECOG_DETAIL_NAME, descStr.name);
				/*SetDlgItemText(IDC_STATIC_RECOG_DETAIL_GENDER, descStr.gender);*/
				/*SetDlgItemText(IDC_STATIC_RECOG_DETAIL_BIRTH, descStr.birth);*/
				/*SetDlgItemText(IDC_STATIC_RECOG_DETAIL_PROV, descStr.province);
				SetDlgItemText(IDC_STATIC_RECOG_DETAIL_CITY, descStr.city);
				SetDlgItemText(IDC_STATIC_RECOG_DETAIL_CERTTYPE, descStr.certType);
				SetDlgItemText(IDC_STATIC_RECOG_DETAIL_CERTNO, descStr.certNumber);*/
			}

			m_faceCount++;
			CFile file;
			CString facePath;
			TCHAR szFilePath[256 + 1]; 

			GetModuleFileName(NULL, szFilePath, MAX_PATH); 
			(_tcsrchr(szFilePath, _T('\\')))[0] = 0;
			//strcat(szFilePath, "\\recog_face1.jpg");
			sprintf(szFilePath, "%s\\recog_face%d.jpg", szFilePath, i + 1);
			facePath.Format(_T(szFilePath));
			file.Open(facePath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
			file.Write(m_desc.photo, m_desc.photoSize);
			file.Close();

			CDC *pDC = NULL;  
			pDC = GetDlgItem(IDC_STATIC_MATCH_FACE1 + i)->GetDC();  
			ShowImage(pDC,facePath, 0, 0);  
			ReleaseDC(pDC); // �ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶  
		  }
		}
		CFile file;
		CString facePath;
		TCHAR szFilePath[256 + 1]; 
		unsigned char *snap = (unsigned char *)m_recog + sizeof(FaceRecogTopNSimi);

		GetModuleFileName(NULL, szFilePath, MAX_PATH); 
		(_tcsrchr(szFilePath, _T('\\')))[0] = 0;
		strcat(szFilePath, "\\recog_snap.jpg");
		facePath.Format(_T(szFilePath));
		file.Open(facePath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
		file.Write(snap, m_recog->pathDetectJpgSize);
		file.Close();

		CDC *pDC = NULL;  
		pDC = GetDlgItem(IDC_STATIC_SNAP_PIC)->GetDC();  
		ShowImage(pDC,facePath, 0, 0);  
		ReleaseDC(pDC); // �ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶  
	}

	UpdateData(FALSE);

	return TRUE;
}



void RecogDetailDlg::OnPaint()
{

	if (m_recog) // && 0 == m_hasShowPic)
	{
		for (int i = 0; i < m_faceCount; i++)
		{
			CString facePath;
			TCHAR szFilePath[256 + 1]; 

			GetModuleFileName(NULL, szFilePath, MAX_PATH); 
			(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
			sprintf(szFilePath, "%s\\recog_face%d.jpg", szFilePath, i + 1);
			facePath.Format(_T(szFilePath));

			CDC *pDC = NULL;  
			pDC = GetDlgItem(IDC_STATIC_MATCH_FACE1 + i)->GetDC();  
			ShowImage(pDC,facePath, 0, 0);  
			ReleaseDC(pDC); // �ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶  
		}

		CString facePath;
		TCHAR szFilePath[256 + 1]; 

		GetModuleFileName(NULL, szFilePath, MAX_PATH); 
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
		strcat(szFilePath, "\\recog_snap.jpg");
		facePath.Format(_T(szFilePath));

		CDC *pDC = NULL;  
		pDC = GetDlgItem(IDC_STATIC_SNAP_PIC)->GetDC();  
		ShowImage(pDC,facePath, 0, 0);  
		ReleaseDC(pDC); // �ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶  

		m_hasShowPic = 1;
	}

	CDialog::OnPaint();
}

void RecogDetailDlg::OnBnClickedButtonViewScene()
{
	// TODO: Add your control notification handler code here
	if (TRUE == m_RecogSceneDlg.setParam(m_hDev, m_recog->pts64))\
	{
	    m_RecogSceneDlg.DoModal();
	}
}
