// AlarmLinkageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "AlarmLinkageDlg.h"


// AlarmLinkageDlg dialog

IMPLEMENT_DYNAMIC(AlarmLinkageDlg, CDialog)

AlarmLinkageDlg::AlarmLinkageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AlarmLinkageDlg::IDD, pParent)
{

}

AlarmLinkageDlg::~AlarmLinkageDlg()
{
}

void AlarmLinkageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ALARM1, m_ctrlAlarm1);
	DDX_Control(pDX, IDC_CHECK_ALARM2, m_ctrlAlarm2);
	DDX_Control(pDX, IDC_CHECK_ALARMSOUND, m_ctrlAlarmSound);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN1, m_ctrlStartTime1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN2, m_ctrlStartTime2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN3, m_ctrlStartTime3);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN4, m_ctrlStartTime4);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN5, m_ctrlStartTime5);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN6, m_ctrlStartTime6);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BEGIN7, m_ctrlStartTime7);
}


BEGIN_MESSAGE_MAP(AlarmLinkageDlg, CDialog)
END_MESSAGE_MAP()


// AlarmLinkageDlg message handlers

void AlarmLinkageDlg::DlgSaveData(NvdHandle hNvd)
{
	int nRet;
	int i;
	SmartAlarmLinkageCfg pParam;
	CTime tm;

	for (i = 0; i < WEEK_DATE; i++)
	{
		((CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_BEGIN1+i*2))->GetTime(tm);
		pParam.timeArea[i].bTime = tm.GetHour() * 60 + tm.GetMinute();
		if (pParam.timeArea[i].bTime == 1439)
		{
			pParam.timeArea[i].bTime += 1;
		}
		((CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_END1+i*2))->GetTime(tm);
		pParam.timeArea[i].eTime = tm.GetHour() * 60 + tm.GetMinute();
		if (pParam.timeArea[i].eTime == 1439)
		{
			pParam.timeArea[i].eTime += 1;
		}
	}
	
	pParam.alarm1 = m_ctrlAlarm1.GetCheck();
	pParam.alarm2 = m_ctrlAlarm2.GetCheck();
	pParam.alarmSound = m_ctrlAlarmSound.GetCheck();

	nRet = NvdSmartSetAlarmLinkage(hNvd, &pParam);
	if (nRet != NVD_OK)
	{
		MessageBox(_T("…Ë÷√ ß∞‹"), _T("DEV"), MB_ICONERROR);
		return;
	}
}

void AlarmLinkageDlg::LoadData(NvdHandle hNvd)
{
	int nRet;
	int i;
	SmartAlarmLinkageCfg pParam;
	CTime tm;

	nRet = NvdSmartGetAlarmLinkage(hNvd, &pParam);
	if (nRet != NVD_OK)
	{
		MessageBox(_T("ªÒ»° ß∞‹"), _T("DEV"), MB_ICONERROR);
		return;
	}

	m_ctrlAlarm1.SetCheck(pParam.alarm1);
	m_ctrlAlarm2.SetCheck(pParam.alarm2);
	m_ctrlAlarmSound.SetCheck(pParam.alarmSound);

	for (i = 0; i < WEEK_DATE; i++)
	{
		if (pParam.timeArea[i].bTime >= 1440)
		{
			pParam.timeArea[i].bTime -= 1;
		}
		if (pParam.timeArea[i].eTime >= 1440)
		{
			pParam.timeArea[i].eTime -= 1;
		}
		tm = CTime(2000, 1, 1, pParam.timeArea[i].bTime/60, pParam.timeArea[i].bTime%60, 0);
		((CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_BEGIN1+i*2))->SetTime(&tm);
		tm = CTime(2000, 1, 1, pParam.timeArea[i].eTime/60, pParam.timeArea[i].eTime%60, 0);
		((CDateTimeCtrl *)GetDlgItem(IDC_DATETIMEPICKER_END1+i*2))->SetTime(&tm);
	}	
}
