#include "main.h"
void setup()
{
  Serial.begin(115200, RAK_AT_MODE);
  delay(1000);
  Serial1.begin(115200);
  delay(1000);
  setup_lorawan(ABP_DEVADDR, ABP_APPSKEY, ABP_NWKSKEY);
  delay(5000);
  load_dentaT();
  delay(dentaT);
  uplink_connect(4,200);
  first_send = true;

  // while(!(api.lorawan.cfs.get()))
  // {
  //   int random_dentaT = random(1, 50)*100;
  //   if((millis() - send_retry) > (5000+random_dentaT))
  //   {
  //     uplink_staus(4,200);
  //     #ifdef DEBUG
  //     Serial.println("Send RETRY 200");
  //     #endif
  //     send_retry = millis();
  //   }
  // }
  send_retry = millis();
}

void loop()
{
  if(first_send)
  {
    if(!(api.lorawan.cfs.get()))
    {
      int random_dentaT = random(1, 50)*100;
      if((millis() - send_retry) > (5000+random_dentaT))
      {
        uplink_connect(4,200);
        #ifdef DEBUG
        Serial.println("Send RETRY 200");
        #endif
        send_retry = millis();
      }
    }
    else first_send = false;
  }
  // static uint64_t last = 0;
  // static uint64_t elapsed;

  // if(!(api.lorawan.cfs.get()))
  // {
  //   if((millis() - send_retry) > 1000)
  //   {
  //     retry ++;
  //     send_retry = millis();
  //     if(retry == 5)
  //     {
  //         uplink(collected_data,data_len);
  //         retry = 0;
  //         #ifdef DEBUG
  //         Serial.println("Send RETRY");
  //         Serial.println("Data Packet:");
  //         for (int i = 0; i < data_len; i++) {
  //           Serial.printf("0x%02X ", collected_data[i]);
  //         }
  //         Serial.println("");
  //         #endif
  //     }
  //   }
  // }
  if(check_control && ((millis() - send_retry_control) > 5000))
  {
    Serial1.println(JSON.stringify(object));
    send_retry_control = millis();
  }
  if (check_multicast >= 1 && (millis() - time_multicast) > MULTICAST_PERIOD) {
    #ifdef DEBUG
      Serial.println("Hết thời gian chờ 60s");
      Serial.println("add_Multicast");
    #endif
    
    (4,10);
    check_multicast = 0;
  }  
  sendFromQueue();
  Receive();
}

void load_dentaT(void)
{
  uint8_t flash_read[4] = {0};

  if (api.system.flash.get(0, flash_read, 4))
  {
    dentaT |= flash_read[0] << 0;
    dentaT |= flash_read[1] << 8;
    dentaT |= flash_read[2] << 16;
    dentaT |= flash_read[3] << 24;
    #ifdef DEBUG
    Serial.print("DentaT = ");
    Serial.println(dentaT);
    #endif
  }
  else
  {
    #ifdef DEBUG
    Serial.println("Failed to read data from Flash");
    #endif
  }
}