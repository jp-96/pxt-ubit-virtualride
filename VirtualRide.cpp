#include "pxt.h"

namespace virtualride {

    int lastResistanceLevel=1;

    //%
    void advertise() {
        return;
    }

    //%
    void setResistanceLevel(int resistanceLevel) {
        lastResistanceLevel = resistanceLevel;
    }
    
    //%
    void notifyIndoorBikeData(int speed100, int cadence2, int power) {
        return;
    }

    // Coefficient of Cadence and Speed
    static const uint64_t K_CRANK_CADENCE =  120000000;
    static const uint64_t K_CRANK_SPEED   = 1800000000;

    // https://diary.cyclekikou.net/archives/15876
    static const double K_POWER = 0.8 * (70 * 9.80665) / (360 * 0.95 * 100); // weight(70kg)
    static const double K_INCLINE_A = 0.9; // Incline(%) - a
    static const double K_INCLINE_B = 0.6; // Incline(%) - b

    //%
    void calcAndNotifyIndoorBikeData(int crankIntervalTime) {
        int speed100;
        int cadence2;
        int power;
        if (crankIntervalTime<=0)
        {
            cadence2 = 0;
            speed100 = 0;
            power = 0;
        }
        else
        {
            cadence2  = (int)( K_CRANK_CADENCE / crankIntervalTime );
            speed100 = (int)( K_CRANK_SPEED   / crankIntervalTime );
            // https://diary.cyclekikou.net/archives/15876
            power = (int)( (double)(speed100) * (K_INCLINE_A * ((double)lastResistanceLevel + K_INCLINE_B) * K_POWER) );
        }
        notifyIndoorBikeData(speed100, cadence2, power);
    }

    /**
     * The Fitness Machine Control Point is used by a Client to control certain behaviors of the Server. 
     */
    enum virtualrideFMCP {
        //% block="Set Target Resistance Level Procedure"
        SET_TARGET_RESISTANCE_LEVEL = 0x04,
        //% block="Set Indoor Bike Simulation Parameters Procedure"
        SET_INDOOR_BIKE_SIMULATION = 0x11
    };

    //%
    void onFitnessMachineControlPoint(virtualrideFMCP op, Action body) {
        // https://www.sciencedirect.com/science/article/pii/S1383762118306088
        // 接続
        // registerWithDal(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, body);
    }

    //%
    int getTargetResistanceLevel10() {
        return 10;
    }

    //%
    int getGrade100() {
        return 0;
    }

}