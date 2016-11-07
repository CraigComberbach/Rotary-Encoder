//Fill out Header information
//Add ASCII flow chart using ASCIIflow.com
/**************************************************************************************************
Target Hardware:		PIC24F...
Chip resources used:	
Code assumptions:		
Purpose:				
Notes:					
 
Version History:
v0.1.0	2016-11-06	Craig Comberbach	Compiler: XC16 v1.11	IDE: MPLABx 3.30	Tool: RealICE	Computer: Intel Core2 Quad CPU 2.40 GHz, 5 GB RAM, Windows 10 Home 64-bit
	First version
**************************************************************************************************/
/*************    Header Files    ***************/
#include "Config.h"
#include "RotaryEncoder.h"
#include "../../../Libraries/Pins/Pins.h"

/************* Semantic Versioning***************/
#if ROTARY_ENCODER_MAJOR != 0
	#error "RotaryEncoder.c has had a change that loses some previously supported functionality"
#elif ROTARY_ENCODER_MINOR != 1
	#error "RotaryEncoder.c has new features that this code may benefit from"
#elif ROTARY_ENCODER_PATCH != 0
	#error "RotaryEncoder.c has had a bug fix, you should check to see that we weren't relying on a bug for functionality"
#endif

/************Arbitrary Functionality*************/
/*************   Magic  Numbers   ***************/
#define DEBOUNCE_THRESHOLD	2
#define FAST_THRESHOLD		75
#define SLOW_THRESHOLD		1000

/*************    Enumeration     ***************/
/***********State Machine Definitions*************/
/*************  Global Variables  ***************/
int16_t changeNotification[3];
int16_t debouncing;
int16_t direction;

/*************Function  Prototypes***************/
/************* Device Definitions ***************/
/************* Module Definitions ***************/
/************* Other  Definitions ***************/

void Rotary_Encoder_Routine(uint32_t time_mS)
{
	Quadrature_Decoding(Pin_Read(RE_CHB_PIN_RA1), Pin_Read(RE_CHA_PIN_RA2));

	return;
}

void Quadrature_Decoding(int16_t CHA, int16_t CHB)
{
	static int16_t previousState = -1;
	static int16_t timerActive = 0;
	static int16_t timer = 0;
	int16_t currentState = -1;
	int16_t localDirection = 0;

	//Error checking
	if((CHA != 0) && (CHA != 1))
		return;
	if((CHB != 0) && (CHB != 1))
		return;

	//Combine channels (CH A/B)
	currentState = CHA << 1;
	currentState |= CHB;

	if(previousState == -1)
	{
		previousState = currentState;
		return;
	}

	//Switch has not moved
 	if(previousState == currentState)
	{
		//And timer is active
		if(timerActive == 1)
		{
			timer++;//Advance Timer
			if(timer > SLOW_THRESHOLD)
			{
				timer = 0;//Reset Timer
				timerActive = 0;//Stop Timer
				direction = 0;//No Advance detected
			}
			return;
		}
		else//Timer is not active
		{
			timer = 0;//Reset Timer
			return;
		}
	}
	else//Switch has moved
	{
		//Valid move?
		switch(previousState)
		{
			case 0b00:
				switch(currentState)
				{
					case 0b10://Increasing
						localDirection = 1;//Record Direction
						break;
					case 0b01://Decreasing
						localDirection = -1;//Record Direction
						break;
					case 0b00://No change
						//Fall through
					case 0b11://Skipped a step?
						//Fall through
					default:
						previousState = currentState;//Record State
						timer = 0;//Reset Timer 
						return;
				}
				break;
			case 0b10:
				switch(currentState)
				{
					case 0b11://Increasing
						localDirection = 1;//Record Direction
						break;
					case 0b00://Decreasing
						localDirection = -1;//Record Direction
						break;
					case 0b10://No Change
						//Fall through
					case 0b01://Skipped a step?
						//Fall through
					default:
						previousState = currentState;//Record State
						timer = 0;//Reset Timer 
						return;
				}
				break;
			case 0b11:
				switch(currentState)
				{
					case 0b01://Increasing
						localDirection = 1;//Record Direction
						break;
					case 0b10://Decreasing
						localDirection = -1;//Record Direction
						break;
					case 0b11://No Change
						//Fall through
					case 0b00://Skipped a step?
						//Fall through
					default:
						previousState = currentState;//Record State
						timer = 0;//Reset Timer 
						return;
				}
				break;
			case 0b01:
				switch(currentState)
				{
					case 0b00://Increasing
						localDirection = 1;//Record Direction
						break;
					case 0b11://Decreasing
						localDirection = -1;//Record Direction
						break;
					case 0b01://No Change
						//Fall through
					case 0b10://Skipped a step?
						//Fall through
					default:
						previousState = currentState;//Record State
						timer = 0;//Reset Timer
						return;
				}
				break;
			default:
				return;
		}
		
		//Start Timer
		timerActive = 1;

		if(timer <= DEBOUNCE_THRESHOLD)
		{
			previousState = currentState;//Record State
			timer = 0;//Reset Timer
			return;
		}
		else if(timer <= FAST_THRESHOLD)
		{
			direction = localDirection;//Record Direction
			direction *=10;//Fast Advance detected
			previousState = currentState;//Record State
			timer = 0;//Reset Timer
			return;
		}
		else if(timer <= SLOW_THRESHOLD)
		{
			direction = localDirection;//Record Direction
			direction *=1;//Slow Advance detected
			previousState = currentState;//Record State
			timer = 0;//Reset Timer
			return;
		}
		else
		{
			direction = 0;//No Advance detected
			timerActive = 0;//Stop Timer
			previousState = currentState;//Record State
			timer = 0;//Reset Timer
			return;
		}

		//Ermahgerd, how did we get hear!
		while(1)
			ClrWdt();
	}

	return;
}

int16_t Get_Direction(void)
{
	return direction;
}
