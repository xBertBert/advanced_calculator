#include <iostream>
#include <map>
#include <functional>
#include <queue>
#include <sstream>
#include <stack>
#include <cctype>

int priority(char op){              // koleność wykonywania działań +-*/
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

std::queue<std::string> toPostfix(const std::string &input)     // infix -> postfix
{
    std::stack<char> ops;
    std::queue<std::string> output;     // kolejka postfix
    std::stringstream number;
    bool expectUnary = true;        // flaga do obsługi liczb ujemnych i nawiasów

    for (size_t i=0; i<input.size(); i++) {
        char c = input[i];
        // pomijamy spacje na wypadek dodania jej przez użytkownika np. 2 + 4 * 5 zamiast 2+4*5
        if (std::isspace(c)) continue;

        if (std::isdigit(c) || c == '.' || (expectUnary && c == '-')) {     // obsługa liczby
            number.str(""); number.clear();
            // dodaje minus jeśli liczba jest ujemna, else wraca znak, jeśli nie jest to liczba ujemna
            if (c == '-') number << '-'; else i--;

            i++;
            // składamy liczby z kolejnych cyfr i kropek oznaczających częsci dziesiętne, setne, itd...
            while (i < input.size() && (std::isdigit(input[i]) || input[i] == '.')) {
                number << input[i++];
            }
            output.push(number.str()); i--;     // gotową liczbę odsyłamy dalej i cofamy index, bo pętla go nadpisze
            expectUnary = false;
        }
        // obsługa nawiasów
        else if (c == '(') {
            ops.push(c);
            expectUnary = true;     // po nawiasie otwierającym może być liczba ujemna
        }
        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                output.push(std::string(1, ops.top()));
                ops.pop();
            }
            if (!ops.empty() && ops.top() == '(') ops.pop();
            else throw std::runtime_error("Unmatched parentheses");
            expectUnary = false;
        }
        // operatory +-*/ z uwzględnieniem priorytetu
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!ops.empty() && priority(ops.top()) >= priority(c)) {
                if (ops.top() == '(') break;
                output.push(std::string(1, ops.top()));
                ops.pop();
            }
            ops.push(c);
            expectUnary = true;
        }
    }

    // przerzucamy pozostałe operatory do koljki
    while (!ops.empty()) {
        if (ops.top() == '(' || ops.top() == ')') throw std::runtime_error("Unmatched parentheses");
        output.push(std::string(1, ops.top()));
        ops.pop();
    }

    return output;      // zwracamy postać postfiksową
}

double evaluatePostfix(std::queue<std::string> postfix)
{
    std::stack<double> stack;           // stos do przechowywania liczb
    while (!postfix.empty()) {
        std::string token = postfix.front(); postfix.pop();

        // liczba może być ujemna
        if (std::isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            stack.push(std::stod(token));
        } else {
            // pobieramy operator i dwie liczby ze stosu
            if (stack.size() < 2) throw std::runtime_error("Invalid expression");
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
            // wykonujemy operację na switchu
            switch (token[0]) {
                case '+': stack.push(a+b); break;
                case '-': stack.push(a-b); break;
                case '*': stack.push(a*b); break;
                case '/':
                    if (b != 0) {
                        stack.push(a/b); break;
                    } else
                        throw std::runtime_error("You can't devide by 0");
            }
        }
    }
    if (stack.size() != 1) throw std::runtime_error("Invalid expression");
    return stack.top();     // wynik końcowy
}

double calculator(const std::string &input)     // główna funkcja, konwersja infix -> postfix i obliczanie
{
    std::queue<std::string> postfix = toPostfix(input);
    return evaluatePostfix(postfix);
}

int main(){
    system("title Calculator");              // tylko do tytułu okna (windows)
    std::string input;
    std::cout << "Enter the operration:\n   ";
    std::getline(std::cin, input);           // wczytujemy od użytkownika działanie
    system("cls");                           // czyścimy terminal dla przejrzystości

    try
        { std::cout << input << " = " << calculator(input); }   // wynik działania
    catch (const std::exception &e)
        { std::cerr << "Error: " << e.what() << std::endl; }    // obsługa błędów

    std::cout << "\n___________________";
    std::cout << "\nPress Enter to Exit";
    std::cin.ignore();      // czekamy na Enter przed zakończeniem aby można było sprawdzić 
    return 0;
}
