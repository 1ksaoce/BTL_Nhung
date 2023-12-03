/*Xuân Oai*/
// #include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <WifiClient.h>

// define cac chan dieu khien 
#define  RED 4 
#define  WHITE 16 
#define  LED 2
#define  FAN 0
#define tat 0
#define bat 1

// define pwm
#define PWM_channel 0  // chon kenh 0
#define PWM_channel 8 // chon kenh 8
#define Freg 20000  // led duoc su dung voi tan so 20khz
#define Resolution 8

// button 
#define BUTTON_RED_PIN 5    // Chân cho nút điều khiển đèn đỏ
#define BUTTON_WHITE_PIN 17  // Chân cho nút điều khiển đèn xanh
#define BUTTON_FAN_PIN 19 // Chân cho nút điều khiển đèn vàng
#define BUTTON_LED_PIN 18 

#define ssid "Ducnguyen"
#define password "11110000"

#define FIREBASE_HOST "https://btl-nhung-843d2-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "ii6bZXZNLPr40Z5R4rHICtoRKcz0ErpJVUeKBtKA"

FirebaseData firebaseData;

// KHAI BAO TRANG THAI 
int RED_STATE=0, WHITE_STATE=0, LED_STATE =0, FAN_STATE = 0, DO_SANG=0;

void Wifi_init(void); // cau hinh ket noi wifi
void Firebase_init(void); // cau hinh ket noi firebase
void Setup_device(void); // thiet lap gia tri ban dau cho thiet bi , ban dau thiet bi tat
void Write_data_firebase(void);// ghi du lieu toi firebase
void Read_data_firebase(void);// doc du lieu tu firebase
void control_device(int device, int state); // dieu khien thiet bi// tham so truyen thiet bi va trang thai cua thiet bi
void Pwm_init(void);// ham setup chan
void dieukhiendosang(int value);//tang giam phan tram do sang
void checkButtons(void);
void setup()
{
  Wifi_init();
  Firebase_init();
  Setup_device();
  Write_data_firebase();

  Pwm_init(); // Thêm hàm khởi tạo PWM
  pinMode(BUTTON_RED_PIN, INPUT_PULLUP);
  pinMode(BUTTON_WHITE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_FAN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_FAN_PIN, INPUT_PULLUP);
  
}

void loop()
{
  Read_data_firebase();
  checkButtons(); // Thêm hàm để kiểm tra nút nhấn
}

void Wifi_init(void)//
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED )
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

}
void Firebase_init(void)
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}


void Setup_device(void)
{
  pinMode(RED,OUTPUT);
  pinMode(WHITE,OUTPUT);
  pinMode(FAN,OUTPUT);
  pinMode(LED,OUTPUT);
  digitalWrite(RED,LOW);
  digitalWrite(WHITE,LOW); 
  digitalWrite(FAN,LOW);
  digitalWrite(LED,OUTPUT);
}

void Read_data_firebase(void)
{
  if(Firebase.get(firebaseData,"/BTL_NHUNG/WHITE"))// kiem tra xem co duong /DA1_DEMO/LAMPWHITE khong, neu co thi lay du lieu
  {
    if(firebaseData.dataType() == "string") // sau khi lay duoc du lieu thi kiem ra xem du lieu co la kieu string 
    {
      WHITE_STATE=firebaseData.stringData().toInt();// neu co thi lay du lieu tu firebase chuyen thanh kieu int va gan cho bien trang thai led WHITE
      control_device(WHITE,WHITE_STATE);
    }
    
  }
  if(Firebase.get(firebaseData,"/BTL_NHUNG/RED"))// kiem tra xem co duong /DA1_DEMO/LEDRED khong, neu co thi lay du lieu
  {
    if(firebaseData.dataType() == "string") // sau khi lay duoc du lieu thi kiem ra xem du lieu co la kieu string 
    {
      RED_STATE=firebaseData.stringData().toInt();// neu co thi lay du lieu tu firebase chuyen thanh kieu int va gan cho bien trang thai led red
      control_device(RED,RED_STATE);
    }
  }
    if(Firebase.get(firebaseData,"/BTL_NHUNG/DO_SANG"))// kiem tra xem co duong /DA1_DEMO/LAMPWHITE khong, neu co thi lay du lieu
  {
    if(firebaseData.dataType() == "string") // sau khi lay duoc du lieu thi kiem ra xem du lieu co la kieu string 
    {
      DO_SANG = firebaseData.stringData().toInt();// neu co thi lay du lieu tu firebase chuyen thanh kieu int va gan cho bien trang thai led WHITE
    }
    
  }

  if(Firebase.get(firebaseData,"/BTL_NHUNG/FAN"))// kiem tra xem co duong /DA1_DEMO/LAMPWHITE khong, neu co thi lay du lieu
  {
    if(firebaseData.dataType() == "string") // sau khi lay duoc du lieu thi kiem ra xem du lieu co la kieu string 
    {
      FAN_STATE=firebaseData.stringData().toInt();// neu co thi lay du lieu tu firebase chuyen thanh kieu int va gan cho bien trang thai led WHITE
      control_device(FAN,FAN_STATE);
    }
    
  }
  if(Firebase.get(firebaseData,"/BTL_NHUNG/LED"))// kiem tra xem co duong /DA1_DEMO/LAMPWHITE khong, neu co thi lay du lieu
  {
    if(firebaseData.dataType() == "string") // sau khi lay duoc du lieu thi kiem ra xem du lieu co la kieu string 
    {
      FAN_STATE=firebaseData.stringData().toInt();// neu co thi lay du lieu tu firebase chuyen thanh kieu int va gan cho bien trang thai led WHITE
      control_device(FAN,FAN_STATE);
    }
    
  }
 
}

