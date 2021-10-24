#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 125


#define DATA_PIN 7
#define CLOCK_PIN 13 //not used


    struct points {
      //each induvidual unit time length of strip take 6% of memory
        uint8_t x;
        uint8_t y;
        uint8_t ySc; // scaled up to 256 y
    };

    
// Define the array of leds
CRGB leds[NUM_LEDS];
points ledsPoints[NUM_LEDS];
CRGB randomColors[3];

void setup() { 

  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  addxyCor();

  randomColors[0] = CHSV(0,255,127);
  randomColors[1] = CHSV(85,0,127);
  randomColors[2] = CHSV(170,255,127);

}


uint8_t cycler=253;
uint8_t lastcycler=0;
uint8_t whichMode=4;
uint8_t timerSeconds;
uint8_t lastTime;


bool beenDone = true;
void loop() { 
  
 //  timerSeconds=millis()>>6;
   timerSeconds=millis()>>10; //10 would give you seconds


  //this is okay but having a second time variable may be easier;
  //or if((byte)timerSeconds=millis()>>10 < timerSeconds){ //this may be the way to do it
if(timerSeconds%20 == 0 && beenDone ){  beenDone = false;
  whichMode++;
  whichMode%=7; 
//  whichMode =0  ;//stay on 0 xxx
}else if( timerSeconds%10 > 0 && !beenDone){ beenDone=true; }


lastTime = timerSeconds;




if(whichMode == 1)
for(uint8_t i =0; i<NUM_LEDS; i++){
  
 leds[i] = CHSV(ledsPoints[i].x + cycler, 255,127);
}

if(whichMode == 2)
for(uint8_t i =0; i<NUM_LEDS; i++){
  
 leds[i] = CHSV((i*2) + cycler , 255,127);
}

#define center 127


//sine wave gradient
if(whichMode == 5 ){
byte rotation = cycler;
for(uint8_t i =0; i<NUM_LEDS; i++){

  uint16_t gradientNumber =  sin8(cycler)+ledsPoints[i].x;
  if(gradientNumber>255){
    
  leds[i] = blend( randomColors[1] , randomColors[2], gradientNumber );
  }else{
  leds[i] = blend( randomColors[0] , randomColors[1], gradientNumber );

  }
}

}



//rotating rainbow
if(whichMode == 6  ){
float radian = cycler*3.14;
radian /= 128.02;//180 / 1.406 256 to 360 convertion

for(uint8_t i =0; i<NUM_LEDS; i++){
  
  
  int16_t xTrans =  cos(radian)*(ledsPoints[i].x -40);

      xTrans = xTrans + ((sin(radian)*(ledsPoints[i].ySc-127)+40));


 leds[i] = CHSV(xTrans  , 255,127);

 /*
 if(xTrans < 0){
 leds[i] = CHSV(0 , 255,127);
 }else if(xTrans > 255){
 leds[i] = CHSV(0 , 255,127); }
*/

    

  }
}

//rotating gradient
if(whichMode == 3 || whichMode == 4 ){
 static uint16_t mover;
mover+=2;
float radian = cycler*3.14;
radian /= 128.02;//180 / 1.406 256 to 360 convertion
points centerP;
centerP.x = mover>>8;
centerP.y = 127;
for(uint8_t i =0; i<NUM_LEDS; i++){
  
  
int16_t xTrans =  cos(radian)*(ledsPoints[i].x-centerP.x);

      xTrans = xTrans + ((sin(radian)*(ledsPoints[i].ySc-centerP.y)+centerP.y));
   // xTrans /= 0x;

  //cos(angle)(x-centerX)+sin(angle)(y-centerY)+centerX,//repeate for y

  if(xTrans>255){
    
  leds[i] = blend( randomColors[1] , randomColors[2], xTrans );
  }else{
  leds[i] = blend( randomColors[2] , randomColors[1], xTrans );
  }
  
  uint16_t xTransint = xTrans; //has to be positive for gradients.
  if(xTransint>512){
 leds[i] =randomColors[2];
  }else if(xTransint<0){
    randomColors[1];
  }
    

  }
}




//randome gradient moving from right to left
if(whichMode == 0){
if(cycler == 0){
 uint8_t rando = random8();

  //when we are at the end of the cycle 
 //shift the colors down 0<1<2 new 2.
 randomColors[0] =randomColors[1]; 
 randomColors[1] =randomColors[2];
  

 
 if(rando < 16){  randomColors[2] =  CHSV(0,255,0);  } //sometimes black
 else if(rando > 240){  randomColors[2] =  CHSV(0,0,127);   }//sometime white
 else{ randomColors[2] =  CHSV(rando,255,127);}
}

for(uint8_t i =0; i<NUM_LEDS; i++){
  uint16_t gradientNumber =  ledsPoints[i].x + cycler; 
  
  if(gradientNumber>255){
    
  leds[i] = blend( randomColors[1] , randomColors[2], gradientNumber );
  }else{
  leds[i] = blend( randomColors[0] , randomColors[1], gradientNumber );

  }

}
}//which gradiant to gradient

  for(uint8_t i =0; i<NUM_LEDS; i++){
 // leds[i] = CRGB(100,100,100);
  }
  FastLED.show();
  delay(3);
  cycler++;
}


