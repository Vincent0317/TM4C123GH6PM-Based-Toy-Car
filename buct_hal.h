#include  "tm4c123.h"
#include <stdint.h>
#include <stdio.h>
#define MAX_DUTY 2000


void digitalWrite(unsigned int PortNumber, unsigned int bit, unsigned state);
int digitalRead(unsigned int PortNumber, unsigned int bit);
void enableGPIO(unsigned int PortNumber);
void pinMode(unsigned int PortNumber, unsigned int bit, unsigned int mode);
void initQEI0(void);
int getQEI0Position();
int getQEI0Velocity();
void initSerial(int BaudRate);
void eputchar(char c);
char egetchar();
void eputs (const char *s);
char HexDigit(int Value);
void eputi(int x);
void eputd(int d);
void egets(char *string, int max);
int fputs(const char * s, FILE *  F);
int fputc(int c, FILE *  F);
void initQEI1(void);
int getQEI1Position();
int getQEI1Velocity();
void initMotorPWM(void);
void setMotor1(int Percent);
void setMotor2(int Percent);
