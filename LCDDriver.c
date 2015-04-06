/*
 * File:   LCDDriver.c
 * Author: Eng. Juan Camilo Gómez Cadavid MSc.
 * Generic HD447804 Hitachi LCD_Driver 2x16 - 4x16
 * Created on 15 de febrero de 2010, 08:17 AM
 */

#include "LCDDriver.h"
__LCD_DVR_iopins_t  __lcd_dvr_io_pins;
const unsigned char rowaddr[4] = {0x00, 0x40, 0x14, 0x54};
unsigned char __lcd_dispctrl=0x00;
/*==================================================================================================================================================================================*/
void LCD_Init( void (*RS_Control)(unsigned char c), void (*RW_Control)(unsigned char c), void (*EN_Control)(unsigned char c), void (*DataControl)(unsigned char c), void (*WaitusFcn)(unsigned int time)){
    __lcd_dvr_io_pins.RS_Control = RS_Control;
    __lcd_dvr_io_pins.RW_Control = RW_Control;
    __lcd_dvr_io_pins.EN_Control = EN_Control;
    __lcd_dvr_io_pins.DataControl = DataControl;
    __lcd_dvr_io_pins.WaitusFcn = WaitusFcn;
    __lcd_dvr_io_pins.EN_Control(0);
    __lcd_dvr_io_pins.RS_Control(0);
    if(__lcd_dvr_io_pins.RW_Control!=NULL)  __lcd_dvr_io_pins.RW_Control(0);
    __lcd_dvr_io_pins.WaitusFcn(LCD_INIT_DELAY_US);

    LCD_IOWrite4(0x03);
    __lcd_dvr_io_pins.WaitusFcn(5);
    LCD_Strobe();
    __lcd_dvr_io_pins.WaitusFcn(200);
    LCD_Strobe();
    __lcd_dvr_io_pins.WaitusFcn(200);
    LCD_IOWrite4(0x02);
    __lcd_dvr_io_pins.WaitusFcn(120);
    LCD_Command(E_FUNCTION_SET | BIT_DL_DATALENGTH_4 | BIT_N_DISP_LINES_2 | BIT_F_FONT_5_10);
    LCD_Command(E_DISPLAY_ON_OFF_CTRL | BIT_D_DISPLAY_OFF);
    LCD_Command(E_CLEAR_DISPLAY);
    LCD_Command(E_ENTRY_MODE_SET | BIT_S_AUTOSCROLL_OFF | BIT_ID_INCREMENT_CURSOR);
}
/*============================================================================*/
void LCD_IOWrite4(unsigned char data){
    __lcd_dvr_io_pins.DataControl(data);
    __lcd_dvr_io_pins.EN_Control(1);
    __lcd_dvr_io_pins.WaitusFcn(LCD_SEND_STROBE_US);
    __lcd_dvr_io_pins.EN_Control(0);
    __lcd_dvr_io_pins.WaitusFcn(LCD_SEND_STROBE_US); 
 }
/*============================================================================*/
void LCD_Strobe(void){
    unsigned char x=3;
    __lcd_dvr_io_pins.EN_Control(1);
    while(x--);
    x=3;
    __lcd_dvr_io_pins.EN_Control(0);
    while(x--);
}
/*============================================================================*/
void LCD_Send(unsigned char data, unsigned char rs){
    __lcd_dvr_io_pins.RS_Control(rs);
    LCD_IOWrite4(data>>4); // send high nibble first
    LCD_IOWrite4(data);
    __lcd_dvr_io_pins.WaitusFcn(LCD_SEND_DELAY_US);
}
/*============================================================================*/
void LCD_OnOff(unsigned char mode){
    if (mode) __lcd_dispctrl |= BIT_D_DISPLAY_ON;
    else __lcd_dispctrl &= ~BIT_D_DISPLAY_ON; 
    LCD_Command(E_DISPLAY_ON_OFF_CTRL | __lcd_dispctrl);
}
/*============================================================================*/
void LCD_Cursor(enum enLCDCursorModes mode){
    __lcd_dispctrl &= 0xFC;
    __lcd_dispctrl |= mode;
    LCD_Command(E_DISPLAY_ON_OFF_CTRL | __lcd_dispctrl);
}
/*============================================================================*/
void LCD_CursorPos(unsigned char left_rigth){
    LCD_Command(E_CURSOR_DISPLAY_SHIFT | BIT_SC_SHIFT_CURSOR | ((left_rigth)? BIT_RL_SHIFT_LEFT: BIT_RL_SHIFT_RIGHT));
}
/*============================================================================*/
void LCD_ScrollPos(unsigned char left_rigth){
    LCD_Command(E_CURSOR_DISPLAY_SHIFT | BIT_SC_SHIFT_DISPLAY | ((left_rigth)? BIT_RL_SHIFT_LEFT: BIT_RL_SHIFT_RIGHT));
}
/*============================================================================*/
void LCD_AutoScroll(unsigned char on_off){
    LCD_Command(E_ENTRY_MODE_SET | ((on_off)? BIT_S_AUTOSCROLL_ON:BIT_S_AUTOSCROLL_ON) | BIT_ID_INCREMENT_CURSOR);
}
/*============================================================================*/
void LCD_16x2_ScreenOut(const char *LCDRow1, const char *LCDRow2, unsigned char clear_screen){
    if (clear_screen) LCD_Clear();
    LCD_Goto(0,0);
    LCD_Puts(LCDRow1);
    LCD_Goto(1,0);
    LCD_Puts(LCDRow2);
}
/*============================================================================*/
void LCD_16x4_ScreenOut(const char *LCDRow1, const char *LCDRow2, const char *LCDRow3, const char *LCDRow4, unsigned char clear_screen){
    if (clear_screen) LCD_Clear();
    LCD_Goto(0,0);
    LCD_Puts(LCDRow1);
    LCD_Goto(1,0);
    LCD_Puts(LCDRow2);
    LCD_Goto(2,0);
    LCD_Puts(LCDRow3);
    LCD_Goto(3,0);
    LCD_Puts(LCDRow4);
}
/*============================================================================*/
void LCD_Put(const char c){
   LCD_Write(c);
}
/*============================================================================*/
void LCD_Puts(const char *s){
   while (*s) LCD_Write(*s++);
}
/*============================================================================*/
void LCD_Goto(const unsigned char row, const unsigned char col){
   LCD_Command(E_SET_DDRAM_ADDR | (col + rowaddr[ row ]));
}
/*============================================================================*/
void LCD_Home(void){
    LCD_Command(E_RETURN_HOME);
    __lcd_dvr_io_pins.WaitusFcn(LCD_COMMAND_DELAY_US); 
}
/*============================================================================*/
void LCD_Clear (void){
    LCD_Command(E_CLEAR_DISPLAY);
    __lcd_dvr_io_pins.WaitusFcn(LCD_COMMAND_DELAY_US); 
}
/*============================================================================*/
void LCD_CreateChar(unsigned charnum, const unsigned char *chardata){
    unsigned char i;
    charnum &= 0x07;
    LCD_Command(E_SET_CGRAM_ADDR | (charnum << 3));
    for (i = 0; i < 8; i++)
    	LCD_Write(chardata[i]);
}
/*============================================================================*/