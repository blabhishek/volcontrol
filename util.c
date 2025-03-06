#include "util.h"

const char *options[] = {
	[VOL_GET] = "show",
	[VOL_INC] = "up",
	[VOL_SET] = "set",
	[MUTE_SET] = "mute",
	[VOL_DEC] = "down",
};

const char* usage = 
"Usage : volcontrol [OPTION] [VALUE]\n\
\n\
 Options :\n\
 \n\
 show - shows the current volume\n\
 set VALUE - sets volume to VALUE\n\
 up VALUE - increase volume by VALUE(default is 10)\n\
 down VALUE - decrease volume by VALUE(default is 10)\n\
 mute - mute or unmute audio output\n\
 \n\
 If no VALUE is passed, the default values are used\n";

int mixer_error_handler(mixer_error type, snd_mixer_t *handler)
{
	switch(type) {
		case FD_MIXER_OPEN:
			fprintf(stderr,"failed to open mixer\n");
			break;
		case FD_ATTACH:
			fprintf(stderr,"failed to attach mixer\n");
			break;
		case FD_REGISTER:
			fprintf(stderr,"failed to register mixer\n");
			break;
		case FD_LOAD:
			fprintf(stderr,"failed to load mixer\n");
			break;
		case FD_ELEM_FIND:
			fprintf(stderr,"failed to find mixer element\n");
			break;
		case FD_MIXER_CLOSE:
			fprintf(stderr,"failed to close mixer handler\n");
			break;
		default :
			fprintf(stderr,"mixer_error_handler() : unknown error type\n");
	}
	if(type != FD_MIXER_OPEN && type != FD_MIXER_CLOSE)
		free_mixer_handler(handler);
	exit(1);
}
void free_mixer_handler(snd_mixer_t *handler)
{
	snd_mixer_free(handler);
	if(snd_mixer_close(handler) < 0)
		mixer_error_handler(FD_MIXER_CLOSE,NULL);
}
int volume_error_handler(volume_error type)
{
	switch(type) {
		case FD_MUTE_CHECK:
			fprintf(stderr,"failed to check mute status\n");
			break;
		case FD_SET_VOL:
			fprintf(stderr,"failed to set volume\n");
			break;
		case FD_GET_VOL:
			fprintf(stderr,"failed to get volume\n");
			break;
		case FD_SET_MUTE:
			fprintf(stderr,"failed to mute/unmute\n");
			break;
	}
	return 1;
}

bool args_handler(int argc, char *argv[], struct simplevol *vol_info)
{
	if(argc == 1)
		return input_error_handler(USAGE);
	int op;
	bool found = false;

	for(op = 0; op < OP_SIZE; op++) {
		if(!strcmp(options[op],argv[1])) {
			vol_info -> vol_option = op;
			found = true;
		}
	}
	if(found == false) 
		return input_error_handler(INVALID_OP);

	op = vol_info -> vol_option;
	if(op == VOL_GET || op == MUTE_SET) 
		return true;

	if(argc >= 3) {
		long int temp_vol = atoi(argv[2]);
		if(temp_vol < VOL_MIN || temp_vol > VOL_MAX) 
			return input_error_handler(INVALID_VAL);
		vol_info -> volume = temp_vol;
		return true;
	}
	else {
		vol_info -> volume = (op == VOL_SET) ? DEFAULT_VOL_SET : DEFAULT_VOL_CHANGE;
		return input_error_handler(NO_VAL);
	}
}
bool input_error_handler(input_error type)
{
	switch(type) {
		case USAGE:
			fprintf(stderr,"%s",usage);
			return false;
			break;
		case INVALID_OP:
			fprintf(stderr,"Given option is invalid\n");
			fprintf(stderr,"volcontrol -h : for help\n"); 
			return false;
			break;
		case INVALID_VAL:
			fprintf(stderr,"Given value is out of ranger\n");
			return false;
			break;
		case NO_VAL:
			fprintf(stderr,"volume value is not given\n");
			fprintf(stderr,"using default values\n");
			return true;
			break;
		default:
			fprintf(stderr,"input_error_handler() : unknown error type\n");
			return false;
			break;
	}
}
