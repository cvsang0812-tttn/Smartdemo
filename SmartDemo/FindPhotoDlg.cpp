// FindPhotoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "FindPhotoDlg.h"


// FindPhotoDlg dialog

IMPLEMENT_DYNAMIC(FindPhotoDlg, CDialog)

FindPhotoDlg::FindPhotoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FindPhotoDlg::IDD, pParent)
{

}

FindPhotoDlg::~FindPhotoDlg()
{
}

void FindPhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FIND_FACE_PROV, m_ctrlProvince);
	DDX_Control(pDX, IDC_COMBO_FIND_FACE_CITY, m_ctrlCity);
	DDX_Control(pDX, IDC_COMBO_FIND_FACE_CERTTYPE, m_ctrlCertType);
	DDX_Control(pDX, IDC_EDIT_FIND_FACE_NAME, m_ctrlName);
	DDX_Control(pDX, IDC_COMBO_FIND_FACE_GENDER, m_ctrlGender);
	DDX_Control(pDX, IDC_EDIT_FIND_FACE_CERT, m_ctrlCertNumber);
	DDX_Control(pDX, IDC_DATETIMEPICKER_FIND_FACE_BEGIN, m_ctrlBirthDayBegin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_FIND_FACE_END, m_ctrlBirthDayEnd);
	DDX_Control(pDX, IDC_LIST_FIND_FACE, m_ctrlFaceList);
}


BEGIN_MESSAGE_MAP(FindPhotoDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_FIND_FACE_PROV, &FindPhotoDlg::OnCbnSelchangeComboFindFaceProv)
	ON_BN_CLICKED(IDC_BTN_FIND_FACE_FIND, &FindPhotoDlg::OnBnClickedBtnFindFaceFind)
	ON_BN_CLICKED(IDC_BTN_FIND_FACE_RESET, &FindPhotoDlg::OnBnClickedBtnFindFaceReset)
	ON_BN_CLICKED(IDC_BTN_FACE_DEL, &FindPhotoDlg::OnBnClickedBtnFaceDel)
	ON_BN_CLICKED(IDC_BTN_VIEW_FACE, &FindPhotoDlg::OnBnClickedBtnViewFace)
	ON_BN_CLICKED(IDC_BTN_FACE_DEL_ALL, &FindPhotoDlg::OnBnClickedBtnFaceDelAll)
END_MESSAGE_MAP()


// FindPhotoDlg message handlers

void FindPhotoDlg::LoadData(NvdHandle hNvd)
{
	char *province;
	char *city;
	int count = 0;
	int i;
	int id = 0;
	int nIndex = 0;
	CString str;

	m_hNvd = hNvd;

	m_ctrlGender.ResetContent();
	m_ctrlProvince.ResetContent();
	m_ctrlCity.ResetContent();

	nIndex = m_ctrlGender.AddString(_T("无限制"));
	m_ctrlGender.SetItemData(nIndex, 0);
	nIndex = m_ctrlGender.AddString(_T("男"));
	m_ctrlGender.SetItemData(nIndex, 1);
	nIndex = m_ctrlGender.AddString(_T("女"));
	m_ctrlGender.SetItemData(nIndex, 2);
	m_ctrlGender.SetCurSel(0);

	NvdSmartGetProvinceList(hNvd, &province, &count);
	for (i = 0; i < count; i++)
	{
		str.Format(_T("%s"), province+(i*16));
		nIndex = m_ctrlProvince.AddString(str);
		m_ctrlProvince.SetItemData(nIndex, i);
	}
	m_ctrlProvince.SetCurSel(0);


	NvdSmartGetCityList(hNvd, &id, &city, &count);
	for (i = 0; i < count; i++)
	{
		str.Format(_T("%s"), city+(i*16));
		nIndex = m_ctrlCity.AddString(str);
		m_ctrlCity.SetItemData(nIndex, i);
	}
	m_ctrlCity.SetCurSel(0);

	str.Format(_T("无限制"));
	nIndex = m_ctrlCertType.AddString(str);
	m_ctrlCertType.SetItemData(nIndex, 0);
	str.Format(_T("身份证"));
	nIndex = m_ctrlCertType.AddString(str);
	m_ctrlCertType.SetItemData(nIndex, 1);
	str.Format(_T("军官证"));
	nIndex = m_ctrlCertType.AddString(str);
	m_ctrlCertType.SetItemData(nIndex, 2);
	m_ctrlCertType.SetCurSel(0);
#if 0
	i = 0;
	m_ctrlFaceList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE);
	m_ctrlFaceList.InsertColumn(i++, _T("序号"), LVCFMT_LEFT, 40);
	m_ctrlFaceList.InsertColumn(i++, _T("姓名"), LVCFMT_LEFT, 80);
	m_ctrlFaceList.InsertColumn(i++, _T("性别"), LVCFMT_LEFT, 40);
	m_ctrlFaceList.InsertColumn(i++, _T("生日"), LVCFMT_LEFT, 100);
	m_ctrlFaceList.InsertColumn(i++, _T("籍贯"), LVCFMT_LEFT, 120);
	m_ctrlFaceList.InsertColumn(i++, _T("证件类型"), LVCFMT_LEFT, 80);
	m_ctrlFaceList.InsertColumn(i++, _T("证件号"), LVCFMT_LEFT, 200);
	m_ctrlFaceList.InsertColumn(i++, _T("ID"), LVCFMT_LEFT, 40);
