#include "util.h"
#include "volcontrol.h"

int simple_vol(struct simplevol *vol_info)
{
	snd_mixer_t *handler;
	snd_mixer_elem_t *element;
	snd_mixer_selem_id_t *sid;

	static const char* mixer_name = "Master";
	static const char* card = "default";
	static int mix_index = 0;

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, mix_index);
	snd_mixer_selem_id_set_name(sid, mixer_name);

	if((snd_mixer_open(&handler,0)) < 0) 
		mixer_error_handler(FD_MIXER_OPEN,NULL);

	if((snd_mixer_attach(handler,card)) < 0)
		mixer_error_handler(FD_ATTACH,NULL);

	if((snd_mixer_selem_register(handler,NULL,NULL)) < 0) 
		mixer_error_handler(FD_REGISTER,handler);

	if((snd_mixer_load(handler)) < 0 )
		mixer_error_handler(FD_LOAD,NULL);

	if((element = snd_mixer_find_selem(handler,sid)) == NULL)
		mixer_error_handler(FD_ELEM_FIND,handler);

	if(vol_control(element,vol_info) < 0) {
		free_mixer_handler(handler);	
		return 0;
	}
	free_mixer_handler(handler);	
	return 1;
}
int set_mute(snd_mixer_elem_t *element, int value)
{
	if(snd_mixer_selem_set_playback_switch_all(element,value) < 0) {
		volume_error_handler(FD_SET_MUTE);
		return 0;
	}
	return 1;
}

int vol_control(snd_mixer_elem_t *element, struct simplevol *vol_info)
{
	bool temp_mute;
	if((temp_mute = check_mute_status(element)) < 0)
		volume_error_handler(FD_MUTE_CHECK);

	if(vol_info -> vol_option == MUTE_SET) {
		set_mute(element, !temp_mute);
		vol_info -> mute_status = !temp_mute;
		return !temp_mute;
	}
	if(!temp_mute) {
		vol_info -> mute_status = false;
		return 0;
	}
	long int temp_volume;
	if(!volume_get(element,&temp_volume))
		return -1;

	if(vol_info -> vol_option == VOL_GET) {
		vol_info -> volume = temp_volume;
		return 1;
	}
	if(vol_info -> vol_option == VOL_SET)
		return (volume_set(element,vol_info -> volume)) ? 1 : -1;

	if(vol_info -> vol_option == VOL_INC) {
		temp_volume += vol_info -> volume;
		temp_volume = (temp_volume <= 100) ? temp_volume : 100;
	}
	if(vol_info -> vol_option == VOL_DEC) {
		temp_volume -= vol_info -> volume;
		temp_volume = (temp_volume >= 0) ? temp_volume : 0;
	}
	if(!volume_set(element,temp_volume))
		return -1;
	else {
		vol_info -> volume  = temp_volume;
		return 1;
	}
}
int check_mute_status(snd_mixer_elem_t *element)
{
	int status;
	/* status = 0 is muted and 1 is unmuted */
	if(snd_mixer_selem_get_playback_switch(element,0,&status) < 0) {
		volume_error_handler(FD_MUTE_CHECK);
		return -1;
	}
	return status;
}
int volume_get(snd_mixer_elem_t *element, long int *volume)
{
	long int min_vol, max_vol;

	snd_mixer_selem_get_playback_volume_range(element,&min_vol,&max_vol);
	if(snd_mixer_selem_get_playback_volume(element, 0,volume) < 0) {
		volume_error_handler(FD_GET_VOL);
		return 0;
	}
	max_vol -= min_vol;
	*volume -= min_vol;
	*volume = (int) *volume / (float) max_vol * 100 + 0.5;
	return 1;
}
int volume_set(snd_mixer_elem_t *element, long int volume)
{
	long int min_vol,max_vol;

	snd_mixer_selem_get_playback_volume_range(element,&min_vol,&max_vol);
	volume = (volume * (max_vol - min_vol) / 100) + min_vol;
	if(snd_mixer_selem_set_playback_volume_all(element,volume) < 0) {
		volume_error_handler(FD_SET_VOL);
		return 0;
	}
	return 1;
}
