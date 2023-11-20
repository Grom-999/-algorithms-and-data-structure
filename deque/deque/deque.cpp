#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Deque {
private:
    std::string *data;
    size_t N;
    size_t sizeDec;
    size_t head;
    size_t tail;

public:
    Deque(){
        N = 0;
        data = nullptr;
        head = 0;
        tail = 0;
        sizeDec = 0;
    }

    Deque(int set_size){          
        N = set_size;
        data = new std::string[N];
        head = 0;
        tail = 0;
        sizeDec = 0;
    }

    void pushf(const std::string& X) {
        if (sizeDec == N)
        {
            std::cout << "overflow\n";
            return;
        }


        if (sizeDec == 0)
        {
            head = 0;
            tail = 0;
        }
        else if (head == 0)
        {
            head = N - 1;
        }
        else
        {
            --head;
        }

        data[head] = X;
        ++sizeDec;


    }

    void pushb(const std::string& X) {
        if (sizeDec == N)
        {
            std::cout << "overflow\n";
            return;
        }

        if (sizeDec == 0)
        {
            head = 0;
            tail = 0;
        }
        else if (tail == N - 1)
        {
            tail = 0;
        }
        else {
            ++tail;
        }

        data[tail] = X;
        ++sizeDec;
      
    }

    void popf() {
        if (sizeDec == 0)
        {
            std::cout << "underflow\n"; 
            return;
        }

        std::cout << data[head] << "\n";

        if (head == N-1)
        {
            head = 0;
        }
        else {
            ++head;
        }

        --sizeDec;
    }

    void popb() {
        if (sizeDec == 0)
        {
            std::cout << "underflow\n";
            return;
        }

        std::cout << data[tail] << "\n";
      

        if (tail == 0) {
            tail = N - 1;
        }
        else {
            --tail;
        }

        --sizeDec;
    }

    void print() {
        if (sizeDec == 0)
        {
            std::cout << "empty\n";
            return;
        }  

        int i = head;
        while (i != tail) {

            std::cout << data[i] << " ";

            i = (i + 1) % N;

        }

        std::cout << data[tail];


        std::cout << "\n";
    }

    Deque& operator=(const Deque& temp) {
        if (this == &temp)
        {
            return *this;
        }

        this->N = temp.N;
        this->head = temp.head;
        this->tail = temp.tail;

        this->data = new std::string[N];

        for (int i = 0; i < N; i++)
        {
            this->data[i] = temp.data[i];
        }

        return *this;

    }


    ~Deque() {
        if (data != nullptr)
        {
            delete[] data;
        }
    }

};

bool CheckSetSize(std::string& value) {

    if (value.empty())
    {
        return false;
    }

    if (std::count(value.begin(), value.end(), ' ') > 0)
    {
        return false;
    }

    for (const char c : value)
    {
        if ((c < '0' || c > '9' ) && c != '-')
        {
            return false;
        }
    }
    return true;
}


bool CheckPush(std::string& value) {

    if (value.empty())
    {
        return true;
    }

    if (std::count(value.begin(), value.end(), ' ') > 0)
    {
        return false;
    }

    return true;
}


int main()
{
    std::string input;
    std::string value;
    Deque dec;

    while (std::getline(std::cin, input)) {
        if (input.empty()) {
            continue;
        }
        if (input.find("set_size") == 0 && input.find(' ') == 8)
        {
            value = input.substr(9);

            if (CheckSetSize(value))
            {
                if (std::stoi(value) >= 0)
                {
                    dec = Deque(std::stoi(value));
                    break;
                }
            }

            std::cout << "error\n";

        }
        else {
            std::cout << "error\n";
        }
    }


    while (std::getline(std::cin, input))
    {
        if (input.empty()) {
            continue;
        }
        if (input.find("pushf") == 0 && input.find(' ') == 5) {
            
            value = input.substr(6);

            if (CheckPush(value)) {
                dec.pushf(value);
                continue;
            }

            std::cout << "error\n";

        }
        else if (input.find("pushb") == 0 && input.find(' ') == 5) {
       
            value = input.substr(6);

            if (CheckPush(value)) {
                dec.pushb(value);
                continue;
            }

            std::cout << "error\n";

        }
        else if (input == "popf") {
            dec.popf();
        }
        else if (input == "popb") {
            dec.popb();
        }
        else if (input == "print") {
            dec.print();
        }
        else {
            std::cout << "error\n";
        }

    }
   
 }