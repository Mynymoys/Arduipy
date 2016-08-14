/*
Arduino connect  DS1302
Added serial adjustment time code
*/
#include <stdio.h>
#include <string.h>
#include <DS1302.h>

/* Interface definition
CE(DS1302 pin5) -> Arduino D5
IO(DS1302 pin6) -> Arduino D6
SCLK(DS1302 pin7) -> Arduino D7
*/
uint8_t CE_PIN   = 5;
uint8_t IO_PIN   = 6;
uint8_t SCLK_PIN = 7;

/* Date variable cache */
char buf[50];
char day[10];
/* Serial data cache */
String comdata = "";
int numdata[7] ={0}, j = 0, mark = 0;
/* Create DS1302 object */
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);


void print_time()
{
    /* 从 DS1302 Get the current time */
    Time t = rtc.time();
    /* Converts a week from the number to a name */
    memset(day, 0, sizeof(day));
    switch (t.day)
    {
    case 1: strcpy(day, "Sunday"); break;
    case 2: strcpy(day, "Monday"); break;
    case 3: strcpy(day, "Tuesday"); break;
    case 4: strcpy(day, "Wednesday"); break;
    case 5: strcpy(day, "Thursday"); break;
    case 6: strcpy(day, "Friday"); break;
    case 7: strcpy(day, "Saturday"); break;
    }
    /* Formatting the date code to buf wait for the output */
    snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d", day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);
    /* The output date to the serial port */
    Serial.println(buf);
}


void setup()
{
    Serial.begin(9600);
    rtc.write_protect(false);
    rtc.halt(false);
}

void loop()
{

    /* When the serial data, the data will be joined to the variable  comdata */
    while (Serial.available() > 0)
    {
        comdata += char(Serial.read());
        delay(2);
        mark = 1;
    }
/* Comma separated comdata string，Decomposition results become converted into digital to numdata[] array
 */
    if(mark == 1)
    {
        Serial.print("You inputed : ");
        Serial.println(comdata);
        for(int i = 0; i < comdata.length() ; i++)
        {
            if(comdata[i] == ',' || comdata[i] == 0x10 || comdata[i] == 0x13)
            {
                j++;
            }
            else
            {
                numdata[j] = numdata[j] * 10 + (comdata[i] - '0');
            }
        }
        /* Will be converted into a good numdata time format，Write to DS1302 */
        Time t(numdata[0], numdata[1], numdata[2], numdata[3], numdata[4], numdata[5], numdata[6]);
        rtc.time(t);
        mark = 0;j=0;
        /* Empty comdata variable，In order to wait for the next input */
        comdata = String("");
        /* empty numdata */
        for(int i = 0; i < 7 ; i++) numdata[i]=0;
    }
    
    /* Print the current time */
    print_time();
    delay(1000);
}
