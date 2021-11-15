#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <intrin.h>
#include <atlstr.h>
#include <codecvt>
using namespace std;

class Registry
{
private :
	string PathToNode = "";
	HKEY currentRoot = NULL;
	HKEY SearchInNode(HKEY node, string name);
public:
	BOOL Root(string root);
	DWORD Open();
	void PrintKeys(HKEY hKey);
	DWORD Create(string name);
	BOOL Add(string parameterName, string data);
	HKEY Search(string name);
	HKEY ReadFlags();
	BOOL GetInfo();
};

