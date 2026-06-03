/*
====================================================
SMART HOME AUTOMATION + WATER LEAKAGE DETECTION
Reconstructed Version
====================================================

Hardware:
- Arduino Uno
- PIR Motion Sensor
- Relay Module
- 220V AC Bulb
- 2 x YF-S201 Water Flow Sensors

Author: Shivansh Dubey
====================================================
*/

const int PIR_PIN = 7;
const int RELAY_PIN = 8;

const int FLOW_SENSOR_IN = 2;   // Interrupt 0
const int FLOW_SENSOR_OUT = 3;  // Interrupt 1

volatile unsigned long pulseCountIn = 0;
volatile unsigned long pulseCountOut = 0;

float totalVolumeIn = 0;
float totalVolumeOut = 0;

unsigned long previousMillis = 0;
const unsigned long interval = 1000;

// YF-S201 Constants
const float PULSES_PER_LITRE = 450.0;
const float FLOW_CONSTANT = 7.5;

void countIn()
{
    pulseCountIn++;
}

void countOut()
{
    pulseCountOut++;
}

void setup()
{
    Serial.begin(9600);

    pinMode(PIR_PIN, INPUT);

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    pinMode(FLOW_SENSOR_IN, INPUT_PULLUP);
    pinMode(FLOW_SENSOR_OUT, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(FLOW_SENSOR_IN),
        countIn,
        RISING);

    attachInterrupt(
        digitalPinToInterrupt(FLOW_SENSOR_OUT),
        countOut,
        RISING);

    Serial.println("=================================");
    Serial.println("SYSTEM INITIALIZED");
    Serial.println("=================================");
}

void loop()
{
    //-------------------------------------------------
    // HOME AUTOMATION
    //-------------------------------------------------

    int motion = digitalRead(PIR_PIN);

    if (motion == HIGH)
    {
        digitalWrite(RELAY_PIN, HIGH);
    }
    else
    {
        digitalWrite(RELAY_PIN, LOW);
    }

    //-------------------------------------------------
    // FLOW MONITORING
    //-------------------------------------------------

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        noInterrupts();

        unsigned long pulsesIn = pulseCountIn;
        unsigned long pulsesOut = pulseCountOut;

        pulseCountIn = 0;
        pulseCountOut = 0;

        interrupts();

        //-------------------------------------------------
        // Instantaneous Flow Rate
        //-------------------------------------------------

        float flowRateIn =
            pulsesIn / FLOW_CONSTANT;

        float flowRateOut =
            pulsesOut / FLOW_CONSTANT;

        //-------------------------------------------------
        // Volume Calculation
        //-------------------------------------------------

        float litresIn =
            pulsesIn / PULSES_PER_LITRE;

        float litresOut =
            pulsesOut / PULSES_PER_LITRE;

        totalVolumeIn += litresIn;
        totalVolumeOut += litresOut;

        //-------------------------------------------------
        // Leak Detection
        //-------------------------------------------------

        float volumeDifference =
            totalVolumeIn - totalVolumeOut;

        //-------------------------------------------------
        // Serial Monitor Output
        //-------------------------------------------------

        Serial.println();
        Serial.println("---------- WATER DATA ----------");

        Serial.print("Flow Rate In : ");
        Serial.print(flowRateIn);
        Serial.println(" L/min");

        Serial.print("Flow Rate Out: ");
        Serial.print(flowRateOut);
        Serial.println(" L/min");

        Serial.print("Total Volume In : ");
        Serial.print(totalVolumeIn);
        Serial.println(" L");

        Serial.print("Total Volume Out: ");
        Serial.print(totalVolumeOut);
        Serial.println(" L");

        Serial.print("Water Loss: ");
        Serial.print(volumeDifference);
        Serial.println(" L");

        //-------------------------------------------------
        // Leak Decision
        //-------------------------------------------------

        if (volumeDifference > 0.25)
        {
            Serial.println("ALERT: POSSIBLE LEAK DETECTED");
        }
        else
        {
            Serial.println("STATUS: NORMAL");
        }

        //-------------------------------------------------
        // Motion Status
        //-------------------------------------------------

        Serial.println();
        Serial.print("Motion Status: ");

        if (motion)
        {
            Serial.println("DETECTED");
        }
        else
        {
            Serial.println("NO MOTION");
        }

        Serial.println("-------------------------------");
    }
}
