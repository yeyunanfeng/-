#include <reg52.h> 
#include <string.h>	
#include "stdio.h"
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char   	//�궨��
#define LCD1602 P0
sbit SET=P2^5;    			//���������
sbit DEC=P3^2;    			//������ټ�
sbit ADD=P3^3;    			//�������Ӽ�
sbit BUZZ=P2^4;    			//���������
sbit ALAM=P1^2;				//����ƹⱨ��
sbit ALAM1=P1^4;
sbit DQ=P2^0;     			//�����ĸ�DS18B20�¶ȴ�����	
sbit DQ1=P2^1;
sbit DQ2=P2^2;
sbit DQ3=P2^3;
sbit RS = P2^7;
sbit EN = P2^6;
sbit led1 = P3^4;                                //�����ĸ�LED��
sbit led2 = P3^5;
sbit led3 = P3^6;
sbit led4 = P3^7;
sbit KEY1 = P1^1;  
sbit send = P1^0;
bit shanshuo_st;    			//��˸�����־
bit beep_st;     				//�����������־
uchar x=0;      				//������
char Time[] = __TIME__;
void UsartInit();
void SendStr(char * str);

uchar code tab1[]={"T:  .  C    .  C"};
uchar code tab2[]={"  .  C      .  C"};
uchar code tab3[]={"TH:   C  TL:   C"};
uint c,c1,c2,c3,c4;
uchar Mode=0;     			//״̬��־
signed char TH=40;  		//���ޱ����¶ȣ�Ĭ��ֵΪ40
signed char TL=10;   		//���ޱ����¶ȣ�Ĭ��ֵΪ10
//============================================================================================
//====================================DS18B20=================================================
//============================================================================================
/*****��ʱ�ӳ���*****/
void Delay_DS18B20(int num)
{
  while(num--) ;
}
void delay(uint xms)//��ʱ�������вκ���
{
	uint x,y;
	for(x=xms;x>0;x--)
	 for(y=110;y>0;y--);
}
/*****��ʼ��DS18B20 �¶ȴ�����1*****/
void Init_DS18B20(void)
{
  unsigned char x=0;
  DQ = 1;         //DQ��λ
  Delay_DS18B20(8);    //������ʱ
  DQ = 0;         //��Ƭ����DQ����
  Delay_DS18B20(80);   //��ȷ��ʱ������480us
  DQ = 1;         //��������
  Delay_DS18B20(14);
  x = DQ;           //������ʱ�����x=0���ʼ���ɹ���x=1���ʼ��ʧ��
  Delay_DS18B20(20);
}

/*****��һ���ֽ�*****/
unsigned char ReadOneChar(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ = 0;     // �������ź�
    dat>>=1;
    DQ = 1;     // �������ź�
    if(DQ)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return(dat);
}
/*****дһ���ֽ�*****/
void WriteOneChar(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ = 0;
    DQ = dat&0x01;
    Delay_DS18B20(5);
    DQ = 1;
    dat>>=1;
  }
}
/*****��ȡ�¶�*****/
unsigned int ReadTemperature(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init_DS18B20();
  WriteOneChar(0xCC);  //����������кŵĲ���
  WriteOneChar(0x44);  //�����¶�ת��
  Init_DS18B20();
  WriteOneChar(0xCC);  //����������кŵĲ���
  WriteOneChar(0xBE);  //��ȡ�¶ȼĴ���
  a=ReadOneChar();     //����8λ
  b=ReadOneChar();    //����8λ
  t=b;
  t<<=8;
  t=t|a;
  tt=t*0.0625;
 // t= tt*10+0.5;     //�Ŵ�10���������������
  t= tt*10+0.5; 
  return(t);
}


/*****��ʼ��DS18B20  �¶ȴ�����2*****/
void Init1_DS18B20(void)
{
  unsigned char x=0;
  DQ1 = 1;         //DQ��λ
  Delay_DS18B20(8);    //������ʱ
  DQ1 = 0;         //��Ƭ����DQ����
  Delay_DS18B20(80);   //��ȷ��ʱ������480us
  DQ1 = 1;         //��������
  Delay_DS18B20(14);
  x = DQ1;           //������ʱ�����x=0���ʼ���ɹ���x=1���ʼ��ʧ��
  Delay_DS18B20(20);
}

