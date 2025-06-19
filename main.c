#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "hw_memmap.h"
#include "tm4c123.h"
#include "buct_hal.h"
#include "tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"
#include "oled.h"

void UART2Send(const char *pcBuf)
{
    while (*pcBuf)
    {
        UARTCharPut(UART2_BASE, *pcBuf++);
    }
}

void LEDBlingANDDiDi(void)//开机提示
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);//将GPIOF的2,3,4,5引脚设置为输出
    //先白灯频闪三次,伴有蜂鸣器
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x00);//三个灯
    SysCtlDelay(SysCtlClockGet()/20);//延时0.15秒
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x3C);//灭
    SysCtlDelay(SysCtlClockGet()/20);//延时0.15秒
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x00);//三个灯
    SysCtlDelay(SysCtlClockGet()/20);//延时0.15秒
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x3C);//灭
    SysCtlDelay(SysCtlClockGet()/20);//延时0.15秒
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x00);//三个灯
    SysCtlDelay(SysCtlClockGet()/20);//延时0.15秒
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x3C);//灭
    SysCtlDelay(SysCtlClockGet()/6);//延时0.5秒
    //后红绿蓝依次亮
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x18);//红
    SysCtlDelay(SysCtlClockGet()/15);//延时0.15秒
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x14);//绿
    SysCtlDelay(SysCtlClockGet()/15);//延时0.15秒
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x0C);//蓝
    SysCtlDelay(SysCtlClockGet()/15);//延时0.15秒
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x3C);//灭
    SysCtlDelay(SysCtlClockGet()/15);//延时0.15秒
}

void redOff()
{
    GPIOA_DATA |=(1<<2);
}
void redOn()
 {
    GPIOA_DATA &= ~(1 << 2); // clear bit 2
 }
//红：0x18，绿：0x14，蓝：0x0C，黄：0x10，白：0x00
void greenOff()
{
    GPIOA_DATA |=(1<<3);
}
void greenOn()
 {
    GPIOA_DATA &= ~(1 << 3); //
 }
void blueOff()
{
    GPIOA_DATA |=(1<<4);
}
void blueOn()
 {
    GPIOA_DATA &= ~(1 << 4); //
 }
void yellowOff()
{
    GPIOA_DATA |=((1<<2)|(1<<3));
}
void yellowOn()
 {
    GPIOA_DATA &= ~((1<<2)|(1<<3)); //
 }
void whiteOff()
{
    GPIOA_DATA |=((1<<2)|(1<<3)|(1<<4));
}
void whiteOn()
 {
    GPIOA_DATA &= ~((1<<2)|(1<<3)|(1<<4)); //
 }
void purpleOff()
{
    GPIOA_DATA |=((1<<2)|(1<<4));
}
void purpleOn()
 {
    GPIOA_DATA &= ~((1<<2)|(1<<4)); //
 }
void cyanOff()
{
    GPIOA_DATA |=((1<<3)|(1<<4));
}
void cyanOn()
 {
    GPIOA_DATA &= ~((1<<3)|(1<<4)); //
 }
void DiOff()
{
    GPIOA_DATA |=(1<<5);
}
void DiOn()
{
    GPIOA_DATA &= ~(1<<5);
}

void InitUART5(void)
{
    // 开启时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART5)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}

    // 配置引脚复用功能
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);

    // 设置引脚为 UART 功能
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // 配置 UART 参数
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // 启用 UART5
    UARTEnable(UART5_BASE);
}

char UART5_ReceiveChar(void) {
    if (UARTCharsAvail(UART5_BASE)) {
        return UARTCharGet(UART5_BASE);
    }
    return 0;  // 无数据返回0
}

void UART5_SendString(const char *str) {
    while (*str) {
        UARTCharPut(UART5_BASE, *str++);
    }
}
// 软件延时函数
void soft_delay(volatile uint32_t dly)
{
    while(dly--);
}

// 打印当前电机速度函数
void displayMotorSpeeds(void)
{
    int leftSpeed = getQEI0Velocity();
    int rightSpeed = -getQEI1Velocity();
    char ls[32];
    char rs[32];

    // 打印电机速度
    sprintf(ls, "Left: %d\r\n", leftSpeed);
    UART5_SendString(ls);
    sprintf(rs, "Right: %d\r\n", rightSpeed);
    UART5_SendString(rs);
}

// 分别控制左右电机速度
void moveForward(int leftSpeed, int rightSpeed)
{
    if(leftSpeed < 0) leftSpeed = 0;
    if(rightSpeed < 0) rightSpeed = 0;
    if(leftSpeed > 100) leftSpeed = 100;
    if(rightSpeed > 100) rightSpeed = 100;

    setMotor1(leftSpeed);
    setMotor2(rightSpeed);
}

