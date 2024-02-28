
// GDI_2018View.cpp : implementation of the CGDI2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI_2018.h"
#endif

#include "GDI_2018Doc.h"
#include "GDI_2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define toRAD 0.0174533

#include "DImage.h"

// CGDI2018View

IMPLEMENT_DYNCREATE(CGDI2018View, CView)

BEGIN_MESSAGE_MAP(CGDI2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGDI2018View construction/destruction

CGDI2018View::CGDI2018View() noexcept
{
	// TODO: add construction code here
	this->arm1AngleBig = 0;
	this->arm2Angle = -90 * toRAD;
	this->forkAngle = 0;
	this->bagerPomeraj = 0;
	bager = new DImage();
	bager->Load(CString("./res/bager.png"));
	arm1 = new DImage();
	arm1->Load(CString("./res/arm1.png"));
	arm2 = new DImage();
	arm2->Load(CString("./res/arm2.png"));
	pozadina = new DImage();
	pozadina->Load(CString("./res/pozadina.png"));

	viljuska = GetEnhMetaFile(CString("./res/viljuska.emf"));
}
CGDI2018View::~CGDI2018View()
{
	delete bager;
	delete arm1;
	delete arm2;
	delete pozadina;
	DeleteEnhMetaFile(viljuska);
}

BOOL CGDI2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


#pragma region transformations
void CGDI2018View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM Xform;

	Xform.eM11 = 1.;
	Xform.eM12 = 0.;
	Xform.eM21 = 0.;
	Xform.eM22 = 1.;
	Xform.eDx = dX;
	Xform.eDy = dY;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&Xform, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&Xform, MWT_LEFTMULTIPLY);
}
void CGDI2018View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM Xform;

	float sinus = sin(angle);
	float cosinus = cos(angle);

	Xform.eM11 = cosinus;
	Xform.eM12 = sinus;
	Xform.eM21 = -sinus;
	Xform.eM22 = cosinus;
	Xform.eDx = 0.;
	Xform.eDy = 0.;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&Xform, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&Xform, MWT_LEFTMULTIPLY);
}
void CGDI2018View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM Xform;

	Xform.eM11 = sX;
	Xform.eM12 = 0.;
	Xform.eM21 = 0.;
	Xform.eM22 = sY;
	Xform.eDx = 0.;
	Xform.eDy = 0.;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&Xform, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&Xform, MWT_LEFTMULTIPLY);
}
#pragma endregion
void CGDI2018View::DrawCoord(CDC* pDC)
{
	CPen* redPen = new CPen(PS_SOLID, 4, RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(redPen);

	pDC->MoveTo(-50, 0);
	pDC->LineTo(250, 0);
	pDC->MoveTo(0, -50);
	pDC->LineTo(0, 250);
}
void CGDI2018View::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	int w = rect.Width();
	int h = rect.Height();

	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), CRect(0, 0, rect.Width(), rect.Height()));

}
void CGDI2018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CBitmap* bmpImage = pImage->GetBitmap();

	BITMAP bm;
	bmpImage->GetBitmap(&bm);

	CBitmap bmpMask;
	bmpMask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);	//monohromatska bitmapa

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* pOldSrcBmp = srcDC->SelectObject(bmpImage);
	CBitmap* pOldDstBmp = dstDC->SelectObject(&bmpMask);

	/* moze i ovako:
	dstDC->SelectObject(&bmpMask);
	srcDC->SelectObject(bmpImage);*/

	COLORREF clrTopLeft = srcDC->GetPixel(0, 0);
	COLORREF clrSaveBk = srcDC->SetBkColor(clrTopLeft);
	dstDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, srcDC, 0, 0, SRCCOPY);	//kompletna maska; ovo je OR

	//Cuvanje originalne bitmape:
	CDC* tmp = new CDC();
	tmp->CreateCompatibleDC(pDC);
	CBitmap tmpBmp;
	tmpBmp.CreateCompatibleBitmap(srcDC, bm.bmWidth, bm.bmHeight);
	tmp->SelectObject(&tmpBmp);
	tmp->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, srcDC, 0, 0, SRCCOPY);	//cuvam tako sto iskopiram src u tmp
	//-----------------------------

	COLORREF clrSaveDstText = srcDC->SetTextColor(RGB(255, 255, 255));	//figura -> 1
	srcDC->SetBkColor(RGB(0, 0, 0));									//sve ostalo -> 0
	srcDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dstDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, srcDC, 0, 0, SRCPAINT);
	
	srcDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, tmp, 0, 0, SRCAND);

	delete tmp;
	delete srcDC;
	delete dstDC;
}

