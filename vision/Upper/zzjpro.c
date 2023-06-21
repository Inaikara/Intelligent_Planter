#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int
sbit relay=P2^0;     //继电器控制口
sbit in1=P1^0;       //抓手控制脉冲输入
sbit in2=P1^1;       //同上
sbit pulse_x=P1^2;   //x	dirver
sbit   dir_x=P1^3;   //同上
sbit pulse_y=P1^4;   //y	dirver
sbit   dir_y=P1^5;   //同上
sbit pulse_z=P1^6;   //z	dirver
sbit   dir_z=P1^7;   //同上
sbit dula=P2^6;      //数码管锁存端
sbit wela=P2^7;      //数码管锁存端
sbit trig=P2^4;		               
sbit echo=P2^5;		
bit  eflag=0;
unsigned int time;
unsigned long s;	 
uchar flag,a;
uchar code table_dula[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                    0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
uchar code table_wela[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf};
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
                   0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
void delayms(uint xms);//长延时
void delay(uint x);   //短延时
uchar matrixkeyscan();//按键控制
void pump();          //水泵控制
void grap();          //抓手控制
//串口通信初始化
void manipulator();   //ppp结构操作臂控制      
void smgdisplay(uchar num,uchar loc);
void start();
void count();
void display_loc();
void run_ultrasonic();
void Init_uart();
void Init_timer();
void Init();
void chuanxing();
void main()
{
  Init();
  while(1)
  {
//if(flag==0)
//{
//	manipulator();
//	pump();
//	grap(); 
//}
	smgdisplay(s,5);
	chuanxing();
  }
}
void delayms(uint xms)
{
  uint i,j;
  for(i=xms;i>0;i--)
    for(j=110;j>0;j--);
}
void delay(uint x)
{
  uint i,j;
  for(i=x;i>0;i--)
    for(j=10;j>0;j--);
}
void display(uchar num)
{

  dula=1;
  P0=table[num];
  dula=0;
  P0=0Xff;
  wela=1;
  P0=0xfd;
  wela=0;
}
uchar matrixkeyscan()
{
  uchar temp,key;  
  P3=0xfe;
  temp=P3;
  temp=temp&0xf0;
  if(temp!=0xf0)
  {
    delayms(10);
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      temp=P3;
      switch(temp)
      {
      case 0xee:
      //key=0;
      break;

      case 0xde:
      key=1;
      break;

      case 0xbe:
      //key=2;
      break;
 
      case 0x7e:
      //key=3;
	  key=7;
      break;
      }
      while(temp!=0xf0)
      {
       temp=P3;
       temp=temp&0xf0;
      }
      display(key);
    }
  }
  P3=0xfd;
  temp=P3;
  temp=temp&0xf0;
  if(temp!=0xf0)
  {
    delayms(10);
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      temp=P3;
      switch(temp)
      {
      case 0xed:
      //key=4;
	  key=3;
      break;

      case 0xdd:
      //key=5;
	  key=2;
      break;

      case 0xbd:
      //key=6;
	  key=4;
      break;

      case 0x7d:
      //key=7;
	  key=8;
      break;
      }
      while(temp!=0xf0)
      {
        temp=P3;
        temp=temp&0xf0;
      }
      display(key);
    }
  }
  P3=0xfb;
  temp=P3;
  temp=temp&0xf0;
  if(temp!=0xf0)
  {
    delayms(10);
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      temp=P3;
      switch(temp)
      {
      case 0xeb:
      //key=8;
      break;

      case 0xdb:
      //key=9;
	  key=5;
      break;

      case 0xbb:
      //key=10;
      break;

      case 0x7b:
      //key=11;
	  key=9;
      break;
      }
      while(temp!=0xf0)
      {
        temp=P3;
        temp=temp&0xf0;
      }
      display(key);
    }
  }
  P3=0xf7;
  temp=P3;
  temp=temp&0xf0;
  if(temp!=0xf0)
  {
    delayms(10);
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      temp=P3;
      switch(temp)
      {
        case 0xe7:
        //key=12;
        break;

        case 0xd7:
        //key=13;
		key=6;
        break;

        case 0xb7:
        //key=14;
        break;

        case 0x77:
        //key=15;
		key=10;
        break;
      }
      while(temp!=0xf0)
      {
        temp=P3;
        temp=temp&0xf0;
      }
      display(key);
    }
  }
  return key;
}
void pump()
{
 if(matrixkeyscan()==10)
   {
    delayms(1);           
    if(matrixkeyscan()==10)              
     {
      relay=0;
 	 }
   }
 if(matrixkeyscan()!=10)
   {
    delayms(1);
    if(matrixkeyscan()!=10)
	{
     relay=1;
	}
   }
}
void grap()
{
  if (matrixkeyscan()==7)
  {
   delayms(10);
   if (matrixkeyscan()==7)
   {
   in1=1,in2=0;
   }
  }
  if (matrixkeyscan()==8)
  {
   delayms(10);
   if (matrixkeyscan()==8)
   {
   in1=0,in2=1;
   }
  }
  if (matrixkeyscan()==9)
  {
   delayms(10);
   if (matrixkeyscan()==9)
   {
   in1=0,in2=0;
   }
  }
}