// 分别控制左右电机速度
void moveBackward(int leftSpeed, int rightSpeed)
{
    if(leftSpeed < 0) leftSpeed = 0;
    if(rightSpeed < 0) rightSpeed = 0;
    if(leftSpeed > 100) leftSpeed = 100;
    if(rightSpeed > 100) rightSpeed = 100;

    setMotor1(-leftSpeed);
    setMotor2(-rightSpeed);
}

// 停止函数
void stopMotors(void)
{
    setMotor1(0);
    setMotor2(0);
}

// 左前转弯函数
void turnForwardLeft(int leftSpeed, int rightSpeed)
{
    if(leftSpeed < 0) leftSpeed = 0;
    if(rightSpeed < 0) rightSpeed = 0;
    if(leftSpeed > 100) leftSpeed = 100;
    if(rightSpeed > 100) rightSpeed = 100;

    if(leftSpeed > rightSpeed) {
        int temp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = temp;
    }

    setMotor1(leftSpeed);
    setMotor2(rightSpeed);
}

// 右前转弯函数
void turnForwardRight(int leftSpeed, int rightSpeed)
{
    if(leftSpeed < 0) leftSpeed = 0;
    if(rightSpeed < 0) rightSpeed = 0;
    if(leftSpeed > 100) leftSpeed = 100;
    if(rightSpeed > 100) rightSpeed = 100;

    if(rightSpeed > leftSpeed) {
        int temp = rightSpeed;
        rightSpeed = leftSpeed;
        leftSpeed = temp;
    }

    setMotor1(leftSpeed);
    setMotor2(rightSpeed);
}

// 左后转弯函数
void turnBackwardLeft(int leftSpeed, int rightSpeed)
{
    if(leftSpeed < 0) leftSpeed = 0;
    if(rightSpeed < 0) rightSpeed = 0;
    if(leftSpeed > 100) leftSpeed = 100;
    if(rightSpeed > 100) rightSpeed = 100;

    if(leftSpeed > rightSpeed) {
        int temp = leftSpeed;
        leftSpeed = rightSpeed;
        rightSpeed = temp;
    }

    setMotor1(-leftSpeed);
    setMotor2(-rightSpeed);
}

// 右后转弯函数
void turnBackwardRight(int leftSpeed, int rightSpeed)
{
    if(leftSpeed < 0) leftSpeed = 0;
    if(rightSpeed < 0) rightSpeed = 0;
    if(leftSpeed > 100) leftSpeed = 100;
    if(rightSpeed > 100) rightSpeed = 100;

    if(rightSpeed > leftSpeed) {
        int temp = rightSpeed;
        rightSpeed = leftSpeed;
        leftSpeed = temp;
    }

    setMotor1(-leftSpeed);
    setMotor2(-rightSpeed);
}

// 左自转函数 - 左轮后退，右轮前进
void spinLeft(int speed)
{
    if(speed < 0) speed = 0;
    if(speed > 100) speed = 100;

    setMotor1(-speed);
    setMotor2(speed);
}

// 右自转函数 - 左轮前进，右轮后退
void spinRight(int speed)
{
    if(speed < 0) speed = 0;
    if(speed > 100) speed = 100;

    setMotor1(speed);
    setMotor2(-speed);
}
void Frank(void)
{
    // 初始化UART
    initSerial(9600);

    // 初始化电机编码器
    initQEI0();
    initQEI1();

    // 初始化电机PWM
    initMotorPWM();

    // 等待串口稳定
    soft_delay(1000000);
}

// 初始化超声波传感器
void InitUltrasonic(void)
{
    // 保存原有的引脚配置
    uint32_t originalPadConfig;
    uint32_t originalDEN;
    uint32_t originalDIR;
    
    // 读取当前配置
    originalDEN = GPIOA_DEN;
    originalDIR = GPIOA_DIR;
    
    // 只配置需要的部分，不修改其他位
    // PA7(TRIG)配置为输出，PA6(ECHO)配置为输入
    GPIOA_DEN |= (BIT6 | BIT7);           // 使能数字功能
    GPIOA_DIR |= BIT7;                    // PA7为输出
    GPIOA_DIR &= ~BIT6;                   // PA6为输入
    
    // 配置PA6为弱下拉但不使用完整的GPIOPadConfigSet，避免覆盖其他引脚设置
    uint32_t padReg = GPIOA_PDR;
    padReg |= BIT6;
    GPIOA_PDR = padReg;
    
    // 确保TRIG初始为低电平
    GPIOA_DATA &= ~BIT7;
}

