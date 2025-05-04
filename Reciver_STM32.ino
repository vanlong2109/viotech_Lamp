void Receive(void){

  if (Serial1.available()) // kiem tra xem co du lieu truyen den hay khong
  {
    //Serial.readStringUntil(<kí tự dừng>) // cho phếp đọc toàn bộ chuỗi được gửi đến cho đến khi gặp kí tự dừng.
    Data = Serial1.readStringUntil('\n');
    #ifdef DEBUG
    Serial.print("Data nhan: ");
    Serial.print(Data);
    #endif
    //Xu ly JSon-truyen --------------------
    DeserializationError error = deserializeJson(JSON_Doc, Data);
    if(error)
    {
      #ifdef DEBUG
      Serial.print(F("bi loi roi() Loi la: "));
      Serial.println(error.c_str());
      #endif
      return;  
    }
    else
    {
      check_control = false;
      if (JSON_Doc.containsKey("timestamp")) 
      {
          String timestampStr = JSON_Doc["timestamp"];
          uint32_t timestamp = strtoul(timestampStr.c_str(), NULL, 10);
          data_len = 0;
          uint8_t header_device = 3;
          collected_data[data_len++] = (uint8_t)(header_device);
          collected_data[data_len++] = (uint8_t)(timestamp);
          collected_data[data_len++] = (uint8_t)(timestamp >> 8);
          collected_data[data_len++] = (uint8_t)(timestamp >> 16);
          collected_data[data_len++] = (uint8_t)(timestamp >> 24);
          enqueuePacket(collected_data,data_len);
          #ifdef DEBUG
          Serial.print("Timestamp: ");
          Serial.println(timestamp);
          Serial.print("Timestamp_lenght: ");
          Serial.println(timestampStr.length());
          #endif     
      }
      if (JSON_Doc.containsKey("statusCodeSchedule"))
      {
          String statusCodeStr = JSON_Doc["statusCodeSchedule"];
          String IdScheduleStr = JSON_Doc["ID"];
          data_len = 0;
          uint8_t header_device = 4;
          uint32_t IdSchedule = (uint32_t) strtoul(IdScheduleStr.c_str(), NULL, 10);
          uint16_t statusCode = (uint16_t) strtoul(statusCodeStr.c_str(), NULL, 10);
             
          collected_data[data_len++] = (uint8_t)(header_device);
          collected_data[data_len++] = (uint8_t)(statusCode);
          collected_data[data_len++] = (uint8_t)(statusCode >> 8);
          collected_data[data_len++] = (uint8_t)(IdSchedule);
          collected_data[data_len++] = (uint8_t)(IdSchedule >> 8);
          collected_data[data_len++] = (uint8_t)(IdSchedule >> 16);
          collected_data[data_len++] = (uint8_t)(IdSchedule >> 24);          
          enqueuePacket(collected_data,data_len);        
      }
      if (JSON_Doc.containsKey("statusCode"))
      {
          String statusCodeStr = JSON_Doc["statusCode"];
          data_len = 0;
          uint8_t header_device = 4;
          uint16_t statusCode = (uint16_t) strtoul(statusCodeStr.c_str(), NULL, 10);
          collected_data[data_len++] = (uint8_t)(header_device);
          collected_data[data_len++] = (uint8_t)(statusCode);
          collected_data[data_len++] = (uint8_t)(statusCode >> 8);
          enqueuePacket(collected_data,data_len);        
      }      
      if (JSON_Doc.containsKey("lat"))
      {
          String latStr = JSON_Doc["lat"];
          String lonStr = JSON_Doc["lon"];
          String altStr = JSON_Doc["alt"];
  
          uint32_t lat = (uint32_t) strtoul(latStr.c_str(), NULL, 10);
          uint32_t lon = (uint32_t) strtoul(lonStr.c_str(), NULL, 10);
          uint32_t alt = (uint32_t) strtoul(altStr.c_str(), NULL, 10);
             
          data_len = 0;
          uint8_t header_device = 2;
          collected_data[data_len++] = (uint8_t)(header_device);
          collected_data[data_len++] = (uint8_t)lat;
          collected_data[data_len++] = (uint8_t)(lat >> 8);
          collected_data[data_len++] = (uint8_t)(lat >> 16);
          collected_data[data_len++] = (uint8_t)(lat >> 24);
        
          collected_data[data_len++] = (uint8_t)lon;
          collected_data[data_len++] = (uint8_t)(lon >> 8);
          collected_data[data_len++] = (uint8_t)(lon >> 16);
          collected_data[data_len++] = (uint8_t)(lon >> 24);
        
          collected_data[data_len++] = (uint8_t)alt;
          enqueuePacket(collected_data,data_len);
          #ifdef DEBUG
          Serial.println("Uplink GPS");
          #endif   
      }
      if (JSON_Doc.containsKey("status"))
      {
        String statusStr = JSON_Doc["status"];
        String DimmingStr = JSON_Doc["Dimming"];
        String VrmsStr = JSON_Doc["Vrms"];
        String IrmsStr = JSON_Doc["Irms"];
        String PFStr = JSON_Doc["PF"];
        String PowerStr = JSON_Doc["Power"];
        String EnergyStr = JSON_Doc["Energy"];
        String TiltStr = JSON_Doc["Tilt"];
        
        int DimmingInt = DimmingStr.toInt();
        int statusInt = statusStr.toInt();
        uint32_t Energy = strtoul(EnergyStr.c_str(), NULL, 10);
        uint16_t Vrms = (uint16_t) strtoul(VrmsStr.c_str(), NULL, 10);
        uint16_t Irms = (uint16_t) strtoul(IrmsStr.c_str(), NULL, 10);
        uint16_t PF = (uint16_t) strtoul(PFStr.c_str(), NULL, 10);
        uint16_t Power = (uint16_t) strtoul(PowerStr.c_str(), NULL, 10);
        uint16_t Tilt = (uint16_t) strtoul(TiltStr.c_str(), NULL, 10);

        data_len = 0;
        uint8_t header_device = 1;

        
        collected_data[data_len++] = (uint8_t)(header_device);
        collected_data[data_len++] = (uint8_t)(DimmingInt);
        collected_data[data_len++] = (uint8_t)(statusInt);
        
        collected_data[data_len++] = (uint8_t)(Energy);
        collected_data[data_len++] = (uint8_t)(Energy >> 8);
        collected_data[data_len++] = (uint8_t)(Energy >> 16);
        collected_data[data_len++] = (uint8_t)(Energy >> 24);

        collected_data[data_len++] = (uint8_t)(Vrms);
        collected_data[data_len++] = (uint8_t)(Vrms >> 8);
        collected_data[data_len++] = (uint8_t)(Irms);
        collected_data[data_len++] = (uint8_t)(Irms >> 8);
        collected_data[data_len++] = (uint8_t)(PF);
        collected_data[data_len++] = (uint8_t)(PF >> 8);
        collected_data[data_len++] = (uint8_t)(Power);
        collected_data[data_len++] = (uint8_t)(Power >> 8);
        collected_data[data_len++] = (uint8_t)(Tilt);
        collected_data[data_len++] = (uint8_t)(Tilt >> 8);

        enqueuePacket(collected_data,data_len);
      }
                    
     } 
  }
      //Clear du lieu trong mang de nhan moi-----------------------
     Data = ""; 
}
