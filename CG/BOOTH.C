#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<stdlib.h>
#include<string.h>
#define w 30
#define l 10
#define o 8
#define dt 2
#define n 5
#define dig1col RED
#define dig0col GREEN
#define blkcol WHITE
#define bgcol BLACK
#define tc 8000
//tc is time constant for the delay

int acc[n]={0},m[n],q[n],mc[n],bits;
int reg[2*n+1]={0};//main register to store the content of acc q q-1
//dt-->digit thickness

//shift reg right
void rightshift()
{
  int i,j,k;
  //copying acc q into reg
  for(i=0;i<bits;i++)
	reg[i]=acc[i];
  for(j=0;j<bits;i++,j++)
	reg[i]=q[j];

   //right shifting
  for(i=2*bits;i>0;i--)
  {
	reg[i]=reg[i-1];
  }
  //copying back acc q from reg
  for(i=0;i<bits;i++)
	acc[i]=reg[i];
  for(j=0;j<bits;i++,j++)
	q[j]=reg[i];

}

void drawDig0(int x,int y,int col)
{
	int i=0;
	//thickness of 0
	setcolor(col);
	for(;i<dt;i++)
	{
		x+=i;
		y+=i;
		rectangle(x+w/2-l/2,y+w/2-l/2-o/2,x+w/2+l/2,y+w/2+l/2+o/2);
	}
	setcolor(blkcol);
}

void drawDig1(int x,int y,int col)
{
	int i=0;
	int y1=y+w/2-l/2-o/2;
	int y2=y+w/2+l/2+o/2;
	x=x+w/2-dt/2;
	setcolor(col);
	for(;i<=dt;i++)
	{
		x++;
		line(x,y1,x,y2);
		//setcolor(GREEN);
		//delay();
	}
	setcolor(blkcol);
}

void makeBlock(int x,int y,int blkstyle)
{
	//floodfill(x+1,y+1,blkcol);
	int current_dig=getpixel(x+w/2,y+w/2);

	if(current_dig==dig1col)
		drawDig1(x,y,bgcol);
	else
		drawDig0(x,y,bgcol);

	setlinestyle(blkstyle,1,1);
	rectangle(x,y,x+w,y+w);
}

void makeReg(int bitlen,int x,int y,int blkstyle)
{
	int i=0;
	for(i=0;i<bitlen;i++)
	{
		makeBlock(x,y,blkstyle);
		x+=w;
	}
}

void makeScreen(int bitlen,int x,int y)
{
	//line style 0->solid line or rect ,3->dashed line or rect
	int blkstyle=0;

	//Accumulator reg
	makeReg(bitlen,x,y,blkstyle);
	x+=(bitlen+1)*w;

	//Q(Multiplier) reg
	makeReg(bitlen,x,y,blkstyle);
	x+=(bitlen+1)*w;

	blkstyle=3;
	//Q-1 reg
	makeReg(1,x,y,blkstyle);
	x+=(1+1)*w;

	blkstyle=0;
	//M(Multiplicand) reg
	makeReg(bitlen,x,y,blkstyle);
	x+=(bitlen+1)*w;

	/*blkstyle=0;
	//-M reg
	makeReg(bitlen,x,y,blkstyle); */
}

void initScreen(int A[n],int Q[n],int M[n],int x,int y)
{
	int i=0;
	//A[0]=0,A[1]=0,A[2]=0,A[3]=0;
	//M[0]=0,M[1]=1,M[2]=0,M[3]=1;
	//Q[0]=1,Q[1]=1,Q[2]=0,Q[3]=1;
	//checking the parameter values
	makeScreen(bits,x,y);
	//putting content in accumulator
	for(i=0;i<bits;i++)
	{
		if(A[i]==0)
			drawDig0(x,y,dig0col);
		else
			drawDig1(x,y,dig1col);
		x+=w;
	}

	x+=w;//moving to Q(Multiplier)

	//putting digits in Multiplier
	for(i=0;i<bits;i++)
	{
		if(Q[i]==0)
			drawDig0(x,y,dig0col);
		else
			drawDig1(x,y,dig1col);
		x+=w;
	}

	x+=w;//moving to Q-1
	{
		if( reg[2*bits]==0)
			drawDig0(x,y,dig0col);
		else
			drawDig1(x,y,dig1col);
		x+=w;
	}

	x+=w;//moving to M

	//putting digits in multiplicand
	for(i=0;i<bits;i++)
	{
		if(M[i]==0)
			drawDig0(x,y,dig0col);
		else
			drawDig1(x,y,dig1col);
		x+=w;
	}
}

