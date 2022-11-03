/*
MIT License

Copyright (c) 2021-2022 jp-96

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

// Advertising includes
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "ble_dis.h"

// base uuid : x00000000-0000-1000-8000-00805F9B34FB
const uint8_t  BLEFitnessMachineServiceDal::service_base_uuid[ 16] =
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9b,0x34,0xfb };

const uint8_t  BLEFitnessMachineServiceDal::char_base_uuid[ 16] =
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9b,0x34,0xfb };

// UUID(0x1826) FTMS
const uint16_t BLEFitnessMachineServiceDal::serviceUUID               = 0x1826;
// UUID(0x2AD2) mbbs_cIdxIndoorBikeData,
// UUID(0x2AD9) mbbs_cIdxFitnessMachineControlPoint,
// UUID(0x2ACC) mbbs_cIdxFitnessMachineFeature,
// UUID(0x2ADA) mbbs_cIdxFitnessMachineStatus,
// UUID(0x2AD3) mbbs_cIdxFitnessTrainingStatus,
// UUID(0x2AD6) mbbs_cIdxFitnessSupportedResistanceLevelRange,
const uint16_t BLEFitnessMachineServiceDal::charUUID[ mbbs_cIdxCOUNT] = 
{ 0x2AD2,0x2AD9,0x2ACC,0x2ADA,0x2AD3,0x2AD6};

BLEFitnessMachineServiceDal::BLEFitnessMachineServiceDal(BLEDevice &_ble) :
        ble(_ble)
{

    // Register the base UUID and create the service.
    RegisterBaseUUID( service_base_uuid);
    CreateService( serviceUUID);

    // Register the base UUID and create the characteristics.
    RegisterBaseUUID( char_base_uuid);
    CreateCharacteristic( mbbs_cIdxIndoorBikeData, charUUID[ mbbs_cIdxIndoorBikeData],
                         (uint8_t *)&indoorBikeDataCharacteristicBuffer,
                         sizeof(indoorBikeDataCharacteristicBuffer), sizeof(indoorBikeDataCharacteristicBuffer),
                         microbit_propNOTIFY);
    CreateCharacteristic( mbbs_cIdxFitnessMachineControlPoint, charUUID[ mbbs_cIdxFitnessMachineControlPoint],
                         (uint8_t *)&fitnessMachineControlPointCharacteristicBuffer,
                         sizeof(fitnessMachineControlPointCharacteristicBuffer), sizeof(fitnessMachineControlPointCharacteristicBuffer),
                         microbit_propWRITE | microbit_propINDICATE);
    CreateCharacteristic( mbbs_cIdxFitnessMachineFeature, charUUID[ mbbs_cIdxFitnessMachineFeature],
                         (uint8_t *)&fitnessMachineFeatureCharacteristicBuffer,
                         sizeof(fitnessMachineFeatureCharacteristicBuffer), sizeof(fitnessMachineFeatureCharacteristicBuffer),
                         microbit_propREAD);
    CreateCharacteristic( mbbs_cIdxFitnessMachineStatus, charUUID[ mbbs_cIdxFitnessMachineStatus],
                         (uint8_t *)&fitnessMachineStatusCharacteristicBuffer,
                         sizeof(fitnessMachineStatusCharacteristicBuffer), sizeof(fitnessMachineStatusCharacteristicBuffer),
                         microbit_propNOTIFY);
    CreateCharacteristic( mbbs_cIdxFitnessTrainingStatus, charUUID[ mbbs_cIdxFitnessTrainingStatus],
                         (uint8_t *)&fitnessTrainingStatusCharacteristicBuffer,
                         sizeof(fitnessTrainingStatusCharacteristicBuffer), sizeof(fitnessTrainingStatusCharacteristicBuffer),
                         microbit_propREAD | microbit_propNOTIFY);
    CreateCharacteristic( mbbs_cIdxFitnessSupportedResistanceLevelRange, charUUID[ mbbs_cIdxFitnessSupportedResistanceLevelRange],
                         (uint8_t *)&fitnessSupportedResistanceLevelRangeCharacteristicBuffer,
                         sizeof(fitnessSupportedResistanceLevelRangeCharacteristicBuffer), sizeof(fitnessSupportedResistanceLevelRangeCharacteristicBuffer),
                         microbit_propREAD);
    
    // // FTMS - Service Advertising Data
    // const uint8_t FTMS_UUID[2] = {0x26, 0x18};
    // ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, FTMS_UUID, sizeof(FTMS_UUID));
    // uint8_t serviceData[2+1+2];
    // struct_pack(serviceData, "<HBH", 0x1826, 0x01, 1<<5);
    // ble.accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, serviceData, sizeof(serviceData));

    // Default values.
    writeChrValue(mbbs_cIdxFitnessMachineFeature
         ,(const uint8_t *)&fitnessMachineFeatureCharacteristicBuffer, sizeof(fitnessMachineFeatureCharacteristicBuffer));
    writeChrValue(mbbs_cIdxFitnessTrainingStatus
         ,(const uint8_t *)&fitnessTrainingStatusCharacteristicBuffer, sizeof(fitnessTrainingStatusCharacteristicBuffer));
    writeChrValue(mbbs_cIdxFitnessSupportedResistanceLevelRange
         ,(const uint8_t *)&fitnessSupportedResistanceLevelRangeCharacteristicBuffer, sizeof(fitnessSupportedResistanceLevelRangeCharacteristicBuffer));
    
    overwriteAdvertisingPayload();
}

void BLEFitnessMachineServiceDal::onDataWritten( const microbit_ble_evt_write_t *params)
{
    microbit_charattr_t type;
    int index = charHandleToIdx(params->handle, &type);

    if (index == mbbs_cIdxFitnessMachineControlPoint && params->len >= 1)
    {
        onFitnessMachineControlPoint((const uint8_t *)params->data, params->len);
    }
}

bool BLEFitnessMachineServiceDal::getGapStateConnected()
{
    return getConnected();
}

void BLEFitnessMachineServiceDal::notifyCharFitnessTrainingStatus(const uint8_t *data, uint16_t length)
{
    notifyChrValue(mbbs_cIdxFitnessTrainingStatus, data, length);
}

void BLEFitnessMachineServiceDal::notifyCharFitnessMachineStatus(const uint8_t *data, uint16_t length)
{
    notifyChrValue(mbbs_cIdxFitnessMachineStatus, data, length);
}

void BLEFitnessMachineServiceDal::notifyCharIndoorBikeData(const uint8_t *data, uint16_t length)
{
    notifyChrValue(mbbs_cIdxIndoorBikeData, data, length);
}

void BLEFitnessMachineServiceDal::writeCharFitnessMachineControlPoint(const uint8_t *data, uint16_t length)
{
    writeChrValue(mbbs_cIdxFitnessMachineControlPoint, data, length);
}

void BLEFitnessMachineServiceDal::overwriteAdvertisingPayload()
{
    // Stop
    uBit.bleManager.stopAdvertising();

    uint8_t m_adv_handle;
    static ble_advdata_t m_advdata;
    static uint8_t  m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];
    static ble_uuid_t uuid;
    
    uuid.type = BLE_UUID_TYPE_BLE;
    uuid.uuid = 0x1826; // FTMS 
    m_advdata.uuids_complete.p_uuids = &uuid;
    m_advdata.uuids_complete.uuid_cnt = 1;
    m_advdata.include_appearance = false;
    //MICROBIT_BLE_ECHK( sd_ble_gap_appearance_set( BLE_APPEARANCE_UNKNOWN));
    m_advdata.name_type = BLE_ADVDATA_FULL_NAME;
    
    m_advdata.flags = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED | BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE;

    ble_gap_adv_params_t    gap_adv_params;
    memset( &gap_adv_params, 0, sizeof( gap_adv_params));
    gap_adv_params.properties.type  = true /* connectable */
                                    ? BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED
                                    : BLE_GAP_ADV_TYPE_NONCONNECTABLE_SCANNABLE_UNDIRECTED;
    gap_adv_params.interval         = ( 1000 * MICROBIT_BLE_ADVERTISING_INTERVAL/* interval_ms */) / 625;  // 625 us units
    if ( gap_adv_params.interval < BLE_GAP_ADV_INTERVAL_MIN) gap_adv_params.interval = BLE_GAP_ADV_INTERVAL_MIN;
    if ( gap_adv_params.interval > BLE_GAP_ADV_INTERVAL_MAX) gap_adv_params.interval = BLE_GAP_ADV_INTERVAL_MAX;
    gap_adv_params.duration         = MICROBIT_BLE_ADVERTISING_TIMEOUT /* timeout_seconds */ * 100;              //10 ms units
    gap_adv_params.filter_policy    = false /* whitelist */
                                    ? BLE_GAP_ADV_FP_FILTER_BOTH
                                    : BLE_GAP_ADV_FP_ANY;
    gap_adv_params.primary_phy      = BLE_GAP_PHY_1MBPS;
                
    ble_gap_adv_data_t  gap_adv_data;
    memset( &gap_adv_data, 0, sizeof( gap_adv_data));
    gap_adv_data.adv_data.p_data    = m_enc_advdata;
    gap_adv_data.adv_data.len       = BLE_GAP_ADV_SET_DATA_SIZE_MAX;

    MICROBIT_BLE_ECHK( ble_advdata_encode( &m_advdata, gap_adv_data.adv_data.p_data, &gap_adv_data.adv_data.len));
    MICROBIT_BLE_ECHK( sd_ble_gap_adv_set_configure( &m_adv_handle, &gap_adv_data, &gap_adv_params));

    // Restart
    uBit.bleManager.advertise();
} 

