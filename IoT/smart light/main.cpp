/*
   This VL6180X Expansion board test application performs a range measurement
   and als measurement in polling mode on the onboard embedded top sensor. 
   The result of both the measures are printed on the serial over.  
   get_distance() and get_lux() are synchronous! They block the caller until the
   result will be ready.
*/


/* Includes ------------------------------------------------------------------*/

#include "mbed.h"
#include "XNucleo6180XA1.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


/* Definitions ---------------------------------------------------------------*/

#define VL6180X_I2C_SDA   D14 
#define VL6180X_I2C_SCL   D15 

#define RANGE   0
#define ALS     1

#define DELAY 2000  // 2Sec


/* Types ---------------------------------------------------------------------*/

/* Operating mode */
operating_mode_t operating_mode, prev_operating_mode;
enum op_mode_int_poll_t {
    PollMeasure,
    IntMeasure
};

/* Variables -----------------------------------------------------------------*/

static XNucleo6180XA1 *board = NULL;
DigitalOut led(LED1);
DigitalIn button(USER_BUTTON);
int status;
uint32_t lux, dist;
DevI2C *device_i2c = new DevI2C(VL6180X_I2C_SDA, VL6180X_I2C_SCL);

/* Measure data */
measure_data_t data_sensor_top;

/* Flags that handle interrupt request */
bool int_sensor_top = false, int_stop_measure = false;  
bool On = false;

Timer t; // to count the period where lux is less than 20
Timer t2;
Timer t3;

Ticker ticker;

/* Functions -----------------------------------------------------------------*/
void blinky(void){	
	led = !led;
	if(t2.read() >30){
		led = 0;
		t2.stop();
		t2.reset();
	}
}

//
void blinky2(void){
	led = 1;
	wait(1);
	led = 0;
}

void alwaysOn(void){
	led = 1; 
}

void button_ISR(){
	On = !On;
}

void longMode(void){
	if(!button){
		blinky2();
	}else{
		alwaysOn();
	}
}

/*=================================== Main ==================================
  Prints on the serial over USB the measured distance and lux.
  The measures are run in single shot polling mode.
=============================================================================*/
int main()
{ 
    /* Creates the 6180XA1 expansion board singleton obj. */
    board = XNucleo6180XA1::instance(device_i2c, A3, A2, D2, D2);
	/* Initializes the 6180XA1 expansion board with default values. */
	status = board->init_board();
	
	 /* Start continous measures Interrupt based */
    //int_continous_als_or_range_measure (device_i2c);
	if (status) {
		printf("Failed to init board!\n\r");
		return 0;
	}
	button.mode(PullUp);
	while (true) {
		
		board->sensor_top->get_distance(&dist);
		board->sensor_top->get_lux(&lux);
		if(lux <20){
			t.start();
		}else if(t3.read()>10){
			led = 0;
			t.stop();
			t.reset();
		}else if(t2.read()>10){
			t.stop();
			t.reset();
		}else{
			t.stop();
		}
		
		if(lux <150 && lux >=20){
			t2.start();
		}else if(t.read()>10){
			t2.stop();
			t2.reset();
		}else if(t3.read()>10){
			led = 0;
			t.stop();
			t.reset();
		}else{
			t2.stop();
		}
		
		if(lux >=150){
			t3.start();
		}else{
			t3.stop();
			t3.reset();
		}
		
		printf("The time for lux <20 taken was %f seconds\n", t.read());
		printf("The time for lux <150 taken was %f seconds\n", t2.read());
		printf("The time for lux >=150 taken was %f seconds\n", t3.read());
		//led keep on mode, if it is dark over a period, led is always on until lux > 50
		if(lux <20 && t.read()>= 5){
			longMode();
		}
		//20<lux<50, led is on blinky mode for 10 sec
		if (lux <150 && t2.read() >= 10){
			blinky();
		}
		printf ("Distance: %d, Lux: %d\n\r", dist, lux);
		wait(1);
	}
}