unsigned char ReadOneChar1(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ1 = 0;     // �������ź�
    dat>>=1;
    DQ1 = 1;     // �������ź�
    if(DQ1)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return(dat);
}
void WriteOneChar1(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ1 = 0;
    DQ1 = dat&0x01;
    Delay_DS18B20(5);
    DQ1 = 1;
    dat>>=1;
  }
}

/*****��ȡ�¶�*****/
unsigned int ReadTemperature1(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init1_DS18B20();
  WriteOneChar1(0xCC);  //����������кŵĲ���
  WriteOneChar1(0x44);  //�����¶�ת��
  Init1_DS18B20();
  WriteOneChar1(0xCC);  //����������кŵĲ���
  WriteOneChar1(0xBE);  //��ȡ�¶ȼĴ���
  a=ReadOneChar1();     //����8λ
  b=ReadOneChar1();    //����8λ
  t=b;
  t<<=8;
  t=t|a;
  tt=t*0.0625;
 // t= tt*10+0.5;     //�Ŵ�10���������������
  t= tt*10+0.5; 
  return(t);
}


/*****��ʼ��DS18B20  �¶ȴ�����3*****/
void Init2_DS18B20(void)
{
  unsigned char x=0;
  DQ2 = 1;         //DQ��λ
  Delay_DS18B20(8);    //������ʱ
  DQ2 = 0;         //��Ƭ����DQ����
  Delay_DS18B20(80);   //��ȷ��ʱ������480us
  DQ2 = 1;         //��������
  Delay_DS18B20(14);
  x = DQ2;           //������ʱ�����x=0���ʼ���ɹ���x=1���ʼ��ʧ��
  Delay_DS18B20(20);
}

unsigned char ReadOneChar2(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ2 = 0;     // �������ź�
    dat>>=1;
    DQ2 = 1;     // �������ź�
    if(DQ2)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return(dat);
}
void WriteOneChar2(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ2 = 0;
    DQ2 = dat&0x01;
    Delay_DS18B20(5);
    DQ2 = 1;
    dat>>=1;
  }
}

/*****��ȡ�¶�*****/
unsigned int ReadTemperature2(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init2_DS18B20();
  WriteOneChar2(0xCC);  //����������кŵĲ���
  WriteOneChar2(0x44);  //�����¶�ת��
  Init2_DS18B20();
  WriteOneChar2(0xCC);  //����������кŵĲ���
  WriteOneChar2(0xBE);  //��ȡ�¶ȼĴ���
  a=ReadOneChar2();     //����8λ
  b=ReadOneChar2();    //����8λ
  t=b;
  t<<=8;
  t=t|a;
  tt=t*0.0625;
 // t= tt*10+0.5;     //�Ŵ�10���������������
  t= tt*10+0.5; 
  return(t);
}

/*****��ʼ��DS18B20  �¶ȴ�����4*****/
void Init3_DS18B20(void)
{
  unsigned char x=0;
  DQ3	= 1;         //DQ��λ
  Delay_DS18B20(8);    //������ʱ
  DQ3 = 0;         //��Ƭ����DQ����
  Delay_DS18B20(80);   //��ȷ��ʱ������480us
  DQ3 = 1;         //��������
  Delay_DS18B20(14);
  x = DQ3;           //������ʱ�����x=0���ʼ���ɹ���x=1���ʼ��ʧ��
  Delay_DS18B20(20);
}

unsigned char ReadOneChar3(void)
{
  unsigned char i=0;
  unsigned char dat = 0;
  for (i=8;i>0;i--)
  {
    DQ3 = 0;     // �������ź�
    dat>>=1;
    DQ3 = 1;     // �������ź�
    if(DQ3)
    dat|=0x80;
    Delay_DS18B20(4);
  }
  return(dat);
}
void WriteOneChar3(unsigned char dat)
{
  unsigned char i=0;
  for (i=8; i>0; i--)
  {
    DQ3 = 0;
    DQ3 = dat&0x01;
    Delay_DS18B20(5);
    DQ3 = 1;
    dat>>=1;
  }
}