void addxyCor(){
ledsPoints[0].x = 131;   ledsPoints[0].y = 51;   ledsPoints[0].ySc = 136;   
ledsPoints[1].x = 135;   ledsPoints[1].y = 55;   ledsPoints[1].ySc = 146;   
ledsPoints[2].x = 139;   ledsPoints[2].y = 58;   ledsPoints[2].ySc = 155;   
ledsPoints[3].x = 143;   ledsPoints[3].y = 62;   ledsPoints[3].ySc = 165;   
ledsPoints[4].x = 148;   ledsPoints[4].y = 66;   ledsPoints[4].ySc = 174;   
ledsPoints[5].x = 152;   ledsPoints[5].y = 69;   ledsPoints[5].ySc = 184;   
ledsPoints[6].x = 156;   ledsPoints[6].y = 73;   ledsPoints[6].ySc = 193;   
ledsPoints[7].x = 160;   ledsPoints[7].y = 76;   ledsPoints[7].ySc = 203;   
ledsPoints[8].x = 164;   ledsPoints[8].y = 80;   ledsPoints[8].ySc = 212;   
ledsPoints[9].x = 168;   ledsPoints[9].y = 83;   ledsPoints[9].ySc = 221;   
ledsPoints[10].x = 172;   ledsPoints[10].y = 87;   ledsPoints[10].ySc = 231;   
ledsPoints[11].x = 177;   ledsPoints[11].y = 89;   ledsPoints[11].ySc = 237;   
ledsPoints[12].x = 179;   ledsPoints[12].y = 92;   ledsPoints[12].ySc = 244;   
ledsPoints[13].x = 187;   ledsPoints[13].y = 94;   ledsPoints[13].ySc = 248;   
ledsPoints[14].x = 192;   ledsPoints[14].y = 95;   ledsPoints[14].ySc = 252;   
ledsPoints[15].x = 198;   ledsPoints[15].y = 96;   ledsPoints[15].ySc = 254;   
ledsPoints[16].x = 203;   ledsPoints[16].y = 96;   ledsPoints[16].ySc = 254;   
ledsPoints[17].x = 208;   ledsPoints[17].y = 96;   ledsPoints[17].ySc = 254;   
ledsPoints[18].x = 213;   ledsPoints[18].y = 95;   ledsPoints[18].ySc = 252;   
ledsPoints[19].x = 219;   ledsPoints[19].y = 94;   ledsPoints[19].ySc = 248;   
ledsPoints[20].x = 224;   ledsPoints[20].y = 92;   ledsPoints[20].ySc = 243;   
ledsPoints[21].x = 229;   ledsPoints[21].y = 89;   ledsPoints[21].ySc = 237;   
ledsPoints[22].x = 233;   ledsPoints[22].y = 87;   ledsPoints[22].ySc = 229;   
ledsPoints[23].x = 238;   ledsPoints[23].y = 83;   ledsPoints[23].ySc = 221;   
ledsPoints[24].x = 242;   ledsPoints[24].y = 79;   ledsPoints[24].ySc = 211;   
ledsPoints[25].x = 245;   ledsPoints[25].y = 75;   ledsPoints[25].ySc = 200;   
ledsPoints[26].x = 248;   ledsPoints[26].y = 71;   ledsPoints[26].ySc = 188;   
ledsPoints[27].x = 251;   ledsPoints[27].y = 66;   ledsPoints[27].ySc = 175;   
ledsPoints[28].x = 252;   ledsPoints[28].y = 61;   ledsPoints[28].ySc = 162;   
ledsPoints[29].x = 254;   ledsPoints[29].y = 56;   ledsPoints[29].ySc = 148;   
ledsPoints[30].x = 255;   ledsPoints[30].y = 50;   ledsPoints[30].ySc = 134;   
ledsPoints[31].x = 255;   ledsPoints[31].y = 48;   ledsPoints[31].ySc = 127;   
ledsPoints[32].x = 255;   ledsPoints[32].y = 45;   ledsPoints[32].ySc = 120;   
ledsPoints[33].x = 254;   ledsPoints[33].y = 40;   ledsPoints[33].ySc = 106;   
ledsPoints[34].x = 252;   ledsPoints[34].y = 35;   ledsPoints[34].ySc = 92;   
ledsPoints[35].x = 251;   ledsPoints[35].y = 29;   ledsPoints[35].ySc = 79;   
ledsPoints[36].x = 248;   ledsPoints[36].y = 25;   ledsPoints[36].ySc = 66;   
ledsPoints[37].x = 245;   ledsPoints[37].y = 20;   ledsPoints[37].ySc = 54;   
ledsPoints[38].x = 242;   ledsPoints[38].y = 16;   ledsPoints[38].ySc = 43;   
ledsPoints[39].x = 238;   ledsPoints[39].y = 12;   ledsPoints[39].ySc = 33;   
ledsPoints[40].x = 233;   ledsPoints[40].y = 9;   ledsPoints[40].ySc = 25;   
ledsPoints[41].x = 229;   ledsPoints[41].y = 6;   ledsPoints[41].ySc = 17;   
ledsPoints[42].x = 224;   ledsPoints[42].y = 4;   ledsPoints[42].ySc = 11;   
ledsPoints[43].x = 219;   ledsPoints[43].y = 2;   ledsPoints[43].ySc = 6;   
ledsPoints[44].x = 213;   ledsPoints[44].y = 1;   ledsPoints[44].ySc = 2;   
ledsPoints[45].x = 208;   ledsPoints[45].y = 0;   ledsPoints[45].ySc = 0;   
ledsPoints[46].x = 203;   ledsPoints[46].y = 0;   ledsPoints[46].ySc = 0;   
ledsPoints[47].x = 198;   ledsPoints[47].y = 0;   ledsPoints[47].ySc = 0;   
ledsPoints[48].x = 192;   ledsPoints[48].y = 1;   ledsPoints[48].ySc = 2;   
ledsPoints[49].x = 187;   ledsPoints[49].y = 2;   ledsPoints[49].ySc = 6;   
ledsPoints[50].x = 179;   ledsPoints[50].y = 4;   ledsPoints[50].ySc = 10;   
ledsPoints[51].x = 177;   ledsPoints[51].y = 6;   ledsPoints[51].ySc = 17;   
ledsPoints[52].x = 172;   ledsPoints[52].y = 8;   ledsPoints[52].ySc = 23;   
ledsPoints[53].x = 168;   ledsPoints[53].y = 12;   ledsPoints[53].ySc = 33;   
ledsPoints[54].x = 164;   ledsPoints[54].y = 16;   ledsPoints[54].ySc = 42;   
ledsPoints[55].x = 160;   ledsPoints[55].y = 19;   ledsPoints[55].ySc = 51;   
ledsPoints[56].x = 156;   ledsPoints[56].y = 23;   ledsPoints[56].ySc = 61;   
ledsPoints[57].x = 152;   ledsPoints[57].y = 26;   ledsPoints[57].ySc = 70;   
ledsPoints[58].x = 148;   ledsPoints[58].y = 30;   ledsPoints[58].ySc = 80;   
ledsPoints[59].x = 143;   ledsPoints[59].y = 33;   ledsPoints[59].ySc = 89;   
ledsPoints[60].x = 139;   ledsPoints[60].y = 37;   ledsPoints[60].ySc = 99;   
ledsPoints[61].x = 135;   ledsPoints[61].y = 41;   ledsPoints[61].ySc = 108;   
ledsPoints[62].x = 131;   ledsPoints[62].y = 44;   ledsPoints[62].ySc = 118;   
ledsPoints[63].x = 127;   ledsPoints[63].y = 48;   ledsPoints[63].ySc = 127;   
ledsPoints[64].x = 123;   ledsPoints[64].y = 51;   ledsPoints[64].ySc = 136;   
ledsPoints[65].x = 119;   ledsPoints[65].y = 55;   ledsPoints[65].ySc = 146;   
ledsPoints[66].x = 115;   ledsPoints[66].y = 58;   ledsPoints[66].ySc = 155;   
ledsPoints[67].x = 111;   ledsPoints[67].y = 62;   ledsPoints[67].ySc = 165;   
ledsPoints[68].x = 106;   ledsPoints[68].y = 66;   ledsPoints[68].ySc = 174;   
ledsPoints[69].x = 102;   ledsPoints[69].y = 69;   ledsPoints[69].ySc = 184;   
ledsPoints[70].x = 98;   ledsPoints[70].y = 73;   ledsPoints[70].ySc = 193;   
ledsPoints[71].x = 94;   ledsPoints[71].y = 76;   ledsPoints[71].ySc = 203;   
ledsPoints[72].x = 90;   ledsPoints[72].y = 80;   ledsPoints[72].ySc = 212;   
ledsPoints[73].x = 86;   ledsPoints[73].y = 83;   ledsPoints[73].ySc = 221;   
ledsPoints[74].x = 82;   ledsPoints[74].y = 87;   ledsPoints[74].ySc = 231;   
ledsPoints[75].x = 77;   ledsPoints[75].y = 89;   ledsPoints[75].ySc = 237;   
ledsPoints[76].x = 75;   ledsPoints[76].y = 92;   ledsPoints[76].ySc = 244;   
ledsPoints[77].x = 67;   ledsPoints[77].y = 94;   ledsPoints[77].ySc = 248;   
ledsPoints[78].x = 62;   ledsPoints[78].y = 95;   ledsPoints[78].ySc = 252;   
ledsPoints[79].x = 56;   ledsPoints[79].y = 96;   ledsPoints[79].ySc = 254;   
ledsPoints[80].x = 51;   ledsPoints[80].y = 96;   ledsPoints[80].ySc = 254;   
ledsPoints[81].x = 46;   ledsPoints[81].y = 96;   ledsPoints[81].ySc = 254;   
ledsPoints[82].x = 41;   ledsPoints[82].y = 95;   ledsPoints[82].ySc = 252;   
ledsPoints[83].x = 35;   ledsPoints[83].y = 94;   ledsPoints[83].ySc = 248;   
ledsPoints[84].x = 30;   ledsPoints[84].y = 92;   ledsPoints[84].ySc = 243;   
ledsPoints[85].x = 25;   ledsPoints[85].y = 89;   ledsPoints[85].ySc = 237;   
ledsPoints[86].x = 21;   ledsPoints[86].y = 87;   ledsPoints[86].ySc = 229;   
ledsPoints[87].x = 16;   ledsPoints[87].y = 83;   ledsPoints[87].ySc = 221;   
ledsPoints[88].x = 12;   ledsPoints[88].y = 79;   ledsPoints[88].ySc = 211;   
ledsPoints[89].x = 9;   ledsPoints[89].y = 75;   ledsPoints[89].ySc = 200;   
ledsPoints[90].x = 6;   ledsPoints[90].y = 71;   ledsPoints[90].ySc = 188;   
ledsPoints[91].x = 3;   ledsPoints[91].y = 66;   ledsPoints[91].ySc = 175;   
ledsPoints[92].x = 2;   ledsPoints[92].y = 61;   ledsPoints[92].ySc = 162;   
ledsPoints[93].x = 0;   ledsPoints[93].y = 45;   ledsPoints[93].ySc = 120;   
ledsPoints[94].x = 0;   ledsPoints[94].y = 40;   ledsPoints[94].ySc = 106;   
ledsPoints[95].x = 2;   ledsPoints[95].y = 35;   ledsPoints[95].ySc = 92;   
ledsPoints[96].x = 3;   ledsPoints[96].y = 29;   ledsPoints[96].ySc = 79;   
ledsPoints[97].x = 6;   ledsPoints[97].y = 25;   ledsPoints[97].ySc = 66;   
ledsPoints[98].x = 9;   ledsPoints[98].y = 20;   ledsPoints[98].ySc = 54;   
ledsPoints[99].x = 12;   ledsPoints[99].y = 16;   ledsPoints[99].ySc = 43;   
ledsPoints[100].x = 16;   ledsPoints[100].y = 12;   ledsPoints[100].ySc = 33;   
ledsPoints[101].x = 21;   ledsPoints[101].y = 9;   ledsPoints[101].ySc = 25;   
ledsPoints[102].x = 25;   ledsPoints[102].y = 6;   ledsPoints[102].ySc = 17;   
ledsPoints[103].x = 30;   ledsPoints[103].y = 4;   ledsPoints[103].ySc = 11;   
ledsPoints[104].x = 35;   ledsPoints[104].y = 2;   ledsPoints[104].ySc = 6;   
ledsPoints[105].x = 41;   ledsPoints[105].y = 1;   ledsPoints[105].ySc = 2;   
ledsPoints[106].x = 46;   ledsPoints[106].y = 0;   ledsPoints[106].ySc = 0;   
ledsPoints[107].x = 51;   ledsPoints[107].y = 0;   ledsPoints[107].ySc = 0;   
ledsPoints[108].x = 56;   ledsPoints[108].y = 0;   ledsPoints[108].ySc = 0;   
ledsPoints[109].x = 62;   ledsPoints[109].y = 1;   ledsPoints[109].ySc = 2;   
ledsPoints[110].x = 67;   ledsPoints[110].y = 2;   ledsPoints[110].ySc = 6;   
ledsPoints[111].x = 75;   ledsPoints[111].y = 4;   ledsPoints[111].ySc = 10;   
ledsPoints[112].x = 77;   ledsPoints[112].y = 6;   ledsPoints[112].ySc = 17;   
ledsPoints[113].x = 82;   ledsPoints[113].y = 8;   ledsPoints[113].ySc = 23;   
ledsPoints[114].x = 86;   ledsPoints[114].y = 12;   ledsPoints[114].ySc = 33;   
ledsPoints[115].x = 90;   ledsPoints[115].y = 16;   ledsPoints[115].ySc = 42;   
ledsPoints[116].x = 94;   ledsPoints[116].y = 19;   ledsPoints[116].ySc = 51;   
ledsPoints[117].x = 98;   ledsPoints[117].y = 23;   ledsPoints[117].ySc = 61;   
ledsPoints[118].x = 102;   ledsPoints[118].y = 26;   ledsPoints[118].ySc = 70;   
ledsPoints[119].x = 106;   ledsPoints[119].y = 30;   ledsPoints[119].ySc = 80;   
ledsPoints[120].x = 111;   ledsPoints[120].y = 33;   ledsPoints[120].ySc = 89;   
ledsPoints[121].x = 115;   ledsPoints[121].y = 37;   ledsPoints[121].ySc = 99;   
ledsPoints[122].x = 119;   ledsPoints[122].y = 41;   ledsPoints[122].ySc = 108;   
ledsPoints[123].x = 123;   ledsPoints[123].y = 44;   ledsPoints[123].ySc = 118;
ledsPoints[124].x = 126;   ledsPoints[124].y = 48;   ledsPoints[124].ySc = 127;
}
