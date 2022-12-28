// VideoWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SmartDemo.h"
#include "VideoWnd.h"
#include "VideoDlg.h"

// CVideoWnd dialog

IMPLEMENT_DYNAMIC(CVideoWnd, CDialog)

CVideoWnd::CVideoWnd(int nNumber, CWnd* pParent /*=NULL*/)
	: CDialog(CVideoWnd::IDD, pParent)
{
	m_nCurrentNum = nNumber;
	m_bMaxFlag = FALSE;
	m_color = UNACTIVECOLOR;
	m_hMainHwnd = pParent;
}

CVideoWnd::~CVideoWnd()
{
}

void CVideoWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoWnd, CDialog)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CVideoWnd message handlers
void CVideoWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bMaxFlag = !m_bMaxFlag;
	//((VideoDlg *)AfxGetMainWnd())->SubWndLButtonDblClk(m_nCurrentNum);
	((VideoDlg *)m_hMainHwnd)->SubWndLButtonDblClk(m_nCurrentNum);

	CDialog::OnLButtonDblClk(nFlags, point);
}


void CVideoWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	//((VideoDlg *)AfxGetMainWnd())->SubWndRButtonDown(m_nCurrentNum);
	((VideoDlg *)m_hMainHwnd)->SubWndRButtonDown(m_nCurrentNum);

	CDialog::OnRButtonDown(nFlags, point);
}


void CVideoWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//((VideoDlg *)AfxGetMainWnd())->SubWndLButtonDown(m_nCurrentNum);
	((VideoDlg *)m_hMainHwnd)->SubWndLButtonDown(m_nCurrentNum);

	CDialog::OnLButtonDown(nFlags, point);
}


void CVideoWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	UpdateInterface(&dc);
}


void CVideoWnd::UpdateInterface(CPaintDC *pDC)
{
	CRect rc;
	GetWindowRect(&rc);
	ScreenToClient(&rc);

	CBrush brush;

	CBrush *pBrush = NULL;
	CGdiObject *pGDIObject = NULL;
	CPen   *pPen = NULL;

	brush.CreateSolidBrush(RGB(100, 100, 100));
	if (0)
	{

		pGDIObject = pDC->SelectStockObject(NULL_BRUSH);

	}
	else
	{
		pBrush = pDC->SelectObject(&brush);
	}

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_color);

	pPen = pDC->SelectObject(&pen);

	pDC->Rectangle(rc);

	// 如果设备场景原刷子句柄不为空的话，则
	if (NULL != pBrush)
	{
		pDC->SelectObject(pBrush);
	}
	else
	{
		// 如果设备场景原刷子句柄不为空的话，则
		if (NULL != pGDIObject)
		{
			pDC->SelectObject(pGDIObject);
		}
	}

	// 如果设备场景原画笔句柄不为空的话，则
	if (NULL != pPen)
	{
		pDC->SelectObject(pPen);
	}

	brush.DeleteObject();
	pen.DeleteObject();
}



BOOL CVideoWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (IDCANCEL == wParam)
	{
		//((VideoDlg *)m_hMainHwnd)->OnCancel();
		return TRUE;	
	}

	return CDialog::OnCommand(wParam, lParam);
}

BOOL CVideoWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}