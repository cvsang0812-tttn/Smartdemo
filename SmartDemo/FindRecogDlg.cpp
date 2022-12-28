// FindRecogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "FindRecogDlg.h"
#include "sdk.h"

// FindRecogDlg dialog

IMPLEMENT_DYNAMIC(FindRecogDlg, CDialog)

FindRecogDlg::FindRecogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FindRecogDlg::IDD, pParent)
{
	isInited = 0;
    m_result = NULL;
    m_length = 0;
	m_isExporting = FALSE;
	memset(m_items, 0, sizeof(m_items));
	m_total = 0;
}

FindRecogDlg::~FindRecogDlg()
{
	if (m_result)
	{
		free(m_result);
	}
}

void FindRecogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_CtrlDateBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_CtrlTimeBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_ctrlDateEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_ctrlTimeEnd);
	DDX_Control(pDX, IDC_LISTFIND_RECOG, m_ctrlListFindRecog);
	DDX_Control(pDX, IDC_CHECK_FINDRECOG_USE_THRESH, m_ctrlCheckThresh);
}


BEGIN_MESSAGE_MAP(FindRecogDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FINDRECOG, &FindRecogDlg::OnBnClickedButtonFindrecog)
	ON_BN_CLICKED(IDC_BUTTON_FIND_RECOG_DETAIL, &FindRecogDlg::OnBnClickedButtonFindRecogDetail)
	ON_BN_CLICKED(IDC_BTN_EXPORT_RECOG_HTML, &FindRecogDlg::OnBnClickedBtnExportRecogHtml)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ALL_RECOG, &FindRecogDlg::OnBnClickedButtonDelAllRecog)
END_MESSAGE_MAP()


// FindRecogDlg message handlers
void FindRecogDlg::LoadData(NvdHandle hNvd)
{
#if 0
	if (0 == isInited)
	{
		m_hNvd = hNvd;

		int i = 0;
		m_ctrlListFindRecog.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE);
		m_ctrlListFindRecog.InsertColumn(i++, _T("Number"), LVCFMT_LEFT, 80);
		m_ctrlListFindRecog.InsertColumn(i++, _T("Time"), LVCFMT_LEFT, 200);
		m_ctrlListFindRecog.InsertColumn(i++, _T("Percent"), LVCFMT_LEFT, 80);
		m_ctrlListFindRecog.InsertColumn(i++, _T("Name"), LVCFMT_LEFT, 200);
		isInited = 1;
	}
#else
	m_hNvd = hNvd;
#endif
}

void FindRecogDlg::OnBnClickedButtonFindrecog()
{
	// TODO: Add your control notification handler code here
	FaceRecogSearchCond cond;
	CTime beginDate, beginTime, endDate, endTime;
	unsigned long long beginPts64, endPts64;
	int ret = 0;

	m_CtrlDateBegin.GetTime(beginDate);
    m_CtrlTimeBegin.GetTime(beginTime);
	CTime beginPts(beginDate.GetYear(), beginDate.GetMonth(), beginDate.GetDay(), beginTime.GetHour(), beginTime.GetMinute(), beginTime.GetSecond());
	beginPts64 = beginPts.GetTime() * 1000000;

	m_ctrlDateEnd.GetTime(endDate);
    m_ctrlTimeEnd.GetTime(endTime);
	CTime endPts(endDate.GetYear(), endDate.GetMonth(), endDate.GetDay(), endTime.GetHour(), endTime.GetMinute(), endTime.GetSecond());
	endPts64 = endPts.GetTime() * 1000000;

	if (beginPts64 >= endPts64)
	{
		MessageBox(_T("Note"), _T("DEV"), MB_ICONERROR);
		return;
	}

	memset(&cond, 0, sizeof(cond));
	cond.startPts64 = beginPts64;
	cond.endPts64 = endPts64;
	cond.limit = 1000;
	cond.order = 0;
	if (BST_CHECKED == m_ctrlCheckThresh.GetCheck())
	{
	    cond.isUsingThreshold = 1;
	}
	else
	{
	    cond.isUsingThreshold = 0;
	}
	cond.isNeedJpgContent = 1;

	if (m_result)
	{
		free(m_result);
	}

	m_length = cond.limit * 40 * 1024;
	m_result = (unsigned char *)malloc(m_length); //假设每张抓拍照不超过40K字节
	ret = NvdSmartQueryRecogResult(m_hNvd, &cond, m_result, m_length);
	if (ret)
	{
		MessageBox(_T("Note"), _T("DEV"), MB_ICONERROR);
		return;
	}

	if (FALSE == ParseQueryResult())
	{
		MessageBox(_T("Note"), _T("DEV"), MB_ICONERROR);
		return;
	}

    m_ctrlListFindRecog.DeleteAllItems();
	int i, j;
	CString value;
    FacePhotoDescStr descStr;


	for(i = 0; i < m_total; i++)
	{
		j = 0;

		value.Format(_T("%d"), i+1);
		m_ctrlListFindRecog.InsertItem(i, value);

		value.Format(_T("%d"), i+1);	//序号
		m_ctrlListFindRecog.SetItemText(i, j++, value);

		CTime snapTime(m_items[i]->pts64/1000000);
		//value.Format(_T("%llu"), m_items[i]->pts64);	//抓拍时间
		value.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), 
			snapTime.GetYear(), snapTime.GetMonth(), snapTime.GetDay(), snapTime.GetHour(), snapTime.GetMinute(), snapTime.GetSecond());	//抓拍时间
		m_ctrlListFindRecog.SetItemText(i, j++, value);

		value.Format(_T("%d%%"), m_items[i]->simiList[0].similarity);	//相似度
		m_ctrlListFindRecog.SetItemText(i, j++, value);

		if (m_items[i]->simiList[0].matchId > 0)
		{
          	NvdSmartConvertFaceDesc(m_hNvd, & m_items[i]->simiList[0].desc, &descStr);
		    value.Format(_T("%s"),  descStr.name);	//姓名
		    m_ctrlListFindRecog.SetItemText(i, j++, value);
		}
		else
		{
		    value.Format(_T("%s"),  "未知");	//姓名
		    m_ctrlListFindRecog.SetItemText(i, j++, value);
		}
	}
	
	return;
}