/*****��ȡ�¶�*****/
unsigned int ReadTemperature3(void)
{
  unsigned char a=0;
  unsigned char b=0;
  unsigned int t=0;
  float tt=0;
  Init3_DS18B20();
  WriteOneChar3(0xCC);  //����������кŵĲ���
  WriteOneChar3(0x44);  //�����¶�ת��
  Init3_DS18B20();
  WriteOneChar3(0xCC);  //����������кŵĲ���
  WriteOneChar3(0xBE);  //��ȡ�¶ȼĴ���
  a=ReadOneChar3();     //����8λ
  b=ReadOneChar3();    //����8λ
  t=b;
  t<<=8;
  t=t|a;
  tt=t*0.0625;
 // t= tt*10+0.5;     //�Ŵ�10���������������
  t= tt*10+0.5; 
  return(t);
}

/*****��ȡ�¶�*****/
void check_wendu(void)
{
	c=ReadTemperature()-5;  			//��ȡ�¶�ֵ����ȥDS18B20����Ư���
	if(c>1200)
	c=1200;
	c1=ReadTemperature1()-5;  			//��ȡ�¶�ֵ����ȥDS18B20����Ư���
	if(c1>1200)
	c1=1200;
	c2=ReadTemperature2()-5;  			//��ȡ�¶�ֵ����ȥDS18B20����Ư���
	if(c2>1200)
	c2=1200;
	c3=ReadTemperature3()-5;  			//��ȡ�¶�ֵ����ȥDS18B20����Ư���
	if(c3>1200)
	c3=1200;
	c4=(c1+c2+c+c3)/4;
}



/********Һ��д��ָ�����д�����ݺ������Ժ�ɵ���**************/

void write_1602com(uchar com)//****Һ��д��ָ���****
{
	RS=0;//����/ָ��ѡ����Ϊָ��
//	rw=0; //��дѡ����Ϊд
	LCD1602=com;//��������
	delay(1);
	EN=1;//����ʹ�ܶˣ�Ϊ������Ч���½�����׼��
	delay(1);
	EN=0;//en�ɸ߱�ͣ������½��أ�Һ��ִ������
}


void write_1602dat(uchar dat)//***Һ��д�����ݺ���****
{
	RS=1;//����/ָ��ѡ����Ϊ����
//	rw=0; //��дѡ����Ϊд
	LCD1602=dat;//��������
	delay(1);
	EN=1; //en�øߵ�ƽ��Ϊ�����½�����׼��
	delay(1);
	EN=0; //en�ɸ߱�ͣ������½��أ�Һ��ִ������
}


void lcd_init()//***Һ����ʼ������****
{
	uchar a;
	write_1602com(0x38);//����Һ������ģʽ����˼��16*2����ʾ��5*7����8λ����
	write_1602com(0x0c);//����ʾ����ʾ���
	write_1602com(0x06);//�������ƶ�������Զ�����
	write_1602com(0x01);//����ʾ

	write_1602com(0x80);//������ʾ�̶����Ŵӵ�һ�е�1��λ��֮��ʼ��ʾ
	for(a=0;a<16;a++)
	{
		write_1602dat(tab1[a]);//��Һ����д������ʾ�Ĺ̶����Ų���
		delay(3);
	}
	write_1602com(0x80+0x40);//ʱ����ʾ�̶�����д��λ�ã��ӵ�2��λ�ú�ʼ��ʾ
	for(a=0;a<16;a++)
	{
		write_1602dat(tab2[a]);//д��ʾʱ��̶����ţ�����ð��
		delay(3);
	}

}

void display()//�¶�ֵ��ʾ
{
	if(Mode==0)
	{
		write_1602com(0x80+2);
		write_1602dat((c%1000)/100+0x30);
		write_1602dat(((c%1000)%100)/10+0x30);
		write_1602com(0x80+5);
		write_1602dat(((c%1000)%100)%10+0x30);
		write_1602com(0x80+6);
		write_1602dat(0xdf);
		write_1602com(0x80+10);
		write_1602dat((c1%1000)/100+0x30);
		write_1602dat(((c1%1000)%100)/10+0x30);
		write_1602com(0x80+13);
		write_1602dat(((c1%1000)%100)%10+0x30);
		write_1602com(0x80+14);
	  write_1602dat(0xdf);
		write_1602com(0x80+0x40);
		write_1602dat((c2%1000)/100+0x30);
		write_1602dat(((c2%1000)%100)/10+0x30);
		write_1602com(0x80+0x40+3);
		write_1602dat(((c2%1000)%100)%10+0x30);
		write_1602com(0x80+0x40+4);
	  write_1602dat(0xdf);
		write_1602com(0x80+0x40+10);
		write_1602dat((c3%1000)/100+0x30);
		write_1602dat(((c3%1000)%100)/10+0x30);
		write_1602com(0x80+0x40+13);
		write_1602dat(((c3%1000)%100)%10+0x30);
		write_1602com(0x80+0x40+14);
	  write_1602dat(0xdf);	
	}
}
//=====================================================================================

