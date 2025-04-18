void readSerial()
{
  uint8_t bytes_read = 0;
  if(Serial.hasRxError())
  {
    publish("ERR: RX ERROR");
    return;
  }
  if (Serial.hasOverrun())
  {
    publish("ERR: OVERRUN");
    return;    
  }
  while(Serial.available() && (bytes_read < 64))
  { 
    static int serial_case = 0;
    static bool alloc = true;    
    
    uint8_t c = Serial.read();
    bytes_read++;

    if (alloc)
    {
      serial_case = ((c >> 5) & 0b00000111);
      alloc = false;
      if ((c == END_BYTE) || (c == 42))
        serial_case = 8; //Default
    }        
  

    switch(serial_case){
      case 0:
        if(readVerbose(c)) alloc = true;
        break;

      case 1:
        if(readInterpret(c)) alloc = true;
        break;        

      case 2:
        if(setLEDs(c)) alloc = true;
        break;

      case 3:
        if(floatVerbose(c)) alloc = true;
        break;
      
      case 4:
        if(stateInfo(c)) alloc = true;
        break;

      case 6:
        if(dataLog(c)) alloc = true;
        break;
      
      case 7:
        if(relayToComputer(c)) alloc = true;
        break;   

      default:
        if(serialErrorHandle(c) || relayErrorHandle(c)) alloc = true;
    }
  }
}


bool readVerbose(uint8_t c)
{
  static char serial_packet[100];
  static uint8_t serial_buf_loc = 0;
  static uint8_t serial_len;
  static uint8_t readCase = 0;
  static bool error = false;

  // char nuff[50];
  // sprintf(nuff, "INFO: char = %d", int(c));
  // publish(nuff);
  if(serial_buf_loc  > 16)
  {
    if (!error) {
      purgeSerial();
      error = true;
    }
    else{
      if(serialErrorHandle(c))
        serial_buf_loc = 0;
        readCase = 0;
        error = false;      
      return true;
    }
    return false;
  }
  
  switch(readCase){
    case 0: 
      memset(serial_packet, 0, sizeof(serial_packet));
      sprintf(serial_packet, "VBS: ");
      readCase = 1;
      break;
    case 1:
      serial_len = c-1;
      readCase = 2;
      break;

    case 2:
      // serial_packet[serial_buf_loc+5] = c;
      if ((c == char(END_BYTE)) && (serial_buf_loc-1 == serial_len))
      {
        publish(serial_packet); 
        serial_buf_loc = 0;
        readCase = 0;
        return true;
      }
      // serial_packet[serial_buf_loc+5] = c;
      sprintf(serial_packet, "%s %x", serial_packet, c); 
      if (serial_buf_loc > serial_len)
      {
        if(serialErrorHandle(c))
        {
          publish("ERR: buf_loc > serial_len, vbs");
    
          char buff[50];
          sprintf(buff, "INFO: buf = %d, ser = %d", int(serial_buf_loc), int(serial_len));
          publish(buff);

          char duff[50];
          sprintf(duff, "INFO: packet = %s", serial_packet);  
          publish(duff);
          serial_buf_loc = 0;
          readCase = 0;
          return true;
        }
      }
      serial_buf_loc++;
      break;
  }
  return false;
}


