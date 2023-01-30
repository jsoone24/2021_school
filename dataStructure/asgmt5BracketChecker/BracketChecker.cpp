#include <stack>
#include <string>
#include <iostream>

int BracketChecker(std::string UserInput)
{
        if(UserInput.length() < 2)       //입력 괄호가 하나면 -1 리턴
                return -1;
        std::stack<char> BracketStack;  //Stack 선언

        for (std::string::iterator iter = UserInput.begin(); iter != UserInput.end(); ++iter)   //UserInput iterate 함
        {
                char n = *iter;

                if (n == '(' || n == '{' || n == '[')   //여는 괄호는 스택에 푸시한다.
                        BracketStack.push(n);
                else    //여는 괄호가 아닌경우
                {
                        if (BracketStack.empty())       //Stack이 비어있을때 Stack.top()으로 인한 Segfault 발생방지
                                return -1;

                        char top = BracketStack.top();  //Stack 의 top을 저장
                        if (top == '(' && n == ')')     //top에 따라 input 비교
                                BracketStack.pop();
                        else if (top == '{' && n == '}')
                                BracketStack.pop();
                        else if (top == '[' && n == ']')
                                BracketStack.pop();
                        else    //다른 문자인 경우 에러 발생
                                return -1;
                }
        }

        if (BracketStack.empty())       //루프의 끝은 스택에 아무것도 없어야하므로 아무것도 없을때 리턴 0
                return 0;
        else                            //만약 스택에 무엇이 남아있는경우 잘못 된 경우이므로 리턴 -1
                return -1;
}

int main()
{
        std::string UserInput;

        std::cout << "type bracket : ";
        std::cin >> UserInput;  //User로 부터 문자열 입력받음

        if (BracketChecker(UserInput) < 0)      //결과에 따라 출력
                std::cout << "bad bracket" << std::endl;
        else
                std::cout << "good bracket" << std::endl;

        return 0;
}