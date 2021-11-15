#if !defined(AFX_PALETTED_H__6213F722_B292_11D1_BE3E_444553540000__INCLUDED_)
#define AFX_PALETTED_H__6213F722_B292_11D1_BE3E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PaletteD.h : header file
//

CBrush* BrushFromColorAndPalette(CDC *pDC, COLORREF color);

/////////////////////////////////////////////////////////////////////////////
// CPaletteDialog dialog

class CPaletteDialog : public CDialog
{
// Construction
public:
	CPaletteDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPaletteDialog)
	enum { IDD = IDD_PALETTE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaletteDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

CTurtleView* m_view;	// turtle View
int m_step;				// distance among origin of boxes
int m_side;				// size of color box
int m_offset;			// x and y offset of box 0
int m_index;			// index of selected color
int	m_nEntries;			// number of colors in palette
//CButton m_buttons[256];	// color buttons

int PointToIndex(CPoint point);
CPoint IndexToOrigin(int index);
void DrawBox (CPaintDC* pDC, int index, COLORREF color, BOOL isSelected);
//CButton* ButtonCreate (CRect rect, COLORREF color);

	// Generated message map functions
	//{{AFX_MSG(CPaletteDialog)
	afx_msg void OnPaint();
	afx_msg void OnPen();
	afx_msg void OnFill();
	afx_msg void OnBk();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTED_H__6213F722_B292_11D1_BE3E_444553540000__INCLUDED_)
