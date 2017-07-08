//********************************************************

//########################################################################

#include<linux/i2c-dev.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>
#include<curl/curl.h>

//////////////////////////////////////////////////////////////////////////
//Galileo commands

#define MSG_RESET 0x0       /* reset the sensor to initial state */
#define MSG_PING  0x1       /* check if the sensor is working properly */  
#define MSG_GET   0x2       /* obtain the most recent ADC result */

/////////////////////////////////////////////////////////////////////////////
// Sensor Device Responses
 
#define MSG_ACK 0xE        /* acknowledgement to the commands */

#define MSG_NOTHING 0xF    /* reserved */

/////////////////////////////////////////////////////////////////////////////
// RTC commands 
#define SET_TIME 0x03     /*enter time setting routine*/
#define Slave_address 0x68

/////////////////////////////////////////////////////////////////////////////
//GPIO Pins being used

#define GP_4 28     //Dataline D0
#define GP_5 17     //Dataline D1
#define GP_6 24     //Dataline D2
#define GP_7 27     //Dataline D3

#define GP_8 26     //strobe signal


#define GP_A4 29   //I2C Mux sel

#define GPIO_DIRECTION_IN 1
#define GPIO_DIRECTION_OUT 0
#define ERROR -1

////////////////////////////////////////////////////////////////////////////

pthread_mutex_t lock;
static int comd, old_comd;
static double ADC_value;
static int setting_time;
static int sec, min, hr, date, mth, yr;
static char day[3];

////////////////////////////////////////////////////////////////////////////
int msg_reset();
int expogpio();
int set_time();
int find_day(char *day, int index);
void HTTP_GET(const char* url);

/////////////////////////////////////////////////////////////////////////////
int expogpio(int gpiopin, int gpio_dir)
{
  int fp1,fp2,fp3,filemode,filehandle;
  char buffer[256];

  filehandle=open("/sys/class/gpio/export",O_WRONLY);
  if(ERROR == filehandle)
        {
               puts("Error: Unable to opening /sys/class/gpio/export");
               return(-1);
        }

  sprintf(buffer, "%d", gpiopin);
  write(filehandle, buffer, strlen(buffer));
  close(filehandle);
  ////////////////////////Direction/////////////////////////////////////

  sprintf(buffer, "/sys/class/gpio/gpio%d/direction", gpiopin);
  if(gpio_dir==0)
    {
      
      filehandle=open(buffer,O_WRONLY);
      if(ERROR == filehandle)
        {
               puts("Error: Unable to open");
               puts(buffer);
               return(-1);
        }


      write(filehandle, "out", 3);
      filemode=O_WRONLY;
      close(filehandle);
      ////////////Drive//////////
      /* sprintf(buffer,"/sys/class/gpio/gpio%d/drive", gpiopin);
      filehandle=open(buffer,O_WRONLY);

             if(ERROR == filehandle)
            {
               puts("Error: Unable to open");
               puts(buffer);
               return(-1);
            }
      
      write(filehandle, "strong", 6);
      close(filehandle); */
    }

  else

    {
      sprintf(buffer, "/sys/class/gpio/gpio%d/direction", gpiopin);

      filehandle=open(buffer,O_WRONLY);
            if(ERROR == filehandle)
              {
                  puts("Error: Unable to open");
                  puts(buffer);
                   return(-1);
              } 

      write(filehandle,"in", 2);
      filemode=O_RDONLY;
      close(filehandle);

      /*  sprintf(buffer,"/sys/class/gpio/gpio%d/drive", gpiopin);
      filehandle=open(buffer,O_WRONLY);
              if(ERROR == filehandle)
                {
                    puts("Error: Unable to open");
                    puts(buffer);
                    return(-1);
                }

      write(filehandle, "strong", 6);
      close(filehandle); */
                              
    }
 
//Open GPIO for Read / Write
        sprintf(buffer, "/sys/class/gpio/gpio%d/value", gpiopin);
        filehandle = open(buffer, filemode);
        if(ERROR == filehandle)
        {
               puts("Unable to open file:");
               puts(buffer);
               return(-1);
        }

        return(filehandle);  //This file handle will be used in read/write and close operations.
 

}
/////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////

int writegpio(int fhandle, int val)
{
        if(val ==  0)
        {
               // Set GPIO low status
               write(fhandle, "0", 1);
        }
        else
        {
               // Set GPIO high status
               write(fhandle, "1", 1);
        }

        return(0);
}

int readgpio1(int filehandle)
{
  //puts("\nInside read function\n");
        int value;
        
        read(filehandle, &value, 1);
	//printf("\n%d",value);
        if('0' == value)
        {
             // Current GPIO status low
               value = 0;
        }
        else
        {
             // Current GPIO status high
               value = 1;
        }

        return value;
}


