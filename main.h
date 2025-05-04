#define DEBUG

#define ABP_PERIOD   (60000)
#define MULTICAST_PERIOD   (60000)
#define ABP_BAND     (RAK_REGION_AS923_2)

#include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include "RAKLorawan.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "Key.h"

String Data = "";
StaticJsonDocument<200> JSON_Doc;

uint8_t NODE_MC_ADDR[4];
uint8_t NODE_MC_APPSKEY[16];
uint8_t NODE_MC_NWKSKEY[16];

int check_multicast = 0;
static uint64_t time_multicast = 0;
long send_retry = 0;
long send_retry_control = 0;
int retry = 0;
uint8_t collected_data[50];
uint8_t data_len;
bool checkgroup = false;

/* Thiết lập queue ở thiết bị*/
#define MAX_PACKET_SIZE 50
#define QUEUE_SIZE 10   // Tối đa 10 gói trong hàng đợi

struct UplinkPacket {
  uint8_t data[MAX_PACKET_SIZE];
  uint8_t len;
};

UplinkPacket packetQueue[QUEUE_SIZE];
UplinkPacket pkt;
int head = 0;  // Điểm ghi tiếp theo
int tail = 0;  // Điểm đọc tiếp theo
int count = 0; // Số phần tử hiện tại trong queue
unsigned long lastSend = 0; // Biến thời gian chờ dentaT
uint32_t dentaT = 0;
bool check_control = false; // Kiểm tra có gói tin điều khiển gửi lên chưa
bool first_send = false; // Thời gian chờ của gói tin đầu tiên
bool check_resend = false;
int count_resend = 1;
JSONVar object; // object của chuỗi json