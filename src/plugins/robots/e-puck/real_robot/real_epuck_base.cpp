/**
 * @file argos3/plugins/robots/e-puck/real_robot/real_epuck_base.cpp
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */


#include "real_epuck_base.h"

#ifdef __cplusplus
namespace argos {

    /****************************************/
    /****************************************/

    bool isset(char un_byte, size_t un_pos) {
        return un_byte & (0x00000001 << un_pos);
    }

    /****************************************/
    /****************************************/

    std::ostream& operator<<(std::ostream& t_os,
                             const BaseSensorState& s_state) {
        t_os << "[ prox={"
             << s_state.Proximity[0] << " "
             << s_state.Proximity[1] << " "
             << s_state.Proximity[2] << " "
             << s_state.Proximity[3] << " "
             << s_state.Proximity[4] << " "
             << s_state.Proximity[5] << " "
             << s_state.Proximity[6] << " "
             << s_state.Proximity[7]
             << "}, accel={"
             << s_state.Accel[0] << " "
             << s_state.Accel[1] << " "
             << s_state.Accel[2]
             << "}, micro={"
             << s_state.Micro[0] << " "
             << s_state.Micro[1] << " "
             << s_state.Micro[2]
             << "}, light={"
             << s_state.Light[0] << " "
             << s_state.Light[1] << " "
             << s_state.Light[2] << " "
             << s_state.Light[3] << " "
             << s_state.Light[4] << " "
             << s_state.Light[5] << " "
             << s_state.Light[6] << " "
             << s_state.Light[7]
             << "}, RB={";

        if (s_state.RBhasReceived) {
            UInt8 Data0 = (s_state.RBMessage.Data & 0xFF);
            UInt8 Data1 = (s_state.RBMessage.Data >> 8);
            t_os << " Received ["
                 << Data0 << " , "
                 << Data1 << " R "
                 << s_state.RBMessage.Range << " B "
                 << s_state.RBMessage.Bearing
                 << "]";
        } else {
            t_os << "NOT Received";
        }

        t_os << "}, IRCOM={";
        if (s_state.IRComHasReceived) {
            t_os << " Received ["
                 << s_state.IRComMessage.Data << " ,  DIST "
                 <<  s_state.IRComMessage.Range << " ,  DIR "
                 << s_state.IRComMessage.Bearing << " ,  S "
                 << s_state.IRComMessage.Sensor << "]";
        } else {
            t_os << "NOT Received";
        }

        t_os << "} ]";
        t_os << "" << std::endl;
        return t_os;
    }

    /****************************************/
    /****************************************/

    std::ostream& operator<<(std::ostream& t_os,
                             const BaseActuatorState& s_state) {
        t_os << "[ lwheel=" << s_state.LWheel << ", "
             << "rwheel=" << s_state.RWheel << ", "
             << "ring_leds={"
             << isset(s_state.BaseLEDs,0) << " "
             << isset(s_state.BaseLEDs,1) << " "
             << isset(s_state.BaseLEDs,2) << " "
             << isset(s_state.BaseLEDs,3) << " "
             << isset(s_state.BaseLEDs,4) << " "
             << isset(s_state.BaseLEDs,5) << " "
             << isset(s_state.BaseLEDs,6) << " "
             << isset(s_state.BaseLEDs,7)
             << "}, front_led = "
             << isset(s_state.OtherLEDs,0) << ", "
             << "body_led = "
             << isset(s_state.OtherLEDs,1)
             << " ]"
             << "}, RB={";

        t_os << "TO SEND [" << s_state.RBMessageData << "]";
        t_os << "}, IRCOM={";

        if(s_state.IRComEnabled){
            t_os << "TO SEND [" << s_state.IRComMessageData << "]";
        } else {
            t_os << "INACTIVE";
        }
        t_os << "} ]" << std::endl;
        return t_os;
    }

    /****************************************/
    /****************************************/
}
#endif
