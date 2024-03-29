/* mbed Microcontroller Library
* Copyright (c) 2006-2013 ARM Limited
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/**
  ******************************************************************************
  * @file    BlueNRGGap.h 
  * @author  STMicroelectronics
  * @brief   Header file for BlueNRG BLE_API Gap Class
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  */ 
  
#ifndef __BLUENRG_GAP_H__
#define __BLUENRG_GAP_H__

#include "mbed.h"
#include "ble/blecommon.h"
#include "btle.h"
#include "ble/GapAdvertisingParams.h"
#include "ble/GapAdvertisingData.h"
#include "ble/Gap.h"

#define BLE_CONN_HANDLE_INVALID 0x0
#define BDADDR_SIZE 6

#define BLUENRG_GAP_ADV_INTERVAL_MIN (0)
#define BLUENRG_GAP_ADV_INTERVAL_MAX (0)
#define BLE_GAP_ADV_NONCON_INTERVAL_MIN (0)

// Scanning and Connection Params used by Central for creating connection
#define LIMITED_DISCOVERY_PROCEDURE 0x01
#define GENERAL_DISCOVERY_PROCEDURE 0x02

#define SCAN_P         (0x4000)
#define SCAN_L         (0x4000)
#define SUPERV_TIMEOUT (600)
#define CONN_P(x)      ((int)((x)/1.25f))
#define CONN_L(x)      ((int)((x)/0.625f))
#define CONN_P1        (CONN_P(50))//(CONN_P(1000))
#define CONN_P2        (CONN_P(50))//(CONN_P(1000))
#define CONN_L1        (CONN_L(5)) 
#define CONN_L2        (CONN_L(5))
#define UUID_BUFFER_SIZE 13 //6*2(16-bit UUIDs)+1
#define ADV_DATA_MAX_SIZE 31

/**************************************************************************/
/*!
    \brief

*/
/**************************************************************************/
class BlueNRGGap : public Gap
{
public:
    static BlueNRGGap &getInstance() {
        static BlueNRGGap m_instance;
        return m_instance;
    }

    // <<<ANDREA>>>
    /*
    enum AdvType_t {
        ADV_IND           = GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED,//Gap::ADV_IND,
        ADV_DIRECT_IND    = GapAdvertisingParams::ADV_CONNECTABLE_DIRECTED,//Gap::ADV_DIRECT_IND,
        ADV_SCAN_IND      = GapAdvertisingParams::ADV_SCANNABLE_UNDIRECTED,//Gap::ADV_SCAN_IND,
        ADV_NONCONN_IND   = GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED//Gap::ADV_NONCONN_IND
    };
    */
    enum Reason_t {
        DEVICE_FOUND,
        DISCOVERY_COMPLETE
    };
    
    /* Functions that must be implemented from Gap */
    virtual ble_error_t setAddress(addr_type_t type,   const Address_t address);
    virtual ble_error_t getAddress(addr_type_t *typeP, Address_t address);
    virtual ble_error_t setAdvertisingData(const GapAdvertisingData &, const GapAdvertisingData &);
    virtual ble_error_t startAdvertising(const GapAdvertisingParams &);
    virtual ble_error_t stopAdvertising(void);
    virtual ble_error_t stopScan();
    virtual uint16_t    getMinAdvertisingInterval(void) const;
    virtual uint16_t    getMinNonConnectableAdvertisingInterval(void) const;
    virtual uint16_t    getMaxAdvertisingInterval(void) const;
    virtual ble_error_t disconnect(DisconnectionReason_t reason);
    virtual ble_error_t disconnect(Handle_t connectionHandle, DisconnectionReason_t reason);
    virtual ble_error_t getPreferredConnectionParams(ConnectionParams_t *params);
    virtual ble_error_t setPreferredConnectionParams(const ConnectionParams_t *params);
    virtual ble_error_t updateConnectionParams(Handle_t handle, const ConnectionParams_t *params);

    virtual ble_error_t setDeviceName(const uint8_t *deviceName);
    virtual ble_error_t getDeviceName(uint8_t *deviceName, unsigned *lengthP);
    virtual ble_error_t setAppearance(GapAdvertisingData::Appearance appearance);
    virtual ble_error_t getAppearance(GapAdvertisingData::Appearance *appearanceP);
    
    virtual ble_error_t setTxPower(int8_t txPower);
    virtual void        getPermittedTxPowerValues(const int8_t **, size_t *);
    // <<<ANDREA>>>
    virtual ble_error_t connect(const Address_t peerAddr,
                                Gap::AddressType_t peerAddrType,
                                const ConnectionParams_t *connectionParams,
                                const GapScanningParams *scanParams);
    

    void                Discovery_CB(Reason_t reason,
                                     uint8_t adv_type,
                                     uint8_t *addr_type,
                                     uint8_t *addr,
                                     uint8_t *data_length,
                                     uint8_t *data,
                                     uint8_t *RSSI);
    ble_error_t         makeConnection(void);
    void     setConnectionHandle(uint16_t con_handle);
    uint16_t getConnectionHandle(void);
    
    bool getIsSetAddress();
    
    Timeout getAdvTimeout(void) const {
        return advTimeout;
    }
    uint8_t getAdvToFlag(void) {
        return AdvToFlag;
    }
    void setAdvToFlag(void);
    
    void Process(void);

protected:
    virtual ble_error_t startRadioScan(const GapScanningParams &scanningParams);

private:
    uint16_t m_connectionHandle;
    AddressType_t addr_type;
    Address_t _peerAddr;
    uint8_t bdaddr[BDADDR_SIZE];
    bool _scanning;
    bool _connecting;
    bool isSetAddress;
    tBleStatus ret; // FIXME: delete this
    uint8_t *DeviceName;
    uint8_t deviceAppearance[2];

    uint8_t *local_name;
    uint8_t local_name_length;
    
    bool txPowerAdType;

    uint8_t servUuidlength;
    uint8_t servUuidData[UUID_BUFFER_SIZE];

    uint8_t AdvLen;
    uint8_t AdvData[ADV_DATA_MAX_SIZE];
    
    Timeout advTimeout;
    bool AdvToFlag;

    BlueNRGGap() {
        m_connectionHandle = BLE_CONN_HANDLE_INVALID;
        addr_type = Gap::ADDR_TYPE_PUBLIC;
        isSetAddress = false;
        DeviceName = NULL;     
    }

    BlueNRGGap(BlueNRGGap const &);
    void operator=(BlueNRGGap const &);
};

#endif // ifndef __BLUENRG_GAP_H__
