//----------------------- Recieved Message --------------------------//
void commands(uint8_t* payload, unsigned int len)
{
  for(int i=0; i<len; i++)
  {
    verbose_print((char)payload[i]);
  }

  char topic[3];
  for(int i=0; i < 3; i++)
  {
    topic[i] = (char)payload[i];
  }
  
  int sw_case = numCmds;
  for(int i=0; i < numCmds; i++)
  {
    if (!memcmp(topic, cmdLine[i], 3)) //4 is number of bytes in memory to compare (3 chars + stop)
    {
      sw_case = i;
      break;
    }
      
  }

  switch(sw_case){
    case 0:   //mac
      pubMac("MAC: ");
      pubIP();
      break;
    
    case 1:    //vg
      runState = 3;
      pubMac("MAC: ");
      pubIP();
      purgeSerial();
      verbose_println("Version Excellent");
      break;

    case 2:   //vb
      runState = 1;
      verbose_println("Version Bad");
      break;

    case 3:   //stopudp
      runState = 3;
      verbose_println("UDP communication paused");
      break;

    case 4:   //hello
      publish("INFO: Hello!");
      break;

    case 5:   //g_shape (set shape)
      pubShape();
      break;

    case 6:   //udp
      verbose_println("Recieved UDP Command");
      runState = 4;
      break;

    case 7:   //noudp
      verbose_println("Stop UDP Operation");
      runState = 6;
      break;

    case 8:   //verb
      verbose_flag = true;
      verbose_println("Starting Verbose Operation");
      break;

    case 9:   //noverb
      verbose_flag = false;
      verbose_println("Stopping Verbose Operation");
      break;

    case 10:  //shape (set shape)
      relay(payload, len);
      break;

    case 11:  //cont
      runState = 7;
      verbose_println("Starting Controller");
      break;

    case 12:  //nocon      
      runState = 9;
      verbose_println("Starting Controller");
      break;

    case 13:  //rled
      led_red.Blink();
      break;

    case 14:  //gled
      led_green.Blink();
      break;

    case 15:  //bled
      led_blue.Blink();
      break;

    case 16:  //wedge
      setWifiEdge(payload, len);
      break;

    case 17:  //rshape
      requestShape();
      break;

    case 18:  //redge
      requestEdge();
      break;

    case 19:  //rang",
      requestAngle();
      break;

    case 20:  //rorient
      requestOrientation();
      break;

    case 21:  //rwedge
      requestWifiEdge();
      break;      

    case 22:  //rneigh
      requestNeighbour(payload, len);
      break;

    case 23:
      disableWifi();
      break;

    case 24:
      runState = 12;
      break;

    case 25:
      ESP.reset();
      break;

    case 26:  //Restart Pic
      serial_write_one(RUN_PIC_RESTART);
      break;

    case 27: //png
      // publish("INFO: Ping received by ESP, command understood!"); // debugging
      echoPing(payload, len);
      break;

    case 28: //LED
      setPicLED(payload, len);
      break;      

    case 29: //Couplings
      openPicCouplings(payload, len);
      break;      

    case 30: //Edges
      setPicEdges(payload, len);
      break;      

    case 31: //Angles
      setPicAngles(payload, len);
      break;                                    

    case 32: //Party
      PARTYMODE();
      break;          

    case 33: //Set Hub
      setAsHub();
      break;

    case 34: //Disable Hub
      stopHub();
      break;                

    case 35: //flag
      enableRotMotors();
      break;  

    case 36: //flag
      disbleRotMotors();
      break;

    case 37: //flag
      enableLinMotors();
      break;

    case 38: //flag
      disbleLinMotors();
      break;

    case 39: //flag
      enableFlag1();
      break;

    case 40: //flag
      disableFlag1();
      break;

    case 41: //flag
      enableFlag2();
      break;

    case 42: //flag
      disableFlag2();
      break;      

    case 43: //flag
      enableFlag3();
      break;

    case 44: //flag
      disableFlag3();
      break;      

    case 45: //flag
      enableFlag4();
      break;

    case 46: //flag
      disableFlag4();
      break;      

    case 47: //flag
      enableFlag5();
      break;

    case 48: //flag
      disableFlag5();
      break;      

    case 49:
      wiggleCoupling(payload, len);
      break;      

    case 50:
      driveAndCouple(payload, len);
      break;

    case 51:
      setDatalogFlag(payload, len);
      break;      

    case 52:
      setDatalogPeriod(payload, len);
      break;         

    case 53:
      stopParty();
      break;

    case 54:
      pubName();
      break;

    case 55:  //Angular speed
      setPicSpeed(payload, len);      
      break;

    case 56:
      sendEmergencyStop();  
      break;

    case 57:  //torque
      setRotTorque(payload, len);  
      break;

    case 58:  //rer
      requestErrorCode();  
      break;      

    case 59:  //rer
      setPIDdeadband(payload, len);  
      break;

    case 60:  //set pid gains
      setPIDgains(payload, len);  
      break;     

    case 61:  //ksp
      setSpeedGain(payload, len);  
      break;     

    case 62:  //rok  - stable range
      setOKRange(payload, len);  
      break;           

    case 63:
      setVelocityCommand(payload, len);
      break;

    case 64:
      setKalmanNoise(payload, len);
      break;

    case 65:
      setPicWheels(payload, len);
      break;

    case 66:
      permIdleMori();
      break;

    case 67:
      setSensorFusionRelayDistance(payload, len);
      break;

    default:
      publish("ERR: Command not understood");
  }

  verbose_println("-----------------------");
}


