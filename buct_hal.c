#include  "tm4c123.h"
#include <stdint.h>
#include <stdio.h>
#include "buct_hal.h"

void digitalWrite(unsigned int PortNumber, unsigned int bit, unsigned state)
{
    if (PortNumber < 6)
    {
        volatile uint32_t *Port = (volatile uint32_t *)(GPIOA_BASE + 0x1000*PortNumber);
        if (state != 0)
        {
            *(Port + (GPIODATA>>2)) |= (1 << bit);
        }
        else
        {
            *(Port + (GPIODATA>>2)) &= ~(1 << bit);
        }
    }
}
int digitalRead(unsigned int PortNumber, unsigned int bit)
{
    if (PortNumber < 6)
    {
        volatile uint32_t *Port = (volatile uint32_t *)(GPIOA_BASE + 0x1000*PortNumber);
        uint32_t state = (*(Port + (GPIODATA>>2))) & (1 << bit);
        if (state != 0)
            state = 1;
        return state;
    }
    else
        return 0;
}
void enableGPIO(unsigned int PortNumber)
{
    if (PortNumber < 6)
    {
        SYSCTL_RCGC2 = SYSCTL_RCGC2 | (1 << PortNumber); // turn on GPIO
        SYSCTL_GPIOHBCTL = SYSCTL_GPIOHBCTL | (1 << PortNumber); // turn on AHB access to GPIO
    }
}
void pinMode(unsigned int PortNumber, unsigned int bit, unsigned int mode)
{
    if (PortNumber < 6)
    {
        enableGPIO(PortNumber); // make sure the port is enabled
        volatile uint32_t *Port = (volatile uint32_t *)(GPIOA_BASE + 0x1000*PortNumber);
        if (PortNumber==GPIOF)
        {
            // Bit 0 of GPIOF is locked at boot.  Need to unlock first
            GPIOF_LOCK = 0x4C4F434B;    // password
            GPIOF_CR = 1; // Commit changes for Bit 0
        }
        switch (mode)
        {
            case INPUT: {
                *(Port+(GPIODEN>>2)) |= (1 << bit); // enable digital mode
                *(Port+(GPIOAFSEL>>2)) &= ~(1 << bit); // disable alternative function
                *(Port+(GPIODIR>>2)) &= ~(1 << bit); // configure the bit as an input
                break;
            }
            case INPUT_PULLUP: {
                            *(Port+(GPIODEN>>2)) |= (1 << bit); // enable digital mode
                            *(Port+(GPIOAFSEL>>2)) &= ~(1 << bit); // disable alternative function
                            *(Port+(GPIODIR>>2)) &= ~(1 << bit); // configure the bit as an input
                            *(Port+(GPIOPUR>>2)) |= (1 << bit); // enable pull-up resistor
                            break;
             }
             case OUTPUT: {
                *(Port+(GPIODEN>>2)) |= (1 << bit); // enable digital mode
                *(Port+(GPIOAFSEL>>2)) &= ~(1 << bit); // disable alternative function
                *(Port+(GPIODIR>>2)) |= (1 << bit); // configure the bit as an output
                break;
            }
            case ALTERNATE: {
                *(Port+(GPIODEN>>2)) |= (1 << bit); // enable digital mode (assumption)
                *(Port+(GPIOAFSEL>>2)) |= (1 << bit); // enable alternative function
                break;
            }
            case ANALOG: {
                *(Port+(GPIODEN>>2)) &= ~(1 << bit); // disable digital mode (assumption)
                *(Port+(GPIOAFSEL>>2)) &= ~(1 << bit); // disable alternative function
                *(Port+(GPIOAMSEL>>2)) |= (1 << bit); // enable analog mode
                break;
            }
        }
    }
}

void initQEI0(void)
{
    // Quadrature encoder is connected to PD6 and PD7
    // These correspond to PhaseA0 and PhaseB0
    SYSCTL_RCGCQEI |= (1 << 0); // enable the clock for QEI0
    SYSCTL_RCGCGPIO |= (1 << 3); // enable GPIOD
    SYSCTL_GPIOHBCTL |= (1 << 3); // enable AHB access to GPIOD
    GPIOD_LOCK = 0x4C4F434B; // password
    GPIOD_CR = 1<<7; // Commit changes
    GPIOD_DEN |= (1 << 6) + (1 << 7); // digital mode for bits 6 and 7 of GPIOD
    GPIOD_AFSEL |= (1 << 6) + (1 << 7); // alternate function mode for bits 6 and 7
    GPIOD_PCTL &= ~((0x0f << 24) + (0x0f << 28)); // zero out pin control value for bits 6 and 7
    GPIOD_PCTL |= ((6 << 24) + (6 << 28)); // set pin control value for bits 6 and 7

    QEI0_CTL = 0x00000020;
    QEI0_LOAD = 8000000; // This sets the timing window to 1 second when system clock is 16MHz

    QEI0_MAXPOS = 1000;
    QEI0_CTL |= 1;
}
int getQEI0Position()
{
    return QEI0_POS;
}
int getQEI0Velocity()
{
    int speed = QEI0_SPEED;
    int direction = QEI0_STAT >> 1;
    if (direction)
        speed = -speed;
    return speed;
}


