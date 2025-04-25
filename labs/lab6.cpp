
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClangTidyClangDiagnosticDeprecatedCopyWithDtor
// ReSharper disable CppClangTidyConcurrencyMtUnsafe
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <unordered_map>

class document_lab6
{
public:
    virtual void display() const = 0;
    virtual std::unique_ptr<document_lab6> clone() const = 0;
    virtual ~document_lab6() = default;
};

class passport final : public document_lab6
{
    std::string passport_number_;

public:
    explicit passport(std::string number) : passport_number_(std::move(number)) {}

    void display() const override
    {
        std::cout << "Passport: " << passport_number_ << '\n';
    }

    std::unique_ptr<document_lab6> clone() const override
    {
        return std::make_unique<passport>(*this);
    }
};

class visa final : public document_lab6
{
    std::string visa_type_;
    int duration_;

public:
    visa(std::string type, const int duration) : visa_type_(std::move(type)), duration_(duration) {}

    void display() const override
    {
        std::cout << "Visa: " << visa_type_ << " for " << duration_ << " days" << '\n';
    }

    std::unique_ptr<document_lab6> clone() const override
    {
        return std::make_unique<visa>(*this);
    }
};

class residence_permit final : public document_lab6
{
    std::string permit_number_;

public:
    explicit residence_permit(std::string number) : permit_number_(std::move(number)) {}

    void display() const override
    {
        std::cout << "Residence Permit: " << permit_number_ << '\n';
    }

    std::unique_ptr<document_lab6> clone() const override
    {
        return std::make_unique<residence_permit>(*this);
    }
};

class application_form final : public document_lab6
{
    std::string application_id_;

public:
    explicit application_form(std::string id) : application_id_(std::move(id)) {}

    void display() const override
    {
        std::cout << "Application Form ID: " << application_id_ << '\n';
    }

    std::unique_ptr<document_lab6> clone() const override
    {
        return std::make_unique<application_form>(*this);
    }
};

class migration_card final : public document_lab6
{
    std::string card_number_;

public:
    explicit migration_card(std::string number) : card_number_(std::move(number)) {}

    void display() const override
    {
        std::cout << "Migration Card: " << card_number_ << '\n';
    }

    std::unique_ptr<document_lab6> clone() const override
    {
        return std::make_unique<migration_card>(*this);
    }
};

class document_factory
{
public:
    virtual std::unique_ptr<document_lab6> create_document(const std::vector<std::string>& args) const = 0;
    virtual ~document_factory() = default;
};

class passport_factory final : public document_factory
{
public:
    std::unique_ptr<document_lab6> create_document(const std::vector<std::string>& args) const override
    {
        return std::make_unique<passport>(args[0]);
    }
};

class visa_factory final : public document_factory
{
public:
    std::unique_ptr<document_lab6> create_document(const std::vector<std::string>& args) const override
    {
        return std::make_unique<visa>(args[0], std::stoi(args[1]));
    }
};

class residence_permit_factory final : public document_factory
{
public:
    std::unique_ptr<document_lab6> create_document(const std::vector<std::string>& args) const override
    {
        return std::make_unique<residence_permit>(args[0]);
    }
};

class application_form_factory final : public document_factory
{
public:
    std::unique_ptr<document_lab6> create_document(const std::vector<std::string>& args) const override
    {
        return std::make_unique<application_form>(args[0]);
    }
};

class migration_card_factory final : public document_factory
{
public:
    std::unique_ptr<document_lab6> create_document(const std::vector<std::string>& args) const override
    {
        return std::make_unique<migration_card>(args[0]);
    }
};

class document_registry
{
    std::unordered_map<std::string, std::unique_ptr<document_factory>> factories_;

public:
    void register_factory(const std::string& document_type, std::unique_ptr<document_factory> factory)
    {
        factories_[document_type] = std::move(factory);
    }

    std::unique_ptr<document_lab6> create_document(const std::string& document_type, const std::vector<std::string>& args) const
    {
        const auto it = factories_.find(document_type);
        if (it != factories_.end())
            return it->second->create_document(args);
        throw std::runtime_error("Unknown document type: " + document_type);
    }
};

class document_manager
{
    std::vector<std::unique_ptr<document_lab6>> documents_;

public:
    void add_document(std::unique_ptr<document_lab6> doc)
    {
        documents_.push_back(std::move(doc));
    }

