#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

// 常用命令定义
#define OLED_ADDR        0x3C    // OLED I2C地址(0x3C或0x3D)
#define OLED_CMD_MODE    0x00    // 命令模式
#define OLED_DATA_MODE   0x40    // 数据模式

// 显示参数
#define OLED_WIDTH       128     // OLED宽度
#define OLED_HEIGHT      64      // OLED高度
#define OLED_PAGES       8       // OLED页数 (64/8=8页)

// 函数声明
void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetPosition(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, char chr);
void OLED_ShowString(uint8_t x, uint8_t y, char *str);
void OLED_WriteCmd(uint8_t cmd);
void OLED_WriteData(uint8_t data);
void OLED_Display_On(void);
void OLED_Display_Off(void);

#endif /* OLED_H_ */