bool setLEDs(uint8_t d)
{
  static uint8_t readCase = 0;
  static uint8_t LED = 0;
  static uint8_t byteCount = 0;
  static uint8_t serial_len;
  static uint8_t packet;

  uint8_t c = d;


  if(LED == 0)
  {
    LED = ((c & 0b00011000) >> 3);
    readCase = (c & 0b0000111);
    if (readCase > 3)
    {
      serial_len = 2;
    } else {
      serial_len = 1;
    }
  }


  switch(readCase){
    case 0:
      // publish("INFO: LED Toggle");
      if ((c == char(END_BYTE)) && (byteCount == serial_len))
      {
        if(LED == 1)
        {
          led_red.Toggle();
        } else if (LED == 2)
        {
          led_green.Toggle();
        } else {
          led_blue.Toggle();
        }
        byteCount = 0;
        readCase = 0;
        LED = 0;
        return true;        
      }
      break;

    case 1:
      // publish("INFO: LED Off");
      if ((c == 0b0001110) && (byteCount == serial_len))
      {
        if(LED == 1)
        {
          led_red.On(); //backwards, I know
        } else if (LED == 2)
        {
          led_green.On();
        } else {
          led_blue.On();
        }
        byteCount = 0;
        readCase = 0;
        LED = 0;
        return true;        
      }
      break;    
    
    case 2:
      // publish("INFO: LED On");
      if ((c == char(END_BYTE)) && (byteCount == serial_len))
      {
        if(LED == 1)
        {
          led_red.Off();  //not a type - just bad code
        } else if (LED == 2)
        {
          led_green.Off();
        } else {
          led_blue.Off();
        }
        byteCount = 0;
        readCase = 0;
        LED = 0;
        return true;        
      }
      break; 

    case 3:
      if ((c == char(END_BYTE)) && (byteCount == serial_len))
      {
        if(LED == 1)
        {
          led_red.Blink();
        } else if (LED == 2)
        {
          led_green.Blink();
        } else {
          led_blue.Blink();
        }
        byteCount = 0;
        readCase = 0;
        LED = 0;
        return true;        
      }      
    
    case 4:
      if ((c == char(END_BYTE)) && (byteCount == serial_len))
      {
        if(LED == 1)
        {
          if(!led_red.getBlinkFlag())
            led_red.Blink();
          led_red.setBlinkFreq(packet);
        } else if (LED == 2) {
          if(!led_green.getBlinkFlag())
            led_green.Blink();
          led_green.setBlinkFreq(packet);
        } else {
          if(!led_blue.getBlinkFlag())
            led_blue.Blink();
          led_blue.setBlinkFreq(packet);
        }
        byteCount = 0;
        readCase = 0;
        packet = 0;
        LED = 0;
        return true;        
      } else {
        packet = c;
        break;
      }

  }

  if (byteCount > serial_len)
  {
    if(serialErrorHandle(c))
    {    
      publish("ERR: buf_loc > serial_len, LED");
      
      char buff[50];
      sprintf(buff, "INFO: buf = %d, ser = %d", int(byteCount), int(serial_len));
      publish(buff);

      byteCount = 0;
      readCase = 0;
      return true;      
    }
  }

  byteCount++;
  return false;
}


bool floatVerbose(uint8_t c) {
  static char serial_packet[40];
  static uint8_t storage[4];
  static uint8_t count = 0;

  if (count == 0) {
    memset(serial_packet, 0, sizeof(serial_packet));
    count++;
  }
  else if (count >= 5) {
    if (c == char(END_BYTE)) {
      float angle;
      memcpy(&angle,storage,4);
      sprintf(serial_packet, "VBS: %.2f", angle);
      publish(serial_packet);
      memset(serial_packet, 0, sizeof(serial_packet));
      memset(storage, 0, sizeof(storage));
      count = 0;
      return true;      
    } else {
      if(serialErrorHandle(c)) {
        publish("ERR: Float Verbose overran");
        memset(serial_packet, 0, sizeof(serial_packet));               
        count = 0;
        return true;
      }    
    }
  } else {
    storage[count-1] = c;
    count++;
  }
  return false;
}


bool readInterpret(uint8_t c)
{
  static uint8_t serial_packet[100];
  static uint8_t serial_buf_loc = 0;
  static uint8_t serial_len;
  static uint8_t readCase = 0;
  
  switch(readCase){
    case 0: 
      memset(serial_packet, 0, sizeof(serial_packet));
      readCase = 1;
      break;
    case 1:
      serial_len = c;
      readCase = 2;
      break;

    case 2:
      serial_packet[serial_buf_loc] = c;   

      if ((c == char(END_BYTE)) && (serial_buf_loc == serial_len))
      {
        commands(serial_packet, (unsigned int) serial_len);
        serial_buf_loc = 0;
        readCase = 0;
        return true;
      }
      if (serial_buf_loc > serial_len)
      {
        if(serialErrorHandle(c))
        {
          publish("ERR: buf_loc > serial_len, readInterpret");
          
          char buff[50];
          sprintf(buff, "INFO: buf = %d, ser = %d", int(serial_buf_loc), int(serial_len));
          publish(buff);

          char tmp[100];
          memcpy(tmp, serial_packet, serial_len);

          char duff[50];
          sprintf(duff, "INFO: packet = %s", tmp);  
          publish(duff);

          serial_buf_loc = 0;
          readCase = 0;
          return true;      
        }
        break;
      }
      serial_buf_loc++;
      break;
  }
  return false;
}



