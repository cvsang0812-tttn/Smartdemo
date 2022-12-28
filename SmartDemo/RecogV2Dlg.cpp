// RecogV2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "RecogV2Dlg.h"


// RecogV2Dlg dialog

IMPLEMENT_DYNAMIC(RecogV2Dlg, CDialog)

RecogV2Dlg::RecogV2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(RecogV2Dlg::IDD, pParent)
	, m_SimilarityThreshold(0)
	, m_DetThreshold(0)
	, m_ClarityThreshold(0)
	, m_FrontalThreshold(0)
{

}

RecogV2Dlg::~RecogV2Dlg()
{
}

void RecogV2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIMILARITY_EDIT, m_SimilarityThreshold);
	DDX_Text(pDX, IDC_DET_EDIT, m_DetThreshold);
	DDX_Text(pDX, IDC_CLARITY_EDIT, m_ClarityThreshold);
	DDX_Text(pDX, IDC_FRONTAL_EDIT, m_FrontalThreshold);
}


BEGIN_MESSAGE_MAP(RecogV2Dlg, CDialog)
END_MESSAGE_MAP()


// RecogV2Dlg message handlers

void RecogV2Dlg::LoadData(NvdHandle hNvd)
{
	int nRet;
	CString str;

	nRet = NvdSmartRecogParamGet(hNvd, &pParam);
	if (nRet != NVD_OK)
	{
		MessageBox(_T("获取失败"), _T("DEV"), MB_ICONERROR);
		return;
	}
#ifdef DPU_COMM_STRUCT_V2
	str.Format(_T("%d"), pParam.detThreshold);
	SetDlgItemText(IDC_DET_EDIT, str);

	str.Format(_T("%d"), pParam.similarityThreshold);
	SetDlgItemText(IDC_SIMILARITY_EDIT, str);

	str.Format(_T("%d"), pParam.clarityThreshold);
	SetDlgItemText(IDC_CLARITY_EDIT, str);

	str.Format(_T("%d"), pParam.frontalThreshold);
	SetDlgItemText(IDC_FRONTAL_EDIT, str);
#endif
	UpdateData(TRUE);
}

void RecogV2Dlg::DlgSaveData(NvdHandle hNvd)
{
	int nRet;

	UpdateData(TRUE);
#ifdef DPU_COMM_STRUCT_V2
	pParam.similarityThreshold = m_SimilarityThreshold;
	pParam.detThreshold = m_DetThreshold;
	pParam.clarityThreshold = m_ClarityThreshold;
	pParam.frontalThreshold = m_FrontalThreshold;
#endif
	nRet = NvdSmartRecogParamSet(hNvd, &pParam);
	if (nRet != NVD_OK)
	{
		MessageBox(_T("设置失败"), _T("DEV"), MB_ICONERROR);
	}
	MessageBox(_T("设置成功"), _T("DEV"), MB_ICONERROR);
}
