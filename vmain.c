/*
 * volcontrol is used to set,view,mute/unmute volume.
 * Author : Abhishek
 * Last Modified : 28/08/2021
 */

#include "common.h"
#include "util.h"
#include "volcontrol.h"

const char *muted = "muted";
const char *unmuted = "unmuted";
void print_vol(struct simplevol *vol_info);

int main(int argc, char *argv[]) 
{
	struct simplevol vol_info = { 20, true, VOL_GET } ;
	if(args_handler(argc,argv,&vol_info) == false)
		return 1;
	if(!simple_vol(&vol_info))
		return 2;
	print_vol(&vol_info);
	return 0;
}
void print_vol(struct simplevol *vol_info)
{
	bool temp_status = vol_info -> mute_status;
	if(vol_info -> vol_option == MUTE_SET) {
		fprintf(stdout,"%s\n",(temp_status) ? unmuted : muted);
		return;
	}
	if(temp_status == false) {
		fprintf(stdout,"%s\n",muted);
		return;
	}
	fprintf(stdout,"%ld\n",vol_info -> volume);
	return;
}
