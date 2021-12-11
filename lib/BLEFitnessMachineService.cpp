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

#include "pxt.h"
#include "struct/include/struct/struct.h"
#include "BLEFitnessMachineService.h"

//================================================================
#if MICROBIT_CODAL
//================================================================

BLEFitnessMachineServiceDal::BLEFitnessMachineServiceDal(BLEDevice &_ble) :
        ble(_ble)
{
    //
}

void BLEFitnessMachineServiceDal::notifyIndoorBikeData(uint32_t speed100, uint32_t cadence2, int32_t resistanceLevel, int32_t power)
{
    //
}

uint8_t BLEFitnessMachineServiceDal::getTargetResistanceLevel10()
{
    //
    return 10;
}

int16_t BLEFitnessMachineServiceDal::getGrade100()
{
    //
    return 0;
}

//================================================================
#else // MICROBIT_CODAL
//================================================================

#include "ble/UUID.h"

BLEFitnessMachineServiceDal::BLEFitnessMachineServiceDal(BLEDevice &_ble) :
        ble(_ble)
{
    //   
}

void BLEFitnessMachineServiceDal::notifyIndoorBikeData(uint32_t speed100, uint32_t cadence2, int32_t resistanceLevel, int32_t power)
{
    //
}

uint8_t BLEFitnessMachineServiceDal::getTargetResistanceLevel10()
{
    //
    return 10;
}

int16_t BLEFitnessMachineServiceDal::getGrade100()
{
    //
    return 0;
}

//================================================================
#endif // MICROBIT_CODAL
//================================================================

BLEFitnessMachineService::BLEFitnessMachineService()
{
    pBLEFitnessMachineServiceDal = new BLEFitnessMachineServiceDal(*uBit.ble);
}

void BLEFitnessMachineService::notifyIndoorBikeData(uint32_t speed100, uint32_t cadence2, int32_t resistanceLevel, int32_t power)
{
    pBLEFitnessMachineServiceDal->notifyIndoorBikeData( speed100,  cadence2,  resistanceLevel,  power);
}

uint8_t BLEFitnessMachineService::getTargetResistanceLevel10()
{
    return pBLEFitnessMachineServiceDal->getTargetResistanceLevel10();
}

int16_t BLEFitnessMachineService::getGrade100()
{
    return pBLEFitnessMachineServiceDal->getGrade100();
}
