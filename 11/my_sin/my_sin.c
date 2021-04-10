#include <stdio.h>
#include <math.h>

double absolute(double n) {
    if (n >= 0) {
        return n;
    }
    return -n;
}

#define SIN_PRECISION 0.000001

double my_sin(double x) {
    double item; /* stores the value of a single iteration
                    when is smaller than precision we stop and return 
                    the final value */
    double result = 0; 
    int i=0;
    
    double factorial = 1;
    double power = x;
    int sign = 1;

    do {
        item = sign * (power / factorial);
        result += item;

        i++;
        factorial *= (i*2) * (i*2+1);
        power *= x*x;
        sign *= -1;
    } while(absolute(item) >= SIN_PRECISION);

    return result;
}

int main() {
    double number;

    printf("Hello, please enter a radian between -25 to 25\n");
    scanf("%lf", &number);
    printf("calculating sin(%f)\n", number);

    printf("my_sin(%f) = %f\n",  number, my_sin(number));
    printf("sin(%f) = %f\n", number, sin(number));
    return 0;
}
