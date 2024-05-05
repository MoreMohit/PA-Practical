//6.LED
#include <xc.h> //Include Controller specific .h
//Configuration bit settings
//#pragma config OSC = HS //Oscillator Selection
#pragma config WDT = OFF //Disable Watchdog timer
#pragma config LVP = OFF //Disable Low Voltage Programming
#pragma config PBADEN = OFF //Disable PORTB Analog inputs
//Function Prototypes
void msdelay (unsigned int time);//Function for delay
//Start of Program Code
void main()
{
	INTCON2bits.RBPU=0;
	ADCON1 = 0x0F; //To disable the all analog inputs
	TRISD = 0x00; //To configure PORTD as output
	while (1) //While loop for repeated operation
	{
		PORTD = 0xFF; //Turn ON the all LED’s
		msdelay(250); // Delay
		PORTD = 0x00; //Turn OFF the all LED’s
		msdelay(250); // Delay
	}	
} //End of the Program
//Function Definition for delay degeneration
void msdelay (unsigned int time)
{
	unsigned int i, j;
	for (i = 0; i < time; i++)
	for (j = 0; j < 710; j++); //Calibrated for a 1 ms delay in MPLAB
}

//7.BUZZER
#include <xc.h> //Include Controller specific .h
//Configuration bits setting
//#pragma config OSC = HS //Oscillator Selection
#pragma config WDT = OFF //Disable Watchdog timer
#pragma config LVP = OFF //Disable Low Voltage Programming
#pragma config PBADEN = OFF //Disable PORTB Analog inputs
// Timer0 ISR Definition
void __interrupt () timer0_isr (void)
{
	TMR0H = 0X85; //Reload the timer values after overflow
	TMR0L = 0XEE;
	PORTBbits.RB3 = ~PORTBbits.RB3; //Toggle the RB3
	INTCONbits.TMR0IF = 0; //Reset the timer overflow interrupt flag
}
// Start of main Program
void main()
{
	ADCON1 = 0x0F; //Configuring the PORTE pins as digital I/O
	TRISBbits.TRISB3 = 0; //Configuring the RB3 as output
	PORTBbits.RB3 = 0; //Setting the initial value
	T0CON = 0x02; //Set the timer to 16-bit,Fosc/4,1:16 pre-scaler
	TMR0H = 0x85; //load timer value to generate delay 50 ms
	TMR0L = 0xEE;
	INTCONbits.TMR0IF =0; // Clear Timer0 overflow flag
	INTCONbits.TMR0IE =1; // TMR0 interrupt enabled
	T0CONbits.TMR0ON = 1; // Start timer0
	INTCONbits.GIE = 1; // Global interrupt enabled
	while(1); //Program execution stays here until the

}

//8.EXTERNAL_INTERRUPT(RELAY)
#include <xc.h>
//#pragma config OSC = HS //Oscillator Selection
#pragma config WDT = OFF //Disable Watchdog timer
#pragma config LVP = OFF //Disable Low Voltage Programming
#pragma config PBADEN = OFF //Disable PORTB Analog inputs
void __interrupt(high_priority) switch_isr (void)
{
	PORTBbits.RB2 = ~PORTBbits.RB2; //Toggle the RB2
	INTCONbits.INT0IF=0; //Reset the interrupt flag
}
void main()
{
	ADCON1 = 0x0F; // Configuring PORT pins as digital I/O
	TRISBbits.TRISB2 = 0; // Configuring the RB2 as output
	PORTBbits.RB2 = 0; // Setting the initial value
	TRISBbits.TRISB0=1; // Make INT0 pin as an input pin
	INTCON2=0x00; // Interrupt detection on falling Edge
	INTCONbits.INT0IF=0; // Clear INT0IF flag
	INTCONbits.INT0IE=1; // Enable INT0 external interrupt
	INTCONbits.GIE=1; // Enable Global Interrupt
	while(1);
}

//9.LCD
#include <xc.h> //Include Controller specific .h

// Configuration bit settings
//#pragma config OSC = HS //Oscillator Selection
#pragma config WDT = OFF //Disable Watchdog timer
#pragma config LVP = OFF //Disable Low Voltage Programming
#pragma config PBADEN = OFF //Disable PORTB Analog inputs

// Declarations
#define LCD_DATA PORTD //LCD data port to PORTD
#define ctrl PORTE //LCD control port to PORTE
#define rs PORTEbits.RE0 //register select signal to RE0
#define rw PORTEbits.RE1 //read/write signal to RE1
#define en PORTEbits.RE2 //enable signal to RE2

