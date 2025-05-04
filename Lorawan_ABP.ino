void setup_lorawan(uint8_t* ABP_DEVADDR, uint8_t* ABP_APPSKEY, uint8_t* ABP_NWKSKEY) {
  if (api.lorawan.nwm.get() != 1) {
    #ifdef DEBUG
    Serial.printf("Set Node device work mode %s\r\n", api.lorawan.nwm.set() ? "Success" : "Fail");
    #endif
    api.system.reboot();
  }
  if (!api.lorawan.njm.set(RAK_LORA_ABP)) {
    #ifdef DEBUG
    Serial.printf("LoRaWan ABP - set network join mode is incorrect! \r\n");
    #endif
    return;
  }
  if (!api.lorawan.daddr.set(ABP_DEVADDR, 4)) {
    #ifdef DEBUG
    Serial.printf("LoRaWan ABP - set device addr is incorrect! \r\n");
    #endif
    return;
  }
  if (!api.lorawan.appskey.set(ABP_APPSKEY, 16)) {
    #ifdef DEBUG
    Serial.printf("LoRaWan ABP - set application session key is incorrect! \r\n");
    #endif
    return;
  }
  if (!api.lorawan.nwkskey.set(ABP_NWKSKEY, 16)) {
    #ifdef DEBUG
    Serial.printf("LoRaWan ABP - set network session key is incorrect! \r\n");
    #endif
    return;
  }
  if (!api.lorawan.band.set(ABP_BAND)) {
    #ifdef DEBUG
    Serial.printf("LoRaWan ABP - set band is incorrect! \r\n");
    #endif
    return;
  }
  if (!api.lorawan.deviceClass.set(RAK_LORA_CLASS_C)) {
    #ifdef DEBUG
    Serial.printf("LoRaWan ABP - set device class is incorrect! \r\n");
    #endif
    return;
  }

  // if (!api.lorawan.adr.set(true)) {
  //   #ifdef DEBUG
  //   Serial.printf("LoRaWan ABP - set adaptive data rate is incorrect! \r\n");
  //   #endif
  //   return;
  // }

  if (!api.lorawan.dr.set(5)) {
    #ifdef DEBUG
    Serial.printf("LoRaWan ABP - set data rate is incorrect! \r\n");
    #endif
    return;
  }  

  // if (!api.lora.pfreq.set(922000000)) {
  //   #ifdef DEBUG
  //   Serial.printf("LoRaWan ABP - set channel is incorrect! \r\n");
  //   #endif
  //   return;
  // }  
  if (!api.lorawan.rety.set(0)) {
    #ifdef DEBUG
    Serial.printf("LoRaWan ABP - set retry times is incorrect! \r\n");
    #endif
    return;
  }
  if (!api.lorawan.cfm.set(1)) {
    #ifdef DEBUG
    Serial.printf("LoRaWan ABP - set confirm mode is incorrect! \r\n");
    #endif
    return;
  }

  #ifdef DEBUG
  /** Check LoRaWan Status */
  Serial.printf("Duty cycle is %s\r\n", api.lorawan.dcs.get() ? "ON" : "OFF");  // Check Duty Cycle status
  Serial.printf("Packet is %s\r\n", api.lorawan.cfm.get() ? "CONFIRMED" : "UNCONFIRMED"); // Check Confirm status
  uint8_t assigned_dev_addr[4] = {0};
  api.lorawan.daddr.get(assigned_dev_addr, 4);
  Serial.printf("Device Address is %02X%02X%02X%02X\r\n", assigned_dev_addr[0], assigned_dev_addr[1], assigned_dev_addr[2], assigned_dev_addr[3]);  // Check Device Address
  // Serial.printf("Uplink period is %ums\r\n", ABP_PERIOD);
  Serial.println("");
  #endif
  api.lorawan.registerRecvCallback(recvCallback);
  api.lorawan.registerSendCallback(sendCallback);
}

void sendCallback(int32_t status)
{
  if (status == RAK_LORAMAC_STATUS_OK) {
    Serial.println("Successfully sent");
  } else {
    Serial.println("Sending failed");
    Serial.println(status);
  }
}

/*************************************
   enum type for LoRa Event
    RAK_LORAMAC_STATUS_OK = 0,
    RAK_LORAMAC_STATUS_ERROR,
    RAK_LORAMAC_STATUS_TX_TIMEOUT,
    RAK_LORAMAC_STATUS_RX1_TIMEOUT,
    RAK_LORAMAC_STATUS_RX2_TIMEOUT,
    RAK_LORAMAC_STATUS_RX1_ERROR,
    RAK_LORAMAC_STATUS_RX2_ERROR,
    RAK_LORAMAC_STATUS_JOIN_FAIL,
    RAK_LORAMAC_STATUS_DOWNLINK_REPEATED,
    RAK_LORAMAC_STATUS_TX_DR_PAYLOAD_SIZE_ERROR,
    RAK_LORAMAC_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS,
    RAK_LORAMAC_STATUS_ADDRESS_FAIL,
    RAK_LORAMAC_STATUS_MIC_FAIL,
    RAK_LORAMAC_STATUS_MULTICAST_FAIL,
    RAK_LORAMAC_STATUS_BEACON_LOCKED,
    RAK_LORAMAC_STATUS_BEACON_LOST,
    RAK_LORAMAC_STATUS_BEACON_NOT_FOUND,
 *************************************/
