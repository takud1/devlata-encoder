#ifndef CUSTOM_LIBS_CD4094_H_
#define CUSTOM_LIBS_CD4094_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define RELAY13         0x01
#define RELAY18         0x02
#define RELAY14         0x04
#define RELAY10         0x08
#define RELAY11         0x10
#define RELAY17         0x20
#define RELAY12         0x40
#define RELAY07         0x80

#define RELAY09         0x0100
#define RELAY04         0x0200
#define RELAY05         0x0400
#define RELAY01         0x0800
#define RELAY06         0x1000
#define RELAY15         0x2000
#define RELAY08         0x4000
#define RELAY16         0x8000

#define RELAY19         0x010000
#define RELAY20         0x020000
#define RELAY21         0x040000

#define HCF4094_count               3
#define HCF4094_outputsper          8
#define HCF4094_outputs             HCF4094_count * HCF4094_outputsper
#define MaxAllowedNum               0xFFFFFF

//#define Clock_4094_IOdirection      P1DIR
//#define Clock_4094_port             P1OUT
//#define Clock_4094_pin              BIT5
//
//#define Strobe_4094_IOdirection     P1DIR
//#define Strobe_4094_port            P1OUT
//#define Strobe_4094_pin             BIT7
//
//#define Data_4094_IOdirection       P1DIR
//#define Data_4094_port              P1OUT
//#define Data_4094_pin               BIT6
//
//#define QS_4094_IOdirection         P3DIR
//#define QS_4094_port                P3IN
//#define QS_4094_pin                 BIT4
//
//#define WDI_IOdirection             P1DIR
//#define WDI_port                    P1OUT
//#define WDI_pin                     BIT2
//
//#define ANDgate_IOdirection         P2DIR
//#define ANDgate_port                P2OUT
//#define ANDgate_pin                 BIT5
//
//#define Relay2_IOdirection          P2DIR
//#define Relay2_port                 P2OUT
//#define Relay2_pin                  BIT3
//
//#define Relay3_IOdirection          P1DIR
//#define Relay3_port                 P1OUT
//#define Relay3_pin                  BIT4

void portInit_shiftreg(void);
void writeToShiftRegister(uint32_t value);
void triggerRelays(uint32_t ActiveRelays);

//#define CLOCK_4094_HIGH()           (Clock_4094_port |= Clock_4094_pin)
//#define CLOCK_4094_LOW()            (Clock_4094_port &= ~Clock_4094_pin)
//#define STROBE_4094_HIGH()          (Strobe_4094_port |= Strobe_4094_pin)
//#define STROBE_4094_LOW()           (Strobe_4094_port &= ~Strobe_4094_pin)
//#define DATA_4094_HIGH()            (Data_4094_port |= Data_4094_pin)
//#define DATA_4094_LOW()             (Data_4094_port &= ~Data_4094_pin)
//#define WDI_HIGH()                  (WDI_port |= WDI_pin)
//#define WDI_LOW()                   (WDI_port &= ~WDI_pin)
//#define WDI_TOGGLE()                (WDI_port ^= WDI_pin)
//#define ANDgate_HIGH()              (ANDgate_port |= ANDgate_pin)
//#define ANDgate_LOW()               (ANDgate_port &= ~ANDgate_pin)
//#define RELAY2_HIGH()               (Relay2_port |= Relay2_pin)
//#define RELAY2_LOW()                (Relay2_port &= ~Relay2_pin)
//#define RELAY3_HIGH()               (Relay3_port |= Relay3_pin)
//#define RELAY3_LOW()                (Relay3_port &= ~Relay3_pin)



#endif /* CUSTOM_LIBS_CD4094_H_ */
