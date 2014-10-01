#include <stdio.h>
#include <assert.h>
const int SQRT_MAXINT = 46340; // for problem B which is from variant 1
#define NO 0 // for problem C which is from variant 1
const int SIZE = 48; // for problem D which is from variant 1
const int SIZE_1 = 50; // for problem A which is from variant 2

void PrimeNumber(); // variant 1 problem A

void NumberWithoutSquare(); // variant 1 problem B

void Weighing(); // variant 1 problem C

void MatrixOfGoodWord(); // variant 1 problem D

void Transposition(); // variant 2 problem A

int main(void)
{
    int i = 0;
    printf("Enter number of problem, which you want to decide: 1) PrimeNumber, 2) NumberWithoutSquare, 3) Weighing, 4) MatrixOfGoodWord or 5) Transposition\n");
    scanf("%d", &i);
    switch (i) {
        case 1:
            PrimeNumber();
            break;
        case 2:
            NumberWithoutSquare();
            break;
        case 3:
            Weighing();
            break;
        case 4:
            MatrixOfGoodWord();
            break;
        case 5:
            Transposition();
            break;
        default:
            printf("wrong number");
            break;
    }
    return 0;
}
void Transposition()
{
    long int arr[SIZE_1][SIZE_1], i = 0, j = 0, n = 0;
    scanf("%ld", &n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%ld", &arr[i][j]);
        }
    }
    for (j = 0; j < n; j++) {
        for (i = 0; i < n; i++) {
            printf("%ld ", arr[i][j]);
        }
        printf("\n");
    }
}

void MatrixOfGoodWord()
{
    long int matrix[SIZE][SIZE], i = 0, j = 0, n = 0;
    scanf("%ld", &n);
    for (i = 0; i < SIZE; i++) {
        assert(0 <= i && i < SIZE);
        for (j = 0; j < SIZE; j++) {
            assert(0 <= j && j < SIZE);
            matrix[i][j] = 0;
        }
    }
    for (i = 0; i < SIZE; i++) {
        assert(0 <= i && i < SIZE);
        matrix[i][0] = 1;
    }
    matrix[1][1] = 1;
    for (i = 2; i < SIZE; i++) {
        assert(0 <= i && i < SIZE);
        for (j = 1; j < SIZE; j++) {
            assert(0 <= j && j < SIZE);
            matrix[i][j] = matrix[i-1][j] + matrix[i-2][j-1];
        }
    }
    for (i = 0; i <= n; i++) {
        for (j = 0; matrix[i][j] != 0; j++) {
            printf("%ld ", matrix[i][j]);
        }
        printf("\n");
    }
}

void Weighing()
{
    int n = 0;
    scanf("%d", &n);
    while (n > 4) {
        switch (n % 4) {
            case 2:
                n = NO;
                printf("NO");
                break;
            case 3:
                n = n/4 + 1;
                break;
            default:
                n = n / 4;
                break;
        }
    }
    switch (n) {
        assert(0 <= n && n <= 4);
        case 4: case 1:
            printf("YES");
            break;
        case NO: break; // it means that solution has already printed
        default:
            printf("NO");
            break;
    }
}

void NumberWithoutSquare()
{
    int arr[SQRT_MAXINT], i = 0, n = 0, boolw = 0;
    for (i = 0; i <= SQRT_MAXINT; i++) {
        assert(0 <= i && i <= SQRT_MAXINT);
        arr[i] = i*i;
    }
    scanf("%d", &n);
    for (i = 2; (i <= SQRT_MAXINT && boolw == 0); i++) {
        assert(2 <= i && i <= SQRT_MAXINT);
        boolw = (boolw || ( (n % arr[i]) == 0 ));
    }
    if (boolw) {
        printf("YES");
    } else {
        printf("NO");
    }
}

void PrimeNumber()
{
    int arr[1001], i = 0, j = 0, n = 0, a = 0, numdel = 0;
    for (i = 0; i < 1001; i++) {
        assert(0 <= i && i <=1000);
        arr[i] = i;
    }
    arr[1] = 0; // number 1 is not prime!
    for (i = 2; i < 1001; i++) { // if number is prime array's member is equal to zero
        assert(0 <= i && i <=1000);
        if (arr[i] != 0) {
            for (j = i * 2; j < 1001; j += i) {
                assert(0 <= j && j <= 1000);
                arr[j] = 0;
            }
        }
    }
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &a);
        if (a <= 1000) {
            if (arr[a] != 0) {
                printf("%d ", a);
            }
        } else {
            numdel = 0;
            for (j = 2; j < 1001; j++) {
                assert(0 <= j && j <= 1000);
                if (arr[j] != 0) {
                    numdel = ( (a % arr[j]) == 0 ) ? numdel + 1 : numdel;
                }
            }
            if (numdel == 0) {
                printf("%d ", a);
            }
        }
    }


}
