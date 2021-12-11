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

/**
* Use this file to define custom functions and blocks.
* Read more at https://makecode.microbit.org/blocks/custom
*/

// enums: see VirtualRide.cpp, and also enums.d.ts

/**
 * Custom blocks  
 * icon: a Unicode identifier for an icon from the Font Awesome icon set.  
 *       http://fontawesome.io/icons  
 *       icon="\uf206" https://fontawesome.com/v5.15/icons/bicycle  
 */
//% weight=100 color=#fd7e14 icon="\uf206"
//% block="Virtual Ride"
namespace virtualride {

    /**
     * Start and advertise the Fitness Machine Service.
     */
    //% blockId=virtualride_advertise
    //% block="Start the Fitness Machine Service"
    //% shim=virtualride::advertise
    export function advertise(): void {
        return;
    }

    /**
     * Set resistanceLevel 
     * @param resistanceLevel Resistance Level (1 - 8)
     */
    //% blockId=virtualride_set_resistance_level
    //% block="Set Resistance Level %resistanceLevel"
    //% resistanceLevel.min=1 resistanceLevel.max=8 resistanceLevel.defl=1
    //% shim=virtualride::setResistanceLevel
    export function setResistanceLevel(resistanceLevel: number): void {
        return;
    }
    
    /**
     * The Indoor Bike Data characteristic is used to send training-related data to the Client from an indoor bike (Server). 
     * @param speed100 Instantaneous Speed (uint16), Kilometer per hour with a resolution of 0.01, eg: 42.20km/h -> 4220. 
     * @param cadence2 Instantaneous Cadence (uint16), 1/minute with a resolution of 0.5, eg: 60.5rpm -> 121.
     * @param power Instantaneous Power (sint16), Watts with a resolution of 1, eg:200watt -> 200
     */
    //% blockId=virtualride_notify_indoor_bike_data
    //% block="Notify Indoor Bike Data speed100:=%speed100 cadence2:=%cadence2 power:=%power"
    //% speed100.min=0 speed100.max=9900 speed100.defl=3500
    //% cadence2.min=0 cadence2.max=1000 cadence2.defl=240
    //% power.min=0 power.max=500 power.defl=220
    //% shim=virtualride::notifyIndoorBikeData
    export function notifyIndoorBikeData(speed100: number, cadence2: number, power: number): void {
        return;
    }

    /**
     * Calc speed100, cadence2, power by crankIntervalTime and resistanceLevel, and then call notifyIndoorBikeData. 
     * @param crankIntervalTime Crank Interval Time (microsecond)
     */
    //% blockId=virtualride_calc_and_notify_indoor_bike_data
    //% block="Calc And Notify Indoor Bike Data CrankIntervalTime:=%crankIntervalTime"
    //% crankIntervalTime.min=0 crankIntervalTime.max=120000000 crankIntervalTime.defl=1000000
    //% shim=virtualride::calcAndNotifyIndoorBikeData
    export function calcAndNotifyIndoorBikeData(crankIntervalTime: number): void {
        return;
    }

    /**
     * Register code to run when requested procedure through the Fitness Machine Control Point characteristic.
     * @param op The Fitness Machine Control Point Procedure.
     * @param body Code to run when the Fitness Machine Control Point characteristic requested.
     */
    //% blockId=virtualride_on_fitness_machine_control_point
    //% block="On %op through Fitness Machine Control Point"
    //% shim=virtualride::onFitnessMachineControlPoint
    export function onFitnessMachineControlPoint(op: virtualrideFMCP, body: () => void): void {
        return;
    }

    /**
     * Get Target Resistance Level through the `Set Target Resistance Level Procedure`.
     * Target Resistance Level, UINT8, Unitless with a resolution of 0.1.
     */
    //% blockId=virtualride_get_target_resistance_level_10
    //% block="Get Target Resistance Level"
    //% shim=virtualride::getTargetResistanceLevel10
    export function getTargetResistanceLevel10(): number {
        return 10;
    }

    /**
     * Get Grade through the `Set Indoor Bike Simulation Parameters Procedure`.
     * Grade, SINT16, Percentage with a resolution of 0.01.
     */
    //% blockId=virtualride_get_grade_100
    //% block="Get Simulation Grade"
    //% shim=virtualride::getGrade100
    export function getGrade100(): number {
        return 0;
    }

}
