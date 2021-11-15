#include <iostream>

#include "Registry.cpp"
using namespace std;

int main()
{
	cout << "Enter the command:\n";
	cout << "ROOT[root]\n";
	cout << "OPEN[subkey]\n";
	cout << "RETURN\n";
	cout << "CREATE[name]\n";
	cout << "ADD [param_name][param_type][data]\n";
	cout << "SEARCH[subkey]\n";
	cout << "READ_FLAGS[path]\n";
	cout << "GET_INFO\n";
	cout << "EXIT\n";

	string command;
	cin >> command;
	HKEY hKey = NULL;
	int rooti = 0;
	auto registry = new Registry();
	while (TRUE)
	{
		if (strcmp(command.c_str(), "ROOT"))
		{
			string root;
			cout << "Enter root: ";
			cin >> root;
			cout << "\n";
			BOOL res = registry->Root(root);
			if (!res)
			{
				cout << "ERROR";
			}
			else
			{
				cout << "SUCCESS";
			}
		}
		else if (strcmp(command.c_str(), "OPEN"))
		{
			string subKey;
			cout << "Enter subkey: ";
			cin >> subKey;
			cout << "\n";
			auto res = registry->Open();
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
				cout << "SUCCESS";
			}
		}
		else if (strcmp(command.c_str(), "CREATE"))
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
				cout << "SUCCESS";
			}
		}
		else if (strcmp(command.c_str(), "ADD"))
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
				cout << "SUCCESS";
			}
		}
		else if (strcmp(command.c_str(), "SEARCH"))
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
				cout << "SUCCESS";
			}
		}
		else if (strcmp(command.c_str(), "READ_FLAGS"))
		{

		}
		else if (strcmp(command.c_str(), "GET_INFO"))
		{

		}
		else if (strcmp(command.c_str(), "EXIT"))
		{
			return;
		}

		cout << "\n";
		std::cin >> command;
		std::cout << "\n";
	}
}
