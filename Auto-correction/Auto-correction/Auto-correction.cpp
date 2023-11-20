#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <locale>
#include <functional>

using namespace std;

class Trie {
public:
    struct Node {
        wstring data;
        bool is_end = false;
        unordered_map<wchar_t, Node*> next;

        Node() = default;
        Node(const wstring& s) : data(s) {}
        Node(wstring str, bool is_end) :data(str), is_end(is_end) {}
    };
    ~Trie()
    {
        destruct(root);
    }

    Trie(locale& loc) :loc(loc)
    {
    }

 
    // вызывает insert_recursive, сложность описана ниже
    void insert( wstring str) {
        transform(str.begin(), str.end(), str.begin(), bind(tolower<wchar_t>, placeholders::_1, cref(loc)));
        wstring word = str;

        if (word.empty()) {
            return;
        }
        if (!root) {
            root = new Node(word, true);
        }
        insert_recursive(word, root);
    }

    //Эта функция выполняет проверку, принадлежит ли строка дереву.Алгоритм работает с временной сложностью O(n)
    //n - длина строки.
    // Поиск префиксов осуществляется за константное время.
    // Мы просто проходим по префиксам и, если такой префикс существует, спускаемся в соответствующий узел, повторяя процесс до конца слова.
    // Если хотя бы на одном уровне не удается найти соответствующий префикс, функция возвращает false.
    bool contain( wstring str) {
       
        transform(str.begin(), str.end(), str.begin(), bind(tolower<wchar_t>, placeholders::_1, cref(loc)));
        wstring word = str;
        auto curr = root;
       
       while(!word.empty() && curr->next.find(word[0]) != curr->next.end()) {
            if (word.find(curr->next[word[0]]->data) == 0) {
                wchar_t temp = word[0];
                word.erase(0, curr->next[word[0]]->data.length());
                curr = curr->next[temp];
            }
            else {
                break;
            }
       }

        if (word.empty() && curr->is_end) {
            return true;
        }
        return false;
    }

    // создаст вектор ответов и отсортирует, вызывет check_word
    vector<wstring> check_error( wstring str) {
        transform(str.begin(), str.end(), str.begin(), bind(tolower<wchar_t>, placeholders::_1, cref(loc)));
        wstring word = str;

        vector<wstring> vec;
        check_word(word, vec);
        sort(vec.begin(), vec.end());
        return vec;
    }


private:

    //Алгоритм Дамерау-Левенштейна
    //O(n*m)  
    // n,m - длины входных слов
    // Считает расстояние Левенштейна
    int dam_lev_dist(const wstring& A, const wstring& B) {
        vector<vector<int>> matr(A.length() + 1, vector<int>(B.length() + 1));

        int cost;
        for (int i = 0; i <= A.length(); i++) {
            matr[i][0] = i;
        }
        for (int j = 0; j <= B.length(); j++) {
            matr[0][j] = j;
        }

        for (int i = 1; i <= A.length(); i++) {
            for (int j = 1; j <= B.length(); j++) {
                if (A[i - 1] == B[j - 1]) {
                    cost = 0;
                }
                else {
                    cost = 1;
                }
                matr[i][j] = min(matr[i - 1][j] + 1,
                    min(matr[i][j - 1] + 1, matr[i - 1][j - 1] + cost));

                if ((i > 1) && (j > 1) && (A[i - 1] == B[j - 2]) && (A[i - 2] == B[j - 1])) {
                    matr[i][j] = min(matr[i][j], matr[i - 2][j - 2] + cost);
                }
            }
        }
        return matr[A.length()][B.length()];
    }

