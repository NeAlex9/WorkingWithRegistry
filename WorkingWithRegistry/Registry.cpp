#define _CRT_SECURE_NO_WARNINGS
#include "Registry.h"

#include <algorithm>
#include <sstream>

BOOL Registry::Root(string root)
{
	HKEY hKey = NULL;
	if (root.compare("HKEY_CLASSES_ROOT") == 0)
	{
		this->currentRoot = HKEY_CLASSES_ROOT;
	}
	else if (root.compare("HKEY_CURRENT_USER") == 0)
	{
		this->currentRoot = HKEY_CURRENT_USER;
	}
	else if (root.compare("HKEY_LOCAL_MACHINE") == 0)
	{
		this->currentRoot = HKEY_LOCAL_MACHINE;
	}
	else if (root.compare("HKEY_USERS") == 0)
	{
		this->currentRoot = HKEY_USERS;
	}
	else if (root.compare("HKEY_CURRENT_CONFIG") == 0)
	{
		this->currentRoot = HKEY_CURRENT_CONFIG;
	}
	else
		return FALSE;

	return TRUE;
}

DWORD Registry::Open(string name)
{
	if (this->currentRoot != NULL)
	{
		wchar_t wtext[200];
		mbstowcs(wtext, name.c_str(), strlen(name.c_str()) + 1);
		LPWSTR ptr = wtext;
		HKEY hKey = this->currentRoot;
		int lResult = RegOpenKeyEx(this->currentRoot, ptr,
			0, KEY_ALL_ACCESS, &hKey);
		this->currentRoot = hKey;
		if (lResult != ERROR_SUCCESS)
		{
			if (lResult == ERROR_FILE_NOT_FOUND) {
				printf("Key not found.\n");
				return ERROR_FILE_NOT_FOUND;
			}
			else
			{
				printf("Error opening key.\n");
				return FALSE;
			}
		}
	}

	return 0;
}

void Registry::PrintKeys()
{
	DWORD    cSubKeys = 0;
	DWORD    cValues = 0;
	DWORD i, retCode;
	if (this->currentRoot != NULL)
	{
		retCode = RegQueryInfoKey(this->currentRoot, NULL, NULL, NULL, &cSubKeys, NULL,
			NULL, &cValues, NULL, NULL, NULL, NULL);
		if (cSubKeys)
		{
			for (i = 0; i < cSubKeys; i++)
			{
				TCHAR    achKey[16383];
				DWORD    cbName;
				cbName = 32767;
				retCode = RegEnumKeyEx(this->currentRoot, i, achKey, &cbName,
					NULL, NULL, NULL, NULL);
				if (retCode == ERROR_SUCCESS)
				{
					std::cout << "subkey: ";
					std::wcout << achKey;
					std::cout << '\n';
				}
			}
		}

		/*if (cValues)
		{
			for (i = 0; i < cValues; i++)
			{
				DWORD cbValue;
				cbValue = 255;
				TCHAR  achValue[255];
				retCode = RegEnumValue(this->currentRoot, i, achValue, &cbValue,
					NULL, NULL, NULL, NULL);
				if (retCode == ERROR_SUCCESS)
				{
					std::cout << "value: ";
					std::wcout << achValue;
					std::cout << '\n';
				}
			}
		}*/
		//OPEN[HKEY_LOCAL_MACHINE][SOFTWARE\Adobe\DefaultLanguage\CS6]
	}
}

DWORD Registry::Create(string name)
{
	wchar_t wtext[200];
	mbstowcs(wtext, name.c_str(), strlen(name.c_str()) + 1);
	LPWSTR ptr = wtext;
	HKEY newKey = this->currentRoot;
	int lResult = RegCreateKeyEx(this->currentRoot, ptr, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &newKey, NULL);
	this->currentRoot = newKey;
	RegCloseKey(newKey);
	return lResult;
}