//================================================================
#else // MICROBIT_CODAL
//================================================================

#include "ble/UUID.h"

BLEFitnessMachineServiceDal::BLEFitnessMachineServiceDal(BLEDevice &_ble) :
        ble(_ble)
{
    // Caractieristic
    GattCharacteristic  indoorBikeDataCharacteristic(
        UUID(0x2AD2)
        , (uint8_t *)&indoorBikeDataCharacteristicBuffer, 0, sizeof(indoorBikeDataCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    );
    GattCharacteristic  fitnessMachineControlPointCharacteristic(
        UUID(0x2AD9)
        , (uint8_t *)&fitnessMachineControlPointCharacteristicBuffer, 0, sizeof(fitnessMachineControlPointCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE|GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_INDICATE
    );
    GattCharacteristic  fitnessMachineFeatureCharacteristic(
        UUID(0x2ACC)
        , (uint8_t *)&fitnessMachineFeatureCharacteristicBuffer, 0, sizeof(fitnessMachineFeatureCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
    );
    GattCharacteristic  fitnessMachineStatusCharacteristic(
        UUID(0x2ADA)
        , (uint8_t *)&fitnessMachineStatusCharacteristicBuffer, 0, sizeof(fitnessMachineStatusCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    );
    GattCharacteristic  fitnessTrainingStatusCharacteristic(
        UUID(0x2AD3)
        , (uint8_t *)&fitnessTrainingStatusCharacteristicBuffer, 0, sizeof(fitnessTrainingStatusCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY
    );
    GattCharacteristic  fitnessSupportedResistanceLevelRangeCharacteristic(
        UUID(0x2AD6)
        , (uint8_t *)&fitnessSupportedResistanceLevelRangeCharacteristicBuffer, 0, sizeof(fitnessSupportedResistanceLevelRangeCharacteristicBuffer)
        , GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ
    );
    
    // Set default security requirements
    indoorBikeDataCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessMachineControlPointCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessMachineFeatureCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessMachineStatusCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessTrainingStatusCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    fitnessSupportedResistanceLevelRangeCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    // Service
    GattCharacteristic *characteristics[] = {
        &indoorBikeDataCharacteristic,
        &fitnessMachineControlPointCharacteristic,
        &fitnessMachineFeatureCharacteristic,
        &fitnessMachineStatusCharacteristic,
        &fitnessTrainingStatusCharacteristic,
        &fitnessSupportedResistanceLevelRangeCharacteristic,
    };
    GattService service(
        UUID(0x1826), characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *)
    );
    ble.addService(service);

    // FTMS - Service Advertising Data
    const uint8_t FTMS_UUID[2] = {0x26, 0x18};
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, FTMS_UUID, sizeof(FTMS_UUID));
    // uint8_t serviceData[2+1+2];
    // struct_pack(serviceData, "<HBH", 0x1826, 0x01, 1<<5);
    // ble.accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, serviceData, sizeof(serviceData));

    // Characteristic Handle
    indoorBikeDataCharacteristicHandle = indoorBikeDataCharacteristic.getValueHandle();
    fitnessMachineControlPointCharacteristicHandle = fitnessMachineControlPointCharacteristic.getValueHandle();
    fitnessMachineFeatureCharacteristicHandle = fitnessMachineFeatureCharacteristic.getValueHandle();
    fitnessMachineStatusCharacteristicHandle = fitnessMachineStatusCharacteristic.getValueHandle();
    fitnessTrainingStatusCharacteristicHandle = fitnessTrainingStatusCharacteristic.getValueHandle();
    fitnessSupportedResistanceLevelRangeCharacteristicHandle = fitnessSupportedResistanceLevelRangeCharacteristic.getValueHandle();

    // Default values.
    ble.gattServer().write(fitnessMachineFeatureCharacteristicHandle
        ,(uint8_t *)&fitnessMachineFeatureCharacteristicBuffer, sizeof(fitnessMachineFeatureCharacteristicBuffer));
    ble.gattServer().write(fitnessTrainingStatusCharacteristicHandle
        ,(uint8_t *)&fitnessTrainingStatusCharacteristicBuffer, sizeof(fitnessTrainingStatusCharacteristicBuffer));
    ble.gattServer().write(fitnessSupportedResistanceLevelRangeCharacteristicHandle
        ,(uint8_t *)&fitnessSupportedResistanceLevelRangeCharacteristicBuffer, sizeof(fitnessSupportedResistanceLevelRangeCharacteristicBuffer));
    
    // onDataWritten
    ble.onDataWritten(this, &BLEFitnessMachineServiceDal::onDataWritten);
    
}

void BLEFitnessMachineServiceDal::onDataWritten(const GattWriteCallbackParams *params)
{
    if (params->handle == fitnessMachineControlPointCharacteristicHandle && params->len >= 1)
    {
        onFitnessMachineControlPoint((const uint8_t *)params->data, params->len);
    }
}

bool BLEFitnessMachineServiceDal::getGapStateConnected()
{
    return ble.getGapState().connected;
}

void BLEFitnessMachineServiceDal::notifyCharFitnessTrainingStatus(const uint8_t *data, uint16_t length)
{
    ble.gattServer().notify(fitnessTrainingStatusCharacteristicHandle, data, length);
}

void BLEFitnessMachineServiceDal::notifyCharFitnessMachineStatus(const uint8_t *data, uint16_t length)
{
    ble.gattServer().notify(fitnessMachineStatusCharacteristicHandle, data, length);
}

void BLEFitnessMachineServiceDal::notifyCharIndoorBikeData(const uint8_t *data, uint16_t length)
{
    ble.gattServer().notify(indoorBikeDataCharacteristicHandle, data, length);
}

void BLEFitnessMachineServiceDal::writeCharFitnessMachineControlPoint(const uint8_t *data, uint16_t length)
{
    ble.gattServer().write(fitnessMachineControlPointCharacteristicHandle, data, length);
}

//================================================================
#endif // MICROBIT_CODAL
//================================================================

BLEFitnessMachineServiceBase::BLEFitnessMachineServiceBase()
{
    // Initialise
    lastTargetResistanceLevel10=0;
    lastWindSpeed1000=0;
    lastGrade100=0;
    lastCrr10000=0;
    lastCw100=0;
    nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize=0;

    // Initialise our characteristic values.
    memset(&indoorBikeDataCharacteristicBuffer, 0, sizeof(indoorBikeDataCharacteristicBuffer));
    memset(&fitnessMachineControlPointCharacteristicBuffer, 0, sizeof(fitnessMachineControlPointCharacteristicBuffer));
    memset(&fitnessMachineFeatureCharacteristicBuffer, 0, sizeof(fitnessMachineFeatureCharacteristicBuffer));
    memset(&fitnessMachineStatusCharacteristicBuffer, 0, sizeof(fitnessMachineStatusCharacteristicBuffer));
    memset(&fitnessTrainingStatusCharacteristicBuffer, 0, sizeof(fitnessTrainingStatusCharacteristicBuffer));
    memset(&fitnessSupportedResistanceLevelRangeCharacteristicBuffer, 0, sizeof(fitnessSupportedResistanceLevelRangeCharacteristicBuffer));

    // Default values.
    struct_pack(fitnessMachineFeatureCharacteristicBuffer
        , "<II"
        , FTMP_FLAGS_FITNESS_MACINE_FEATURES_FIELD
        , FTMP_FLAGS_TARGET_SETTING_FEATURES_FIELD
    );
    struct_pack(fitnessTrainingStatusCharacteristicBuffer
        , "<BB"
        , FTMP_FLAGS_TRAINING_STATUS_FIELD_00_STATUS_ONLY
        , FTMP_VAL_TRAINING_STATUS_01_IDEL
    );
    struct_pack(fitnessSupportedResistanceLevelRangeCharacteristicBuffer
        , "<hhH"
        , VAL_MINIMUM_RESISTANCE_LEVEL
        , VAL_MAXIMUM_RESISTANCE_LEVEL
        , VAL_INCREMENT_RESISTANCE_LEVEL
    );

}

void BLEFitnessMachineServiceBase::onFitnessMachineControlPoint(const uint8_t *data, uint16_t length)
{
    uint8_t responseBuffer[3];
    responseBuffer[0] = FTMP_OP_CODE_CPPR_80_RESPONSE_CODE;
    uint8_t *opCode=&responseBuffer[1];
    opCode[0]=data[0];
    uint8_t *result=&responseBuffer[2];
    result[0] = FTMP_RESULT_CODE_CPPR_03_INVALID_PARAMETER;
    bool changedSimu=false;
    int16_t windSpeed1000=0;
    int16_t grade100=0;
    uint8_t crr10000=0;
    uint8_t cw100=0;
    switch (opCode[0])
    {
    case FTMP_OP_CODE_CPPR_00_REQUEST_CONTROL:
        if (length == 1)
        {
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_01_RESET:
        if (length == 1)
        {
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL:
        if (length == 2
            && data[1] >= VAL_MINIMUM_RESISTANCE_LEVEL
            && data[1] <= VAL_MAXIMUM_RESISTANCE_LEVEL)
        {
            lastTargetResistanceLevel10 = data[1];
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_07_START_RESUME:
        if (length == 1)
        {
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_08_STOP_PAUSE:
        if (length == 2)
        {
            //uint8_t stopOrPause = params->data[1];
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
        }
        break;

    case FTMP_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION:
        switch (length)
        {
        case 3:
            struct_unpack(&data[1], "<h", &windSpeed1000);
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            break;
        
        case 5:
            struct_unpack(&data[1], "<hh", &windSpeed1000, &grade100);
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            break;
        
        case 6:
            struct_unpack(&data[1], "<hhB", &windSpeed1000, &grade100, &crr10000);
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            break;
            
        case 7:
            struct_unpack(&data[1], "<hhBB", &windSpeed1000, &grade100, &crr10000, &cw100);
            result[0] = FTMP_RESULT_CODE_CPPR_01_SUCCESS;
            break;

        default:
            break;

        }
        if (result[0]==FTMP_RESULT_CODE_CPPR_01_SUCCESS)
        {
            changedSimu = false;
            // for sendFitnessMachineStatusIndoorBikeSimulationParametersChanged
            nextFitnessMachineStatusIndoorBikeSimulationParametersChanged[0]=FTMP_OP_CODE_FITNESS_MACHINE_STATUS_12_INDOOR_BIKE_SIMULATION_PARAMETERS_CHANGED;
            nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize=length;
            for (int i=1; i<length; i++ )
            {
                nextFitnessMachineStatusIndoorBikeSimulationParametersChanged[i] = data[i];
            }
            if (lastWindSpeed1000!=windSpeed1000)
            {
                changedSimu=true;
                lastWindSpeed1000=windSpeed1000;
            }
            if (lastGrade100!=grade100)
            {
                changedSimu=true;
                lastGrade100=grade100;
            }
            if (lastCrr10000!=crr10000)
            {
                changedSimu=true;
                lastCrr10000=crr10000;
            }
            if (lastCw100!=cw100)
            {
                changedSimu=true;
                lastCw100=cw100;
            }
        }
        break;

    default:
        result[0] = FTMP_RESULT_CODE_CPPR_02_NOT_SUPORTED;
        break;
    }

    // Response - Fitness Machine Control Point
    writeCharFitnessMachineControlPoint((const uint8_t *)&responseBuffer, sizeof(responseBuffer));
    
    // Procedure
    switch (opCode[0])
    {
    case FTMP_OP_CODE_CPPR_00_REQUEST_CONTROL:
        // # 0x00 M Request Control
        // (NOP)
        break;
    case FTMP_OP_CODE_CPPR_01_RESET:
        // # 0x01 M Reset
        sendTrainingStatusManualMode();
        break;
    case FTMP_OP_CODE_CPPR_04_SET_TARGET_RESISTANCE_LEVEL:
        // # 0x04 C.3 Set Target Resistance Level [UINT8, Level]
        sendFitnessMachineStatusTargetResistanceLevelChanged(lastTargetResistanceLevel10);
        MicroBitEvent(CUSTOM_EVENT_ID_VIRTUAL_RIDE, VIRTUAL_RIDE_EVT_SET_TARGET_RESISTANCE_LEVEL);
        break;
    case FTMP_OP_CODE_CPPR_07_START_RESUME:
        // # 0x07 M Start or Resume
        sendTrainingStatusManualMode();
        break;
    case FTMP_OP_CODE_CPPR_08_STOP_PAUSE:
        // # 0x08 M Stop or Pause [UINT8, 0x01-STOP, 0x02-PAUSE]
        sendTrainingStatusIdle();
        break;
    case FTMP_OP_CODE_CPPR_11_SET_INDOOR_BIKE_SIMULATION:
        // # 0x11 C.14 Set Indoor Bike Simulation [SINT16, Wind Speed], [SINT16, Grade], [UINT8 CRR], [UINT8, CW]
        if (changedSimu)
        {
            sendFitnessMachineStatusIndoorBikeSimulationParametersChanged();
            MicroBitEvent(CUSTOM_EVENT_ID_VIRTUAL_RIDE, VIRTUAL_RIDE_EVT_SET_INDOOR_BIKE_SIMULATION);
        }
        break;
    default:
        break;
    }
    
}

void BLEFitnessMachineServiceBase::sendTrainingStatusIdle(void)
{
    if (getGapStateConnected())
    {
        static const uint8_t buff[]={FTMP_FLAGS_TRAINING_STATUS_FIELD_00_STATUS_ONLY, FTMP_VAL_TRAINING_STATUS_01_IDEL};
        notifyCharFitnessTrainingStatus((const uint8_t *)&buff, sizeof(buff));
    }
}

void BLEFitnessMachineServiceBase::sendTrainingStatusManualMode(void)
{
    if (getGapStateConnected())
    {
        static const uint8_t buff[]={FTMP_FLAGS_TRAINING_STATUS_FIELD_00_STATUS_ONLY, FTMP_VAL_TRAINING_STATUS_0D_MANUAL_MODE};
        notifyCharFitnessTrainingStatus((const uint8_t *)&buff, sizeof(buff));
    }
}
    
void BLEFitnessMachineServiceBase::sendFitnessMachineStatusReset(void)
{
    if (getGapStateConnected())
    {
        static const uint8_t buff[]={FTMP_OP_CODE_FITNESS_MACHINE_STATUS_01_RESET};
        notifyCharFitnessMachineStatus((const uint8_t *)&buff, sizeof(buff));
    }
}

void BLEFitnessMachineServiceBase::sendFitnessMachineStatusTargetResistanceLevelChanged(const uint8_t targetResistanceLevel10)
{
    if (getGapStateConnected())
    {
        uint8_t buff[2];
        buff[0]=FTMP_OP_CODE_FITNESS_MACHINE_STATUS_07_TARGET_RESISTANCE_LEVEL_CHANGED;
        buff[1]=targetResistanceLevel10;
        notifyCharFitnessMachineStatus((const uint8_t *)&buff, sizeof(buff));
    }
}

void BLEFitnessMachineServiceBase::sendFitnessMachineStatusIndoorBikeSimulationParametersChanged(void)
{
    if (getGapStateConnected() && nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize>0)
    {
        notifyCharFitnessMachineStatus(
            (const uint8_t *)&nextFitnessMachineStatusIndoorBikeSimulationParametersChanged,
            nextFitnessMachineStatusIndoorBikeSimulationParametersChangedSize);
    }
}

void BLEFitnessMachineServiceBase::notifyIndoorBikeData(uint32_t speed100, uint32_t cadence2, int32_t resistanceLevel, int32_t power)
{
    if (getGapStateConnected())
    {
        uint8_t buff[2+2+2+2+2];
        struct_pack(buff, "<HHHhh",
            FTMP_FLAGS_INDOOR_BIKE_DATA_CHAR,
            speed100,
            cadence2,
            resistanceLevel,
            power
        );
        notifyCharIndoorBikeData((const uint8_t *)&buff, sizeof(buff));
    }
}

uint8_t BLEFitnessMachineServiceBase::getTargetResistanceLevel10()
{
    return lastTargetResistanceLevel10;
}

int16_t BLEFitnessMachineServiceBase::getGrade100()
{
    return lastGrade100;
}

BLEFitnessMachineService::BLEFitnessMachineService()
{
    pService = new BLEFitnessMachineServiceDal(*uBit.ble);
}

void BLEFitnessMachineService::notifyIndoorBikeData(uint32_t speed100, uint32_t cadence2, int32_t resistanceLevel, int32_t power)
{
    pService->notifyIndoorBikeData( speed100,  cadence2,  resistanceLevel,  power);
}

uint8_t BLEFitnessMachineService::getTargetResistanceLevel10()
{
    return pService->getTargetResistanceLevel10();
}

int16_t BLEFitnessMachineService::getGrade100()
{
    return pService->getGrade100();
}
