
#include "u8g2.h"
#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "ui_u8g2.h"

  
u8g2_t u8g2;
ui_t ui;

uint8_t my_value = 2;
uint8_t my_value2 = 2;

uint8_t touchscreen_selection = 255;

uint8_t uif_single_line_option_invers_select_u8g2(ui_t *ui, uint8_t msg);


uif_t uif_list[] = {
UIF("B0",UIF_CFLAG_IS_CURSOR_SELECTABLE,0,uif_frame_button_invers_select_u8g2),
UIF("B1",UIF_CFLAG_IS_CURSOR_SELECTABLE,0,uif_frame_button_invers_select_u8g2),
UIF("I1",UIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value,uif_input_uint8_invers_select_u8g2),
UIF("I2",UIF_CFLAG_IS_CURSOR_SELECTABLE,&my_value2,uif_single_line_option_invers_select_u8g2),
UIF("TS",UIF_CFLAG_IS_CURSOR_SELECTABLE,&touchscreen_selection,uif_single_line_option_invers_select_u8g2),
UIF("FG",UIF_CFLAG_IS_CURSOR_SELECTABLE,0,uif_goto_frame_button_invers_select_u8g2),    /* UI_GOTO has the id FG */
UIF("Fg",UIF_CFLAG_IS_CURSOR_SELECTABLE,0,uif_goto_line_button_invers_select_u8g2),    /* UI_goto has the id Fg */
UIF("FL",0,0,uif_label_u8g2)
};

fds_t fds = 

UI_FORM(1)
UI_goto(32,10,2, "Test 1")
UI_goto(32,22,101, "Story")
UI_FORM(2)
//UI_LABEL(64,10, "Ag(1jm")
UI_FIELD("I1",10, 30)
UI_BUTTON("I2",30, 30, "Banana|Apple|Peach")
//UI_BUTTON("B1",32,50, "Ok")
UI_GOTO(32,50,3, "Goto 2")
UI_BUTTON("B0",96,50, "Cancel")

UI_FORM(3)
UI_goto(32,50,2, "Goto 1")

UI_FORM(101)
UI_LABEL(0, 10, "Robot enabled.")
UI_LABEL(0, 20, "KI activated.")
UI_GOTO(32,60,102, "Move Out")
UI_GOTO(96,60,3, "Diagnostics")

UI_FORM(102)
UI_LABEL(0, 9, "Floor.")
UI_LABEL(0, 19, "Robot holder in the wall.")
UI_LABEL(0, 29, "")
UI_goto(10,41,107, "Forward")
UI_goto(10,51,103, "Backward")
UI_goto(10,61,101, "Into Holder")

UI_FORM(103)
UI_LABEL(0, 9, "Floor.")
UI_LABEL(0, 19, "")
UI_LABEL(0, 29, "")
UI_goto(10,41,102, "Forward")
UI_goto(10,51,104, "Visual Scan")
UI_goto(10,61,106, "Audio Scan")

UI_FORM(104)
UI_LABEL(0, 9, "Visual scan mode.")
UI_LABEL(0, 19, "Floor to front.")
UI_LABEL(0, 29, "Backside Window.")
//UI_goto(10,41,102, "Forward")
UI_goto(10,51,105, "Scan Window")
UI_goto(10,61,103, "Exit Scan")

UI_FORM(105)
UI_LABEL(0, 9, "Visual scan mode.")
UI_LABEL(0, 19, "Window:")
UI_LABEL(0, 29, "Black with white dots.")
//UI_goto(10,41,102, "Forward")
UI_goto(10,51,104, "Visual Scan")
UI_goto(10,61,103, "Exit Scan")

UI_FORM(106)
UI_LABEL(0, 9, "Audio scan mode.")
UI_LABEL(0, 19, "No audio.")
//UI_LABEL(0, 29, "")
//UI_goto(10,41,102, "Forward")
//UI_goto(10,51,104, "Visual Scan")
UI_goto(10,61,103, "Exit Scan")

UI_FORM(107)
UI_LABEL(0, 9, "Floor.")
UI_LABEL(0, 19, "")
//UI_LABEL(0, 29, "")
//UI_goto(10,41,102, "Forward")
UI_goto(10,51,108, "Visual Scan")
UI_goto(10,61,102, "Backward")

UI_FORM(108)
UI_LABEL(0, 9, "Visual scan mode.")
UI_LABEL(0, 19, "Touchscreen.")
UI_LABEL(0, 29, "Door with button.")
//UI_LABEL(0, 29, "")
UI_goto(10,41,109, "Press Door Button")
UI_goto(10,51,110, "Use Touchscreen")
UI_goto(10,61,107, "Exit Scan")

UI_FORM(109)
UI_LABEL(0, 9, "Visual scan mode.")
UI_LABEL(0, 19, "Door does not open.")
//UI_LABEL(0, 29, "")
//UI_goto(10,41,109, "Press Door Button")
//UI_goto(10,51,110, "Use Touchscreen")
UI_goto(10,61,107, "Exit Scan")

UI_FORM(110)
UI_LABEL(0, 9, "Touchscreen.")
UI_BUTTON("TS",0, 25, "Navigation|Energy|Damage|Robot|Security")
UI_goto(10,61,108, "Exit Touchscreen")

;


int main(void)
{
  int x, y;
  int k;

  
  u8g2_SetupBuffer_SDL_128x64_4(&u8g2, &u8g2_cb_r0);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);  
  
  
  ui_Init(&ui, fds, uif_list, sizeof(uif_list)/sizeof(uif_t));
  ui.graphics_data = &u8g2;
  ui_EnterForm(&ui);

  puts(fds);
  
  
  x = 4;  // use as height for the box
  y = 0;
  
  for(;;)
  {
    u8g2_FirstPage(&u8g2);
    do
    {
      u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);      
      u8g2_SetFont(&u8g2, u8g2_font_inb16_mf  );      
      u8g2_SetFont(&u8g2, u8g2_font_courB12_tr);      
      u8g2_SetFont(&u8g2, u8g2_font_timR12_tr);      
      u8g2_SetFont(&u8g2, u8g2_font_helvR08_tr);      
      u8g2_SetFontMode(&u8g2, 1);
      //u8g2_SetFontPosTop(&u8g2);
      //u8g2_SetFontPosBottom(&u8g2);
      //u8g2_SetFontPosCenter(&u8g2);
      u8g2_SetFontRefHeightExtendedText(&u8g2);
      
      //u8g2_DrawBox(&u8g2, 10, 10+y, 20, x);
      //u8g2_DrawFrame(&u8g2, 34, 10+y, 20, x);
      //u8g2_DrawStr(&u8g2, 10, 50, "XY");
      ui_Draw(&ui);
    } while( u8g2_NextPage(&u8g2) );
    
    do
    {
      k = u8g_sdl_get_key();
    } while( k < 0 );
    
    if ( k == 273 ) y -= 1;
    if ( k == 274 ) y += 1;
    if ( k == 276 ) x -= 1;
    if ( k == 275 ) x += 1;
    
    /*
    if ( k == 'e' ) y -= 1;
    if ( k == 'x' ) y += 1;
    if ( k == 's' ) x -= 1;
    if ( k == 'd' ) x += 1;
    */
    
    if ( k == 'q' ) break;

    if ( k == 'n' ) ui_NextField(&ui);
    if ( k == 'p' ) ui_PrevField(&ui);
    if ( k == 's' ) ui_SendSelect(&ui);

    
    if ( x < 0 )
      x = 0;
    
  }
  return 0;
}

