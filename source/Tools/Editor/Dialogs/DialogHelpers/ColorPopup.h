#ifndef COLORPOPUP_INCLUDED
#define COLORPOPUP_INCLUDED
#pragma once 

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// ColorPopup.h : header file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.
// It's free - so you get what you pay for.


// CColorPopup messages
#define CPN_SELCHANGE        WM_USER + 1001        // Color Picker Selection change
#define CPN_DROPDOWN         WM_USER + 1002        // Color Picker drop down
#define CPN_CLOSEUP          WM_USER + 1003        // Color Picker close up
#define CPN_SELENDOK         WM_USER + 1004        // Color Picker end OK
#define CPN_SELENDCANCEL     WM_USER + 1005        // Color Picker end (cancelled)

// forward declaration
class CColorPicker;

// To hold the colours and their names
typedef struct {
    COLORREF crColor;
    TCHAR    *szName;
} ColorTableEntry;

/////////////////////////////////////////////////////////////////////////////
// CColorPopup window

class CColorPopup : public CWnd
{
// Construction
public:
    CColorPopup();
    CColorPopup(CPoint p, COLORREF crColor, CWnd* pParentWnd,
                 LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL);
    void Initialise();

// Attributes
public:

// Operations
public:
    BOOL Create(CPoint p, COLORREF crColor, CWnd* pParentWnd, 
                LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorPopup)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CColorPopup();

protected:
    BOOL GetCellRect(int nIndex, const LPRECT& rect);
    void FindCellFromColor(COLORREF crColor);
    void SetWindowSize();
    void CreateToolTips();
    void ChangeSelection(int nIndex);
    void EndSelection(int nMessage);
    void DrawCell(CDC* pDC, int nIndex);

    COLORREF GetColor(int nIndex)              { return m_crColors[nIndex].crColor; }
    LPCTSTR GetColorName(int nIndex)           { return m_crColors[nIndex].szName; }
    int  GetIndex(int row, int col) const;
    int  GetRow(int nIndex) const;
    int  GetColumn(int nIndex) const;

// protected attributes
protected:
    static ColorTableEntry m_crColors[];
    int            m_nNumColors;
    int            m_nNumColumns, m_nNumRows;
    int            m_nBoxSize, m_nMargin;
    int            m_nCurrentSel;
    int            m_nChosenColorSel;
    CString        m_strDefaultText;
    CString        m_strCustomText;
    CRect          m_CustomTextRect, m_DefaultTextRect, m_WindowRect;
    CFont          m_Font;
    CPalette       m_Palette;
    COLORREF       m_crInitialColor, m_crColor;
    CToolTipCtrl   m_ToolTip;
    CWnd*          m_pParent;

    BOOL           m_bChildWindowVisible;

    // Generated message map functions
protected:
    //{{AFX_MSG(CColorPopup)
    afx_msg void OnNcDestroy();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	// KROUER: make it compatible with VC6
	#if _MFC_VER < 0x0700
	afx_msg void OnActivateApp(BOOL bActive, HTASK dwThreadID) ;
	#else
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	#endif
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORPOPUP_H__D0B75902_9830_11D1_9C0F_00A0243D1382__INCLUDED_)
