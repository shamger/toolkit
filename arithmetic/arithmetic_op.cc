#include <stack>
#include <limits>
#include "arithmetic_op.h"

bool isoperator(const char c)
{
    if (c == '+' || c == '-' ||
        c == '*' || c == '/' ||
        c == '(' || c == ')')
        return true;
    else
        return false;
}

int64_t calcPostfix(const std::string& postfix)
{
    std::stack<int64_t> num_stack;
    for (std::size_t i = 0; i < postfix.length(); i++)
    {
        if (isdigit(postfix[i]))
        {
            num_stack.push(postfix[i]);
            continue;
        }
        else if (num_stack.size() < 2) 
            return std::numeric_limits<int64_t>::max();

        int64_t b = num_stack.top() - '0';
        num_stack.pop();
        int64_t a = num_stack.top() - '0';
        num_stack.pop();

        if ('+' == postfix[i])
            num_stack.push(a + b + '0');
        else if ('-' == postfix[i]) 
            num_stack.push(a - b + '0');
        else if ('*' == postfix[i])
            num_stack.push(a * b + '0');
        else if ('/' == postfix[i])
            num_stack.push(a / b + '0');
        else
            return std::numeric_limits<int64_t>::min();
    }
    return num_stack.top() - '0';
}

bool isTopLower(const char top, const char i)
{
    if ((i == '*' || i == '/') && (top == '+' || top == '-'))
        return true;
    else 
        return false;
}

std::string changePostfix(const std::string& input)
{
    std::stack<char> stack;
    std::string output;
    for (std::size_t i = 0; i < input.length(); i++)
    {
        if (isdigit(input[i]))
        {
            output.push_back(input[i]);
            continue;
        }

        if (isoperator(input[i]))
        {
            if (stack.empty())
                stack.push(input[i]);
            else if ('(' == input[i])
                stack.push(input[i]);
            else if (')' == input[i])
            {
                if (stack.empty()) return "error express";
                char top;
                do
                {
                    top = stack.top();
                    if (top != '(')
                        output.push_back(top);
                    stack.pop();
                } while (top != '(');
            }
            else if ('(' == stack.top())
                stack.push(input[i]);
            else 
            {
                if (!isTopLower(stack.top(), input[i]))
                {
                    char top;
                    do
                    {
                        top = stack.top();
                        output.push_back(top);
                        stack.pop();
                    } while(!stack.empty() && !isTopLower(stack.top(), input[i]) && stack.top() != '(');
                }

                stack.push(input[i]);
            }
        }
    }

    while (!stack.empty())
    {
        output.push_back(stack.top());
        stack.pop();
    }
    return output;
}

int64_t arithmetic(const std::string& instr, 
                   std::string *dbg_str, 
                   std::string *err_str)
{
    std::string postfix_express = changePostfix(instr);
    if (dbg_str)
        dbg_str->append("input:").append(instr).append("\n")
                .append("postfix:").append(postfix_express).append("\n");

    return calcPostfix(postfix_express);
}