int readgpio2(int filehandle)
{
  //puts("\nInside read function\n");
        int value;
        
        read(filehandle, &value, 1);
	//printf("\n%d",value);
        if((value%2)==0)
        {
             // Current GPIO status low
               value = 0;
        }
        else
        {
             // Current GPIO status high
               value = 1;
        }

        return value;
}








///////////////////////////////////////////////////////////////////////////////

int closegpio(int gpiopin, int filehandle)
{
        char buffer[256];

        close(filehandle); //This is the file handle of opened GPIO for Read / Write earlier.

        filehandle = open("/sys/class/gpio/unexport", O_WRONLY);
        if(ERROR == filehandle)
        {
               puts("Unable to open file:");
               puts(buffer);
               return(-1);
        }
        sprintf(buffer, "%d", gpiopin);
        write(filehandle, buffer, strlen(buffer));
        close(filehandle);

        return(0);
}




///////////////////////////////////////////////////////////////////////////////
int msg_reset()
{
        int strobe,i,j,k,l,d0,d1,d2,d3,r0,r1,r2,r3; 
	char buf[3];
  puts("\nInside msg_reset function");
  strobe=expogpio(GP_8,GPIO_DIRECTION_OUT);
       if(ERROR == strobe)
            {
               return(-1);
            }
       //////////////////////////////////////
       writegpio(strobe,1);
       printf("\nstrobe is default high");      
       for (i=0;i<=1000;i++)
	 {  printf("#");    }
       ///////////////////////////////////////
       writegpio(strobe,0);
       printf("\nstrobe is low1");
       printf("\nPIC should be ready now");
       for (j=0;j<=2000;j++)
	  {  printf("."); }

	 
	 d0=expogpio(GP_4,GPIO_DIRECTION_OUT);
         d1=expogpio(GP_5,GPIO_DIRECTION_OUT);
         d2=expogpio(GP_6,GPIO_DIRECTION_OUT);
         d3=expogpio(GP_7,GPIO_DIRECTION_OUT);

           
              writegpio(d0,0);
	      writegpio(d1,0);
              writegpio(d2,0);
              writegpio(d3,0);
	      puts("\ncommand is on BUS");        
         for (j=201;j<=40000;j++)
	  {  printf("."); }

	///////////////////////////////////////
        writegpio(strobe,1);
        printf("\nstrobe is high indicating PIC command is on BUS");
        for (k=0;k<=5000;k++)
	  {  printf("*");             }
        //////////////////////////////////////
        writegpio(strobe,0);
        printf("\nstrobe is low and end of write command on BUS");
          
        
        closegpio(GP_4, d0);
        closegpio(GP_5, d1);
        closegpio(GP_6, d2);
        closegpio(GP_7, d3); 
        puts("\nGalileo stops sending command on BUS");
	for (l=0;l<=20;l++)
	  {  printf("$");             }

	////////////readprotocol///////////

        d0=expogpio(GP_4,GPIO_DIRECTION_IN);
        d1=expogpio(GP_5,GPIO_DIRECTION_IN);
        d2=expogpio(GP_6,GPIO_DIRECTION_IN);
        d3=expogpio(GP_7,GPIO_DIRECTION_IN);
        
        puts("\nBUS is in input mode");

        //puts("\nPIC puts response on BUS"); 
        for (l=21;l<=2000;l++)
	  {  printf("$");   }
        
	//////////////////////////////////////////
         writegpio(strobe,1);
	 printf("\nstrobe is high for reading");


       r0=readgpio1(d0);
       //puts("read f0");
       r1=readgpio2(d1);
       //puts("read f1");
       r2=readgpio2(d2);
       //puts("read f2");
       r3=readgpio2(d3);
       //puts("read f3");    
       
       ////////////////////////////////////////   
       printf("\nGalileo reads:\n");
       puts("\t-7654");
       printf("\t %d%d%d%d\n",r3,r2,r1,r0);   


	//////////////////////////////////////////
        writegpio(strobe,0);

        puts("\nstrobe is low and pic stops sending ACk."); 

        puts("\nmessage reset response has finished");
        ///////////////////////////////////////////

        writegpio(strobe,1);
        puts("\nstrobe is default high");



return(0);
}

/////////////////////////////////////////////////////////////////////////////

int msg_ping()

