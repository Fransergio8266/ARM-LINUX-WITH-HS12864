#include "u8g2port.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
// GPIO chip number for character device
#define GPIO_CHIP_NUM 0
// SPI bus uses upper 4 bits and lower 4 bits, so 0x10 will be /dev/spidev1.0
#define SPI_BUS 0x01
#define OLED_SPI_PIN_RES            U8X8_PIN_NONE
#define OLED_SPI_PIN_DC             U8X8_PIN_NONE

// CS pin is controlled by linux spi driver, thus not defined here, but need to be wired
#define OLED_SPI_PIN_CS             U8X8_PIN_NONE

#define DEBUG_TRACE(x) { \
        struct timeval  tv; \
        gettimeofday(&tv, NULL); \
        double t0 = ((tv.tv_sec)*1000000 + tv.tv_usec); \
        x \
        gettimeofday(&tv, NULL); \
        double t1 = ((tv.tv_sec)*1000000 + tv.tv_usec); \
        printf(" Levou %lf us\n",t1-t0); \
}

char* i_a = "I-a=235.54A";
char* i_b = "I-b=58.36A";
char* i_c = "I-c=149.54A";
char* v_a = "V-a=127.45V";

void drawBannerText(u8g2_t *u8g2) {

    u8g2_ClearBuffer(u8g2);
    u8g2_SetFontMode(u8g2, 0);

    u8g2_SetFont(u8g2,u8g2_font_t0_17_mf);
    u8g2_DrawStr(u8g2, 1, 14, i_a);
    u8g2_DrawStr(u8g2, 1, 30, i_b);
    u8g2_DrawStr(u8g2, 1, 46, i_c);
    u8g2_DrawStr(u8g2, 1, 62, v_a);

    u8g2_SendBuffer(u8g2);
	return;
}

int main(void) {
	u8g2_t u8g2;

	// Initialization
	u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0,
			u8x8_byte_arm_linux_hw_spi, u8x8_arm_linux_gpio_and_delay);
	init_spi_hw(&u8g2, GPIO_CHIP_NUM, SPI_BUS, OLED_SPI_PIN_DC,
			OLED_SPI_PIN_RES, OLED_SPI_PIN_CS);

	u8g2_InitDisplay(&u8g2);
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);

	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
	u8g2_DrawStr(&u8g2, 1, 18, "U8g2 HW SPI");

	u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);
	u8g2_DrawGlyph(&u8g2, 112, 56, 0x2603);

	u8g2_SendBuffer(&u8g2);

	printf("Initialized ...\n");
	sleep_ms(5000);
	u8g2_SetPowerSave(&u8g2, 1);
	
        while(1){
             DEBUG_TRACE(drawBannerText(&u8g2);)
                sleep_ms(1000);
        }

        	return 0;
}
