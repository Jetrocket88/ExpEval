#include <iostream>  
#include <stack>
#include <queue>
#include <vector>  
#include <sstream>  
#include <string>  
#include <numbers>

#define _USE_MATH_DEFINES

int getPrecedence(const std::string& s) {
    if (s == "^") return 3;
    if (s == "*" || s == "/") return 2;
    if (s == "+" || s == "-") return 1;
    return 0;
}

bool isLeftAssociative(const std::string& s) {
    return s != "^";
}

bool isOperator(const std::string& s) {
    return (s == "+" || s == "-" || s == "*" || s == "/" || s == "^");
}

bool isFunction(const char& c) {
    return (c == 's' || c == 'c' || c == 't');
}

std::vector<std::string> tokenise(const std::string& s) {
   //input serialisation into tokens
   std::vector<std::string> tokens;  
   for (size_t i = 0; i < s.length(); i++) {  
       if(std::isdigit(s[i])) {
           std::string num;
           while (i < s.length() && (std::isdigit(s[i]) || s[i] == '.')) {
               num += s[i];
               i++;
           }
           tokens.push_back(num);
           i--;
       }  
       else if (isOperator(std::string(1, s[i]))) { //is operator
           tokens.push_back(std::string(1, s[i])); 
       }  
       else if (isFunction(s[i])) { // is a function
           std::string func;
           if (s[i] == 's') func = "sin";
           else if (s[i] == 'c') func = "cos";
           else if (s[i] == 't') func = "tan";
           i += 2;
           tokens.push_back(func);
       }
       else if (s[i] == ')' || s[i] == '(') {
           tokens.push_back(std::string(1, s[i]));
       }
   }  
   return tokens;
}

std::vector<std::string> convertToPostfix(const std::vector<std::string>& tokens) {
   //shunting yard algo
   std::vector<std::string> outputVec;
   std::stack<std::string> opStack;

   for (const std::string token : tokens) {
       if (std::isdigit(token[0])) {
           outputVec.push_back(token);
       }
       else if (isOperator(token)) {
           while (!opStack.empty() && isOperator(opStack.top()) &&
               (
                   (getPrecedence(opStack.top()) > getPrecedence(token)) ||
                   (getPrecedence(opStack.top()) == getPrecedence(token) && isLeftAssociative(token))
                   ) &&
               (opStack.top() != "("))
           {
               outputVec.push_back(opStack.top());
               opStack.pop();
           }
           opStack.push(token);
       }
       else if (token == ")") {
           while (!opStack.empty() && opStack.top() != "(") {
               outputVec.push_back(opStack.top());
               opStack.pop();
           }
           if (!opStack.empty() && opStack.top() == "(") {
               opStack.pop();
           }
           if (!opStack.empty() && isFunction(opStack.top()[0])) {
               outputVec.push_back(opStack.top());
               opStack.pop();
           }
       }
       else if (token == "(") {
           opStack.push(token);
       }
       else if (isFunction(token[0])) {
           opStack.push(token);
       }
   }
   while (!opStack.empty()) {
       outputVec.push_back(opStack.top());
       opStack.pop();
   }
   return outputVec;
}

float evalPostfix(const std::vector<std::string>& outputVec) {  
   //evaluating the posfix expression
   std::stack<float> valStack;
   for (const std::string token : outputVec) {
       if (std::isdigit(token[0])) {
           valStack.push(std::stof(token));
       }
       else if (isOperator(token)) {
           float val1 = valStack.top();
           valStack.pop();
           float val2 = valStack.top();
           valStack.pop();
           if      (token == "+") valStack.push(val1 + val2);
           else if (token == "-") valStack.push(val2 - val1);
           else if (token == "*") valStack.push(val1 * val2);
           else if (token == "/") valStack.push(val2 / val1);
           else if (token == "^") valStack.push(std::pow(val2, val1));
       }
       else if (isFunction(token[0])) {
           float val1 = valStack.top();
           valStack.pop();
           float rads = val1 * (3.14159 / 180);
           if (token[0] == 's') valStack.push(std::sin(rads));
           else if (token[0] == 'c') valStack.push(std::cos(rads));
           else if (token[0] == 't') valStack.push(std::tan(rads));
       }
   }
   return valStack.top();  
}  

int main()  
{  
   std::string exp;
   std::getline(std::cin, exp);

   std::vector<std::string> tokens = tokenise(exp);
   std::vector<std::string> outputVec = convertToPostfix(tokens);
   std::cout << evalPostfix(outputVec) << std::endl;
}
