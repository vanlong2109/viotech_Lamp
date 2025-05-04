void recvCallback(SERVICE_LORA_RECEIVE_T * data)
{
  if (data->BufferSize > 0) {
    #ifdef DEBUG
    Serial.println("Something received!");
    for (int i = 0; i < data->BufferSize; i++) {
      Serial.printf("%x", data->Buffer[i]);
      #endif
    }
    processReceivedData(data);
    #ifdef DEBUG
    Serial.print("\r\n");
    #endif
  }
}
void hexStringToByteArray(const char *hexStr, uint8_t *byteArray, size_t byteArraySize) {
    for (size_t i = 0; i < byteArraySize; i++) {
        char buf[3] = { hexStr[2*i], hexStr[2*i+1], '\0' };
        byteArray[i] = (uint8_t)strtol(buf, NULL, 16);
    }
}

void processReceivedData(SERVICE_LORA_RECEIVE_T *data) {
    
    // Chuyển đổi mảng byte thành chuỗi JSON
    char jsonString[data->BufferSize + 1];
    for (size_t i = 0; i < data->BufferSize; i++) {
      jsonString[i] = (char)data->Buffer[i];
    }
    jsonString[data->BufferSize] = '\0';
    
    // Parse chuỗi JSON
    object = JSON.parse(jsonString);

    // Kiểm tra lỗi parse
    if (JSON.typeof(object) == "undefined") {
    #ifdef DEBUG
      Serial.println("Parsing input failed!");
    #endif
      return;
    }
    
    // Lấy các key từ object
    JSONVar keys = object.keys();
    #ifdef DEBUG
      Serial.println("Keys:");
      Serial.println(JSON.stringify(keys));
      Serial.println("Object:");
      Serial.println(JSON.stringify(object));
    #endif 
    
    for (int i = 0; i < keys.length(); i++) 
    {
        String key = JSON.stringify(keys[i]);
        key.replace("\"", "");  // Loại bỏ dấu ngoặc kép
        if (key == "addr")
        {
          // Lấy các chuỗi hex từ JSON
          const char *addrStr = ((const char*)object["addr"]);
          hexStringToByteArray(addrStr, NODE_MC_ADDR, 4);
          check_multicast++;
            #ifdef DEBUG
              Serial.print("addr: ");
              for (int i = 0; i < 4; i++) {
                Serial.printf("%02X", NODE_MC_ADDR[i]);
                if (i < 3) Serial.print(",");
              }
              Serial.println();
            #endif 
          if(check_multicast == 1)
          {
              time_multicast = millis();
          }            
          if(check_multicast == 3)
          { 
            check_multicast = 0;
            //add multicast group
            check_multicast_Id(NODE_MC_ADDR,NODE_MC_APPSKEY,NODE_MC_NWKSKEY);
            #ifdef DEBUG
              Serial.println("add_Multicast");
            #endif  
          }                                 
        }
        else if (key == "nwk")
        {
          // Lấy các chuỗi hex từ JSON
          const char *nwkKeyStr = ((const char*)object["nwk"]);
          hexStringToByteArray(nwkKeyStr, NODE_MC_NWKSKEY, 16);
          check_multicast++;
            #ifdef DEBUG
              Serial.print("nwk: ");
              for (int i = 0; i < 16; i++) {
                Serial.printf("%02X", NODE_MC_NWKSKEY[i]);
                if (i < 15) Serial.print(",");
              }
              Serial.println();
            #endif 
          if(check_multicast == 1)
          {
              time_multicast = millis();
          }            
          if(check_multicast == 3)
          { 
            check_multicast = 0;
            //add multicast group
            check_multicast_Id(NODE_MC_ADDR,NODE_MC_APPSKEY,NODE_MC_NWKSKEY);
            #ifdef DEBUG
              Serial.println("add_Multicast");
            #endif  
          }                      
        }
        else if (key == "app")
        {
          // Lấy các chuỗi hex từ JSON
          const char *appKeyStr = ((const char*)object["app"]);
          hexStringToByteArray(appKeyStr, NODE_MC_APPSKEY, 16);
          check_multicast++;
            #ifdef DEBUG
              Serial.print("app: ");
              for (int i = 0; i < 16; i++) {
                Serial.printf("%02X", NODE_MC_APPSKEY[i]);
                if (i < 15) Serial.print(",");
              }
              Serial.println();         
            #endif 
          if(check_multicast == 1)
          {
              time_multicast = millis();
          }            
          if(check_multicast == 3)
          { 
            check_multicast = 0;
            //add multicast group
            check_multicast_Id(NODE_MC_ADDR,NODE_MC_APPSKEY,NODE_MC_NWKSKEY);
            #ifdef DEBUG
              Serial.println("add_Multicast");
            #endif  
          }                    
        }
        else if(key == "remove_multicast")
        {
          Serial1.println(JSON.stringify(object));
          Remove_MulticastGroup();
          dentaT = 0;
        }
       else if(key == "set_dentaT")
       {
          const char* dentaTStr = ((const char*)object["set_dentaT"]);
          char* endptr;
          unsigned long temp = strtoul(dentaTStr, &endptr, 10);

          // Kiểm tra lỗi chuyển đổi
          if (*endptr != '\0') {
              // Xử lý lỗi: chuỗi không phải là số hợp lệ
          } else if (temp > UINT32_MAX) {
              // Xử lý lỗi: giá trị vượt quá giới hạn của uint32_t
          } else {
              dentaT = static_cast<uint32_t>(temp);
              uint8_t flash_value[4] = {0};
              bool wr_result = false;
              flash_value[0] = (uint8_t)(dentaT >> 0);
              flash_value[1] = (uint8_t)(dentaT >> 8);
              flash_value[2] = (uint8_t)(dentaT >> 16);
              flash_value[3] = (uint8_t)(dentaT >> 24);
              wr_result = api.system.flash.set(0, flash_value, 4);
              if(wr_result) uplink_staus(4,43);
              else uplink_staus(4,45);
          }      
       }
       else if(key == "get_dentaT")
        {
           // Uplink Admin
        }                   
       else
       {
        Serial1.println(JSON.stringify(object));
        #ifdef DEBUG
        Serial.println("Control");
        #endif
        check_control = true;
        send_retry_control = millis();
       }
  }
}
