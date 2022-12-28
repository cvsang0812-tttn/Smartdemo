#pragma once
#include "SDK.h"
#include "afxwin.h"
#include "afxdtctl.h"

// AddPhotoDlg dialog

class AddPhotoDlg : public CDialog
{
	DECLARE_DYNAMIC(AddPhotoDlg)

public:
	AddPhotoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~AddPhotoDlg();
	 
// Dialog Data
	enum { IDD = IDD_ADDPHOTO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	void DrawImg(CDC * pDC, CString strPath, int x, int y);

public:
	CString m_strFacePath;
	CEdit m_ctrlName;
	CComboBox m_ctrlGender;
	CDateTimeCtrl m_ctrlBirthDay;
	CComboBox m_ctrlProvince;
	CComboBox m_ctrlCity;
	CComboBox m_ctrlCertType;
	CEdit m_ctrlCertNumber;
	CEdit m_ctrlOther;
	NvdHandle m_hNvd;
    HANDLE m_batchImportProgThread;
    HANDLE m_batchImportThread;
	BOOL m_isBatchImporting;
    CString m_csvPath;
	FaceNetPhotoDescVector m_faceInfo;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAddface();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedBtnBatchImport();
	void LoadData(NvdHandle hNvd);
	afx_msg void OnCbnSelchangeComboProvince();
	void DlgSaveData(NvdHandle hNvd);
	afx_msg void OnBnClickedButtonExportFaceLib();
	afx_msg void OnBnClickedButtonSyncFaceLib();
	afx_msg void OnBnClickedButtonImportFromDir();
};
