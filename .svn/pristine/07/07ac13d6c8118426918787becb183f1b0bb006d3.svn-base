#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "SDK.h"

// AlarmLinkageDlg dialog

class AlarmLinkageDlg : public CDialog
{
	DECLARE_DYNAMIC(AlarmLinkageDlg)

public:
	AlarmLinkageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~AlarmLinkageDlg();

// Dialog Data
	enum { IDD = IDD_ALARMLINKAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void DlgSaveData(NvdHandle hNvd);
	void LoadData(NvdHandle hNvd);
	CButton m_ctrlAlarm1;
	CButton m_ctrlAlarm2;
	CButton m_ctrlAlarmSound;
	CDateTimeCtrl m_ctrlStartTime1;
	CDateTimeCtrl m_ctrlStartTime2;
	CDateTimeCtrl m_ctrlStartTime3;
	CDateTimeCtrl m_ctrlStartTime4;
	CDateTimeCtrl m_ctrlStartTime5;
	CDateTimeCtrl m_ctrlStartTime6;
	CDateTimeCtrl m_ctrlStartTime7;
};
