#ifndef COMMON_H
#define COMMON_H

#include<stdio.h>
#include<alsa/asoundlib.h>
#include<stdlib.h>

extern const char *options[];
extern const char *usage;

#define OP_SIZE sizeof(options) / sizeof(options[0])
#define VOL_MAX 100
#define VOL_MIN 0
#define DEFAULT_VOL_SET 50
#define DEFAULT_VOL_CHANGE 10


typedef enum { FD_MIXER_OPEN,FD_ATTACH,FD_REGISTER,FD_LOAD,FD_ELEM_FIND,FD_MIXER_CLOSE} mixer_error;
typedef enum { FD_MUTE_CHECK,FD_SET_VOL,FD_GET_VOL,FD_SET_MUTE} volume_error;
typedef enum { USAGE,INVALID_OP,INVALID_VAL,NO_VAL } input_error;

typedef enum { VOL_GET, VOL_INC, VOL_DEC, VOL_SET, MUTE_SET } volume_action;
typedef enum { false = 0,true = 1 } bool;

struct simplevol {
	long int volume;
	bool mute_status;
	volume_action vol_option;
};

#endif
