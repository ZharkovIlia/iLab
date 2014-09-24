#include <stdio.h>
#include <math.h>
int main(void)
{
    double a, b, c, x1, x2, discriminant;
    printf("Enter a, b, c through space, please\n");
    scanf("%lf %lf %lf", &a, &b, &c);
    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                printf("infinite number of solution");
            } else {
                printf("no solutions");
            }
        } else {
            x1 = -c/b;
            printf("x = %.5lf", x1);
        }
    } else {
        discriminant = b*b - 4*a*c;
        if (discriminant == 0) {
            x1 = -b / (2*a);
            printf("x = %.5lf", x1);
        } else if (discriminant > 0) {
            x1 = (-b + sqrt (discriminant) ) / (2*a);
            x2 = (-b - sqrt (discriminant) ) / (2*a);
            printf("x1 = %.5lf\nx2 = %.5lf", x1, x2);
        } else {
            printf("no solutions");
        }
    }
    return 0;
}

