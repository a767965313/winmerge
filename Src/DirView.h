/////////////////////////////////////////////////////////////////////////////
//    WinMerge:  an interactive diff/merge utility
//    Copyright (C) 1997  Dean P. Grimm
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
/////////////////////////////////////////////////////////////////////////////
/**
 *  @file DirView.h
 *
 *  @brief Declaration of class CDirView
 */
//
// ID line follows -- this is updated by SVN
// $Id: DirView.h 7062 2009-12-27 14:43:51Z kimmov $

#if !defined(AFX_DirView_H__16E7C721_351C_11D1_95CD_444553540000__INCLUDED_)
#define AFX_DirView_H__16E7C721_351C_11D1_95CD_444553540000__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CDirView view
#include <afxcview.h>
#include <map>
#include <boost/scoped_ptr.hpp>
#define POCO_NO_UNWINDOWS 1
#include <Poco/Types.h>
#include "OptionsDiffColors.h"
#include "SortHeaderCtrl.h"
#include "UnicodeString.h"
#include "DirItemIterator.h"
#include "DirActions.h"

class FileActionScript;

struct DIFFITEM;

typedef enum { eMain, eContext } eMenuType;

class CDirDoc;
class CDirFrame;

class PackingInfo;
class PathContext;
class DirCompProgressBar;
class CompareStats;
struct DirColInfo;
class CLoadSaveCodepageDlg;
class CShellContextMenu;
class CDiffContext;
class DirViewColItems;
class DirItemEnumerator;
struct IListCtrl;

/**
 * @brief Position value for special items (..) in directory compare view.
 */
const Poco::UIntPtr SPECIAL_ITEM_POS = (Poco::UIntPtr) - 1L;

/** Default column width in directory compare */
const UINT DefColumnWidth = 150;

/**
 * @brief Directory compare results view.
 *
 * Directory compare view is list-view based, so it shows one result (for
 * folder or file, commonly called as 'item') in one line. User can select
 * visible columns, re-order columns, sort by column etc.
 *
 * Actual data is stored in CDiffContext in CDirDoc. Dircompare items and
 * CDiffContext items are linked by storing POSITION of CDiffContext item
 * as CDirView listitem key.
 */
class CDirView : public CListView
{
	friend struct FileCmpReport;
	friend DirItemEnumerator;
protected:
	CDirView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDirView)

// Attributes
public:
	CDirDoc* GetDocument(); // non-debug version is inline
	// const version, for const methods to be able to call
	const CDirDoc * GetDocument() const { return const_cast<CDirView *>(this)->GetDocument(); }
	const CDiffContext& GetDiffContext() const;
	CDiffContext& GetDiffContext();

// Operations
public:
	CDirFrame * GetParentFrame();

	void StartCompare(CompareStats *pCompareStats);
	void Redisplay();
	void RedisplayChildren(Poco::UIntPtr diffpos, int level, UINT &index, int &alldiffs, const DirViewFilterSettings& dirfilter);
	void UpdateResources();
	void LoadColumnHeaderItems();
	Poco::UIntPtr GetItemKey(int idx) const;
	int GetItemIndex(Poco::UIntPtr key);
	// for populating list
	void DeleteItem(int sel);
	void DeleteAllDisplayItems();
	void SetFont(const LOGFONT & lf);

	void SortColumnsAppropriately();

	UINT GetSelectedCount() const;
	int GetFirstSelectedInd();
	void AddParentFolderItem(bool bEnable);
	void RefreshOptions();

	LRESULT HandleMenuMessage(UINT message, WPARAM wParam, LPARAM lParam);

// Implementation types
private:

// Implementation in DirActions.cpp
private:
	void GetItemFileNames(int sel, String& strLeft, String& strRight) const;
	void GetItemFileNames(int sel, PathContext * paths) const;
	void FormatEncodingDialogDisplays(CLoadSaveCodepageDlg * dlg);
	bool IsItemLeftOnly(int code);
	bool IsItemRightOnly(int code);
	DirActions MakeDirActions(DirActions::method_type func) const;
	DirActions MakeDirActions(DirActions::method_type2 func) const;
	Counts Count(DirActions::method_type2 func) const;
	void DoDirAction(DirActions::method_type func, const String& status_message);
	void DoDirActionTo(SIDE_TYPE stype, DirActions::method_type func, const String& status_message);
	void DoOpen(SIDE_TYPE stype);
	void DoOpenWith(SIDE_TYPE stype);
	void DoOpenWithEditor(SIDE_TYPE stype);
	void DoUpdateOpen(SELECTIONTYPE selectionType, CCmdUI* pCmdUI);
	void ConfirmAndPerformActions(FileActionScript & actions);
	void PerformActionList(FileActionScript & actions);
	void UpdateAfterFileScript(FileActionScript & actionList);
	void DoFileEncodingDialog();

// End DirActions.cpp
	void ReflectGetdispinfo(NMLVDISPINFO *);

// Implementation in DirViewColHandler.cpp
public:
	void UpdateColumnNames();
	void SetColAlignments();
	// class CompareState is used to pass parameters to the PFNLVCOMPARE callback function.
	class CompareState
	{
	private:
		const DirViewColItems *const pColItems;
		const CDiffContext *const pCtxt;
		const int sortCol;
		const bool bSortAscending;
		const bool bTreeMode;
	public:
		CompareState(const CDiffContext *pCtxt, const DirViewColItems *pColItems, int sortCol, bool bSortAscending, bool bTreeMode);
		static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	} friend;
	void UpdateDiffItemStatus(UINT nIdx);
private:
	void InitiateSort();
	void NameColumn(const char* idname, int subitem);
	int AddNewItem(int i, Poco::UIntPtr diffpos, int iImage, int iIndent);
// End DirViewCols.cpp

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirView)
public:
	virtual void OnInitialUpdate();
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDirView();
	int GetFocusedItem();
	int GetFirstDifferentItem();
	int GetLastDifferentItem();
	int AddSpecialItems();
	void GetCurrentColRegKeys(std::vector<String>& colKeys);
	void OpenSpecialItems(Poco::UIntPtr pos1, Poco::UIntPtr pos2, Poco::UIntPtr pos3);