void setWifiEdge(uint8_t* payload, unsigned int len)
{
  int byte_count = 0;

  byte_count = detectChar(payload, byte_count, len, " ");
  if(byte_count > len){
    publish("ERR: payload has no space");
    return;
  }
  byte_count++;

  uint8_t tmp_wifi_edge = payload[byte_count]-48-1;   //-48 = ascii to int conversion

  if (tmp_wifi_edge > 3) {
    publish("ERR: Wifi edge > 4");
    return;
  }
  if (tmp_wifi_edge == 3){
    setAsHub();
    return;
  }
  else if (wifi_edge == 3){ //tmp_hub <= 2
    stopHub();
  }

  wifi_edge = tmp_wifi_edge;

  char buff[50];
  sprintf(buff, "INFO: WiFi Edge set to %d", int(wifi_edge)+1);
  publish(buff);

  serial_write_two(SET_CMD_WEDGE, wifi_edge);
}


// Depricated
void requestShape()
{
  serial_write_one(REQ_CMD_SHAPE);
}


void requestEdge()
{
  serial_write_one(REQ_CMD_EDGS);
}


void requestAngle()
{
  serial_write_one(REQ_CMD_ANGS);
}


void requestOrientation()
{
  serial_write_one(REQ_CMD_ORNT);
}


void requestWifiEdge()
{
  serial_write_one(REQ_CMD_WEDGE);
}

void requestErrorCode()
{
  serial_write_one(REQ_CMD_ERROR);
}


void requestNeighbour(uint8_t* payload, unsigned int len)
{
  int byte_count = 0;

  byte_count = detectChar(payload, byte_count, len, " ");
  if(byte_count > len){
    publish("ERR: payload has no space");
    return;
  }
  byte_count++;

  uint8_t neighbour = payload[byte_count]-48;   //-48 = ascii to int conversion

  serial_write_two(REQ_CMD_NBR, neighbour);
}


void disableWifi()
{
  if(wifi_edge < 3)
  {
    publish("WIFI: Off");
    runState = 10;
  } else {
    publish("ERR: WiFi edge not set");
  }
  
}


