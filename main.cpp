#include <Arduino.h>

#define FIR_FILTER_LENGTH 16

typedef struct {
  float buf[FIR_FILTER_LENGTH];
   uint8_t bufIndex;
  float out;
}FIRFilter;

void FIRFilter_init(FIRFilter *fir);
float FIRFilter_update(FIRFilter *fir, float inp);

static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH]  = {0.0010846f,0.0019684f,0.0043198f,0.0083349f,0.0136848f,0.0195712f,0.0249198f,0.0286580f,0.0300000f,0.0286580f,0.0249198f,0.0195712f,0.0136848f,0.0083349f,0.0043198f,0.0019684f};

  FIRFilter lpFilter;

void setup() {
  Serial.begin(9600);
  analogReadResolution(16);
FIRFilter_init(&lpFilter);
}

void loop() {
  float voltage = 3.3 * analogRead(A0)/ 65536.0;
  FIRFilter_update(&lpFilter,voltage);
  Serial.print(">unfiltered:");
  Serial.println(voltage);
  Serial.print(">filtered:");
  Serial.println(FIRFilter_update(&lpFilter,voltage));

}

void FIRFilter_init(FIRFilter *fir)
{

  //clear filter buffer 
  for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++)
  {
    fir->buf[n] = 0.0f;

  }
  // reset buffer index
  fir->bufIndex = 0; 

  //clear filter output
  fir->out = 0.0f;
}
float FIRFilter_update(FIRFilter *fir, float inp)
{

 //store latest sample in buffer
 fir->buf[fir->bufIndex] = inp;
 

 //increment buffer index 
fir->bufIndex++;
if(fir->bufIndex == FIR_FILTER_LENGTH)
{
   fir->bufIndex = 0;
}

 //compute new output sample (convolution)
 fir->out = 0.0f;
uint8_t sumIndex = fir->bufIndex;

 for(uint8_t n = 0; n< FIR_FILTER_LENGTH; n++)
 {
   //decrement index
    if(sumIndex > 0)
    {
       sumIndex--;
    }else{
      sumIndex = FIR_FILTER_LENGTH-1;
    }
   //multiply impulse response with shifted sample and add to ouput
   fir->out += FIR_IMPULSE_RESPONSE[n] * fir->buf[sumIndex];
 }
 //return filtered output
 return fir->out;
}
