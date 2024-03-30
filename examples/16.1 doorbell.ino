#include "HomeSpan.h"

int doorbellPin = D8; // button on 3.3v

struct DEV_Doorbell : Service::Doorbell { // StatelessProgrammableSwitch
  SpanCharacteristic *switchEvent; // reference to the ProgrammableSwitchEvent Characteristic
  // constructor:
  DEV_Doorbell(int doorbellPin) : Service::Doorbell() { 
    switchEvent = new Characteristic::ProgrammableSwitchEvent(); // Programmable Switch Event Characteristic (will be set to SINGLE)
    new SpanButton(doorbellPin, 2000, 5, 200, SpanButton::TRIGGER_ON_HIGH); // create new SpanButton specifying a single press ( pin number ,debounce (time to avoid registering false triggers from mechanical noise after a span), span threshold (time to register a button press), span duration (time between two consecutive spans to prevent rapid presses from being registered) )
  }
  //loop:
  void button(int pin, int pressType) override {
    switchEvent->setVal(pressType);
  }
};

void setup() {
  Serial.begin(115200);
  homeSpan.begin(Category::ProgrammableSwitches, "Doorbell");
  homeSpan.enableWebLog(); // http://homespan-19ad214bea32.local/status
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
    new DEV_Doorbell(doorbellPin);
}

void loop() {
  homeSpan.poll();
}