void manipulator()
{
  if(matrixkeyscan()==1)//x轴正转
  {
    delayms(1);
    if(matrixkeyscan()==1)
    {
      dir_x = 1;
  	  while(matrixkeyscan()==1)
      {
        delay(1);
        pulse_x=0;
        delay(1);
        pulse_x=1;
        delay(1);        
      }
    }
  }
  if(matrixkeyscan()==2)//x轴反转
  {
    delayms(1);
    if(matrixkeyscan()==2)
    {
      dir_x = 0;
      while(matrixkeyscan()==2)
      {
        delay(1);
        pulse_x=0;
        delay(1);
        pulse_x=1;
        delay(1);        
      }
    } 
  }
  if(matrixkeyscan()==3)//y轴正转
  {
	delayms(1);
	if(matrixkeyscan()==3)
	{
      dir_y = 1;
	  while(matrixkeyscan()==3)
      {
        delay(1);
        pulse_y=0;
        delay(1);
        pulse_y=1;
        delay(1);        
      }
    }
  }
  if(matrixkeyscan()==4)//y轴反转
  {
	delayms(1);
	if(matrixkeyscan()==4)
	{
      dir_y = 0;
	  while(matrixkeyscan()==4)
      {
        delay(1);
        pulse_y=0;
        delay(1);
        pulse_y=1;
        delay(1);        
      }
    }
  }
  if(matrixkeyscan()==5)//z轴正转
  {
	delayms(1);
	if(matrixkeyscan()==5)
	{
      dir_z = 1;
	  while(matrixkeyscan()==5)
      {
        delay(1);
        pulse_z=0;
        delay(1);
        pulse_z=1;
        delay(1);        
      }
    }
  }
  if(matrixkeyscan()==6)//z轴反转
  {
	delayms(1);
	if(matrixkeyscan()==6)
	{
      dir_z = 0;
	  while(matrixkeyscan()==6)
      {
        delay(1);
        pulse_z=0;
        delay(1);
        pulse_z=1;
        delay(1);        
      }
    }
  }
}
void chuanxing()
{
  if(flag==1)
  {
	uchar temp=0;
    RI=0;
    switch(a&0x0f)
	{
	  case 0: 
			 
			 dir_z=0;		 
		        while(s>8)
	            {
					if(temp>=100)
					{
						run_ultrasonic();
						temp=0;
					}
				temp=temp+1;
		         delay(1);
               pulse_z=0;
               delay(1);
               pulse_z=1;
               delay(1); 
  
		          }
		   switch(a&0xf0)
			 {
					case 16 :
	        in1=1,in2=0;
		      delayms(1000);
		      in1=0,in2=0;
					break;		
				  case 32 : 
           relay=0;	
           delayms(500);	
           relay=1;	
		break;				
			 }			
			 dir_z=1;
			while(s<15)
				{
				if(temp>=100)
				{
					run_ultrasonic();
					temp=0;
				}
				temp=temp+1;
			    delay(1);
               pulse_z=0;
               delay(1);
               pulse_z=1;
               delay(1);  
               }								 
	  break;								
	  case 1:                    //00000001B:
	  dir_x = 0;
      while(!RI)
      {
        delay(1);
        pulse_x=0;
        delay(1);
        pulse_x=1;
        delay(1);        
      }
	  break;
	  case 2:                    //00000010B:
	  dir_x = 1;
      while(!RI)
      {
        delay(1);
        pulse_x= 0;
        delay(1);
        pulse_x= 1;
        delay(1);        
      }
	  break;
      case 4:                    //00000100B:
	  dir_y = 0;
      while(!RI)
      {
        delay(1);
        pulse_y= 0;
        delay(1);
        pulse_y= 1;
        delay(1);        
      }
	  break;
	  case 5:                    //00000101B:
      dir_x = 0;
	  dir_y = 0;
      while(!RI)
      {
        delay(1);
        pulse_x= 0;
        delay(1);
        pulse_x= 1;
        delay(1);  
        pulse_y= 0;
        delay(1);
        pulse_y= 1;
        delay(1);  
      }
	  break;
	  case 6:                      //00000110B:
	  dir_x = 1;
	  dir_y = 0;
      while(!RI)
      {
        delay(1);
        pulse_x= 0;
        delay(1);
        pulse_x= 1;
        delay(1);  
        pulse_y= 0;
        delay(1);
        pulse_y= 1;
        delay(1);  
      }
 	  break;
	  case 8:                       //00001000B:
	  dir_y = 1;
      while(!RI)
      {
        delay(1);
        pulse_y= 0;
        delay(1);
        pulse_y= 1;
        delay(1);        
      }
	  break;
	  case 11:                        //00001001B:
	  dir_x = 0;
	  dir_y = 1;
	  while(!RI)
      {
        delay(1);
        pulse_x= 0;
        delay(1);
        pulse_x= 1;
        delay(1);  
        pulse_y= 0;
        delay(1);
        pulse_y= 1;
        delay(1);  
      }
	  break;
	  case 12:                       //00001010B:
	  dir_x = 1;
	  dir_y = 1;
	  while(!RI)
      {
        delay(1);
        pulse_x= 0;
        delay(1);
        pulse_x= 1;
        delay(1);  
        pulse_y= 0;
        delay(1);
        pulse_y= 1;
        delay(1);  			
      }		
	  break;
      case 15:                       //00001111B:		
      break;
    }				 
			flag=0;
  }
}				
//		if(flag==1)
//		{
//			ES=0;
//			for(i=0;i<6;i++)
//			{
//				SBUF=table[i];
//				while(!TI);
//				TI=0;
//			}
//			SBUF=a;
//			while(!TI);
//			TI=0;
//			ES=1;
//			flag=0;
//		}
void smgdisplay(uchar num,uchar loc)
{
			dula=1;
			P0=table_dula[num];
			dula=0;
			P0=0xff;	
			wela=1;
			P0=table_wela[loc];
			wela=0;
}