void Write_data_firebase(void)
{
  if(Firebase.setString(firebaseData,"/BTL_NHUNG/WHITE",String(WHITE_STATE)))// kiem tra tren firebase co duong dan /DA1_DEMO/LAMPWHITE khong,neu co ep kieu bien trang thai cua led WHITE va gui len firebase
  {
    Serial.println("du lieu da duoc gui thanh cong: ok");// in ra tren cong serial monitor du lieu da duoc gui thanh cong: ok
    Serial.print("PATH: ");
    Serial.println(firebaseData.dataPath());// in ra tren cong serial monitor duong dan cua csdl firebase
    Serial.print("TYPE: ");
    Serial.println(firebaseData.dataType());// in ra tren cong serial monitor kieu du lieu , vd la kieu string
    Serial.print("ETag: ");
    Serial.println(firebaseData.ETag());// in ra tren cong serial monitor cai tag, vd lampWHITE
    Serial.println("------------------------------------");// in ra tren cong serial monitor -----------------------------------------------
    Serial.println();// in ra tren cong serial monitor ki tu xuong dong

  }
  else
  {
    Serial.println("du lieu bi loi roi, gui lai di");// in ra tren cong serial monitor du lieu bi loi roi, gui lai di
    Serial.print("REASON: " );
    Serial.println(firebaseData.errorReason());// in ra tren cong serial monitor li do bi loi khi truyen du lieu len firebase
    Serial.println("------------------------------------");// in ra tren cong serial monitor ----------------------------------------------------
    Serial.println();// in ra tren cong serial monitor ki tu xuong dong
  }
  delay(100); // delay 300ms de ranh xung dot  du lieu

  Firebase.setString(firebaseData,"/BTL_NHUNG/RED",String(RED_STATE));// kiem tra tren firebase co duong dan /DA1_DEMO/LAMPWHITE khong,neu co ep kieu bien trang thai cua led WHITE va gui len firebase
  Firebase.setString(firebaseData,"/BTL_NHUNG/LED",String(LED_STATE));
  Firebase.setString(firebaseData,"/BTL_NHUNG/FAN",String(FAN_STATE));
  Firebase.setString(firebaseData,"/BTL_NHUNG/WHITE",String(RED_STATE));

}


void control_device(int device, int state)
{
  switch(device)
  {
    case WHITE:
    {
      digitalWrite(device, state);// ghi gia tri ra chan vi dieu khien, vd device blue va trang thai bang 0 thi thiet bi se tat
      break;
    }
    case RED:
     {digitalWrite(device, state);
      break;
     }
    case LED:
     {
      if(state)
      {
        dieukhiendosang(DO_SANG);
      }
      else
        dieukhiendosang(0);
      break;
     }
     case FAN:
     {
      digitalWrite(device, state);
      break;
     }
  }
}

void checkButtons()
{
  // Kiểm tra nút điều khiển đèn đỏ
  if (digitalRead(BUTTON_RED_PIN) == LOW)
  {
    delay(100);
    while(digitalRead(BUTTON_RED_PIN) == LOW);
    RED_STATE = !RED_STATE; // Chuyển đổi trạng thái đèn đỏ (tắt <-> bật)
    control_device(RED, RED_STATE);

  }

  // Kiểm tra nút điều khiển đèn xanh
  if (digitalRead(BUTTON_WHITE_PIN) == LOW)
  {
    delay(100);
    while(digitalRead(BUTTON_WHITE_PIN) == LOW);
    WHITE_STATE = !WHITE_STATE; // Chuyển đổi trạng thái đèn xanh (tắt <-> bật)
    control_device(WHITE, WHITE_STATE);

  }

  // Kiểm tra nút điều khiển đèn vàng
  if (digitalRead(BUTTON_FAN_PIN) == LOW)
  {
    delay(100);
    while(digitalRead(BUTTON_FAN_PIN) == LOW);
    FAN_STATE = !FAN_STATE; // Chuyển đổi trạng thái đèn vàng (tắt <-> bật)
    control_device(FAN, FAN_STATE);

  }

  if (digitalRead(BUTTON_LED_PIN) == LOW)
  {
    delay(100);
    while(digitalRead(BUTTON_LED_PIN) == LOW);
    FAN_STATE = !FAN_STATE; // Chuyển đổi trạng thái đèn vàng (tắt <-> bật)
    control_device(LED, LED_STATE);

  }
   
}

void Pwm_init(void) {
  ledcSetup(PWM_channel, Freg, Resolution);
  ledcAttachPin(LED, PWM_channel);
}

void dieukhiendosang(int value) {
  int dutyCycle = map(value, 0, 100, 0, 255);
  ledcWrite(PWM_channel, dutyCycle);
  delay(10);
}