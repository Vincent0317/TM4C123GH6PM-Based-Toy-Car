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

void LEDBlingANDDiDi(void)//������ʾ
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);//��GPIOF��2,3,4,5��������Ϊ���
    //�Ȱ׵�Ƶ������,���з�����
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x00);//������
    SysCtlDelay(SysCtlClockGet()/20);//��ʱ0.15��
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x3C);//��
    SysCtlDelay(SysCtlClockGet()/20);//��ʱ0.15��
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x00);//������
    SysCtlDelay(SysCtlClockGet()/20);//��ʱ0.15��
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x3C);//��
    SysCtlDelay(SysCtlClockGet()/20);//��ʱ0.15��
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x00);//������
    SysCtlDelay(SysCtlClockGet()/20);//��ʱ0.15��
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x3C);//��
    SysCtlDelay(SysCtlClockGet()/6);//��ʱ0.5��
    //�������������
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x18);//��
    SysCtlDelay(SysCtlClockGet()/15);//��ʱ0.15��
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x14);//��
    SysCtlDelay(SysCtlClockGet()/15);//��ʱ0.15��
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x0C);//��
    SysCtlDelay(SysCtlClockGet()/15);//��ʱ0.15��
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,0x3C);//��
    SysCtlDelay(SysCtlClockGet()/15);//��ʱ0.15��
}

void redOff()
{
    GPIOA_DATA |=(1<<2);
}
void redOn()
 {
    GPIOA_DATA &= ~(1 << 2); // clear bit 2
 }
//�죺0x18���̣�0x14������0x0C���ƣ�0x10���ף�0x00
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
    // ����ʱ��
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART5)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}

    // �������Ÿ��ù���
    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);

    // ��������Ϊ UART ����
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // ���� UART ����
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // ���� UART5
    UARTEnable(UART5_BASE);
}

char UART5_ReceiveChar(void) {
    if (UARTCharsAvail(UART5_BASE)) {
        return UARTCharGet(UART5_BASE);
    }
    return 0;  // �����ݷ���0
}

void UART5_SendString(const char *str) {
    while (*str) {
        UARTCharPut(UART5_BASE, *str++);
    }
}
// �����ʱ����
void soft_delay(volatile uint32_t dly)
{
    while(dly--);
}

// ��ӡ��ǰ����ٶȺ���
void displayMotorSpeeds(void)
{
    int leftSpeed = getQEI0Velocity();
    int rightSpeed = -getQEI1Velocity();
    char ls[32];
    char rs[32];

    // ��ӡ����ٶ�
    sprintf(ls, "Left: %d\r\n", leftSpeed);
    UART5_SendString(ls);
    sprintf(rs, "Right: %d\r\n", rightSpeed);
    UART5_SendString(rs);
}

// �ֱ�������ҵ���ٶ�
void moveForward(int leftSpeed, int rightSpeed)
{
    if(leftSpeed < 0) leftSpeed = 0;
    if(rightSpeed < 0) rightSpeed = 0;
    if(leftSpeed > 100) leftSpeed = 100;
    if(rightSpeed > 100) rightSpeed = 100;

    setMotor1(leftSpeed);
    setMotor2(rightSpeed);
}

// �ֱ�������ҵ���ٶ�
void moveBackward(int leftSpeed, int rightSpeed)
{
    if(leftSpeed < 0) leftSpeed = 0;
    if(rightSpeed < 0) rightSpeed = 0;
    if(leftSpeed > 100) leftSpeed = 100;
    if(rightSpeed > 100) rightSpeed = 100;

    setMotor1(-leftSpeed);
    setMotor2(-rightSpeed);
}

// ֹͣ����
void stopMotors(void)
{
    setMotor1(0);
    setMotor2(0);
}

// ��ǰת�亯��
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

// ��ǰת�亯��
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

// ���ת�亯��
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

// �Һ�ת�亯��
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

// ����ת���� - ���ֺ��ˣ�����ǰ��
void spinLeft(int speed)
{
    if(speed < 0) speed = 0;
    if(speed > 100) speed = 100;

    setMotor1(-speed);
    setMotor2(speed);
}

// ����ת���� - ����ǰ�������ֺ���
void spinRight(int speed)
{
    if(speed < 0) speed = 0;
    if(speed > 100) speed = 100;

    setMotor1(speed);
    setMotor2(-speed);
}
void Frank(void)
{
    // ��ʼ��UART
    initSerial(9600);

    // ��ʼ�����������
    initQEI0();
    initQEI1();

    // ��ʼ�����PWM
    initMotorPWM();

    // �ȴ������ȶ�
    soft_delay(1000000);
}

// ��ʼ��������������
void InitUltrasonic(void)
{
    // ����ԭ�е���������
    uint32_t originalPadConfig;
    uint32_t originalDEN;
    uint32_t originalDIR;
    
    // ��ȡ��ǰ����
    originalDEN = GPIOA_DEN;
    originalDIR = GPIOA_DIR;
    
    // ֻ������Ҫ�Ĳ��֣����޸�����λ
    // PA7(TRIG)����Ϊ�����PA6(ECHO)����Ϊ����
    GPIOA_DEN |= (BIT6 | BIT7);           // ʹ�����ֹ���
    GPIOA_DIR |= BIT7;                    // PA7Ϊ���
    GPIOA_DIR &= ~BIT6;                   // PA6Ϊ����
    
    // ����PA6Ϊ����������ʹ��������GPIOPadConfigSet�����⸲��������������
    uint32_t padReg = GPIOA_PDR;
    padReg |= BIT6;
    GPIOA_PDR = padReg;
    
    // ȷ��TRIG��ʼΪ�͵�ƽ
    GPIOA_DATA &= ~BIT7;
}

