/*

  raspi_gpio_hal.c

  This could needs to be compiled with -DU8X8_USE_PINS 
  
  Code partly taken over from 
    http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_GPIO_C.html
    https://github.com/olikraus/u8g2/wiki/Porting-to-new-MCU-platform#template-for-the-gpio-and-delay-callback


*/

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "u8x8.h"


void delaynanoseconds(unsigned long ns)
{
  struct timespec ts;

  ts.tv_sec = 0;
  ts.tv_nsec = ns;
  nanosleep(&ts, (struct timespec *)NULL);
}

int gpio_export(int pin)
{
  char buffer[6];  
  ssize_t bytes;
  int fd;

  fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd < 0)
    return perror("/sys/class/gpio/export"), 0;
  
  bytes = sprintf(buffer, "%d", pin);
  if (write(fd, buffer, bytes) < 0)
    return perror("write"), close(fd), 0;
  close(fd);
  return 1;
}

/* dir: 0=in, 1=out */
int gpio_direction(int pin, int dir)
{
  char path[256];
  int fd;
  int res;

  snprintf(path, 256, "/sys/class/gpio/gpio%d/direction", pin);
  fd = open(path, O_WRONLY);
  if (fd < 0)
    return perror(path), 0;
  
  switch (dir)
  {
    case 0 : res = write(fd,"in",2); break;
    case 1: res = write(fd,"out",3); break;
    default: return close(fd), 0;
  }
  
  if (res < 0)
    return perror("write"), close(fd), 0;
  close(fd);
  return 1;
}

int gpio_fd[U8X8_PIN_CNT];

int gpio_prepare_write(int msg, int pin)
{
  char path[256];
  snprintf(path, 256, "/sys/class/gpio/gpio%d/value", pin);
  gpio_fd[msg] = open(path, O_WRONLY);
  if (gpio_fd[msg]  < 0)
    return perror(path), 0;
  printf("prepare write success '%s' fd %d msg %d\n", path, gpio_fd[msg], msg);
  return 1;
}

int gpio_quick_write(int msg, int value)
{
  int res;
  //printf("quick write fd %d msg %d\n", gpio_fd[msg], msg);
  if ( msg > U8X8_PIN_OUTPUT_CNT )
    exit(0);
  
  switch (value)
  {
    case 0 : res = write(gpio_fd[msg] ,"0",1); break;
    default: res = write(gpio_fd[msg] ,"1",1); break;
  }
  
  
  
  if (res < 0)
    return perror("quick_write"), 0;
  return 1;
}

int gpio_write(int pin, int value)
{
  char path[256];
  int fd;
  int res;

  snprintf(path, 256, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(path, O_WRONLY);
  if (fd < 0)
    return perror(path), 0;
  switch (value)
  {
    case 0 : res = write(fd,"0",1); break;
    default: res = write(fd,"1",1); break;
  }
  if (res < 0)
    return perror("write"), close(fd), 0;
  close(fd);
  return 1;
}

uint8_t u8x8_gpio_and_delay_raspi_gpio_hal(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  int i;
  //printf("msg %d\n", msg);
  switch(msg)
  {
    case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
        //printf("i2c clock msg %d %d\n", msg, U8X8_MSG_GPIO_I2C_CLOCK);
    
    case U8X8_MSG_GPIO_I2C_DATA:			// arg_int=0: Output low at I2C data pin
    case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
    case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
    case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
    case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
    case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
    case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
    case U8X8_MSG_GPIO_CS1:				// CS1 (chip select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_CS2:				// CS2 (chip select) pin: Output level in arg_int
      if ( u8x8_GetPinValue(u8x8, msg) != U8X8_PIN_NONE )
      {
        //gpio_write(u8x8_GetPinValue(u8x8, msg), arg_int);
        //printf("gpio msg %d\n", msg);
        gpio_quick_write(u8x8_GetPinIndex(u8x8, msg), arg_int);
      }
      break;
    
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8      
      for( i = 0; i < U8X8_PIN_CNT; i++ )
	if ( u8x8->pins[i] != U8X8_PIN_NONE )
	{
	  if ( i < U8X8_PIN_OUTPUT_CNT )
	  {
            gpio_export(u8x8->pins[i]);
            gpio_direction(u8x8->pins[i], 1);
            gpio_prepare_write(i, u8x8->pins[i]);
	  }
	  else
	  {
            // not supported at the moment
            //gpio_direction(u8x8->pins[i], 0);
	  }
	}
      delaynanoseconds(100000);
      break;							// can be used to setup pins
    case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
      delaynanoseconds(arg_int);
      break;    
    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
      delaynanoseconds((unsigned long)arg_int*100UL);
      break;
    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
      delaynanoseconds((unsigned long)arg_int*10000UL);
      break;
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
      delaynanoseconds((unsigned long)arg_int*1000000UL);
      break;
    case U8X8_MSG_DELAY_I2C:				// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
      //delaynanoseconds(1250UL*arg_int);
      delaynanoseconds(1000UL*arg_int);
      break;							// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us        
    
    case U8X8_MSG_GPIO_MENU_SELECT:
      u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_NEXT:
      u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_PREV:
      u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
      break;
    case U8X8_MSG_GPIO_MENU_HOME:
      u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
      break;
    default:
      //u8x8_SetGPIOResult(u8x8, 1);			// default return value
      break;
  }
  return 1;
}

