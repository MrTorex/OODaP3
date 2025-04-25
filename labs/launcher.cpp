
// ReSharper disable CppClangTidyConcurrencyMtUnsafe
// ReSharper disable CppClangTidyClangDiagnosticInvalidSourceEncoding
#include <iostream>
#include <string>

void lab_enter(const int num)
{
	const std::string base_path = "\"C:/Desktop/ÁÃÓÈÐ/3 סולוסענ/ÎÎÏטÏ/lab_launcher/launcher/x64/Debug/lab";
	const std::string full_path = base_path + std::to_string(num) + ".exe\"";
	system(full_path.c_str());
	system("pause");
	system("cls");
}

int main()
{
	int choice;

	do
	{
		std::cout << "Choose your lab (-1 for exit): ";
		std::cin >> choice;

		if (choice == -1)
			std::cout << "Have a nice day!";
		else if (choice < 1 || choice > 6)
			std::cout << "Wrong lab number!\n";
		else
			lab_enter(choice);
	} while (choice != -1);

	return 0;
}