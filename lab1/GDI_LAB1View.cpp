
// GDI_LAB1View.cpp : implementation of the CGDILAB1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI_LAB1.h"
#endif

#include "GDI_LAB1Doc.h"
#include "GDI_LAB1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGDILAB1View

IMPLEMENT_DYNCREATE(CGDILAB1View, CView)

BEGIN_MESSAGE_MAP(CGDILAB1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGDILAB1View construction/destruction

CGDILAB1View::CGDILAB1View() noexcept
{
	this->gridBlock = 25;	//500px slika => 500/20 = 25px;
	this->gridOn = false;
}

CGDILAB1View::~CGDILAB1View()
{
}

BOOL CGDILAB1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDILAB1View drawing

void CGDILAB1View::OnDraw(CDC* pDC)
{
	CGDILAB1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	//===============BRUSHES===============
	CBrush* greenBrush = new CBrush(RGB(50, 205, 50));
	CBrush* yellowBrush = new CBrush(RGB(255, 255, 0));
	CBrush* blueDiagCrossBrush = new CBrush(HS_DIAGCROSS, RGB(0, 0, 255));
	CBrush* orangeBrush = new CBrush(RGB(255, 153, 51));
	CBrush* redBrush = new CBrush(RGB(255, 0, 0));
	CBrush* pinkBrush = new CBrush(RGB(255, 153, 204));
	CBrush* purpleBrush = new CBrush(RGB(153, 0, 204));
	CBrush* grayBrush = new CBrush(RGB(221, 221, 221));
	CBrush* nullBrush = new CBrush(TBS_TRANSPARENTBKGND);

	//==============PENS==================
	CPen* greenPen = new CPen(PS_SOLID, 4, RGB(0, 255, 0));
	CPen* whitePen = new CPen(PS_SOLID, 1, RGB(238, 238, 238));
	CPen* transparentPen = new CPen(PS_NULL, 1, RGB(0, 0, 0));


	//===========POINTS=============
	POINT whiteBlueTriangle[3]
	{
		CPoint(8 * this->gridBlock + 12, 7 * this->gridBlock),
		CPoint(14 * this->gridBlock + 12, 7 * this->gridBlock),
		CPoint(14 * this->gridBlock + 12, this->gridBlock)
	};
	POINT greenTriangle[3]
	{
		CPoint(5 * this->gridBlock + 12, 10 * this->gridBlock),
		CPoint(8 * this->gridBlock + 12, 13 * this->gridBlock),
		CPoint(8 * this->gridBlock + 12, 7 * this->gridBlock)
	};
	POINT rombus[4]
	{
		CPoint(8 * this->gridBlock + 12, 7 * this->gridBlock),
		CPoint(11 * this->gridBlock + 12, 10 * this->gridBlock),
		CPoint(14 * this->gridBlock + 12, 10 * this->gridBlock),
		CPoint(11 * this->gridBlock + 14, 7 * this->gridBlock)
	};
	POINT orangeTriangle[3]
	{
		CPoint(11 * this->gridBlock + 14, 7 * this->gridBlock),
		CPoint(14 * this->gridBlock + 12, 10 * this->gridBlock),
		CPoint(14 * this->gridBlock + 12, 7 * this->gridBlock)
	};
	POINT redTriangle[3]
	{
		CPoint(11 * this->gridBlock + 12, 10 * this->gridBlock),
		CPoint(11 * this->gridBlock + 12, 13 * this->gridBlock),
		CPoint(8 * this->gridBlock + 12, 13 * this->gridBlock)
	};
	POINT pinkRect[4]
	{
		CPoint(11 * this->gridBlock + 12, 10 * this->gridBlock),
		CPoint(11 * this->gridBlock + 12, 13 * this->gridBlock),	
		CPoint(14 * this->gridBlock + 12, 13 * this->gridBlock),
		CPoint(14 * this->gridBlock + 12, 10 * this->gridBlock)
	};
	POINT purpleTriangle[3]
	{
		CPoint(8 * this->gridBlock + 12, 13 * this->gridBlock),
		CPoint(14 * this->gridBlock + 12, 13 * this->gridBlock),
		CPoint(14 * this->gridBlock + 12, 19 * this->gridBlock)
	};

	//=============MAIN_SHAPES============
	CBrush* oldBrush = pDC->SelectObject(grayBrush);
	CRect* grayRectangle = new CRect(0, 0, 20 * gridBlock, 20 * gridBlock);

	pDC->Rectangle(grayRectangle);
	pDC->SelectObject(oldBrush);

	pDC->SelectObject(greenPen);
	pDC->SelectObject(blueDiagCrossBrush);
	pDC->Polygon(whiteBlueTriangle, 3);
	
	pDC->SelectObject(greenBrush);
	pDC->Polygon(greenTriangle, 3);
	
	pDC->SelectObject(yellowBrush);
	pDC->Polygon(rombus, 4);
	
	pDC->SelectObject(orangeBrush);
	pDC->Polygon(orangeTriangle, 3);
	
	pDC->SelectObject(redBrush);
	pDC->Polygon(redTriangle, 3);
	
	pDC->SelectObject(pinkBrush);
	pDC->Polygon(pinkRect, 4);
	
	pDC->SelectObject(purpleBrush);
	pDC->Polygon(purpleTriangle, 3);

	CPen* oldPen = pDC->SelectObject(greenPen);
	pDC->SelectObject(nullBrush);
	DrawRegularPolygon(pDC, 5, 5, 3, 5, 0);

	pDC->SelectObject(whitePen);

	//===========GRID====================
	if (this->gridOn)
	{

		for (int x = 0; x < this->gridBlock * 20; x += this->gridBlock)
		{
			pDC->MoveTo(x, 0);
			pDC->LineTo(x, 500);

			pDC->MoveTo(0, x);
			pDC->LineTo(500, x);

		}
	}



	
}


// CGDILAB1View printing

BOOL CGDILAB1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDILAB1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDILAB1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDILAB1View diagnostics

#ifdef _DEBUG
void CGDILAB1View::AssertValid() const
{
	CView::AssertValid();
}

void CGDILAB1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDILAB1Doc* CGDILAB1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDILAB1Doc)));
	return (CGDILAB1Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDILAB1View message handlers

void CGDILAB1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 32)
	{
		this->gridOn = !this->gridOn;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CGDILAB1View::DrawRegularPolygon(CDC* pDC,
										double cx, 
										double cy, 
										double r,
										int n,
										float rotAngle)
{
	// TODO: Add your implementation code here.

	double angle = rotAngle;
	double angleIncrement = 2 * 3.14159265358979323846 / n;

	double minRadius = r * this->gridBlock;
	CPoint* mnogougao = new CPoint[n];

	for (int i = 0; i < n; i++) 
	{
		double coordX = (int)(minRadius * cos(angle)) + (double)(cx * this->gridBlock);
		double coordY = (int)(minRadius * sin(angle)) + (double)(cy * this->gridBlock);
		mnogougao[i] = CPoint(coordX, coordY);
		angle += angleIncrement;
	}
	pDC->Polygon(mnogougao, n);

}