// �������뺯��
float MeasureDistance(void)
{
    uint32_t pulseWidth = 0;
    float distance = 0.0;
    uint32_t timeout;
    
    // ȷ��TRIG����Ϊ��
    GPIOA_DATA &= ~BIT7;                  // TRIG = 0
    SysCtlDelay(1333);                    // Լ100us��ʱ
    
    // ���ʹ�������
    GPIOA_DATA |= BIT7;                   // TRIG = 1
    SysCtlDelay(200);                     // Լ15us��ʱ��ȷ������10us
    
    GPIOA_DATA &= ~BIT7;                  // TRIG = 0
    
    // �ȴ�ECHO���
    timeout = 1000000;
    while(!(GPIOA_DATA & BIT6) && timeout--) {
        // �ȴ��ز���ʼ
    }
    
    if(timeout == 0) {
        return -1.0f; // ��ʱ��δ�յ��ز�
    }
    
    // ����ECHO�ߵ�ƽ����ʱ��
    pulseWidth = 0;
    timeout = 1000000;
    
    while((GPIOA_DATA & BIT6) && timeout--) {
        pulseWidth++;
        // �������ʱ����߾���
    }
    
    if(timeout == 0) {
        return -1.0f; // ��ʱ���ز�����
    }
    
    // ������� - ʹ�ñ�׼��ʽ
    // ����340m/s��������Ҫ����2
    // ����(cm) = ʱ��(us) / 142
    distance = (float)pulseWidth / 142.0f; // ��׼У׼ϵ��
    
    // ��Ч�Լ��
    if(distance < 1.0f || distance > 400.0f) {
        return -1.0f; // ������Ч��Χ
    }
    
    return distance;
}
void Stop(void)
{
    whiteOff();
    redOn();
    DiOn();
    SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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
    // ����ϵͳʱ��Ϊ 16MHz ����PLL��Ƶ4��ϵͳʱ��40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL |SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//ʹ������
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {}
    LEDBlingANDDiDi();//�ƺͷ�������ʼ��

    // ��ʼ��UART
    InitUART5();
    initSerial(9600);


    // ��ʼ��I2C
    initI2C();

    // ��ʼ��OLED
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

    // ��ʼ��������������
    InitUltrasonic();

    // ��ʼ�����������
    initQEI0();
    initQEI1();

    // ��ʼ�����PWM
    initMotorPWM();

    // �ȴ������ȶ�
    soft_delay(1000000);


    // ��ӡ��ӭ��Ϣ
    eputs("Car Control Demo Started\r\n");
    UART5_SendString("JDY-31 Bluetooth Motor Control Ready\r\n");
    char cmd;
    // �򵥵���ʾ����
    GPIOA_DEN = GPIOA_DEN | ( (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) ); // digital mode bits 0,1,2,3,4 of GPIOF
    GPIOA_DIR = GPIOA_DIR | ( (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) ); // make bits 1,2,3 outputs on GPIOF
//        GPIOA_DEN |= (1<<6)|(1<<7);
//        GPIOA_DIR |=  (1<<6);   // PA6 ���
//        GPIOA_DIR &= ~(1<<7);   // PA7 ����

    //  enableGPIO(GPIOA);
    //  // �����ֿ���
    //  GPIOA_DEN |= (BIT6 | BIT7);
    //  // PA6 ���������PA7 ����
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
                        case 'M': // ��������                           
                            // ��������
                            UART5_SendString("Measuring distance...\r\n");
                            distance = MeasureDistance();
                            
                            // ����������
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
                        case 'O': // ǰ���Զ�����
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
                                moveForward(62, 60);  // ����ǰ��
                                soft_delay(200000);   // ������ʱ��������
                            }
                            break;

                        case 'F': // ǰ��
                            //��
                            whiteOff();
                            greenOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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
                        case 'B': // ����
                            //��
                            whiteOff();
                            blueOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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
                       case 'Q': // ����ת
                            //��
                            whiteOff();
                            yellowOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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
                        case 'E': // ����ת
                            //��
                            whiteOff();
                            yellowOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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
                        case 'S': // ֹͣ
                            //��
                            whiteOff();
                            redOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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
                        case 'A'://��ǰת
                            //��
                            whiteOff();
                            purpleOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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
                        case 'D'://��ǰת
                            //��
                            whiteOff();
                            purpleOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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
                        case 'Z'://���ת
                            //��
                            whiteOff();
                            cyanOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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
                        case 'C'://�Һ�ת
                            //��
                            whiteOff();
                            cyanOn();
                            DiOn();
                            SysCtlDelay(SysCtlClockGet()/10);//��ʱ0.15��
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



