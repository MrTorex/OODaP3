
// ReSharper disable CppClangTidyConcurrencyMtUnsafe
#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <fstream>
#include <string>

class employee
{
	int id_;
    std::string name_;
    std::string position_;
    int age_;

public:
    employee() = default;

	employee(const int id, std::string name, std::string position, const int age)
        : id_(id), name_(std::move(name)), position_(std::move(position)), age_(age) {}

    int get_id() const { return id_; }
    std::string get_name() const { return name_; }
    std::string get_position() const { return position_; }
    int get_age() const { return age_; }

    void display() const
	{
        std::cout << "ID: " << id_ << ", Name: " << name_
            << ", Position: " << position_ << ", Age: " << age_ << '\n';
    }

    bool is_manager() const
	{
        return position_ == "Manager" || position_ == "CEO";
    }
};

void add_employee(std::vector<employee>& employees, const employee& emp)
{
    employees.push_back(emp);
}

void display_employees(const std::vector<employee>& employees)
{
    for (const auto& emp : employees)
        emp.display();
}

void add_older_employee(std::list<employee>& senior_employees, const employee& emp, const int age_threshold)
{
    if (emp.get_age() > age_threshold)
        senior_employees.push_back(emp);
}

void display_employees(const std::list<employee>& employees)
{
    for (const auto& emp : employees)
        emp.display();
}

template <std::size_t N>
void add_manager(std::array<employee, N>& managers, const employee& emp, std::size_t& current_manager_count)
{
    if (emp.is_manager() && current_manager_count < N) 
        managers[current_manager_count++] = emp;
}

template <std::size_t N>
void display_employees(const std::array<employee, N>& employees, const std::size_t current_manager_count)
{
    for (std::size_t i = 0; i < current_manager_count; ++i) 
        employees[i].display();
}

template <typename Container>
void save_to_file(const Container& employees, const std::string& filename)
{
    std::ofstream file(filename);
    for (const auto& emp : employees)
        file << emp.get_id() << " " << emp.get_name() << " "
			<< emp.get_position() << " " << emp.get_age() << std::endl;
    file.close();
}

void end()
{
    system("pause");
    system("cls");
}

int main()
{
    std::vector<employee> employees;
    std::list<employee> senior_employees;
    std::array<employee, 2> managers;
    std::size_t current_manager_count = 0;
    // ReSharper disable once CppTooWideScope
    constexpr int senior_age_threshold = 50;

    int choice = 0;

    do 
    {
        if (choice) end();

        std::cout << "Menu:\n";
        std::cout << "1. Add employee to company\n";
        std::cout << "2. Display all employees\n";
        std::cout << "3. Display senior employees (older than " << senior_age_threshold << ")\n";
        std::cout << "4. Display managers\n";
        std::cout << "5. Save all employees to file\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
    	{
        case 1: 
        {
            int id, age;
            std::string name, position;
            std::cout << "Enter ID: ";
            std::cin >> id;
            std::cout << "Enter Name: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Enter Position: ";
            std::getline(std::cin, position);
            std::cout << "Enter Age: ";
            std::cin >> age;

            employee emp(id, name, position, age);
            add_employee(employees, emp);
            add_older_employee(senior_employees, emp, senior_age_threshold);
            add_manager(managers, emp, current_manager_count);
            break;
        }
        case 2:
            std::cout << "All employees:\n";
            display_employees(employees);
            break;
        case 3:
            std::cout << "Senior employees:\n";
            display_employees(senior_employees);
            break;
        case 4:
            std::cout << "Managers:\n";
            display_employees(managers, current_manager_count);
            break;
        case 5:
            save_to_file(employees, "employees.txt");
            std::cout << "Employees saved to file.\n";
            break;
        case 0:
            std::cout << "Have a nice day!\n";
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}