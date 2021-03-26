#include <stdio.h>


double absolute(double n) {
    if (n >= 0) {
        return n;
    }

    return -n;
}


double my_sin(double x) {

    double item;
    double result = 0;
    int i=0;
    
    double factorial = 1;
    double power = x;
    int sign = 1;

    do {
        item = power / factorial;
        item = item * sign;

        result += item;
        i++;
        factorial *= (i*2) * (i*2+1);
        power *= x*x;
        sign *= -1;
    } while(absolute(item) >  0.000001);

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
