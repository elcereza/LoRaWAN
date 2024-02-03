#include "LoRaWAN_Radioenge.h"

String LoRaWAN_Radioenge::feedbackSerial(String val, bool exception){
  String buff = "";
  uint8_t count = 8;
  
  SerialLoRaWAN->println(val);
  if(feedback)
    Serial.println("TX: " + val);
  
  uint16_t timerout_count = 0;
  while(true){
    if(SerialLoRaWAN->available() > 0){
      buff = SerialLoRaWAN->readString();
      if(feedback)
        Serial.println("RX: " + buff);
      
      buff.replace("\n", "");
      buff.replace("\r", "");
  
      if(!exception){
        if(buff.indexOf("ERRO") > 0)
          return "";
        break;
      }
      else{         
        if(buff.indexOf("ERRO") > 0 && count > 0)
          count -= 1;
        else if(count <= 0)
          break;
        else if(buff == "AT_JOIN_OK" || buff == "AT_ALREADY_JOINED")
          break;
      }
    }
    else if(!exception){
        if(timerout_count < 1000){
          ++timerout_count;
          delay(1);
        }
        else
          break;
    }
  }
  return buff;
}

String LoRaWAN_Radioenge::commandAT(uint8_t cmd, String val, bool exception){
  String AT = "AT+";
  
  if(!exception && val == "")
    AT = AT + AT_CMD[cmd] + "=?";
  else if(exception)
    AT = AT + AT_CMD[cmd];
  else
    AT = AT + AT_CMD[cmd] + "=" + val;
  delay(50);
  return feedbackSerial(AT, exception);
}

String LoRaWAN_Radioenge::bool_to_intString(uint8_t val){
  if(val)
    return "1";
  return "0";
}

void LoRaWAN_Radioenge::deserializeAT(uint8_t cmd){
  String val_buff = commandAT(cmd);
  char val_char[val_buff.length()];
  val_buff.toCharArray(val_char, val_buff.length());
    
  uint8_t count = 0;  
  for(uint8_t i = 0; i < val_buff.length(); ++i){
    if(val_char[i] != ':')
      payloads[count] += val_char[i];
    else
      ++count;
  }
}  

String LoRaWAN_Radioenge::separator(String val){
  uint8_t val_size = val.length();
  char val_char[val_size];
  val.toCharArray(val_char, val_size + 1);
  
  if(val_size % 2 == 0 && val.indexOf(":") < 1){
    val = "";
    uint8_t count = 0;
    for(uint8_t i = 0; i < val_size; ++i){
      
      val += val_char[i];
      ++count;
      if(count >= 2 && i + 1 < val_size){
        val += ":";
        count = 0;       
      }
    }
  }
  return val;
} 

uint16_t LoRaWAN_Radioenge::GPIO(uint8_t cmd, uint8_t pin, uint8_t val){
  String buff = "";
  
  if(val != 2)
    buff = commandAT(cmd, String(pin) + ":" + String(val));
  else
    buff = commandAT(cmd, String(pin));        
  buff.remove(0, 2);   
  buff.replace("AT_OK", "");
  buff.replace("AT_ERROR", String(0xFFFF));
  return (uint16_t)buff.toInt();
}


LoRaWAN_Radioenge::LoRaWAN_Radioenge(Stream* _SerialLoRaWAN){
  SerialLoRaWAN = _SerialLoRaWAN;
}

void LoRaWAN_Radioenge::printParameters(){
  String version = VER();
  
  Serial.println("---------------------------------------------------");
  Serial.println("                  LoRaWAN Radioenge\n");
  Serial.println(" Version        = " + version);
  Serial.println(" DevEui         = " + _DEUI);
  Serial.println(" DevAddr        = " + _DADDR);
  Serial.println(" AppKey         = " + _APPKEY);
  Serial.println(" AppSKey        = " + _APPSKEY);
  Serial.println(" NwkSKey        = " + _NWKSKEY);
  Serial.println(" AppEui/JoinEui = " + _APPEUI + "\n");
  Serial.println("                    elcereza.com");
  Serial.println("--------------------------------------------------\n");
}

void LoRaWAN_Radioenge::begin(bool _feedback){
  feedback = _feedback;      
  
  DEUI();
  DADDR();
  APPKEY();
  APPSKEY();
  NWKSKEY();
  APPEUI();
}

