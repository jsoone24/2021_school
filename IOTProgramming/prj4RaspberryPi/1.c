#include <stdio.h>
#include <pigpio.h>

int main()
{
    // output pin
    const int d0 = 12;
    const int d1 = 18;
    const int d2 = 23;
    const int d3 = 25;
    // input pin
    const int i0 = 16; // down
    const int i1 = 24; // up
    // variable
    signed char num = 0;
    char temp = 0;

    // gpio setup
    if (gpioInitialise() < 0)
    {
        printf("init fail\n");
        return 0;
    }
    gpioSetMode(d0, PI_OUTPUT);
    gpioSetMode(d1, PI_OUTPUT);
    gpioSetMode(d2, PI_OUTPUT);
    gpioSetMode(d3, PI_OUTPUT);
    gpioSetMode(i0, PI_INPUT);
    gpioSetMode(i1, PI_INPUT);

    gpioWrite(d0, 0);
    gpioWrite(d1, 0);
    gpioWrite(d2, 0);
    gpioWrite(d3, 0);

    // loop start
    while (1)
    {
        // read button
        if (gpioRead(i0) == 0)
            --num;
        if (gpioRead(i1) == 0)
            ++num;

        temp = num < 0 ? 0 : num % 16;

        // write led status
        printf("num : %d, temp : %d, d0 : %d, d1 : %d, d2 : %d, d3 : %d\n",
               num, temp, temp & 1, (temp >> 1) & 1, (temp >> 2) & 1, (temp >> 3) & 1);
        gpioDelay(500000);

        gpioWrite(d0, temp & 1);
        gpioWrite(d1, (temp >> 1) & 1);
        gpioWrite(d2, (temp >> 2) & 1);
        gpioWrite(d3, (temp >> 3) & 1);
    }
    gpioTerminate();
    return 0;
}