{
  
  int strobe,i,j,k,l,d0,d1,d2,d3,r0,r1,r2,r3; 
  puts("\nInside msg_ping function");
  strobe=expogpio(GP_8,GPIO_DIRECTION_OUT);
       if(ERROR == strobe)
            {
               return(-1);
            }
       //////////////////////////////////////
       writegpio(strobe,1);
       printf("\nstrobe is default high");      
       for (i=0;i<=1000;i++)
	 {  printf("#");    }
       ///////////////////////////////////////
       writegpio(strobe,0);
       printf("\nstrobe is low");
       printf("\nPIC should be ready now");
       for (j=0;j<=2000;j++)
	  {  printf("."); }

	 
	 d0=expogpio(GP_4,GPIO_DIRECTION_OUT);
         d1=expogpio(GP_5,GPIO_DIRECTION_OUT);
         d2=expogpio(GP_6,GPIO_DIRECTION_OUT);
         d3=expogpio(GP_7,GPIO_DIRECTION_OUT);

           
              writegpio(d0,1);
	      writegpio(d1,0);
              writegpio(d2,0);
              writegpio(d3,0);
	      puts("\ncommand is on BUS");        
         for (j=201;j<=40000;j++)
	  {  printf("."); }

	///////////////////////////////////////
        writegpio(strobe,1);
	printf("\nstrobe is high indicating PIC command is on BUS");
        for (k=0;k<=5000;k++)
	  {  printf("*");             }
        //////////////////////////////////////
        writegpio(strobe,0);
        printf("\nstrobe is low and end of write command on BUS");
          
        
        closegpio(GP_4, d0);
        closegpio(GP_5, d1);
        closegpio(GP_6, d2);
        closegpio(GP_7, d3); 
        puts("\nGalileo stops sending command on BUS");
	for (l=0;l<=20;l++)
	  {  printf("$");             }

	////////////readprotocol///////////

        d0=expogpio(GP_4,GPIO_DIRECTION_IN);
        d1=expogpio(GP_5,GPIO_DIRECTION_IN);
        d2=expogpio(GP_6,GPIO_DIRECTION_IN);
        d3=expogpio(GP_7,GPIO_DIRECTION_IN);
        
        puts("\nBUS is in input mode");

        puts("\nPIC puts response on BUS"); 
        for (l=21;l<=2000;l++)
	  {  printf("$");   }
        
	//////////////////////////////////////////
         writegpio(strobe,1);
	 printf("\nstrobe is high for reading");


       r0=readgpio1(d0);
       //puts("read f0");
       r1=readgpio2(d1);
       //puts("read f1");
       r2=readgpio2(d2);
       //puts("read f2");
       r3=readgpio2(d3);
       //puts("read f3");    
       
       ////////////////////////////////////////   
       printf("\nGalileo reads:\n");
       puts("\t-7654");
       printf("\t %d%d%d%d\n",r3,r2,r1,r0);   
       


	//////////////////////////////////////////
        writegpio(strobe,0);

        puts("\nstrobe is low and pic stops sending ACk."); 

        puts("\nmessage ping response has finished");
        ///////////////////////////////////////////

        writegpio(strobe,1);
        puts("\nstrobe is default high");

  return(0);
}



/////////////////////////////////////////////////////////////////////////////
int msg_get()

