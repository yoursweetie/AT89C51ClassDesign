//模拟东西方向的十字路口交通信号控制情况。东西向通行时间为80s，南北向通行时间为60s，缓冲时间为3s。
//P1接各个方向的灯，P0接代表十位的七位数码管，P2接代表个位的七位数码管
#include <reg51.h>
sbit g1 = P1^0; 	//位定义, P1口控制红绿黄灯的亮灭
sbit r1 = P1^1; 
sbit y1 = P1^2; 
sbit g2 = P1^3; 
sbit r2 = P1^4; 
sbit y2 = P1^5; 
unsigned char f = 0;
unsigned char nanbei_time = 80;	 //定义南北的时间长度
unsigned char dongxi_time = 60;    //定义东西的时间长度
unsigned char m;    //m是每个方向可通行的时间 
unsigned char code  t[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; 

void init_timer0(void) 	   //中断初始化函数
{
    TMOD = 0x01;  /*定时器工作方式寄存器，工作在方式1*/ 
    TH0 = (65536 - 50000) / 256;  
    TL0 = (65536 - 50000) % 256;  
    EA = 1;   /*中断允许寄存器IE(8位)中的中断总控制位，EA=1时，CPU开放所有中断*/  
    ET0 = 1;  /*IE寄存器中的一位,定时/计数器T0中断控制位，ET0=1时,允许T0中断*/  
    TR0 = 1;  /*TR0：T0运行控制位，TR0=0,启动定时器T0*/ 
} 

void timer(void) interrupt 1  //中断服务函数
{
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    f++;
    
    if (f == 20)
    {
        f = 0;
        m--;
    }
}

void display(unsigned char time)    //控制两个数码管的显示倒计时函数
{
    unsigned char shiwei;    //十位
    unsigned char gewei;     //个位

    shiwei = time / 10;
    gewei = time % 10;

    P0 = t[shiwei];
    P2 = t[gewei];
}

void main()    //主函数
{
    m = nanbei_time;
    P1 = 0x00;    //所有灯熄灭
    init_timer0();
    
    while (1) 
    {
        do
        {
	    display(m);
   	    g1 = 1; 
            r1 = 0;
            g2 = 0;
            r2 = 1;
        }
        while (m != 3);

        do
        {        
            if (m <= 3)
            {
                y1 = 1;    //取反，y1输出0变为1，黄灯亮起
                g1 = 0;
                r1 = 0;
                g2 = 0;
                r2 = 1;
            }
            display(m);    
        }
        while (m != 0);

	if (m == 0)
        {
            m = dongxi_time;
	    y1 = 0;    
	    y2 = 0;    //黄灯熄灭
        }

	do
        {
	    display(m);
	    g1 = 0;
	    r1 = 1;
	    g2 = 1;
	    r2 = 0;
	}
	while (m != 3);

	do
        {
	    if(m <= 3)
            {   
		y2 = ~y2;
                r1 = 1;
		g1 = 0; 
		r2 = 0;
		g2 = 0;
             }
             display(m);
	}
        while (m != 0);

        if (m == 0)
        {
            m = nanbei_time;
            y1 = 0;
            y2 = 0;
        }
    }
}
