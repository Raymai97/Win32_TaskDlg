// Title: Wrapper for Win32 TaskDialog
// Author: raymai97
// Date: Dec 2021
// Dependencies:
// - Windows.ole32

#pragma once
#include <Windows.h>

EXTERN_C FARPROC x_comctl32__TaskDialogIndirect;

typedef enum TASKDLG_ICONTYPE_t {
	TASKDLG_ICON_BY_HICON = 0,
	TASKDLG_ICON_WARNING,
	TASKDLG_ICON_ERROR,
	TASKDLG_ICON_INFO,
	TASKDLG_ICON_SHIELD,
	/* UAC shield, blue-teal gradient bg */
	TASKDLG_ICON_TRUSTED_SHIELD,
	/* Warning shield, orange bg */
	TASKDLG_ICON_WARNING_SHIELD,
	/* Red X mark shield, red bg */
	TASKDLG_ICON_ERROR_SHIELD,
	/* Green checkmark shield, green bg */
	TASKDLG_ICON_OK_SHIELD,
	/* UAC shield, medium gray bg */
	TASKDLG_ICON_GRAY_SHIELD
} TASKDLG_ICONTYPE_t;

typedef struct TASKDLG_ICON_t{
	TASKDLG_ICONTYPE_t type;
	HICON hico;
} TASKDLG_ICON_t;

typedef struct TASKDLG_BUTTON_t {
	int id;
	LPCWSTR szText;
} TASKDLG_BUTTON_t;

typedef struct TASKDLG_SPEC_t {
	int selectedBtnId;
	int selectedRadioId;
	BOOL isVerifyBoxTicked;

	HWND hwndOwner;
	TASKDLG_ICON_t mainIcon;
	LPCWSTR szTitle;
	LPCWSTR szBigText;
	LPCWSTR szText;

	BOOL commbtnOk;
	BOOL commbtnCancel;
	BOOL commbtnYes;
	BOOL commbtnNo;
	BOOL commbtnRetry;
	BOOL commbtnClose;
	TASKDLG_BUTTON_t * pButtons;
	UINT nButtons;
	TASKDLG_BUTTON_t * pRadioButtons;
	UINT nRadioButtons;

	LPCWSTR szPromptBE; /* BeforeExpanded */
	LPCWSTR szPromptAE; /* AfterExpanded */
	LPCWSTR szVerifyBox;

	TASKDLG_ICON_t footerIcon;
	LPCWSTR szFooter;

	LPCWSTR szExpanded;

	BOOL doUseCommandLink;
	BOOL commandLinkNoIcon;
	BOOL cancelable;
	BOOL sizeToContent;
	UINT clientWidth; /* In dialog units. If 0, auto-calc. */
} TASKDLG_SPEC_t;

EXTERN_C
HRESULT
TaskDlg_ShowDialog(
	TASKDLG_SPEC_t * pSpec
);
