#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<stdlib.h>
#define w 30
#define l 10
#define o 8
#define dt 2
#define n 4
#define tc 8000
//tc is time constant for the delay

int acc[n]={0},m[n],q[n], mc[n];
int reg[2*n+1]={0};//main register to store the content of acc q q-1
//dt-->digit thickness

//shift reg right
void rightshift()
{
  int i,j,k;
  //copying acc q into reg
  for(i=0;i<n;i++)
	reg[i]=acc[i];
  for(j=0;j<n;i++,j++)
	reg[i]=q[j];

   //right shifting
  for(i=2*n;i>0;i--)
  {
	reg[i]=reg[i-1];
  }
  //copying back acc q from reg
  for(i=0;i<n;i++)
	acc[i]=reg[i];
  for(j=0;j<n;i++,j++)
	q[j]=reg[i];

}

void makeBlock(int x,int y)
{
	//setfillstyle(SOLID_FILL,BLACK);
	//floodfill(x+1,y+1,WHITE);
	rectangle(x,y,x+w,y+w);
}

void makeReg(int bitlen,int x,int y)
{
	int i=0;
	for(i=0;i<bitlen;i++)
	{
		makeBlock(x,y);
		x+=w;
	}
}

void drawDig0(int x,int y)
{
	int i=0;
	//thickness of 0
       //	setcolor(GREEN);
	for(;i<dt;i++)
	{
		x+=i;
		y+=i;
		rectangle(x+w/2-l/2,y+w/2-l/2-o/2,x+w/2+l/2,y+w/2+l/2+o/2);
	}
}

void drawDig1(int x,int y)
{
	int i=0;
       //	setcolor(GREEN);
	for(;i<=dt;i++)
	{
		x+=i;
		line(x+w/2-dt/2,y+w/2-l/2-o/2,x+w/2-dt/2,y+w/2+o/2);
	}
     //	setcolor(WHITE);
}

void makeScreen(int bitlen,int x,int y)
{
	//Accumulator reg
	makeReg(bitlen,x,y);
	x+=(bitlen+1)*w;

	//Q(Multiplier) reg
	makeReg(bitlen,x,y);
	x+=(bitlen+1)*w;

	//Q-1 reg
	makeReg(1,x,y);
	x+=(1+1)*w;

	//M(Multiplicand) reg
	makeReg(bitlen,x,y);
	x+=(bitlen+1)*w;

}


void initScreen(int A[n],int Q[n],int M[n],int x,int y)
{
  //	cleardevice();
	int i=0;
       //	A[0]=0,A[1]=0,A[2]=0,A[3]=0;
	//M[0]=0,M[1]=1,M[2]=0,M[3]=1;
	//Q[0]=1,Q[1]=1,Q[2]=0,Q[3]=1;
	//checking the parameter values
	cleardevice();
	makeScreen(n,x,y);

	//putting content in accumulator
	for(i=0;i<n;i++)
	{
		if(A[i]==0)
			drawDig0(x,y);
		else
			drawDig1(x,y);
		x+=w;
	}

	x+=w;//moving to Q(Multiplier)

	//putting digits in Multiplier
	for(i=0;i<n;i++)
	{
		if(Q[i]==0)
			drawDig0(x,y);
		else
			drawDig1(x,y);
		x+=w;
	}

	x+=w;//moving to Q-1
	{
		if( reg[2*n]==0)
			drawDig0(x,y);
		else
			drawDig1(x,y);
		x+=w;
	}

	x+=w;//moving to M

	//putting digits in multiplicand
	for(i=0;i<n;i++)
	{
		if(M[i]==0)
			drawDig0(x,y);
		else
			drawDig1(x,y);
		x+=w;
	}
}
void add(int *temp1, int *temp2)
{       int i;
	int c=0;
	for(i=n-1;i>=0;i--)
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
	int i=n-1;

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
	i=n-1;
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
	for(i=0;i<n;i++)
		printf("%d\t",temp[i]);


}

void input()
{
	int multiplicand,multiplier,i=0;
	printf("Enter value of Multiplicand\n");
	scanf("%d",&multiplicand);
	printf("Enter value of Multiplier\n");
	scanf("%d",&multiplier);

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
	for(i=0;i<n;i++)
		printf("%d",m[i]);
	printf("\nQ:\n");
	for(i=0;i<n;i++)
		printf("%d",q[i]);
}
char cmpmsg[]="Comparing Q0Q-1....";
char nmsg[]="";
char negq[]="Q0Q-1 10...";
char posq[]="Q0Q-1 01...";
char addm[]="Adding M to A";
char subm[]="Adding -M to A";

void booths(int x,int y)
{
	int i,j;

	initScreen(acc,q,m,x,y);
	delay(tc);
	for(i=0;i<n;i++)
	{       sprintf(nmsg,"N:%d\n",n-i);
		outtext(cmpmsg);
		if(q[n-1]!=reg[2*n])
		{
			if(q[n-1]>reg[2*n])
			{
				gotoxy(0,0);


				outtext(negq);
				delay(1000);
				outtext(subm);
				add(acc,mc);
				delay(tc/4);
				initScreen(acc,q,m,x,y);

			       /*	printf("-M is :\n");
				for(j=0;j<n;j++)
					printf("%d\t",mc[j]);

				printf("A<- A-M\n");
			       /*	printf("A+(-M):\n");
				for(j=0;j<n;j++)
					printf("%d\t",acc[j]);*/
				delay(tc/2);
			}
			else
			{
				gotoxy(0,0);

				outtext(posq);
				delay(1000);
				outtext(addm);
				delay(3000);
				add(acc,m);
				initScreen(acc,q,m,x,y);

			     /*	printf("-M is :\n");
				for(j=0;j<n;j++)
					printf("%d\t",mc[j]);

				printf("\nA<A+M\n");
			      *	printf("A+(-M):\n");
				for(j=0;j<n;j++)
					printf("%d\t",acc[j]);
				*/

				delay(tc);
			}
		}//end of comparing Q0Q-1
		else
		{
			gotoxy(0,0);
			printf("Q0Q-1 ->00...\n");
			delay(1000);
		}
		printf("Right shifting AQQ-1...\n");
		delay(tc/4);
		rightshift();
		initScreen(acc,q,m,x,y);
		delay(tc/2);

		printf("DECREMENTING count to:%d...\n",n-i-1);
		delay(tc/4);
		initScreen(acc,q,m,x,y);
	       //	clrscr();
	}
	printf("\nFINAL RESULT..\n");
	delay(tc/2);
	initScreen(acc,q,m,x,y);
}
int main()
{
	int gd=DETECT,gm=0;
	int xi=50,yi=50,xtemp,ytemp,mul,mpr;
	int j,i=0;
 //      clrscr();
       //	cleardevice();
	setgraphbufsize(16*1024);

	detectgraph(&gd,&gm);
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	setbkcolor(BLACK);
	cleardevice();
       //	 printf("Enter m and q:");
       //	 scanf("%d%d",&mul,&mpr);
       // printf("\n %x is add..\n",m);
       //	 convert2(mul,&m[0]);
       //	 convert2(mpr,&q[0]);
	input();
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
   booths(5,50);
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
