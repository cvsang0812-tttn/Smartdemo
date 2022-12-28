// AddPhotoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "AddPhotoDlg.h"
#include "SDK.h"

// AddPhotoDlg dialog

IMPLEMENT_DYNAMIC(AddPhotoDlg, CDialog)

AddPhotoDlg::AddPhotoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AddPhotoDlg::IDD, pParent)
	, m_strFacePath(_T(""))
{
    m_isBatchImporting = FALSE;
}

AddPhotoDlg::~AddPhotoDlg()
{
}

void AddPhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ctrlName);
	DDX_Control(pDX, IDC_COMBO_GENDER, m_ctrlGender);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BIRTHDAY, m_ctrlBirthDay);
	DDX_Control(pDX, IDC_COMBO_PROVINCE, m_ctrlProvince);
	DDX_Control(pDX, IDC_COMBO_CITY, m_ctrlCity);
	DDX_Control(pDX, IDC_COMBO_CERTTYPE, m_ctrlCertType);
	DDX_Control(pDX, IDC_EDIT_CERTNUMBER, m_ctrlCertNumber);
	DDX_Control(pDX, IDC_EDIT_OTHER, m_ctrlOther);
}


BEGIN_MESSAGE_MAP(AddPhotoDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADDFACE, &AddPhotoDlg::OnBnClickedButtonAddface)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &AddPhotoDlg::OnBnClickedButtonImport)
	ON_CBN_SELCHANGE(IDC_COMBO_PROVINCE, &AddPhotoDlg::OnCbnSelchangeComboProvince)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_FACE_LIB, &AddPhotoDlg::OnBnClickedButtonExportFaceLib)
	ON_BN_CLICKED(IDC_BUTTON_SYNC_FACE_LIB, &AddPhotoDlg::OnBnClickedButtonSyncFaceLib)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_FROM_DIR, &AddPhotoDlg::OnBnClickedButtonImportFromDir)
END_MESSAGE_MAP()


// AddPhotoDlg message handlers
BOOL AddPhotoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int nIndex = 0;
	int i;
	CString str;
	
	nIndex = m_ctrlGender.AddString(_T("Male"));
	m_ctrlGender.SetItemData(nIndex, 1);
	nIndex = m_ctrlGender.AddString(_T("Female"));
	m_ctrlGender.SetItemData(nIndex, 2);
	m_ctrlGender.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void AddPhotoDlg::OnBnClickedButtonAddface()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, _T("jpg"), _T("*.jpg"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("JPEG�ļ�(*.jpg)|*.jpg"), NULL);

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	((CStatic *)GetDlgItem(IDC_STATIC_FACE))->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);

	m_strFacePath = dlg.GetPathName();

	CDC *pDC = NULL;
	pDC = GetDlgItem(IDC_STATIC_FACE)->GetDC();
	DrawImg(pDC, m_strFacePath, 0, 0);

	ReleaseDC(pDC);
}

void AddPhotoDlg::DrawImg(CDC * pDC, CString strPath, int x, int y)
{
	IPicture *pPic = NULL;
	OleLoadPicturePath(CComBSTR(strPath.GetBuffer()), (LPUNKNOWN)NULL, 0, 0, IID_IPicture, (LPVOID *)&pPic);
	if (NULL == pPic)
	{
		return;
	}

	OLE_XSIZE_HIMETRIC hmWidth;
	OLE_YSIZE_HIMETRIC hmHeight;
	pPic->get_Width(&hmWidth);
	pPic->get_Height(&hmHeight);

	RECT rtWnd;
	pDC->GetWindow()->GetWindowRect(&rtWnd);
	int iWndWidth = rtWnd.right - rtWnd.left;
	int iWndHeight = rtWnd.bottom - rtWnd.top;
	

	if(FAILED(pPic->Render(*pDC, x, y, iWndWidth, iWndHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)))      
    {    
        pPic->Release();    
        return;    
    }    
  
    //�ǵ��ͷ���Դ����Ȼ�ᵼ���ڴ�й¶    
    pPic->Release();     
  
    return; 
}

