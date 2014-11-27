#include <gamma.h>
#include <RGBmatrixPanel.h>kj

#include <Adafruit_GFX.h>   // Core graphics library


//Uno
//#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
//#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
//#define LAT A3
//#define OE  9
//#define A   A0
//#define B   A1
//#define C   A2

//Mega
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT 10
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define PNLS 2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);

void setup() {
  Serial3.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false); 
}

void createGraphColumns(int column_number, int column, int row, int size, int bar, int color){
   for(int i = 0; i < column_number; i++){
     matrix.drawFastGraphLine(column+i, row, size, bar, color); 
   }
}

void loop() {
  matrix.fillScreen(0);

  int brightness = 70;
  int saturation = 254;

  int red = matrix.ColorHSV(5, saturation, brightness, true);            //4096, 8192, 1226, 1126X, 1638X, 2045X, 2452X, 2867X, 
  int yellow = matrix.ColorHSV(255, saturation, brightness, true);       //
  int lt_blue = matrix.ColorHSV(775, saturation, brightness, true);
  int blue = matrix.ColorHSV(900, saturation, brightness, true);         //2, 4, 6, 134, 136, 138, 140, 142, 
  //int violet = matrix.ColorHSV(1200, saturation, brightness, true);
  int pink = matrix.ColorHSV(1255, saturation, brightness, true);
  int green = matrix.ColorHSV(425, saturation, brightness, true); 
  int white = matrix.Color333(7,7,7); 
  int bright_yellow = matrix.Color333(7,7,0);
  int light_white = matrix.ColorHSV(70, 10, 70, true);
  int bright_red = matrix.Color333(7,0,0);

  /*  begin rpm display */
  
  int rpm;
  //boolean rpm_read = obd.read(PID_RPM, rpm);
  boolean rpm_read = false;
  
  int size = 8;
  int value = rpm_read ? rpm * 0.001 : random(750,8250); //remove when obd is reading
  int sing_value = value >= 1000 ? ((value % 10000) * 0.001) : 0;
  int sing_value1 = value >= 100 ? ((value% 1000) * 0.01) : 0;
  int sing_value2 = value >= 10 ? ((value % 100) * 0.1) : 0;
  int sing_value3 = value >= 1 ? (value % 10) : 0;
  int column = 4;
  int column2 = column+20;
  int column_number = 3;
  int row = 0;  
  
  int shift_light = 6;

  if(sing_value >= shift_light){
    int shift_light_color = sing_value > 6 ? bright_red : bright_yellow;
    createGraphColumns(5, column+column_number+6, row+1, 2, 2, shift_light_color);
  }

  int rpm_color = blue; 
  createGraphColumns(column_number, column, size, 1, 1, rpm_color);
  createGraphColumns(column_number, column2, size, 1, 1, rpm_color);

  if(sing_value > 0){ 
    rpm_color = lt_blue; 
    int bar = sing_value > 2 ? 2 : sing_value;
    createGraphColumns(column_number, column, row, size, bar, rpm_color);
    createGraphColumns(column_number, column2, row, size, bar, rpm_color);

    if(sing_value > 2){
      rpm_color = green;
      int bar = sing_value > 5 ? 4 : sing_value - 2;
      createGraphColumns(column_number, column, row, size-2, bar, rpm_color);
      createGraphColumns(column_number, column2, row, size-2, bar, rpm_color);

      if(sing_value > 5){
        rpm_color = yellow;
        int bar = 1;
        createGraphColumns(column_number, column, row, size-5, bar, rpm_color);
        createGraphColumns(column_number, column2, row, size-5, bar, rpm_color);
        
        if(sing_value > 6){
          rpm_color = red;
          int bar = sing_value > 8 ? 2 : sing_value - 6;
          createGraphColumns(column_number, column, row, size-6, bar, rpm_color);
          createGraphColumns(column_number, column2, row, size-6, bar, rpm_color);
        }
      }
    }
  }

  int rpm_number_row = 4;
  int rpm_column_number = column+4;
  int rpm_number_color = light_white;
  if(sing_value > 0){
    matrix.draw3x5digit(rpm_column_number,rpm_number_row,sing_value,rpm_number_color);
  }
  matrix.draw3x5digit(rpm_column_number+4,rpm_number_row,sing_value1,rpm_number_color);
  matrix.draw3x5digit(rpm_column_number+8,rpm_number_row,sing_value2,rpm_number_color);
  matrix.draw3x5digit(rpm_column_number+12,rpm_number_row,sing_value3,rpm_number_color);  
  /*  end rpm display */
 
  /*  begin speed  display */
  int mph;
  int speed_column = column-4;
  int speed_number_row = 10;
  int car_speed = false ? mph : random(0,121); //remove when obd is reading
  int speed_color = yellow;
  if(car_speed > 99){
      int speed_zero = (car_speed % 1000) * 0.01;
      matrix.draw3x5digit(speed_column,speed_number_row,speed_zero,speed_color);
  }
  if(car_speed > 9){
      int speed_one = (car_speed % 100) * 0.1;
      matrix.draw3x5digit(speed_column+4,speed_number_row,speed_one,speed_color);
  }
  if(car_speed >= 0){
      int speed_two = (car_speed % 10);
      matrix.draw3x5digit(speed_column+8,speed_number_row,speed_two,speed_color);
  }
  
  /*  end speed  display */
 
  /* begin water temp display */
 
  int water_temp = random(1,10);
  if(water_temp < 3){
    matrix.drawFastGraphLine(31, 10, 3, water_temp, blue); 
  } else if(water_temp > 7){
    matrix.drawFastGraphLine(31, 6, 7, 7, lt_blue); 
    matrix.drawFastGraphLine(31, 3, 3, water_temp-7, red); 
  } else {
    matrix.drawFastGraphLine(31, 3, 10, water_temp, lt_blue); 
  }
 
  /* begin fuel guage display */
  
  int fuel_level = random(1,10);
  int fuel_color = fuel_level < 3 ? red : green;
  matrix.drawFastGraphLine(0, 3, 10, fuel_level, fuel_color); 
  
  /*  begin traction  display */
  
//  matrix.drawRect(27,1,1,2,randomWheelSlip(value, brightness));
//  matrix.drawRect(27,6,1,2,randomWheelSlip(value, brightness));
//  matrix.drawRect(31,1,1,2,randomWheelSlip(value, brightness));
//  matrix.drawRect(31,6,1,2,randomWheelSlip(value, brightness));

  /*  end traction  display */
  
  /*  begin g-force  display */
  
  //matrix.drawFastGraphLine(29,3,2,1,blue);

  /*  end g-force  display */

  // Update display
  matrix.swapBuffers(false);
  delay(500); // remove or lessen value when live in car
}
  /*
    notes on colors using HSV
   with brightness of 70 lowest before not displayed
   and saturation of 254 the highest
   broghtness     70           90            100           120         150           190
   0     red      0..230       0..190        0..170        0..140      0..110        0..90
   golden                                                        110..170      90..130
   golden-yellow                                                               130..170 
   240   yellow   240..274     195..325      170..345      140..200    170..210      170..210
   grn-ylw                                           200..300    210..300
   lt-grn                                            300..370    300..340      300..340
   neo grn                                                       340..400      340..400
   425   green    275..747     328..696      345..680      370..650    400..625      400..
   teal     750..780     698..837      680..855      650..720    625..680
   turq                                                          680..720
   led blue                                                      720..750
   775   lgt blue                                          720..810    750..780
   other blue                                                    780..810
   900   blue     781..1245    840..1208     855..1190     885..1155   855..910
   drk blue                                                      910..1155
   lavender                                          1155.1240   1155..1200
   cotton candy                                                  1200..1240
   lt pink                                           1240..1320  1240..1300
   1300  pink     1250..1290   1210..1349    1190..1365    1320..1395  1300..1360
   hot pink                                                      1360..1420
   1535  red      1300..1535   1350..1535    1365..1535    1395..1535  1420..1535
   
   
   */

