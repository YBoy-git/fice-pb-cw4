#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_LENGTH 1000

#define RANDOM_LOWER_BOUND -100000
#define RANDOM_UPPER_BOUND 100000

void bubbleSort(double *array, size_t arrayLength)
{
    for (size_t i = 0; i < arrayLength - 1; i++)
    {
        for (size_t j = i + 1; j < arrayLength; j++)
        {
            if (array[i] > array[j])
            {
                double temp = array[i];
                array[i] = array[j];
                array[j] = temp;
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

char isInputTypeCorrect(char inputType)
{
    if (inputType != 'u' && inputType != 'r')
    {
        printf("Invalid option, try again\n");
        return 0;
    }
    return 1;
}

void getInputType(char *inputType, const char *prompt)
{
    do
    {
        getInput(inputType, "%c", prompt);
    } while (!isInputTypeCorrect(*inputType));
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
    getInputType(&inputType, "Enter input type (u - user input, r - random): ");

    double array[arrayLength];
    getArray(array, arrayLength, inputType);

    printf("Before sorting: ");
    printArray(array, arrayLength);

    bubbleSort(array, arrayLength);

    printf("After sorting: ");
    printArray(array, arrayLength);
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