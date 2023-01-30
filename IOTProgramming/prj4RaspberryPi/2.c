#include <stdio.h>
#include <pigpio.h>

int main()
{
    //input pin
    const int i0 = 16; // down
    const int i1 = 24; // up
    //variable
    int stage = 1;
    // PWM setting
    const int freq = 10000;
    // output pin
    const int led = 18;
    // gpio setup
    if (gpioInitialise() < 0)
    {
        printf("init fail\n");
        return 0;
    }
    gpioSetMode(led, PI_OUTPUT);
    gpioSetMode(i0, PI_INPUT);
    gpioSetMode(i1, PI_INPUT);

    // loop start
    while (1)
    {
        // button read
        if (gpioRead(i0) == 0)
            stage--;
        if (gpioRead(i1) == 0)
            stage++;
        if (stage < 1)
            stage = 1;
        if (stage > 10)
            stage = 10;
        printf("stage : %d, duty : %d\n", stage, stage * 100000);
        gpioDelay(500000);

        // write led
        gpioHardwarePWM(led, freq, stage * 100000);
    }
    gpioTerminate();

    return 0;
}