#endif
}

void FindPhotoDlg::OnCbnSelchangeComboFindFaceProv()
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

void FindPhotoDlg::OnBnClickedBtnFindFaceFind()
{
	// TODO: Add your control notification handler code here
	FaceQueryReq  cond;
	CString value;
	CTime time;
	CString certNumber;
	char birthday[16] = {0};
	int count = 0, ret = 0;
	FacePhotoDesc *faces = NULL;

	memset(&cond, 0, sizeof(cond));

	cond.name.comparison = FACE_PHOTO_QUERY_OP_CONTAIN;
	m_ctrlName.GetWindowTextA(value);
	strcpy((char *)cond.name.value, value);
	cond.gender.comparison = FACE_PHOTO_QUERY_OP_EQUAL;
	cond.gender.value = (unsigned char)m_ctrlGender.GetItemData(m_ctrlGender.GetCurSel());
	cond.province.comparison = FACE_PHOTO_QUERY_OP_EQUAL;
	cond.province.value = (unsigned char)m_ctrlProvince.GetItemData(m_ctrlProvince.GetCurSel());
	cond.city.comparison = FACE_PHOTO_QUERY_OP_EQUAL;
	cond.city.value = (unsigned char)m_ctrlCity.GetItemData(m_ctrlCity.GetCurSel());
	cond.certType.comparison = FACE_PHOTO_QUERY_OP_EQUAL;
	cond.certType.value = (unsigned char)m_ctrlCertType.GetItemData(m_ctrlCertType.GetCurSel());
	cond.certNumber.comparison = FACE_PHOTO_QUERY_OP_CONTAIN;
	m_ctrlCertNumber.GetWindowTextA(value);
	strcpy((char *)cond.certNumber.value, value);
	cond.beginBirth.comparison = FACE_PHOTO_QUERY_OP_EQUAL_BIG;
	m_ctrlBirthDayBegin.GetTime(time);
	sprintf(birthday, "%04d-%02d-%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	strcpy((char *)cond.beginBirth.value, birthday);
	cond.endBirth.comparison = FACE_PHOTO_QUERY_OP_EQUAL_LESS;
	m_ctrlBirthDayEnd.GetTime(time);
	sprintf(birthday, "%04d-%02d-%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	strcpy((char *)cond.endBirth.value, birthday);

	cond.limit = 10000;
	faces = (FacePhotoDesc *)malloc(cond.limit * sizeof(FacePhotoDesc));

	ret = NvdSmartQueryFaceLib(m_hNvd, &cond, &count, &faces);
	if (ret)
	{
		MessageBox(_T("查询人脸库出错"), _T("DEV"), MB_ICONERROR);
		if (faces)
		{
			free(faces);
		}
		return;
	}

    m_ctrlFaceList.DeleteAllItems();
	int i, j;
	FacePhotoDescStr descStr;
	for(i = 0; i < count; i++)
	{
		j = 0;
		memset(&descStr, 0, sizeof(descStr));
		NvdSmartConvertFaceDesc(m_hNvd, &faces[i], &descStr);

		value.Format(_T("%d"), i+1);
		m_ctrlFaceList.InsertItem(i, value);
		value.Format(_T("%d"), i+1);	//序号
		m_ctrlFaceList.SetItemText(i, j++, value);
		value.Format(_T("%s"), descStr.name);	//姓名
		m_ctrlFaceList.SetItemText(i, j++, value);
		value.Format(_T("%s"), descStr.gender);	//性别
		m_ctrlFaceList.SetItemText(i, j++, value);
		value.Format(_T("%s"), descStr.birth);	//生日
		m_ctrlFaceList.SetItemText(i, j++, value);
		value.Format(_T("%s-%s"), descStr.province, descStr.city);	//籍贯
		m_ctrlFaceList.SetItemText(i, j++, value);
		value.Format(_T("%s"), descStr.certType);	//证件类型
		m_ctrlFaceList.SetItemText(i, j++, value);
		value.Format(_T("%s"), descStr.certNumber);	//证件号
		m_ctrlFaceList.SetItemText(i, j++, value);
		value.Format(_T("%d"), faces[i].id);	//ID
		m_ctrlFaceList.SetItemText(i, j++, value);
	}
	
	
	if (faces)
	{
		free(faces);
	}
	return;
}

void FindPhotoDlg::OnBnClickedBtnFindFaceReset()
{
    m_ctrlFaceList.DeleteAllItems();
}

void FindPhotoDlg::OnBnClickedBtnFaceDel()
{
	CString idStr;
	unsigned int id = 0;
	int ret = 0;

	POSITION pos = m_ctrlFaceList.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}

	int nItem = m_ctrlFaceList.GetNextSelectedItem(pos);
	idStr = m_ctrlFaceList.GetItemText(nItem, 7);
	id = atoi(idStr);
	
	if (id > 0)
	{
	    ret = NvdSmartDelOnePhoto(m_hNvd, id);
	    if (ret)
	    {
		    MessageBox(_T("删除人脸出错"), _T("DEV"), MB_ICONERROR);	
	    }
	}
}


void FindPhotoDlg::OnBnClickedBtnViewFace()
{
	CString idStr;
	unsigned int id = 0;
	int ret = 0;

	POSITION pos = m_ctrlFaceList.GetFirstSelectedItemPosition();
	if (NULL == pos)
	{
		return;
	}

	int nItem = m_ctrlFaceList.GetNextSelectedItem(pos);
	idStr = m_ctrlFaceList.GetItemText(nItem, 7);
	id = atoi(idStr);
	
	if (id > 0)
	{
	    m_FaceDetailDlg.setParam(m_hNvd);
	    m_FaceDetailDlg.setFaceId(id);
	    //m_FaceDetailDlg.LoadData();
	    m_FaceDetailDlg.DoModal();
	}
}

BOOL FindPhotoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int	i = 0;
	m_ctrlFaceList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE);
	m_ctrlFaceList.InsertColumn(i++, _T("序号"), LVCFMT_LEFT, 40);
	m_ctrlFaceList.InsertColumn(i++, _T("姓名"), LVCFMT_LEFT, 80);
	m_ctrlFaceList.InsertColumn(i++, _T("性别"), LVCFMT_LEFT, 40);
	m_ctrlFaceList.InsertColumn(i++, _T("生日"), LVCFMT_LEFT, 100);
	m_ctrlFaceList.InsertColumn(i++, _T("籍贯"), LVCFMT_LEFT, 120);
	m_ctrlFaceList.InsertColumn(i++, _T("证件类型"), LVCFMT_LEFT, 80);
	m_ctrlFaceList.InsertColumn(i++, _T("证件号"), LVCFMT_LEFT, 200);
	m_ctrlFaceList.InsertColumn(i++, _T("ID"), LVCFMT_LEFT, 40);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void FindPhotoDlg::OnBnClickedBtnFaceDelAll()
{
	int ret;

	ret = NvdSmartDelAllFaceLibs(m_hNvd);
	if (0 == ret)
	{
		    MessageBox(_T("清空人脸库成功"), _T("DEV"), MB_ICONWARNING);	
	}
	else
	{
		    MessageBox(_T("清空人脸库失败"), _T("DEV"), MB_ICONWARNING);	
	}

}
