
// GDI_LAB1View.h : interface of the CGDILAB1View class
//

#pragma once


class CGDILAB1View : public CView
{
protected: // create from serialization only
	CGDILAB1View() noexcept;
	DECLARE_DYNCREATE(CGDILAB1View)

// Attributes
public:
	CGDILAB1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDILAB1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	int gridBlock;
	bool gridOn = false;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void DrawRegularPolygon(CDC* pDC, double cx, double cy, double r, int n, float rotAngle);
};

#ifndef _DEBUG  // debug version in GDI_LAB1View.cpp
inline CGDILAB1Doc* CGDILAB1View::GetDocument() const
   { return reinterpret_cast<CGDILAB1Doc*>(m_pDocument); }
#endif

