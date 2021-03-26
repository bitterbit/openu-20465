#include <stdio.h>


double abs(double n) {
    if (n >= 0) {
        return n;
    }

    return -n;
}

double pow(double base, unsigned power) {
    if (power == 0) {
        return 1;
    }

    /* 2^3 = 2*2*2  */
    double number = 1;

    for(unsigned i=0; i<power; i++) {
        number *= base;
    }

    /* printf("pow(%lf, %u) = %lf\n", base, power, number); */
    return number;
}

double factorial(unsigned n) {
    double fact = 1;
    for (unsigned i=1; i<=n; i++) {
        fact *= i;
    }

    return fact;
}

double my_sin(double x) {

    double item;
    double result = 0;
    int i=0;
    
    double last_factorial = 1;

    do {
        int sign = pow(-1, i);
        item = pow(x, 2*i+1) / last_factorial;
        item = item * sign;

        printf("%lf %lf\n", factorial(2*i+1), last_factorial);
        /* printf("(%lf / %lf) * %d\n", pow(x, 2*i+1), factorial(2*i+1), sign); */

        result += item;
        i++;

        printf("%d %d\n", i*2, i*2+1);
        last_factorial *= (i*2) * (i*2+1);
    } while(abs(item) >  0.000001);

    return result;
}

int main() {
    double number;
    double result;

    printf("What number?\n");
    scanf("%lf", &number);
    printf("calculating sin(%f)\n", number);

    result = my_sin(number);

    printf("number %f\n", result);
    return 0;
}
