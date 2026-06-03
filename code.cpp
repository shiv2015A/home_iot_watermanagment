/*
  IoT Home Automation + Water Leakage Detection
  Reconstructed implementation

  Hardware:
  - Arduino Uno
  - PIR Motion Sensor
  - LED
  - 2 x YF-S201 Water Flow Sensors
*/

const byte FLOW_SENSOR_1 = 2;   // Interrupt 0
const byte FLOW_SENSOR_2 = 3;   // Interrupt 1

const byte PIR_PIN = 7;
const byte LED_PIN = 13;

volatile unsigned long pulseCount1 = 0;
volatile unsigned long pulseCount2 = 0;

unsigned long previousMillis = 0;
const unsigned long interval = 1000; // 1 second

float totalVolume1 = 0.0;
float totalVolume2 = 0.0;

// YF-S201 calibration
const float PULSES_PER_LITRE = 450.0;

void countSensor1()
{
    pulseCount1++;
}

void countSensor2()
{
    pulseCount2++;
}

void setup()
{
    Serial.begin(9600);

    pinMode(PIR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);

    pinMode(FLOW_SENSOR_1, INPUT_PULLUP);
    pinMode(FLOW_SENSOR_2, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_1), countSensor1, RISING);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_2), countSensor2, RISING);

    Serial.println("System Started");
}

void loop()
{
    // --------------------------
    // Motion Detection
    // --------------------------

    int motion = digitalRead(PIR_PIN);

    if (motion == HIGH)
    {
        digitalWrite(LED_PIN, HIGH);
    }
    else
    {
        digitalWrite(LED_PIN, LOW);
    }

    // --------------------------
    // Flow Monitoring
    // --------------------------

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        noInterrupts();

        unsigned long pulses1 = pulseCount1;
        unsigned long pulses2 = pulseCount2;

        pulseCount1 = 0;
        pulseCount2 = 0;

        interrupts();

        // Flow rate calculation
        float flowRate1 = pulses1 / 7.5; // L/min
        float flowRate2 = pulses2 / 7.5; // L/min

        // Volume calculation
        float litres1 = pulses1 / PULSES_PER_LITRE;
        float litres2 = pulses2 / PULSES_PER_LITRE;

        totalVolume1 += litres1;
        totalVolume2 += litres2;

        float leakAmount = totalVolume1 - totalVolume2;

        Serial.println("--------------------------------");

        Serial.print("Flow Rate 1: ");
        Serial.print(flowRate1);
        Serial.println(" L/min");

        Serial.print("Flow Rate 2: ");
        Serial.print(flowRate2);
        Serial.println(" L/min");

        Serial.print("Total Volume In : ");
        Serial.print(totalVolume1);
        Serial.println(" L");

        Serial.print("Total Volume Out: ");
        Serial.print(totalVolume2);
        Serial.println(" L");

        Serial.print("Estimated Loss: ");
        Serial.print(leakAmount);
        Serial.println(" L");

        if (leakAmount > 0.5)
        {
            Serial.println("WARNING: Possible Leak Detected");
        }
        else
        {
            Serial.println("System Status: Normal");
        }
    }
}
