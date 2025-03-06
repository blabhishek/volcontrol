#ifndef UTIL_H
#define UTIL_H

#include "common.h"
int mixer_error_handler(mixer_error type, snd_mixer_t *handler);
void free_mixer_handler(snd_mixer_t *handler);
int volume_error_handler(volume_error type);
bool args_handler(int argc, char *argv[], struct simplevol *vol_info);
bool input_error_handler(input_error type);

#endif
