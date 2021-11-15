// turtleMdl.h : interface of the CTurtleModel class
//

#if !defined(AFX_TURTLEMDL_H__BCD4B56B_5259_11D3_A012_444553540000__INCLUDED_)
#define AFX_TURTLEMDL_H__BCD4B56B_5259_11D3_A012_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTurtleModel : public CODModel
{
	DECLARE_SERIAL(CTurtleModel)

public:
	CTurtleModel();

	CODSymbolComponent* SymbolLoad (LPCSTR lpszPath);
	CODLinkComponent* NodesConnect(CODSymbolComponent* pSource, CODSymbolComponent* pTarget);
	void EdgeInvert(CODLinkComponent* pLinkComp);
	BOOL CompRemove(CODComponent* pComp);
	CODComponent* CompDup(CODComponent* pComp);
	void Clear();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTurtleModel)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTurtleModel();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif // _DEBUG
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURTLEMDL_H__BCD4B56B_5259_11D3_A012_444553540000__INCLUDED_)
