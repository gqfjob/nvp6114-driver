#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/list.h>
#include <asm/delay.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>

#include "gpio_i2c.h"

/*
param:
dec[4]= {0x60,0x62,0x64,0x66};
ch_num: audio channel number
samplerate: 0[8k], 1[16k]
bits: 0[16bits], 1[8bits]
*/
void audio_init(unsigned char dec, unsigned char ch_num, unsigned char samplerate, unsigned char bits)
{
	gpio_i2c_write(dec, 0xff, 0x01);
	if( (dec == 0x60) || (dec == 0x64))
	{
		gpio_i2c_write(dec, 0x06, 0x1a);	
		gpio_i2c_write(dec, 0x07, 0x80|(samplerate<<3)|(bits<<2));	//Rec I2C 8K 16bit : master
		if(8 == ch_num)
		{
			gpio_i2c_write(dec, 0x06, 0x1b);
			gpio_i2c_write(dec, 0x08, 0x02);
			gpio_i2c_write(dec, 0x0f, 0x54);    //set I2S right sequence
			gpio_i2c_write(dec, 0x10, 0x76);
		}
		else if(4 == ch_num)
		{
			gpio_i2c_write(dec, 0x06, 0x1b);
			gpio_i2c_write(dec, 0x08, 0x01);
			gpio_i2c_write(dec, 0x0f, 0x32);   //set I2S right sequence
		}
		gpio_i2c_write(dec, 0x13, 0x80|(samplerate<<3)|(bits<<2));	// PB I2C 8k 16bit : master
		gpio_i2c_write(dec, 0x23, 0x10);  // Audio playback out
		//gpio_i2c_write(dec, 0x23, 0x18);  // Audio mix out
	}
	else
	{
		gpio_i2c_write(dec, 0x06, 0x19);	
		gpio_i2c_write(dec, 0x07, 0x00|(samplerate<<3)|(bits<<2));	//Rec I2C 16K 16bit : slave
		gpio_i2c_write(dec, 0x13, 0x00|(samplerate<<3)|(bits<<2));	// PB I2C 8k 16bit : slave
	}	
	
	gpio_i2c_write(dec, 0x24, 0x14); //set mic_1's data to i2s_sp left channel
	gpio_i2c_write(dec, 0x25, 0x15); //set mic_2's data to i2s_sp right channel
}

void set_audio_out_vol(unsigned char dec, int val)
{
	gpio_i2c_write(dec, 0xff, 0x01);
	gpio_i2c_write(dec, 0x22, val);
}

void set_audio_in_vol(unsigned char dec, int val)
{
	gpio_i2c_write(dec, 0xff, 0x01);
	gpio_i2c_write(dec, 0x01, val);
}