DWORD WINAPI 
batchImportProgThread( LPVOID lpvoid )
{
	int nRet = 0, total = 0, success = 0, fail = 0;
	AddPhotoDlg *dlg = (AddPhotoDlg *)lpvoid;
    NvdHandle nvdHandle = dlg->m_hNvd;
    char msg[128];

	while(dlg->m_isBatchImporting)
	{
	    nRet = NvdSmartSmartBatchImportGetProgress(nvdHandle, &total, &success, &fail);
		if (0 == nRet)
		{
			sprintf(msg, "���������������� ������ %d, �ɹ� %d, ʧ�� %d", total, success, fail);
			dlg->SetDlgItemTextA(IDC_STATIC_BATCHIMPORT_PROGRESS, msg);
		}
		Sleep(1000);
	}

	return 0;
}

static char failedItems[2 * 1024 * 1024] = {0};

DWORD WINAPI 
batchImportThread( LPVOID lpvoid )
{
	int nRet = 0, total = 0, success = 0, fail = 0;
	AddPhotoDlg *dlg = (AddPhotoDlg *)lpvoid;
    NvdHandle nvdHandle = dlg->m_hNvd;
    char msg[128];
	char *csvHead = "����,�Ա�,����,ʡ��,����,֤������,֤����,�Զ���,������Ƭ�ļ���\r\n";

	nRet = NvdSmartSmartBatchImportFaceLib(dlg->m_hNvd, dlg->m_csvPath.GetBuffer(dlg->m_csvPath.GetLength()), failedItems, &total, &success);
    dlg->m_isBatchImporting = FALSE;
	
	sprintf(msg, "���������������� ������ %d, �ɹ� %d, ʧ�� %d", total, success, total - success);
	dlg->SetDlgItemTextA(IDC_STATIC_BATCHIMPORT_PROGRESS, msg);

	sprintf(msg, "�������� total %d, success %d", total, success);
	if (nRet != NVD_OK)
	{
    	MessageBox(dlg->m_hWnd, msg, _T("DEV"), MB_ICONERROR);
    	//MessageBox(dlg->m_hWnd, failedItems, _T("����ʧ����Ŀ"), MB_ICONERROR);
		CString szFilePath;
		CFile file;
		szFilePath.Format(_T("failed.csv"));
		file.Open(szFilePath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
		file.Write(csvHead, strlen(csvHead));
		file.Write(failedItems, strlen(failedItems));
		file.Close();
		return 0;
	}
	MessageBox(dlg->m_hWnd, msg, _T("DEV"), MB_ICONERROR);

	return 0;
}

void AddPhotoDlg::OnBnClickedButtonImport()
{
	// TODO: Add your control notification handler code here
	//CString csvPath;

	CFileDialog dlg(TRUE,"csv","*.csv", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"CSV�ļ�(*.csv)",NULL);   

    if(dlg.DoModal()==IDOK)   
    {   
		char failedItems[1024] = {0};
		int total = 0, success = 0;
		int nRet = 0;
		char msg[64];
    	m_csvPath = dlg.GetPathName();

		m_isBatchImporting = TRUE;
		m_batchImportProgThread = CreateThread( NULL, 64*1024, batchImportProgThread, this, 0, 0 );
		if ( NULL == m_batchImportProgThread )
		{
			sprintf(msg, "�����߳�ʧ��");
			MessageBox(msg, _T("DEV"), MB_ICONERROR);
			return;
		}

		m_batchImportThread = CreateThread( NULL, 64*1024, batchImportThread, this, 0, 0 );
		if ( NULL == m_batchImportThread )
		{
			sprintf(msg, "�����߳�ʧ��");
			MessageBox(msg, _T("DEV"), MB_ICONERROR);
			return;
		}

		return;
	}   
}

void AddPhotoDlg::LoadData(NvdHandle hNvd)
{
	char *province;
	char *city;
	int count = 0;
	int i;
	int id = 0;
	int nIndex = 0;
	CString str;

	m_hNvd = hNvd;

	 NvdSmartGetProvinceList(hNvd, &province, &count);
	/*for (i = 0; i < count; i++)
	{
		str.Format(_T("%s"), province+(i*16));
		nIndex = m_ctrlProvince.AddString(str);
		m_ctrlProvince.SetItemData(nIndex, i);
	}*/
	m_ctrlProvince.SetCurSel(0);


	NvdSmartGetCityList(hNvd, &id, &city, &count);
	/*for (i = 0; i < count; i++)
	{
		str.Format(_T("%s"), city+(i*16));
		nIndex = m_ctrlCity.AddString(str);
		m_ctrlCity.SetItemData(nIndex, i);
	}*/
	m_ctrlCity.SetCurSel(0);

	str.Format(_T("ID"));
	nIndex = m_ctrlCertType.AddString(str);
	m_ctrlCertType.SetItemData(nIndex, 1);
	str.Format(_T("Army ID"));
	nIndex = m_ctrlCertType.AddString(str);
	m_ctrlCertType.SetItemData(nIndex, 2);
	m_ctrlCertType.SetCurSel(0);
}

void AddPhotoDlg::OnCbnSelchangeComboProvince()
{
	// TODO: Add your control notification handler code here
	char *city;
	int count = 0;
	int i;
	int id = 0;
	int nIndex = 0;
	CString str;
	
	id = m_ctrlProvince.GetCurSel();

	m_ctrlCity.ResetContent();
	NvdSmartGetCityList(m_hNvd, &id, &city, &count);
	for (i = 0; i < count; i++)
	{
		str.Format(_T("%s"), city+(i*16));
		nIndex = m_ctrlCity.AddString(str);
		m_ctrlCity.SetItemData(nIndex, i);
	}
	m_ctrlCity.SetCurSel(0);
}


void AddPhotoDlg::DlgSaveData(NvdHandle hNvd)
{
	FaceNetPhotoDesc *photo;
	FaceVector vector;
	CString value;
	CTime time;
	CString certNumber;
	CString other;
	int nRet;
	CFile file;

	ULONGLONG size;  
	CFileStatus fileStatus;  
  
	photo = (FaceNetPhotoDesc *)malloc(sizeof(FaceNetPhotoDesc));
	photo->photoSize = 0;

	if (CFile::GetStatus(m_strFacePath, fileStatus))  
	{  
		photo->photoSize = fileStatus.m_size;  
	}

	if (photo->photoSize <= 0 || photo->photoSize > 1024*1024*1.4)
	{
		MessageBox(_T("Note"), _T("DEV"), MB_ICONERROR);
		return;
	}

	file.Open(m_strFacePath, CFile::modeRead|CFile::shareDenyNone);
	file.Read(photo->photo, photo->photoSize);
	file.Close();

	m_ctrlName.GetWindowTextA(value);
	strcpy(photo->desc.name, value);
	photo->desc.gender = (unsigned char)m_ctrlGender.GetItemData(m_ctrlGender.GetCurSel());
	m_ctrlBirthDay.GetTime(time);
	photo->desc.birthYear = time.GetYear();
	photo->desc.birthMon = time.GetMonth();
	photo->desc.birthDay = time.GetDay();
	photo->desc.certType = (unsigned char)m_ctrlGender.GetItemData(m_ctrlCertType.GetCurSel());
	photo->desc.province = (unsigned char)m_ctrlProvince.GetItemData(m_ctrlProvince.GetCurSel());
	photo->desc.city = (unsigned char)m_ctrlCity.GetItemData(m_ctrlCity.GetCurSel());
	m_ctrlCertNumber.GetWindowTextA(value);
	strcpy(photo->desc.certNumber, value);
	m_ctrlOther.GetWindowTextA(value);
	strcpy((char *)photo->desc.res2, value);

	nRet = NvdSmartAddOnePhoto(hNvd, photo, &vector);
	if (NVD_OK != nRet)
	{
		MessageBox(_T("����ʧ��"), _T("DEV"), MB_ICONERROR);
	}
	free(photo);
}

static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
    switch(uMsg)
    {
    case BFFM_INITIALIZED:
        ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);
        break;
    }

    return 0;
}

