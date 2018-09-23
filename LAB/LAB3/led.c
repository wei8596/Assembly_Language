#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "xil_cache.h"
#define GPIO_BITWIDTH 8
#define GPIO_DEVICE_ID XPAR_AXI_GPIO_0_DEVICE_ID
#define LED_DELAY 5000000
#define LED_MAX_BLINK 0x1
#define LED_CHANNEL 1
#define printf xil_printf

XGpio Gpio;
XGpio GpioOutput;

int GpioMarquee(u16 DeviceId, u32 GpioWidth)
{
    volatile int Delay;
    u32 LedBit;
    u32 LedLoop;
    int Status;

    Status = XGpio_Initialize(&GpioOutput, DeviceId);
    if(Status != XST_SUCCESS)
    {
    	return XST_FAILURE;
    }

    XGpio_SetDataDirection(&GpioOutput, LED_CHANNEL, 0x0);

    XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, 0x0);

    int i;
    for(i = 1;i <= 128;i*=2)
    {
    	XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, i);
    	for(Delay = 0; Delay<LED_DELAY; Delay++);
    	//
    	XGpio_DiscreteClear( &GpioOutput, LED_CHANNEL, i);
    	for(Delay = 0; Delay<LED_DELAY; Delay++);
    }

    for(i = 128;i >= 1;i/=2)
    {
    	XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, i);
        for(Delay = 0; Delay<LED_DELAY; Delay++);
        //
        XGpio_DiscreteClear( &GpioOutput, LED_CHANNEL, i);
        for(Delay = 0; Delay<LED_DELAY; Delay++);
     }

    for(i = 3;i <= 255;i*=4)
    {
        XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, i);
        for(Delay = 0; Delay<LED_DELAY; Delay++);
        //
        XGpio_DiscreteClear( &GpioOutput, LED_CHANNEL, i);
        for(Delay = 0; Delay<LED_DELAY; Delay++);
    }

    for(i = 192;i >= 3;i/=4)
    {
         XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, i);
         for(Delay = 0; Delay<LED_DELAY; Delay++);
         //
         XGpio_DiscreteClear( &GpioOutput, LED_CHANNEL, i);
         for(Delay = 0; Delay<LED_DELAY; Delay++);
    }

    for(i = 1;i <= 10;i++)
    {
        XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, 170);
        for(Delay = 0; Delay<LED_DELAY; Delay++);
        //
        XGpio_DiscreteClear( &GpioOutput, LED_CHANNEL, 170);
       	for(Delay = 0; Delay<LED_DELAY; Delay++);

       	XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, 85);
       	for(Delay = 0; Delay<LED_DELAY; Delay++);
       	//
       	XGpio_DiscreteClear( &GpioOutput, LED_CHANNEL, 85);
       	for(Delay = 0; Delay<LED_DELAY; Delay++);
    }

    XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, 255);
    for(Delay = 0; Delay<100000000; Delay++);
    //
    XGpio_DiscreteClear( &GpioOutput, LED_CHANNEL, 255);
    for(Delay = 0; Delay<100000000; Delay++);

    return XST_SUCCESS;
}

int main(void)
{
	int i = 0;
	for( ; i < 100; i++)
	{
		u32 status;
		status = GpioMarquee(GPIO_DEVICE_ID, GPIO_BITWIDTH);
		if(status ==0)
			printf("SUCCESS!.rn");
		else
			printf("FAILED.rn");
	}

	return XST_SUCCESS;

}