String LoRaWAN_Radioenge::DADDR(String val){
  if(val != "") commandAT(_DADDR_, separator(val));
  _DADDR = commandAT(_DADDR_);
  return _DADDR;
}

String LoRaWAN_Radioenge::APPKEY(String val){
  if(val != "") commandAT(_APPKEY_, separator(val));
  _APPKEY = commandAT(_APPKEY_);
  return _APPKEY;
}

String LoRaWAN_Radioenge::APPSKEY(String val){
  if(val != "") commandAT(_APPSKEY_, separator(val));
  _APPSKEY = commandAT(_APPSKEY_);
  return _APPSKEY;
}

String LoRaWAN_Radioenge::NWKSKEY(String val){
  if(val != "") commandAT(_NWKSKEY_, separator(val));
  _NWKSKEY = commandAT(_NWKSKEY_);
  return _NWKSKEY;
}

String LoRaWAN_Radioenge::APPEUI(String val){
  if(val != "") commandAT(_APPEUI_, separator(val));
  _APPEUI = commandAT(_APPEUI_);
  return _APPEUI;
}

String LoRaWAN_Radioenge::DEUI(){
  _DEUI = commandAT(_DEUI_);
  return _DEUI;
}

String LoRaWAN_Radioenge::CHMASK(String val){
  if(val != "") commandAT(_CHMASK_, separator(val));
  return commandAT(_CHMASK_);
}

void LoRaWAN_Radioenge::ATZ(){
  feedbackSerial("ATZ");      
}   

bool LoRaWAN_Radioenge::ADR(uint8_t val){
  if(val != 0xFF) commandAT(_ADR_, bool_to_intString(val));
  return commandAT(_ADR_).toInt();
}

uint8_t LoRaWAN_Radioenge::DR(uint8_t val){
  if(val < 14) (uint8_t)commandAT(_DR_, String(val)).toInt();
  return (uint8_t)commandAT(_DR_).toInt();
}

bool LoRaWAN_Radioenge::DCS(uint8_t val){
  if(val != 0xFF) commandAT(_DCS_, bool_to_intString(val));
  return commandAT(_DCS_).toInt();
}

bool LoRaWAN_Radioenge::PNM(uint8_t val){
  if(val != 0xFF) commandAT(_PNM_, bool_to_intString(val));
  return commandAT(_PNM_).toInt();
}

uint32_t LoRaWAN_Radioenge::RX2FQ(uint32_t val){
  if(val != 0xFF) commandAT(_RX2FQ_, String(val));
  return (uint32_t)commandAT(_RX2FQ_).toInt();
}

uint16_t LoRaWAN_Radioenge::RX2DR(uint16_t val){
  if(val != 0xFF) commandAT(_RX2DR_, String(val));
  return (uint16_t)commandAT(_RX2DR_).toInt();
}

uint16_t LoRaWAN_Radioenge::RX1DL(uint16_t val){
  if(val != 0xFF) commandAT(_RX1DL_, String(val));
  return (uint16_t)commandAT(_RX1DL_).toInt();
}

uint16_t LoRaWAN_Radioenge::RX2DL(uint16_t val){
  if(val != 0xFF) commandAT(_RX2DL_, String(val));
  return (uint16_t)commandAT(_RX2DL_).toInt();
}

uint16_t LoRaWAN_Radioenge::JN1DL(uint16_t val){
  if(val != 0xFF) commandAT(_JN1DL_, String(val));
  return (uint16_t)commandAT(_JN1DL_).toInt();
}

uint16_t LoRaWAN_Radioenge::JN2DL(uint16_t val){
  if(val != 0xFF) commandAT(_JN2DL_, String(val));
  return (uint16_t)commandAT(_JN2DL_).toInt();
}

bool LoRaWAN_Radioenge::NJM(uint8_t val){
  if(val != 0xFF) commandAT(_NJM_, bool_to_intString(val));
  return commandAT(_NJM_).toInt();
}

String LoRaWAN_Radioenge::NWKID(){
  return commandAT(_NWKID_);
}

