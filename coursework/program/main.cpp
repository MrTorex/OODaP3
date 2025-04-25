
// ReSharper disable CppClangTidyClangDiagnosticInvalidSourceEncoding
// ReSharper disable CppClangTidyClangDiagnosticInvalidUtf8
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClangTidyConcurrencyMtUnsafe
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <functional>
#include <exception>

namespace app
{
    class question
	{
        std::string text_;
        std::vector<std::string> answers_;
        int correct_answer_;

    public:
        question() : correct_answer_(-1) {}

        question(std::string q_text, const std::vector<std::string>& ans, const int correct)
            : text_(std::move(q_text)), answers_(ans), correct_answer_(correct) {}

        void set_question(const std::string& q_text, const std::vector<std::string>& ans, const int correct)
    	{
            text_ = q_text;
            answers_ = ans;
            correct_answer_ = correct;
        }

        const std::string& get_text() { return text_; }

        const std::vector<std::string>& get_answers() { return answers_; }

        bool is_correct(const int answer_index) const { return answer_index == correct_answer_; }

        friend std::ostream& operator<<(std::ostream& os, const question& q);
        friend std::istream& operator>>(std::istream& is, question& q);

        void print() const
    	{
            std::cout << "������: " << text_ << '\n';
            for (size_t i = 0; i < answers_.size(); ++i)
                std::cout << i + 1 << ". " << answers_[i] << '\n';
        }
    };

    std::ostream& operator<<(std::ostream& os, const question& q)
	{
        os << q.text_ << ';';
        for (size_t i = 0; i < q.answers_.size(); ++i) 
        {
            os << q.answers_[i];
            if (i < q.answers_.size() - 1)
                os << ',';
        }
        os << ';' << q.correct_answer_;
        return os;
    }

    std::istream& operator>>(std::istream& is, question& q)
	{
        std::getline(is, q.text_, ';');
        if (q.text_.empty()) 
        {
            is.setstate(std::ios::failbit);
            return is;
        }

        std::string answers_line;
        std::getline(is, answers_line, ';');
        if (answers_line.empty()) 
        {
            is.setstate(std::ios::failbit);
            return is;
        }

        std::stringstream ss(answers_line);
        std::string answer;

        q.answers_.clear();

        while (std::getline(ss, answer, ','))
            if (!answer.empty())
                q.answers_.push_back(answer);

        if (!(is >> q.correct_answer_))
            is.setstate(std::ios::failbit);

        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return is;
    }

    class i_manageable
	{
    public:
        virtual void save(std::ostream& os) const = 0;
        virtual void load(std::istream& is) = 0;
        virtual ~i_manageable() = default;
    };

    class test final : public i_manageable
	{
        std::string title_;
        std::vector<question> questions_;

    public:
        test() = default;

        explicit test(std::string test_title) : title_(std::move(test_title)) {}

        void add_question(const question& q) { questions_.push_back(q); }

        void edit_question(const size_t index, const question& q)
    	{
            if (index >= questions_.size())
                throw std::out_of_range("Invalid question index.");
            questions_[index] = q;
        }

        void remove_question(const size_t index)
    	{
            if (index >= questions_.size())
                throw std::out_of_range("Invalid question index.");
            questions_.erase(questions_.begin() + static_cast<long long>(index));
        }

        const std::string& get_title() const { return title_; }

        const std::vector<question>& get_questions() const { return questions_; }

        void save(std::ostream& os) const override
    	{
            os << title_ << '\n';
            os << questions_.size() << '\n';
            for (const auto& q : questions_)
                os << q << '\n';
        }

        void load(std::istream& is) override
    	{
            std::getline(is, title_);
            if (title_.empty())
                throw std::runtime_error("������: ��������� ����� �� ������.");

            size_t count;
            if (!(is >> count))
                throw std::runtime_error("������: ������������ ���������� ��������.");

            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            questions_.clear();
            for (size_t i = 0; i < count; ++i) 
            {
                question q;
                if (!(is >> q))
                    throw std::runtime_error("������: ������������ ������ �������.");
                questions_.push_back(q);
            }
        }

        void print_questions() const
    	{
            for (size_t i = 0; i < questions_.size(); ++i) 
            {
                std::cout << i + 1 << ". ";
                questions_[i].print();
            }
        }
    };

    class user
	{
        std::string login_;
        std::string password_;
        bool is_admin_;

    public:
        user(std::string log, std::string pass, const bool admin)
            : login_(std::move(log)), password_(std::move(pass)), is_admin_(admin) {}

