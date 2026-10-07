#include "software_timer.h"

volatile int counterArr[4] = {0,0,0,0};
int cycles = 10;
volatile int flags[4] = {0,0,0,0};

volatile int timer0_counter = 0;
volatile int timer0_flag = 0;

void setTimer(int duration){
    timer0_counter = duration / cycles;
	timer0_flag = 0;
}

void setTimer0(int duration)
{
    /* Keep both existing interfaces; timer0_flag follows the lab skeleton. */
    setTimer(duration);
    counterArr[0] = duration / cycles;
	flags[0] = 0;
}

void setTimer1(int duration)
{
    counterArr[1] = duration / cycles;
	flags[1] = 0;
}

void setTimer3(int duration)
{
	counterArr[3] = duration / cycles;
	flags[3] = 0;
}

void setTimer2(int duration)
{
    counterArr[2] = duration / cycles;
    flags[2] = 0;
}

void timerRun(){
	if (timer0_counter > 0 && --timer0_counter == 0)
	{
		timer0_flag = 1;
	}

	for (int i = 0; i < 4; i++)
	{
		if (counterArr[i] > 0 && --counterArr[i] == 0)
		{
			flags[i] = 1;
		}
	}
}

/* Lab spelling, retaining timerRun() for existing exercise code. */
void timer_run(void)
{
    timerRun();
}