bool LoRaWAN_Radioenge::CLASS(uint8_t val){
  if(val == 0) commandAT(_CLASS_, "A");
  else if(val == 1) commandAT(_CLASS_, "C");
  if(commandAT(_CLASS_) == "C") return true;
  return false;
}

bool LoRaWAN_Radioenge::JOIN(){
  if(commandAT(_JOIN_, "", true) == "AT_JOIN_OK")
    return true;
  return false;
}

bool LoRaWAN_Radioenge::AJOIN(uint8_t val){
  if(val != 0xFF) commandAT(_AJOIN_, bool_to_intString(val));
  return commandAT(_AJOIN_).toInt();
}

bool LoRaWAN_Radioenge::NJS(){
  return commandAT(_NJS_).toInt();
}

String LoRaWAN_Radioenge::VER(){
  return commandAT(_VER_);
}

bool LoRaWAN_Radioenge::CFM(uint8_t val){
  if(val != 0xFF) commandAT(_CFM_, bool_to_intString(val));
  return commandAT(_CFM_).toInt();
}

uint8_t LoRaWAN_Radioenge::SNR(){
  return (uint8_t)commandAT(_SNR_).toInt();
}

int LoRaWAN_Radioenge::RSSI(){
  return commandAT(_RSSI_).toInt();
}

float LoRaWAN_Radioenge::BAT(){
  return commandAT(_BAT_).toInt() * 100 / 253;
}

uint16_t LoRaWAN_Radioenge::BAUDRATE(uint16_t val){
  if(val != 0xFF) commandAT(_BAUDRATE_, String(val));
  return commandAT(_BAUDRATE_).toInt();
}

uint8_t LoRaWAN_Radioenge::NBTRIALS(uint8_t val){
  if(val != 0xFF) commandAT(_NBTRIALS_, String(val));
  return commandAT(_NBTRIALS_).toInt();
}

bool LoRaWAN_Radioenge::TXCFM(uint8_t _port, bool _confirmado, uint8_t _retries, char* payload){
  uint8_t index = 0;
  
  memset(array, 0, BUFFER_SIZE);
  
  strcpy((char*)&array[index], payload);
  index += strlen(payload);
  
  if(index > BUFFER_SIZE)
    return false;
  
  String _payload = "";
  for(int i = 0; i < index; ++i)
    _payload += array[index];
  
  commandAT(_SENDB_, String(_port) + ":" + String(_retries) + ":" + bool_to_intString(_confirmado) + ":" + String(_payload));
  return true;
}

bool LoRaWAN_Radioenge::KEEPALIVE(bool habilitado, uint8_t _port, uint8_t _confirmado, int _periodicidade){
  if(habilitado != 0xFF && _port != 0xFF && _confirmado != 0xFF, _periodicidade != 0xFF)
    commandAT(_KEEPALIVE_, bool_to_intString(habilitado) + ":" + String(_port) + ":" + String(_confirmado) + ":" + String(_periodicidade));      
  deserializeAT(_KEEPALIVE_);
  port          = (uint8_t)payloads[1].toInt();
  confirmado    = (uint8_t)payloads[2].toInt();
  periodicidade = payloads[3].toInt();
  
  return payloads[0].toInt();
}

bool LoRaWAN_Radioenge::pinMode(uint8_t pin, uint8_t modo){
  uint8_t pull = 0; 
  
  if(pin > 9 || modo > 10) return false;
  else if((modo == OUTPUT_FA_PUSHPULL || modo == OUTPUT_FA_OPENDRAIN) && pin != 0 && pin != 1) return false;     
  else if(modo == INPUT_ADC && pin != 0 && pin != 1 && pin != 7 && pin != 8) return false; 
  else if((modo == INTERRUPT_RISING || modo == INTERRUPT_FALLING || modo == INTERRUPT_CHANGE) && pin == 0 && pin == 3 && pin == 7 && pin == 8) return false;
        
  if(modo == INPUT)
    modo = 0;
  else if(modo == OUTPUT)
    modo = 1;
  if(modo == INPUT_PULLUP){
    modo = 0;        
    pull = 1;        
  }
  else if(pull == INPUT_PULLDOWN){
    modo = 0;
    pull = 2;
  }

  deserializeAT(_GPIOC_);
  uint8_t _modo = (uint8_t)payloads[1].toInt();
  uint8_t _pull = (uint8_t)payloads[2].toInt();
  
  if(_modo != modo || _pull != pull)

  commandAT(_GPIOC_, String(pin) + ":" + String(modo) + ":" + String(pull));
  return true;
}