void add(int *temp1, int *temp2)
{       int i;
	int c=0;
	for(i=bits-1;i>=0;i--)
	{
		temp1[i]=temp1[i]+temp2[i]+c;
		//adjusting for carry
		if(temp1[i]>1)
		{
			temp1[i]-=2;
			c=1;
		}
		else
		{
			c=0;
		}
	}
}

void twoComp(int *temp)
{
	int i=bits-1;

	while(temp[i]!=1 && i>=0)
	{
		i--;
	}

	i=i-1;
	while(i>=0)
	{
		temp[i]=!temp[i];
		i--;
	}
}

void convert2(int num, int *temp)
{
	int i,sign,t=num;
	printf("%d converting..",num);
	printf("%x is add",temp);
       //	temp=(int *)malloc(sizeof(int)*n);
	i=bits-1;
	sign=(num>=0)?0:1;

	num=abs(num);

	while(num>=0 && i>=0)
	{
		temp[i]=num%2;
		printf("%d\t",temp[i]);
		num/=2;
		i--;
	}

	while(i>=0)
	{
		temp[i]=0;
		i--;
	}

	if(sign)
		twoComp(temp);
    //displaying
    printf("Converted %d:\n",t);
	for(i=0;i<bits;i++)
		printf("%d\t",temp[i]);
}

int input()
{
	int multiplicand,multiplier,i=0;
	printf("Enter value of Multiplicand\n");
	scanf("%d",&multiplicand);
	printf("Enter value of Multiplier\n");
	scanf("%d",&multiplier);

	if(multiplicand<8 && multiplier<8)
		bits=4;
	else if(multiplicand<16 && multiplier<16)
		bits=5;
	else return 0;
	clrscr();
     //	printf("%x is add\n",m);
	convert2(multiplicand,m);

	convert2(multiplicand,mc);//initializes mc with M
	twoComp(mc);//taking -M in mc

	convert2(multiplier,q);

      /*	printf("m:");
	while(m[i]!=-1)
		printf(" %d",m[i]);
		i++;
	printf("\nq:");
	while(q[i]!=-1)
		printf(" %d",q[i]);
		i++; */
	printf("\nM:\n");
	for(i=0;i<bits;i++)
		printf("%d",m[i]);
	printf("\nQ:\n");
	for(i=0;i<bits;i++)
		printf("%d",q[i]);
	return 1;
}

char cmpmsg[]="Comparing Q0Q-1...";
char nmsg[]="";
char negq[]="Q0Q-1 ->10...";
char posq[]="Q0Q-1 ->01...";
char addm[]="Adding M to A";
char subm[]="Adding -M to A";

void reglabel(int x,int y)
{
	outtextxy(x,y,"A");
	outtextxy(x+w+w*bits,y,"Q");
	outtextxy(x+(5*w)/2+(3*w*bits)/2,y,"Q");
	outtextxy(x+(5*w)/2+(3*w*bits)/2+5,y+o/4,"-1");
	outtextxy(x+4*w+2*w*bits,y,"M");
}

