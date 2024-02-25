#include "cd4094.h"
#include "main.h"

//switches connected on encoder - D1, D4, D5, D8, D9, D12, D13, D16, D17, D20, D21, D22
void portInit_shiftreg(void)
{
    //Outputs
//    Clock_4094_IOdirection      |= Clock_4094_pin;
//    Strobe_4094_IOdirection     |= Strobe_4094_pin;
//    Data_4094_IOdirection       |= Data_4094_pin;
//    WDI_IOdirection             |= WDI_pin;
//    ANDgate_IOdirection         |= ANDgate_pin;
//    Relay2_IOdirection          |= Relay2_pin;
//    Relay3_IOdirection          |= Relay3_pin;

    //Inputs
//    QS_4094_IOdirection       &= ~QS_4094_pin;

    //default conditions
//    WDI_TOGGLE();
//    ANDgate_LOW();
//    STROBE_4094_LOW();
	HAL_GPIO_WritePin(STROBE_4094_GPIO_Port, STROBE_4094_Pin, RESET);
//    CLOCK_4094_LOW();
	HAL_GPIO_WritePin(CLK_4094_GPIO_Port, CLK_4094_Pin, RESET);
//    DATA_4094_LOW();
	HAL_GPIO_WritePin(DATA_4094_GPIO_Port, DATA_4094_Pin, RESET);
}


void writeToShiftRegister(uint32_t value)
{
    uint32_t tempValue = value;
    if(tempValue > MaxAllowedNum)
    {
        tempValue = MaxAllowedNum;
    }
    int j;

//    STROBE_4094_LOW();
	HAL_GPIO_WritePin(STROBE_4094_GPIO_Port, STROBE_4094_Pin, RESET);

    for(j = HCF4094_outputs-1; j>=0; j--)
    {
        bool num = 0;
        uint32_t temp1Val = 0;
        temp1Val = (tempValue >> (j - 1));
        num = (bool)(temp1Val & 0x01);

//        CLOCK_4094_LOW();
    	HAL_GPIO_WritePin(CLK_4094_GPIO_Port, CLK_4094_Pin, RESET);

        if(num == 0)
        {
//            DATA_4094_HIGH();
        	HAL_GPIO_WritePin(DATA_4094_GPIO_Port, DATA_4094_Pin, SET);

        }
        else
        {
//            DATA_4094_LOW();
        	HAL_GPIO_WritePin(DATA_4094_GPIO_Port, DATA_4094_Pin, RESET);

        }
//        CLOCK_4094_HIGH();
    	HAL_GPIO_WritePin(CLK_4094_GPIO_Port, CLK_4094_Pin, SET);

    }
//    STROBE_4094_HIGH();
	HAL_GPIO_WritePin(STROBE_4094_GPIO_Port, STROBE_4094_Pin, SET);

}

void triggerRelays(uint32_t ActiveRelays)
{
  int j;
  uint32_t ShiftRegInput = 0;

  for(j = HCF4094_outputs; j>0; j--)
  {
    bool num = 0;
    num = (bool)((ActiveRelays >> (j - 1)) & 0x01);
    if(num == 1)
    {
        switch(j)
        {
            case 1: ShiftRegInput += RELAY01;     break;
//            case 2: RELAY2_HIGH();                break;
            case 2: HAL_GPIO_WritePin(RELAY2_GPIO_Port, RELAY2_Pin, SET); break;
//            case 3: RELAY3_HIGH();                break;
            case 3: HAL_GPIO_WritePin(RELAY3_GPIO_Port, RELAY3_Pin, SET); break;

            case 4: ShiftRegInput += RELAY04;     break;
            case 5: ShiftRegInput += RELAY05;     break;
            case 6: ShiftRegInput += RELAY06;     break;
            case 7: ShiftRegInput += RELAY07;     break;
            case 8: ShiftRegInput += RELAY08;     break;
            case 9: ShiftRegInput += RELAY09;     break;
            case 10: ShiftRegInput += RELAY10;     break;
            case 11: ShiftRegInput += RELAY11;     break;
            case 12: ShiftRegInput += RELAY12;     break;
            case 13: ShiftRegInput += RELAY13;     break;
            case 14: ShiftRegInput += RELAY14;     break;
            case 15: ShiftRegInput += RELAY15;     break;
            case 16: ShiftRegInput += RELAY16;     break;
            case 17: ShiftRegInput += RELAY17;     break;
            case 18: ShiftRegInput += RELAY18;     break;
            case 19: ShiftRegInput += RELAY19;     break;
            case 20: ShiftRegInput += RELAY20;     break;
            case 21: ShiftRegInput += RELAY21;     break;
            default: break;
        }
    }
    else if(num == 0)
    {
        switch(j)
        {
//            case 2: RELAY2_LOW();           break;
			case 2: HAL_GPIO_WritePin(RELAY2_GPIO_Port, RELAY2_Pin, RESET); break;
//            case 3: RELAY3_LOW();           break;
            case 3: HAL_GPIO_WritePin(RELAY3_GPIO_Port, RELAY3_Pin, RESET); break;

            default: break;
        }
    }
  }
  writeToShiftRegister(ShiftRegInput);
}