bool stateInfo(uint8_t c)
{
  static uint8_t readCase = 0;
  static uint8_t state;
  static uint8_t count;
  static uint8_t storage[40];
  static bool alloc = true;
  static char serial_packet[100];

  if (alloc)
  {
      state = (c & 0b00011111);
      alloc = false;
      count = 1;
      return false;
  }  

  switch (state) {  //request
    case 0:
      if ((c == char(END_BYTE)) && (count == 3))
      {
        char error[42];
  // char buff[85];// = "PNG: 12345678901234567890123456789012";
  // sprintf(buff, "PNG: ");
  // for (int i = 4; i < len; i++)
  // {
  //   // if(((payload[i] == 0x00) && (i!=len-1)) || (payload[i] == 0xFF))
  //   // if((payload[i] == 0xFF))
  //   // {
  //   //   publish("PNG: Err");
  //   //   return;
  //   // }
  //   sprintf(buff, "%s%c", buff, payload[i]);
  // }
  // publish(buff);
        switch (int(storage[0])) {
          case 1:
            sprintf(error, "%s", "Neighbour Lost");
            break;

          case 2:
            sprintf(error, "%s", "Neighbour Told Me");
            break;

          case 3:
            sprintf(error, "%s", "Connection flag received out of order");
            break;

          case 4:
            sprintf(error, "%s", "Acknowledge flag received out of order");
            break;

          case 20:
            sprintf(error, "%s", "I2C failed when reading angle encorder");
            break;

          case 21:
            sprintf(error, "%s", "I2C failed when setting LEDs");
            break;

          case 22:
            sprintf(error, "%s", "I2C failed when reading accelerometer");
            break;         

          case 32:
            sprintf(error, "%s", "Manual trigger from button");
            break;   

          case 90:
            sprintf(error, "%s", "ESP received error instruction");
            break;         

          default:
            sprintf(error, "Unknown error code %d", int(storage[0]));
            break; 
        }

        sprintf(serial_packet, "FAIL: %s E%d", error, int(storage[1])); 
        publish(serial_packet);
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        return true;          
      } else if (count <= 2) {
        storage[count-1]=c;
        count++;
      } else {
        publish("ERR: Info Case 0");
        count = 0;
        alloc = true;
        return true;
      }
      break;

    case 1:
      if ((c == char(END_BYTE)) && (count == 2))
      {
        sprintf(serial_packet, "ERR: Coms123 Overflow, Edge %d", int(storage[0])); 
        publish(serial_packet);
        memset(serial_packet, 0, sizeof(serial_packet));
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        return true;          
      } else if (count == 1) {
        storage[0]=c;
        count++;
      } else {
        publish("ERR: Info Case 1");
        count = 0;
        alloc = true;
        return true;
      }
      break;



    case 12:
      if (c == char(END_BYTE))
      {    
        updateStableState(STATE_UNSTABLE);
        alloc = true;
        return true;
      } else {
        if(serialErrorHandle(c)) {
          alloc = true;
          return true;
        }
      }
      break;

    case 13:
      if (c == char(END_BYTE))
      {    
        updateStableState(STATE_STABLE);
        alloc = true;
        return true;
      } else {
        if(serialErrorHandle(c)) {
          alloc = true;
          return true;
        }
      }
      break;

    //Case 15-16 free 

    case 17:
      if ((c == char(END_BYTE)) && (count == 2))
      {
        clientLetter = char(storage[0]);
        sprintf(publishName, "%c/p", clientLetter);
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        return true;          
      } else if (count == 1) {
        storage[0]=c;
        count++;
      } else {
        publish("ERR: Info Case 17");
        count = 0;
        alloc = true;
        return true;
      }
      break;

    case 18:    //Neighbour Disconnected
      if ((c == char(END_BYTE)) && (count == 2))
      {
        sprintf(serial_packet, "REQ: NO %d", int(storage[0])); 
        publish(serial_packet);
        memset(serial_packet, 0, sizeof(serial_packet));
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        return true;          
      } else if (count == 1) {
        storage[0]=c;
        count++;
      } else {
        publish("ERR: Info Case 18");
        count = 0;
        alloc = true;
        return true;
      }
      break;

    case 19:    //ID
      publish("INFO: ID set");
      if (c == char(END_BYTE))
      {
          serial_write_id();
      }
      alloc = true;
      return true;
      break;

    case 20:  //read edges
      if ((c == char(END_BYTE)) && (count == 4))
      {
        sprintf(serial_packet, "REQ: EL %d %d %d", storage[0], storage[1], storage[2]);     
        publish(serial_packet);
        memset(serial_packet, 0, sizeof(serial_packet));
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        return true;
      } else if (count < 4) {
        storage[count-1]=c;
        count++;
      } else {
        publish("ERR: Info Case 20");
        count = 0;
        alloc = true;
        return true;
      }
      break;

    case 21:  //read angles
      if ((c == char(END_BYTE)) && (count == 13))
      {
        float angle1;
        float angle2;
        float angle3;
        memcpy(&angle1,storage,4);
        memcpy(&angle2,storage+4,4);
        memcpy(&angle3,storage+8,4);
        sprintf(serial_packet, "REQ: AN %.2f %.2f %.2f", angle1, angle2, angle3);
        publish(serial_packet);
        memset(serial_packet, 0, sizeof(serial_packet));
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        return true;
      } else if (count < 13) {
        storage[count-1]=c;
        count++;
      } else {
        publish("ERR: Info Case 21");
        count = 0;
        alloc = true;
        return true;
      }
      break;

    case 22:  //read orientation  
      if ((c == char(END_BYTE)) && (count == 7))
      {
        sprintf(serial_packet, "REQ: OR %d %d %d", (storage[0]*256+storage[1]), 
                      (storage[2]*256+storage[3]), (storage[4]*256+storage[5]));
        publish(serial_packet);
        memset(serial_packet, 0, sizeof(serial_packet));
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        return true;
      } else if (count < 7) {
        storage[count-1]=c;
        count++;
      } else {
        publish("ERR: Info Case 22");
        count = 0;
        alloc = true;
        return true;
      }
      break;

    case 23:  //read neighbour
      if ((c == char(END_BYTE)) && (count == 8))
      {
        sprintf(serial_packet, "REQ: NB %.2x%.2x%.2x%.2x%.2x%.2x%.2x", storage[0], storage[1], 
                            storage[2], storage[3], storage[4], storage[5], storage[6]);     
        publish(serial_packet);
        memset(serial_packet, 0, sizeof(serial_packet));
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        return true;
      } else if (count < 8) {
        storage[count-1]=c;
        count++;
      } else {
        publish("ERR: Info Case 23");
        count = 0;
        alloc = true;
        return true;
      }
      break;


    case 24:  //read WiFIEDGE
      if ((c == char(END_BYTE)) && (count == 2))
      {
        sprintf(serial_packet, "REQ: ED %d", int(storage[0])+1); 
        publish(serial_packet);
        memset(serial_packet, 0, sizeof(serial_packet));
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        return true;
      } else if (count == 1) {
        storage[0]=c;
        count++;
      } else {
        publish("ERR: Info Case 24");
        count = 0;
        alloc = true;
        return true;
      }
      break;

    case 25:   //WiFiEdge
      if (c == char(END_BYTE))
      {
        if(wifi_edge < 3)
        {
          serial_write_two(0b11011010, wifi_edge);
        } else {
          publish("REQ: WE"); //Request WiFi Edge
        }
      }
      alloc = true;
      return true;
      break;

    case 31:
      if (c == char(END_BYTE))
      {
        enableWifi();
        alloc = true;
        return true;
      } else {
        if(serialErrorHandle(c)) {
          alloc = true;
          return true;
        }
      }
      break;
    
    default:
      if(serialErrorHandle(c))
      {
        char error[42];
        sprintf(error, "ERR: Case %d", state);
        publish(error);
        memset(serial_packet, 0, sizeof(serial_packet));
        memset(storage, 0, sizeof(storage));        
        alloc = true;
        count = 0;        
        return true;
      }
  }
  return false;
}