void AddPhotoDlg::OnBnClickedButtonExportFaceLib(void)
{
    char szDir[MAX_PATH];
    BROWSEINFO bi; 
	char strDir[MAX_PATH];
	int count = 0, ret = 0, i;
	CFile file, idListFile;
	CString faceFilePath;
    TCHAR szFilePath[256 + 1]; 
	char msg[256];

	strcpy(msg, "�������� ��");
    this->SetDlgItemTextA(IDC_STATIC_EXPORT_FACE_LIB_PROGRESS, msg);

    bi.hwndOwner = (HWND)this->m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szDir;
    bi.lpszTitle = "ѡ�񵼳�������Ŀ¼��";
    bi.iImage = 0;
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
    bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)strDir;

    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if (!lp || FALSE == SHGetPathFromIDList(lp,szDir))
    {
		return;
	}
	strcpy(strDir, szDir);

	FaceQueryReq  cond;
	CString value;
	FacePhotoDesc *faces = NULL;

	memset(&cond, 0, sizeof(cond));
	cond.limit = 100000;
	faces = (FacePhotoDesc *)malloc(cond.limit * sizeof(FacePhotoDesc));

	ret = NvdSmartQueryFaceLib(m_hNvd, &cond, &count, &faces);
	if (ret)
	{
		MessageBox(_T("��ѯ���������"), _T("DEV"), MB_ICONERROR);
		if (faces)
		{
			free(faces);
		}
		return;
	}

    sprintf(szFilePath, "%s\\idlist.bin", strDir);
	faceFilePath.Format(_T(szFilePath));
	idListFile.Open(faceFilePath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
	for(i = 0; i < count; i++)
	{
		memset(&m_faceInfo, 0, sizeof(m_faceInfo));
		ret = NvdSmartGetOnePhoto(m_hNvd, faces[i].id,  &m_faceInfo);
	    if (ret)
		{
			MessageBox(_T("��ȡ������Ϣ����"), _T("DEV"), MB_ICONERROR);
            continue;
		}
		sprintf(szFilePath, "%s\\%d.bin", strDir, faces[i].id);
	    faceFilePath.Format(_T(szFilePath));
	    file.Open(faceFilePath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
	    file.Write(&m_faceInfo, offsetof(FaceNetPhotoDescVector, photo) + m_faceInfo.photoSize);
	    file.Close();

	    idListFile.Write(&faces[i].id, sizeof(unsigned int));

		sprintf(msg, "�������� ������ %d, �Ѿ���� %d, ���� %d%%", count, i + 1, (i + 1) * 100 / count);
	    this->SetDlgItemTextA(IDC_STATIC_EXPORT_FACE_LIB_PROGRESS, msg);
	}
	sprintf(msg, "�������� ������ %d, �Ѿ���� %d, ���� %d%%", count, count, 100);
    this->SetDlgItemTextA(IDC_STATIC_EXPORT_FACE_LIB_PROGRESS, msg);
    idListFile.Close();
	
	if (faces)
	{
		free(faces);
	}
	return;
}

void AddPhotoDlg::OnBnClickedButtonSyncFaceLib()
{
    char szDir[MAX_PATH];
    BROWSEINFO bi; 
	char strDir[MAX_PATH];
	int count = 0, ret = 0, i;
	CFile file, idListFile;
	CString faceFilePath;
    TCHAR szFilePath[256 + 1]; 
	char msg[256];
	unsigned int *idList = NULL;
	int length = 0;
	int id;

	strcpy(msg, "������� ��");
    this->SetDlgItemTextA(IDC_STATIC_SYNC_FACE_LIB_PROGRESS, msg);

    bi.hwndOwner = (HWND)this->m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szDir;
    bi.lpszTitle = "ѡ����������Ŀ¼��";
    bi.iImage = 0;
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
    bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)strDir;

    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if (!lp || FALSE == SHGetPathFromIDList(lp,szDir))
    {
		return;
	}
	strcpy(strDir, szDir);


    sprintf(szFilePath, "%s\\idlist.bin", strDir);
	faceFilePath.Format(_T(szFilePath));
	if (TRUE == idListFile.Open(faceFilePath, CFile::modeRead, NULL))
	{
		idList = (unsigned int *)malloc(sizeof(unsigned int) * 1000000);
		count = idListFile.Read(idList, sizeof(unsigned int) * 1000000);
		count /= sizeof(unsigned int);
   	    idListFile.Close();

		for(i = 0; i < count; i++)
		{
			id = idList[i];

			memset(&m_faceInfo, 0, sizeof(m_faceInfo));
			sprintf(szFilePath, "%s\\%d.bin", strDir, id);
			faceFilePath.Format(_T(szFilePath));
			if (FALSE == file.Open(faceFilePath, CFile::modeRead, NULL))
			{
				//MessageBox(_T("��ȡ������Ϣ����"), _T("DEV"), MB_ICONERROR);
				continue;
			}
			length = file.Read(&m_faceInfo, sizeof(m_faceInfo));
			file.Close();

			if (length <= 0 || length >= sizeof(m_faceInfo))
			{
				MessageBox(_T("�ļ����ȳ���"), _T("DEV"), MB_ICONERROR);
				continue;
			}

			ret = NvdSmartSyncOnePhoto(m_hNvd, &m_faceInfo);
			if (ret)
			{
				MessageBox(_T("ͬ��������Ϣ����"), _T("DEV"), MB_ICONERROR);
				continue;
			}

			sprintf(msg, "������� ������ %d, �Ѿ���� %d, ���� %d%%", count, i + 1, (i + 1) * 100 / count);
			this->SetDlgItemTextA(IDC_STATIC_SYNC_FACE_LIB_PROGRESS, msg);
		}

		sprintf(msg, "������� ������ %d, �Ѿ���� %d, ���� %d%%", count, count, 100);
		this->SetDlgItemTextA(IDC_STATIC_SYNC_FACE_LIB_PROGRESS, msg);

		free(idList);
		return;
	}

    // no idList file 

	CFileFind finder;
    TCHAR cFilePath[1024];
	char *extPos = NULL;
    faceFilePath.Format(_T(strDir));
	BOOL bWorking = finder.FindFile(faceFilePath + _T("\\*.bin"));

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}

		if (finder.IsDirectory())	//��Ϊ��Ŀ¼
		{
			continue;
		}
		else	//�����ļ�
		{
			faceFilePath = finder.GetFilePath();	//ȫ·��
			id = 0;
            _tcscpy_s(cFilePath, faceFilePath.GetLength() + 1, faceFilePath);
			extPos = strrchr(cFilePath, '\\');
			if (!extPos)
			{
				continue;
			}
			extPos++;
			id = atoi(extPos);
			if (0 == id)
			{
				continue;
			}
			count++;

			memset(&m_faceInfo, 0, sizeof(m_faceInfo));
			if (FALSE == file.Open(faceFilePath, CFile::modeRead, NULL))
			{
				//MessageBox(_T("��ȡ������Ϣ����"), _T("DEV"), MB_ICONERROR);
				continue;
			}

			length = file.Read(&m_faceInfo, sizeof(m_faceInfo));
			file.Close();
			
			if (length <= 0 || length >= sizeof(m_faceInfo))
			{
				MessageBox(_T("�ļ����ȳ���"), _T("DEV"), MB_ICONERROR);
				continue;
			}

			ret = NvdSmartSyncOnePhoto(m_hNvd, &m_faceInfo);
			if (ret)
			{
				MessageBox(_T("ͬ��������Ϣ����"), _T("DEV"), MB_ICONERROR);
				continue;
			}

			sprintf(msg, "������� ���Ѿ���� %d", count);
			this->SetDlgItemTextA(IDC_STATIC_SYNC_FACE_LIB_PROGRESS, msg);
		}
	}

	return;
}