void echoPing(uint8_t* payload, unsigned int len)
{
  // Echo the message back to the external computer
  char buff[85];// = "PNG: 12345678901234567890123456789012";
  sprintf(buff, "PNG: ");
  for (int i = 4; i < len; i++)
  {
    // if(((payload[i] == 0x00) && (i!=len-1)) || (payload[i] == 0xFF))
    // if((payload[i] == 0xFF))
    // {
    //   publish("PNG: Err");
    //   return;
    // }
    sprintf(buff, "%s%c", buff, payload[i]);
  }
  publish(buff);
}


void openPicCouplings(uint8_t* payload, unsigned int len)
{
  setCouplingShapeCommand(payload, len, SHAPE_CMD_COUP_MASK, SHAPE_CMD_COUP_ALLOC);
}


void driveAndCouple(uint8_t *payload, unsigned int len)
{
  setCouplingShapeCommand(payload, len, SHAPE_CMD_DRVE_MASK, SHAPE_CMD_DRVE_ALLOC);
}


void setCouplingShapeCommand(uint8_t *payload, unsigned int len, uint8_t alloc_mask, uint8_t alloc)
{
  char tmp_payload[5];
  
  uint8_t byte_count = detectChar(payload, byte_count, len, " ");
  if(byte_count > len) {
    byte_count = len;
  }
  byte_count++;
  tmp_payload[0] = payload[byte_count];
  tmp_payload[1] = '\0';
  
  uint8_t value = byte(atoi(tmp_payload))-1;

  if(value > 3) {
    publish("ERR: Can\'t open coupling > 3");
  }

  alloc = alloc | (alloc_mask & value);

  char buff[50];
  sprintf(buff, "INFO: Opening coupling %d", value+1);
  publish(buff);  

  write_to_buffer(SHAPE_COMMAND_ALLOC); //205
  write_to_buffer(alloc);
  write_to_buffer(END_BYTE);
  return;  
}


void setPicLED(uint8_t *payload, unsigned int len)
{
  setShapeCommand(payload, len, SHAPE_CMD_LEDS_MASK, SHAPE_CMD_LEDS_ALLOC, false);
}


void setPicEdges(uint8_t *payload, unsigned int len)
{
  setShapeCommand(payload, len, SHAPE_CMD_EDGS_MASK, SHAPE_CMD_EDGS_ALLOC, false);
}


void setPicSpeed(uint8_t *payload, unsigned int len)
{
  setShapeCommand(payload, len, SHAPE_CMD_SPDS_MASK, SHAPE_CMD_SPDS_ALLOC, false);
}


void setPicAngles(uint8_t *payload, unsigned int len)
{
  setShapeCommand(payload, len, SHAPE_CMD_ANGS_MASK, SHAPE_CMD_ANGS_ALLOC, true);
}


void setPicWheels(uint8_t *payload, unsigned int len)
{
  setShapeCommand(payload, len, SHAPE_CMD_WEEL_MASK, SHAPE_CMD_WEEL_ALLOC, false);
}

void setShapeCommand(uint8_t *payload, unsigned int len, uint8_t alloc_mask, 
          uint8_t alloc, bool flg_two_bytes)
{
  uint8_t num_following = 0;
  uint8_t values[6];
  uint8_t cmd_id = 0;
  
  // Ext and ang commands mask is 0b00000000
  if((alloc & 0b11000000) == 0){
    bool successFlag = true;
    cmd_id = extractCommandID(payload, len, &successFlag);
    if ((successFlag == false)) 
      return;
  }

  if(!extractValuesForShape(payload, len, alloc_mask, &alloc, &num_following, values, flg_two_bytes)) {
    return;
  }

  write_to_buffer(SHAPE_COMMAND_ALLOC); //205
  write_to_buffer(alloc);
  for(uint8_t i=0; i< num_following; i++) {
    write_to_buffer(values[i]);
  }
  if((alloc & 0b11000000) == 0) {
    write_to_buffer(cmd_id);
  }
  write_to_buffer(END_BYTE);
}


