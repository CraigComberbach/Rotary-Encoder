#ifndef ROTARYENCODER_H
#define	ROTARYENCODER_H
/***********Add to config file header************/
/*
//Rotary Encoder Library
#define ROTARY_ENCODER_MAJOR	0
#define ROTARY_ENCODER_MINOR	1
#define ROTARY_ENCODER_PATCH	0

//Rotary Encoder Modules
enum ROTARY_ENCODER_MODULE
{
	NUMBER_OF_ROTARY_ENCODERS
};
*/

/***************Add to config file***************/
/*
#ifndef ROTARY_ENCODER_LIBRARY
	#error "You need to include the Rotary Encoder library for this code to compile"
#endif
 */

/************* Semantic Versioning***************/
#define ROTARY_ENCODER_LIBRARY

/*************   Magic  Numbers   ***************/
/*************    Enumeration     ***************/
/***********State Machine Definitions************/
/*************Function  Prototypes***************/
void Rotary_Encoder_Routine(uint32_t time_mS);
int16_t Get_Direction(void);
void Quadrature_Decoding(int16_t CHA, int16_t CHB);

#endif	/* ROTARYENCODER_H */