uint8_t LoRaWAN_Radioenge::digitalRead(uint8_t pin){
  return (uint8_t)GPIO(_RPIN_, pin);   
}

uint8_t LoRaWAN_Radioenge::digitalWrite(uint8_t pin, uint8_t val){
  return (uint8_t)GPIO(_WPIN_, pin, val);
}

uint16_t LoRaWAN_Radioenge::analogRead(uint8_t pin){
  return GPIO(_ADC_, pin); 
}

void LoRaWAN_Radioenge::ConfigNetwork(uint8_t njm, uint8_t net, String appkey, String appeui, String nwkskey, String daddr){
  if(NJM() != njm) NJM(njm);
  if(njm == OTAA && CLASS()) CLASS(false);
  String buff_appkey = appkey;   buff_appkey.replace(":", "");
  String buff_appeui = appeui;   buff_appeui.replace(":", "");
  String buff_nwkskey = nwkskey; buff_nwkskey.replace(":", "");
  String buff_daddr  = daddr;    buff_daddr.replace(":", "");    
  
  if(njm == ABP) if(_APPSKEY != buff_appkey && buff_appkey != "") APPSKEY(appkey);
  else if(_APPKEY != buff_appkey && buff_appkey != "") APPKEY(appkey);
  if(_APPEUI != buff_appeui && buff_appeui != "") APPEUI(appeui);
  if(_NWKSKEY != buff_nwkskey && buff_nwkskey != "") NWKSKEY(nwkskey);
  if(_DADDR != buff_daddr && buff_daddr != "") DADDR(daddr);
  uint16_t buff_uint16;
  buff_uint16 = RX1DL(); if((CS == net || TTN == net) && buff_uint16 != 1000) RX1DL(1000); else if(EN == net && buff_uint16 != 5000) RX1DL(5000);
  buff_uint16 = RX2DL(); if((CS == net || TTN == net) && buff_uint16 != 2000) RX2DL(2000); else if(EN == net && buff_uint16 != 6000) RX2DL(6000);
  buff_uint16 = JN1DL(); if((CS == net || TTN == net) && buff_uint16 != 5000) JN1DL(5000); else if(EN == net && buff_uint16 != 5000) JN1DL(5000);
  buff_uint16 = JN2DL(); if((CS == net || TTN == net) && buff_uint16 != 6000) JN2DL(6000); else if(EN == net && buff_uint16 != 6000) JN2DL(5000);
  String buff_string = CHMASK();
  if(net == EN && buff_string != "00ff00000000000000010000") CHMASK("00ff:0000:0000:0000:0001:0000");
  else if((CS == net || TTN == net) && buff_string != "ff0000000000000000020000") CHMASK("ff00:0000:0000:0000:0002:0000");
}

bool LoRaWAN_Radioenge::JoinNetwork(uint8_t njm, uint8_t net,  bool autoconfig, bool automatic, String appkey, String appeui, String nwkskey, String daddr){
  if(autoconfig)
    ConfigNetwork(njm, net, appkey, appeui, nwkskey, daddr);
  if(automatic != AJOIN()) AJOIN(automatic);
  if(!NJS())
    return JOIN();
  else 
    return true;
  return false;
}

bool LoRaWAN_Radioenge::SendString(char* string, uint8_t _port)
{
  if(string == "" || strnlen(string, BUFFER_SIZE) >= BUFFER_SIZE)
    return false;
  
  sprintf(g_payload, "%d:%s\0", _port, string);
  commandAT(_SEND_, g_payload);
  return true;
}

bool LoRaWAN_Radioenge::SendRaw(char* payload, uint8_t _port)
{
  uint8_t index = 0;
  
  memset(array, 0, BUFFER_SIZE);
  
  strcpy((char*)&array[index], payload);
  index += strlen(payload);
  
  if(index > BUFFER_SIZE)
    return false;
  
  String _payload = "";
  for(int i = 0; i < index; ++i)
    _payload += array[index];
  String val = String(_port) + ":" + String(_payload);
  
  commandAT(_SENDB_, val);
  return true;
}
