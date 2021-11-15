// turtleMdl.cpp : implementation of the CTurtleModel class
//
/*	turtleMdl.cpp

991014	aggiunto Clear()
*/

#include "stdafx.h"
#include "turtleMdl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurtleModel

IMPLEMENT_SERIAL(CTurtleModel, CODModel, 0)

CTurtleModel::CTurtleModel()
{
}

CTurtleModel::~CTurtleModel()
{
}

/////////////////////////////////////////////////////////////////////////////
// CTurtleModel serialization

void CTurtleModel::Serialize(CArchive& ar)
{
	CODModel::Serialize(ar);

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTurtleModel diagnostics

#ifdef _DEBUG
void CTurtleModel::AssertValid() const
{
	CODModel::AssertValid();
}

void CTurtleModel::Dump(CDumpContext& dc) const
{
	CODModel::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

// estratto da CODModel::Initialize()
void CTurtleModel::Clear()
{
//	CSize szPrev = GetSize();

	RemoveAllChildren();

	if (m_pBkgndComponent)
	{
		delete m_pBkgndComponent;
		m_pBkgndComponent = NULL;
	}

//	SetSize(odg_sizeDefaultDiagramSize);
//	SetBkgndColor(odg_crDefaultBkgndColor);

	SetDragSource(FALSE);
	SetDragTarget(FALSE);
	SetModifiedFlag(FALSE);

	// Initialize default properties
//	RemoveProperty(OD_PROP_EDIT);
//	CODEditProperties propEdit;
//	propEdit.SetCanContain(TRUE);
//	SetProperty(&propEdit);
//
//	RemoveProperty(OD_PROP_EDIT_LABEL);
//	CODEditProperties labelEditProps(OD_PROP_EDIT_LABEL);
//	labelEditProps.SetCanSelect(TRUE);
//	SetProperty(&labelEditProps);

	// Update observers
	CODUpdateCommand cmdUpdate(this);
	cmdUpdate.SetUpdateType(OD_UPDATE_DIAGRAM_INIT);
	UpdateAllObservers(NULL, &cmdUpdate);

	CPoint ptOrigin = GetOrigin();
	CSize size = GetSize();

	m_bspTree.Create(CRect(ptOrigin.x, ptOrigin.y,
		ptOrigin.x + size.cx, ptOrigin.y + size.cy));

	m_bspTree.AddComponents(GetComponents());

	SetModifiedFlag(FALSE);
}

CODSymbolComponent* SymbolLoad (LPCSTR lpszPath)
{
	CODSymbolComponent* pSym = new(CODSymbolComponent);
	BOOL created = pSym->Create(lpszPath);
	if (created) {
		ULONG nRef = pSym->AddRef();
		return pSym;
	}
	else {
		delete(pSym);
		return NULL;
	}
}

CODSymbolComponent* CTurtleModel::SymbolLoad (LPCSTR lpszPath)
{
#ifdef OLD_SYMBOL
	CODSymbolComponent* pSym = new(CODSymbolComponent);
	BOOL created = pSym->Create(lpszPath);
	if (created) {
		BOOL appended = AppendChild(pSym);
		if (appended)
			return pSym;
	}
	delete(pSym);
	return NULL;
#else // OLD_SYMBOL
	CODSymbolComponent* pSym = ::SymbolLoad (lpszPath);
	if (pSym) {
		BOOL appended = AppendChild(pSym);
		if (! appended) {
			delete(pSym);
			return NULL;
		}
	}
	return pSym;
#endif // OLD_SYMBOL
}


// vedi anche ShowcaseCtlr.cpp
CODLinkComponent* CTurtleModel::NodesConnect(CODSymbolComponent* pSourceComp, CODSymbolComponent* pTargetComp)
{
	CODPortComponent* pSourcePort = pSourceComp->GetCenterPort();
	CODPortComponent* pTargetPort = pTargetComp->GetCenterPort();
	CODLinkComponent* pLinkComp = new(CODLinkComponent);
	BOOL created = pLinkComp->Create(pSourcePort, pTargetPort);
	pLinkComp->SetTargetEndpoint(new CODArrowEndpoint());
	if (created) {
		BOOL appended = AppendChild(pLinkComp);
		if (appended) {
			CODConnection* pSourceConnection = pLinkComp->ConnectTail(pSourcePort);
			CODConnection* pTargetConnection = pLinkComp->ConnectHead(pTargetPort);
			UpdateComponent(pLinkComp, OD_UPDATE_CURRENT_POS);
			return pLinkComp;
		}
	}
	delete(pLinkComp);
	return NULL;
}

void CTurtleModel::EdgeInvert(CODLinkComponent* pLinkComp)
{
	CODSymbolComponent* pSourceComp = (CODSymbolComponent*) pLinkComp->GetTailComponent();
	CODSymbolComponent* pTargetComp = (CODSymbolComponent*) pLinkComp->GetHeadComponent();
	CODPortComponent* pSourcePort = pSourceComp->GetCenterPort();
	CODPortComponent* pTargetPort = pTargetComp->GetCenterPort();

	pLinkComp->DisconnectTail();
	pLinkComp->DisconnectHead();

	CODConnection* pSourceConnection = pLinkComp->ConnectTail(pTargetPort);
	CODConnection* pTargetConnection = pLinkComp->ConnectHead(pSourcePort);

	UpdateComponent(pLinkComp, OD_UPDATE_OLD_AND_NEW_POS);
}

BOOL CTurtleModel::CompRemove(CODComponent* pComp)
{
	BOOL removed = RemoveChild(pComp);
	return removed;
}

CODComponent* CTurtleModel::CompDup(CODComponent* pComp)
{
	CODComponent* pNewComp = pComp->Dup();
	BOOL appended = AppendChild(pNewComp);
	if (appended)
		return pNewComp;
	else {
		delete(pNewComp);
		return NULL;
	}
}

float GetComponentDirection (CODComponent* pComp)
{
	float fScaleX = 1.0, fScaleY = 1.0;
	int nRotation = 0;
	BOOL isFlipped = FALSE;
	CODTransform* pXform = pComp->GetTransform();
	if (pXform) {
		pXform->GetScaling(fScaleX, fScaleY);
		isFlipped = (fScaleX < 0.0) || (fScaleY < 0.0);
		if (pXform->IsRotated()) {
			nRotation = pXform->GetRotation();
			if (isFlipped)
				nRotation += 1800;
		}
		else if (isFlipped)
			nRotation = 1800;
	}
	return ((float) nRotation) / 10;
}

#ifdef NONUSATO

CPoint GetComponentPoint (CODComponent* pComp, int index)
{
	CRect bounds = pComp->GetBounds();
	int x, y;

	switch (index) {
	case 0:		// center
		x = (bounds.left + bounds.right) / 2;
		y = (bounds.top + bounds.bottom) / 2;
		break;
	case 1:		// nord
		x = (bounds.left + bounds.right) / 2;
		y = bounds.top;
		break;
	case 2:		// nord-est
		x = bounds.right;
		y = bounds.top;
		break;
	case 3:		// est
		x = bounds.right;
		y = (bounds.top + bounds.bottom) / 2;
		break;
	case 4:		// sud-est
		x = bounds.right;
		y = bounds.bottom;
		break;
	case 5:		// sud
		x = (bounds.left + bounds.right) / 2;
		y = bounds.bottom;
		break;
	case 6:		// sud-ovest
		x = bounds.left;
		y = bounds.bottom;
		break;
	case 7:		// ovest
		x = bounds.left;
		y = (bounds.top + bounds.bottom) / 2;
		break;
	case 8:		// nord-ovest
		x = bounds.left;
		y = bounds.top;
		break;
		}

	return CPoint(x, y);
}

#endif // NONUSATO

CPoint GetComponentCenter (CODComponent* pComp)
{
//	CRect bounds = pComp->GetBounds();
//	return CPoint((bounds.left + bounds.right) / 2, (bounds.top + bounds.bottom) / 2);
	return pComp->GetControlPoint (OD_CENTER);
}

void ComponentRotate (CODComponent* pComp, float delta, CPoint ptOrigin)
{
	pComp->Rotate(delta * 10,ptOrigin.x, ptOrigin.y);
}
void ComponentRotate (CODComponent* pComp, float delta, int iOrigin)
{
//	CPoint origin = GetComponentPoint (pComp, iOrigin);
	CPoint ptOrigin = pComp->GetControlPoint ((OD_CONTROL_POINT) iOrigin);
	ComponentRotate (pComp, delta, ptOrigin);
}

void SetComponentDirection (CODComponent* pComp, float alfa, CPoint origin)
{
	float fScaleX, fScaleY;
	int nRotation;
	CODTransform* pXform = pComp->GetTransform();
	if (! pXform)
		pXform = new(CODTransform);
	nRotation = 0;
	pXform->GetScaling(fScaleX, fScaleY);
	BOOL isFlipped = (fScaleX < 0.0) || (fScaleY < 0.0);
	if (pXform->IsRotated()) {
		nRotation = pXform->GetRotation();
		if (isFlipped)
			nRotation += 1800;
	}
	else if (isFlipped)
		nRotation = 1800;
	int neededRotation = ((int) alfa * 10) - nRotation;
	if (neededRotation != 0)  {
		pXform->Rotate(neededRotation, origin.x, origin.y);
		pComp->SetTransform(pXform);
		pComp->OnMove();
	}
}
void SetComponentDirection (CODComponent* pComp, float alfa, int iOrigin)
{
//	CPoint origin = GetComponentPoint (pComp, iOrigin);
	CPoint ptOrigin = pComp->GetControlPoint ((OD_CONTROL_POINT) iOrigin);
	SetComponentDirection(pComp, alfa, ptOrigin);
}

void GetComponentScale (CODComponent* pComp, float& fScaleX, float& fScaleY)
{
	fScaleX = 1.0;
	fScaleY = 1.0;
	CODTransform* pXform = pComp->GetTransform();
	if (pXform)
		pXform->GetScaling(fScaleX, fScaleY);
	return;
}

void SetComponentScale (CODComponent* pComp, float fScaleX, float fScaleY, CPoint origin)
{
	CODTransform* pXform = pComp->GetTransform();
	if ((fScaleX == 1.0) && (fScaleY == 1.0) && (! pXform))
		return;
	if (! pXform)
		pXform = new(CODTransform);
	else {
		float fCurrentScaleX, fCurrentScaleY;
		GetComponentScale (pComp, fCurrentScaleX, fCurrentScaleY);
		fScaleX /= fCurrentScaleX;
		fScaleY /= fCurrentScaleY;
	}
	pXform->Scale(fScaleX, fScaleY, origin.x, origin.y);
	pComp->SetTransform(pXform);
	pComp->OnMove();
}
void SetComponentScale (CODComponent* pComp, float fScaleX, float fScaleY, int iOrigin)
{
//	CPoint origin = GetComponentPoint (pComp, iOrigin);
	CPoint ptOrigin = pComp->GetControlPoint ((OD_CONTROL_POINT) iOrigin);
	SetComponentScale (pComp, fScaleX, fScaleY, ptOrigin);
}