void booths(int x,int y)
{
	int i;
	char count[1],e[1];

	initScreen(acc,q,m,x,y);
	//Labels A Q Q-1 M
	/*outtextxy(x+(w*bits)/2-2,y+w+o/2,"A");
	outtextxy(x+w+(3*w*bits)/2-2,y+w+o/2,"Q");
	outtextxy(x+(5*w)/2+(2*w*bits)-2,y+w+o/2,"Q");
	outtextxy(x+(5*w)/2+(2*w*bits),y+w+(3*o)/4,"-1");
	outtextxy(x+4*w+(5*w*bits)/2-2,y+w+o/2,"M");*/
	reglabel(x+(w*bits)/2-2,y+w+o/2);

	//itoa-->convert bits-i i.e int to string
	itoa(bits,count,10);
	outtextxy(x+3*w*bits+5*w+w/2,y+o,count);

	delay(tc/8);
	for(i=0;i<bits;i++)
	{
		//sprintf(nmsg,"N:%d\n",bits-i);
		line(x,y+(i+1)*w+(i*6+1)*o+o/2,x+3*w*bits+10*w,y+(i+1)*w+(i*6+1)*o+o/2);
		outtextxy(x,y+(i+1)*w+(i*6+2)*o,cmpmsg);
		if(q[bits-1]!=reg[2*bits])
		{
			if(q[bits-1]>reg[2*bits])
			{
				//gotoxy(0,0);
				outtextxy(x+w*bits+2*w,y+(i+1)*w+(i*6+2)*o,negq);
				delay(2000);
				outtextxy(x+2*w*bits+2*w,y+(i+1)*w+(i*6+2)*o,subm);
				add(acc,mc);
				delay(tc/4);
				initScreen(acc,q,m,x,y+(i+1)*w+(i+1)*6*o);
				//Labels A Q Q-1 M
				/*outtextxy(x+(w*n)/2-2,y+(i+2)*w+(i+2)*o+o/2,"A");
				outtextxy(x+w+(3*w*n)/2-2,y+(i+2)*w+(i+2)*o+o/2,"Q");
				outtextxy(x+(5*w)/2+(2*w*n)-2,y+(i+2)*w+(i+2)*o+o/2,"Q");
				outtextxy(x+(5*w)/2+(2*w*n),y+(i+2)*w+(i+2)*o+(3*o)/4,"-1");
				outtextxy(x+4*w+(5*w*n)/2-2,y+(i+2)*w+(i+2)*o+o/2,"M");
				*/
				reglabel(x+(w*bits)/2-2,y+(i+2)*w+(i+1)*6*o+o/2);
			       /*	printf("-M is :\n");
				for(j=0;j<bits;j++)
					printf("%d\t",mc[j]);

				printf("A<- A-M\n");
			       /*	printf("A+(-M):\n");
				for(j=0;j<bits;j++)
					printf("%d\t",acc[j]);*/
				delay(tc/2);
			}
			else
			{
				//gotoxy(0,0);
				outtextxy(x+w*bits+2*w,y+(i+1)*w+(i*6+2)*o,posq);
				delay(1000);
				outtextxy(x+2*w*bits+2*w,y+(i+1)*w+(i*6+2)*o,addm);
				delay(3000);
				add(acc,m);
				initScreen(acc,q,m,x,y+(i+1)*w+(i+1)*6*o);
				reglabel(x+(w*bits)/2-2,y+(i+2)*w+(i+1)*6*o+o/2);

			     /*	printf("-M is :\n");
				for(j=0;j<bits;j++)
					printf("%d\t",mc[j]);

				printf("\nA<A+M\n");
			      *	printf("A+(-M):\n");
				for(j=0;j<bits;j++)
					printf("%d\t",acc[j]);
				*/

				delay(tc/8);
			}
		}//end of comparing Q0Q-1
		else if(q[bits-1]==0)//implies Q0Q-1-->00
		{
			//gotoxy(0,0);
			//printf("Q0Q-1 ->00...\n");
			outtextxy(x+w*bits+2*w,y+(i+1)*w+(i*6+2)*o,"Q0Q-1 ->00...");
			delay(tc/8);
		}
		else//Q0Q-1-->11
		{
			outtextxy(x+w*bits+2*w,y+(i+1)*w+(i*6+2)*o,"Q0Q-1 ->11...");
			delay(tc/8);
		}
		//printf("Right shifting AQQ-1...\n");
		outtextxy(x,y+(i+1)*w+(i*6+4)*o,"Right shifting AQQ-1...");
		delay(tc/8);
		rightshift();
		initScreen(acc,q,m,x,y+(i+1)*w+(i+1)*6*o);
		reglabel(x+(w*bits)/2-2,y+(i+2)*w+(i+1)*6*o+o/2);

		//itoa-->convert bits-i i.e int to string
		itoa(bits-(i+1),count,10);
		outtextxy(x+3*w*bits+5*w+w/2,y+(i+1)*w+(i+1)*6*o+o,count);

		delay(tc/8);

		//printf("DECREMENTING count to:%d...\n",n-i-1);
		delay(tc/8);
		//initScreen(acc,q,m,x,y);
		//clrscr();
		//return 0;
	}
	//printf("\nFINAL RESULT..\n");
	line(x,y+(i+1)*w+(i*6+1)*o+o/2,x+3*w*bits+10*w,y+(i+1)*w+(i*6+1)*o+o/2);
	outtextxy(x,y+(i+1)*w+(i*6+2)*o,"STOP...");
	delay(tc/2);
	//initScreen(acc,q,m,x,y+(i+1)*w+(i+1)*6*o);
	//reglabel(x+(w*bits)/2-2,y+(i+2)*w+(i+1)*6*o+o/2);
	cleardevice();
	outtextxy(x,y+2*w,"FINAL RESULT...");
	initScreen(acc,q,m,x,y+3*w);
	reglabel(x+(w*bits)/2-2,y+4*w+o/2);
	delay(tc/4);
}