/*****��ʼ����ʱ��0*****/
void InitTimer(void)
{
	TMOD=0x1;
	TH0=0x3c;
	TL0=0xb0;     //50ms������12M��
	EA=1;      //ȫ���жϿ���
	TR0=1;
	ET0=1;      //������ʱ��0
}

void KEY()//����ɨ��
{
	uchar a;
			//���ܼ�
	if(SET==0)
	{	
		write_1602com(0x01);
		write_1602com(0x80+0x40);//ʱ����ʾ�̶�����д��λ�ã��ӵ�2��λ�ú�ʼ��ʾ
		for(a=0;a<16;a++)
		{
			write_1602dat(tab3[a]);//д��ʾʱ��̶����ţ�����ð��
			delay(3);
		}	
		BUZZ=0;
		delay(10);
		if(SET==0)
		{
			Mode++;
			if(Mode==3)
			Mode=0;	
			BUZZ=1;
			write_1602com(0x80+0x40+3);
			write_1602dat(TH/10+0x30);
			write_1602dat(TH%10+0x30);
			write_1602dat(0xdf);
			write_1602com(0x80+0x40+12);
			write_1602dat(TL/10+0x30);
			write_1602dat(TL%10+0x30);
			write_1602dat(0xdf);
		}
		while(SET==0)
		{
			if(Mode==0)
				{
				//	write_1602com(0x80+0x40+6);
					write_1602com(0x0c);
					lcd_init();										// Һ����ʾΪ�������
			
				}	
			else if(Mode==1)
				{
					write_1602com(0x80+0x40+3);
					write_1602com(0x0f);
				}	
			else
				{
					write_1602com(0x80+0x40+12);
					write_1602com(0x0f);
				}							
		}
	}
	//����
	if(ADD==0&&Mode==1)
	{
		BUZZ=0;
		delay(10);
		if(ADD==0)	
		{
			TH++;
			if(TH>=99)	
			TH=99;
			write_1602com(0x80+0x40+3);
			write_1602dat(TH/10+0x30);
			write_1602dat(TH%10+0x30);
			write_1602dat(0xdf);
			BUZZ=1;
		}
		while(ADD==0);
		
	}
	//����
	if(DEC==0&&Mode==1)
	{
		BUZZ=0;
		delay(10);
		if(DEC==0)
		{
			TH--;
			if(TH==TL)	
			TH=TL+1;
			write_1602com(0x80+0x40+3);
			write_1602dat(TH/10+0x30);
			write_1602dat(TH%10+0x30);
			write_1602dat(0xdf);
			BUZZ=1;
		}
		while(DEC==0);
	}
	if(ADD==0&&Mode==2)
	{
		BUZZ=0;
		delay(10);
		if(ADD==0)	
		{
			TL++;
			if(TL==TH)	
			TL=TH-1;
			BUZZ=1;
			write_1602com(0x80+0x40+12);
			write_1602dat(TL/10+0x30);
			write_1602dat(TL%10+0x30);
			write_1602dat(0xdf);
		}	
		while(ADD==0);
		
	}
	//����
	if(DEC==0&&Mode==2)
	{
		BUZZ=0;
		delay(10);
		if(DEC==0)
		{
			TL--;
			if(TL<=0)	
			TL=0;	
			write_1602com(0x80+0x40+12);
			write_1602dat(TL/10+0x30);
			write_1602dat(TL%10+0x30);
			write_1602dat(0xdf);
			BUZZ=1;
		}
		while(DEC==0);		
	}
}