void AddPhotoDlg::OnBnClickedButtonImportFromDir()
{
	// TODO: Add your control notification handler code here
	CString csvPath, faceFilePath;
    char szDir[MAX_PATH];
    BROWSEINFO bi; 
	char strDir[MAX_PATH];
	int count = 0, ret = 0, i;
	CFile file;
    TCHAR szFilePath[256 + 1]; 
	char msg[256];
	int length = 0;
	int id;

    bi.hwndOwner = (HWND)this->m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szDir;
    bi.lpszTitle = "ѡ��������ƬĿ¼��";
    bi.iImage = 0;
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
    bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)strDir;

    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if (!lp || FALSE == SHGetPathFromIDList(lp,szDir))
    {
		return;
	}
	strcpy(strDir, szDir);

    sprintf(szFilePath, "%s\\����������Ϣ.csv", strDir);
	csvPath.Format(_T(szFilePath));
	file.Open(csvPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
	strcpy(msg, "����,�Ա�,����,ʡ��,����,֤������,֤����,�Զ���,������Ƭ�ļ���\r\n");
	file.Write(msg, strlen(msg));


	CFileFind finder;
    TCHAR cFilePath[1024];
	char *extPos = NULL;
    faceFilePath.Format(_T(strDir));
	BOOL bWorking = finder.FindFile(faceFilePath + _T("\\*.jp*g"));

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}

		if (finder.IsDirectory())	//��Ϊ��Ŀ¼
		{
			continue;
		}
		else	//�����ļ�
		{
			char name[1024];
			char *pos = NULL;

			faceFilePath = finder.GetFilePath();	//ȫ·��
			id = 0;
            _tcscpy_s(cFilePath, faceFilePath.GetLength() + 1, faceFilePath);
			extPos = strrchr(cFilePath, '\\');
			if (!extPos)
			{
				continue;
			}
			extPos++;
			strncpy(name, extPos, sizeof(name) - 1);
			name[sizeof(name) - 1] = '\0';
			pos = strrchr(name, '.');
			if (pos)
			{
				*pos = '\0';
			}
			//sprintf(msg, "name%d,��,2010/01/01,����,����,����֤,123456789012345678,none,%s\r\n", ++count, extPos);
			sprintf(msg, "%s,��,2010/01/01,����,����,����֤,123456789012345678,none,%s\r\n", name, extPos);
			count++;
	        file.Write(msg, strlen(msg));
		}
	}
	file.Close();

	if (0 == count)
	{
		MessageBox("û����Ƭ�ļ�", _T("DEV"), MB_ICONERROR);
		return;
	}

	//CFileDialog dlg(TRUE,"csv","*.csv", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"CSV�ļ�(*.csv)",NULL);   

    //if(dlg.DoModal()==IDOK)   
    {   
		char failedItems[1024] = {0};
		int total = 0, success = 0;
		int nRet = 0;
    	m_csvPath = csvPath;

		m_isBatchImporting = TRUE;
		m_batchImportProgThread = CreateThread( NULL, 64*1024, batchImportProgThread, this, 0, 0 );
		if ( NULL == m_batchImportProgThread )
		{
			sprintf(msg, "�����߳�ʧ��");
			MessageBox(msg, _T("DEV"), MB_ICONERROR);
			return;
		}

		m_batchImportThread = CreateThread( NULL, 64*1024, batchImportThread, this, 0, 0 );
		if ( NULL == m_batchImportThread )
		{
			sprintf(msg, "�����߳�ʧ��");
			MessageBox(msg, _T("DEV"), MB_ICONERROR);
			return;
		}

		return;
	}   
}