uint8_t extractCommandID(uint8_t* payload, unsigned int len, bool *successFlag) 
{  
  uint8_t byte_count = 0;
  byte_count = detectChar(payload, byte_count, len, "/");
  if(byte_count > len){
    *successFlag = false;
    publish("ERR: payload has no slash");
    return 0;
  }
  uint8_t cmd_id = (byte)extractFollowingNumber(payload+byte_count, len-byte_count, successFlag, "/");
  if (cmd_id == 0) {
    *successFlag = false;
    publish("ERR: no number following");
  }

  return cmd_id;
}


//Used in setPicEdges setPicLED, and setPicAngles
bool extractValuesForShape(uint8_t* payload, unsigned int len, uint8_t alloc_mask, 
          uint8_t *alloc, uint8_t *num_following, uint8_t *values, bool flg_two_bytes)
{
  uint8_t byte_count = 0;
  uint8_t pre_count = 0;
  char tmp_payload[5];

  byte_count = detectChar(payload, byte_count, len, " ");
  byte_count++;
  for(uint8_t i=0; i<3; i++){ //For the 3 Edges
    bool jumpFlag = false;
    pre_count = byte_count; //count between whitespace ("led 255" -> "255", len 3)
    
    byte_count = detectChar(payload, byte_count, len, " ");

    if(byte_count > len) { // no space at the end of the command (e.g., "led 10 10 10")
      byte_count = len;
    }

    if(byte_count >= len && i < 2) { // no space at the end of the command (e.g., "led 10 10 10")
      publish("ERR: No values following");
      return false;
    }

    if(flg_two_bytes){
      if(byte_count > pre_count + 4) {
        publish("ERR: Angle value > 4 chars");
        return false;
      }
    } else {
      if(byte_count > pre_count + 3) {
        publish("ERR: Shape value > 3 chars");
        return false;
      }
    }

    uint8_t j;
    for(j=0; j<byte_count-pre_count; j++) {
      if((payload[pre_count+j] < 48) || (payload[pre_count+j] > 57)) {  //not ascii number - byte(48) = "0"
        jumpFlag = true;
      }
      tmp_payload[j] = char(payload[pre_count+j]);
    }
    if(jumpFlag) {
      byte_count++;
      continue;
    }

    tmp_payload[j] = '\0';
    if(flg_two_bytes){
      int tmp_val = atoi(tmp_payload);
      values[*num_following] = highByte(tmp_val);
      values[*num_following + 1] = lowByte(tmp_val);
      if(tmp_val > 3600) { //skips loop if value too high
        byte_count++;
        continue;
      }
      *num_following = *num_following + 2;
    } else {
      values[*num_following] = byte(atoi(tmp_payload));
      if(values[*num_following] > 255) {  
        byte_count++;
        continue;
      }
      *num_following = *num_following + 1;
    }

    *alloc = *alloc | (alloc_mask  >> i);
    byte_count++;
  }
  return true;
}


uint8_t detectChar(uint8_t* payload, int byte_count, unsigned int max_len, const char *ascii_char)
{
  while(payload[byte_count] != *ascii_char)   //0b00101111 = /
  {
    byte_count++;
    if(byte_count > max_len) {
      break;
    }
  }
  return byte_count;
}


void PARTYMODE()
{
  led_red.setBlinkFreq(random(10, 255));
  if(!led_red.getBlinkFlag())
    led_red.Blink();
  led_blue.setBlinkFreq(random(10, 255));
  if(!led_blue.getBlinkFlag())  
    led_blue.Blink();
  led_green.setBlinkFreq(random(10, 255));
  if(!led_green.getBlinkFlag())  
    led_green.Blink();
  serial_write_one(SET_PARTY_HYPE);
}

void stopParty()
{
  led_red.On();
  led_green.On();
  led_blue.On();
  serial_write_one(ITS_THE_POPO);
}

