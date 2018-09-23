#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void){

	/*variable for c code*/
	int x,y;	

	/*framebuffer variable*/	
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	int ffp=0;
	long screensize=0;
	unsigned int *fbp = 0;
	int outx = 0, outy = 0;
	long location = 0;
	int page_size_times=0;

	/*input datas*/
	

	/*open framebuffer*/
	ffp = open ("/dev/fb0",O_RDWR);
	if(ffp < 0){
		printf("Error : Can not open framebuffer device\n");
		return -1;
	}
	if(ioctl(ffp,FBIOGET_FSCREENINFO,&finfo)){
		printf("Error reading fixed information\n");
		return -1;
	}
	if(ioctl(ffp,FBIOGET_VSCREENINFO,&vinfo)){
		printf("Error reading variable information\n");
		return -1;	
	}

	/*print framebuffer information*/
	
	printf("framebuffer is :%d Byte\n",finfo.smem_len);
 	printf("The line_length is :%d\n",finfo.line_length);
 	printf("The xres is :%d\n",vinfo.xres);
 	printf("The yres is :%d\n",vinfo.yres);
 	printf("bits_per_pixel is :%d\n",vinfo.bits_per_pixel);
	printf("page size is : %d Byte\n",getpagesize());
	
	/*memory map the framebuffer*/
	page_size_times = (vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8) / getpagesize();
	if( (vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8) % getpagesize() != 0)
	{
		page_size_times = page_size_times + 1;
	}
	screensize = page_size_times * getpagesize(); //count framebuffer size(byte)
	printf("screensize is :%ld\n", screensize);
	fbp =(unsigned int *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, ffp,0);
	if ((int*) fbp == NULL)
	{
		printf ("Error: failed to map framebuffer device to memory.\n");
		return -1;
	}


	

	/*clean the framebuffer ; I leave 600*300 for drawing result*/
	for(x=0;x<vinfo.xres;x++){	
		for(y=0;y<vinfo.yres;y++){
			
  			location = ( x * (vinfo.bits_per_pixel / 8) + y  *  finfo.line_length ) >> 2;//location is calculated in 4 bytes, so right shift 2
			*(fbp + location) = (unsigned int)(0<<24 | 255<<16 | 255<<8 | 255<<0 );//A R G B		
			
		}
	}

	

	//white background
	
	for(x=0;x<vinfo.xres;x++){
		for(y=0;y<vinfo.yres;y++){			 
			location = ( x * (vinfo.bits_per_pixel / 8) + y  *  finfo.line_length ) >> 2;
			*(fbp + location) = (unsigned int)(0<<24 | 255<<16 | 255<<8 | 255<<0 );
		}
	}


	/* red circle
	 * center : (650 ,150)
	 * r = 75
	 */

	for(x=0;x<vinfo.xres;x++){
		for(y=0;y<vinfo.yres;y++){
			double xsq = (x-960) * (x-960);
			double ysq = (y-540) * (y-540);
			double rsq = 270 * 270;
			if(xsq+ysq <= rsq) {			 
				location = ( x * (vinfo.bits_per_pixel / 8) + y  *  finfo.line_length ) >> 2;
				*(fbp + location) = (unsigned int)(0<<24 | 255<<16 | 0<<8 | 0<<0 );
			}
		}
	}
	
	
	


	printf("End\n");

	munmap (fbp, screensize);//memory unmap
  	close (ffp);//close device file


  	return 0;
}