void initQEI1(void)
{
    // Quadrature encoder is connected to PC5 and PC6
    // These correspond to PhaseA1 and PhaseB1
    SYSCTL_RCGCQEI |= (1 << 1); // enable the clock for QEI1
    SYSCTL_RCGCGPIO |= (1 << 2); // enable GPIOC
    SYSCTL_GPIOHBCTL |= (1 << 2); // enable AHB access to GPIOC
    GPIOC_DEN |= (1 << 5) + (1 << 6); // digital mode for bits 5 and 6 of GPIOC
    GPIOC_AFSEL |= (1 << 5) + (1 << 6); // alternate function mode for bits 5 and 6
    GPIOC_PCTL &= ~((0x0f << 20) + (0x0f << 24)); // zero out pin control value for bits 5 and 6
    GPIOC_PCTL |= ((6 << 20) + (6 << 24)); // zero out pin control value for bits 5 and 6

    QEI1_CTL = 0x00000020;
    QEI1_LOAD = 8000000; // This sets the timing window to 1 second when system clock is 16MHz

    QEI1_MAXPOS = 1000;
    QEI1_CTL |= 1;
}
int getQEI1Position()
{
    return QEI1_POS;
}
int getQEI1Velocity()
{
    int speed = QEI1_SPEED;
    int direction = QEI1_STAT >> 1;
    if (direction)
        speed = -speed;
    return speed;
}
void initMotorPWM(void)
{
    enableGPIO(GPIOB);
    pinMode(GPIOB,4,ALTERNATE);  // alternate function for Timer (AF 7)
    pinMode(GPIOB,5,ALTERNATE);  // alternate function for Timer (AF 7)
    pinMode(GPIOB,6,ALTERNATE);  // alternate function for Timer (AF 7)
    pinMode(GPIOB,7,ALTERNATE);  // alternate function for Timer (AF 7)
    GPIOB_PCTL &= ~(0xf << 4*4);
    GPIOB_PCTL &= ~(0xf << 4*5);
    GPIOB_PCTL &= ~(0xf << 4*6);
    GPIOB_PCTL &= ~(0xf << 4*7);
    GPIOB_PCTL |= (0x07 << 4*4);
    GPIOB_PCTL |= (0x07 << 4*5);
    GPIOB_PCTL |= (0x07 << 4*6);
    GPIOB_PCTL |= (0x07 << 4*7);
    // Turn on Timer 0 and Timer 1
    SYSCTL_RCGCTIMER |= BIT0 + BIT1;
    // Configure the Timers
    GPTM0_32_CTL = 0; // start by disabling the timers
    GPTM0_32_CFG = 4; // select 16 bit mode
    GPTM1_32_CTL = 0; // start by disabling the timers
    GPTM1_32_CFG = 4; // select 16 bit mode

    // Set bit 10 to force reload of register to align with
    // end of PWM period (avoids glitches)
    // Set T0BMS=1, T0BCMR = 0; T0BMR=2;
    // This puts the timer into PWM mode
    GPTM0_32_TAMR = BIT3+BIT1+BIT10;
    // Set T0BMS=1, T0BCMR = 0; T0BMR=2;
    GPTM0_32_TBMR = BIT3+BIT1+BIT10;
    // Set T1AMS=1, T1ACMR = 0; T1AMR=2;
    GPTM1_32_TAMR = BIT3+BIT1+BIT10;;
    // Set T1BMS=1, T1BCMR = 0; T1BMR=2;
    GPTM1_32_TBMR = BIT3+BIT1+BIT10;

// Set period and initial duty for all 4 channels
    GPTM0_32_TAILR = MAX_DUTY+1;
    GPTM0_32_TAMATCHR = 0;

    GPTM0_32_TBILR = MAX_DUTY+1;
    GPTM0_32_TBMATCHR = 0;

    GPTM1_32_TAILR = MAX_DUTY+1;
    GPTM1_32_TAMATCHR = 0;

    GPTM1_32_TBILR = MAX_DUTY+1;
    GPTM1_32_TBMATCHR = 0;


    // enable T0A and T0B with output inversion
    GPTM0_32_CTL |= BIT0+BIT8+BIT6+BIT14;
    // enable T1A and T1B with output inversion
    GPTM1_32_CTL |= BIT0+BIT8+BIT6+BIT14;
}
void stop(void)
{
    GPTM0_32_TAMATCHR = 0;
    GPTM0_32_TBMATCHR = 0;
    GPTM1_32_TAMATCHR = 0;
    GPTM1_32_TBMATCHR = 0;
}
void forwards()
{
    GPTM0_32_TAMATCHR = 0;
    GPTM0_32_TBMATCHR = MAX_DUTY/2;
    GPTM1_32_TAMATCHR = MAX_DUTY/2;
    GPTM1_32_TBMATCHR = 0;
}
void backwards()
{
    GPTM0_32_TAMATCHR = MAX_DUTY;
    GPTM0_32_TBMATCHR = 0;
    GPTM1_32_TAMATCHR = 0;
    GPTM1_32_TBMATCHR = MAX_DUTY;
}

