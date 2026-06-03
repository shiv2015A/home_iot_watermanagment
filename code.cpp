/*
====================================================
SMART HOME AUTOMATION + WATER LEAKAGE DETECTION
Optimized & Debugged Version
====================================================

Hardware:
- Arduino Uno
- PIR Motion Sensor
- Relay Module (Active Low / High configurable)
- 220V AC Bulb
- 2 x YF-S201 Water Flow Sensors

Author: Shivansh Dubey
====================================================
*/

// --- PINS ---
const int PIR_PIN = 7;
const int RELAY_PIN = 8;
const int FLOW_SENSOR_IN = 2;   // Interrupt 0
const int FLOW_SENSOR_OUT = 3;  // Interrupt 1

// --- CONSTANTS ---
const float PULSES_PER_LITRE = 450.0;
const float FLOW_CONSTANT = 7.5;
const unsigned long INTERVAL = 1000;

// Relay logic (Most 5V relay modules are active LOW)
// Change to HIGH if using an active-high relay module
const bool RELAY_ON = LOW; 
const bool RELAY_OFF = HIGH; 

// --- VOLATILE GLOBALS (For Interrupts) ---
volatile unsigned long pulseCountIn = 0;
volatile unsigned long pulseCountOut = 0;

// For tracking absolute volume without floating point truncation errors
volatile unsigned long globalPulseCountIn = 0;
volatile unsigned long globalPulseCountOut = 0;

unsigned long previousMillis = 0;

// --- INTERRUPT SERVICE ROUTINES ---
void countIn() {
    pulseCountIn++;
    globalPulseCountIn++;
}

void countOut() {
    pulseCountOut++;
    globalPulseCountOut++;
}

void setup() {
    Serial.begin(9600);

    // Initialize PIR Sensor
    pinMode(PIR_PIN, INPUT);

    // Initialize relay safely to OFF state before setting as OUTPUT
    digitalWrite(RELAY_PIN, RELAY_OFF);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, RELAY_OFF);

    // Initialize Flow Sensors with Pullups (required for YF-S201)
    pinMode(FLOW_SENSOR_IN, INPUT_PULLUP);
    pinMode(FLOW_SENSOR_OUT, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_IN), countIn, RISING);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_OUT), countOut, RISING);

    // Using F() macro to save SRAM for string literals
    Serial.println(F("================================="));
    Serial.println(F("SYSTEM INITIALIZED"));
    Serial.println(F("================================="));
}

void loop() {
    // -------------------------------------------------
    // 1. HOME AUTOMATION (Polled continually)
    // -------------------------------------------------
    int motion = digitalRead(PIR_PIN);
    if (motion == HIGH) {
        digitalWrite(RELAY_PIN, RELAY_ON);
    } else {
        digitalWrite(RELAY_PIN, RELAY_OFF);
    }

    // -------------------------------------------------
    // 2. FLOW MONITORING (Time-triggered)
    // -------------------------------------------------
    unsigned long currentMillis = millis();

    // Prevent drift over time by adding INTERVAL instead of setting to currentMillis
    if (currentMillis - previousMillis >= INTERVAL) {
        previousMillis += INTERVAL;

        // Safely read volatile variables
        noInterrupts();
        unsigned long pulsesIn = pulseCountIn;
        unsigned long pulsesOut = pulseCountOut;
        pulseCountIn = 0;
        pulseCountOut = 0;
        
        unsigned long totalPulsesIn = globalPulseCountIn;
        unsigned long totalPulsesOut = globalPulseCountOut;
        interrupts();

        // --- Instantaneous Flow Rate ---
        float flowRateIn = pulsesIn / FLOW_CONSTANT;
        float flowRateOut = pulsesOut / FLOW_CONSTANT;

        // --- Volume Calculation ---
        // BUG FIX: Calculate total volume directly from total pulses to prevent
        // floating point accumulated truncation error over long uptimes.
        float totalVolumeIn = totalPulsesIn / PULSES_PER_LITRE;
        float totalVolumeOut = totalPulsesOut / PULSES_PER_LITRE;

        // --- Leak Detection ---
        float volumeDifference = totalVolumeIn - totalVolumeOut;

        // --- Serial Monitor Output ---
        Serial.println();
        Serial.println(F("---------- WATER DATA ----------"));

        Serial.print(F("Flow Rate In : "));
        Serial.print(flowRateIn);
        Serial.println(F(" L/min"));

        Serial.print(F("Flow Rate Out: "));
        Serial.print(flowRateOut);
        Serial.println(F(" L/min"));

        Serial.print(F("Total Volume In : "));
        Serial.print(totalVolumeIn);
        Serial.println(F(" L"));

        Serial.print(F("Total Volume Out: "));
        Serial.print(totalVolumeOut);
        Serial.println(F(" L"));

        Serial.print(F("Water Loss: "));
        Serial.print(volumeDifference);
        Serial.println(F(" L"));

        // --- Leak Decision ---
        // Threshold check to avoid false positives from sensor noise/bubbles
        if (volumeDifference > 0.25) {
            Serial.println(F("ALERT: POSSIBLE LEAK DETECTED (LOSS > 0.25L)"));
        } else if (volumeDifference < -0.10) {
            Serial.println(F("WARNING: SENSOR CALIBRATION ISSUE (OUT > IN)"));
        } else {
            Serial.println(F("STATUS: NORMAL"));
        }

        // --- Motion Status ---
        Serial.println();
        Serial.print(F("Motion Status: "));
        if (motion == HIGH) {
            Serial.println(F("DETECTED"));
        } else {
            Serial.println(F("NO MOTION"));
        }
        Serial.println(F("-------------------------------"));
    }
}
