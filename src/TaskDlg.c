#include "TaskDlg.h"
#include "TaskDlg_EXTEND.h"
#include <objbase.h>

FARPROC x_comctl32__TaskDialogIndirect;

static LPVOID
MyAllocZero(SIZE_T cb)
{
	LPVOID ptr = CoTaskMemAlloc(cb);
	if (ptr) { ZeroMemory(ptr, cb); }
	return ptr;
}

static void
MyFree(LPVOID ptr)
{
	if (ptr) { CoTaskMemFree(ptr); }
}

static HRESULT
MyTaskDialogIndirect(
	TASKDIALOGCONFIG const *pTaskConfig,
	int *pnButton,
	int *pnRadioButton,
	BOOL *pfVerificationFlagChecked)
{
	typedef HRESULT(WINAPI *fn_t)(
		TASKDIALOGCONFIG const *,
		int *,
		int *,
		BOOL *);
	fn_t fn = (fn_t)x_comctl32__TaskDialogIndirect;
	return fn ? fn(
		pTaskConfig,
		pnButton,
		pnRadioButton,
		pfVerificationFlagChecked
	) : E_NOTIMPL;
}

static int
MyFlags(TASKDLG_SPEC_t const *p)
{
	int tdf = TDF_POSITION_RELATIVE_TO_WINDOW | TDF_EXPAND_FOOTER_AREA;
	if (p->isVerifyBoxTicked) { tdf |= TDF_VERIFICATION_FLAG_CHECKED; }
	if (p->doUseCommandLink) { tdf |= TDF_USE_COMMAND_LINKS; }
	if (p->commandLinkNoIcon) { tdf |= TDF_USE_COMMAND_LINKS_NO_ICON; }
	if (p->cancelable) { tdf |= TDF_ALLOW_DIALOG_CANCELLATION; }
	if (p->sizeToContent) { tdf |= TDF_SIZE_TO_CONTENT; }
	if (p->mainIcon.type == TASKDLG_ICON_BY_HICON) {
		tdf |= TDF_USE_HICON_MAIN;
	}
	if (p->footerIcon.type == TASKDLG_ICON_BY_HICON) {
		tdf |= TDF_USE_HICON_FOOTER;
	}
	return tdf;
}

static int
MyCommBtnFlags(TASKDLG_SPEC_t const *p)
{
	return
		(p->commbtnOk ? TDCBF_OK_BUTTON : 0) |
		(p->commbtnCancel ? TDCBF_CANCEL_BUTTON : 0) |
		(p->commbtnYes ? TDCBF_YES_BUTTON : 0) |
		(p->commbtnNo ? TDCBF_NO_BUTTON : 0) |
		(p->commbtnRetry ? TDCBF_RETRY_BUTTON : 0) |
		(p->commbtnClose ? TDCBF_CLOSE_BUTTON : 0);
}

static void
MySetIcon(TASKDLG_ICON_t const *p, HICON *phico, PCWSTR *ppwsz)
{
	PCWSTR pwsz =
		p->type == TASKDLG_ICON_ERROR ? TD_ERROR_ICON :
		p->type == TASKDLG_ICON_INFO ? TD_INFORMATION_ICON :
		p->type == TASKDLG_ICON_WARNING ? TD_WARNING_ICON :
		p->type == TASKDLG_ICON_SHIELD ? TD_SHIELD_ICON :
		p->type == TASKDLG_ICON_TRUSTED_SHIELD ? MAKEINTRESOURCEW(-5) :
		p->type == TASKDLG_ICON_WARNING_SHIELD ? MAKEINTRESOURCEW(-6) :
		p->type == TASKDLG_ICON_ERROR_SHIELD ? MAKEINTRESOURCEW(-7) :
		p->type == TASKDLG_ICON_OK_SHIELD ? MAKEINTRESOURCEW(-8) :
		p->type == TASKDLG_ICON_GRAY_SHIELD ? MAKEINTRESOURCEW(-9) :
		NULL;
	if (!pwsz) { *phico = p->hico; }
	else { *ppwsz = pwsz; }
}

static HRESULT
MyShowDialog(TASKDLG_SPEC_t * p)
{
	HRESULT hr = 0;
	UINT i = 0;
	TASKDIALOG_BUTTON *pBtns = NULL, *pRBtns = NULL;
	TASKDIALOGCONFIG cfg = { sizeof(cfg) };
	cfg.dwFlags = MyFlags(p);
	cfg.dwCommonButtons = MyCommBtnFlags(p);
	cfg.hwndParent = p->hwndOwner ? p->hwndOwner : GetActiveWindow();
	cfg.cxWidth = p->clientWidth;
	{
		/* avoid warning C4366 (unaligned) */
		HICON hico = NULL;
		PCWSTR psz = NULL;
		MySetIcon(&p->mainIcon, &hico, &psz);
		cfg.hMainIcon = hico;
		cfg.pszMainIcon = psz;
	}
	cfg.pszWindowTitle = p->szTitle;
	cfg.pszMainInstruction = p->szBigText;
	cfg.pszContent = p->szText;
	cfg.pszCollapsedControlText = p->szPromptBE;
	cfg.pszExpandedControlText = p->szPromptAE;
	cfg.pszVerificationText = p->szVerifyBox;
	{
		HICON hico = NULL;
		PCWSTR psz = NULL;
		MySetIcon(&p->footerIcon, &hico, &psz);
		cfg.hFooterIcon = hico;
		cfg.pszFooterIcon = psz;
	}
	cfg.pszFooter = p->szFooter;
	cfg.pszExpandedInformation = p->szExpanded;
	if (p->nButtons > 0) {
		cfg.pButtons = pBtns =
			MyAllocZero(sizeof(TASKDIALOG_BUTTON) * p->nButtons);
		if (!pBtns) { hr = E_OUTOFMEMORY; goto eof; }
		for (i = 0; i < p->nButtons; ++i)
		{
			pBtns[i].nButtonID = p->pButtons[i].id;
			pBtns[i].pszButtonText = p->pButtons[i].szText;
		}
		cfg.cButtons = p->nButtons;
		cfg.nDefaultButton = p->selectedBtnId;
	}
	if (p->nRadioButtons > 0) {
		cfg.pRadioButtons = pRBtns = 
			MyAllocZero(sizeof(TASKDIALOG_BUTTON) * p->nRadioButtons);
		if (!pBtns) { hr = E_OUTOFMEMORY; goto eof; }
		for (i = 0; i < p->nRadioButtons; ++i)
		{
			pRBtns[i].nButtonID = p->pRadioButtons[i].id;
			pRBtns[i].pszButtonText = p->pRadioButtons[i].szText;
		}
		cfg.cRadioButtons = p->nRadioButtons;
		cfg.nDefaultRadioButton = p->selectedRadioId;
	}
	hr = MyTaskDialogIndirect(&cfg,
		&(p->selectedBtnId),
		&(p->selectedRadioId),
		&(p->isVerifyBoxTicked));
eof:
	MyFree(pBtns);
	MyFree(pRBtns);
	return hr;
}

EXTERN_C
HRESULT
TaskDlg_ShowDialog(
	TASKDLG_SPEC_t * pSpec)
{
	return MyShowDialog(pSpec);
}