bool dataLog(uint8_t c) 
{
  static bool alloc = true;
  static uint8_t count;
  static uint8_t len;
  static uint8_t storage[PACKET_SIZE];
  long time = 0;

  if (alloc) {
    alloc = false;
    storage[0] = (c & 0b00011111);
    len = dataLogCalcLen(storage[0]);
    time = millis();
    storage[1] = (byte) (time >> 24);
    storage[2] = (byte) (time >> 16);
    storage[3] = (byte) (time >> 8);
    storage[4] = (byte) time;
    count = 1;
    return false;
  }

   if (count < len) {
    storage[count+4]=c;
    count++;
  } else if ((c == END_BYTE) && (count == len)) {
    char buff[100];
    sprintf(buff, "DLG: ");
    for(int i=0; i<len+4; i++)  
    {
      sprintf(buff, "%s%.2x", buff, storage[i]);
    }
    publish(buff);
    memset(storage, 0, sizeof(storage));
    count = 0;
    alloc = true;
    len = 0;
    return true;
  } else {
    if(serialErrorHandle(c))
    {
      publish("ERR: Unable to Log");
      memset(storage, 0, sizeof(storage));               
      count = 0;
      alloc = true;
      len = 0;
      return true;
    }        
  }  
  return false;
}


uint8_t dataLogCalcLen(uint8_t alloc_byte)
{
  uint8_t len = 1; // alloc
  if(alloc_byte & 0b00000001) len += 12; //angles = 3*4
  // if(alloc_byte & 0b00000001) len += 6; //angles = 3*2
  if(alloc_byte & 0b00000010) len += 3; //edges = 3*1
  if(alloc_byte & 0b00000100) len += 12; //Orient = 3*2
  if(alloc_byte & 0b00001000) len += 12; //Velocity = 3*4
  if(alloc_byte & 0b00010000) len += 12; //Angle Floats = 3*4
  return len;
}