void setAsHub()
{
  wifi_edge = 3;
  serial_write_two(SET_CMD_WEDGE, wifi_edge);
  publish("HUB: On");
  if((runState != 3) && (runState != 5)){
    runState = 4;
    flag_control = false;
  }
}

void stopHub()
{
  publish("HUB: Off");
}


void enableRotMotors() {
  serial_write_flags(FLG_CMD_ENBL_ROTM);
  return;
}

void disbleRotMotors() {
  serial_write_flags(FLG_CMD_DSBL_ROTM);
  return;
}

void enableLinMotors() {
  serial_write_flags(FLG_CMD_ENBL_LINM);
  return;
}

void disbleLinMotors() {
  serial_write_flags(FLG_CMD_DSBL_LINM);
  return;
}

void enableFlag1() {
  serial_write_flags(FLG_CMD_ENBL_FLG1);
  return;
}

void disableFlag1() {
  serial_write_flags(FLG_CMD_DSBL_FLG1);
  return;
}

void enableFlag2() {
  serial_write_flags(FLG_CMD_ENBL_FLG2);
  return;
}

void disableFlag2() {
  serial_write_flags(FLG_CMD_DSBL_FLG2);
  return;
}

void enableFlag3() {
  serial_write_flags(FLG_CMD_ENBL_FLG3);
  return;
}

void disableFlag3() {
  serial_write_flags(FLG_CMD_DSBL_FLG3);
  return;
}

void enableFlag4() {
  serial_write_flags(FLG_CMD_ENBL_FLG4);
  return;
}

void disableFlag4() {
  serial_write_flags(FLG_CMD_DSBL_FLG4);
  return;
}

void enableFlag5() {
  serial_write_flags(FLG_CMD_ENBL_FLG5);
  return;
}

void disableFlag5() {
  serial_write_flags(FLG_CMD_DSBL_FLG5);
  return;
}

void wiggleCoupling(uint8_t* payload, unsigned int len)
{
  int byte_count = 0;

  byte_count = detectChar(payload, byte_count, len, " ");
  if(byte_count > len){
    publish("ERR: payload has no space");
    return;
  }
  byte_count++;

  uint8_t coupling = payload[byte_count]-48-1;   //-48 == ascii to int conversion

  if (coupling > 3) { //0-2 == couplings, 3 == all couplings
    publish("ERR: Wifi edge > 4");
    return;
  } 

  serial_write_two(RUN_COUP_WIGGLE, coupling);
}


void setDatalogFlag(uint8_t* payload, unsigned int len) 
{
  bool successFlag = true;
  uint8_t flag = (uint8_t)extractFollowingNumber(payload, len, &successFlag, " ");
  if(successFlag)
    serial_write_two(DLG_CMD_FLAG_SET, flag);
}


void setDatalogPeriod(uint8_t* payload, unsigned int len) 
{
  int byte_count = 0;
  byte_count = detectChar(payload, byte_count, len, " ");
  byte_count += 1;
  char request = payload[byte_count];
  bool successFlag = true;
  int tmp = extractFollowingNumber(payload+byte_count, len-byte_count, &successFlag, " ");
  uint8_t freq = (uint8_t)(tmp/10);
  if(successFlag)
  {
    char r_data = 0;
    if(request == 'a')
      r_data = 0b00000001;
    if(request == 'e')
      r_data = 0b00000010;
    if(request == 'o')
      r_data = 0b00000100;
    if(request == 'v')
      r_data = 0b00001000;
    if(request == 'f')
      r_data = 0b00010000;    
    
    char buff[50];
    sprintf(buff, "INFO: DLP %d %d", r_data, freq);
    publish(buff);  

    serial_write_three(DLG_CMD_PERD_SET, r_data, freq);
  }
}