{
  int strobe,d0,d1,d2,d3,r0,r1,r2,r3,dec,i2c_muxselect;
  //int sec, min, hr, index, date, mth, yr;
  int index;
  int res;
  //char day[3];
  char buf[7];
  long int i,j,k,l; 

  int file1;
  int adapter_nr = 0; /* probably dynamically determined */ 
  char filename[20];
  snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); 

  puts("\nInside msg_get function");
  strobe=expogpio(GP_8,GPIO_DIRECTION_OUT);
       if(ERROR == strobe)
            {
               return(-1);
            }
       //////////////////////////////////////
       writegpio(strobe,1);
       printf("\nstrobe is default high");      
       for (i=0;i<=1000;i++)
	 {  printf("#");    }
       ///////////////////////////////////////
       writegpio(strobe,0);
       printf("\nstrobe is low");
       printf("\nPIC should be ready now");
       for (j=0;j<=2000;j++)
	  {  printf("."); }

	 
	 d0=expogpio(GP_4,GPIO_DIRECTION_OUT);
         d1=expogpio(GP_5,GPIO_DIRECTION_OUT);
         d2=expogpio(GP_6,GPIO_DIRECTION_OUT);
         d3=expogpio(GP_7,GPIO_DIRECTION_OUT);

           
              writegpio(d0,0);
	      writegpio(d1,1);
              writegpio(d2,0);
              writegpio(d3,0);
	      puts("\ncommand is on BUS");        
         for (j=201;j<=40000;j++)
	  {  printf("."); }

	///////////////////////////////////////
        writegpio(strobe,1);
        printf("\nstrobe is high indicating PIC command is on BUS");
        for (k=0;k<=5000;k++)
	  {  printf("*");             }
        //////////////////////////////////////
        writegpio(strobe,0);
        printf("\nstrobe is low and end of write command on BUS");
          
        
        closegpio(GP_4, d0);
        closegpio(GP_5, d1);
        closegpio(GP_6, d2);
        closegpio(GP_7, d3); 
        puts("\nGalileo stops sending command on BUS");
	for (l=0;l<=20;l++)
	  {  printf("$");             }

	////////////readprotocol///////////

        d0=expogpio(GP_4,GPIO_DIRECTION_IN);
        d1=expogpio(GP_5,GPIO_DIRECTION_IN);
        d2=expogpio(GP_6,GPIO_DIRECTION_IN);
        d3=expogpio(GP_7,GPIO_DIRECTION_IN);
        
        //puts("\nBUS is in input mode");

        //puts("\nPIC puts response on BUS"); 
        for (l=21;l<=2000;l++)
	  {  printf("$");   }
        
	//////////////////////////////////////////
         writegpio(strobe,1);
	 printf("\nstrobe is high for reading");


       r0=readgpio2(d0);
       //puts("read f0");
       r1=readgpio2(d1);
       //puts("read f1");
       r2=readgpio2(d2);
       //puts("read f2");
       r3=readgpio2(d3);
       //puts("read f3");    
       
       ////////////////////////////////////////   
       printf("\nGalileo reads:\n");
       puts("\t-7654");
       printf("\t %d%d%d%d",r3,r2,r1,r0);   

       

	//////////////////////////////////////////
        writegpio(strobe,0);

	puts("\nstrobe is low and pic stops sending ACk."); 

        /////////////////////////////////////////////////////////
        
        d0=expogpio(GP_4,GPIO_DIRECTION_IN);
        d1=expogpio(GP_5,GPIO_DIRECTION_IN);
        d2=expogpio(GP_6,GPIO_DIRECTION_IN);
        d3=expogpio(GP_7,GPIO_DIRECTION_IN);
        /////////////////////////////////////////////////////////////
	puts("\nPIC starts keeping ADC result--1 bits----> 9-8-7-6 values on BUS");
         
        for (l=0;l<=20000;l++)


	  {     printf(".");                 }

        ///////////////////////////////////////////////

        writegpio(strobe,1);
	puts("strobe is high for reading ");
        r0=readgpio2(d0);
	//puts("read f0");
       r1=readgpio2(d1);
       //puts("read f1");
       r2=readgpio2(d2);
       //puts("read f2");
       r3=readgpio2(d3);
       //puts("read f3");    
       
      
      for (l=0;l<=20000;l++)


	{   printf(".");                   }

       ////////////////////////////////////////   
       printf("\nGalileo reads:\n");
       puts("\tADC result bits----> 9-8-7-6");
       printf("\t                     %d-%d-%d-%d",r3,r2,r1,r0);

       dec= r3;
       dec <<= 1;
       dec+= r2;
       dec <<= 1;
       dec+= r1;
       dec <<= 1;
       dec+= r0;
       dec <<= 1;
       

      /////////////////////////////////////////////
       
       writegpio(strobe,0);
        /////////////////////////////////////////////////////////
        
        d0=expogpio(GP_4,GPIO_DIRECTION_IN);
        d1=expogpio(GP_5,GPIO_DIRECTION_IN);
        d2=expogpio(GP_6,GPIO_DIRECTION_IN);
        d3=expogpio(GP_7,GPIO_DIRECTION_IN);
        /////////////////////////////////////////////////////////////
     

	puts("\nPIC starts keeping ADC result bits--2----> 5-4-3-2 values on BUS");
         
        for (l=0;l<=2000;l++)


	  {         printf(".");             }

        ///////////////////////////////////////////////

        writegpio(strobe,1);
	puts("strobe is high for reading ");
        r0=readgpio2(d0);
       //puts("read f0");
       r1=readgpio2(d1);
       //puts("read f1");
       r2=readgpio2(d2);
       //puts("read f2");
       r3=readgpio2(d3);
       //puts("read f3");    
       for (l=0;l<=20000;l++)


	 {         printf(".");             }

       ////////////////////////////////////////   
       printf("\nGalileo reads:\n");
       puts("\tADC result bits----> 5-4-3-2");
       printf("\t                     %d-%d-%d-%d",r3,r2,r1,r0); 


       dec+= r3;
       dec <<= 1;
       dec+= r2;
       dec <<= 1;
       dec+= r1;
       dec <<= 1;
       dec+= r0;
       dec <<= 1;


       ///////////////////////////////////////////////////
        

       writegpio(strobe,0);
         /////////////////////////////////////////////////////////
        
        d0=expogpio(GP_4,GPIO_DIRECTION_IN);
        d1=expogpio(GP_5,GPIO_DIRECTION_IN);
        d2=expogpio(GP_6,GPIO_DIRECTION_IN);
        d3=expogpio(GP_7,GPIO_DIRECTION_IN);
        /////////////////////////////////////////////////////////////

    

	puts("\nPIC starts keeping ADC result bits--3----> 1-0-N-N values on BUS");
         
        for (l=0;l<=2000;l++)


	  {              printf(".");        }

        ///////////////////////////////////////////////

        writegpio(strobe,1);
	puts("strobe is high for reading ");
        r0=readgpio2(d0);
       //puts("read f0");
       r1=readgpio2(d1);
       //puts("read f1");
       r2=readgpio2(d2);
       //puts("read f2");
       r3=readgpio2(d3);
       //puts("read f3");    
       
       for (l=0;l<=20000;l++)


	 {         printf(".");             }
       ////////////////////////////////////////   
       printf("\nGalileo reads:\n");
       puts("\tADC result bits----> 1-0-N-N");
       printf("\t                     %d-%d-%d-%d",r3,r2,r1,r0);   
        dec+= r3;
       dec <<= 1;
       dec+= r2;
       //dec <<= 1;

       

        
       /////////////////////////////////////////

       writegpio(strobe,0);
       puts("\nPIC sends ADC Result completion Ack;");

        d0=expogpio(GP_4,GPIO_DIRECTION_IN);
        d1=expogpio(GP_5,GPIO_DIRECTION_IN);
        d2=expogpio(GP_6,GPIO_DIRECTION_IN);
        d3=expogpio(GP_7,GPIO_DIRECTION_IN);

         for (l=0;l<=2000;l++)


	   { printf(".");                     }
       

       //////////////////////////////////////// 
       writegpio(strobe,1);
       printf("\nstrobe is high for reading");


       r0=readgpio2(d0);
       //puts("read f0");
       r1=readgpio2(d1);
       //puts("read f1");
       r2=readgpio2(d2);
       //puts("read f2");
       r3=readgpio2(d3);
       //puts("read f3");    
       
       ////////////////////////////////////////   
       printf("\nGalileo reads:\n");
       puts("\t-7654");
       printf("\t %d%d%d%d",r3,r2,r1,r0);  



       ////////////////////////////////////////////
       writegpio(strobe,0);

       puts("\nstrobe is low and pic stops sending ACk."); 

       puts("\nmessage get response has finished");
        ///////////////////////////////////////////

        writegpio(strobe,1);
	puts("\nstrobe is default high");

	///////////////////GPIO_A4 29 is set to 0////////////////////////

        i2c_muxselect=expogpio(GP_A4,GPIO_DIRECTION_OUT);
             if(ERROR ==i2c_muxselect )
            {
               return(-1);
            }
	writegpio(i2c_muxselect,0);





       //////////////////////////////////////////
	  file1 = open(filename, O_RDWR);	
	  if (file1 < 0) { 
 	/* ERROR HANDLING; you can check errno to see what went wrong */ 
	 puts("Error open file for reading\n");	
	exit(1); 
  }
