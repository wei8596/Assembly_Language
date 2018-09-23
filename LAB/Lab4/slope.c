///******************************************************************************
//*
//* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
//*
//* Permission is hereby granted, free of charge, to any person obtaining a copy
//* of this software and associated documentation files (the "Software"), to deal
//* in the Software without restriction, including without limitation the rights
//* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//* copies of the Software, and to permit persons to whom the Software is
//* furnished to do so, subject to the following conditions:
//*
//* The above copyright notice and this permission notice shall be included in
//* all copies or substantial portions of the Software.
//*
//* Use of the Software is limited solely to applications:
//* (a) running on a Xilinx device, or
//* (b) that interact with a Xilinx device through a bus or interconnect.
//*
//* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
//* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//* SOFTWARE.
//*
//* Except as contained in this notice, the name of the Xilinx shall not be used
//* in advertising or otherwise to promote the sale, use or other dealings in
//* this Software without prior written authorization from Xilinx.
//*
//******************************************************************************/
//
///*
// * helloworld.c: simple test application
// *
// * This application configures UART 16550 to baud rate 9600.
// * PS7 UART (Zynq) is not initialized by this application, since
// * bootrom/bsp configures it to baud rate 115200
// *
// * ------------------------------------------------
// * | UART TYPE   BAUD RATE                        |
// * ------------------------------------------------
// *   uartns550   9600
// *   uartlite    Configurable only in HW design
// *   ps7_uart    115200 (configured by bootrom/bsp)
// */
//
#include <stdio.h>
#include "platform.h"


#define rst_n    0
#define wea      1
#define x        2
#define y        3
#define m        4
#define b        5
#define dir      6
#define range    7
#define index_rd 8
#define line_x   9
#define line_y   10
#define finish   11

int main()
{
	int i=0;
    init_platform();

    volatile int *base =  (int*) 0x43C00000;
    //
    base[rst_n] = 0;
    base[wea] = 1;
    base[x] = 10;
    base[y] = 10;
    base[m] = 2;
    base[b] = 2;
    base[dir] = 0;
    base[range] = 5;
    base[rst_n] = 1;

    while(base[finish] ==0);
    base[wea] = 0;
    for(i=0;i<base[range];i++)
    {
    	base[index_rd] = i;
    	printf("(x, y) = (%d, %d)\n",base[line_x], base[line_y]);
    }

    //
    base[rst_n] = 0;
    base[wea] = 1;
    base[x] = 10;
    base[y] = 10;
    base[m] = 2;
    base[b] = 2;
    base[dir] = 1;
    base[range] = 5;
    base[rst_n] = 1;

    while(base[finish] ==0);
    base[wea] = 0;
    for(i=0;i<base[range];i++)
    {
       	base[index_rd] = i;
       	printf("(x, y) = (%d, %d)\n",base[line_x], base[line_y]);
    }



    cleanup_platform();
    return 0;
}
