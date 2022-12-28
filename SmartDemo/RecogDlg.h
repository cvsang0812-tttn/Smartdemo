#pragma once
#include "SDK.h"

// RecogDlg dialog

class RecogDlg : public CDialog
{
	DECLARE_DYNAMIC(RecogDlg)

public:
	RecogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~RecogDlg();

// Dialog Data
	enum { IDD = IDD_RECOG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_valueSpeed;
	int m_valueSnapCount;
	int m_valuePolicy;
	int m_valueSnapInterval;
	int m_valueSnapThreshold;
	int m_valueCompThreshold;

	void LoadData(NvdHandle hNvd);
	void DlgSaveData(NvdHandle hNvd);
};
