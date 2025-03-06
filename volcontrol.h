#ifndef VOLCONTROL_H
#define VOLCONTROL_H

#include "common.h"
int simple_vol(struct simplevol *vol_info);
int set_mute(snd_mixer_elem_t *element, int value);
int vol_control(snd_mixer_elem_t *element, struct simplevol *vol_info);
int check_mute_status(snd_mixer_elem_t *element);
int volume_get(snd_mixer_elem_t *element, long int *volume);
int volume_set(snd_mixer_elem_t *element, long int volume);

#endif
