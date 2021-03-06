#include <stdio.h>
#include <math.h>
#define DONT_RIGHT -1
#define RIGHT 0
#define NEGATIVE -1
#define POSITIVE 1
#define BEFORE_SIGN 1
#define BEFORE_DIGIT 2
#define DIGIT_BEFORE_POINT 3
#define DIGIT_AFTER_POINT 4
#define INFINITE_ROOTS -1

struct Roots {
        int count;
        double roots[2];
};
/* reads the coefficients a, b, c from the keyboard */
void getCoefficients(double* a, double* b, double* c);
/* solves the equation ax^2 + bx + c = 0 and returns solutions to struct Roots */
struct Roots solveEquation(double a, double b, double c);
/* reads one coefficient and checks it if it's right */
int isrightDouble(double* koef);
/* prints solutions or says that solutions are infinite number */
void putSolution(struct Roots Solution);

int main(void)
{
    double a = 0, b = 0, c = 0;
    getCoefficients(&a, &b, &c);
    struct Roots Solution = solveEquation(a, b, c);
    putSolution(Solution);
    return 0;
}
int isrightDouble(double* koef)
{
    int ch = 0;
    if (scanf("%lf", koef) == 0) {
        while ( (ch = getchar()) != '\n' );
        return DONT_RIGHT;
    }
    return RIGHT;
}

void getCoefficients(double* a, double* b, double* c)
{
    int i = 0;
    double arr_coef[3];
    for (i = 0; i < 3; i++) {
        printf("enter coefficient ");
        switch (i) {
            case 0:
                printf("a, please\n");
                break;
            case 1:
                printf("b, please\n");
                break;
            default:
                printf("c, please\n");
                break;
        }
        while ( isrightDouble(&arr_coef[i]) == DONT_RIGHT ) {
            printf("use standard syntax\n");
        }
    }
    *a = arr_coef[0];
    *b = arr_coef[1];
    *c = arr_coef[2];
}

struct Roots solveEquation(double a, double b, double c)
{
    struct Roots Solution = {
        .count = 0,
        .roots[0] = 0,
        .roots[1] = 0,
    };
    double discriminant = 0;;
    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                Solution.count = INFINITE_ROOTS;
            } else {
                Solution.count = 0;
            }
        } else {
            if (c == 0) {
                Solution.count = 1;
                Solution.roots[0] = 0;
            } else {
                Solution.count = 1;
                Solution.roots[0] = -c / b;
            }
        }
    } else {
        if (c == 0) {
            if (b == 0) {
                Solution.count = 1;
                Solution.roots[0] = 0;
            } else {
                Solution.count = 2;
                Solution.roots[0] = 0;
                Solution.roots[1] = -b / a;
            }
        } else {
            discriminant = b*b - 4*a*c;
            if (discriminant == 0) {
                Solution.count = 1;
                Solution.roots[0] = -b / (2*a);
            } else if (discriminant > 0) {
                discriminant = sqrt(discriminant);
                Solution.count = 2;
                Solution.roots[0] = ( -b - discriminant ) / (2*a);
                Solution.roots[1] = ( -b + discriminant ) / (2*a);
            } else {
                Solution.count = 0;
            }
        }
    }
    return Solution;
}

void putSolution(struct Roots Solution)
{
    printf("Answer:\n");
    switch (Solution.count) {
        case INFINITE_ROOTS:
            printf("Solutions are infinite number\n");
        break;
        case 0:
            printf("No solutions\n");
            break;
        case 1:
            printf("x = %.5lf\n", Solution.roots[0]);
            break;
        default:
            printf("x1 = %.5lf\nx2 = %0.5lf\n", Solution.roots[0], Solution.roots[1]);
            break;
    }
}