// Function Prototypes
void init_LCD(void); //Function to initialize the LCD
void LCD_command(unsigned char cmd); //Function to pass command to LCD
void LCD_data(unsigned char data); //Function to write char to LCD
void LCD_write_string(char *str); //Function to write string
void msdelay(unsigned int time); //Function to generate delay

// Start of Main Program
void main(void)
{
    char var1[] = " Wel-Come"; //Declare message to be displayed
    char var2[] = "Smart Logic Tech";
    ADCON1 = 0x0F; //Configuring the PORTE pins as digital I/O
    TRISD = 0x00; //Configuring PORTD as output
    TRISE = 0x00; //Configuring PORTE as output
    init_LCD(); // call function to initialize of LCD
    msdelay(50); // delay of 50 milliseconds
    LCD_write_string(var1); //Display message on first line
    msdelay(15);
    LCD_command(0xC0); // initiate cursor to second line
    LCD_write_string(var2); //Display message on second line
    while (1); //Loop here
}

// Function Definitions
void msdelay(unsigned int time) 
{ //Function to generate delay
    unsigned int i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 710; j++) {} //Calibrated for a 1 ms delay in MPLAB
}

void init_LCD(void)
{ // Function to initialize the LCD
    LCD_command(0x38); // initialization of 16X2 LCD in 8bit mode
    msdelay(15);
    LCD_command(0x01); // clear LCD
    msdelay(15);
    LCD_command(0x0C); // cursor off
    msdelay(15);
    LCD_command(0x80); // go to first line and 0th position
    msdelay(15);
}

void LCD_command(unsigned char cmd) 
{ //Function to pass command to LCD
    LCD_DATA = cmd; //Send data on LCD data bus
    rs = 0; //RS = 0 since command to LCD
    rw = 0; //RW = 0 since writing to LCD
    en = 1; //Generate High to low pulse on EN
    msdelay(15);
    en = 0;
}

void LCD_data(unsigned char data) 
{ //Function to write data to the LCD
    LCD_DATA = data; //Send data on LCD data bus
    rs = 1; //RS = 1 since data to LCD
    rw = 0; //RW = 0 since writing to LCD
    en = 1; //Generate High to low pulse on EN
    msdelay(15);
    en = 0;
}

//Function to write string to LCD
void LCD_write_string(char *str) 
{
    int i = 0;
    while (str[i] != '\0') { //Check for end of the string
        LCD_data(str[i]); // sending data on LCD byte by byte
        msdelay(15);
        i++;
    }
}

//10.PWM
#include <xc.h> //Include Controller specific .h
//#pragma config OSC = HS //Oscillator Selection
#pragma config WDT = OFF //Disable Watchdog timer
#pragma config LVP = OFF //Disable Low Voltage Programming
#pragma config PBADEN = OFF //Disable PORTB Analog inputs
void myMsDelay (unsigned int time) // Definition of delay subroutine
{
	unsigned int i, j;
	for (i = 0; i < time; i++) // Loop for time
	for (j = 0; j < 710; j++); // Calibrated for a 1 ms delay in MPLAB
}
void main()
{
	TRISCbits.TRISC0 = 0; // Set PORTC, RC6 as output (DCM IN1)
	TRISCbits.TRISC1 = 0; // Set PORTC, RC6 as output (DCM IN2)
	TRISCbits.TRISC2 = 0; // Set PORTC, RC2 as output (CCP1)
	PR2 = 0x4E; // set PWM Frequency 4KHz
	CCP1CON = 0x0C; // Configure CCP1CON as PWM mode.
	T2CON = 0x07; // Start timer 2 with prescaler 1:16
	PORTCbits.RC0 = 1; // Turn ON the Motor
	PORTCbits.RC1 = 0;
	while(1) // Endless Loop
	{
		// ----------Duty Cycle 80%-----------
		CCP1CONbits.DC1B0 = 0;
		CCP1CONbits.DC1B1 = 1;
		CCPR1L = 0x3E;
		myMsDelay(2000);
		// ----------Duty Cycle 60%-----------
		CCP1CONbits.DC1B0 = 1;
		CCP1CONbits.DC1B1 = 1;
		CCPR1L = 0x2E;
		myMsDelay(2000);
		// ----------Duty Cycle 40%-----------
		CCP1CONbits.DC1B0 = 1;
		CCP1CONbits.DC1B1 = 0;
		CCPR1L = 0x1F;
		myMsDelay(2000);
		// ----------Duty Cycle 20%-----------
		CCP1CONbits.DC1B0 = 0;
		CCP1CONbits.DC1B1 = 1;
		CCPR1L = 0x0F;
		myMsDelay(2000);
}