        bool authenticate(const std::string& log, const std::string& pass) const {
            return login_ == log && password_ == en_decrypt_password(pass);
        }

        bool is_administrator() const { return is_admin_; }

        const std::string& get_login() const { return login_; }

        const std::string& get_encrypted_password() const { return password_; }

        static std::string en_decrypt_password(const std::string& password)
        {
            const char key = static_cast<char>(password.length() % 10);
            std::string encrypted = password;
            for (char& c : encrypted)
                c ^= key;  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
            return encrypted;
        }
    };

    class result final : public i_manageable
	{
        std::string student_login_;
        std::string test_title_;
        int score_;

    public:
        result(std::string login, std::string title, const int s)
            : student_login_(std::move(login)), test_title_(std::move(title)), score_(s) {}

        void save(std::ostream& os) const override
    	{
            os << student_login_ << ',' << test_title_ << ',' << score_ << '\n';
        }

        void load(std::istream& is) override
    	{
            std::getline(is, student_login_, ',');
            std::getline(is, test_title_, ',');
            is >> score_;
            is.ignore();
        }

        const std::string& get_student_login() const { return student_login_; }

        const std::string& get_test_title() const { return test_title_; }

        int get_score() const { return score_; }
    };

    class application
	{
        std::vector<std::shared_ptr<user>> users_;
        std::vector<std::shared_ptr<test>> tests_;
        std::vector<std::shared_ptr<result>> results_;

    public:
        void load_users(const std::string& filename)
    	{
            std::ifstream file(filename);
            if (!file.is_open()) 
            {
                std::cerr << "������: �� ������� ������� ���� " << filename << ". ��������� ������������� �����.\n";
                return;
            }
            if (file.peek() == std::ifstream::traits_type::eof())
            {
                std::cerr << "������: ���� " << filename << "����.\n";
                return;
            }
            users_.clear();
            std::string login, password;
            bool is_admin;
            while (file >> login >> password >> is_admin)
                users_.push_back(std::make_shared<user>(login, password, is_admin));
        }

        void save_users(const std::string& filename) const
        {
            std::ofstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "������: �� ������� ������� ���� ��� ������: " << filename << ".\n";
                return;
            }
            try
            {
                for (const auto& user : users_)
                    file << user->get_login() << ' '
                    << user->get_encrypted_password() << ' '
                    << (user->is_administrator() ? 1 : 0) << '\n';
            }
            catch (const std::exception& ex)
            {
                std::cerr << "������ ��� ���������� " << filename << ": " << ex.what() << '\n';
            }
        }

        void load_tests(const std::string& filename)
    	{
            std::ifstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "������: �� ������� ������� ���� " << filename << ". ��������� ������������� �����.\n";
                return;
            }
            if (file.peek() == std::ifstream::traits_type::eof())
            {
                std::cerr << "������: ���� " << filename << "����.\n";
                return;
            }

