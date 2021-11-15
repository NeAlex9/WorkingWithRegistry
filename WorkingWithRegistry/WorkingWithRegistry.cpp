#include <iostream>

#include "Registry.h"
using namespace std;

int main()
{
	cout << "Enter the command:\n";
	cout << "ROOT\n";
	cout << "OPEN\n";
	cout << "CREATE\n";
	cout << "ADD\n";
	cout << "SEARCH\n";
	cout << "READ_FLAGS\n";
	cout << "PRINT_KEYS\n";
	cout << "EXIT\n";
	cout << "\n";

	string command;
	cin >> command;
	HKEY hKey = NULL;
	int rooti = 0;
	auto registry = new Registry();
	while (TRUE)
	{
		if (!strcmp(command.c_str(), "ROOT"))
		{
			string root;
			cout << "Enter root: ";
			cin >> root;
			BOOL res = registry->Root(root);
			if (!res)
			{
				cout << "ERROR";
			}
			else
			{
				registry->PrintKeys();
			}
		}
		else if (!strcmp(command.c_str(), "OPEN"))
		{
			string subKey;
			cout << "Enter subkey: ";
			cin >> subKey;
			cout << "\n";
			auto res = registry->Open(subKey);
			if (res != ERROR_SUCCESS)
			{
				if (res == ERROR_FILE_NOT_FOUND) {
					cout << "Key not found";
				}
				else
				{
					printf("Error opening key.\n");
				}
			}
			else
			{
				registry->PrintKeys();
			}
		}
		else if (!strcmp(command.c_str(), "CREATE"))
		{
			string name;
			cout << "Enter name: ";
			cin >> name;
			cout << "\n";

			auto res = registry->Create(name);

			if (res != ERROR_SUCCESS)
			{
				if (res == ERROR_FILE_NOT_FOUND) {
					cout << "Key not found.";
				}
				else {
					cout << "Error opening key";
				}
			}
			else
			{
				registry->PrintKeys();
			}
		}
		else if (!strcmp(command.c_str(), "ADD"))
		{
			string name;
			cout << "Enter parameters name: ";
			cin >> name;
			string data;
			cout << "Enter data: ";
			cin >> data;
			cout << "\n";

			BOOL res = registry->Add(name, data);

			if (res != ERROR_SUCCESS)
			{
				if (res == ERROR_FILE_NOT_FOUND) {
					cout << "Key not found.";
				}
				else {
					cout << "Error opening key.";
				}
			}
			else
			{
				registry->PrintKeys();
			}
		}
		else if (!strcmp(command.c_str(), "SEARCH"))
		{
			string name;
			cout << "Enter name: ";
			cin >> name;
			cout << "\n";
			auto key = registry->Search(name);
			if (!key)
			{
				cout << "Key not found.";
			}
			else
			{
				registry->PrintKeys();
			}
		}
		else if (!strcmp(command.c_str(), "READ_FLAGS"))
		{
			string customPath;
			cout << "Enter path after HKEY_LOCAL_MACHINE\\: ";
			cin >> customPath;
			cout << "\n";

			std::string path = "reg flags HKEY_LOCAL_MACHINE\\" + customPath;
			system(path.c_str());
		}
		else if (!strcmp(command.c_str(), "PRINT_KEYS"))
		{
			registry->PrintKeys();
		}
		else if (!strcmp(command.c_str(), "EXIT"))
		{
			cout << "\nExit from application\n";
			break;
		}

		cout << endl;
		cout << endl;
		cout << "Enter the command:\n";
		cout << "ROOT\n";
		cout << "OPEN\n";
		cout << "CREATE\n";
		cout << "ADD\n";
		cout << "SEARCH\n";
		cout << "PRINT_KEYS\n";
		cout << "READ_FLAGS\n";
		cout << "EXIT" << endl;
		cout << endl;
		std::cin >> command;
		std::cout << "\n" << endl;

	}
}
