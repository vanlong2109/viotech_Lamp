void check_multicast_Id(uint8_t* NODE_MC_ADDR,uint8_t* NODE_MC_APPSKEY, uint8_t* NODE_MC_NWKSKEY)
{
  uint32_t Deviceaddr_mult_new = NODE_MC_ADDR[0] << 24 | NODE_MC_ADDR[1] << 16 | NODE_MC_ADDR[2] << 8 | NODE_MC_ADDR[3];
  
  RAK_LORA_McSession multicast_list_check;
  #ifdef DEBUG
  Serial.println("Get all multicast groups");
  #endif
  if(api.lorawan.lstmulc(&multicast_list_check) == true && multicast_list_check.McDevclass != 0)
  {
    uint32_t Deviceaddr_mult_old = multicast_list_check.McAddress;
    if(Deviceaddr_mult_old == Deviceaddr_mult_new) 
    {
      uplink_staus(4,11);
      #ifdef DEBUG
      Serial.println("Id Group đã tồn tại");
      #endif
    }
    else
    {
      // Xử lý sau, khi cho phép 1 thiết bị có nhiều group
    }
  }
  else
  {
    #ifdef DEBUG
    Serial.println("Add Group mới");
    #endif
    setup_Multicast(NODE_MC_ADDR,NODE_MC_APPSKEY, NODE_MC_NWKSKEY);
  }
}
void setup_Multicast(uint8_t* NODE_MC_ADDR,uint8_t* NODE_MC_APPSKEY, uint8_t* NODE_MC_NWKSKEY)
{
  RAK_LORA_McSession session = {
        .McDevclass = 2,
        .McAddress =
            NODE_MC_ADDR[0] << 24 | NODE_MC_ADDR[1] << 16 |
            NODE_MC_ADDR[2] << 8 | NODE_MC_ADDR[3],
        .McFrequency = 921400000,
        .McDatarate = 2,
        .McPeriodicity = 0,
        .McGroupID = 2,
        .entry = 0,
    };
    memcpy(session.McAppSKey, NODE_MC_APPSKEY, 16);
    memcpy(session.McNwkSKey, NODE_MC_NWKSKEY, 16);

  /**LoRaWAN Multicast Setting*/
  if (api.lorawan.addmulc(session) == true) {
    // Gửi bản tin thể hiện đã add thiết bị thành công
    uplink_staus(4,11);
    #ifdef DEBUG
    Serial.println("Add Multicast Success");
    #endif
  } else 
    {
    // Gửi bản tin thể hiện add thiết bị thất bại
    uplink_staus(4,10);
    #ifdef DEBUG
    Serial.println("Add Multicast Fail");
    #endif   
    }
  checkgroup = true;
}
void Remove_MulticastGroup()
{
    // Xóa multicast group để tránh bị lỗi
    RAK_LORA_McSession multicast_list;
    
    #ifdef DEBUG
    Serial.println("Get all multicast groups");
    #endif
    // Kiểm tra xem thiết bị có group hay không
    if(api.lorawan.lstmulc(&multicast_list) == true && multicast_list.McDevclass == 0)
    {
      uplink_staus(4,14);
    }
    else
    {
        uint32_t Deviceaddr_mult = multicast_list.McAddress;
        #ifdef DEBUG
        Serial.print("MCAddr:");
        Serial.println(multicast_list.McAddress,HEX);
        #endif
        // Xóa multicast group        
        bool remove_status = api.lorawan.rmvmulc(Deviceaddr_mult);
        if(remove_status) uplink_staus(4,12);
        else uplink_staus(4,13);
        #ifdef DEBUG
        Serial.printf("Remove multicast group: %s\r\n", remove_status ? "Success" : "Fail");
        #endif      
    }
    // Lặp lại để xóa tất cả multicast groups
//    while (api.lorawan.lstmulc(&multicast_list) == true && multicast_list.McDevclass != 0) {
//        uint32_t Deviceaddr_mult = multicast_list.McAddress;
//        #ifdef DEBUG
//        Serial.print("MCAddr:");
//        Serial.println(multicast_list.McAddress,HEX);
//        #endif
//        // Xóa multicast group        
//        bool remove_status = api.lorawan.rmvmulc(Deviceaddr_mult);
//        if(remove_status) uplink_staus(4,12);
//        else uplink_staus(4,13);
//        #ifdef DEBUG
//        Serial.printf("Remove multicast group: %s\r\n", remove_status ? "Success" : "Fail");
//        #endif
//    }
    #ifdef DEBUG
    Serial.println("All multicast groups removed");
    #endif
}  
