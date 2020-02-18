

#include <MIDI.h>



//mic val last frame
int _micVal = 0;

//running millis
long timeRunning = 0;

//last hit time
long lastHit = 50;

//how long before next hit could happen
int timeThreshold = 30;

//how hard should the sensor be hit?
int intensityThreshold = 50;

//measure how hard the sensor will fly
int hitMaxIntensity = 0;

// have we reached the end of the peak yet?
bool peakReached = false;



//resulting running average from filter
float runningAverage;

//how heavy to smooth the input? closer to 1.0 is smoother
float smoothVal = 0.999;

//low pass filter definition
class LPFilter {
  public:
    float lastval;
    float val;

    virtual float update(float input, float freq) {
      val = input * (1.0 - freq);
      val = val + (lastval * freq);
      lastval = val;
      return val;
    }
};

//filter to smooth the input signal to generate a good callibration
LPFilter runningAverageFilter;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  Serial.println("Hi");

  initPixels();
}

void loop() {
  // put your main code here, to run repeatedly:
  timeRunning = millis();

  //read mic value
  int micVal = analogRead(23);

  //only do this every 10ms
  if (timeRunning % 10 == 0) {
    //  if (true) {
    runningAverage = runningAverageFilter.update(micVal, smoothVal);

//    Serial.print("0,1023,");
//    Serial.print(micVal);
//    Serial.print(",");
//    Serial.print(runningAverage);
//    Serial.print(",");
//    Serial.print(intensityThreshold);
//    Serial.println("");


  }

  if (timeRunning % 20 == 0) {
    updatePixels();
  }


  //the minumum value needed for a note trigger is the average plus a little bit
  intensityThreshold = runningAverage + 30;

  float intensityThresholdMin = runningAverage - 30;


  //detect mic hit
  if (micVal > intensityThreshold || micVal < intensityThresholdMin) {

    //check if it was not too soon after last hit
    if (timeRunning - lastHit > timeThreshold) {
      //      Serial.println("Real hit! Time since last = " + String(timeRunning - lastHit));



      //update last hit time
      lastHit = timeRunning;

      //reset intensity and peak reached
      hitMaxIntensity = 0;
      peakReached = false;

    }
  }

  //keep track of mic values the coming 50 ms after hit
  if (timeRunning - lastHit < 20 && !peakReached) {

    //if this val is higher than the previous
    if (micVal > hitMaxIntensity ) {
      //store it
      hitMaxIntensity = micVal;
      //      Serial.println(hitMaxIntensity);
    } else {
      //else if the val drops again
      //      Serial.println("END MAX = " + String(hitMaxIntensity));
      int note = 60;
      int velocity = map(hitMaxIntensity, 0, 1023, 0, 127);
      int channel = 1;
      usbMIDI.sendNoteOn(note, velocity, channel);
      //turn LED on
      pixelHit();
      digitalWrite(13, HIGH);
      peakReached = true;
      delay(20);
      usbMIDI.sendNoteOff(note, 0, channel);
      digitalWrite(13, LOW);

    }


  }

  //update last mic val
  _micVal = micVal;
}
