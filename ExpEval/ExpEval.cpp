#include <iostream>  
#include <stack>
#include <queue>
#include <vector>  
#include <sstream>  
#include <string>  

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
    if (s == "+" || s == "-" || s == "*" || s == "/" || s == "^") return true;
    return false;
}

float evalExpression(std::string s) {  
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
       else if (!std::isspace(s[i])) {  
           tokens.push_back(std::string(1, s[i])); 
       }  
   }  

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
       }
       else if (token == "(") {
           opStack.push(token);
       }
   }
   while (!opStack.empty()) {
       outputVec.push_back(opStack.top());
       opStack.pop();
   }

   //evaluating the posfix expression
   std::stack<float> valStack;
   for (const std::string token : outputVec) {
       if (std::isdigit(token[0])) {
           valStack.push(std::stoi(token));
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
   }

   return valStack.top();  
}  

int main()  
{  
   std::string exp;
   std::cin >> exp;
   float result = evalExpression(exp);  
   std::cout << result << std::endl;
   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
   std::cout << "Press enter to exit";
   std::cin.get();
}
