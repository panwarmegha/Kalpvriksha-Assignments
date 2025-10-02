#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_SIZE 100 


int values[MAX_SIZE];
int valueTop = -1;

char operators[MAX_SIZE];
int opTop = -1;


void pushValue(int val);
int popValue();
void pushOperator(char op);
char popOperator();
char peekOperator();
int getPrecedence(char op);
void performOperation();
void throwError(const char *msg);

int main()
{
    char expression[MAX_SIZE];

    
    if (fgets(expression, sizeof(expression), stdin) == NULL)
    {
        throwError("Error: Could not read expression.");
    }

    
    for (int i = 0; i < strlen(expression); i++)
    {
        
        if (isspace(expression[i]))
            continue;

        
        else if (isdigit(expression[i]))
        {
            int value = 0;
            while (i < strlen(expression) && isdigit(expression[i]))
            {
                value = value * 10 + (expression[i] - '0');
                i++;
            }
            pushValue(value);
            i--; 
        }

        
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
        {
            while (opTop != -1 && getPrecedence(peekOperator()) >= getPrecedence(expression[i]))
            {
                performOperation();
            }
            pushOperator(expression[i]);
        }

        
        else
        {
            throwError("Error: Invalid character in expression.");
        }
    }

    
    while (opTop != -1)
    {
        performOperation();
    }

    
    if (valueTop == 0 && opTop == -1)
        printf("Result: %d", popValue());
    else
        throwError("Error: Malformed expression.");

    return 0;
}

void pushValue(int val)
{
    if (valueTop >= MAX_SIZE - 1)
        throwError("Error: Value stack overflow.");
    values[++valueTop] = val;
}

int popValue()
{
    if (valueTop < 0)
        throwError("Error: Value stack underflow.");
    return values[valueTop--];
}

void pushOperator(char op)
{
    if (opTop >= MAX_SIZE - 1)
        throwError("Error: Operator stack overflow.");
    operators[++opTop] = op;
}

char popOperator()
{
    if (opTop < 0)
        throwError("Error: Operator stack underflow.");
    return operators[opTop--];
}

char peekOperator()
{
    if (opTop < 0)
        throwError("Error: No operator found.");
    return operators[opTop];
}

int getPrecedence(char op)
{
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;
}

void performOperation()
{
    int rightOperand = popValue();
    int leftOperand = popValue();
    char op = popOperator();

    switch (op)
    {
    case '+':
        pushValue(leftOperand + rightOperand);
        break;
    case '-':
        pushValue(leftOperand - rightOperand);
        break;
    case '*':
        pushValue(leftOperand * rightOperand);
        break;
    case '/':
        if (rightOperand == 0)
            throwError("Error: Division by zero.");
        pushValue(leftOperand / rightOperand);
        break;
    default:
        throwError("Error: Unknown operator.");
    }
}

void throwError(const char *msg)
{
    printf("%s", msg);
    exit(EXIT_FAILURE);
}