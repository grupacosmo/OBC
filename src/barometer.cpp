#include "barometer.hpp"

#include <Arduino.h>

namespace obc
{
    void init(BMP &bmp)
    {
        Serial.println("Initializing BMP module");

        if(!bmp.begin())
        {
            Serial.println("BMP init failed. Please check connections "
            "or reboot");
            while(true) {}
        }
        else 
            Serial.println("BMP init success!\n\n");

        bmp.setOversampling(4);
    }

    bmpMeasurements bmp_measurements(BMP &bmp)
    {
        char result = bmp.startMeasurment();
        Measurements temp;

        if(result == 0) { return {{}, Error::Busy}; }
        else 
        {
            result = bmp.getTemperatureAndPressure(temp.temperature, temp.pressure);
            if(result == 0) { return {{}, Error::Busy}; }
            else
            {
                temp.altitude = bmp.altitude(temp.pressure, a_pr);
                return
                {
                    {temp.temperature, temp.pressure, temp.altitude}, 
                    Error::Ok
                };
            }
        } 
    }

    void print_bmp_measurements(Measurements measurements)
    {
        Serial.print(measurements.temperature);
        Serial.print("\t");
        Serial.print(measurements.pressure);
        Serial.print("\t");
        Serial.print(measurements.altitude);
        Serial.println();
    }
}