void start()                            //???????
{   
    trig=1;			                    
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    trig=0;			             
}

void count()                            //????
{   time=TH0*256+TL0;				
    s=time*1.7/100;				    		
    TH0=0x00;						
    TL0=0x00;	
}

void run_ultrasonic()
{
		start();
    while(echo==0);
		TR0=1;			   
    while((echo==1)||(eflag==1));	
	  TR0=0;	
    eflag==0;	
    count();			   
   // delayms(50);
   ES=0;
		SBUF=s;
    while(TI==0);           //???TI????1
    TI = 0;                 //TI??0,???????	
	ES=1;
}


void Init_uart()                             //??
{   
    TMOD |= 0x20;                       //?????1  
    TH1 = 0xFD;                         //???9600    
    TL1 = 0xFD;
    TR1 = 1;                            //?????
    REN = 1;                            //??????
    SM0 = 0;                            //???????1
    SM1 = 1;                            
    ES = 1;                             //??????
}

void Init_timer()
{
		TMOD=0x01;			   
    TH0=0x00;			   
    TL0=0x00;			   
    EA=1;			       
    ET0=1;	
}

void Init()
{
	Init_timer();
	Init_uart();
}
void ser() interrupt 4
{
  a=SBUF;
  flag=1;
}
void InterruptTimer0() interrupt 1			  
{
	eflag=1;
}