void uplink(uint8_t* collected_data, uint8_t data_len)
{
#ifdef DEBUG
  Serial.println("Data Packet:");
  for (int i = 0; i < data_len; i++) {
    Serial.printf("0x%02X ", collected_data[i]);
  }
  Serial.println("");
#endif

  /** Send the data package */
  if (api.lorawan.send(data_len, collected_data, 2, true, 0)) {
    
    #ifdef DEBUG
    Serial.println("Sending is requested");
    #endif
  } else {
    #ifdef DEBUG
    Serial.println("Sending false");
    //api.system.reboot();
    #endif
  }
}

void uplink_staus(uint8_t header_device, uint16_t status_code)
{

  data_len = 0;
  collected_data[data_len++] = (uint8_t)(header_device);
  collected_data[data_len++] = (uint8_t)(status_code);
  collected_data[data_len++] = (uint8_t)(status_code >> 8);
  enqueuePacket(collected_data,data_len);
  #ifdef DEBUG
  Serial.print("Uplink status: ");
  Serial.println(status_code);
  #endif  
}
void uplink_connect(uint8_t header_device, uint16_t status_code)
{

  data_len = 0;
  collected_data[data_len++] = (uint8_t)(header_device);
  collected_data[data_len++] = (uint8_t)(status_code);
  collected_data[data_len++] = (uint8_t)(status_code >> 8);
  uplink(collected_data,data_len);
  #ifdef DEBUG
  Serial.print("Uplink status: ");
  Serial.println(status_code);
  #endif  
}
// void uplink_admin(uint8_t header_device, uint8_t mode, uint8_t* payload, uint8_t len)
// {

//   data_len = 0;
//   collected_data[data_len++] = (uint8_t)(header_device);
//   collected_data[data_len++] = (uint8_t)(mode);
//   collected_data[data_len++] = (uint8_t)(status_code >> 8);
//   enqueuePacket(collected_data,data_len);
//   #ifdef DEBUG
//   Serial.print("Uplink status: ");
//   Serial.println(status_code);
//   #endif  
// }
// Thêm gói tin vào hàng đợi
bool enqueuePacket(uint8_t* data, uint8_t len) {
  if (count >= QUEUE_SIZE) {
    // Hàng đợi đầy
    return false;
  }
  if (len > MAX_PACKET_SIZE) {
    // Dữ liệu quá dài
    return false;
  }
  // Copy dữ liệu vào hàng đợi
  memcpy(packetQueue[head].data, data, len);
  packetQueue[head].len = len;
  // Cập nhật vị trí và số lượng
  head = (head + 1) % QUEUE_SIZE;
  count++;
  if(count == 1)
  {
    lastSend = millis();
  }
  return true;
}

// Lấy gói tin từ hàng đợi
bool dequeuePacket(UplinkPacket* pkt) {
  if (count == 0) {
    // Hàng đợi rỗng
    return false;
  }
  // Copy dữ liệu ra ngoài
  for(int i=0;i<MAX_PACKET_SIZE;i++)
  {pkt->data[i]=0;}
  pkt->len = 0;
  memcpy(pkt->data, packetQueue[tail].data, packetQueue[tail].len);
  pkt->len = packetQueue[tail].len;
  // Cập nhật vị trí và số lượng
  tail = (tail + 1) % QUEUE_SIZE;
  count--;
  return true;
}

//Kiểm tra xem có gói tin để gửi hay không
void sendFromQueue() {
  int random_dentaT = random(1, 100)*10;
  if (millis() - lastSend >= (random_dentaT+dentaT*1000) && (count > 0) && (check_resend == false)) {  // Gửi mỗi DentaT giây nếu có gói
    if (dequeuePacket(&pkt)) {
      // Gửi LoRa: ví dụ
      check_resend = true;
      uplink(pkt.data, pkt.len);
      #ifdef DEBUG
      Serial.println("Đã gửi gói tin từ hàng đợi.");
      Serial.print("dentaT: ");    
      Serial.println(dentaT);
      #endif
    }
    lastSend = millis();
    send_retry = millis();
  }
  if(check_resend)
  {
    if(((millis() - send_retry) > 2000) && (count_resend < 4))
    {
      if(!(api.lorawan.cfs.get()))
      {
        int dms = random(1, 100)*10;
        if((millis() - send_retry) > (2000 + dms))
        {
          uplink(pkt.data, pkt.len);
          #ifdef DEBUG
          Serial.println("Send RETRY Data_packet");
          #endif
          send_retry = millis();
          count_resend++;
        }
        else
        {
          //Không làm gì cả
        }
      }
      else{
        check_resend = false;
        count_resend = 1;
      }
    }
    if(count_resend == 4){
    check_resend = false;
    count_resend = 1;
    }
  }
}