/*
 * File:   LCDDriver.h
 * Author: Eng. Juan Camilo Gómez Cadavid MSc.
 * Generic HD447804 Hitachi LCD_Driver 2x16 - 4x16
 * Created on 15 de febrero de 2010, 08:17 AM
 */


#ifndef LCDDRIVER_H
#define	LCDDRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

    
    #include <stdlib.h>
    
    #define LCD_SEND_DELAY_US      (40)
    #define LCD_SEND_STROBE_US     (1)
    #define LCD_COMMAND_DELAY_US   (1640)   // 1.64mS
    #define LCD_INIT_DELAY_US      (16000)  //16mS  power on lcd stabilization time

    // Entry Mode Set Control Bits
    #define BIT_S_AUTOSCROLL_ON	(1<<0)		//!< Enable autoscroll. For use with Entry Mode Set command
    #define BIT_S_AUTOSCROLL_OFF	0		//!< Disable autoscroll. For use with Entry Mode Set command
    #define BIT_ID_INCREMENT_CURSOR	(1<<1)	//!< Increment cursor position after each char. For use with Entry Mode Set command
    #define BIT_ID_DECREMENT_CURSOR	0		//!< Decrement cursor position after each char. For use with Entry Mode Set command
    // Display On/Off Control Bits
    #define BIT_B_CURSOR_BLINK	(1<<0)
    #define BIT_B_CURSOR_NO_BLINK	0
    #define BIT_C_CURSOR_ON		(1<<1)
    #define BIT_C_CURSOR_OFF	0
    #define BIT_D_DISPLAY_ON	(1<<2)
    #define BIT_D_DISPLAY_OFF	0
    // Cursor / Display Shift Control Bits
    #define BIT_RL_SHIFT_RIGHT	(1<<2)
    #define BIT_RL_SHIFT_LEFT	0
    #define BIT_SC_SHIFT_DISPLAY	(1<<3)		//!< Seting this bit causes a display scroll
    #define BIT_SC_SHIFT_CURSOR	0		//!< Clearing this bits causes a cursor move
    // Function set Control Bits
    #define BIT_F_FONT_5_10		(1<<2)
    #define BIT_F_FONT_5_8		0
    #define BIT_N_DISP_LINES_2	(1<<3)
    #define BIT_N_DISP_LINES_1	0
    #define BIT_DL_DATALENGTH_8	(1<<4)
    #define BIT_DL_DATALENGTH_4	0

    enum enLcdCommands {
        E_CLEAR_DISPLAY = 0x01,
        E_RETURN_HOME = 0x02,
        E_ENTRY_MODE_SET = 0x04,
        E_DISPLAY_ON_OFF_CTRL = 0x08,
        E_CURSOR_DISPLAY_SHIFT = 0x10,
        E_FUNCTION_SET = 0x20,
        E_SET_CGRAM_ADDR = 0x40,
        E_SET_DDRAM_ADDR = 0x80,
    };

    enum enLCDCursorModes { //This enumeration defines the available cursor modes
        E_LCD_CURSOR_OFF = 0x00,
        E_LCD_CURSOR_ON = 0x02,
        E_LCD_CURSOR_ON_BLINK = 0x03,
    };

    //typedef unsigned char lcd_pin_t;
    //typedef unsigned char lcd_config_t;

    typedef struct{
        void (*RS_Control)(unsigned char c);
        void (*RW_Control)(unsigned char c);
        void (*EN_Control)(unsigned char c);
        void (*DataControl)(unsigned char c);
        void (*WaitusFcn)(unsigned int time);
    }__LCD_DVR_iopins_t;
    extern __LCD_DVR_iopins_t  __lcd_dvr_io_pins;

  
    #define LCD_Command(_data_)  LCD_Send(_data_, 0)
    #define LCD_Write(_data_)    LCD_Send(_data_, 1)

    void LCD_Init( void (*RS_Control)(unsigned char c), void (*RW_Control)(unsigned char c), void (*EN_Control)(unsigned char c), void (*DataControl)(unsigned char c), void (*WaitusFcn)(unsigned int time));
    void LCD_IOWrite4(unsigned char data);
    void LCD_OnOff(unsigned char mode);
    void LCD_Send(unsigned char data, unsigned char rs);
    void LCD_Put(const char c);
    void LCD_Puts(const char *s);
    void LCD_Goto(const unsigned char row, const unsigned char col);
    void LCD_Clear (void);
    void LCD_Home(void);
    void LCD_16x2_ScreenOut(const char *LCDRow1, const char *LCDRow2, unsigned char clear_screen);
    void LCD_16x4_ScreenOut(const char *LCDRow1, const char *LCDRow2, const char *LCDRow3, const char *LCDRow4, unsigned char clear_screen);
    void LCD_CreateChar(unsigned charnum, const unsigned char *chardata);
    void LCD_Cursor(enum enLCDCursorModes mode);
    void LCD_AutoScroll(unsigned char on_off);
    void LCD_CursorPos(unsigned char left_rigth);
    void LCD_ScrollPos(unsigned char left_rigth);
    void LCD_Strobe(void);
#ifdef	__cplusplus
}
#endif

#endif	/* LCDDRIVER_H */

