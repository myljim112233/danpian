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
uchar minute=59,hour=11,second=40; // ʱ���֣��붨��
uchar num=0;
uint count=0;			  //��ʱ��������,��ʱ50ms,count��20,���1
/***********�Ӻ�������*************************************/
void xianshishuzu(); //	��ʾ�����ӳ���					
/**********************��ʱ�ӳ���*****************************/  
void delay(uint z)
{   uint x,y;
	for(x=0;x<z;x++)
	for(y=0;y<110;y++);
}

/**********************��ʾʱ���ӳ���*************************/
 void dispaly(uchar w[8])
{  unsigned int  i,j,aa;			         
    aa=0xfe;              //λѡ��ֵ1111 1110
	for(i=0;i<6;i++)	     //���ν�����w�а˸���ȡ��������ʾ
	{	  
		j=w[i];		//ȡ��Ҫ��ʾ������
		P0=0;       //ȡ����ѡ����
		dula=1;          //λѡ
		dula=0;
		P0=aa;
	  wela=1;		     //λѡ
		wela=0;
		P0=tab[j];  	    //ȡ����ѡ����
		dula=1;		     
		dula=0;
		aa=_crol_(aa,1);   //λѡ�ź�ѭ������
		delay(10);		    //��ʾ��ʱ
		P0=0xff;         //��Ӱ
	}	 		
}

/***********************��ʾʱ�������ӳ���***********************/
void xianshishuzu()					
    {
      StrTab[4]=second/10;		      //���λ
      StrTab[5]=second%10;		  //��ʮλ
      StrTab[2]=minute/10;		      //�ָ�λ
      StrTab[3]=minute%10;		  //��ʮλ
      StrTab[0]=hour/10;		      //ʱ��λ
      StrTab[1]=hour%10;		      //ʱʮλ
	  	  
	}
/*******************�������ӳ���****************************/
void beng()
{
                bear=1;
		P3=0xfd;
		delay(100);
		bear=0;
		P3=0XFf;
		delay(100);
}
/*****************���㱨���ӳ���**************************/
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
		
/**************************�ж��ӳ���*********************************/
  void time_() interrupt 1                    //�жϳ���
   {
		count++;
	    TH0=(65536-50000)/256;		     //0.5ms�����ͳ�ֵ
	    TL0=(65536-50000)%256;	
		if(count==20)					     //��ʱ������,��ʱ50ms,count��20,���1
		 { second++; count=0;
		  if(second==60) 				     //��ֵ����60,������,�ּ�1
	        { second=0;minute++;
			if(minute==60)				 //��ֵ����60,������,ʱ��1
			  { minute=0; hour++;
			   if(hour==24)				 //ʱֵ����24,ʱ����,����,ȫ������
			      {hour=0;}
  }
			 }
		   }
		   xianshishuzu();
   }
/***********************������***************************/
 void main()
{ 
	P1=0XFF;
	TMOD = 0x11;             //time0Ϊ��ʱ��,��ʽ1
	TH0=(65536-50000)/256;	//Ԥ�ü�����ֵ,50ms	   
	TL0=(65536-50000)%256;
	EA=1;                  //���жϿ�
	ET0=1;                //����ʱ��0�ж�
	TR0=1;                //������ʱ��0
  
while(1)                  //��ѭ��
    {	       
     dispaly(StrTab); zhengdian (); 
		}
	}