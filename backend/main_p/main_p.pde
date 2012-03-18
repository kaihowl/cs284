
String getCommand ();
String processCommand (String command);
String getReading (String type);


void setup()
{
  // start the serial interface
  Serial.begin(9600);
}

void loop()
{
  // wait until a serial input is available
  while (Serial.available() == 0);
  
  // read a command from the serial port
  String Str = getCommand();

  // print the command to the serial interface
  Serial.println(Str);

}

// When a serial input is available this function will
// read the serial interface. It will interpret it as
// a string that terminates with a semi-colon.
String getCommand ()
{
  String s = "";
  
  while (Serial.peek() != ';'){
    s = s + char(Serial.read());
    while (Serial.available() == 0);
  }
  s = s + char(Serial.read());

  String ret = processCommand (s);
  return ret;
}

String processCommand (String command)
{
  String ret = "";
  switch (command.charAt(0)) {
    // a GET command
    case 'G':
      ret = getReading ("dummy");
      break;
    default:
      ret = "ERROR";
      break;
  }
      
  return ret+":"+command.substring(1);
  
}

String getReading (String type)
{
 // we assume  
 String ret = "";
 ret = random (40, 45);
 return ret;
}