if (ioctl(file1, I2C_SLAVE, Slave_address) < 0) {    
     	/* ERROR HANDLING; you can check errno to see what went wrong */   
	puts("Error connect device for reading\n");	
	exit(1);

  }
 buf[0] = 0;
 if (write(file1, buf, 1) != 1) {   
 	/* ERROR HANDLING: i2c transaction failed */  
	puts("Error writing\n");		
 }
 pthread_mutex_lock(&lock);
if (read(file1, buf, 7) != 7) {   
 /* ERROR HANDLING: i2c transaction failed */   
 puts("Error reading\n");
 }

 else {    
       sec  = (buf[0] & 0xF) + (((buf[0] & 0x70) >> 4) * 10);
       min  = (buf[1] & 0xF) + (((buf[1] & 0x70) >> 4) * 10);
       hr   = (buf[2] & 0xF) + (((buf[2] & 0x30) >> 4) * 10);
       index  =  buf[3] & 0x7;
       res = get_day(day, index);
       date = (buf[4] & 0xF) + (((buf[4] & 0x30) >> 4) * 10);
       mth  = (buf[5] & 0xF) + (((buf[5] & 0x10) >> 4) * 10);
       yr   = (buf[6] & 0xF) + (((buf[6] & 0xF0) >> 4) * 10);
       
       printf("\ntimestamp (in real time format ");
       printf("20%02d/%02d/%02d %s %02d:%02d:%02d)", yr, mth, date, day, hr, min, sec);       
       /*
       timestamp = "20";
       strcat(timestamp, yr);
       strcat(timestamp, '/');
       strcat(timestamp, mth);
       strcat(timestamp, '/');
       strcat(timestamp, date);
       strcat(timestamp, '-');
       strcat(timestamp, day);
       strcat(timestamp, '-');
       strcat(timestamp, hr);
       strcat(timestamp, ':');

       strcat(timestamp, '-');
       */
       
 }
       ADC_value = (dec*5.0)/1024;
       printf("\n\t ADC voltage value (%.2lfV)\n", ADC_value);
       pthread_mutex_unlock(&lock);