BOOL Registry::Add(string parameterName, string data)
{
	BYTE* bytes = 0;
	DWORD size;
	wchar_t wtext[200];
	mbstowcs(wtext, parameterName.c_str(), strlen(parameterName.c_str()) + 1);
	LPWSTR ptr = wtext;
	wchar_t wwtext[200];
	size = strlen(data.c_str()) * 2;
	mbstowcs(wwtext, data.c_str(), strlen(data.c_str()) + 1);
	LPWSTR wptr = wwtext;
	bytes = (LPBYTE)wptr;
	int lResult = RegSetValueEx(this->currentRoot, ptr, 0, REG_EXPAND_SZ, bytes, size);
	return lResult;
}

BOOL Registry::Search(string name)
{
	DWORD    cSubKeys = 0;
	DWORD    cValues = 0;

	DWORD i, retCode;
	if (RegQueryInfoKey(this->currentRoot, NULL, NULL, NULL, &cSubKeys, NULL, NULL,
		&cValues, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		for (i = 0; i < cSubKeys; i++)
		{
			DWORD    cSubKeys = 0;
			DWORD    cValues = 0;
			DWORD i, retCode;

			if (this->currentRoot != NULL)
			{
				if (RegQueryInfoKey(this->currentRoot, NULL, NULL, NULL,
					&cSubKeys, NULL, NULL, &cValues, NULL,
					NULL, NULL, NULL) == ERROR_SUCCESS)
				{
					for (i = 0; i < cSubKeys; i++)
					{
						TCHAR    achKey[16383];
						DWORD    cbName;
						cbName = 32767;
						retCode = RegEnumKeyEx(this->currentRoot, i, achKey, &cbName,
							NULL, NULL, NULL, NULL);
						HKEY newKey = NULL;
						if (retCode == ERROR_SUCCESS)
						{
							int lResult = RegOpenKeyEx(this->currentRoot, achKey, 0, KEY_ALL_ACCESS, &newKey);
							std::wstring wsTmp(name.begin(), name.end());
							if (achKey == wsTmp)
							{
								this->currentRoot = newKey;
								return TRUE;
							}

							//RegCloseKey(newKey);
							auto in = SearchInNode(newKey, name);
							if (in != NULL)
							{
								this->currentRoot = in;
								return TRUE;
							}
						}
					}
				}
			}
		}
	}

	return NULL;
}

HKEY Registry::SearchInNode(HKEY node, string name)
{
	DWORD    cSubKeys = 0;
	DWORD    cValues = 0;

	DWORD i, retCode;
	if (RegQueryInfoKey(node, NULL, NULL, NULL, &cSubKeys, NULL, NULL,
		&cValues, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		for (i = 0; i < cSubKeys; i++)
		{
			DWORD    cSubKeys = 0;
			DWORD    cValues = 0;
			DWORD i, retCode;

			if (node != NULL)
			{
				if (RegQueryInfoKey(node, NULL, NULL, NULL,
					&cSubKeys, NULL, NULL, &cValues, NULL,
					NULL, NULL, NULL) == ERROR_SUCCESS)
				{
					for (i = 0; i < cSubKeys; i++)
					{
						TCHAR    achKey[16383];
						DWORD    cbName;
						cbName = 32767;
						retCode = RegEnumKeyEx(node, i, achKey, &cbName, 
							NULL, NULL, NULL, NULL);
						HKEY newKey = NULL;
						if (retCode == ERROR_SUCCESS)
						{
							int lResult = RegOpenKeyEx(node, achKey, 0, KEY_ALL_ACCESS, &newKey);
							std::wstring wsTmp(name.begin(), name.end());
							if (achKey == wsTmp)
							{
								return newKey;
							}

							//RegCloseKey(newKey);
							auto in = SearchInNode(newKey, name);
							if (in != NULL)
							{
								return in;
							}
						}
					}
				}
			}
		}
	}

	return NULL;
}
