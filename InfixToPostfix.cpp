#include <iostream>
#include <vector>
#include<map>
#include<string.h>
#include <cmath>
using namespace std;

//class stack to implement stacks of generic data-type
template <typename T>
class stack {
    private:
        T* Stack; 			//declaring a pointer array of generic datatype
        int Size;           //variable to store stack size
        int top = -1;  			//top is -1 when stack is empty
        bool isEmpty = false, isFull = false;  //bool to check if stack is empty or full
    
    public:
        stack(int s) { 			//for creating stack of user-defined size
            Size = s;
            Stack = new T[s];
        }
    
        stack() {  				// for creating stack of default size 50
            Size = 50;
            Stack = new T[50];
        };
    
        //function to check if stack is empty
        bool IsEmpty() {
            return top <= -1; 			//if top is -1, stack is empty
        }
    
        //function to check if bool is full
        bool IsFull() {
            return top >= (Size - 1); 		//if top equals to (size-1) stack is full
        }
    
        //function to insert an element in stack
        void push(T value) {
            if (!IsFull()) {  			//only add if stack is not full
                Stack[++top] = value; 		//incrementing top and adding new value at top
            }
        }
    
        //function to pop last added element from stack without returning
        void pop() {
            if (!IsEmpty()) { 			//only pop top element if stack is not empty
                top--; 				//decrementing top by 1 for each removed element
            }
        }
    
        //function to return top element from stack without removing it
        T peek() {
            if (!IsEmpty()) { 			//checking if stack contains elements
                return Stack[top]; 		//returning top element
            }
        }
    
        //function to get stack size
        int size() { 
            return Size;
        }
    
        //function to make stack empty
        void clear() {
            top = -1;
        }
};

class Postfix {
    
    //making utility functions private
    private:
        
        //function to split expression based on empty spaces and store in vector
        vector <string> splitOnSpace(string expr) {
        	vector <string> tokens;     //initializing string vector
        	string token = "";          //initializing empty string to store each token
        	
        	//for loop to iterate for each element in expression
        	for (int i = 0; i < expr.length(); i++) {   
        		if (isspace(expr[i])) { //if space is found
        			tokens.push_back(token); //push the token into vector
        			token = "";
        		}
        		else
        			token += expr[i]; //storing non-space elements in token
        	}
        	tokens.push_back(token); //pushing last element in vector
        	return tokens;
        }
        
        
        //function to check if element is opening bracket
        bool isOpeningBracket(string ele) {
            return ele == "{" || ele == "[" || ele == "(";
        }
        
        //function to check if element is closing bracket
        bool isClosingBracket(string ele) {
            return ele == "}" || ele == "]" || ele == ")";
        }
        
        //function to check if element is an operator
        bool isOperator(string ele) {
            return ele == "+" || ele == "-" || ele == "*" || ele == "/" || ele == "^";
        }
        
        //function to check if oper1 has lower precedence than operator 2
        bool getPrecedence(string oper1, string oper2)
        {
            map <string, int> prec = { {"^", 3}, {"*", 2}, {"/", 2}, {"+", 1}, {"-", 1} };
            return prec[oper1] <= prec[oper2];
        }
        
        //function to evaluate top two elements of stack based on operator found 
        float evaluate(float num1, float num2, string oper) {
            if ( oper == "+") {
                return num1 + num2;
            } else if (oper == "-"){
                return num1 - num2;   
            } else if (oper == "*"){
                return num1 * num2;    
            } else if (oper == "/"){
                return num1 / num2;    
            } else if (oper == "^"){
                return pow(num1, num2);   
            }
        }

    public:
        //function to convert infix expression to postfix
        string infixToPostfix(string expr) {
        	stack <string> s (expr.length()); //initializing string stack equal to expression size
        	string postfix;                 //initializing string to store postfix expression
        	vector <string> tokens = splitOnSpace(expr); //splitting given expression based on spaces
        	
        	//for loop to run for each token
        	for (int i = 0; i < tokens.size(); i++) {
        		if (isOpeningBracket(tokens[i])) { //if token is an opening bracket
        			s.push(tokens[i]); //pushing it into stack
        		}
        		
        		else if (isClosingBracket(tokens[i])) { //if token is a closing bracket
        			if (tokens[i] == ")") {  //looking for each type of closing bracket
        				while (s.peek() != "(") {  //runs until equivalent opening bracket is found
        					postfix += " " + s.peek(); //removing elements from stack and adding to postfix expression
        					s.pop();
        				}
        			}
        			else if (tokens[i] == "}") { 
        				while (s.peek() != "{") { 
        					postfix += " " + s.peek();
        					s.pop(); 
        				}
        			}
        			else if (tokens[i] == "]") { 
        				while (s.peek() != "[") { 
        					postfix += " " + s.peek();
        					s.pop(); 
        				}
        			}
        			s.pop();
        		}
        		
        		else if (isOperator(tokens[i])) {  //if token is an operator
        			while (!s.IsEmpty() && getPrecedence(tokens[i], s.peek())) { //comparing precedence token and stack top
        				postfix += " " + s.peek(); //if precedence of operator in stack top is more pop it
        				s.pop(); //removing stack top elements lower precedence operator is found or it becomes empty
        			}
        			s.push(tokens[i]); //adding operator to stack
        		}
        		
        		else {      //if token is a number
        			postfix += " " + tokens[i]; //adding numbers to postfix expression
        		}
        
        	}
        	
        	while (!s.IsEmpty()) { //emptying all the leftover elements in stack
        		postfix += " " + s.peek(); //adding them to output expression
        		s.pop();
        	}
        	return postfix;
        }
        
        //function to evaluate postfix expression
        int evaluatePostfix(string postfix) {
        	vector <string> tokens = splitOnSpace(postfix); //splitting postfix expression on basis of space
        	stack <float> s(postfix.length()); //initializing a stack of type float
        	float num1, num2, result;
        
            for (int i = 0; i < tokens.size() ; i++) //for loop to run for each token
            {
                    if (isdigit(tokens[i][0]))    //if token is a digit push it into stack
                        s.push(stof(tokens[i])); //converting token to float from string
                    else                        //if token is an operator
                    {
                        //poping top two elements from stack
                        num2 = s.peek();
                        s.pop();
                        num1 = s.peek();
                        s.pop();
                        result = evaluate(num1, num2, tokens[i]); //evaluating num1 and num2 based on operator token found
                        s.push(result);  //pushing the result back into stack
                    }
        
            }
            
            result = s.peek();  //result is equal to final number left in stack
            s.pop();   //emptying the stack
            return result;
        }
};

int main()
{
    Postfix pf;         //initializing an object of postfix class to access its functions
	string infix;      //initializing string infix to get user input
	
	cout << "---------------------------------------------------------------" << endl;
	cout << "\t\t WELCOME TO POSTFIX EVALUATOR" <<endl;
	cout << "---------------------------------------------------------------\n" << endl;
	
	cout << "Enter Infix Expression With Spaces Between Operator And Operands\n(Ex. 1 + ( 12 / 3 ): ";
	getline(cin, infix); //get input
	cout << "\nPOSTFIX EXPRESSION: " ;
	cout << pf.infixToPostfix(infix) << endl; //display user
	cout << "RESULT AFTER POSTFIX EXPRESSION EVALUATION: "; 
	cout << pf.evaluatePostfix(pf.infixToPostfix(infix)) << endl; 

}