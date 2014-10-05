/**
 * @file  PropGeneral.h
 *
 * @brief Declaration of PropGeneral class
 */
// ID line follows -- this is updated by SVN
// $Id$

#if !defined(AFX_PROPGENERAL_H__30AD07B0_E420_11D1_BBC5_00A024706EDC__INCLUDED_)
#define AFX_PROPGENERAL_H__30AD07B0_E420_11D1_BBC5_00A024706EDC__INCLUDED_

#include "OptionsPanel.h"

class COptionsMgr;

/**
 * @brief Class for General options -propertypage.
 */
class PropGeneral : public OptionsPanel
{
// Construction
public:
	PropGeneral(COptionsMgr *optionsMgr);
	~PropGeneral();

// Implement IOptionsPanel
	virtual void ReadOptions();
	virtual void WriteOptions();

// Dialog Data
	//{{AFX_DATA(PropGeneral)
	enum { IDD = IDD_PROPPAGE_GENERAL };
	bool  m_bScroll;
	bool  m_bDisableSplash;
	bool  m_bSingleInstance;
	bool  m_bVerifyPaths;
	bool  m_bCloseWindowWithEsc;
	bool  m_bAskMultiWindowClose;
	bool  m_bMultipleFileCmp;
	bool  m_bMultipleDirCmp;
	int   m_nAutoCompleteSource;
	bool  m_bPreserveFiletime;
	bool  m_bShowSelectFolderOnStartup;
	bool  m_bCloseWithOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(PropGeneral)
	afx_msg void OnResetAllMessageBoxes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPGENERAL_H__30AD07B0_E420_11D1_BBC5_00A024706EDC__INCLUDED_)