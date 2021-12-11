// Title: TaskDlg Demo (the most simple one)
// Author: raymai97
// Date: Dec 2021
// Dependencies:
// - raymai97.TaskDlg
// - Windows.comctl32 v6.0

#include <Windows.h>
#include "../src/TaskDlg.h"

static void demo_info(void)
{
    TASKDLG_SPEC_t s = { 0 };
    s.mainIcon.type = TASKDLG_ICON_INFO;
    s.cancelable = TRUE;
    s.commbtnOk = TRUE;
    s.szTitle = L"TaskDlg Demo - Information";
    s.szText = L"This code behaves approximately like:\n\n"
        L"MessageBox(... MB_OK | MB_ICONASTERISK)";
    TaskDlg_ShowDialog(&s);
}

static void demo_ques(void)
{
    TASKDLG_SPEC_t s = { 0 };
    s.commbtnYes = TRUE;
    s.commbtnNo = TRUE;
    s.szTitle = L"TaskDlg Demo - Question";
    s.szBigText = L"Do you think Microsoft is right?";
    s.szText = L"This code behaves approximately like:\n\n"
        L"MessageBox(... MB_YESNO)\n\n"
        L"Note that there is no MB_ICONQUESTION for TaskDlg. Microsoft "
        L"decided it's bad idea to show Question icon for question.";
    TaskDlg_ShowDialog(&s);
    if (s.selectedBtnId == IDYES)
    {
        MessageBoxW(0, L"You clicked Yes.", s.szTitle, MB_ICONASTERISK);
    }
    else if (s.selectedBtnId == IDNO)
    {
        MessageBoxW(0, L"You clicked No.", s.szTitle, MB_ICONASTERISK);
    }
    else
    {
        MessageBoxW(0, L"Whoa, this is unexpected.", s.szTitle, MB_ICONEXCLAMATION);
    }
}

static void demo_choices(void)
{
    TASKDLG_SPEC_t s = { 0 };
    PCWSTR const szColors[] = {
        L"&Black",
        L"&Blue",
        L"&Brown",
        L"&Cyan",
        L"&Green",
        L"&Magenta",
        L"&Nyanza",
        L"&Red",
        L"&Silver",
        L"&Teal",
        L"&White",
        L"&Yellow"
    };
    int const button_baseID = 100;
    TASKDLG_BUTTON_t buttons[ARRAYSIZE(szColors)] = { 0 };
	int i = 0;
    WCHAR szResp[1024] = { 0 };
    s.commbtnNo = TRUE;
    s.commbtnYes = TRUE;
    s.szTitle = L"TaskDlg Demo - Choices";
    s.szBigText = L"What is your favorite color?";
    s.szText = L"This demonstrates the ability of TaskDlg to show user a lot of choices.";
    s.szExpanded = L"The CMY (Cyan, Magenta and Yellow) is the opposite of RGB (Red, Blue and Green).";
    s.szPromptAE = L"The More You &Know";
    s.szPromptBE = L"Do You &Know?";
    s.szVerifyBox = L"Verify box here";
    s.szFooter = L"Footer text here";
    s.doUseCommandLink = TRUE;
    s.nButtons = ARRAYSIZE(buttons);
    s.pButtons = buttons;
    for (i = 0; i < ARRAYSIZE(szColors); ++i)
    {
        buttons[i].id = button_baseID + i;
        buttons[i].szText = szColors[i];
    }
    TaskDlg_ShowDialog(&s);
    if (s.selectedBtnId >= button_baseID)
    {
        PCWSTR szColor = szColors[s.selectedBtnId - button_baseID];
        szColor++; // to skip the '&'
        wsprintfW(szResp, L"OK, your favorite color is %s.", szColor);
    }
    else if (s.selectedBtnId == IDNO)
    {
        lstrcpyW(szResp, L"You clicked No.");
    }
    else if (s.selectedBtnId == IDYES)
    {
        lstrcpyW(szResp, L"Yes is your favorite color? Do you mean Yellow?\n\n(Just joking)");
    }
    if (s.isVerifyBoxTicked)
    {
        lstrcatW(szResp, L"\n\nAnd you ticked that Verify Box.");
    }
    MessageBoxW(0, szResp, s.szTitle, MB_ICONASTERISK);
}

int main(void)
{
    HMODULE hMod = LoadLibraryW(L"comctl32");
    if (hMod)
    {
        x_comctl32__TaskDialogIndirect = GetProcAddress(hMod, "TaskDialogIndirect");
    }
    if (!x_comctl32__TaskDialogIndirect)
    {
        MessageBoxW(0, L"TaskDialog API not available.", L"Error", MB_ICONHAND);
        return 1;
    }
    demo_info();
    demo_ques();
    demo_choices();
    return 0;
}
