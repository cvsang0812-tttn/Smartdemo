#pragma once
#include "afxcmn.h"
#include "RecogDlg.h"
#include "AddPhotoDlg.h"
#include "FindPhotoDlg.h"
#include "FindRecogDlg.h"
#include "RecogV2Dlg.h"
#include "RecogV5Dlg.h"
#include "AlarmLinkageDlg.h"

// ParaDlg dialog

class ParaDlg : public CDialog
{
	DECLARE_DYNAMIC(ParaDlg)

public:
	ParaDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ParaDlg();

// Dialog Data
	enum { IDD = IDD_PARAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_ctrlTab;
	RecogDlg m_ctrlRecogDlg;
	RecogV2Dlg m_ctrlRecogV2Dlg;
	RecogV5Dlg m_ctrlRecogV5Dlg;
	AddPhotoDlg m_ctrlAddPhotoDlg;
	FindPhotoDlg m_ctrlFindPhotoDlg;
	FindRecogDlg m_ctrlFindRecogDlg;
	AlarmLinkageDlg m_ctrlAlarmLinkageDlg;
	CDialog *pDialog[15];
	NvdHandle m_hDev;
	int m_CurSelTab;

	void setParam(NvdHandle hDev);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabParam(NMHDR *pNMHDR, LRESULT *pResult);
};