int extractFollowingNumber(uint8_t* payload, unsigned int len, bool *successFlag, const char *character)
{
  int byte_count = 0;
  char tmp_payload[5];

  byte_count = detectChar(payload, byte_count, len, character);
  if(byte_count > len){
    *successFlag = false;
    publish("ERR: payload has no space");
    return 0;
  }
  byte_count++;
  int pre_count = byte_count; //count between whitespace ("led 255" -> "255", len 3)
  byte_count = detectChar(payload, byte_count, len, " ");

  if(byte_count > len) { // no space at the end of the command (e.g., "led 10 10 10")
    byte_count = len;
  }

  uint8_t j;
  for(j=0; j<byte_count-pre_count; j++) {
    if((payload[pre_count+j] < 48) || (payload[pre_count+j] > 57)) {  //not ascii number - byte(48) = "0"
      *successFlag = false;
      publish("ERR: Number not ascii");
      return 0;
    }
    tmp_payload[j] = char(payload[pre_count+j]);
  }
  tmp_payload[j] = '\0';
  return atoi(tmp_payload);
}

void sendEmergencyStop() {
  serial_write_one(SET_EMG_STOP);
  publish("ERR: STOPPING");
  enableWifi();
}

void setRotTorque(uint8_t* payload, unsigned int len) {
  uint8_t alloc2 = 0;
  uint8_t alloc_mask = SET_CMD_TORQUE_MASK;
  uint8_t num_following = 0;
  uint8_t values[6];
  
  if(!extractValuesForShape(payload, len, alloc_mask, &alloc2, &num_following, values, false)) {
    return;
  }

  write_to_buffer(SET_CMD_TORQUE_ALLOC);
  write_to_buffer(alloc2);
  for(uint8_t i=0; i< num_following; i++)
  {
    write_to_buffer(values[i]);
  }
  write_to_buffer(END_BYTE);
}


void setPIDdeadband(uint8_t* payload, unsigned int len) {
  bool successFlag = true;
  uint8_t deadband = (uint8_t)extractFollowingNumber(payload, len, &successFlag, " ");
  if(successFlag){
    serial_write_two(SET_CMD_DEADBND, deadband);
    char buff[50];
    sprintf(buff, "INFO: Deadband set to +/- %.1f", float(deadband)/10.0f);
    publish(buff);
  }
}


void setPIDgains(uint8_t* payload, unsigned int len) {
  uint8_t alloc2 = 0;
  uint8_t alloc_mask = SET_CMD_PID_MASK;
  uint8_t num_following = 0;
  uint8_t values[6];
  
  if(!extractValuesForShape(payload, len, alloc_mask, &alloc2, &num_following, values, false)) {
    return;
  }

  write_to_buffer(SET_CMD_PID_ALLOC);
  write_to_buffer(alloc2);
  for(uint8_t i=0; i< num_following; i++)
  {
    write_to_buffer(values[i]);
  }
  write_to_buffer(END_BYTE);
}


void setSpeedGain(uint8_t* payload, unsigned int len) {
  bool successFlag = true;
  uint8_t speedgain = (uint8_t)extractFollowingNumber(payload, len, &successFlag, " ");
  if(successFlag){
    serial_write_two(SET_CMD_SPDGAIN, speedgain);
    char buff[50];
    sprintf(buff, "INFO: Speed gain set to %d", int(speedgain));
    publish(buff);
  }
}


void setOKRange(uint8_t* payload, unsigned int len) {
  bool successFlag = true;
  uint8_t okRange = (uint8_t)extractFollowingNumber(payload, len, &successFlag, " ");
  if(successFlag){
    serial_write_two(SET_CMD_OKRNGE, okRange);
    char buff[50];
    sprintf(buff, "INFO: Stable range set to +/- %.1f", float(okRange)/10.0f);
    publish(buff);
  }
}