// 测量距离函数
float MeasureDistance(void)
{
    uint32_t pulseWidth = 0;
    float distance = 0.0;
    uint32_t timeout;
    
    // 确保TRIG引脚为低
    GPIOA_DATA &= ~BIT7;                  // TRIG = 0
    SysCtlDelay(1333);                    // 约100us延时
    
    // 发送触发脉冲
    GPIOA_DATA |= BIT7;                   // TRIG = 1
    SysCtlDelay(200);                     // 约15us延时，确保大于10us
    
    GPIOA_DATA &= ~BIT7;                  // TRIG = 0
    
    // 等待ECHO变高
    timeout = 1000000;
    while(!(GPIOA_DATA & BIT6) && timeout--) {
        // 等待回波开始
    }
    
    if(timeout == 0) {
        return -1.0f; // 超时，未收到回波
    }
    
    // 计数ECHO高电平持续时间
    pulseWidth = 0;
    timeout = 1000000;
    
    while((GPIOA_DATA & BIT6) && timeout--) {
        pulseWidth++;
        // 不添加延时，提高精度
    }
    
    if(timeout == 0) {
        return -1.0f; // 超时，回波过长
    }
    
    // 计算距离 - 使用标准公式
    // 声速340m/s，来回需要除以2
    // 距离(cm) = 时间(us) / 142
    distance = (float)pulseWidth / 142.0f; // 标准校准系数
    
    // 有效性检查
    if(distance < 1.0f || distance > 400.0f) {
        return -1.0f; // 超出有效范围
    }
    
    return distance;
}
void Stop(void)
{
    whiteOff();
    redOn();
    DiOn();
    SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
    DiOff();
    redOff();
    redOn();
    eputs("Stopping\r\n");
    stopMotors();
    soft_delay(800000);
    displayMotorSpeeds();
    soft_delay(6000000);
    UART5_SendString("Stopped\r\n");
}