void CGDI2018View::DrawBody(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);

	Translate(pDC, 3 * rect.Width() / 4, rect.Height() / 2 + 100, false);	//doveden bager
	DrawImgTransparent(pDC, bager);

	//namestanje koor sistema za ruku:
	Translate(pDC, 0, bager->Height() / 2 + 80, false);
	Rotate(pDC, -90 * toRAD, false);

}

void CGDI2018View::DrawArm1(CDC* pDC)
{
	//u sustini ne mora da se pamti jer cela ruka zavisi od najvece ruke, a najveca od bagera
	XFORM XoldForm;
	pDC->GetWorldTransform(&XoldForm);	//sada je iz DrawBody koor sistem

	//tacka rotacije oko velikog kruga (58, 61):
	Translate(pDC, 58, 61, false);
	Rotate(pDC, this->arm1AngleBig, false);
	Translate(pDC, -58, -61, false);
	

	DrawImgTransparent(pDC, arm1);
}

void CGDI2018View::DrawArm2(CDC* pDC)
{
	//centar manjeg kruga vece ruke (309, 61) + korigovanje
	Translate(pDC, 309 - 35, 61 - 40, false);

	//tacka rotacije oko veceg kruga:
	Translate(pDC, 36, 40, false);
	Rotate(pDC, arm2Angle, false);
	Translate(pDC, -36, -40, false);

	
	DrawImgTransparent(pDC, arm2);
	//DrawCoord(pDC);

}

void CGDI2018View::DrawFork(CDC* pDC)
{
	

	/*Translate(pDC, 14, 20, false);
	DrawCoord(pDC);
	Rotate(pDC, this->forkAngle, false);
	Translate(pDC, -14, -20, false);
	DrawCoord(pDC);*/

	//pozicioniranje:
	Translate(pDC, arm2->Width() - 28, arm2->Height() / 2  - 15, false);
	
	//centar rotacije:
	Translate(pDC, 14, 20, false);
	Rotate(pDC, forkAngle, false);
	Translate(pDC, -14, -20, false);


	ENHMETAHEADER emfHeader;
	GetEnhMetaFileHeader(viljuska, sizeof(ENHMETAHEADER), &emfHeader);

	int w = emfHeader.rclBounds.right - emfHeader.rclBounds.left;
	int h = emfHeader.rclBounds.bottom - emfHeader.rclBounds.top;

	float factor = 2.5;

	
	pDC->PlayMetaFile(viljuska, CRect(
		emfHeader.rclBounds.left * factor,
		emfHeader.rclBounds.top * factor,
		emfHeader.rclBounds.right * factor,
		emfHeader.rclBounds.bottom * factor));

	

	
	
}


void CGDI2018View::DrawExcavator(CDC* pDC)
{
	Translate(pDC, bagerPomeraj, 0, false);
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
}


// CGDI2018View drawing
void CGDI2018View::OnDraw(CDC* pDC)
{
	CGDI2018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	
	XFORM XoldForm;
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SetGraphicsMode(GM_ADVANCED);


	
	memDC->GetWorldTransform(&XoldForm);

	CRect client;
	GetClientRect(&client);

	CBitmap* memBMP = new CBitmap();
	memBMP->CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	memDC->SelectObject(memBMP);

	
	DrawBackground(memDC);
	DrawExcavator(memDC);


	memDC->SetWorldTransform(&XoldForm);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	delete memDC;
}


// CGDI2018View printing

BOOL CGDI2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDI2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDI2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDI2018View diagnostics

#ifdef _DEBUG
void CGDI2018View::AssertValid() const
{
	CView::AssertValid();
}

void CGDI2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDI2018Doc* CGDI2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDI2018Doc)));
	return (CGDI2018Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDI2018View message handlers

void CGDI2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 87) 
	{
		this->arm1AngleBig += 10 * toRAD;
		Invalidate();
	}
	if (nChar == 83)
	{
		this->arm1AngleBig -= 10 * toRAD;
		Invalidate();
	}
	if (nChar == 81)
	{
		this->arm2Angle += 10 * toRAD;
		Invalidate();
	}
	if (nChar == 65)
	{
		this->arm2Angle -= 10 * toRAD;
		Invalidate();
	}
	if (nChar == 68)
	{
		this->forkAngle += 10 * toRAD;
		Invalidate();
	}
	if (nChar == 69)
	{
		this->forkAngle -= 10 * toRAD;
		Invalidate();
	}
	if (nChar == VK_LEFT)
	{
		this->bagerPomeraj -= 10;
		Invalidate();
	}
	if (nChar == VK_RIGHT)
	{
		this->bagerPomeraj += 10;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CGDI2018View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}




