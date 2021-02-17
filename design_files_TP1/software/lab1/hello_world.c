/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "alt_types.h"
#include "math.h"
#include "sys/alt_timestamp.h"
#include"altera_avalon_timer_regs.h"

volatile int edge_capture;
int lut[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x2,0x78,0x0,0x10};


void exo1(){
	for(int i=0; i<=9999;i++){
		IOWR_ALTERA_AVALON_PIO_DATA (HEX5_BASE, lut[0]);
		IOWR_ALTERA_AVALON_PIO_DATA (HEX4_BASE, lut[0]);
		IOWR_ALTERA_AVALON_PIO_DATA (HEX0_BASE, lut[i%10]);
		IOWR_ALTERA_AVALON_PIO_DATA (HEX1_BASE, lut[(i/10)%10]);
		IOWR_ALTERA_AVALON_PIO_DATA (HEX2_BASE, lut[(i/100)%10]);
		IOWR_ALTERA_AVALON_PIO_DATA (HEX3_BASE, lut[(i/1000)%10]);
		usleep(5000);
		//int data = IORD_ALTERA_AVALON_PIO_DATA (0x8005080):

	}
}
void activate_led(int val){
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,val);
}
void display_hex(int i){
			IOWR_ALTERA_AVALON_PIO_DATA (HEX0_BASE, lut[i%10]);
			IOWR_ALTERA_AVALON_PIO_DATA (HEX1_BASE, lut[(i/10)%10]);
			IOWR_ALTERA_AVALON_PIO_DATA (HEX2_BASE, lut[(i/100)%10]);
			IOWR_ALTERA_AVALON_PIO_DATA (HEX3_BASE, lut[(i/1000)%10]);			IOWR_ALTERA_AVALON_PIO_DATA (HEX5_BASE, lut[0]);
			IOWR_ALTERA_AVALON_PIO_DATA (HEX4_BASE, lut[(i/10000)%10]);			IOWR_ALTERA_AVALON_PIO_DATA (HEX5_BASE, lut[0]);
			IOWR_ALTERA_AVALON_PIO_DATA (HEX5_BASE, lut[(i/100000)%10]);
}
static void handle_button_interrupts(void* context, alt_u32 id){
	volatile int* edge_capture_ptr = (volatile int*) context;
	*edge_capture_ptr =IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE, 0);
	IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE);
	display_hex( log2(*edge_capture_ptr));
}

static void init_button_pio(){
	void* edge_capture_ptr = (void*) &edge_capture;
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEY_BASE, 0xf);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE, 0x0);
	alt_irq_register( KEY_IRQ,edge_capture_ptr,handle_button_interrupts );

}

int main()
{

	init_button_pio();
	//exo1();
	while(1){
		printf("%d \n",edge_capture);
		alt_timestamp_freq();
		alt_timestamp_start();
		usleep(100000);
		printf("%d",gettimestamp());
		activate_led(IORD_ALTERA_AVALON_PIO_DATA (SWITCHES_BASE));
	}
  return 0;
}