int main(void)
{
    // 配置系统时钟为 16MHz 晶振，PLL分频4，系统时钟40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL |SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//使能外设
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {}
    LEDBlingANDDiDi();//灯和蜂鸣器初始化

    // 初始化UART
    InitUART5();
    initSerial(9600);


    // 初始化I2C
    initI2C();

    // 初始化OLED
    OLED_Init();
    soft_delay(500000);
    OLED_ShowString(0, 0, "Hello, I'm your car!");
    soft_delay(500000);
    OLED_ShowString(0, 1, "Group Member:");
    soft_delay(500000);
    OLED_ShowString(0, 2, "Zhang Wencong");
    soft_delay(500000);
    OLED_ShowString(0, 3, "Zhang Fengkai");
    soft_delay(500000);
    OLED_ShowString(0, 4, "Wang Ziyi");
    soft_delay(500000);
    OLED_ShowString(0, 5, "Wen Shiduo");
    soft_delay(500000);
    OLED_ShowString(0, 7, "GOGOGO, chu fa lou!");

    if (init_accel_6050() < 0)
    {
        while(1)
            printf("Can't find the accelerometer\r\n");
    }

    // 初始化超声波传感器
    InitUltrasonic();

    // 初始化电机编码器
    initQEI0();
    initQEI1();

    // 初始化电机PWM
    initMotorPWM();

    // 等待串口稳定
    soft_delay(1000000);


    // 打印欢迎信息
    eputs("Car Control Demo Started\r\n");
    UART5_SendString("JDY-31 Bluetooth Motor Control Ready\r\n");
    char cmd;
    // 简单的演示程序
    GPIOA_DEN = GPIOA_DEN | ( (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) ); // digital mode bits 0,1,2,3,4 of GPIOF
    GPIOA_DIR = GPIOA_DIR | ( (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) ); // make bits 1,2,3 outputs on GPIOF
//        GPIOA_DEN |= (1<<6)|(1<<7);
//        GPIOA_DIR |=  (1<<6);   // PA6 输出
//        GPIOA_DIR &= ~(1<<7);   // PA7 输入

    //  enableGPIO(GPIOA);
    //  // 打开数字口能
    //  GPIOA_DEN |= (BIT6 | BIT7);
    //  // PA6 输出触发，PA7 输入
    //  GPIOA_DIR |=  BIT6;
    //  GPIOA_DIR &= ~BIT7;

    float distance;
    while(1)
    {
        //UART5_SendString("!!JDY-31 Bluetooth Motor Control Ready\r\n");

        cmd = UART5_ReceiveChar();
                if (cmd != 0) {
                    UARTCharPut(UART5_BASE, cmd);
                    UART5_SendString("\r\n");
                    switch (cmd) {
                        case 'M': // 测量距离                           
                            // 测量距离
                            UART5_SendString("Measuring distance...\r\n");
                            distance = MeasureDistance();
                            
                            // 输出测量结果
                            if(distance > 0) {
                                char distStr[32];
                                int intPart = (int)distance;
                                int decPart = (int)((distance - intPart) * 10);
                                sprintf(distStr, "Distance: %d.%d cm\r\n", intPart, decPart);
                                UART5_SendString(distStr);
                            } else {
                                UART5_SendString("Distance measurement failed!\r\n");
                            }
                            break;
                        case 'O': // 前进自动避障
                            whiteOff();
                            greenOn();
                            UART5_SendString("Obstacle Avoiding Forward\r\n");
                            eputs("Obstacle Avoiding Forward Mode\r\n");
                            while (1) {
                                float d = MeasureDistance();
                                if (d > 0 && d < 30.0f) {
                                    UART5_SendString("Obstacle detected! Stopping...\r\n");
                                    Stop();
                                    break;
                                }
                                moveForward(62, 60);  // 继续前进
                                soft_delay(200000);   // 短暂延时后继续检测
                            }
                            break;

                        case 'F': // 前进
                            //绿
                            whiteOff();
                            greenOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
                            DiOff();
                            greenOff();
                            greenOn();
                            eputs("Moving Forward\r\n");
                            moveForward(61, 60);
                            soft_delay(8000000);
                            displayMotorSpeeds();
                            soft_delay(3000000);
                            UART5_SendString("Moving Forward\r\n");
                            stop();
                            break;
                        case 'B': // 后退
                            //蓝
                            whiteOff();
                            blueOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
                            DiOff();
                            blueOff();
                            blueOn();
                            eputs("Moving Backward\r\n");
                            moveBackward(61, 60);
                            soft_delay(8000000);
                            displayMotorSpeeds();
                            soft_delay(3000000);
                            UART5_SendString("Moving Backward\r\n");
                            stop();
                            break;
                       case 'Q': // 左自转
                            //黄
                            whiteOff();
                            yellowOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
                            DiOff();
                            yellowOff();
                            yellowOn();
                            eputs("Spinning Left\r\n");
                            spinLeft(70);
                            soft_delay(6000000);
                            displayMotorSpeeds();
                            soft_delay(3000000);
                            UART5_SendString("Spinning Left\r\n");
                            stop();
                            break;
                        case 'E': // 右自转
                            //黄
                            whiteOff();
                            yellowOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
                            DiOff();
                            yellowOff();
                            yellowOn();
                            eputs("Spinning Right\r\n");
                            spinRight(70);
                            soft_delay(6000000);
                            displayMotorSpeeds();
                            soft_delay(3000000);
                            UART5_SendString("Spinning Right\r\n");
                            stop();
                            break;
                        case 'S': // 停止
                            //红
                            whiteOff();
                            redOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
                            DiOff();
                            redOff();
                            redOn();
                            eputs("Stopping\r\n");
                            stopMotors();
                            soft_delay(800000);
                            displayMotorSpeeds();
                            soft_delay(6000000);
                            UART5_SendString("Stopped\r\n");
                            break;
                        case 'A'://左前转
                            //紫
                            whiteOff();
                            purpleOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
                            DiOff();
                            purpleOff();
                            purpleOn();
                            eputs("Turning Forward Left\r\n");
                            turnForwardLeft(57, 65);
                            soft_delay(8000000);
                            displayMotorSpeeds();
                            soft_delay(3000000);
                            UART5_SendString("Turning Forward Left\r\n");
                            stop();
                            break;
                        case 'D'://右前转
                            //紫
                            whiteOff();
                            purpleOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
                            DiOff();
                            purpleOff();
                            purpleOn();
                            eputs("Turning Forward Right\r\n");
                            turnForwardRight(67, 55);
                            soft_delay(8000000);
                            displayMotorSpeeds();
                            soft_delay(3000000);
                            UART5_SendString("Turning Forward Right\r\n");
                            stop();
                            break;
                        case 'Z'://左后转
                            //青
                            whiteOff();
                            cyanOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
                            DiOff();
                            cyanOff();
                            cyanOn();
                            eputs("Turning Backward Left\r\n");
                            turnBackwardLeft(57, 65);
                            soft_delay(8000000);
                            displayMotorSpeeds();
                            soft_delay(3000000);
                            UART5_SendString("Turning Backward Left\r\n");
                            stop();
                            break;
                        case 'C'://右后转
                            //青
                            whiteOff();
                            cyanOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//延时0.15秒
                            DiOff();
                            cyanOff();
                            cyanOn();
                            eputs("Turning Backward Right\r\n");
                            turnBackwardRight(67, 55);
                            soft_delay(8000000);
                            displayMotorSpeeds();
                            soft_delay(3000000);
                            UART5_SendString("Turning Backward Right\r\n");
                            stop();
                            break;
                        default:
                            UART5_SendString("Unknown Command\r\n");
                            break;


                    }
                }
    }
}



