#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_PRECISION 1e-15
#define M_PI 3.14159265358979323846

typedef double degrees_t;
typedef double radians_t;

long long getFactorial(int n)
{
    long long factorial = 1;
    for (int i = 2; i <= n; i++)
    {
        factorial *= i;
    }
    return factorial;
}

void clearIB()
{
    while (getchar() != '\n')
        ;
}

void getInput(void *input, char *format, const char *message)
{
    char temp = 0;

    do
    {
        printf("%s", message);
        int status = scanf(format, input) + scanf("%c", &temp);
        if (status != 2 || temp != '\n')
        {
            printf("Invalid input, the format is %s, try again\n", format);
            clearIB();
        }
    } while (temp != '\n');
}

double radiansToDegrees(radians_t angle)
{
    return angle * 180.0 / M_PI;
}

double degreesToRadians(degrees_t angle)
{
    return angle * M_PI / 180.0;
}

char isChoiceCorrect(char choice)
{
    if (choice != 's' && choice != 'c')
    {
        printf("Invalid option, try again\n");
        return 0;
    }
    return 1;
}

void getChoice(char *choice, const char *message)
{
    char temp = 0;
    do
    {
        getInput(choice, "%c", message);
    } while (!isChoiceCorrect(*choice));
}

void getAngle(degrees_t *angle, const char *message)
{
    getInput(angle, "%lf", message);
    *angle = degreesToRadians(*angle);
}

void getStep(degrees_t *step, radians_t angle1, radians_t angle2, const char *message)
{
    if (angle1 != angle2)
    {
        do
        {
            getInput(step, "%lf", "Enter step: ");
            if ((angle1 < angle2 && *step <= 0) || (angle1 > angle2 && *step >= 0))
            {
                printf("Invalid step, try again\n");
            }
        } while ((angle1 < angle2 && *step <= 0) || (angle1 > angle2 && *step >= 0));
        *step = degreesToRadians(*step);
    }
}

double validatePrecision(double precision)
{
    if (precision >= 1.0)
    {
        int count = (int)precision;
        precision = 1.0;
        while (count > 0)
        {
            precision /= 10.0;
            count--;
        }
    }
    return precision;
}

char isPrecisionValid(double precision)
{
    if (precision <= 0)
    {
        printf("Precision must be positive, try again\n");
        return 0;
    }

    if (precision < MAX_PRECISION)
    {
        printf("Max precision is %e, try again\n", MAX_PRECISION);
        return 0;
    }

    return 1;
}

void getPrecision(double *precision, const char *message)
{
    do
    {
        getInput(precision, "%lf", message);
        *precision = validatePrecision(*precision);
    } while (!isPrecisionValid(*precision));
}

// return angle in range [0, 2 * M_PI)
double easeDegrees(degrees_t angle)
{
    while (angle >= 360)
    {
        angle -= 360;
    }
    while (angle < 0)
    {
        angle += 360;
    }

    return angle;
}
double easeRadians(radians_t angle)
{
    while (angle >= 2 * M_PI)
    {
        angle -= 2 * M_PI;
    }
    while (angle < 0)
    {
        angle += 2 * M_PI;
    }

    return angle;
}

double calculateCosWithPrecision(radians_t angle, double precision);
double calculateSinWithPrecision(radians_t angle, double precision)
{
    angle = easeRadians(angle);

    // shrink angle to [0, M_PI / 2)
    if (angle >= M_PI)
    {
        return -calculateSinWithPrecision(angle - M_PI, precision);
    }

    // shrink angle to [0, M_PI / 2)
    if (angle >= M_PI / 2)
    {
        return calculateCosWithPrecision(angle - M_PI / 2, precision);
    }

    // shrink angle to [0, M_PI / 4]
    if (angle > M_PI / 4)
    {
        return calculateCosWithPrecision(M_PI / 2 - angle, precision);
    }

    double sine = 0;
    double term = angle;
    int i = 1;
    while (fabs(term) >= precision && fabs(term) != INFINITY)
    {
        sine += term;
        term = pow(-1, i) * pow(angle, 2 * i + 1) / getFactorial(2 * i + 1);
        i++;
    }
    return sine;
}

