#include "pch.h"
#include "TicketHook.h"

BOOL ctrlkey = TRUE;
BOOL altkey = TRUE;
BOOL tkey = TRUE;
HWND g_hWnd;

BOOL IsHotkey(DWORD vkCode)
{
	if (vkCode == 0xA2 || vkCode == 0xA3 || vkCode == 0xA4 || vkCode == 0xA5 || vkCode == 0x54 || vkCode == 0x74)
		return TRUE;

	return FALSE;
}

void TicketHook::InstallHook(HWND hWnd)
{
	g_hWnd = hWnd;
	m_MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MyMouseCallback, NULL, 0);
	m_KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyKeyBoardCallback, NULL, 0);
}

void TicketHook::UninstallHook()
{
	UnhookWindowsHookEx(m_MouseHook);
	UnhookWindowsHookEx(m_KeyboardHook);
}

LRESULT WINAPI MyMouseCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT * pMouseStruct = (MSLLHOOKSTRUCT *)lParam; // WH_MOUSE_LL struct

	if (nCode == 0)
	{ 
		if (wParam == WM_LBUTTONDOWN)
		{
			::SendMessage(g_hWnd, WM_USER_MOUSEHOOK, 0, 0);
		}
	}

	return CallNextHookEx(TicketHook::Instance().m_MouseHook, nCode, wParam, lParam);
}

LRESULT WINAPI MyKeyBoardCallback( int nCode, WPARAM wParam, LPARAM lParam )
{
	KBDLLHOOKSTRUCT* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;

	if (nCode == 0)
	{
		if (pKeyStruct)
		{
			if (!IsHotkey(pKeyStruct->vkCode))
			{
				ctrlkey = FALSE;
				altkey = FALSE;
				tkey = FALSE;
			}

			if (pKeyStruct->vkCode == 0xA2 || pKeyStruct->vkCode == 0xA3)
				ctrlkey = TRUE;

			if (pKeyStruct->vkCode == 0xA4 || pKeyStruct->vkCode == 0xA5)
				altkey = TRUE;

			if (pKeyStruct->vkCode == 0x54 || pKeyStruct->vkCode == 0x74)
				tkey = TRUE;
		}


		switch (wParam)
		{
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			if (ctrlkey && altkey && tkey)
			{
				::SendMessage(g_hWnd, WM_USER_KEYBDHOOK, 0, 0);
			}
			else
			{
				ctrlkey = false;
				altkey = false;
				tkey = false;
			}
		}
		break;
		}
	}

	return CallNextHookEx(TicketHook::Instance().m_KeyboardHook, nCode, wParam, lParam);
}