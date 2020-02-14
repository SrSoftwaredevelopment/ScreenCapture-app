#include "pch.h"
#include "TicketHook.h"

BOOL ctrlkey = FALSE;
BOOL winkey = FALSE;
BOOL tkey = FALSE;
HWND g_hWnd;

BOOL IsHotkey(DWORD vkCode)
{
	if (vkCode == VK_LWIN || vkCode == VK_RWIN || vkCode == VK_LCONTROL || vkCode == VK_RCONTROL || vkCode == 't' || vkCode == 'T')
		return TRUE;

	return FALSE;
}

void CTicketHook::InstallHook(HWND hWnd)
{
	g_hWnd = hWnd;
	GetWindowThreadProcessId(hWnd, &m_dwProcId);
	m_MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MyMouseCallback, NULL, 0);
	m_KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyKeyBoardCallback, NULL, 0);
}

void CTicketHook::UninstallHook()
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
			POINT pt;
			GetCursorPos(&pt);
			HWND hWnd = WindowFromPoint(pt);
			DWORD dwProcess1 = 0xFFFFFFFF;
			GetWindowThreadProcessId(hWnd, &dwProcess1);

			DWORD dwProcess2 = 0xFFFFFFFF;
			GetWindowThreadProcessId(g_hWnd, &dwProcess2);

			if (dwProcess1 != dwProcess2)
				::SendMessage(g_hWnd, WM_USER_MOUSEHOOK, 0, 0);
		}
	}

	return CallNextHookEx(CTicketHook::Instance().m_MouseHook, nCode, wParam, lParam);
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
				winkey = FALSE;
				tkey = FALSE;
			}

			if (pKeyStruct->vkCode == VK_LCONTROL || pKeyStruct->vkCode == VK_RCONTROL)
				ctrlkey = TRUE;

			if (pKeyStruct->vkCode == VK_LWIN || pKeyStruct->vkCode == VK_RWIN)
				winkey = TRUE;

			if (pKeyStruct->vkCode == 0x54 || pKeyStruct->vkCode == 0x74)
				tkey = TRUE;
		}

		switch (wParam)
		{
		case WM_KEYUP:
		case WM_SYSKEYUP:
			{
				if (ctrlkey && winkey && tkey)
					::SendMessage(g_hWnd, WM_USER_KEYBDHOOK, 0, 0);
			
				ctrlkey = FALSE;
				winkey = FALSE;
				tkey = FALSE;
			}
			break;
		}
	}

	return CallNextHookEx(CTicketHook::Instance().m_KeyboardHook, nCode, wParam, lParam);
}