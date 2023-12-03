/* COPY TEST*/
// #include <Arduino.h>
#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <string.h>


//Den
#define  RED 8
#define WHITE 15
#define  FAN 7
#define LED 0
#define tat 0
#define bat 1

// define pwm
#define PWM_channel 8 // chon kenh 8
#define Freg 20000  // led duoc su dung voi tan so 20khz
#define Resolution 8

// button 
#define BUTTON_RED_PIN 16    // Chân cho nút điều khiển đèn đỏ
#define BUTTON_WHITE_PIN 4  // Chân cho nút điều khiển đèn xanh
#define BUTTON_FAN_PIN 5 // Chân cho nút điều khiển quat
#define BUTTON_LED_PIN  17

//Door
#define ROW1 23
#define ROW2 22
#define ROW3 1
#define ROW4 3
#define COL1 21
#define COL2 19
#define COL3 18
#define KHOA 2

const int ROW_NUM    = 4;
const int COLUMN_NUM = 3;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

//LCD
#define RS 32
#define EN 33
#define D4 25
#define D5 26
#define D6 27
#define D7 14

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

#define WIFI_SSID "Ducnguyen"
#define WIFI_PASSWORD "11110000"

#define FIREBASE_HOST "https://esp32-ac263-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "yOCt9u60Y2BPBRag3JISLFlc0x1iEnZblPwiJ8MZ"

FirebaseData firebaseData;


byte pin_rows[ROW_NUM] = {ROW1, ROW2, ROW3, ROW4};
byte pin_column[COLUMN_NUM] = {COL1, COL2, COL3};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

char curectPassword[] = "1234"; // Mật khẩu mặc định
char chagePassword[] = "0601";

int RED_STATE=0, WHITE_STATE=0, FAN_STATE=0, LED_STATE = 0;

int Button_red_pressed = 0;
int Button_white_pressed = 0;
int Button_fan_pressed = 0;
int Button_led_pressed = 0;
int Keypad_pressed = 0;


void initWiFi(){
  Serial.begin(9600);  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");                                                                                                                                                                                          
  while (WiFi.status() != WL_CONNECTED ){
  Serial.print(".");
  delay(1000);
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}


void Firebase_init()
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.reconnectWiFi(true);
}


void Setup_device()
{
  pinMode(RED,OUTPUT);
  pinMode(WHITE,OUTPUT);
  pinMode(LED,OUTPUT);
  digitalWrite(RED,LOW);
  digitalWrite(WHITE,LOW); 
  digitalWrite(LED,LOW);
}
void control_device(int device, int state)
{
  switch(device)
  {
    case WHITE:
    {
      digitalWrite(device, state);// ghi gia tri ra chan vi dieu khien, vd device blue va trang thai bang 0 thi thiet bi se tat
      Serial.print("Den trang gia tri = ");
      Serial.println(state);
      break;
    }
    case RED:
     {digitalWrite(device, state);
      Serial.print("Den do gia tri = ");
      Serial.println(state);
      break;
     }
     case LED:
     {digitalWrite(device, state);
      Serial.print("Den led gia tri = ");
      Serial.println(state);
      break;
     }
     case FAN:
     {digitalWrite(device, state);
      Serial.print("fan gia tri = ");
      Serial.println(state);
      break;
     }
  }  
}

