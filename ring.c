#include <reg51.h>
#include<absacc.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit dula=P1^6;
sbit wela=P1^7;
sbit bear=P2^3;
code unsigned char tab[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71}; 
char code weikong_code[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar ms[8]={2,2,10,4,0,0,1,14};
uchar StrTab[8];
uchar minute=59,hour=11,second=40; // 时，分，秒定义
uchar num=0;
uint count=0;			  //定时器技计数,定时50ms,count满20,秒加1
/***********子函数声明*************************************/
void xianshishuzu(); //	显示数组子程序					
/**********************延时子程序*****************************/  
void delay(uint z)
{   uint x,y;
	for(x=0;x<z;x++)
	for(y=0;y<110;y++);
}

/**********************显示时钟子程序*************************/
 void dispaly(uchar w[8])
{  unsigned int  i,j,aa;			         
    aa=0xfe;              //位选初值1111 1110
	for(i=0;i<6;i++)	     //依次将数组w中八个数取出，并显示
	{	  
		j=w[i];		//取出要显示的数码
		P0=0;       //取出段选编码
		dula=1;          //位选
		dula=0;
		P0=aa;
	  wela=1;		     //位选
		wela=0;
		P0=tab[j];  	    //取出段选编码
		dula=1;		     
		dula=0;
		aa=_crol_(aa,1);   //位选信号循环右移
		delay(10);		    //显示延时
		P0=0xff;         //消影
	}	 		
}

/***********************显示时钟数组子程序***********************/
void xianshishuzu()					
    {
      StrTab[4]=second/10;		      //秒个位
      StrTab[5]=second%10;		  //秒十位
      StrTab[2]=minute/10;		      //分个位
      StrTab[3]=minute%10;		  //分十位
      StrTab[0]=hour/10;		      //时个位
      StrTab[1]=hour%10;		      //时十位
	  	  
	}
/*******************蜂鸣器子程序****************************/
void beng()
{
                bear=1;
		P3=0xfd;
		delay(100);
		bear=0;
		P3=0XFf;
		delay(100);
}
/*****************整点报警子程序**************************/
	void zhengdian (void)
	{		
		uchar i=0;
		if((second==0)&(minute==0))
		{ 
		for(i=0;i<10;i++)
	 	  {
		 TR0=1; beng();dispaly(ms);
		  }
		 } 
	}
		
/**************************中断子程序*********************************/
  void time_() interrupt 1                    //中断程序
   {
		count++;
	    TH0=(65536-50000)/256;		     //0.5ms重新送初值
	    TL0=(65536-50000)%256;	
		if(count==20)					     //定时器计数,定时50ms,count满20,秒加1
		 { second++; count=0;
		  if(second==60) 				     //秒值等于60,秒清零,分加1
	        { second=0;minute++;
			if(minute==60)				 //分值等于60,分清零,时加1
			  { minute=0; hour++;
			   if(hour==24)				 //时值等于24,时清零,返回,全部清零
			      {hour=0;}
  }
			 }
		   }
		   xianshishuzu();
   }
/***********************主函数***************************/
 void main()
{ 
	P1=0XFF;
	TMOD = 0x11;             //time0为定时器,方式1
	TH0=(65536-50000)/256;	//预置计数初值,50ms	   
	TL0=(65536-50000)%256;
	EA=1;                  //总中断开
	ET0=1;                //允许定时器0中断
	TR0=1;                //开启定时器0
  
while(1)                  //主循环
    {	       
     dispaly(StrTab); zhengdian (); 
		}
	}