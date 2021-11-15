#pragma once
#include <Windows.h>
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
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
	HKEY currentRoot = NULL;
	HKEY SearchInNode(HKEY node, string name);
public:
	BOOL Root(string root);
	DWORD Open(string name);
	void PrintKeys();
	DWORD Create(string name);
	BOOL Add(string parameterName, string data);
	BOOL Search(string name);
};

