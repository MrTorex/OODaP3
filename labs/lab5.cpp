
// ReSharper disable CppClangTidyClangDiagnosticInvalidSourceEncoding
// ReSharper disable CppClangTidyConcurrencyMtUnsafe
// ReSharper disable CppClangTidyCertErr33C
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

class document
{
    int id_;
    std::string title_;
    std::string content_;
    int priority_;

public:
    document(const int id, std::string title, std::string content, const int priority)
        : id_(id), title_(std::move(title)), content_(std::move(content)), priority_(priority) {}

    int get_id() const { return id_; }
    std::string get_title() const { return title_; }
    std::string get_content() const { return content_; }
    int get_priority() const { return priority_; }

    void set_id(const int new_id) { id_ = new_id; }
    void set_title(const std::string& new_title) { title_ = new_title; }
    void set_content(const std::string& new_content) { content_ = new_content; }
    void set_priority(const int new_priority) { priority_ = new_priority; }

    void display() const
    {
        std::cout << "ID: " << id_ << "\nНазвание: " << title_ << "\nСодержание: " << content_ << "\nПриоритет: " << priority_ << '\n';
    }

    bool operator<(const document& other) const { return id_ < other.id_; }
    bool operator>(const document& other) const { return id_ > other.id_; }
};

class compare_priority
{
public:
    bool operator()(const document& d1, const document& d2) const
    {
        return d1.get_priority() < d2.get_priority();
    }
};

void edit_document(document& doc)
{
    int new_id, new_priority;
    std::string new_title, new_content;

    std::cout << "Введите новый ID: ";
    std::cin >> new_id;
    std::cin.ignore();
    std::cout << "Введите новое название: ";
    getline(std::cin, new_title);
    std::cout << "Введите новое содержание: ";
    getline(std::cin, new_content);
    std::cout << "Введите новый приоритет: ";
    std::cin >> new_priority;

    doc.set_id(new_id);
    doc.set_title(new_title);
    doc.set_content(new_content);
    doc.set_priority(new_priority);
}

void find_document_in_queue(const std::queue<document>& doc_queue, const int id)
{
    std::queue<document> temp_queue = doc_queue;
    bool found = false;

    while (!temp_queue.empty())
    {
        const document& doc = temp_queue.front();

        if (doc.get_id() == id)
        {
            doc.display();
            found = true;
            break;
        }
        temp_queue.pop();
    }

    if (!found)
        std::cout << "Документ с ID " << id << " не найден.\n";
}

void filter_documents_by_priority(const std::queue<document>& doc_queue, const int min_priority)
{
    std::queue<document> temp_queue = doc_queue;

    std::cout << "Документы с приоритетом выше " << min_priority << ":\n";

    while (!temp_queue.empty())
    {
        const document& doc = temp_queue.front();
        if (doc.get_priority() >= min_priority)
            doc.display();
        temp_queue.pop();
    }
}

void sort_queue(std::queue<document>& doc_queue)
{
    std::vector<document> doc_vector;

    while (!doc_queue.empty())
    {
        doc_vector.push_back(doc_queue.front());
        doc_queue.pop();
    }

    std::sort(doc_vector.begin(), doc_vector.end(), [](const document& d1, const document& d2) { return d1.get_id() < d2.get_id(); });

    std::cout << "Отсортированные документы:\n";

    for (const auto& doc : doc_vector)
    {
        doc.display();
        doc_queue.push(doc);
    }
}

void display_queue(std::queue<document> doc_queue)
{
    while (!doc_queue.empty())
    {
        doc_queue.front().display();
        doc_queue.pop();
    }
}

void display_stack(std::stack<document> doc_stack)
{
    while (!doc_stack.empty())
    {
        doc_stack.top().display();
        doc_stack.pop();
    }
}

void display_priority_queue(std::priority_queue<document, std::vector<document>, compare_priority> doc_priority_queue)
{
    while (!doc_priority_queue.empty())
    {
        doc_priority_queue.top().display();
        doc_priority_queue.pop();
    }
}

void save_to_file(std::queue<document> doc_queue, const std::string& filename)
{
    std::ofstream out_file(filename);

    while (!doc_queue.empty())
    {
        out_file << "ID: " << doc_queue.front().get_id() << " | Название: " << doc_queue.front().get_title()
            << " | Содержание: " << doc_queue.front().get_content() << " | Приоритет: " << doc_queue.front().get_priority() << '\n';
        doc_queue.pop();
    }
    out_file.close();

    std::cout << "Данные сохранены в файл: " << filename << '\n';
}

