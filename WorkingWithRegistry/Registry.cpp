#include "Registry.h"

#include <sstream>
#include <vector>


BOOL Registry::Root(string root)
{
	HKEY hKey = NULL;
	if (root.compare("HKEY_CLASSES_ROOT") == 0)
	{
		this->currentRoot = HKEY_CLASSES_ROOT;
		PathToNode = "HKEY_CLASSES_ROOT";
	}
	else if (root.compare("HKEY_CURRENT_USER") == 0)
	{
		this->currentRoot = HKEY_CURRENT_USER;
		PathToNode = "HKEY_CURRENT_USER";
	}
	else if (root.compare("HKEY_LOCAL_MACHINE") == 0)
	{
		this->currentRoot = HKEY_LOCAL_MACHINE;
		PathToNode = "HKEY_LOCAL_MACHINE";
	}
	else if (root.compare("HKEY_USERS") == 0)
	{
		this->currentRoot = HKEY_USERS;
		PathToNode = "HKEY_USERS";
	}
	else if (root.compare("HKEY_CURRENT_CONFIG") == 0)
	{
		this->currentRoot = HKEY_CURRENT_CONFIG;
		PathToNode = "HKEY_CURRENT_CONFIG";
	}
	else
		return FALSE;

	return TRUE;
}

DWORD Registry::Open()
{
	if (this->currentRoot != NULL)
	{

		vector<string> path;
		stringstream ss(this->PathToNode); // Turn the string into a stream. 
		string tok;

		while (getline(ss, tok, '\\')) {
			path.push_back(tok);
		}

		HKEY hKey = NULL;
		int lResult = RegOpenKeyEx(this->currentRoot, path[path.size()].c_str(), 0, KEY_ALL_ACCESS, &hKey);
		this->currentRoot = hKey;
		return lResult;
		/*if (lResult != ERROR_SUCCESS)
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
		}*/
	}

	return 0;
}

void Registry::PrintKeys(HKEY hKey)
{
	DWORD    cSubKeys = 0;
	DWORD    cValues = 0;
	DWORD i, retCode;
	if (hKey != NULL)
	{
		retCode = RegQueryInfoKey(hKey, NULL, NULL, NULL, &cSubKeys, NULL,
			NULL, &cValues, NULL, NULL, NULL, NULL);
		if (cSubKeys)
		{
			for (i = 0; i < cSubKeys; i++)
			{
				TCHAR    achKey[16383];
				DWORD    cbName;
				cbName = 32767;
				retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL,
					NULL, NULL, NULL);
				if (retCode == ERROR_SUCCESS)
				{
					std::cout << "subkey: ";
					std::cout << achKey;
					std::cout << '\n';
				}
			}
		}

		if (cValues)
		{
			for (i = 0; i < cValues; i++)
			{
				DWORD cbValue;
				cbValue = 255;
				TCHAR  achValue[255];
				retCode = RegEnumValue(hKey, i, achValue, &cbValue, NULL, NULL, NULL, NULL);
				if (retCode == ERROR_SUCCESS)
				{
					std::cout << "value: ";
					std::cout << achValue;
					std::cout << '\n';
				}
			}
		}
		//OPEN[HKEY_LOCAL_MACHINE][SOFTWARE\Adobe\DefaultLanguage\CS6]
	}
}

DWORD Registry::Create(string name)
{
	LPCSTR ptr = name.c_str();
	HKEY newKey = NULL;
	int lResult = RegCreateKeyEx(this->currentRoot, ptr, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &newKey, NULL);
	this->currentRoot = newKey;
	this->PathToNode += ("\\" + name);
	RegCloseKey(newKey);
	return lResult;
	/*if (lResult != ERROR_SUCCESS)
	{
		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("Key not found.\n");
			return TRUE;
		}
		else {
			printf("Error opening key.\n");
			return FALSE;
		}
	}*/
}

BOOL Registry::Add(string parameterName, string data)
{
	auto type = REG_SZ;
	size_t size = strlen(data.c_str());
	int lResult = RegSetValueEx(this->currentRoot, parameterName.c_str(), 0, type, (LPBYTE)(data.c_str()), size);
	/*if (lResult != ERROR_SUCCESS)
	{
		if (lResult == ERROR_FILE_NOT_FOUND) {
			printf("Key not found.\n");
			return TRUE;
		}
		else {
			printf("Error opening key.\n");
			std::cout << lResult;
			return FALSE;
		}
	}*/
}

HKEY Registry::Search(string name)
{
	auto result = SearchInNode(HKEY_CLASSES_ROOT, name);
	if (result != NULL) return result;

	result = SearchInNode(HKEY_CURRENT_USER, name);
	if (result != NULL) return result;

	result = SearchInNode(HKEY_LOCAL_MACHINE, name);
	if (result != NULL) return result;

	result = SearchInNode(HKEY_USERS, name);
	if (result != NULL) return result;

	result = SearchInNode(HKEY_CURRENT_CONFIG, name);
	if (result != NULL) return result;
}

HKEY Registry::ReadFlags()
{
	std::string path = "reg flags " + this->PathToNode;
	system(path.c_str());
}

BOOL Registry::GetInfo()
{
	std::string isexit = "";
	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	while (strcmp(isexit.c_str(), "") == 0)
	{
		LSTATUS lErrorCode = RegNotifyChangeKeyValue(this->currentRoot, TRUE,
			REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_ATTRIBUTES | REG_NOTIFY_CHANGE_LAST_SET | REG_NOTIFY_CHANGE_SECURITY,
			hEvent, TRUE);

		if (lErrorCode != ERROR_SUCCESS)
		{
			std::cout << "Error in RegNotifyChangeKeyValue";
			std::cout << lErrorCode;
			std::cout << "\n";
		}
		std::cout << "Waiting for a change in the specified key...\n";
		if (WaitForSingleObject(hEvent, INFINITE) == WAIT_FAILED)
		{
			std::cout << "Error in WaitForSingleObject";
			std::cout << GetLastError();
			std::cout << "\n";
		}
		else
		{
			std::cin >> isexit;
			std::cout << "Change has occurred\n";
		}
	}
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
			TCHAR achKey[16383];
			DWORD cbName;
			cbName = 32767;
			retCode = RegEnumKeyEx(node, i, achKey, &cbName, NULL,
				NULL, NULL, NULL);
			HKEY newKey = NULL;
			if (retCode == ERROR_SUCCESS)
			{
				int lResult = RegOpenKeyEx(node, achKey, 0, KEY_ALL_ACCESS, &newKey);
				string wsTmp(name.begin(), name.end());
				if (achKey == wsTmp)
				{
					return newKey;
				}
				else
				{
					RegCloseKey(newKey);
					return SearchInNode(newKey, name);
				}
			}
		}
	}

	return NULL;
}