void Read_data_firebase()
{
  if(Firebase.get(firebaseData,"/BTL_NHUNG/WHITE"))// kiem tra xem co duong /DA1_DEMO/LAMPGREEN khong, neu co thi lay du lieu
  {
    if(firebaseData.dataType() == "string") // sau khi lay duoc du lieu thi kiem ra xem du lieu co la kieu string 
    {
      WHITE_STATE=firebaseData.stringData().toInt();// neu co thi lay du lieu tu firebase chuyen thanh kieu int va gan cho bien trang thai led green
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
  if(Firebase.get(firebaseData,"/BTL_NHUNG/LED"))// kiem tra xem co duong /DA1_DEMO/LAMPGREEN khong, neu co thi lay du lieu
  {
    if(firebaseData.dataType() == "string") // sau khi lay duoc du lieu thi kiem ra xem du lieu co la kieu string 
    {
      LED_STATE=firebaseData.stringData().toInt();// neu co thi lay du lieu tu firebase chuyen thanh kieu int va gan cho bien trang thai led green
      control_device(LED,LED_STATE);
    }
  }
  if(Firebase.get(firebaseData,"/BTL_NHUNG/FAN"))// kiem tra xem co duong /DA1_DEMO/LAMPGREEN khong, neu co thi lay du lieu
  {
    if(firebaseData.dataType() == "string") // sau khi lay duoc du lieu thi kiem ra xem du lieu co la kieu string 
    {
      FAN_STATE=firebaseData.stringData().toInt();// neu co thi lay du lieu tu firebase chuyen thanh kieu int va gan cho bien trang thai led green
      control_device(FAN,FAN_STATE);
    }
    
  }
}

void Write_data_firebase()
{
  if(Firebase.setString(firebaseData,"/BTL_NHUNG/WHITE",String(WHITE_STATE)))// kiem tra tren firebase co duong dan /DA1_DEMO/LAMPGREEN khong,neu co ep kieu bien trang thai cua led GREEN va gui len firebase
  {
    Serial.println("du lieu da duoc gui thanh cong: ok");// in ra tren cong serial monitor du lieu da duoc gui thanh cong: ok
    Serial.print("PATH: ");
    Serial.println(firebaseData.dataPath());// in ra tren cong serial monitor duong dan cua csdl firebase
    Serial.print("TYPE: ");
    Serial.println(firebaseData.dataType());// in ra tren cong serial monitor kieu du lieu , vd la kieu string
    Serial.print("ETag: ");
    Serial.println(firebaseData.ETag());// in ra tren cong serial monitor cai tag, vd lampgreen
    Serial.println("------------------------------------");// in ra tren cong serial monitor -----------------------------------------------
    Serial.println();// in ra tren cong serial monitor ki tu xuong dong
  }
  else
  {
    Serial.println("du lieu bi loi roi, gui lai di");// in ra tren cong serial monitor du lieu bi loi roi, gui lai di
    Serial.print("REASON: ");
    Serial.println(firebaseData.errorReason());// in ra tren cong serial monitor li do bi loi khi truyen du lieu len firebase
    Serial.println("------------------------------------");// in ra tren cong serial monitor ----------------------------------------------------
    Serial.println();// in ra tren cong serial monitor ki tu xuong dong
  }
  Firebase.setString(firebaseData,"/BTL_NHUNG/FAN",String(FAN_STATE));
  // kiem tra tren firebase co duong dan /DA1_DEMO/LAMPGREEN khong,neu co ep kieu bien trang thai cua led GREEN va gui len firebase
  Firebase.setString(firebaseData,"/BTL_NHUNG/LED",String(WHITE_STATE));

  Firebase.setString(firebaseData,"/BTL_NHUNG/RED",String(WHITE_STATE));
}


bool checkPassword() {
  char enteredPassword[5]; // Duy trì một mảng để lưu trữ mật khẩu nhập vào
  lcd.setCursor(0, 1);
  lcd.print("                "); // Xóa dòng hiển thị mật khẩu
  lcd.setCursor(0, 1);
  
  // Nhập mật khẩu từ keypad và hiển thị trên LCD
  for (int i = 0; i < 4; ++i) {
    char key = keypad.getKey();
    if (key) {
      enteredPassword[i] = key;
      lcd.print('*');
    }
  }
  
  // Kiểm tra mật khẩu
  enteredPassword[4] = '\0'; // Kết thúc chuỗi
  return strcmp(enteredPassword, curectPassword) == 0;
}

void updatePassword(char key) {
  static char newPassword[5]; // Duy trì một mảng để lưu trữ mật khẩu mới
  static int newPasswordIndex = 0;
  char testPassword[5];

  for (int i = 0; i < 4; ++i) {
    char key = keypad.getKey();
    if (key) {
      testPassword[i] = key;
    }
  }
    testPassword[4] = '\0';
    int k = strcmp(testPassword, chagePassword) ;

  // Nếu nhấn nút 11223344 thì chế độ đổi mật khẩu được kích hoạt

  if (newPasswordIndex == 0 && k == 0) {
    newPassword[0] = key;
    newPasswordIndex = 1;
    lcd.clear();
    lcd.print("Enter New Password:");
    lcd.setCursor(0, 1);
  } else if (newPasswordIndex > 0) {
    // Nhập mật khẩu mới từ keypad và hiển thị trên LCD
    newPassword[newPasswordIndex] = key;
    lcd.print('*');
    newPasswordIndex++;

    // Khi nhập xong, cập nhật mật khẩu và hiển thị thông báo
    if (newPasswordIndex == 4) {
      newPassword[4] = '\0'; // Kết thúc chuỗi
      strcpy(curectPassword, newPassword);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Password Updated");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Enter Password:");
      newPasswordIndex = 0;
    }
  }
}

int tempkhoa;

void check_door(){
  char key = keypad.getKey();

  if (key) {
    Serial.print("Gia tri nhap :");
    Serial.println(key);
    if (key == '#') {
      // Kiểm tra mật khẩu và mở cửa nếu đúng
      if (checkPassword()) {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Access Granted");
        digitalWrite(KHOA, HIGH);
        Serial.println("Door is opened");
        tempkhoa = 1;
        // Mã điều khiển cổng mở cửa ở đây
      } else {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Access Denied");
      }
      delay(200);
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Enter Password:");
    } else {
      updatePassword(key);
    }
  }
}

void tat_khoa(){
  if(tempkhoa ==1){
    delay(5000);
    tempkhoa = 0;
    digitalWrite(KHOA,LOW);
    Serial.println("Door is closing");
  }
}

void write_buttons(){
  if (digitalRead(BUTTON_RED_PIN)==LOW){
    if(RED_STATE == 0){
      Firebase.setString(firebaseData,"/BTL_NHUNG/RED",1);
    }else{
      Firebase.setString(firebaseData,"/BTL_NHUNG/RED",0);
    }
  }

  if (digitalRead(BUTTON_WHITE_PIN)==LOW){
    if(WHITE_STATE == 0){
      Firebase.setString(firebaseData,"/BTL_NHUNG/WHITE",1);
    }else{
      Firebase.setString(firebaseData,"/BTL_NHUNG/WHITE",0);
    }
  }

  if (digitalRead(BUTTON_FAN_PIN)==LOW){
    if(FAN_STATE == 0){
      Firebase.setString(firebaseData,"/BTL_NHUNG/FAN",1);
    }else{
      Firebase.setString(firebaseData,"/BTL_NHUNG/FAN",0);
    }
  }
  if (digitalRead(BUTTON_LED_PIN)==LOW){
    if(LED_STATE == 0){
      Firebase.setString(firebaseData,"/BTL_NHUNG/LED",1);
    }else{
      Firebase.setString(firebaseData,"/BTL_NHUNG/LED",0);
    }
  }
}

void checkButtons()
{
  // Kiểm tra nút điều khiển đèn đỏ
  if (digitalRead(BUTTON_RED_PIN)==LOW)
  {
    Button_red_pressed = 1;
    Serial.print("Button den do gia tri la :");
    Serial.println(Button_red_pressed);
    while(digitalRead(BUTTON_RED_PIN)==LOW);
    RED_STATE = !RED_STATE; // Chuyển đổi trạng thái đèn đỏ (tắt <-> bật)
    control_device(RED, RED_STATE);
  }else{
    Button_red_pressed = 0;
    Serial.print("Button den do gia tri la :");
    Serial.println(Button_red_pressed);
  }

  // Kiểm tra nút điều khiển đèn xanh
  if (digitalRead(BUTTON_WHITE_PIN)==LOW)
  {
    Button_white_pressed = 1;
    Serial.print("Button den trang gia tri la :");
    Serial.println(Button_white_pressed);
    while(digitalRead(BUTTON_WHITE_PIN)==LOW);
    WHITE_STATE = !WHITE_STATE; // Chuyển đổi trạng thái đèn xanh (tắt <-> bật)
    control_device(WHITE, WHITE_STATE);
  }else{
    Button_red_pressed = 0;
    Serial.print("Button den trang gia tri la :");
    Serial.println(Button_white_pressed);
  }

  // Kiểm tra nút điều khiển đèn vàng
  if (digitalRead(BUTTON_FAN_PIN)==LOW)
  {
    while(digitalRead(BUTTON_FAN_PIN)==LOW);
    FAN_STATE = !FAN_STATE; // Chuyển đổi trạng thái đèn vàng (tắt <-> bật)
    control_device(FAN, FAN_STATE);
  }

  if (digitalRead(BUTTON_LED_PIN)==LOW){
    while(digitalRead(BUTTON_LED_PIN)==LOW);
    LED_STATE = !LED_STATE;
    control_device(LED,LED_STATE);
  }
   
}


void dieukhiendosang(int value) {
  int dutyCycle = map(value, 0, 100, 0, 255);
  ledcWrite(PWM_channel, dutyCycle);
  delay(10);
}

void setup()
{
  // khoi tao nut nhan 
  pinMode(BUTTON_RED_PIN, INPUT_PULLUP);
  pinMode(BUTTON_WHITE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_FAN_PIN,INPUT_PULLUP);
  pinMode(BUTTON_LED_PIN, INPUT_PULLUP);
  
  initWiFi();
  Firebase_init();
  Setup_device();
  Write_data_firebase();

  lcd.begin(16, 2);
  lcd.setCursor(3,1);
  lcd.print("Nhap mat khau");

}

void loop()
{
  Read_data_firebase();
  write_buttons();
  checkButtons();
}

