#include <stdio.h>
void test(){
    double a,b;
    a = 1.0/3.0;
    b = 2.0/3.0;
    printf("%.2lf %.2lf\n", a, b);
    printf("%.3lf %.3lf\n", a, b);
    printf("Result should be \n0.33 0.67\n0.333 0.667\n");
}
int main(){
    test();
    return 0;
}