bool relayToComputer(uint8_t c)
{
  static uint8_t readCase = 0;
  static uint8_t state;
  static uint8_t count;
  static uint8_t len;
  static uint8_t storage[PACKET_SIZE];
  static bool alloc = true;
  static char wireless_packet[100];

  if (alloc)
  {
      state = (c & 0b00011111);
      alloc = false;
      count = 1;
      return false;
  } else if (len == 0)
  {
    len = c;
    count = 2;
    return false;
  }

  count++;

  switch (state) {  //request

    case 6:  //Relay to WiFi Edge
      if ((c == char(42)) && (count == len))
      {
        sprintf(wireless_packet, "REL: ");
        for(int i=0; i<len-3; i++)  
        {
          sprintf(wireless_packet, "%s%c", wireless_packet, storage[i]);
        }
        publish(wireless_packet);
        memset(wireless_packet, 0, sizeof(wireless_packet));
        memset(storage, 0, sizeof(storage));
        count = 0;
        alloc = true;
        len = 0;
        return true;
      } else if (count < len) {
        storage[count-2]=c;
      } else {
        if(relayErrorHandle(c))
        {
          publish("ERR: Unable to Relay");
          memset(wireless_packet, 0, sizeof(wireless_packet));
          memset(storage, 0, sizeof(storage));               
          count = 0;
          alloc = true;
          len = 0;
          return true;
        }        
        break;
      }
      break;
  }
  return false;
}


void purgeSerial()
{
  while(Serial.read() >= 0);
}


void updateStableState(bool current_stable_state)
{
  //saves previous 8 states (in case we need tracking for later, and also since a bool takes a byte's of memory anywya)
  stable_status = (stable_status << 1) & 0b11111110;
  stable_status = (stable_status | (current_stable_state & 0b00000001));
  if (((stable_status & 0b00000010) >> 1) != (stable_status & 0b00000001))
    publishStaticState();
}


bool serialErrorHandle(uint8_t c)
{
  publish("ERR: SER_ERR_HANDL");
  if(c == END_BYTE)
    return true;
  return false;
}

bool relayErrorHandle(uint8_t c)
{
  publish("ERR: REL_ERR_HANDL");
  if(c == 42)
    return true;
  return false;
}
