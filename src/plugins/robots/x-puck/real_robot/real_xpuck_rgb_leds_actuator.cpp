/**
 * @file <argos3/plugins/robot/x-puck/real_robot/real_xpuck_rgb_leds_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "real_xpuck_rgb_leds_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

/****************************************/
/*******************
*********************/

CRealXPuckRGBLEDsActuator::CRealXPuckRGBLEDsActuator() {

    m_tDeviceStream = OpenDevice(0x61);

}

/****************************************/
/****************************************/

CRealXPuckRGBLEDsActuator::~CRealXPuckRGBLEDsActuator() {

    SetColors(CColor::BLACK);
    SendData();
    CloseDevice(m_tDeviceStream);

}

/****************************************/
/****************************************/

void CRealXPuckRGBLEDsActuator::SendData() {

    SInt8 nData[9];
    nData[0] = m_tLEDSettings[2].GetRed();
    nData[1] = m_tLEDSettings[2].GetGreen();
    nData[2] = m_tLEDSettings[2].GetBlue();
    nData[3] = m_tLEDSettings[1].GetRed();
    nData[4] = m_tLEDSettings[1].GetGreen();
    nData[5] = m_tLEDSettings[1].GetBlue();
    nData[6] = m_tLEDSettings[0].GetRed();
    nData[7] = m_tLEDSettings[0].GetGreen();
    nData[8] = m_tLEDSettings[0].GetBlue();
    WriteData(m_tDeviceStream, nData, 9);

}

/****************************************/
/****************************************/

}
