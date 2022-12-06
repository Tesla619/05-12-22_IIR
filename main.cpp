#include "mbed.h"

//DOESN'T WORK EITHER DUE TO BAREMETAL OR MBEDIGNORE

AnalogIn Ain(PC_5);
AnalogOut Aout(PA_5);
DigitalIn LPFswitch(PC_13);

float data_in, data_out;

Ticker s20_khz_tick;

float LPF(float LPF_in)
{
    float a[4] = {1.0, 2.6235518066, -2.3146825811, 0.6855359773};
    float b[4] = {0.0006993496,0.0020980489,0.0020980489,0.0006993496};
    static float LPF_out;
    static float x[4],y[4];

    x[3]=x[2];x[2]=x[1];x[1]=x[0];
    y[3]=y[2];y[2]=y[1];y[1]=y[0];

    x[0] = LPF_in;

    y[0] = (b[0]*x[0] + b[1]*x[1] + b[2]*x[2] + b[3]*x[3] + a[1]*x[1] + a[2]*x[2] + a[3]*x[3]);

    LPF_out = y[0];
    return LPF_out;

}

void s20khz_task(void)
{
    data_in = Ain - 0.5;

    if(LPFswitch == 1)
    {
        data_out = LPF(data_in);
    }else
    {
        data_out = data_in;
    }
    Aout = data_out + 0.5;
}

// main() runs in its own thread in the OS
int main()
{
    s20_khz_tick.attach_us(&s20khz_task,50);
    while (1);

}

/*
++ MbedOS Error Info ++
Error Status: 0x80010133 Code: 307 Module: 1
Error Message: Mutex: 0x200005E0, Not allowed in ISR context
Location: 0x800AC45
Error Value: 0x200005E0
Current Thread: main Id: 0x20001C70 Entry: 0x80085AD StackSize: 0x1000 StackMem: 0x20000698 SP: 0x2009FEA4 
For more info, visit: https://mbed.com/s/error?error=0x80010133&tgt=B_L4S5I_IOT01A
-- MbedOS Error Info --
*/