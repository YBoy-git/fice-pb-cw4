#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define MAX_ARRAY_LENGTH 1000

#define RANDOM_LOWER_BOUND -100000
#define RANDOM_UPPER_BOUND 100000

const char inputTypes[] = {'u', 'r', '\0'};
const char sortingTypes[] = {'a', 'd', '\0'};

void swapElements(double *a, double *b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

// order = 1 for ascending, order = -1 for descending
void bubbleSort(double *array, size_t arrayLength, int order)
{
    order = order / abs(order);

    for (size_t i = 0; i < arrayLength - 1; i++)
    {
        for (size_t j = i + 1; j < arrayLength; j++)
        {
            if (order * array[i] > order * array[j])
            {
                swapElements(&array[i], &array[j]);
            }
        }
    }
}

void clearIB()
{
    while (getchar() != '\n')
        ;
}

void getInput(void *input, char *format, const char *prompt)
{
    char temp = 0;
    do
    {
        printf("%s", prompt);
        int status = scanf(format, input) + scanf("%c", &temp);
        if (status != 2 || temp != '\n')
        {
            printf("Invalid input, the format is %s, try again\n", format);
            clearIB();
        }
    } while (temp != '\n');
}

char isArrayLengthValid(size_t arrayLength)
{
    if (arrayLength < 1)
    {
        printf("Invalid input, the length must be greater than 0, try again\n");
        return 0;
    }
    else if (arrayLength > MAX_ARRAY_LENGTH)
    {
        printf("Invalid input, the length must be less than or equal to %d, try again\n", MAX_ARRAY_LENGTH);
        return 0;
    }
    return 1;
}

void getArrayLength(size_t *arrayLength, const char *prompt)
{
    do
    {
        getInput(arrayLength, "%zu", prompt);
    } while (!isArrayLengthValid(*arrayLength));
}

char isOptionValid(char choice, const char *options)
{
    if (!strchr(options, choice))
    {
        printf("Invalid option, try again\n");
        return 0;
    }
    return 1;
}

void getOption(char *choice, const char *options, const char *prompt)
{
    do
    {
        getInput(choice, "%c", prompt);
    } while (!isOptionValid(*choice, options));
}

void getRandomArrayElements(double *array, size_t arrayLength)
{
    srand(time(NULL));
    for (size_t i = 0; i < arrayLength; i++)
    {
        array[i] = (double)rand() / RAND_MAX * (RANDOM_UPPER_BOUND - RANDOM_LOWER_BOUND) + RANDOM_LOWER_BOUND;
    }
}

void getArrayElementsFromUser(double *array, size_t arrayLength, const char *prompt)
{
    printf("%s", prompt);

    for (size_t i = 0; i < arrayLength; i++)
    {
        char elementPrompt[7];
        sprintf(elementPrompt, "%zu: ", i + 1);
        getInput(&array[i], "%lf", elementPrompt);
    }
}

void getArray(double *array, size_t arrayLength, char inputType)
{
    if (inputType == 'u')
    {
        getArrayElementsFromUser(array, arrayLength, "Enter elements:\n");
    }
    else if (inputType == 'r')
    {
        getRandomArrayElements(array, arrayLength);
    }
    else
    {
        printf("Invalid input type in getArray()\n");
    }
}

void printArray(double *array, size_t arrayLength)
{
    printf("[");
    for (size_t i = 0; i < arrayLength; i++)
    {
        printf("%lf", array[i]);
        if (i != arrayLength - 1)
            printf(", ");
    }
    printf("]\n");
}

void UI()
{
    size_t arrayLength = 0;
    getArrayLength(&arrayLength, "Enter the length of the array: ");

    char inputType = 0;
    getOption(&inputType, inputTypes, "Enter input type (u - user input, r - random): ");

    char sortingType = 0;
    getOption(&sortingType, sortingTypes, "Enter sorting type (a - ascending, d - descending): ");

    double array[arrayLength];
    getArray(array, arrayLength, inputType);

    printf("\tBefore sorting: ");
    printArray(array, arrayLength);
    printf("\n");

    bubbleSort(array, arrayLength, sortingType == 'a' ? 1 : -1);

    printf("\tAfter sorting: ");
    printArray(array, arrayLength);
    printf("\n");
}

void endless(void (*function)())
{
    do
    {
        function();
        printf("Press ENTER to continue or any other key to exit\n");
    } while (getchar() == '\n');
}

int main()
{
    endless(UI);

    return 0;
}