#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_SIZE 100 

int numbers[MAX_SIZE];
int numbersTop = -1;

char operators[MAX_SIZE];
int operatorsTop = -1;

void pushValue(int val);
int popValue();
void pushOperator(char operator);
char popOperator();
char peekOperator();
int getPrecedence(char operator);
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
            while (operatorsTop != -1 && getPrecedence(peekOperator()) >= getPrecedence(expression[i]))
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
 
    while (operatorsTop != -1)
    {
        performOperation();
    }
  
    if (numbersTop == 0 && operatorsTop == -1)
        printf("Result: %d", popValue());
    else
        throwError("Error: Malformed expression.");

    return 0;
}

void pushValue(int val)
{
    if (numbersTop >= MAX_SIZE - 1)
        throwError("Error: Value stack overflow.");
    numbers[++numbersTop] = val;
}

int popValue()
{
    if (numbersTop < 0)
        throwError("Error: Value stack underflow.");
    return numbers[numbersTop--];
}

void pushOperator(char operator)
{
    if (operatorsTop >= MAX_SIZE - 1)
        throwError("Error: Operator stack overflow.");
    operators[++operatorsTop] = operator;
}

char popOperator()
{
    if (operatorsTop < 0)
        throwError("Error: Operator stack underflow.");
    return operators[operatorsTop--];
}

char peekOperator()
{
    if (operatorsTop < 0)
        throwError("Error: No operator found.");
    return operators[operatorsTop];
}

int getPrecedence(char operator)
{
    if (operator == '*' || operator == '/')
        return 2;
    if (operator == '+' || operator == '-')
        return 1;
    return 0;
}

void performOperation()
{
    int rightOperand = popValue();
    int leftOperand = popValue();
    char operator = popOperator();

    switch (operator)
    {
    case '+':
        if ((rightOperand > 0 && leftOperand > INT_MAX - rightOperand) ||
            (rightOperand < 0 && leftOperand < INT_MIN - rightOperand)) {
            throwError("Error: Integer overflow in addition.");
        }
        pushValue(leftOperand + rightOperand);
        break;
    case '-':
        if ((rightOperand < 0 && leftOperand > INT_MAX + rightOperand) ||
            (rightOperand > 0 && leftOperand < INT_MIN + rightOperand)) {
            throwError("Error: Integer overflow in subtraction.");
        }
        pushValue(leftOperand - rightOperand);
        break;
    case '*':
        if (leftOperand != 0 && rightOperand != 0) {
            if (leftOperand > INT_MAX / rightOperand || leftOperand < INT_MIN / rightOperand) {
                throwError("Error: Integer overflow in multiplication.");
            }
        }
        pushValue(leftOperand * rightOperand);
        break;
    case '/':
        if (rightOperand == 0)
            throwError("Error: Division by zero.");
         if (leftOperand == INT_MIN && rightOperand == -1)
            throwError("Error: Integer overflow in division.");
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