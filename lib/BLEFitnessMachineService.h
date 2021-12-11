/*
MIT License

Copyright (c) 2021 jp-96

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef BLE_FITNESS_MACHINE_SERVICE_H
#define BLE_FITNESS_MACHINE_SERVICE_H

#include "pxt.h"

//================================================================
#if MICROBIT_CODAL
//================================================================

#include "MicroBitBLEManager.h"
#include "MicroBitBLEService.h"

class BLEFitnessMachineServiceDal //: public MicroBitBLEService
{
    public:

    /**
      * Constructor.
      * @param _ble The instance of a BLE device that we're running on.
      */
    BLEFitnessMachineServiceDal(BLEDevice &_ble);

    /**
     * The Indoor Bike Data characteristic is used to send training-related data to the Client from an indoor bike (Server). 
     * @param speed100 Instantaneous Speed (uint16), Kilometer per hour with a resolution of 0.01, eg: 42.20km/h -> 4220. 
     * @param cadence2 Instantaneous Cadence (uint16), 1/minute with a resolution of 0.5, eg: 60.5rpm -> 121.
     * @param resistanceLevel Resistance Level (sint16), Unitless with a resolution of 1
     * @param power Instantaneous Power (sint16), Watts with a resolution of 1, eg:200watt -> 200
     */
    void notifyIndoorBikeData(uint32_t speed100, uint32_t cadence2, int32_t resistanceLevel, int32_t power);

    /**
     * Get Target Resistance Level through the `Set Target Resistance Level Procedure`.
     * Target Resistance Level, UINT8, Unitless with a resolution of 0.1.
     */
    uint8_t getTargetResistanceLevel10();

    /**
     * Get Grade through the `Set Indoor Bike Simulation Parameters Procedure`.
     * Grade, SINT16, Percentage with a resolution of 0.01.
     */
    int16_t getGrade100();

    private:

    // Bluetooth stack we're running on.
    BLEDevice &ble;

};

//================================================================
#else // MICROBIT_CODAL
//================================================================

#include "ble/BLE.h"

class BLEFitnessMachineServiceDal
{
    public:

    /**
      * Constructor.
      * @param _ble The instance of a BLE device that we're running on.
      */
    BLEFitnessMachineServiceDal(BLEDevice &_ble);

    /**
     * The Indoor Bike Data characteristic is used to send training-related data to the Client from an indoor bike (Server). 
     * @param speed100 Instantaneous Speed (uint16), Kilometer per hour with a resolution of 0.01, eg: 42.20km/h -> 4220. 
     * @param cadence2 Instantaneous Cadence (uint16), 1/minute with a resolution of 0.5, eg: 60.5rpm -> 121.
     * @param resistanceLevel Resistance Level (sint16), Unitless with a resolution of 1
     * @param power Instantaneous Power (sint16), Watts with a resolution of 1, eg:200watt -> 200
     */
    void notifyIndoorBikeData(uint32_t speed100, uint32_t cadence2, int32_t resistanceLevel, int32_t power);

    /**
     * Get Target Resistance Level through the `Set Target Resistance Level Procedure`.
     * Target Resistance Level, UINT8, Unitless with a resolution of 0.1.
     */
    uint8_t getTargetResistanceLevel10();

    /**
     * Get Grade through the `Set Indoor Bike Simulation Parameters Procedure`.
     * Grade, SINT16, Percentage with a resolution of 0.01.
     */
    int16_t getGrade100();

    private:

    // Bluetooth stack we're running on.
    BLEDevice &ble;
    
};

//================================================================
#endif // MICROBIT_CODAL
//================================================================

class BLEFitnessMachineService
{
    
    public:

    /**
      * Constructor.
      * Create a representation of the BLE Fitness Machine Service
      * 
      */
    BLEFitnessMachineService();

    /**
     * The Indoor Bike Data characteristic is used to send training-related data to the Client from an indoor bike (Server). 
     * @param speed100 Instantaneous Speed (uint16), Kilometer per hour with a resolution of 0.01, eg: 42.20km/h -> 4220. 
     * @param cadence2 Instantaneous Cadence (uint16), 1/minute with a resolution of 0.5, eg: 60.5rpm -> 121.
     * @param resistanceLevel Resistance Level (sint16), Unitless with a resolution of 1
     * @param power Instantaneous Power (sint16), Watts with a resolution of 1, eg:200watt -> 200
     */
    void notifyIndoorBikeData(uint32_t speed100, uint32_t cadence2, int32_t resistanceLevel, int32_t power);

    /**
     * Get Target Resistance Level through the `Set Target Resistance Level Procedure`.
     * Target Resistance Level, UINT8, Unitless with a resolution of 0.1.
     */
    uint8_t getTargetResistanceLevel10();

    /**
     * Get Grade through the `Set Indoor Bike Simulation Parameters Procedure`.
     * Grade, SINT16, Percentage with a resolution of 0.01.
     */
    int16_t getGrade100();

private:

    // instance
    BLEFitnessMachineServiceDal* pBLEFitnessMachineServiceDal;

};

#endif