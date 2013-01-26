/*
 Plasma by Jan Møller & Erik Hansen.
 
 It was compiled in Borland c++, in ANSI mode, using the HUGE memory model.
 Feel free to use it as you desire, you may even name it after your
 grandmother. We don't care.
 The reason why we made it? Well... First of all we wanted to make some
 plasma, just for fun. But when we had finished it turned out to be much
 faster than the plasma we have seen in intros, demos etc...
 Normally the color-cells are 2x4 4x4 or even larger, the 2x4 cell-plasma
 was awfully slow, bot ours ain't, even though it is 2x2 cells.
 Well how can that be???                        ^^^^^^^^^
 Our secret lies in the plasma-calculation!
 (I assume you have guessed that	part already)
 We simply calculate as much as possible before we start showing the goddies.
 The table 'Tab1' is a simple table (320x200 yields 64k) with the distance
 from (x,y) to the center (rounded off to char by simple overflow). The second
 table 'Tab2' is similar to 'Tab1', except we molested it with sine.
 In the mainloop we calculate a body (160x100) by accessing the two tables
 with different pairs of (x,y) and add them.
 (see for yourself in 'CalculateBody')
 And KaPoW. We have the fastest plasma...
 (i.e. the fastest we have ever seen.(on a 486)) If I am not correct then please notify me.
 
 If you have any questions, comments or whips, then we would be happy to answer.
 
 Contact us trough E-Mail:
 
 fwiffo@daimi.aau.dk
 
 or
 
 martino@daimi.aau.dk
 
 (If you can optimize it (e.g. write it in asm) we would be very interested
 to see the results!)
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>
#include <dos.h>
#define uchar unsigned char
#define ulong unsigned long
#define uint unsigned int
#define PEL_write 0x3c8
#define PEL_read 0x3c7
#define PEL_data 0x3c9
#define box_w 160
#define box_h 100
#define pi 3.1415926L
//-------------------------------------------------
uchar body[box_w*box_h];	// buffer for the bitmap body
uchar colors[256];			// color table
uchar tab1[64000];			// table one for thr plasma
uchar tab2[64000];			// table two for the plasma
uchar picture[64004];		// buffer for the picture
//-------------------------------------------------

//**********************************************************
//*******************  ASM SOURCES  ************************
//**********************************************************

void OnVideo(void)
{
	asm mov bl,0x36
	asm mov ax,0x1200
	asm int 0x10
}
void OffVideo(void)
{
	asm mov bl,0x36
	asm mov ax,0x1201
	asm int 0x10
}
void SetMode(char mode)
{
	asm mov ah,0x00
	asm mov al,mode
	asm int 0x10
}
char GetMode()
{
	char mode;
	asm mov ah,0x0f
	asm int 0x10
	asm mov mode,al
	return (mode);
}
void WaitRast()
{
	asm mov dx,0x3da
bra1:
	asm in  al,dx
	asm and al,8
	asm jnz bra1
bra2:
	asm in  al,dx
	asm and al,8
	asm jz bra2
}
void SetBank(unsigned char wbank, unsigned char rbank)
{
	unsigned char n=rbank<<4|wbank;
	asm mov al,n
	asm mov dx,0x3cd	// Point to memory segment register
	asm out dx,al		// Set it.
}
void SetDisplayStart(int adsr)
{
	// This part uses standard VGA DisplayStart registers
	asm mov bx,adsr		// Load start adress
	asm mov dx,0x3d0	// color register (mono = 0x3b0
	asm add dx,4
	asm mov al,0x0d
	asm out dx,al
	asm inc dx
	asm mov al,bl
	asm out dx,al
	asm dec dx
	asm mov al,0x0c
	asm out dx,al
	asm inc dx
	asm mov al,bh
	asm out dx,al		// Listing 14.32
}

void DubImage(int off, unsigned char buf[], int nbyte, int nrow)
{
	asm push es
	asm mov ax,0xa000
	asm mov es,ax
	asm mov di,off
	asm lds si,buf
	asm xor dx,dx
	asm mov bx,320
	asm sub bx,nbyte
	asm sub bx,nbyte
	asm mov cx,nrow
	asm xor ch,ch
	asm mov ah,[bp+10]
    
loop1:
	asm mov dx,nbyte
loop2:
	asm mov al,[si]
	asm mov ah,al
	asm mov es:[di],ax
	asm inc di
	asm inc di
	asm inc si
	asm dec dx
	asm jg loop2
	asm add di,bx
	asm sub si,nbyte
	asm mov dx,nbyte
loop3:
	asm mov al,[si]
	asm mov ah,al
	asm mov es:[di],ax
	asm inc di
	asm inc di
	asm inc si
	asm dec dx
	asm jg loop3
	asm add di,bx
	asm loop loop1
	asm pop es
}
void SetPal(uchar col, uchar red, uchar gre, uchar blu)
{
	asm mov al,col		// load palette
	asm mov dx,PEL_write	// Ignite write mode
	asm out dx,al		// store it
	asm mov dx,PEL_data	// Initialize color data
	asm mov al,red		// red color
	asm out dx,al		// store it
	asm mov al,gre		// green color
	asm out dx,al		// store it
	asm mov al,blu		// blue color
	asm out dx,al		// and store it
}
void LoadR13(char *src)
{
	int shandle,dhandle;
	ulong bytes;
	unsigned char head[48];
	unsigned char *srcbuf;
	ulong size,size2;
	ulong dummy,x,y;
    
	if ((shandle = open(src, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1)
	{	printf("Error Opening Src-File\n"); exit(1);   }
	if ((bytes = read(shandle, head, 48)) == -1)
	{   printf("Source Read Failed.\n"); exit(1);   }
	close(shandle);
	x = (int)(head[0])+((int)(head[1])<<8);
	y =	(int)(head[2])+((int)(head[3])<<8);
	size=x*y+4;
	if ((shandle=open(src, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD)) == -1)
	{ printf("Error Opening Src-File\n");exit(1);}
	if ((bytes = read(shandle, picture, size)) == -1)
	{ printf("Source Picture read Failed.\n");exit(1); }
}

void CalcTab1()   // calculate table 1 for plasma
{
	long i=0,j=0;
	while(i<box_h*2)
	{
		j=0;
		while(j<box_w*2)
		{
			tab1[(i*box_w*2)+j]=(uchar) ( sqrt((box_h-i)*(box_h-i)+(box_w-j)*(box_w-j)) *5 );
			j++;
		}
		i++;
	}
}
void CalcTab2()   // calculate table 2 for plasma
{
	long i=0,j=0;
	float temp;
	while(i<box_h*2)
	{
		j=0;
		while(j<box_w*2)
		{
			temp=sqrt(16.0+(box_h-i)*(box_h-i)+(box_w-j)*(box_w-j))-4;
			tab2[(i*box_w*2)+j]=(sin(temp/9.5)+1)*90;
            //			tab2[(i*box_w*2)+j]=(sin(sqrt((box_h-i)*(box_h-i)+(box_w-j)*(box_w-j))/9.5)+1)*90;
			j++;
		}
		i++;
	}
}
void CalculateColors()
{
	static double r=1.0/6.0*pi,g=3.0/6.0*pi,b=5.0/6.0*pi;
	int i=0;
	double u,v;
	while(i<256)
	{
		u=2*pi/256*i;
        //#define mycol(u,a) (max(0.0,cos((u)+(a))))*63 //31
#define mycol(u,a) (cos((u)+(a))+1)*31
		SetPal(i,mycol(u,r),mycol(u,g),mycol(u,b));
		i++;
	}
	r+=0.05;
	g-=0.05;
	b+=0.1;
}
void CalculateBody(long x1,long y1,long x2,long y2,long x3,long y3,long x4,long y4,long roll,long picx,long picy)
{
	long i=0,j=0,k=0;
	char a;
	while(i<box_h)
	{
		j=0;
		k+=box_w;
		while(j<box_w)
		{
            // this is the heart of the plasma
			body[k+j]=
            tab1[320*(i+y1)+j+x1]+roll+
            tab2[320*(i+y2)+j+x2]+
            tab2[320*(i+y3)+j+x3]+
            tab2[320*(i+y4)+j+x4]+
            picture[320*(i+picy)+j+picx+4]
            ;
			j++;
		}
		i++;
	}
}
void UpdatePic(int *picx,int *picy,int *picc)
{
	if(*picc<100)
	{
		*picc+=1;
		return;
	}
	if(*picc<150)
	{
		*picc+=1;
		*picy+=2;
		return;
	}
	if(*picc<250)
	{
		*picc+=1;
		return;
	}
	if(*picc<330)
	{
		*picc+=1;
		*picx+=2;
		return;
	}
	if(*picc<430)
	{
		*picc+=1;
		return;
	}
	if(*picc<480)
	{
		*picc+=1;
		*picy-=2;
		return;
	}
	if(*picc<580)
	{
		*picc+=1;
		return;
	}
	if(*picc<660)
	{
		*picc+=1;
		*picx-=2;
		return;
	}
	*picc=0;
}
void DisplayObject(int x,int y)
{
	DubImage(x+y*320,&body[0],box_w,box_h);
}
int main(void)
{
	float circle1=0,circle2=0,circle3=0,circle4=0,circle5=0,circle6=0,circle7=0,circle8=0;
	int x1,y1,x2,y2,x3,y3,x4,y4,roll1=0;
	int bank=0,i=0;
	int picx=0,picy=0,picc=0;
	char oldmode;
    //	char *picture;
	LoadR13("text.r13");
	oldmode=GetMode();
	printf("\nSorry no music...\nPlease stand by, as CPU is performing heavy calculations.\n");
	CalcTab1();
	CalcTab2();
	OnVideo();
	SetMode(19);
	CalculateColors();
	while(!kbhit())
	{
		i++;
		circle1+=0.085/6;
		circle2-=0.1/6;
		circle3+=.3/6;
		circle4-=.2/6;
		circle5+=.4/6;
		circle6-=.15/6;
		circle7+=.35/6;
		circle8-=.05/6;
		x2=(box_w/2)+(box_w/2)*sin(circle1);
		y2=(box_h/2)+(box_h/2)*cos(circle2);
		x1=(box_w/2)+(box_w/2)*cos(circle3);
		y1=(box_h/2)+(box_h/2)*sin(circle4);
		x3=(box_w/2)+(box_w/2)*cos(circle5);
		y3=(box_h/2)+(box_h/2)*sin(circle6);
		x4=(box_w/2)+(box_w/2)*cos(circle7);
		y4=(box_h/2)+(box_h/2)*sin(circle8);
		CalculateBody(x1,y1,x2,y2,x3,y3,x4,y4,roll1+=5,picx,picy);
		UpdatePic(&picx,&picy,&picc);
		DisplayObject((320-box_w*2)/2,0);
        //		WaitRast();
		if (bank==0)
		{
			bank=1;
			SetBank(0,0);
			SetDisplayStart(16384);
			CalculateColors();
		}
		else
		{
			bank=0;
			SetBank(1,1);
			SetDisplayStart(0);
		}
	}
	OffVideo();
	SetMode(oldmode);
	return 0;
}