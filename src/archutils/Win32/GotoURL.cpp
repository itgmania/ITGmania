#include "global.h"
#include "GotoURL.h"

#include <cstdint>

#include <windows.h>
#include <shellapi.h>
#include <string>
#include <codecvt>
#include <locale>
#include <vector>

static std::wstring ConvertToWString(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

static LONG GetRegKey(HKEY key, const std::wstring& subkey, std::wstring& retdata)
{
	HKEY hKey;
	LONG iRet = RegOpenKeyExW(key, subkey.c_str(), 0, KEY_QUERY_VALUE, &hKey);

	if (iRet != ERROR_SUCCESS)
	{
		return iRet;
	}

	DWORD iDataSize = 0;
	iRet = RegQueryValueExW(hKey, L"emulation", nullptr, nullptr, nullptr, &iDataSize);
	if (iRet != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return iRet;
	}

	std::vector<wchar_t> data(iDataSize / sizeof(wchar_t));
	iRet = RegQueryValueExW(hKey, L"emulation", nullptr, nullptr, reinterpret_cast<LPBYTE>(data.data()), &iDataSize);
	if (iRet == ERROR_SUCCESS)
	{
		retdata.assign(data.begin(), data.end() - 1);
	}

	RegCloseKey(hKey);
	return iRet;
}

bool GotoURL(const RString& sUrl)
{
	// Convert RString to std::wstring
	std::wstring iUrl = ConvertToWString(sUrl);

	// First try ShellExecuteEx
	SHELLEXECUTEINFOW sei = { sizeof(sei) };
	sei.lpVerb = L"open";
	sei.lpFile = iUrl.c_str();
	sei.nShow = SW_SHOWDEFAULT;
	sei.fMask = SEE_MASK_FLAG_NO_UI;

	if (ShellExecuteExW(&sei) && reinterpret_cast<intptr_t>(sei.hInstApp) > 32)
		return true;

	// If it failed, get the .htm regkey and lookup the program
	std::wstring key;
	if (GetRegKey(HKEY_CLASSES_ROOT, L".htm", key) != ERROR_SUCCESS)
		return false;

	key += L"\\shell\\open\\command";

	if( GetRegKey(HKEY_CLASSES_ROOT, key, key) != ERROR_SUCCESS )
		return false;

	size_t pos = key.find(L"\"%1\"");
	if (pos == std::wstring::npos)
	{
		// No quotes found. Check for %1 without quotes
		pos = key.find(L"%1");
		if (pos == std::wstring::npos)
			pos = key.length() - 1; // No parameter.
		else
			key.erase(pos); // Remove the parameter
	}
	else
	{
		key.erase(pos); // Remove the parameter
	}

	key += L" " + iUrl;

	STARTUPINFOW si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	if (CreateProcessW(nullptr, &key[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return true;
	}

	return false;
}

/*
 * (c) 2002-2004 Chris Danford
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