close(file1);
       
  return(0);
}

/////////////////////////////////////////////////////////////////////////////
int set_time() {
  int val, res;
  char buf[8];

  int file1,i2c_muxselect;
  int adapter_nr = 0; /* probably dynamically determined */ 
  char filename[20];
  ////////////////////////////////////////////////////////////
   i2c_muxselect=expogpio(GP_A4,GPIO_DIRECTION_OUT);
             if(ERROR ==i2c_muxselect )
            {
               return(-1);
            }
	     writegpio(i2c_muxselect,0);
	     /////////////////////////////////////


  snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); 


  puts("\nSet year (last 2 digits):");
  res = scanf("%d",&val);
  if (res < 0)
    return res;
  buf[7] = ((val / 10) << 4) + (val % 10);

  puts("\nSet month:");
  res = scanf("%d",&val);
  if (res < 0)
    return res;
  buf[6] = ((val / 10) << 4) + (val % 10);

  puts("\nSet date:");
  res = scanf("%d",&val);
  if (res < 0)
    return res;
  buf[5] = ((val / 10) << 4) + (val % 10);

  puts("\nSet day (Sunday = 7, Monday = 1....):");
  res = scanf("%d",&val);
  if (res < 0)
    return res;
  buf[4] = val;

  puts("\nSet hours (in 24-hr format):");
  res = scanf("%d",&val);
  if (res < 0)
    return res;
  buf[3] = ((val / 10) << 4) + (val % 10);
  buf[3] &= ~0x40;  //Set to 24 hr format 

  puts("\nSet Minutes:");
  res = scanf("%d",&val);
  if (res < 0)
    return res;
  buf[2] = ((val / 10) << 4) + (val % 10);
  
  puts("\nSet Seconds:");
  res = scanf("%d",&val);
  if (res < 0)
    return res;
  buf[1] = ((val / 10) << 4) + (val % 10);
  buf[1] &= ~0x80; //Make sure CH bit is off
  
  buf[0] = 0;

  file1 = open(filename, O_RDWR);
  if (file1 < 0) { 
 	/* ERROR HANDLING; you can check errno to see what went wrong */ 
	 puts("Error open file\n");	
	exit(1); 
  }
  if (ioctl(file1, I2C_SLAVE, Slave_address) < 0) {    
     	/* ERROR HANDLING; you can check errno to see what went wrong */   
	puts("Error connect device\n");	
	exit(1);

 }
 if (write(file1, buf, 8) != 8) {   
 	/* ERROR HANDLING: i2c transaction failed */  
	puts("Error writing\n");		
 }

 close(file1);
}


////////////////////////////////////////////////////////////////////////////
int get_day(char *day, int index) {
  switch (index) {
	case 1:
		strcpy(day, "Mon");
		return 0;
	case 2:
		strcpy(day, "Tue");
		return 0;
	case 3:
		strcpy(day, "Wed");
		return 0;
	case 4:
		strcpy(day, "Thr");
		return 0;
	case 5:
		strcpy(day, "Fri");
		return 0;
	case 6:
		strcpy(day, "Sat");
		return 0;
	case 7:
		strcpy(day, "Sun");		
		return 0;
	default:
		strcpy(day, "Err");
		return -1;
  }

}


void *local_commands(void *input)

{
	int user_input;
  printf("\n*******************************");

  puts("\nthread 1 said:");
  puts("\nSelect one of the below commands:\n");
  puts("\t 0. MSG_RESET\n");
  puts("\t 1. MSG_PING\n");
  puts("\t 2. MSG_GET\n");
  puts("\t 3. SET_TIME\n");
  puts("\nEnter the desired command whenever:");
  while (1) {
    if (comd == 3) {
	    setting_time = 1;
	    while(setting_time);
    }
    scanf("%d",&user_input);
    pthread_mutex_lock(&lock);
    comd = user_input;
    pthread_mutex_unlock(&lock);
  }

}

