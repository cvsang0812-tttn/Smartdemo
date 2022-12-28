#pragma once


// CVideoWnd dialog

class CVideoWnd : public CDialog
{
	DECLARE_DYNAMIC(CVideoWnd)

public:
	CVideoWnd(int nNumber, CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoWnd();

// Dialog Data
	enum { IDD = IDD_VIDEOWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	enum {ACTIVECOLOR = RGB(255, 0, 0), UNACTIVECOLOR = RGB(0, 128, 128)};
	int m_nCurrentNum;			//��ǰ���ں�
	COLORREF m_color;			//�߿�ɫ
	BOOL m_bMaxFlag;			//��󻯱�ʶ
	//HWND m_hMainHwnd;			//�����ھ��
	CWnd* m_hMainHwnd;			//�����ھ��

private:
	void UpdateInterface(CPaintDC *pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