double calculateCosWithPrecision(radians_t angle, double precision)
{
    angle = easeRadians(angle);

    // shrink angle to [0, M_PI / 2)
    if (angle >= M_PI)
    {
        return -calculateCosWithPrecision(angle - M_PI, precision);
    }

    // shrink angle to [0, M_PI / 2)
    if (angle >= M_PI / 2)
    {
        return -calculateSinWithPrecision(angle - M_PI / 2, precision);
    }

    // shrink angle to [0, M_PI / 4]
    if (angle > M_PI / 4)
    {
        return calculateSinWithPrecision(M_PI / 2 - angle, precision);
    }

    double cosine = 0;
    double term = 1;
    int i = 1;
    while (fabs(term) >= precision && fabs(term) != INFINITY)
    {
        cosine += term;
        term = pow(-1, i) * pow(angle, 2 * i) / getFactorial(2 * i);
        i++;
    }
    return cosine;
}

char *getCalculationFormat(double precision)
{
    char *calculationFormat = malloc(10 * sizeof(char));
    sprintf(calculationFormat, "%%.%dlf\t\t", (int)(-log10(precision)));
    return calculationFormat;
}

void sinWrapper(radians_t begin, radians_t end, radians_t step, double precision)
{
    char *degreesFormat = "%lf\t\t";
    char *calculationFormat = getCalculationFormat(precision);

    printf("x | sin(x) | taylorSin(x) | diff\n");
    do
    {
        const double sine = sin(begin);
        const double taylorSine = calculateSinWithPrecision(begin, precision);

        printf(degreesFormat, radiansToDegrees(begin));
        printf(calculationFormat, sine);
        printf(calculationFormat, taylorSine);
        printf(calculationFormat, sine - taylorSine);

        printf("\n");
        begin += step;
    } while ((step > 0 && begin <= end) || (step < 0 && begin >= end));

    free(calculationFormat);
}

void cosWrapper(radians_t begin, radians_t end, radians_t step, double precision)
{
    char *degreesFormat = "%lf\t\t";
    char *calculationFormat = getCalculationFormat(precision);

    printf("x | cos(x) | taylorCos(x) | diff\n");
    do
    {
        const double cosine = cos(begin);
        const double taylorCosine = calculateCosWithPrecision(begin, precision);

        printf(degreesFormat, radiansToDegrees(begin));
        printf(calculationFormat, cosine);
        printf(calculationFormat, taylorCosine);
        printf(calculationFormat, cosine - taylorCosine);

        printf("\n");
        begin += step;
    } while ((step > 0 && begin <= end) || (step < 0 && begin >= end));

    free(calculationFormat);
}

void UI()
{
    char choice = 0;
    getChoice(&choice, "Would you like to calculate sin or cos? (s and c respectively)\n");

    degrees_t angle1 = 0.0;
    degrees_t angle2 = 0.0;
    getAngle(&angle1, "Enter beginning angle (in degrees): ");
    getAngle(&angle2, "Enter ending angle (in degrees): ");

    degrees_t step = 0.0;
    getStep(&step, angle1, angle2, "Enter step (in degrees): ");

    double precision = 0.0;
    char precisionPrompt[50];
    sprintf(precisionPrompt, "Enter precision (min %e:) ", MAX_PRECISION);
    getPrecision(&precision, precisionPrompt);

    printf("\n");
    if (choice == 's')
    {
        sinWrapper(angle1, angle2, step, precision);
    }
    else if (choice == 'c')
    {
        cosWrapper(angle1, angle2, step, precision);
    }
    else
    {
        printf("Unknown option\n");
    }
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