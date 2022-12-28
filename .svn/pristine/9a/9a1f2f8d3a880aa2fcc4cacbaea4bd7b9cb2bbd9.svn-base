// RecogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "RecogDlg.h"


// RecogDlg dialog

IMPLEMENT_DYNAMIC(RecogDlg, CDialog)

RecogDlg::RecogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RecogDlg::IDD, pParent)
	, m_valueSpeed(0)
	, m_valueSnapCount(0)
	, m_valuePolicy(0)
	, m_valueSnapInterval(0)
	, m_valueSnapThreshold(0)
	, m_valueCompThreshold(0)
{

}

RecogDlg::~RecogDlg()
{
}

void RecogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_valueSpeed);
	DDX_Text(pDX, IDC_EDIT_SNAPCOUNT, m_valueSnapCount);
	DDX_Text(pDX, IDC_EDIT_POLICY, m_valuePolicy);
	DDX_Text(pDX, IDC_EDIT_SNAPINTERVAL, m_valueSnapInterval);
	DDX_Text(pDX, IDC_EDIT_SNAPTHRESHOLD, m_valueSnapThreshold);
	DDX_Text(pDX, IDC_EDIT_COMPTHRESHOLD, m_valueCompThreshold);
}


BEGIN_MESSAGE_MAP(RecogDlg, CDialog)
END_MESSAGE_MAP()


// RecogDlg message handlers

void RecogDlg::LoadData(NvdHandle hNvd)
{
	CmFaceSmartFrameInfo pParam;
	int nRet;
	CString str;

	nRet = NvdSmartRecogParamGet(hNvd, &pParam);
	if (nRet != NVD_OK)
	{
		MessageBox(_T("获取失败"), _T("DEV"), MB_ICONERROR);
		return;
	}
#if (!(defined(DPU_COMM_STRUCT_V2) || defined(DPU_COMM_STRUCT_V5)))
	str.Format(_T("%d"), pParam.speed);
	SetDlgItemText(IDC_EDIT_SPEED, str);

	str.Format(_T("%d"), pParam.snapCount);
	SetDlgItemText(IDC_EDIT_SNAPCOUNT, str);

	str.Format(_T("%d"), pParam.policy);
	SetDlgItemText(IDC_EDIT_POLICY, str);

	str.Format(_T("%d"), pParam.snapInterval);
	SetDlgItemText(IDC_EDIT_SNAPINTERVAL, str);

	str.Format(_T("%d"), pParam.snapThreshold);
	SetDlgItemText(IDC_EDIT_SNAPTHRESHOLD, str);

	str.Format(_T("%d"), pParam.compThreshold);
	SetDlgItemText(IDC_EDIT_COMPTHRESHOLD, str);
#endif
	UpdateData(TRUE);
}

void RecogDlg::DlgSaveData(NvdHandle hNvd)
{
	int nRet;
	CmFaceSmartFrameInfo pParam;

	UpdateData(TRUE);

#if (!(defined(DPU_COMM_STRUCT_V2) || defined(DPU_COMM_STRUCT_V5)))
	pParam.speed = m_valueSpeed;
	pParam.snapCount = m_valueSnapCount;
	pParam.policy = m_valuePolicy;
	pParam.snapInterval = m_valueSnapInterval;
	pParam.snapThreshold = m_valueSnapThreshold;
	pParam.compThreshold = m_valueCompThreshold;
#endif

	nRet = NvdSmartRecogParamSet(hNvd, &pParam);
	if (nRet != NVD_OK)
	{
		MessageBox(_T("设置失败"), _T("DEV"), MB_ICONERROR);
	}
	MessageBox(_T("设置成功"), _T("DEV"), MB_ICONERROR);
}
