#pragma once
#include "afxdtctl.h"
#include "SDK.h"
#include "afxcmn.h"
#include "RecogDetailDlg.h"
#include "afxwin.h"

// FindRecogDlg dialog

class FindRecogDlg : public CDialog
{
	DECLARE_DYNAMIC(FindRecogDlg)

public:
	FindRecogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~FindRecogDlg();

// Dialog Data
	enum { IDD = IDD_FINDRECOG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int isInited;
	NvdHandle m_hNvd;
	unsigned char *m_result;
	unsigned int m_length;
	FaceRecogTopNSimi *m_items[10000]; 
	unsigned int  m_total;
	RecogDetailDlg m_RecogDetailDlg;
    unsigned char m_scene[2 * 1024 * 1024];
	BOOL m_isExporting;
	HANDLE m_exportThread;
	char m_exportDir[MAX_PATH];
	afx_msg void OnBnClickedButtonFindrecog();
	CDateTimeCtrl m_CtrlDateBegin;
	CDateTimeCtrl m_CtrlTimeBegin;
	CDateTimeCtrl m_ctrlDateEnd;
	CDateTimeCtrl m_ctrlTimeEnd;
	void LoadData(NvdHandle hNvd);
	afx_msg void OnBnClickedButtonFindRecogDetail();
	int ParseQueryResult();
	CListCtrl m_ctrlListFindRecog;
	CButton m_ctrlCheckThresh;
	afx_msg void OnBnClickedBtnExportRecogHtml();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDelAllRecog();
};