// Implementation data
protected:
	CString GenerateReport();
	CSortHeaderCtrl m_ctlSortHeader;
	CImageList m_imageList;
	CImageList m_imageState;
	CListCtrl *m_pList;
	boost::scoped_ptr<IListCtrl> m_pIList;
	bool m_bEscCloses; /**< Cached value for option for ESC closing window */
	bool m_bExpandSubdirs;
	CFont m_font; /**< User-selected font */
	UINT m_nHiddenItems; /**< Count of items we have hidden */
	bool m_bTreeMode; /**< TRUE if tree mode is on*/
	boost::scoped_ptr<DirCompProgressBar> m_pCmpProgressBar;
	clock_t m_compareStart; /**< Starting process time of the compare */
	bool m_bUserCancelEdit; /**< TRUE if the user cancels rename */
	String m_lastCopyFolder; /**< Last Copy To -target folder. */

	int m_firstDiffItem;
	int m_lastDiffItem;
	bool m_bNeedSearchFirstDiffItem;
	bool m_bNeedSearchLastDiffItem;
	COLORSETTINGS m_cachedColors; /**< Cached color settings */

	boost::scoped_ptr<CShellContextMenu> m_pShellContextMenuLeft; /**< Shell context menu for group of left files */
	boost::scoped_ptr<CShellContextMenu> m_pShellContextMenuMiddle; /**< Shell context menu for group of middle files */
	boost::scoped_ptr<CShellContextMenu> m_pShellContextMenuRight; /**< Shell context menu for group of right files */
	HMENU m_hCurrentMenu; /**< Current shell context menu (either left or right) */
	boost::scoped_ptr<DirViewTreeState> m_pSavedTreeState;
	boost::scoped_ptr<DirViewColItems> m_pColItems;

	// Generated message map functions
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CDirView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	template<SIDE_TYPE srctype, SIDE_TYPE dsttype>
	afx_msg void OnDirCopy();
	template<SIDE_TYPE srctype, SIDE_TYPE dsttype>
	afx_msg void OnCtxtDirCopy();
	template<SIDE_TYPE srctype, SIDE_TYPE dsttype>
	afx_msg void OnUpdateDirCopy(CCmdUI* pCmdUI);
	template<SIDE_TYPE srctype, SIDE_TYPE dsttype>
	afx_msg void OnUpdateCtxtDirCopy(CCmdUI* pCmdUI);
	template<SIDE_TYPE stype>
	afx_msg void OnCtxtDirDel();
	template<SIDE_TYPE stype>
	afx_msg void OnUpdateCtxtDirDel(CCmdUI* pCmdUI);
	afx_msg void OnCtxtDirDelBoth();
	afx_msg void OnUpdateCtxtDirDelBoth(CCmdUI* pCmdUI);
	template<SIDE_TYPE stype>
	afx_msg void OnCtxtDirOpen();
	template<SIDE_TYPE stype>
	afx_msg void OnUpdateCtxtDirOpen(CCmdUI* pCmdUI);
	template<SIDE_TYPE stype>
	afx_msg void OnCtxtDirOpenWith();
	template<SIDE_TYPE stype>
	afx_msg void OnUpdateCtxtDirOpenWith(CCmdUI* pCmdUI);
	template<SIDE_TYPE stype>
	afx_msg void OnCtxtDirOpenWithEditor();
	template<SIDE_TYPE stype>
	afx_msg void OnUpdateCtxtDirOpenWithEditor(CCmdUI* pCmdUI);
	template<SIDE_TYPE stype>
	afx_msg void OnCtxtDirCopyTo();
	template<SIDE_TYPE stype>
	afx_msg void OnUpdateCtxtDirCopyTo(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFirstdiff();
	afx_msg void OnUpdateFirstdiff(CCmdUI* pCmdUI);
	afx_msg void OnLastdiff();
	afx_msg void OnUpdateLastdiff(CCmdUI* pCmdUI);
	afx_msg void OnNextdiff();
	afx_msg void OnUpdateNextdiff(CCmdUI* pCmdUI);
	afx_msg void OnPrevdiff();
	afx_msg void OnUpdatePrevdiff(CCmdUI* pCmdUI);
	afx_msg void OnCurdiff();
	afx_msg void OnUpdateCurdiff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSave(CCmdUI* pCmdUI);
	afx_msg LRESULT OnUpdateUIMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRefresh();
	afx_msg void OnUpdateRefresh(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEditColumns();
	template<SIDE_TYPE stype>
	afx_msg void OnReadOnly();
	template<SIDE_TYPE stype>
	afx_msg void OnUpdateReadOnly(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStatusLeftRO(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStatusMiddleRO(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStatusRightRO(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeColumns();
	afx_msg void OnCtxtOpenWithUnpacker();
	afx_msg void OnUpdateCtxtOpenWithUnpacker(CCmdUI* pCmdUI);
	afx_msg void OnToolsGenerateReport();
	afx_msg void OnCtxtDirZipLeft();
	afx_msg void OnCtxtDirZipRight();
	afx_msg void OnCtxtDirZipBoth();
	afx_msg void OnCtxtDirZipBothDiffsOnly();
	afx_msg void OnUpdateCtxtDir(CCmdUI* pCmdUI);
	afx_msg void OnSelectAll();
	afx_msg void OnUpdateSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnPluginPredifferMode(UINT nID);
	afx_msg void OnUpdatePluginPredifferMode(CCmdUI* pCmdUI);
	template<SIDE_TYPE side>
	afx_msg void OnCopyPathnames();
	afx_msg void OnCopyBothPathnames();
	afx_msg void OnCopyFilenames();
	afx_msg void OnUpdateCopyFilenames(CCmdUI* pCmdUI);
	template<SIDE_TYPE side>
	afx_msg void OnCopyToClipboard();
	afx_msg void OnCopyBothToClipboard();
	afx_msg void OnItemRename();
	afx_msg void OnUpdateItemRename(CCmdUI* pCmdUI);
	afx_msg void OnHideFilenames();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	template<SIDE_TYPE stype>
	afx_msg void OnCtxtDirMoveTo();
	template<SIDE_TYPE stype>
	afx_msg void OnUpdateCtxtDirMoveTo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHideFilenames(CCmdUI* pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnMarkedRescan();
	afx_msg void OnUpdateStatusNum(CCmdUI* pCmdUI);
	afx_msg void OnViewShowHiddenItems();
	afx_msg void OnUpdateViewShowHiddenItems(CCmdUI* pCmdUI);
	afx_msg void OnViewTreeMode();
	afx_msg void OnUpdateViewTreeMode(CCmdUI* pCmdUI);
	afx_msg void OnViewExpandAllSubdirs();
	afx_msg void OnUpdateViewExpandAllSubdirs(CCmdUI* pCmdUI);
	afx_msg void OnViewCollapseAllSubdirs();
	afx_msg void OnUpdateViewCollapseAllSubdirs(CCmdUI* pCmdUI);
	afx_msg void OnMergeCompare();
	template<SELECTIONTYPE seltype>
	afx_msg void OnMergeCompare2();
	afx_msg void OnMergeCompareXML();
	afx_msg void OnMergeCompareHex();
	afx_msg void OnUpdateMergeCompare(CCmdUI *pCmdUI);
	template<SELECTIONTYPE seltype>
	afx_msg void OnUpdateMergeCompare2(CCmdUI *pCmdUI);
	afx_msg void OnViewCompareStatistics();
	afx_msg void OnFileEncoding();
	afx_msg void OnUpdateFileEncoding(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearch();
	afx_msg void OnExpandFolder();
	afx_msg void OnCollapseFolder();
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedComparisonStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL OnHeaderBeginDrag(LPNMHEADER hdr, LRESULT* pResult);
	BOOL OnHeaderEndDrag(LPNMHEADER hdr, LRESULT* pResult);

private:
	void OpenSelection(SELECTIONTYPE selectionType = SELECTIONTYPE_NORMAL, PackingInfo * infoUnpacker = NULL);
	void OpenSelectionHex();
	bool GetSelectedItems(int * sel1, int * sel2, int * sel3);
	void OpenParentDirectory();
	template<SIDE_TYPE srctype, SIDE_TYPE dsttype>
	void DoUpdateDirCopy(CCmdUI* pCmdUI, eMenuType menuType);
	const DIFFITEM & GetDiffItem(int sel) const;
	DIFFITEM & GetDiffItem(int sel);
	int GetSingleSelectedItem() const;
	void MoveFocus(int currentInd, int i, int selCount);

	void FixReordering();
	void HeaderContextMenu(CPoint point, int i);
	void ListContextMenu(CPoint point, int i);
	bool ListShellContextMenu(SIDE_TYPE side);
	CShellContextMenu* GetCorrespondingShellContextMenu(HMENU hMenu) const;
	void ReloadColumns();
	bool IsLabelEdit() const;
	void CollapseSubdir(int sel);
	void ExpandSubdir(int sel, bool bRecursive = false);
	void GetColors(int nRow, int nCol, COLORREF& clrBk, COLORREF& clrText) const;
public:
	DirItemIterator Begin() const { return DirItemIterator(m_pIList.get()); }
	DirItemIterator End() const { return DirItemIterator(); }
	DirItemIterator RevBegin() const { return DirItemIterator(m_pIList.get(), -1, false, true); }
	DirItemIterator RevEnd() const { return DirItemIterator(); }
	DirItemIterator SelBegin() const { return DirItemIterator(m_pIList.get(), -1, true); }
	DirItemIterator SelEnd() const { return DirItemIterator(); }
	DirItemIterator SelRevBegin() const { return DirItemIterator(m_pIList.get(), -1, true, true); }
	DirItemIterator SelRevEnd() const { return DirItemIterator(); }
};

#ifndef _DEBUG  // debug version in DirView.cpp
inline CDirDoc* CDirView::GetDocument()
{ return (CDirDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DirView_H__16E7C721_351C_11D1_95CD_444553540000__INCLUDED_)