BOOL FindRecogDlg::ParseQueryResult()
{
	memset(m_items, 0, sizeof(m_items));
	m_total = 0;

	if (!m_result || m_length <= 8)
	{
		return FALSE;
	}

	memcpy(&m_total, m_result, sizeof(int));
	
	unsigned char *pos = m_result + 8;
	FaceRecogTopNSimi *item = NULL;
	int count = 0, i;

    for (i = 0; i < m_total; i++)
	{
		m_items[i] = (FaceRecogTopNSimi *)pos;
		pos += sizeof(FaceRecogTopNSimi);
		pos += m_items[i]->pathDetectJpgSize;
	}

	return TRUE;
}

void FindRecogDlg::OnBnClickedButtonFindRecogDetail()
{
	CString idStr;
	unsigned int id = 0;
	int ret = 0;

	POSITION pos = m_ctrlListFindRecog.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}

	int nItem = m_ctrlListFindRecog.GetNextSelectedItem(pos);

	if (nItem >= 0 && nItem < m_total)
	{
	    m_RecogDetailDlg.setParam(m_hNvd, m_items[nItem]);
	    m_RecogDetailDlg.DoModal();
	}
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

static FaceNetPhotoDescVector faceDesc;
DWORD WINAPI 
exportRecogToHtmlThread( LPVOID lpvoid )
{
	int nRet = 0, total = 0, finished = 0;
	FindRecogDlg *dlg = (FindRecogDlg *)lpvoid;
    NvdHandle nvdHandle = dlg->m_hNvd;
	CString htmlPath, jpgPath;
	int count = 0, ret = 0, i;
	CFile htmlFile, jpgFile;
    TCHAR szFilePath[256 + 1]; 
	char msg[256];
	int length = 0, size = 0;
	int id;
	char *htmlHead = "<html>\n<head>\n<meta charset=\"GBK\">\n<title>人脸识别记录</title>\n</head>\n<body>\n";
	char *htmlTail = "</body>\n</html>\n";
    char *htmlBodyTemplate = "<div style=\"width:1430px; margin:auto\">\n<div style=\"background-color:#FFA500;text-align:center\">\n<b>识别记录</b>\n</div>\n<div style=\"background-color:#EEEEEE;height:350px;width:300px;float:left;\">\n<img src=\"%s\" alt=\"抓拍人脸\" height=\"350\" width=\"300\">\n</div>\n<div style=\"background-color:#EEEEEE;height:350px;width:30px;float:left;\">\n</div>\n<div style=\"background-color:#EEEEEE;height:350px;width:300px;float:left;\">\n<img src=\"%s\" alt=\"最佳匹配人脸\" height=\"350\" width=\"300\">\n</div>\n<div style=\"background-color:#EEEEEE;height:350px;width:20px;float:left;\">\n</div>\n<div style=\"background-color:#EEEEEE;height:350px;width:100px;float:left;\">\n<img src=\"%s\" alt=\"第二匹配人脸\" height=\"87\" width=\"80\"><br>\n<img src=\"%s\" alt=\"第三匹配人脸\" height=\"87\" width=\"80\"><br>\n<img src=\"%s\" alt=\"第四匹配人脸\" height=\"87\" width=\"80\"><br>\n<img src=\"%s\" alt=\"第五匹配人脸\" height=\"87\" width=\"80\">\n</div>\n<div style=\"background-color:#EEEEEE;height:350px;width:30px;float:left;\">\n</div>\n<div style=\"background-color:#EEEEEE;height:350px;width:650px;float:left;\">\n<img src=\"%s\" alt=\"抓拍全景图\" height=\"350\" width=\"650\">\n</div>\n<div style=\"background-color:#8A9DA8;clear:both\">\n<br>\n抓拍时间:&nbsp&nbsp%s&nbsp%s <br>\n姓名:%s&nbsp&nbsp&nbsp&nbsp性别:%s&nbsp&nbsp&nbsp&nbsp省份:%s&nbsp&nbsp&nbsp&nbsp城市:%s<br>\n生日:%s&nbsp&nbsp&nbsp&nbsp证件类型:%s&nbsp&nbsp&nbsp&nbsp证件号:%s&nbsp&nbsp&nbsp&nbsp比对值:%d<br>\n</div>\n<div style=\"background-color:#FFA500;clear:both;text-align:center; height:10px\">\n</div>\n</div><br><br>";
    char htmlBody[2048];
	char snapPath[MAX_PATH], facePath[5][MAX_PATH], scenePath[MAX_PATH];
	char *snapFileName = NULL, *faceFileName[5] = {0}, *sceneFileName = NULL;
	char yymmdd[32], hhmmss[32], name[128],	gender[16],	provice[32], city[32], birthday[32], certType[32], certNum[64];
	int simi = 0;
    FaceRecogTopNSimi *recogInfo = NULL;


	sprintf(szFilePath, "%s\\人脸识别记录.html", dlg->m_exportDir);
	htmlPath.Format(_T(szFilePath));
	htmlFile.Open(htmlPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
	// 写入html头
	htmlFile.Write(htmlHead, strlen(htmlHead));

	while(dlg->m_isExporting && finished < dlg->m_total)
	{
        unsigned char *snap = NULL; 
		static unsigned long long lastPts = 0;
		int seqInPts = 1; 
	 
		snapPath[0] = facePath[0][0] = facePath[1][0] = facePath[2][0] = facePath[3][0] = facePath[4][0] = scenePath[0] = '\0';
	    yymmdd[0] = hhmmss[0] = name[0] = gender[0] = provice[0] = city[0] = birthday[0] = certType[0] = certNum[0] = '\0';

		recogInfo = dlg->m_items[finished];
		if (lastPts != recogInfo->pts64)
		{
			lastPts = recogInfo->pts64;
			seqInPts = 1;
		}
		else
		{
			seqInPts++;
		}
        finished++;
		if (recogInfo->pathDetectJpgSize > 0)
		{
			snap = (unsigned char *)recogInfo + sizeof(FaceRecogTopNSimi);
		    sprintf(snapPath, "%s\\snap_%llu_%d.jpg", dlg->m_exportDir, recogInfo->pts64, seqInPts);
			jpgPath.Format(_T(snapPath));
			jpgFile.Open(jpgPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
			jpgFile.Write(snap, recogInfo->pathDetectJpgSize);
			jpgFile.Close();
			snapFileName = strrchr(snapPath, '\\');
			if (snapFileName)
			{
			    snapFileName++;
			}
			else
			{
				snapFileName = snapPath;
			}
		}
		
		for (i = 0; i < 5; i++)
		{
			faceFileName[i] = facePath[i];
			id = recogInfo->simiList[i].matchId;
			if (id <= 0)
			{
				strcpy(facePath[i], "no image");
				continue;
			}
		    sprintf(facePath[i], "%s\\face_id_%d.jpg", dlg->m_exportDir, id);
			jpgPath.Format(_T(facePath[i]));
			if (TRUE == jpgFile.Open(jpgPath, CFile::modeRead, NULL))
			{
				faceFileName[i] = strrchr(facePath[i], '\\');
				if (faceFileName)
				{
					faceFileName[i]++;
				}
				else
				{
					faceFileName[i] = facePath[i];
				}
     			jpgFile.Close();
				continue;
			}

			memset(&faceDesc, 0, sizeof(faceDesc));
			nRet = NvdSmartGetOnePhoto(nvdHandle, id, &faceDesc);
			if (nRet)
			{
				continue;
			}
			jpgFile.Open(jpgPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
			jpgFile.Write(faceDesc.photo, faceDesc.photoSize);
			jpgFile.Close();
			faceFileName[i] = strrchr(facePath[i], '\\');
			if (faceFileName)
			{
			    faceFileName[i]++;
			}
			else
			{
				faceFileName[i] = facePath[i];
			}
		}

		nRet = NvdSmartGetSceneSnapByTime(nvdHandle, recogInfo->pts64, dlg->m_scene, sizeof(dlg->m_scene));
		if (nRet)
		{
			continue;
		}
		memcpy(&size, dlg->m_scene, sizeof(int));
	    sprintf(scenePath, "%s\\scene_%llu.jpg", dlg->m_exportDir, recogInfo->pts64);
		jpgPath.Format(_T(scenePath));
		jpgFile.Open(jpgPath, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate, NULL);
		jpgFile.Write(dlg->m_scene + 4, size);
		jpgFile.Close();
		sceneFileName = strrchr(scenePath, '\\');
		if (sceneFileName)
		{
		    sceneFileName++;
		}
		else
		{
			sceneFileName = scenePath;
		}

		CTime snapTime(recogInfo->pts64/1000000);
		sprintf(yymmdd, "%04d-%02d-%02d", snapTime.GetYear(), snapTime.GetMonth(), snapTime.GetDay());
		sprintf(hhmmss, "%02d:%02d:%02d", snapTime.GetHour(), snapTime.GetMinute(), snapTime.GetSecond());

		memset(&faceDesc, 0, sizeof(faceDesc));
		nRet = NvdSmartGetOnePhoto(nvdHandle, recogInfo->simiList[0].matchId, &faceDesc);
		if (nRet)
		{
			continue;
		}

		FacePhotoDescStr descStr;
		nRet = NvdSmartConvertFaceDesc(nvdHandle, &faceDesc.desc, &descStr);
		simi = recogInfo->simiList[0].similarity;

        sprintf(htmlBody, htmlBodyTemplate,
			    snapFileName, //snapPath,
				faceFileName[0], //facePath[0],
				faceFileName[1], //facePath[1],
				faceFileName[2], //facePath[2],
				faceFileName[3], //facePath[3],
				faceFileName[4], //facePath[4],
				sceneFileName, //scenePath,
				yymmdd,
                hhmmss,
				descStr.name,
				descStr.gender,
				descStr.province,
				descStr.city,
				descStr.birth,
				descStr.certType,
				descStr.certNumber,
				simi
				);
	    htmlFile.Write(htmlBody, strlen(htmlBody));
		if (0 == nRet)
		{
			sprintf(msg, "Note %d, %d", dlg->m_total, finished);
			dlg->SetDlgItemTextA(IDC_STATIC_EXPORT_RECOG_PROGRESS, msg);
		}

		//Sleep(1000);
	}

    // 写入html尾
	htmlFile.Write(htmlTail, strlen(htmlTail));
    htmlFile.Close();

	dlg->m_isExporting = FALSE;

	return 0;
}

void FindRecogDlg::OnBnClickedBtnExportRecogHtml()
{
	if (0 == m_total || !m_result)
	{
		return;
	}

    char szDir[MAX_PATH];
    BROWSEINFO bi; 
	char msg[256];

    bi.hwndOwner = (HWND)this->m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szDir;
    bi.lpszTitle = "Note";
    bi.iImage = 0;
    bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
    bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)m_exportDir;

    LPITEMIDLIST lp = SHBrowseForFolder(&bi);
    if (!lp || FALSE == SHGetPathFromIDList(lp,szDir))
    {
		return;
	}
	strcpy(m_exportDir, szDir);
    m_isExporting = TRUE;

	m_exportThread = CreateThread( NULL, 64*1024, exportRecogToHtmlThread, this, 0, 0 );
	if ( NULL == m_exportThread )
	{
		sprintf(msg, "创建线程失败");
		MessageBox(msg, _T("DEV"), MB_ICONERROR);
		return;
	}
}

BOOL FindRecogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int i = 0;
	m_ctrlListFindRecog.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE);
	m_ctrlListFindRecog.InsertColumn(i++, _T("Number"), LVCFMT_LEFT, 80);
	m_ctrlListFindRecog.InsertColumn(i++, _T("Time"), LVCFMT_LEFT, 200);
	m_ctrlListFindRecog.InsertColumn(i++, _T("Percent"), LVCFMT_LEFT, 80);
	m_ctrlListFindRecog.InsertColumn(i++, _T("Name"), LVCFMT_LEFT, 200);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void FindRecogDlg::OnBnClickedButtonDelAllRecog()
{
	int ret;

	ret = NvdSmartDelAllRecogResults(m_hNvd);
	if (0 == ret)
	{
		    MessageBox(_T("Note"), _T("DEV"), MB_ICONWARNING);	
	}
	else
	{
		    MessageBox(_T("Note"), _T("DEV"), MB_ICONWARNING);	
	}
}
