#include <stdio.h>
#include <stdarg.h>

int add_sum(int num,...)
{
    va_list valist;
    int  sum = 0;

    va_start(valist, num);
    for (int i=0; i<num; i++) {
        sum += va_arg(valist, int);
    }
    va_end(valist);

    return sum;
    
}

int main(int argc, char *argv[])
{
    int sum = 0;
    sum = add_sum(4,4,2,3,4);
    printf("sum = %d\n",sum);

    return 0;
}