            tests_.clear();
            try 
            {
                while (file.peek() != EOF) {
                    auto test_item = std::make_shared<test>();
                    test_item->load(file);
                    tests_.push_back(test_item);
                }
            }
            catch (const std::exception& ex) 
            {
                std::cerr << "������ ��� �������� ������: " << ex.what() << '\n';
            }
        }

        void save_tests(const std::string& filename) const
        {
            std::ofstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "������: �� ������� ������� ���� ��� ������: " << filename << ".\n";
                return;
            }
            try
            {
                for (const auto& test : tests_)
                    test->save(file);
            }
            catch (const std::exception& ex)
            {
                std::cerr << "������ ��� ���������� " << filename << ": " << ex.what() << '\n';
            }
            for (const auto& test : tests_)
                test->save(file);
        }

        void load_results(const std::string& filename)
    	{
            std::ifstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "������: �� ������� ������� ���� " << filename << ". ��������� ������������� �����.\n";
                return;
            }
            if (file.peek() == std::ifstream::traits_type::eof())
            {
                std::cerr << "������: ���� " << filename << "����.\n";
                return;
            }
            results_.clear();
            while (!file.eof()) 
            {
                auto res = std::make_shared<result>("", "", 0);
                try
                {
                    res->load(file);
                    results_.push_back(res);
                }
                catch (const std::exception& ex)
                {
                    std::cerr << "������ ��� �������� ����������: " << ex.what() << '\n';
                }

            }
        }

        void save_results(const std::string& filename) const
        {
            std::ofstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "������: �� ������� ������� ���� ��� ������: " << filename << ".\n";
                return;
            }
            try
            {
                for (const auto& result : results_)
                    result->save(file);
            }
            catch (const std::exception& ex)
            {
                std::cerr << "������ ��� ���������� " << filename << ": " << ex.what() << '\n';
            }
        }

    private:
        std::shared_ptr<user> authorize_user() const
        {
            std::string login, password;
            std::cout << "������� �����: ";
            std::cin >> login;
            if (login.empty())
            {
                std::cout << "������: ����� �� ����� ���� ������. ���������� �����.\n";
                return nullptr;
            }

            std::cout << "������� ������: ";
            std::cin >> password;

            for (const auto& user : users_)
            {
                if (user->authenticate(login, password))
                {
                    std::cout << "�������� �����������!\n";
                    return user;
                }
            }
            std::cout << "�������� ����� ��� ������.\n";
            return nullptr;
        }

        void register_user()
    	{
            std::string login, password;
            std::cout << "������� �����: ";
            std::cin >> login;
            if (login.empty())
            {
                std::cout << "������: ����� �� ����� ���� ������. ���������� �����.\n";
                return;
            }
            if (std::any_of(users_.begin(), users_.end(), [&](const auto& user) { return user->get_login() == login; }))
            {
                std::cout << "������: ������������ � ����� ������� ��� ����������.\n";
                return;
            }
            std::cout << "������� ������: ";
            std::cin >> password;

            for (const auto& user : users_) 
            {
                if (user->get_login() == login) 
                {
                    std::cout << "������������ � ����� ������� ��� ����������.\n";
                    return;
                }
            }

            users_.push_back(std::make_shared<user>(login, user::en_decrypt_password(password), false));
            std::cout << "����������� �������!\n";
        }

        void view_results() const
        {
            std::cout << "���������� ��������� (�� ������):\n";
            for (const auto& user : users_)
	            if(!user->is_administrator())
		            for (const auto& result : results_)
                        if (user->get_login() == result->get_student_login())
                            std::cout << "�������: " << result->get_student_login() << ", ����: " << result->get_test_title() << ", ���������: " << result->get_score() << '\n';
        }

    public:
        static void end_of_function()
        {
            system("pause");
            system("cls");
        }
        void admin_menu()
    	{
            while (true) 
            {
                std::cout << "1. ���������� �������\n2. �������� �����������\n3. ����� � ���������� ������\n4. ����������� �����\n5. �����\n�������� �����: ";
                int choice;
                std::cin >> choice;
                if (std::cin.fail()) 
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "������: ������������ ����\n";
                    continue;
                }


                switch (choice)
            	{
                case 1:
                    manage_tests();
                    break;
                case 2:
                    view_results();
                    break;
                case 3:
                    search_and_sort_tests();
                    break;
                case 4:
                    filter_tests();
                    break;
                case 5:
                    return;
                default:
                    std::cout << "�������� ����� ����. ���������� �����.\n";
                }
            }
        }

        void filter_tests() const
        {
            std::cout << "�������� �����:\n1. ���������� ������ �� ���-�� �������� (�� ������ ��� n ��������)\n2. ���������� ������ �� ���-�� �������� (�� ������ ��� n ��������)\n��� �����: ";
            int choice;
            std::cin >> choice;
            if (std::cin.fail()) 
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "������: ������������ ����\n";
                return;
            }


            std::cout << "������� ���������� ��������: ";
            unsigned num;
            std::cin >> num;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "������: ������������ ����\n";
                return;
            }

            std::cout << "��������� �����:\n";

            if (choice == 1)
            {
                for (const auto& test : tests_)
                    if (test->get_questions().size() >= num)
                        std::cout << test->get_title() << '\n';
            }
            else if (choice == 2)
            {
                for (const auto& test : tests_)
                    if (test->get_questions().size() <= num)
                        std::cout << test->get_title() << '\n';
            }
            else
                std::cout << "������������ �����.\n";
        }

        void manage_tests()
    	{
            while (true) 
            {
                std::cout << "1. ������� ����\n2. ������������� ����\n3. ������� ����\n4. �����\n�������� �����: ";
                int choice;
                std::cin >> choice;
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "������: ������������ ����\n";
                    continue;
                }

                switch (choice) {
                case 1:
                    create_test();
                    break;
                case 2:
                    edit_test();
                    break;
                case 3:
                    delete_test();
                    break;
                case 4:
                    return;
                default:
                    std::cout << "�������� ����� ����. ���������� �����.\n";
                }
            }
        }

        void create_test()
    	{
            std::cout << "������� �������� �����: ";
            std::string title;
            std::cin.ignore();
            std::getline(std::cin, title);
            if (title.empty())
            {
                std::cout << "������: �������� ����� �� ����� ���� ������. ���������� �����.\n";
                return;
            }


            const auto te = std::make_shared<test>(title);
            while (true) 
            {
                std::cout << "�������� ������? (1 - ��, 0 - ���): ";
                int choice;
                std::cin >> choice;
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "������: ������������ ����\n";
                    continue;
                }
                if (choice == 0) break;

                std::cin.ignore();
                std::string text;
                std::cout << "������� ����� �������: ";
                std::getline(std::cin, text);

                std::vector<std::string> answers;
                for (int i = 0; i < 4; ++i)
                {
                    std::cout << "������� ������� ������ " << i + 1 << ": ";
                    std::string answer;
                    std::getline(std::cin, answer);
                    if (answer.empty())
                    {
                        std::cout << "������: ������� ������ �� ����� ���� ������. ���������� �����.\n";
                        --i;
                        continue;
                    }
                    answers.push_back(answer);
                }

                int correct;
                std::cout << "������� ����� ����������� ������ (1-4): ";
                std::cin >> correct;
                if (std::cin.fail() || correct < 1 || correct > 4)
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "������: ������� ���������� ����� ����������� ������ (1-4).\n";
                    continue;
                }


                te->add_question(question(text, answers, correct - 1));
            }
            tests_.push_back(te);
            std::cout << "���� ������� ������.\n";
        }

        void edit_test() const
        {
            if (tests_.empty()) 
            {
                std::cout << "��� ��������� ������ ��� ��������������.\n";
                return;
            }
            print_tests();
            std::cout << "������� ����� ����� ��� ��������������: ";
            size_t index;
            std::cin >> index;
            if (std::cin.fail() || index < 1 || index > tests_.size()) 
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "������: ������������ ����� �����. ���������� �����.\n";
                return;
            }

            auto& test = tests_[index - 1];
            test->print_questions();

            std::cout << "������� ����� ������� ��� ��������������: ";
            size_t q_index;
            std::cin >> q_index;
            if (q_index < 1 || q_index > test->get_questions().size()) 
            {
                std::cout << "�������� �����.\n";
                return;
            }

            std::cin.ignore();
            std::string text;
            std::cout << "������� ����� ����� �������: ";
            std::getline(std::cin, text);

            std::vector<std::string> answers;
            for (int i = 0; i < 4; ++i)
            {
                std::cout << "������� ����� ������� ������ " << i + 1 << ": ";
                std::string answer;
                std::getline(std::cin, answer);
                answers.push_back(answer);
            }

            int correct;
            std::cout << "������� ����� ����������� ������ (1-4): ";
            std::cin >> correct;
            if (std::cin.fail() || correct < 1 || correct > 4)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "������: ������� ���������� ����� ����������� ������ (1-4).\n";
                return;
            }

            test->edit_question(q_index - 1, question(text, answers, correct - 1));
            std::cout << "������ ������� ��������������.\n";
        }

        void delete_test()
    	{
            if (tests_.empty()) 
            {
                std::cout << "��� ��������� ������ ��� ��������.\n";
                return;
            }
            print_tests();
            std::cout << "������� ����� ����� ��� ��������: ";
            size_t index;
            std::cin >> index;
            if (std::cin.fail() || index < 1 || index > tests_.size()) 
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "������: ������������ ����� �����. ���������� �����.\n";
                return;
            }
            tests_.erase(tests_.begin() + static_cast<long long>(index) - 1);
            std::cout << "���� ������� �����.\n";
        }

        void print_tests() const
    	{
            if (tests_.empty())
            {
                std::cout << "��� ��������� ������.\n";
                return;
            }
            for (size_t i = 0; i < tests_.size(); ++i)
                std::cout << i + 1 << ". " << tests_[i]->get_title() << '\n';

        }

        void take_test(const std::shared_ptr<user>& user)
    	{
            if (tests_.empty()) 
            {
                std::cout << "��� ��������� ������.\n";
                return;
            }
            print_tests();
            std::cout << "������� ����� ����� ��� �����������: ";
            size_t index;
            std::cin >> index;
            if (std::cin.fail() || index < 1 || index > tests_.size()) 
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "������: ������������ ����� �����. ���������� �����.\n";
                return;
            }

            const auto& test = tests_[index - 1];
            int score = 0;
            for (auto question : test->get_questions()) 
            {
                std::cout << question.get_text() << "\n";

                const auto& answers = question.get_answers();
                for (size_t i = 0; i < answers.size(); ++i)
                    std::cout << i + 1 << ". " << answers[i] << "\n";

                std::cout << "��� �����: ";
                size_t answer;
                std::cin >> answer;
                if (std::cin.fail() || answer < 1 || answer > answers.size())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "������: ������� ����� ������ �� 1 �� " << answers.size() << ".\n";
                    continue;
                }
                if (question.is_correct(static_cast<int>(answer) - 1))
                    ++score;
            }
            std::cout << "�� ��������� ����. ��� ���������: " << score << "/" << test->get_questions().size() << "\n";

            results_.push_back(std::make_shared<result>(user->get_login(), test->get_title(), score));
        }

        void view_student_results(const std::shared_ptr<user>& user) const
        {
            std::cout << "���� ����������:\n";
            bool has_results = false;
            for (const auto& result : results_) 
            {
                if (result->get_student_login() == user->get_login()) 
                {
                    std::cout << "����: " << result->get_test_title()
                        << " | ���������: " << result->get_score() << "\n";
                    has_results = true;
                }
            }
            if (!has_results)
                std::cout << "� ��� ���� ��� �����������.\n";
        }

        void search_and_sort_tests()
    	{
            std::cout << "�������� �����:\n1. ����� �� ��������\n2. ���������� �� ��������\n��� �����: ";
            int choice;
            std::cin >> choice;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "������: ������������ ����\n";
                return;
            }

            if (choice == 1) 
            {
                std::cout << "������� �������� �����: ";
                std::string keyword;
                std::cin.ignore();
                std::getline(std::cin, keyword);

                std::cout << "��������� �����:\n";
                for (const auto& test : tests_)
                    if (test->get_title().find(keyword) != std::string::npos)
                        std::cout << test->get_title() << '\n';
            }
            else if (choice == 2) 
            {
                std::sort(tests_.begin(), tests_.end(), [](const std::shared_ptr<test>& a, 
                    const std::shared_ptr<test>& b) {
                    return a->get_title() < b->get_title();
                    });
                std::cout << "����� ������������� �� ��������:\n";
                for (const auto& test : tests_)
                    std::cout << test->get_title() << '\n';
            }
            else
                std::cout << "������������ �����.\n";
        }

        void student_menu(const std::shared_ptr<user>& user)
    	{
            while (true) 
            {
                std::cout << "1. ������ ����\n2. ����������� ���� ����������\n3. ����� � ���������� ������\n4. ���������� ������\n5. �����\n�������� �����: ";
                int choice;
                std::cin >> choice;
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "������: ������������ ����\n";
                    continue;
                }

                switch (choice)
            	{
                case 1:
                    take_test(user);
                    break;
                case 2:
                    view_student_results(user);
                    break;
                case 3:
                    search_and_sort_tests();
                    break;
                case 4:
                    filter_tests();
                    break;
                case 5:
                    return;
                default:
                    std::cout << "�������� ����� ����. ���������� �����.\n";
                }
            }
        }

        void run()
    	{
            load_users("users.txt");
            load_tests("tests.txt");
            load_results("results.txt");

            while (true) 
            {
                std::cout << "1. �����������\n2. �����������\n3. �����\n�������� �����: ";
                int choice;
                std::cin >> choice;
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "������: ������������ ����\n";
                    continue;
                }

                switch (choice)
            	{
                case 1: 
                {
                    try
                    {
                        auto user = authorize_user();
                        if (user)
                        {
                            if (user->is_administrator())
                                admin_menu();
                            else
                                student_menu(user);
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        std::cerr << "������ ��� �����������: " << ex.what() << '\n';
                    }
                    break;
                }
                case 2:
                    register_user();
                    break;
                case 3:
                    save_users("users.txt");
                    save_tests("tests.txt");
                    save_results("results.txt");
                    return;
                default:
                    std::cout << "�������� ����� ����. ���������� �����.\n";
                }
            }
        }
    };
}

int main()
{
	// ReSharper disable once CppDeclaratorNeverUsed
	const auto locale = setlocale(LC_ALL, "Russian");
    try 
    {
        app::application app;
        app.run();
    }
    catch (const std::exception& ex) 
    {
        std::cerr << "������: " << ex.what() << '\n';
    }
    return 0;
}
