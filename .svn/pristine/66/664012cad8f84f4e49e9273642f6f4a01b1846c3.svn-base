#pragma once
#include "SDK.h"
#include "afxwin.h"

// RecogV2Dlg dialog

class RecogV2Dlg : public CDialog
{
	DECLARE_DYNAMIC(RecogV2Dlg)

public:
	RecogV2Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~RecogV2Dlg();

// Dialog Data
	enum { IDD = IDD_RECOG_V2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void LoadData(NvdHandle hNvd);
	void DlgSaveData(NvdHandle hNvd);

	int m_SimilarityThreshold;
	CmFaceSmartFrameInfo pParam;
	int m_DetThreshold;
	int m_ClarityThreshold;
	int m_FrontalThreshold;
};