     //Этот алгоритм имеет временную сложность O(n⋅m⋅k), где
     //   n - длина входного слова,
     //   m - количество вершин в дереве,
     //   k - длина наибольшего префикса.
     // Алгоритм проходит по префиксам.
     // Если расстояние Левенштейна между префиксом и частью входной строки больше 2, алгоритм завершает ветвь.
     // Если расстояние Левенштейна между префиксом и частью входной строки равно 0 или 1, алгоритм спускается на уровень ниже.
    void check_word(wstring str, vector<wstring>& vec, Node* it = nullptr, wstring answer = L"") {

        auto curr = (it != nullptr) ? it : root;
        wstring temp_str = str.substr(0, answer.length());
        if (dam_lev_dist(temp_str, answer) > 2) {
            return;
        }
        if (curr->is_end && dam_lev_dist(str, answer) == 1 && find(vec.begin(), vec.end(), answer) == vec.end()) {
             vec.push_back(answer);
        }
        for (auto temp : curr->next) {
            check_word(str, vec, temp.second, answer + temp.second->data);
        }
    }

    // O(n) где n - длина входногго слова
    // Алгоритм осуществляет поиск среди ключей у детей узла первого символа строки. 
    // Если совпадение найдено, алгоритм ищет максимальный префикс и уменьшает длину строки на длину найденного префикса. 
    // Затем процесс повторяется рекурсивно для уменьшенной строки. 
    // Поиск среди ключей у детей происходит за константное время, что обеспечивает эффективность алгоритма.

    void insert_recursive(const wstring& word, Node* current) {
        if (word.empty()) {
            return;
        }

        auto next = current->next.find(word[0]);

        if (next != current->next.end()) {
            wstring prefix;
            for (size_t i = 0; ; ++i) {
                if (i == min(word.length(), next->second->data.length()) || word[i] != next->second->data[i]) {
                    prefix = word.substr(0, i);
                    break;
                }
            }
            size_t i = prefix.size();


            wstring remainingWord = word.substr(i);
            wstring postfixNextStr = next->second->data.substr(i);

            if (postfixNextStr.empty()) {
                if (remainingWord.empty()) {
                    if (!next->second->is_end) {
                        next->second->is_end = true;
                    }
                }
                else {
                    insert_recursive(remainingWord, next->second);
                }
            }
            else {
                auto tail = new Node(prefix);
                tail->next.insert({ postfixNextStr[0], next->second });

                next->second->data = postfixNextStr;

                current->next.erase(next);
                current->next.insert({ prefix[0], tail });

                if (remainingWord.empty()) {
                    tail->is_end = true;
                }
                else {
                    auto new_n = new Node(remainingWord);
                    new_n->is_end = true;
                    tail->next.insert({ remainingWord[0], new_n });
                }
            }
        }
        else {
            auto new_n = new Node(word);
            new_n->is_end = true;
            current->next.insert({ word[0], new_n });
        }
    }

    // Деструктор O(n) 
    // n - размер дерева
    void destruct(Node* curr) {
        if (curr != nullptr) {
            for (auto temp : curr->next) {
                destruct(temp.second);
            }
            delete curr;
        }
    }

    locale loc;
    Node* root = nullptr;
};


int main() {
    ios_base::sync_with_stdio(false);
    locale loc{ "" };
    wcin.imbue(loc);
    wcout.imbue(loc);

    Trie t(loc);
    int count;
    wstring str;
    getline(wcin, str);
    count = stoi(str);
    size_t j = 0;
    while (j < count) {
        getline(wcin, str);
        t.insert(str);
        ++j;
    }
    while (getline(wcin, str)) {
        wstring copy_upper = str;
        transform(str.begin(), str.end(), str.begin(), bind(tolower<wchar_t>, placeholders::_1, cref(loc)));
        if (str.empty()) {
            continue;
        }
        if (count == 0) {
            wcout << copy_upper << " -?\n";
            continue;
        }
        if (t.contain(str)) {
            wcout << copy_upper << " - ok\n";
        }
        else {

            vector<wstring> answ = t.check_error(str);
            for (size_t i = 0; i < answ.size(); i++)
            {
                if (answ[i] == L"") {
                    answ.erase(answ.begin() + i);
                }
            }
            if (answ.empty()) {
                wcout << copy_upper << " -?\n";
            }
            else {

                wcout << copy_upper << " -> ";
                for (size_t i = 0; i < answ.size(); i++)
                {
                    if (i == 0) {
                        wcout << answ[i];
                    }
                    else {
                        wcout << ", " << answ[i];
                    }
                }
                wcout << endl;
            }
        }
    }
}