void setVelocityCommand(uint8_t *payload, unsigned int len)
{
  long unsigned current_time = millis();
  if(current_time - last_vel_cmd < 50)
  {
    return;
  }
  last_vel_cmd = current_time;

  int byte_count = 0;
  uint8_t alloc = 0b00000000;
  float storage[3] = {0,0,0};


  byte_count = detectChar(payload, byte_count, len, " ");
  if(byte_count > len){
    publish("ERR: payload has no space");
    return;
  }
  byte_count++;


  for(uint8_t i=0; i<3; i++) {
    if((const char) payload[byte_count] != 0b00101101) // 00101101 = "-"
    {
      float tmp_storage;
      uint8_t *ptr = ( uint8_t* ) &tmp_storage;
      for(uint8_t count=0; count<sizeof(float); count++){
        *(ptr+count) = payload[byte_count];
        byte_count++;
        if(byte_count > len){
          publish("ERR: payload has no space");
          return;
        }
      }
      storage[i] = tmp_storage;
      alloc = alloc | (1 << i);
    } else {
      byte_count++;
    }
    byte_count++;
  }

  write_to_buffer(SET_CMD_VEL_ALLOC); //205
  write_to_buffer(alloc);

  for(uint8_t i=0; i<3; i++) {
    if ((alloc >> i) & 0b00000001) {
      float tmp_storage = storage[i];
      char *c_Data = ( char* ) &tmp_storage;
      for( char c_Index = 0 ; c_Index < sizeof( float ) ; write_to_buffer( c_Data[ c_Index++ ] ) );
    }
  }
  write_to_buffer(END_BYTE);
  
}


void setKalmanNoise(uint8_t *payload, unsigned int len)
{
  int byte_count = 0;
  uint8_t alloc = 0b00000000;
  float storage[3] = {0,0,0};


  byte_count = detectChar(payload, byte_count, len, " ");
  if(byte_count > len){
    publish("ERR: payload has no space");
    return;
  }
  byte_count++;


  for(uint8_t i=0; i<3; i++) {
    if((const char) payload[byte_count] != 0b00101101) // 00101101 = "-"
    {
      float tmp_storage;
      uint8_t *ptr = ( uint8_t* ) &tmp_storage;
      for(uint8_t count=0; count<sizeof(float); count++){
        *(ptr+count) = payload[byte_count];
        byte_count++;
        if(byte_count > len){
          publish("ERR: payload has no space");
          return;
        }
      }
      storage[i] = tmp_storage;
      alloc = alloc | (1 << i);
    } else {
      byte_count++;
    }
    byte_count++;
  }

  write_to_buffer(SET_CMD_KAL_ALLOC); //205
  write_to_buffer(alloc);

  for(uint8_t i=0; i<3; i++) {
      float tmp_storage = storage[i];
      char *c_Data = ( char* ) &tmp_storage;
      for( char c_Index = 0 ; c_Index < sizeof( float ) ; write_to_buffer( c_Data[ c_Index++ ] ) );
  }
  write_to_buffer(END_BYTE);

  char buff[50];
  sprintf(buff, "INFO: kNoise set to %f, %f, %f", storage[0], storage[1], storage[2]);
  publish(buff);
  
}



void permIdleMori() {
  serial_write_one(RUN_PIC_IDLE);
  update_tx_buffer();
  serial_write_one(RUN_PIC_IDLE);
  update_tx_buffer();
  serial_write_one(RUN_PIC_IDLE);
  update_tx_buffer();
  publish("INFO: Goodnight!");
  if (client.connected()) {
    client.loop();
  }
  ESP.deepSleep(0); // 20e8 = 2000 seconds
}


void setSensorFusionRelayDistance(uint8_t* payload, unsigned int len) {
  bool successFlag = true;
  uint8_t distance = (uint8_t)extractFollowingNumber(payload, len, &successFlag, " ");
  if(successFlag){
    serial_write_two(SET_CMD_REL_DIST, distance);
    char buff[50];
    sprintf(buff, "INFO: Speed gain set to %d", int(distance));
    publish(buff);
  } else {
    publish("ERR: Unable to set relay distance");
  }
}