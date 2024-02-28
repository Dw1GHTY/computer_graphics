
// GDI_2018View.h : interface of the CGDI2018View class
//
#include "DImage.h"
#pragma once


class CGDI2018View : public CView
{
protected: // create from serialization only
	CGDI2018View() noexcept;
	DECLARE_DYNCREATE(CGDI2018View)

// Attributes
public:
	CGDI2018Doc* GetDocument() const;

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
	virtual ~CGDI2018View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	DImage* bager;
	DImage* arm1;
	DImage* arm2;
	DImage* pozadina;
	HENHMETAFILE viljuska;
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawCoord(CDC* pDC);
	void DrawBody(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawFork(CDC* pDC);
	void DrawExcavator(CDC* pDC);
	float arm1AngleBig;
	float arm2Angle;
	float forkAngle;
	float bagerPomeraj;
};

#ifndef _DEBUG  // debug version in GDI_2018View.cpp
inline CGDI2018Doc* CGDI2018View::GetDocument() const
   { return reinterpret_cast<CGDI2018Doc*>(m_pDocument); }
#endif

