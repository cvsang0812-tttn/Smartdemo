// ParaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "ParaDlg.h"


// ParaDlg dialog

IMPLEMENT_DYNAMIC(ParaDlg, CDialog)

ParaDlg::ParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ParaDlg::IDD, pParent)
{
}

ParaDlg::~ParaDlg()
{
}

void ParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PARAM, m_ctrlTab);
}


BEGIN_MESSAGE_MAP(ParaDlg, CDialog)
	ON_BN_CLICKED(IDOK, &ParaDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PARAM, &ParaDlg::OnTcnSelchangeTabParam)
END_MESSAGE_MAP()


// ParaDlg message handlers

BOOL ParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ctrlTab.InsertItem(0, _T(""));
	m_ctrlTab.InsertItem(1, _T("Create"));
	m_ctrlTab.InsertItem(2, _T(""));
	m_ctrlTab.InsertItem(3, _T("Search"));
	m_ctrlTab.InsertItem(4, _T(""));

	m_ctrlRecogDlg.Create(IDD_RECOG_DIALOG, &m_ctrlTab);
	m_ctrlRecogV2Dlg.Create(IDD_RECOG_V2_DIALOG, &m_ctrlTab);
	m_ctrlRecogV5Dlg.Create(IDD_RECOG_V5_DIALOG, &m_ctrlTab);
	m_ctrlAddPhotoDlg.Create(IDD_ADDPHOTO_DIALOG, &m_ctrlTab);
	m_ctrlFindPhotoDlg.Create(IDD_FINDPHOTO_DIALOG, &m_ctrlTab);
	m_ctrlFindRecogDlg.Create(IDD_FINDRECOG_DIALOG, &m_ctrlTab);
	m_ctrlAlarmLinkageDlg.Create(IDD_ALARMLINKAGE_DIALOG, &m_ctrlTab);

	CRect rc;
	m_ctrlTab.GetClientRect(rc);
	rc.top += 20;
	m_ctrlRecogDlg.MoveWindow(&rc);
	m_ctrlRecogV2Dlg.MoveWindow(&rc);
	m_ctrlRecogV5Dlg.MoveWindow(&rc);
	m_ctrlAddPhotoDlg.MoveWindow(&rc);
	m_ctrlFindPhotoDlg.MoveWindow(&rc);
	m_ctrlFindRecogDlg.MoveWindow(&rc);
	m_ctrlAlarmLinkageDlg.MoveWindow(&rc);

#if defined(DPU_COMM_STRUCT_V5)
	pDialog[0] = &m_ctrlRecogV5Dlg;
#elif defined(DPU_COMM_STRUCT_V2)
	pDialog[0] = &m_ctrlRecogV2Dlg;
#else
	pDialog[0] = &m_ctrlRecogDlg;
#endif
	pDialog[1] = &m_ctrlAddPhotoDlg;
	pDialog[2] = &m_ctrlFindPhotoDlg;
	pDialog[3] = &m_ctrlFindRecogDlg;
	pDialog[4] = &m_ctrlAlarmLinkageDlg;

	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	pDialog[2]->ShowWindow(SW_HIDE);
	pDialog[3]->ShowWindow(SW_HIDE);
	pDialog[4]->ShowWindow(SW_HIDE);

#if defined(DPU_COMM_STRUCT_V5)
	m_ctrlRecogV5Dlg.LoadData(m_hDev);
#elif defined(DPU_COMM_STRUCT_V2)
	m_ctrlRecogV2Dlg.LoadData(m_hDev);
#else
	m_ctrlRecogDlg.LoadData(m_hDev);
#endif
	
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ParaDlg::setParam(NvdHandle hDev)
{
	m_hDev = hDev;
}

void ParaDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_CurSelTab = m_ctrlTab.GetCurSel();
	switch (m_CurSelTab)
	{
	case 0:
#ifdef DPU_COMM_STRUCT_V2
		((RecogV2Dlg *)pDialog[m_CurSelTab])->DlgSaveData(m_hDev);
#else
		((RecogDlg *)pDialog[m_CurSelTab])->DlgSaveData(m_hDev);
#endif
		break;
	case 1:
		((AddPhotoDlg *)pDialog[m_CurSelTab])->DlgSaveData(m_hDev);
		break;
	case 4:
		((AlarmLinkageDlg *)pDialog[m_CurSelTab])->DlgSaveData(m_hDev);
		break;
	case 2:
	case 3:
	default:
		break;
	}
	//OnOK();
}

void ParaDlg::OnTcnSelchangeTabParam(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	m_CurSelTab = m_ctrlTab.GetCurSel();
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);

	switch (m_CurSelTab)
	{
	case 0:
#if defined(DPU_COMM_STRUCT_V5)
		m_ctrlRecogV5Dlg.LoadData(m_hDev);
#elif defined(DPU_COMM_STRUCT_V2)
		m_ctrlRecogV2Dlg.LoadData(m_hDev);
#else
		m_ctrlRecogDlg.LoadData(m_hDev);
#endif
		break;
	case 1:
		m_ctrlAddPhotoDlg.LoadData(m_hDev);
		break;
	case 2:
		m_ctrlFindPhotoDlg.LoadData(m_hDev);
		break;
	case 3:
		m_ctrlFindRecogDlg.LoadData(m_hDev);
		break;
	case 4:
		m_ctrlAlarmLinkageDlg.LoadData(m_hDev);
		break;
	default:
		break;
	}
	*pResult = 0;
}