int main()
{
	int gd=DETECT,gm=0;
	int xi=0,yi=0,xtemp,ytemp,mul,mpr;
	//int j,i=0;
	clrscr();
	
	setgraphbufsize(16*1024);
	detectgraph(&gd,&gm);
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	setbkcolor(bgcol);
	setlinestyle(3,1,1);
	//textcolor(WHITE);
	//textbackground(bgcol);
	cleardevice();
	//delay(tc);

       //	 printf("Enter m and q:");
       //	 scanf("%d%d",&mul,&mpr);
       // printf("\n %x is add..\n",m);
       //	 convert2(mul,&m[0]);
       //	 convert2(mpr,&q[0]);
	
	if(!input())
	{
		printf("Multiplicand or Multiplier should be less than 16");
		printf("\nTerminating...");
		getch();
		return 0;
	}

	delay(2000);
	clrscr();
	cleardevice();
	/*
	xtemp=xi,ytemp=yi;
	makeReg(4,xi,yi);
	for(;i<4;i++)
	{
		drawDig0(xtemp+=w,ytemp);
	}
	delay(20);
	xtemp=xi,ytemp=yi;
	makeReg(4,xi,yi);
	for(i=0;i<4;i++)
	{
		drawDig1(xtemp+=w,ytemp);
	} */
	//makeScreen(8,5,50);
	/*
	for(i=0;i<n;i++)
	{	initScreen(acc,q,m,5,50+i*w+i*o);
		rightshift();
	}*/
       //	initScreen(acc,q,m,5,50+3*w+3*o);

 //	cleardevice();
	outtextxy(xi,yi,"Visualizing Booth's Algorithm");
	outtextxy(xi+3*w*bits+5*w,yi+w/4,"COUNT");
	booths(5,20);
	/*
	add(acc,mc);
	printf("\n-M is :\n");
				for(j=0;j<n;j++)
					printf("%d\t",mc[j]);

	printf("\nA is :\n");
				for( j=0;j<n;j++)
					printf("%d\t",acc[j]);

	*/
	getch();
	closegraph();
	return 0;
}
