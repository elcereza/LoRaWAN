#ifndef LoRaWAN_Radioenge_h
#define LoRaWAN_Radioenge_h

#include "arduino.h"
#include "Stream.h"

#define _ATZ_               0
#define _DADDR_             1
#define _APPKEY_            2   
#define _APPSKEY_           3   
#define _NWKSKEY_           4   
#define _APPEUI_            5   
#define _ADR_               6
#define _DR_                7
#define _DCS_               8
#define _PNM_               9
#define _RX2FQ_             10 
#define _RX2DR_             11
#define _RX1DL_             12  
#define _RX2DL_             13 
#define _JN1DL_             14
#define _JN2DL_             15
#define _NJM_               16
#define _NWKID_             17
#define _CLASS_             18  
#define _JOIN_              19
#define _NJS_               20
#define _SENDB_             21
#define _SEND_              22
#define _VER_               23
#define _CFM_               24
#define _SNR_               25
#define _RSSI_              26
#define _BAT_               27
#define _BAUDRATE_          28    
#define _NBTRIALS_          29      
#define _KEEPALIVE_         30    
#define _TXCFM_             31
#define _CHMASK_            32  
#define _ADC_               33
#define _GPIOC_             34
#define _WPIN_              35
#define _RPIN_              36
#define _AJOIN_             37
#define _DEUI_              38

#define ABP                 0
#define OTAA                1

#define CS                  0
#define TTN                 1
#define EN                  2

#define OUTPUT_OPENDRAIN    2
#define OUTPUT_FA_PUSHPULL  3
#define OUTPUT_FA_OPENDRAIN 4

#define INPUT_ADC           6
#define INTERRUPT_RISING    7
#define INTERRUPT_FALLING   8
#define INTERRUPT_CHANGE    10

#ifndef INPUT
#define INPUT                0
#endif
#ifndef INPUT_PULLUP
#define INPUT_PULLUP         7
#endif

#ifdef  INPUT_PULLDOWN_16
#define INPUT_PULLDOWN       INPUT_PULLDOWN_16
#endif

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN       8
#endif

#define BUFFER_SIZE   256

class LoRaWAN_Radioenge{
private:
  Stream* SerialLoRaWAN;

  bool feedback = false;
  String _DADDR,
         _APPKEY,
         _APPSKEY,
         _NWKSKEY,
         _APPEUI,
         _DEUI;

  String AT_CMD[39] = {
        "ATZ",
        "DADDR",
        "APPKEY",
        "APPSKEY",
        "NWKSKEY",
        "APPEUI",
        "ADR",
        "DR",
        "DCS",
        "PNM",
        "RX2FQ",
        "RX2DR",
        "RX1DL",
        "RX2DL",
        "JN1DL",
        "JN2DL",
        "NJM",
        "NWKID",
        "CLASS",
        "JOIN",
        "NJS",
        "SENDB",
        "SEND",
        "VER",
        "CFM",
        "SNR",
        "RSSI",
        "BAT",
        "BAUDRATE",
        "NBTRIALS",
        "KEEPALIVE",
        "TXCFM",
        "CHMASK",
        "ADC",
        "GPIOC",
        "WPIN",
        "RPIN",
        "AJOIN",
        "DEUI"
  };

  char g_payload[BUFFER_SIZE];
  uint8_t array[BUFFER_SIZE];
  String payloads[5];  

  String feedbackSerial(String val, bool exception = false);

  String commandAT(uint8_t cmd, String val = "", bool exception = false);

  String bool_to_intString(uint8_t val);

  void deserializeAT(uint8_t cmd);
  
  String separator(String val);

  uint16_t GPIO(uint8_t cmd, uint8_t pin, uint8_t val = 2);

public:
  uint8_t port = 1, confirmado = 0, retries = 0;
  int periodicidade = 0;

  LoRaWAN_Radioenge(Stream* _SerialLoRaWAN);

  void printParameters();

  void begin(bool _feedback = false);

  String DADDR(String val = "");

  String APPKEY(String val = "");

  String APPSKEY(String val = "");

  String NWKSKEY(String val = "");

  String APPEUI(String val = "");

  String DEUI();

  String CHMASK(String val = "");

  void ATZ();

  bool ADR(uint8_t val = 0xFF);

  uint8_t DR(uint8_t val = 14);

  bool DCS(uint8_t val = 0xFF);

  bool PNM(uint8_t val = 0xFF);

  uint32_t RX2FQ(uint32_t val = 0xFF);

  uint16_t RX2DR(uint16_t val = 0xFF);

  uint16_t RX1DL(uint16_t val = 0xFF);

  uint16_t RX2DL(uint16_t val = 0xFF);

  uint16_t JN1DL(uint16_t val = 0xFF);

  uint16_t JN2DL(uint16_t val = 0xFF);

  bool NJM(uint8_t val = 0xFF);

  String NWKID();

  bool CLASS(uint8_t val = 2);

  bool JOIN();

  bool AJOIN(uint8_t val = 0xFF);

  bool NJS();

  String VER();

  bool CFM(uint8_t val = 0xFF);

  uint8_t SNR();

  int RSSI();

  float BAT();

  uint16_t BAUDRATE(uint16_t val = 0xFF);

  uint8_t NBTRIALS(uint8_t val = 0xFF);

  bool TXCFM(uint8_t _port, bool _confirmado, uint8_t _retries, char* payload);

  bool KEEPALIVE(bool habilitado = 0xFF, uint8_t _port = 0xFF, uint8_t _confirmado = 0xFF, int _periodicidade = 0xFF);

  bool pinMode(uint8_t pin, uint8_t modo);

  uint8_t digitalRead(uint8_t pin);

  uint8_t digitalWrite(uint8_t pin, uint8_t val);
  
  uint16_t analogRead(uint8_t pin);

  void ConfigNetwork(uint8_t njm = 0xFF, uint8_t net = 0xFF, String appkey = "", String appeui = "", String nwkskey = "", String daddr = "");

  bool JoinNetwork(uint8_t njm = 0xFF, uint8_t net = 0xFF,  bool autoconfig = true, bool automatic = 0xFF, String appkey = "", String appeui = "", String nwkskey = "", String daddr = "");

  bool SendString(char* string, uint8_t _port = 1);
  
  bool SendRaw(char* payload, uint8_t _port = 1);
};
#endif