void *sensor_control (void *input) {
  int strobe;
  int c1,c2,c3,c4; 
  while (1) {
	  strobe=expogpio(GP_8,GPIO_DIRECTION_OUT);
	  if(ERROR == strobe)
		  {
			  return;
		  }
	  //////////////////////////////////////
	  writegpio(strobe,1);
	  printf("\nstrobe is default high");      
	  
	  switch (old_comd)
		  {
		  case MSG_RESET: 
			  puts("\nthread 2 said:");
			  puts("\nGalileo will give  MSG_RESET command to PIC");
			  c1=msg_reset();                
			  break;
			  
		  case MSG_PING:
			  puts("\nthread 2 said:");
			  puts("\nGalileo will give MSG_PING command to PIC");
			  c2=msg_ping();
			  break;
			  
		  case MSG_GET: 
			  puts("\nthread 2 said:");
			  puts("\nGalileo will give MSG_GET command to PIC");
			  c3=msg_get();
			  break;
			  
		  case SET_TIME: 
			  puts("\nthread 2 said:");
			  puts("\nGalileo will set time to RTC");
			  c4 = set_time();
			  setting_time = 0;
			  puts("\nIt is done, now give me another command");
			  while (comd == 3);
			  break;	 	  
		  default: 
			  while((comd < 0) || (comd > 3));
		  }
	  if (old_comd != comd) {
		  old_comd = comd;
		  printf("\nThe new command is %d\n", old_comd);
	  }
  }
}

///////////////////////////////////////////////////////////////////////////
void *web_update (void *input)
{
	const char* hostname = "localhost";
	const int port = 8000;
	const int id = 10;
	const char* password = "password";
	const char* name = "Knights";
	char timestamp[100];
	char buf[1024];
	while (1) {
		snprintf(timestamp, 100, "%02d/%02d/20%02d-%s-%02d:%02d:%02d", mth,date,yr,day,hr,min,sec);
		snprintf(buf, 1024, "http://%s:%d/update?id=%d&password=%s&name=%s&data=%.2lf&status=0x%02x&timestamp=%s",
			hostname,
			port,
			id,
			password,
			name,
			ADC_value,
			old_comd,
			timestamp);
		HTTP_GET(buf);
		sleep(5);
	}
} 

////////////////////////////////////////////////////////////////////////////
 void HTTP_GET(const char* url){
	CURL *curl;
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
} 



///////////////////////////////////////////////////////////////////////////
int main ()
{
	pthread_t threads[3];
	//	pthread_mutex_t mutexa;
	int res, input;	
	comd = -1;
	ADC_value = 0;
	mth,date = 0;
	yr = 0;
	strcpy(day, "Err");
	hr = 0;
	min = 0;
	sec = 0;
	old_comd = -1;
	pthread_mutex_init(&lock, NULL);
	res = pthread_create(&threads[0], NULL, local_commands, (void *)input);
	if (res) {
	  puts("Error creating 1st thread\n");
	  exit(-1);
	}
	res = pthread_create(&threads[1], NULL, sensor_control, (void *)input);
	if (res) {
	  puts("Error creating 2nd thread\n");
	  exit(-1);
	} 	
	res = pthread_create(&threads[2], NULL, web_update, (void *)input);
	if (res) {
	  puts("Error creating 3rd thread\n");
	  exit(-1);
	  } 
	pthread_mutex_destroy(&lock);		      		          		      		    	    
	pthread_exit(NULL);
}

PIC 16F688 Code:

/*   
 * for UMass Lowell 16.480/552
 * 
 * Lab 2 - Bus communication with Galileo
 *
 * Reference: Start from PIC16F688 Data Sheet Page 67
 */
#pragma config WDTE = OFF        // Watchdog Timer Enable bit

#include <pic16f688.h>
// Bus signal definitions 
#define  MSG_RESET          0x0  /* reset the sensor to initial state */
#define  MSG_PING           0x1  /* check if the sensor is working properly */
#define  MSG_GET            0x2  /* obtain the most recent ADC result */
#define  MSG_ACK            0xE  /* acknowledgement to the commands */
#define  MSG_NOTHING        0xF  /* reserved */

// ADC voltage definitions 
#define  VREF               5   // VDD in volts
#define  VDARK              3   // Photoresistor voltage at dark
//any value higher than this will be consider high
#define  ADC_CODE_DARK      (1024*VDARK)/VREF //VDARK in ADC code format 

// bus direction definitions
#define in                  1 
#define out                 0 


