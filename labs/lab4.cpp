
// ReSharper disable CppClangTidyConcurrencyMtUnsafe
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

class employee_lab4
{
    int id_;
    std::string name_;
    std::string position_;
    std::string department_;
    int experience_;
    double salary_;

public:
    employee_lab4() = default;

    employee_lab4(const int id, std::string name, std::string position, std::string department, const int experience, const double salary)
        : id_(id), name_(std::move(name)), position_(std::move(position)), department_(std::move(department)), experience_(experience), salary_(salary) {}

    int get_id() const { return id_; }
    std::string get_name() const { return name_; }
    std::string get_position() const { return position_; }
    std::string get_department() const { return department_; }
    int get_experience() const { return experience_; }
    double get_salary() const { return salary_; }

    void set_name(const std::string& name) { name_ = name; }
    void set_position(const std::string& position) { position_ = position; }
    void set_department(const std::string& department) { department_ = department; }
    void set_experience(const int experience) { experience_ = experience; }
    void set_salary(const double salary) { salary_ = salary; }

    bool operator<(const employee_lab4& other) const
    {
        return id_ < other.id_;
    }

    bool operator==(const employee_lab4& other) const
    {
        return id_ == other.id_;
    }

    void display() const
    {
        std::cout << "ID: " << id_ << ", Name: " << name_
            << ", Position: " << position_
            << ", Department: " << department_
            << ", Experience: " << experience_
            << " years, Salary: $" << salary_ << '\n';
    }

    struct hash_function
    {
        size_t operator()(const employee_lab4& emp) const
        {
            return std::hash<int>()(emp.get_id());
        }
    };
};

employee_lab4 createemployee_lab4()
{
    int id, experience;
    std::string name, position, department;
    double salary;

    std::cout << "Enter Employee ID: "; std::cin >> id;
    std::cout << "Enter Name: "; std::cin.ignore(); std::getline(std::cin, name);
    std::cout << "Enter Position: "; std::getline(std::cin, position);
    std::cout << "Enter Department: "; std::getline(std::cin, department);
    std::cout << "Enter Experience (years): "; std::cin >> experience;
    std::cout << "Enter Salary: "; std::cin >> salary;

    return employee_lab4{ id, name, position, department, experience, salary };
}

template<typename Container>
void display_all(const Container& container)
{
    for (const auto& entry : container)
        entry.display();
}

template<typename Container>
void display_all_with_key(const Container& container)
{
    for (const auto& entry : container)
        entry.second.display();
}

template<typename Container>
void save_to_file(const std::string& filename, const Container& container)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (const auto& entry : container)
            file << entry.get_id() << "," << entry.get_name() << "," << entry.get_position() << ","
            << entry.get_department() << "," << entry.get_experience() << "," << entry.get_salary() << '\n';
        std::cout << "Data saved to file: " << filename << '\n';
    }
    else
        std::cerr << "Error opening file!\n";
}

template<typename Container>
void save_to_file_with_key(const std::string& filename, const Container& container)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (const auto& entry : container)
            file << entry.second.get_id() << "," << entry.second.get_name() << "," << entry.second.get_position() << ","
            << entry.second.get_department() << "," << entry.second.get_experience() << "," << entry.second.get_salary() << '\n';
        std::cout << "Data saved to file: " << filename << '\n';
    }
    else
        std::cerr << "Error opening file!\n";
}

template<typename Container>
void find_employee_by_id(const Container& container, int id)
{
    auto it = container.find(id);
    if (it != container.end())
        it->second.display();
    else
        std::cout << "Employee with ID " << id << " not found.\n";
}

template<typename Container>
void find_employee_in_unordered(const Container& container, int id)
{
    for (const auto& entry : container)
    {
        if (entry.get_id() == id)
        {
            entry.display();
            return;
        }
    }
    std::cout << "Employee with ID " << id << " not found.\n";
}

template<typename Container>
void edit_employee(Container& container, int id)
{
    auto it = container.find(id);
    if (it != container.end())
    {
        std::string name, position, department;
        int experience;
        double salary;

        std::cout << "Enter new Name: "; std::cin.ignore(); std::getline(std::cin, name);
        std::cout << "Enter new Position: "; std::getline(std::cin, position);
        std::cout << "Enter new Department: "; std::getline(std::cin, department);
        std::cout << "Enter new Experience (years): "; std::cin >> experience;
        std::cout << "Enter new Salary: "; std::cin >> salary;

        it->second.set_name(name);
        it->second.set_position(position);
        it->second.set_department(department);
        it->second.set_experience(experience);
        it->second.set_salary(salary);

        std::cout << "Employee updated.\n";
    }
    else
        std::cout << "Employee not found.\n";
}

