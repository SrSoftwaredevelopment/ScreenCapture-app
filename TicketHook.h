#pragma once

#define  WM_USER_KEYBDHOOK		WM_USER + 0x7000
#define  WM_USER_MOUSEHOOK		WM_USER + 0x7001

class TicketHook
{
public:
	//single ton
	static TicketHook& Instance()
	{
		static TicketHook myHook;
		return myHook;
	}

	void InstallHook(HWND hWnd); // function to install our hook
	void UninstallHook(); // function to uninstall our hook

	HHOOK m_MouseHook;			// handle to the hook	
	HHOOK m_KeyboardHook;
};

LRESULT WINAPI MyMouseCallback(int nCode, WPARAM wParam, LPARAM lParam);	//callback declaration
LRESULT WINAPI MyKeyBoardCallback(int nCode, WPARAM wParam, LPARAM lParam);