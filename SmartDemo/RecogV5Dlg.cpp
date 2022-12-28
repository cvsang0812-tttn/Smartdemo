// RecogV5Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "RecogV5Dlg.h"


// RecogV5Dlg dialog

IMPLEMENT_DYNAMIC(RecogV5Dlg, CDialog)

RecogV5Dlg::RecogV5Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(RecogV5Dlg::IDD, pParent)
	, m_SimilarityThreshold(0)
	, m_DetThreshold(0)
{

}

RecogV5Dlg::~RecogV5Dlg()
{
}

void RecogV5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIMI_V5_EDIT, m_SimilarityThreshold);
	DDX_Text(pDX, IDC_DET_V5_EDIT, m_DetThreshold);
}


BEGIN_MESSAGE_MAP(RecogV5Dlg, CDialog)
END_MESSAGE_MAP()


// RecogV5Dlg message handlers
void RecogV5Dlg::LoadData(NvdHandle hNvd)
{
	int nRet;
	CString str;

	nRet = NvdSmartRecogParamGet(hNvd, &pParam);
	if (nRet != NVD_OK)
	{
		MessageBox(_T("获取失败"), _T("DEV"), MB_ICONERROR);
		return;
	}
#ifdef DPU_COMM_STRUCT_V5
	str.Format(_T("%d"), pParam.detThreshold);
	SetDlgItemText(IDC_DET_V5_EDIT, str);

	str.Format(_T("%d"), pParam.similarityThreshold);
	SetDlgItemText(IDC_SIMI_V5_EDIT, str);
#endif
	UpdateData(TRUE);
}

void RecogV5Dlg::DlgSaveData(NvdHandle hNvd)
{
	int nRet;

	UpdateData(TRUE);
#ifdef DPU_COMM_STRUCT_V5
	pParam.similarityThreshold = m_SimilarityThreshold;
	pParam.detThreshold = m_DetThreshold;
#endif
	nRet = NvdSmartRecogParamSet(hNvd, &pParam);
	if (nRet != NVD_OK)
	{
		MessageBox(_T("设置失败"), _T("DEV"), MB_ICONERROR);
	}
	MessageBox(_T("设置成功"), _T("DEV"), MB_ICONERROR);
}