template<typename Container>
void delete_employee(Container& container, int id)
{
    auto it = container.find(id);
    if (it != container.end())
    {
        container.erase(it);
        std::cout << "Employee deleted.\n";
    }
    else
        std::cout << "Employee not found.\n";
}

template<typename Container>
void delete_employee_in_unordered(Container& container, int id)
{
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        if (it->get_id() == id)
        {
            container.erase(it);
            std::cout << "Employee deleted.\n";
            return;
        }
    }
    std::cout << "Employee not found.\n";
}

template<typename Container>
void sort_by_salary(Container& container)
{
    std::vector<employee_lab4> employees(container.begin(), container.end());

    std::sort(employees.begin(), employees.end(), [](const employee_lab4& a, const employee_lab4& b)
        {
            return a.get_salary() < b.get_salary();
        });

    for (const auto& emp : employees)
        emp.display();
}

void end()
{
    system("pause");
    system("cls");
}

int main()
{
    std::map<int, employee_lab4> employee_map;
    std::set<employee_lab4> employee_set;
    std::multimap<std::string, employee_lab4> employee_multi_map;
    std::multiset<employee_lab4> employee_multi_set;
    std::unordered_map<int, employee_lab4> employee_unordered_map;
    std::unordered_set<employee_lab4, employee_lab4::hash_function> employee_unordered_set;
    std::unordered_multimap<std::string, employee_lab4> employee_unordered_multi_map;
    std::unordered_multiset<employee_lab4, employee_lab4::hash_function> employee_unordered_multi_set;

    int choice = 0;
    do
    {
        if (choice) end();

        std::cout << "\nMenu:\n";
        std::cout << "1. Add Employee\n";
        std::cout << "2. Display All Employees\n";
        std::cout << "3. Save to File\n";
        std::cout << "4. Find Employee by ID\n";
        std::cout << "5. Edit Employee by ID\n";
        std::cout << "6. Delete Employee by ID\n";
        std::cout << "7. Sort Employees by Salary\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            employee_lab4 emp = createemployee_lab4();
            employee_map[emp.get_id()] = emp;
            employee_set.insert(emp);
            employee_multi_map.emplace(emp.get_department(), emp);
            employee_multi_set.insert(emp);
            employee_unordered_map[emp.get_id()] = emp;
            employee_unordered_set.insert(emp);
            employee_unordered_multi_map.emplace(emp.get_department(), emp);
            employee_unordered_multi_set.insert(emp);
            break;
        }
        case 2:
            std::cout << "\nemployee_map:\n";
            display_all_with_key(employee_map);
            std::cout << "\nemployee_set:\n";
            display_all(employee_set);
            std::cout << "\nemployee_multi_map:\n";
            display_all_with_key(employee_multi_map);
            std::cout << "\nemployee_multi_set:\n";
            display_all(employee_multi_set);
            std::cout << "\nemployee_unordered_map:\n";
            display_all_with_key(employee_unordered_map);
            std::cout << "\nemployee_unordered_set:\n";
            display_all(employee_unordered_set);
            std::cout << "\nemployee_unordered_multi_map:\n";
            display_all_with_key(employee_unordered_multi_map);
            std::cout << "\nemployee_unordered_multi_set:\n";
            display_all(employee_unordered_multi_set);
            break;
        case 3:
            save_to_file_with_key("employees_map.txt", employee_map);
            save_to_file("employees_set.txt", employee_set);
            save_to_file_with_key("employees_multi_map.txt", employee_multi_map);
            save_to_file("employees_multi_set.txt", employee_multi_set);
            save_to_file_with_key("employees_unordered_map.txt", employee_unordered_map);
            save_to_file("employees_unordered_set.txt", employee_unordered_set);
            save_to_file_with_key("employees_unordered_multi_map.txt", employee_unordered_multi_map);
            save_to_file("employees_unordered_multi_set.txt", employee_unordered_multi_set);
            break;
        case 4:
        {
            int id;
            std::cout << "Enter Employee ID: "; std::cin >> id;
            find_employee_by_id(employee_map, id);
            find_employee_in_unordered(employee_unordered_set, id);
            break;
        }
        case 5:
        {
            int id;
            std::cout << "Enter Employee ID to edit: "; std::cin >> id;
            edit_employee(employee_map, id);
            break;
        }
        case 6:
        {
            int id;
            std::cout << "Enter Employee ID to delete: "; std::cin >> id;
            delete_employee(employee_map, id);
            delete_employee_in_unordered(employee_unordered_set, id);
            break;
        }
        case 7:
            sort_by_salary(employee_set);
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