/*****�����ӳ���*****/
void Alarm()
{
	if(x>=10){beep_st=~beep_st;x=0;}
	if(Mode==0)
	{	
		led1=0;
		led2=0;
		led3=0;
		led4=0;
		if((c/10)>=TH)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led1=1;
			SendStr("���棡�����¶�1�쳣 \r\n");
		}
		if((c1/10)>=TH)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led2=1;
			SendStr("���棡�����¶�2�쳣 \r\n");
		}
		if((c2/10)>=TH)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led3=1;
			SendStr("���棡�����¶�3�쳣 \r\n");
		}
		if((c3/10)>=TH)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led4=1;
			SendStr("���棡�����¶�4�쳣 \r\n");
		}

		if((c/10)<=TL)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led1=1;
			SendStr("���棡�����¶�1�쳣 \r\n");
		}
		if((c1/10)<=TL)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led2=1;
			SendStr("���棡�����¶�2�쳣 \r\n");
		}
		if((c2/10)<=TL)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led3=1;
			SendStr("���棡�����¶�3�쳣 \r\n");
		}
		if((c3/10)<=TL)
		{
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			led4=1;
			SendStr("���棡�����¶�4�쳣 \r\n");
		}
		
		if((c4/10)>=TH)
		{
			ALAM=0;
			ALAM1=1;
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			SendStr("���棡�����¶��쳣 \r\n");
		}
		else if((c4/10)<TL)
		{
			ALAM1=0;
			ALAM=1;
			if(beep_st==1)
			BUZZ=1;
			else
			BUZZ=0;
			SendStr("���棡�����¶��쳣 \r\n");
		}
		else
		{
			BUZZ=1;
			ALAM=1;
			ALAM1=1;	
		}
	}
	else
	{
		BUZZ=1;
		ALAM=1;
		ALAM1=1;
	}
}

/*****ʱ�亯��*****/
void time(char *time){
	if(time[3]<'6'){
		if(time[4]<'9'){
			if(time[6]<'6'){
				if(time[7]<'9')
					time[7]=time[7]+1;
				else{
					time[7]='0';
					time[6]=time[6]+1;
				}
			}
				else{
					time[6]='0';
					time[4]=time[4]+1;
				}
			}
		else{
			time[4]='0';
			time[3]=time[3]+1;
		}
	}
	else{
		time[3]='0';
		time[2]=time[2]+1;
	}
					
}

/*****������Ϣ����*****/
void BasicInfo(void){
			char string[50];
			sprintf(string,"ʱ��:%s %s \r\n",__DATE__,Time);
			SendStr(string);  //���ڷ���
			sprintf(string,"�¶�1:%2.1f��C \r\n",c*1.0/10);
			SendStr(string);  //���ڷ���
			sprintf(string,"�¶�2:%2.1f��C \r\n",c1*1.0/10);
			SendStr(string);  //���ڷ���
			sprintf(string,"�¶�3:%2.1f��C \r\n",c2*1.0/10);
			SendStr(string);  //���ڷ���
			sprintf(string,"�¶�4:%2.1f��C \r\n",c3*1.0/10);
			SendStr(string);  //���ڷ���
			sprintf(string,"ƽ���¶�:%2.1f��C \r\n",c4*1.0/10);
			SendStr(string);  //���ڷ���
}

/*****������*****/
void main(void)
{
	uint z;
	delay(1);
	lcd_init();
	delay(1);
	InitTimer();    //��ʼ����ʱ��
	
	for(z=0;z<100;z++)
	{
		check_wendu();
		delay(1);        
	} 
	UsartInit();
	while(1)
	{
		display();
		KEY();
		Alarm(); 
		check_wendu();
		time(Time);
		if(send==0&&Time[3]!='6'&&Time[6]!='6')
		{
			BasicInfo();
		}
		if(KEY1==0)
		{
			delay(10);
			if(KEY1==0)
			{
				BUZZ=0;
			}
		}
		else
		{
			BUZZ=1;
		}
	}
}

void UsartInit( void ) //�����ж�
{
	SCON = 0X50;		//����Ϊ������ʽ1
	TMOD = 0X20;		//���ü�����������ʽ2
	PCON &= 0XEF;		//�����ʼӱ�
	TH1  = 0XFD;		//��������ʼֵ���ã�ע�Ⲩ������9600��
	TL1  = 0XFD;
	
	IE = 0X00;
	TR1  = 1;			//�򿪼�����
}

/*******************************************************************************
* �� �� ��		: SendStr
* ��������		: �����ַ���
* ��	��			: �ַ���
* ��	��			: ��
*******************************************************************************/
void SendStr(char * str)//���ڷ��ͺ���
{
	
	while(*str != '\0')
	{
		SBUF = *str;	//����һ���ַ�
		while(!TI);		//�ȴ����ڷ�����ɣ�������ɺ� TI �Զ���1
		TI = 0;				//TI ��0
		str++;				//ָ���һ
	}
}