void end()
{
    system("pause");
    system("cls");
}

int main()
{
    setlocale(LC_ALL, "Russian");

    std::queue<document> doc_queue;
    std::stack<document> doc_stack;
    std::priority_queue<document, std::vector<document>, compare_priority> doc_priority_queue;

    int choice = 0;
    do
    {
        if (choice) end();

        std::cout << "\nМеню:\n";
        std::cout << "1. Добавить документ\n";
        std::cout << "2. Удалить документ (очередь)\n";
        std::cout << "3. Показать все документы (очередь)\n";
        std::cout << "4. Сохранить документы в файл\n";
        std::cout << "5. Добавить документ в стек\n";
        std::cout << "6. Показать все документы (стек)\n";
        std::cout << "7. Добавить документ в очередь с приоритетом\n";
        std::cout << "8. Показать все документы (приоритетная очередь)\n";
        std::cout << "9. Поиск документа по ID\n";
        std::cout << "10. Фильтрация документов по приоритету\n";
        std::cout << "11. Редактировать документ (очередь)\n";
        std::cout << "12. Сортировка документов (очередь)\n";
        std::cout << "13. Выход\n";
        std::cout << "Выберите действие: ";

        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int id, priority;
            std::string title, content;

            std::cout << "Введите ID документа: ";
            std::cin >> id;
            std::cin.ignore();

            std::cout << "Введите название документа: ";
            getline(std::cin, title);

            std::cout << "Введите содержание документа: ";
            getline(std::cin, content);

            std::cout << "Введите приоритет документа: ";
            std::cin >> priority;

            doc_queue.emplace(id, title, content, priority);
            break;
        }
        case 2:
        {
            if (!doc_queue.empty())
            {
                std::cout << "Удален документ:\n";
                doc_queue.front().display();
                doc_queue.pop();
            }
            else
                std::cout << "Очередь пуста.\n";
            break;
        }
        case 3:
        {
            if (!doc_queue.empty())
                display_queue(doc_queue);
            else
                std::cout << "Очередь пуста.\n";
            break;
        }
        case 4:
        {
            std::string filename;

            std::cout << "Введите имя файла: ";
            std::cin >> filename;
            save_to_file(doc_queue, filename);
            break;
        }
        case 5:
        {
            int id, priority;
            std::string title, content;

            std::cout << "Введите ID документа: ";
            std::cin >> id;
            std::cin.ignore();

            std::cout << "Введите название документа: ";
            getline(std::cin, title);

            std::cout << "Введите содержание документа: ";
            getline(std::cin, content);

            std::cout << "Введите приоритет документа: ";
            std::cin >> priority;

            doc_stack.emplace(id, title, content, priority);
            break;
        }
        case 6:
        {
            if (!doc_stack.empty())
                display_stack(doc_stack);
            else
                std::cout << "Стек пуст.\n";
            break;
        }
        case 7:
        {
            int id, priority;
            std::string title, content;

            std::cout << "Введите ID документа: ";
            std::cin >> id;
            std::cin.ignore();

            std::cout << "Введите название документа: ";
            getline(std::cin, title);

            std::cout << "Введите содержание документа: ";
            getline(std::cin, content);

            std::cout << "Введите приоритет документа: ";
            std::cin >> priority;

            doc_priority_queue.emplace(id, title, content, priority);
            break;
        }
        case 8:
        {
            if (!doc_priority_queue.empty())
                display_priority_queue(doc_priority_queue);
            else
                std::cout << "Приоритетная очередь пуста.\n";
            break;
        }
        case 9:
        {
            int id;

            std::cout << "Введите ID для поиска: ";
            std::cin >> id;

            find_document_in_queue(doc_queue, id);
            break;
        }
        case 10:
        {
            int min_priority;

            std::cout << "Введите минимальный приоритет для фильтрации: ";
            std::cin >> min_priority;

            filter_documents_by_priority(doc_queue, min_priority);
            break;
        }
        case 11:
        {
            if (!doc_queue.empty())
            {
	            auto& doc = const_cast<document&>(doc_queue.front());

                std::cout << "Редактирование документа:\n";

                doc.display();
                edit_document(doc);
            }
            else
                std::cout << "Очередь пуста.\n";
            break;
        }
        case 12:
        {
            if (!doc_queue.empty())
                sort_queue(doc_queue);
            else
                std::cout << "Очередь пуста.\n";
            break;
        }
        case 13:
            std::cout << "Выход из программы.\n";
            break;
        default:
            std::cout << "Неверный выбор.\n";
            break;
        }
    } while (choice != 13);

    return 0;
}
