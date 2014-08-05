/*

 Electronic Six Sided Dice
 
 This sketch uses the Serial Interface to allow a user to "roll a six sided dice".  By selecting "y", the user sketch
 will flash a "thinking" sequence, then generate a random number between 1-6, displaying that number on the LEDs, via
 a 74HC595 shift register.
 
 For this revised sketch to work, the LEDs must be arranged in the following pattern:
 
 1       2
 6   7   3
 5       4
 
 http://www.rheingoldheavy.com/electronic-d6/
 
 */

// Initiate our variables
int dataPin    = 4;
int latchPin   = 5;
int clockPin   = 6;

byte leds      = 0;
int roll       = 0;


void setup() 
{

  // Setup our pins
  pinMode         (latchPin,   OUTPUT);
  pinMode         (dataPin,    OUTPUT);  
  pinMode         (clockPin,   OUTPUT);

  // Seed our random number generator
  randomSeed      (analogRead(0));

  // Clear the display
  leds = 0;                               
  refreshShiftRegister();

  // Enable the serial input
  Serial.begin    (9600);
  Serial.println  ("Roll the dice? (y/n)");

}

void loop() 
{

  /*   
   
   The general order of this loop function is to check if the user has typed something into the Serial monitor window.
   If the input is a 'y', then a "thinking pattern" is flashed, before a random number between 1-6 is generated, and 
   displayed on the LED array.  If the input is an 'n', the LED display is cleared.
   
   */

  // Check for user input
  if (Serial.available())
  {
    char ch = Serial.read();
    if (ch == 'y')
    {

      // Clear the display
      leds = 0;                               
      refreshShiftRegister();
      delay(500);

      // Run a cute thinking pattern
      thinkingPattern();

      // Generate a random number between 1-6. The upper bound is excluded so it has to be 6 + 1.
      roll = random(1,7);

      // Set the LED pattern based on the value of 'roll'
      switch (roll)
      {
      case 1:
        leds = 64;
        break;

      case 2:
        leds = 9;
        break;

      case 3:
        leds = 73;
        break;

      case 4:
        leds = 27;
        break;

      case 5:
        leds = 91;
        break;

      case 6:
        leds = 63;
        break;
      }

      refreshShiftRegister();

      // Output the value of 'roll' to the serial monitor
      Serial.print   ("You rolled a ");
      Serial.print   (roll);
      Serial.println ("!");

    }

    // If the user input is 'n', clear the LED display
    if (ch == 'n')
    {
      leds = 0;
      refreshShiftRegister  ();
      Serial.println        ("Cleared");
    }
  }
}

void refreshShiftRegister()
{
  // Sends a full byte value to the shift register and toggles the latch.
  digitalWrite  (latchPin,  LOW);
  shiftOut      (dataPin, clockPin, MSBFIRST, leds);
  digitalWrite  (latchPin,  HIGH);
}


void thinkingPattern()
{

  // Some simple chasing patterns 

  /* PATTERN 1 */
  /*
  for (int i = 0; i < 3; i++) 
  {
    leds = 9;
    refreshShiftRegister();
    delay(150);

    leds = 18;
    refreshShiftRegister();
    delay(150);

    leds = 36;
    refreshShiftRegister();
    delay(150);       
  }
  */

  /* PATTERN 2 */
  /*
  for (int i = 1; i < 64; i = i * 2)
  {
    leds = i;
    refreshShiftRegister();
    delay(150);
  }
  */

  /* PATTERN 3 */
  int x = 1;
  for (int i = 0; i > -1; i = i + x)
  {
    if (i > 0)
    {
      bitClear   (leds, i-1);
      bitSet     (leds, i);
      refreshShiftRegister();
      delay      (150);
    }
    else
    {
      bitSet     (leds, i);
      refreshShiftRegister();
      delay      (150);
    }

    if (i == 5) x = -1;
  }
}






