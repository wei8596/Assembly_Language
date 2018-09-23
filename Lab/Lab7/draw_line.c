#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
//#include <sys/ioctl.h>

 
/*define command for ioctl*/
#define SET_RST 	0
#define SET_WEA 	1
#define SET_X		10
#define SET_Y		3
#define SET_DIR		4
#define SET_LEN		5
#define SET_INDEX	6
#define READ_X		7
#define READ_Y		8
#define READ_FIN	13
/*define command for ioctl over*/


int main(void){
	
	/*c code variable*/	
	int x,y,dir,len,i;

	/*framebuffer variable*/	
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	int ffp=0;
	long screensize=0;
	unsigned int *fbp = 0,*dwp = 0;
	int outx = 0, outy = 0;
	long location = 0;
	int page_size_times=0;
	int finish=0;

	/*driver open*/
	int fp = open("/dev/drawline_dev",O_RDWR);
 	if(fp < 0){
  		printf("can't open device!\n");
  		return 0;
 	}

	/*input datas*/
	printf("\nPlease input the point X Y:\n");
	scanf("%d%d",&x,&y);
	printf("Please input the direction (0~3, default 0):\n");
	scanf("%d",&dir);
	printf("Please input the length:\n");
	scanf("%d",&len);

	
	
	/*dir in 0~3*/
	if(dir>3 || dir<0){
		dir=0;
	}

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
	printf("The mem is :%d\n",finfo.smem_len);
 	printf("The line_length is :%d\n",finfo.line_length);
 	printf("The xres is :%d\n",vinfo.xres);
 	printf("The yres is :%d\n",vinfo.yres);
 	printf("bits_per_pixel is :%d\n",vinfo.bits_per_pixel);
	
	/*memory map the framebuffer*/
	page_size_times = (vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8) / getpagesize();
	if( (vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8) % getpagesize() != 0)
	{
		page_size_times = page_size_times + 1;
	}
	screensize = page_size_times * getpagesize();
	fbp =(unsigned int *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, ffp,0);
	if ((int) fbp == -1)
	{
		printf ("Error: failed to map framebuffer device to memory.\n");
		return -1;
	}


	/*using driver*/	
	//dwp[0] = 0;
	//dwp[1] = 1;
	//dwp[2] = x;
	//dwp[3] = y;
	//dwp[4] = dir;
	//dwp[5] = len;
	//dwp[0] = 1;


	printf("%d\n",ioctl(fp,SET_RST,0));
	printf("%d\n",ioctl(fp,SET_WEA,1));
	printf("%d\n",ioctl(fp,SET_X,x));
	printf("%d\n",ioctl(fp,SET_Y,y));
	printf("%d\n",ioctl(fp,SET_DIR,dir));
	printf("%d\n",ioctl(fp,SET_LEN,len));
	printf("%d\n",ioctl(fp,SET_RST,1));



	/*clean the framebuffer ; I leave 600*300 for drawing result*/
	
	for(y=vinfo.yres-300;y<vinfo.yres;y++){
		for(x=vinfo.xres-600;x<vinfo.xres;x++){
  			location = ( x * (vinfo.bits_per_pixel / 8) + y  *  finfo.line_length ) >> 2;//location is calculated in 4 bytes, so LSL 2
			*(fbp + location) = (unsigned int)(0<<24 | 255<<16 | 255<<8 | 255<<0 );//A R G B		
		}
	}
	

	finish = ioctl(fp,READ_FIN,0);
	while(finish==0)
	{
		finish = ioctl(fp,READ_FIN,0);
	}
	for(i=0;i<999;i++);
	printf("finish=%d",ioctl(fp,READ_FIN,0));
	//while(dwp[9]==0);
	/*read ip output && draw*/
	for(i=0;i<len;i++){

		//dwp[1] = 0;
		//dwp[6] = i;
		ioctl(fp,SET_WEA,0);
		ioctl(fp,SET_INDEX,i);


		outx = ioctl(fp,READ_X,9);
		outx += vinfo.xres-600;

		outy = ioctl(fp,READ_Y,9);
		outy += vinfo.yres-300;

		location = ( outx * (vinfo.bits_per_pixel / 8) + outy  *  finfo.line_length ) >> 2;
		*(fbp + location) = (unsigned int)(0<<24 | 255<<16 | 0<<8 | 0<<0 );
		printf("(x,y) = (%d %d)\n",ioctl(fp,READ_X,i),ioctl(fp,READ_Y,i));
	}


	printf("End\n");

	munmap (fbp, screensize);//memory unmap
  	close (ffp);//close device file

	close(fp);
  	return 0;
}
