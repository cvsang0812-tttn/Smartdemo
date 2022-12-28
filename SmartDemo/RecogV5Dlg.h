#pragma once
#include "SDK.h"
#include "afxwin.h"

// RecogV5Dlg dialog

class RecogV5Dlg : public CDialog
{
	DECLARE_DYNAMIC(RecogV5Dlg)

public:
	RecogV5Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~RecogV5Dlg();

// Dialog Data
	enum { IDD = IDD_RECOG_V5_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void LoadData(NvdHandle hNvd);
	void DlgSaveData(NvdHandle hNvd);

	CmFaceSmartFrameInfo pParam;

	int m_SimilarityThreshold;
	int m_DetThreshold;
};
