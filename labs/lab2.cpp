
// ReSharper disable CppClangTidyCertErr33C
// ReSharper disable CppClangTidyConcurrencyMtUnsafe
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

class item
{
    std::string name_;
    double price_;

public:
    explicit item(std::string n = "", const double p = 0.0) : name_(std::move(n)), price_(p) {}

    void set_name(const std::string& n) { name_ = n; }
    void set_price(const double p) { price_ = p; }

    std::string get_name() const { return name_; }
    double get_price() const { return price_; }

    friend std::ostream& operator<<(std::ostream& os, const item& item);
    friend std::istream& operator>>(std::istream& is, item& item);
};

std::ostream& operator<<(std::ostream& os, const item& item)
{
    os << std::setw(20) << std::left << item.name_ << std::setw(10) << std::fixed << std::setprecision(2) << std::right << item.price_;
    return os;
}

std::istream& operator>>(std::istream& is, item& item)
{
    std::getline(is, item.name_, ',');
    is >> item.price_;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return is;
}

class order
{
    std::vector<item> items_;

public:
    void add_item(const item& item)
    {
        items_.push_back(item);
    }

    void remove_item(const std::string& name)
    {
        const auto it = remove_if(items_.begin(), items_.end(), [&name](const item& item) { return item.get_name() == name; });
        if (it != items_.end())
        {
            items_.erase(it, items_.end());
            std::cout << "Item removed successfully!\n";
        }
        else
            std::cout << "Item not found.\n";
    }

    void edit_item(const std::string& name, const item& new_item)
    {
        for (auto& item : items_)
        {
            if (item.get_name() == name)
            {
                item = new_item;
                std::cout << "Item edited successfully!\n";
                return;
            }
        }
        std::cout << "Item not found.\n";
    }

    void sort_items()
    {
        std::sort(items_.begin(), items_.end(), [](const item& a, const item& b) { return a.get_name() < b.get_name(); });
    }

    void display_items(std::ostream& os) const
    {
        os << std::setw(20) << std::left << "Name" << std::setw(10) << std::right << "Price\n";
        os << std::setfill('-') << std::setw(30) << "" << std::setfill(' ') << '\n';

        for (const auto& item : items_)
            os << item << '\n';
    }

    void find_item(const std::string& name) const
    {
        const auto it = std::find_if(items_.begin(), items_.end(), [&name](const item& item) { return item.get_name() == name; });
        if (it != items_.end())
            std::cout << "Item found: " << *it << '\n';
        else
            std::cout << "Item not found.\n";
    }

    void load_from_file(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error opening file for reading!\n";
            return;
        }

        item new_item;
        file.seekg(0);
        while (file >> new_item)
        {
            if (file.bad())
            {
                std::cerr << "Critical error while reading the file.\n";
                file.clear();
                break;
            }
            add_item(new_item);
        }
        file.close();
    }

    void save_to_file(const std::string& filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error opening file for writing!\n";
            return;
        }

        file.seekp(0);
        for (const auto& item : items_)
        {
            file << item.get_name() << ',' << item.get_price() << '\n';
            if (file.fail())
            {
                std::cerr << "Error writing to file.\n";
                file.clear();
            }
        }
        file.close();
    }

    static void remove_item_from_file(const std::string& filename, const std::string& item_name)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error opening file for reading!\n";
            return;
        }

        std::ofstream temp_file("temp.txt");
        if (!temp_file.is_open())
        {
            std::cerr << "Error opening temporary file for writing!\n";
            return;
        }

        item current_item;
        std::streampos pos;
        while (file >> current_item)
        {
            pos = file.tellg();
            if (current_item.get_name() != item_name)
                temp_file << current_item.get_name() << ',' << current_item.get_price() << '\n';
            else
                std::cout << "Removing item at position: " << pos << "\n";
        }
        file.close();
        temp_file.close();
        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }
};

class shop // NOLINT(cppcoreguidelines-special-member-functions)
{
    order* order_;

public:
    shop() : order_(new order()) {}
    ~shop() { delete order_; }

    void add_order_item(const item& item) const
    {
        order_->add_item(item);
    }

    void remove_order_item(const std::string& name) const
    {
        order_->remove_item(name);
    }

    void edit_order_item(const std::string& name, const item& new_item) const
    {
        order_->edit_item(name, new_item);
    }

    void sort_order_items() const
    {
        order_->sort_items();
    }

    void display_order() const
    {
        order_->display_items(std::cout);
    }

    void find_order_item(const std::string& name) const
    {
        order_->find_item(name);
    }

    void load_order(const std::string& filename) const
    {
        order_->load_from_file(filename);
    }

    void save_order(const std::string& filename) const
    {
        order_->save_to_file(filename);
    }

    static void remove_item_from_file(const std::string& filename, const std::string& item_name)
    {
        order::remove_item_from_file(filename, item_name);
    }
};

void end()
{
    std::cout << "\n";
    system("pause");
    system("cls");
}

int main()
{
    // ReSharper disable once CppTooWideScope
    const shop shop;
    int choice = 0;
    std::string name;
    double price;

    do
    {
        if (choice) end();

        std::cout << "\n===== Dish Shop =====\n";
        std::cout << "1. Add Item\n";
        std::cout << "2. Remove Item\n";
        std::cout << "3. Edit Item\n";
        std::cout << "4. Sort Items\n";
        std::cout << "5. Display Order\n";
        std::cout << "6. Load Order from File\n";
        std::cout << "7. Save Order to File\n";
        std::cout << "8. Find Item by Name\n";
        std::cout << "9. Remove Item from File\n";
        std::cout << "10. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            std::cout << "Enter item name: ";
            std::cin >> name;
            std::cout << "Enter item price: ";
            std::cin >> price;
            shop.add_order_item(item(name, price));
            break;
        }
        case 2:
        {
            std::cout << "Enter item name to remove: ";
            std::cin >> name;
            shop.remove_order_item(name);
            break;
        }
        case 3:
        {
            std::cout << "Enter item name to edit: ";
            std::cin >> name;
            std::cout << "Enter new item name: ";
            std::cin >> name;
            std::cout << "Enter new item price: ";
            std::cin >> price;
            shop.edit_order_item(name, item(name, price));
            break;
        }
        case 4:
            shop.sort_order_items();
            break;
        case 5:
            shop.display_order();
            break;
        case 6:
        {
            std::cout << "Enter file name to load: ";
            std::cin >> name;
            shop.load_order(name);
            break;
        }
        case 7:
        {
            std::cout << "Enter file name to save: ";
            std::cin >> name;
            shop.save_order(name);
            break;
        }
        case 8:
        {
            std::cout << "Enter item name to find: ";
            std::cin >> name;
            shop.find_order_item(name);
            break;
        }
        case 9:
        {
            std::string item_name;
            std::cout << "Enter file name: ";
            std::cin >> name;
            std::cout << "Enter item name to remove: ";
            std::cin >> item_name;
            shop::remove_item_from_file(name, item_name);
            break;
        }
        case 10:
            std::cout << "Have a nice day!\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 10);

    return 0;
}
