#include <iostream>
#include <map>
#include <functional>
#include <queue>
#include <sstream>
#include <stack>
#include <cctype>

int priority(char op){
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

std::queue<std::string> toPostfix(const std::string &input)     //infix -> postfix
{
    std::stack<char> ops;
    std::queue<std::string> output;
    std::stringstream number;

    for (size_t i=0; i<input.size(); i++) {
        char c = input[i];
        if (std::isspace(c)) continue;

        if (std::isdigit(c) || c == '.') {
            number.str(""); number.clear();
            while (i<input.size() && (std::isdigit(input[i]) || input[i] == '.'))
                number << input[i++];
            output.push(number.str());
            i--;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!ops.empty() && priority(ops.top()) >= priority(c)) {    //jeśli priorytet wynosi 1 lub 2
                output.push(std::string(1, ops.top()));
                ops.pop();
            }
            ops.push(c);
        }
    }
    while (!ops.empty()) {
        output.push(std::string(1, ops.top()));
        ops.pop();
    }
    return output;
}

double evaluatePostfix(std::queue<std::string> postfix)
{
    std::stack<double> stack;
    while (!postfix.empty()) {
        std::string token = postfix.front(); postfix.pop();

        if (std::isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            stack.push(std::stod(token));
        } else {
            double b = stack.top(); stack.pop();
            double a = stack.top(); stack.pop();
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
    return stack.top();
}

double calculator(const std::string &input)
{
    std::queue<std::string> postfix = toPostfix(input);
    return evaluatePostfix(postfix);
}

int main(){
    std::string input;
    std::cout << "Enter the operration:\n   ";
    std::getline(std::cin, input);

    system("cls");
    try
        { std::cout << input << " = " << calculator(input); }
    catch (const std::exception &e)
        { std::cerr << "Error: " << e.what() << std::endl; }

    std::cout << "\n___________________";
    std::cout << "\nPress Enter to Exit";
    std::cin.ignore();
    return 0;
}