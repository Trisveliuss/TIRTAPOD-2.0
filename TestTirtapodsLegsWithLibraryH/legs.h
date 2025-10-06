#include <SoftwareSerial.h>
int state_nextStep;
namespace legs {
const float cx = 3.1;  //coxa
const float fm = 5;    //femur
const float tb = 5.5;  // tibia
float L, L1;
float alpha, alpha1, alpha2, alpha3, gama, beta;
int beta2;
String sudut;
int Se, sudut2, sudut3;
int Titik0 = 1500;
float x, y, z;
int X, Y, Z;
bool maju5 = true;
unsigned long int state_lastMoveRecord = 0;
unsigned long int tPutar = 0;

SoftwareSerial servoserial(2, 3);


void setup() {
  Serial.begin(38400);
  servoserial.begin(9600);
}

//=================================START CODE FUNCTION OF CONVERT==========================================///
//Konversi Sudut
String konversisudut(int x) {
  String sudut = String(x);

  return sudut;
}
int myFunctionPWM(float sudut) {
  float result, result2;
  result = 1000 * sudut;
  result2 = result / 90;

  return result2;
}

//==================================END CODE FUNCTION OF CONVERT=========================================

//========================================= StartPosisi Kaki Awal/TItik 0 TIap Kaki================
int CX0 = 1450;
int FM0 = 1430;
int TB0 = 1500;
float K0X = 0;
float K0Y = 8.8;
float K0Z = 11.6;


int walkspeed = 200;  //between 200-500 for better stability and 150 for better speed

//=========================================END Posisi Kaki Awal/TItik 0 TIap Kaki=========

//===================================Start Get Sudut=============================================

float getgama(float x, float y, float z) {
  L1 = sqrt(sq(x) + sq(y));
  gama = atan(x / y) / PI * 180;
  L = sqrt(sq(L1 - cx) + sq(z));
  beta = acos((sq(tb) + sq(fm) - sq(L)) / (2 * tb * fm)) / PI * 180;
  alpha1 = acos(z / L) / PI * 180;
  alpha2 = acos((sq(fm) + sq(L) - sq(tb)) / (2 * fm * L)) / PI * 180;
  alpha = alpha1 + alpha2;
  return gama;
}
float getalpha(float x, float y, float z) {
  L1 = sqrt(sq(x) + sq(y));
  gama = atan(x / y) / PI * 180;
  L = sqrt(sq(L1 - cx) + sq(z));
  beta = acos((sq(tb) + sq(fm) - sq(L)) / (2 * tb * fm)) / PI * 180;
  alpha1 = acos(z / L) / PI * 180;
  alpha2 = acos((sq(fm) + sq(L) - sq(tb)) / (2 * fm * L)) / PI * 180;
  alpha = alpha1 + alpha2;
  beta2 = 90 - beta;
  alpha2 = alpha;
  return alpha2;
}
float getbeta(float x, float y, float z) {
  L1 = sqrt(sq(x) + sq(y));
  gama = atan(x / y) / PI * 180;
  L = sqrt(sq(L1 - cx) + sq(z));
  beta = acos((sq(tb) + sq(fm) - sq(L)) / (2 * tb * fm)) / PI * 180;
  alpha1 = acos(z / L) / PI * 180;
  alpha2 = acos((sq(fm) + sq(L) - sq(tb)) / (2 * fm * L)) / PI * 180;
  alpha = alpha1 + alpha2;
  beta2 = beta;
  alpha2 = 90 - alpha;
  return beta2;
}
//===================================End of Get Sudut=============================================


//===================================Start of Matriks Langkah====================================

String konversi_maju(int x) {
  float K1[3][3] = {
    { K0X + 0, K0Y + 0, K0Z + 2 },
    { K0X + 5, K0Y + 0, K0Z + (-3) },
    { K0X + 5, K0Y + 0, K0Z + 0 }
  };
  int KakiGama = round(CX0 + myFunctionPWM(getgama(K1[x][0], K1[x][1], K1[x][2])));
  int KakiAlpha = round(FM0 + myFunctionPWM(getalpha(K1[x][0], K1[x][1], K1[x][2])));
  int KakiBeta = round(TB0 + myFunctionPWM(getbeta(K1[x][0], K1[x][1], K1[x][2])));


  Serial.println("Gama : " + String(getgama(K1[x][0], K1[x][1], K1[x][2])));
  Serial.println("Alpha : " + String(getalpha(K1[x][0], K1[x][1], K1[x][2])));
  Serial.println("Beta : " + String(getbeta(K1[x][0], K1[x][1], K1[x][2])));

  String hasil = "#0 P" + String(KakiGama) + " #1 P" + String(KakiAlpha) + " #2 P" + String(KakiBeta) + "T" + String(walkspeed) + " <cr>";

  return hasil;
}

//===================================End of Matriks Langkah====================================

void testkoordinat() {
  servoserial.println(String(konversi_maju(0)));
}

void fungsimaju2() {
  switch (state_nextStep) {
    case 0:
      servoserial.println(String(konversi_maju(0)));
      state_nextStep = 1;
      break;
    case 1:
      servoserial.println(String(konversi_maju(1)));
      state_nextStep = 2;
      break;
    case 2:
      servoserial.println(String(konversi_maju(2)));
      state_nextStep = 0;
      break;
  }
}
void maju(int keep) {
  if (millis() - state_lastMoveRecord >= walkspeed) {
    state_lastMoveRecord = millis();
    fungsimaju2();
  }
}
void standby() {
  servoserial.println(String(konversi_maju(0)));
}
// void rotate (unsigned int rotate=0) {
//   if (rotate > 0 && maju5==true ) {
//     unsigned int startCounter = millis();
//     unsigned int currentCounter = millis();
//     while ((currentCounter - startCounter) < rotate) {
//       maju();
//       currentCounter = millis();
//       Serial.println(currentCounter - startCounter);
//       if(currentCounter - startCounter>=rotate){
//         maju5=false;
//       }
//     }
//   }

// }

}