// Functions definition
//initialize port
void port_init();
//initialize ADC
void adc_init();
//read ADC value
void adc_read();
//read bus command
unsigned int read_cmd();
//output value to data bus
void write_data(unsigned int data);
//switch data bus in/out direction
void change_dir(char inout);

 
// Main program
void main (void)
{
    unsigned int i;
    unsigned int cmd;
    unsigned int temp;
    unsigned int data; 
    //========================================//
    //       State 0: Initialization          //
    //========================================//     
    port_init();
    adc_init();
    temp = 0xFFFF;

     while(1)
    {
      //========================================//
      // State 1: If signal detected in bus     //
      //========================================//      
         if (!(PORTC & 0x02)) { 
            cmd = read_cmd();  
            change_dir(out); //Change data bus to output
            /****************** RESET CMD *******************/
            if (cmd == MSG_RESET) {                
                write_data (MSG_ACK);  
                // Wait for the bus to be free
                while (!(PORTC & 0x02));
                PORTA |= 0x04;    //Turn on LED 
                for (i=0;i<0xFFFF;i++);
                PORTA &= ~0x04;   //Turn off 
                // Reset Device 
                port_init();
                adc_init();
                temp = 0xFFFF; 
                
                continue;
            /****************** PING CMD ********************/    
            } else if (cmd == MSG_PING) {
                write_data (MSG_ACK);
                // Wait for the bus to be free
                while (!(PORTC & 0x02));
            /******************* GET CMD ********************/    
            } else if (cmd == MSG_GET) {
                // First transfer: ACK 
                write_data (MSG_ACK);
                // Second to Fourth: Data (MSB)
                data = temp;  //make a copy. Note: Left justified 
                for (i = 0; i < 3; i++) {                    
                    write_data (data >> 12); //Load the highest 4 bits
                    data <<= 4; //Shift the next 4 bits to the highest
                } 
                // Last transfer: ACK
                write_data (MSG_ACK);
                // Wait for the bus to be free
                while (!(PORTC & 0x02));
            /****************** ERROR **********************/    
            } else {
                write_data (MSG_NOTHING);
                // Force the transfer to stop 
            } 
            change_dir(in); //Change data bus to input
            
            
          //========================================//
          // State 2: If no signal detected in bus  //
          //========================================//    
        } else {
            for (i = 0; i < 100; i++); //kill some time
            adc_read();
            temp = ADRESH << 8;
            temp += ADRESL;
            if (temp > (ADC_CODE_DARK<<6))    //0xD0 = 4V 
                PORTA |= 0x04;    //Turn on LED 
            else 
                PORTA &= ~0x04;   //Turn off LED                
        } 
    }
}

void port_init() {
  //          PIN assignment 
  // PIN    type    dir   description 
  // RA2  digital   out   LED output 
  // RC0  analog    in    ADC input (set in ADC init)
  // RC1  digital   in    strobe 
  // RC2  digital   i/o   data 0 (LSB)
  // RC3  digital   i/o   data 1
  // RC4  digital   i/o   data 2
  // RC5  digital   i/o   data 3 (MSB)
  // Others     Don't Care 
  
  ANSEL =  0x10;    // AN4 is ANA
  TRISA = ~0x04;    // Only RA2 is output  
  CMCON0 = 0x07;    // RC0 & RC1 are GPIOs
  TRISC = 0x3F;     // RC0 & RC1 are always input
                    // RC2 - RC5 is initially input  
}


void adc_init() {    
    ADCON1 = 0x00;  // ADC Clock is internal clock /2    
    ADCON0 = 0x11; // Left justified
                   // Vref is VDD
                   // Input from AN4 (PIN10)
} 

void adc_read()
{
  ADCON0 |= 0x02;
  while (ADCON0 & 0x02); //wait for DONE
} 


unsigned int read_cmd () {
    unsigned int cmd; 
    // Wait for write starts
    while (!(PORTC & 0x02));
    cmd = PORTC & 0x3C;
    //Data lines starts at RC2
    cmd >>= 2;
    // Wait for the write ends 
    while(PORTC & 0x02);
    return cmd;
}

void write_data(unsigned int data) {
    data <<= 2;   //Data lines starts at RC2
    PORTC &= ~0x3C; // Clear RC2 - RC5 
    PORTC |= data;
      
   // Wait for read starts
    while (!(PORTC & 0x02));
    // Wait for read ends
    while (PORTC & 0x02);
    
    PORTC &= ~0x3C;
}

void change_dir(char inout) {
    // If RC2 - RC5 are inputs 
    if (inout) {
        TRISC |=  0x3C;
    } else {
        TRISC &= ~0x3C;
    }
}
//*****************************************************************************