    void remove_document(const size_t index)
    {
        if (index < documents_.size())
            documents_.erase(documents_.begin() + static_cast<long long>(index));
    }

    void edit_document(const size_t index, std::unique_ptr<document_lab6> doc)
    {
        if (index < documents_.size())
            documents_[index] = std::move(doc);
    }

    void display_all() const
    {
        for (size_t i = 0; i < documents_.size(); ++i)
        {
            std::cout << i + 1 << ". ";
            documents_[i]->display();
        }
    }

    void save_to_file(const std::string& filename) const
    {
        std::ofstream out_file(filename);
        if (out_file.is_open())
        {
            for (const auto& doc : documents_)
            {
                out_file << "Document: ";
                doc->display();
            }
        }
    }
};

void show_menu()
{
    std::cout << "\n===== Document Management System =====\n";
    std::cout << "1. Add document\n";
    std::cout << "2. Remove document\n";
    std::cout << "3. Edit document\n";
    std::cout << "4. Display all documents\n";
    std::cout << "5. Save documents to file\n";
    std::cout << "6. Exit\n";
    std::cout << "Choose an option: ";
}

std::string choose_document_type()
{
    std::cout << "\nChoose document type:\n";
    std::cout << "passport, visa, residence_permit, application_form, migration_card\n";
    std::cout << "Enter document type: ";
    std::string doc_type;
    std::cin >> doc_type;
    return doc_type;
}

std::vector<std::string> get_document_data(const std::string& document_type)
{
    std::vector<std::string> data;
    std::string input;

    if (document_type == "passport")
    {
        std::cout << "Enter passport number: ";
        std::cin >> input;
        data.push_back(input);
    }
    else if (document_type == "visa")
    {
        std::cout << "Enter visa type: ";
        std::cin >> input;
        data.push_back(input);
        std::cout << "Enter visa duration (in days): ";
        std::cin >> input;
        data.push_back(input);
    }
    else if (document_type == "residence_permit")
    {
        std::cout << "Enter permit number: ";
        std::cin >> input;
        data.push_back(input);
    }
    else if (document_type == "application_form")
    {
        std::cout << "Enter application form ID: ";
        std::cin >> input;
        data.push_back(input);
    }
    else if (document_type == "migration_card")
    {
        std::cout << "Enter migration card number: ";
        std::cin >> input;
        data.push_back(input);
    }
    return data;
}

void end()
{
    system("pause");
    system("cls");
}

int main()
{
    document_manager manager;
    document_registry registry;

    registry.register_factory("passport", std::make_unique<passport_factory>());
    registry.register_factory("visa", std::make_unique<visa_factory>());
    registry.register_factory("residence_permit", std::make_unique<residence_permit_factory>());
    registry.register_factory("application_form", std::make_unique<application_form_factory>());
    registry.register_factory("migration_card", std::make_unique<migration_card_factory>());

    bool running = true;

    while (running)
    {
        show_menu();
        int option;
        std::cin >> option;

        switch (option)
        {
        case 1:
        {
            std::string doc_type = choose_document_type();
            auto data = get_document_data(doc_type);
            try
            {
                manager.add_document(registry.create_document(doc_type, data));
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << '\n';
            }
            break;
        }
        case 2:
        {
            size_t index;
            manager.display_all();
            std::cout << "Enter document number to remove: ";
            std::cin >> index;
            manager.remove_document(index - 1);
            break;
        }
        case 3:
        {
            size_t index;
            manager.display_all();
            std::cout << "Enter document number to edit: ";
            std::cin >> index;
            std::string doc_type = choose_document_type();
            auto data = get_document_data(doc_type);
            try
            {
                manager.edit_document(index - 1, registry.create_document(doc_type, data));
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << '\n';
            }
            break;
        }
        case 4:
            manager.display_all();
            break;
        case 5:
        {
            std::string filename;
            std::cout << "Enter filename: ";
            std::cin >> filename;
            manager.save_to_file(filename);
            break;
        }
        case 6:
            running = false;
            std::cout << "Have a nice day!\n";
            break;
        default:
            std::cout << "Invalid option!\n";
            break;
        }

        if (running) end();
    }

    return 0;
}