void setMotor1(int Percent)
{
    if (Percent >= 0)
    {
        GPTM1_32_TAMATCHR = (Percent*MAX_DUTY)/100;
        GPTM1_32_TBMATCHR = 0;
    }
    else
    {
        GPTM1_32_TAMATCHR = 0;
        GPTM1_32_TBMATCHR = (-Percent*MAX_DUTY)/100;
    }
}
void setMotor2(int Percent)
{
    if (Percent >= 0)
    {
        GPTM0_32_TAMATCHR = (Percent*MAX_DUTY)/100;
        GPTM0_32_TBMATCHR = 0;
    }
    else
    {
        GPTM0_32_TAMATCHR = 0;
        GPTM0_32_TBMATCHR = (-Percent*MAX_DUTY)/100;
    }
}
void initSerial(int BaudRate) {
    int BaudRateDivisor;
// Turn on the clock for GPIOA (uart 0 uses it) - not sure if I need this
    SET_BIT(SYSCTL_RCGC2,BIT0);          // turn on GPIOA
    SET_BIT(SYSCTL_GPIOHBCTL,BIT0);      // turn on AHB access to GPIOA
// Turn on the clock for the UART0 peripheral
    SYSCTL_RCGCUART |= BIT0;

// Ensure alternate function number for PA0 and PA1
    SET_BIT(GPIOA_AFSEL,BIT0+BIT1);
    SET_BIT(GPIOA_PUR,BIT0+BIT1);
    SET_BIT(GPIOA_DEN,BIT0+BIT1);
    BaudRateDivisor = 16000000;                // assuming 16MHz clock
    BaudRateDivisor = BaudRateDivisor / (16 * BaudRate);

    UART0_IBRD = BaudRateDivisor;
    UART0_DR=0;
    UART0_LCRH = BIT6+BIT5; // no parity, 8 data bits, 1 stop bit
    UART0_CTL = BIT8+BIT9+BIT0; // enable tx, rx and uart
}
void eputchar(char c)
{
    UART0_DR=c;
    // Wait for transmission to finish
    while(UART0_FR & BIT3);
}
char egetchar()
{
    while(UART0_FR & BIT4); // wait for a char
    return UART0_DR;
}
void eputs (const char *s)
{
    while(*s != 0)
    {
        eputchar(*s);
        s++;
    }
}
char HexDigit(int Value)
{
    if ((Value >=0) && (Value < 10))
        return Value+'0';
    else if ((Value >9) && (Value < 16))
        return Value-10 + 'A';
    else
        return 'z';
}

void eputi(int x)
{
    // Output the number over the serial port as
    // as hexadecimal string.
    char TxString[9];
    int Index=8;
    TxString[Index]=0; // terminate the string
    Index--;
    while(Index >=0)
    {
        TxString[Index]=HexDigit(x & 0x0f);
        x = x >> 4;
        Index--;
    }
    eputs(TxString);
}
void eputd(int d)
{
// Values will range from -2147483648 to +2147483647
    char TxString[12];
    int Index=11;
    TxString[Index]=0; // terminate the string
    Index--;
    if (d < 0)
    {
        TxString[0] = '-';
        d = -d;
    }
    else
    {
        TxString[0] = '+';
    }
    while(Index > 0)
    {
        TxString[Index]= (d % 10) + 48; // convert digit to ASCII
        d = d / 10;
        Index--;
    }
    eputs(TxString);
}
void egets(char *string, int max)
{
    int index = 0;
    char c;
    eputs("Enter a command\r\n");
    while(index < max)
    {
        c = egetchar();
        string[index]=c;
        index++;
        if (c==13)
        {
            break;
        }

    }
    string[index]=0;
}
// Adding our own version of fputs and fputc
// so that printf output can be redirected to
// the UART.
// NOTE: printf requires a lot of stack space
// If you plan on printing floating point numbers
// then set the stack size to 2kB at least.
int fputs(const char * s, FILE *  F)
{
    eputs(s);
    return 0;
}
int fputc(int c, FILE *  F)
{
    eputchar(c);
    return 0;
}
