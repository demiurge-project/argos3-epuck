/**
 * @file <argos3/plugins/robots/x-puck/real_robot/real_xpuck_range_and_bearing_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_XPUCK_RANGE_AND_BEARING_ACTUATOR_H
#define REAL_XPUCK_RANGE_AND_BEARING_ACTUATOR_H

namespace argos {
   class CRealXPuckRangeAndBearingActuator;
}

#include <argos3/plugins/robots/x-puck/control_interface/ci_xpuck_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/x-puck/real_robot/real_xpuck_i2c_actuator.h>

namespace argos {
   class CRealXPuckRangeAndBearingActuator : virtual public CCI_XPuckRangeAndBearingActuator,
                                             virtual public CRealXPuckI2CActuator {

   public:

      CRealXPuckRangeAndBearingActuator();
      virtual ~CRealXPuckRangeAndBearingActuator();

      virtual void Init(TConfigurationNode& t_node);
      virtual void SendData();



   private:

      void PacketSenderThread();

      static void* StartThread(void* pt_arg);



   private:

      TI2CDeviceStream m_tDeviceStream;


      /** This structure stores what it is actually sent by the thread */
      SDataToSend m_sSentData;

      /** Handle to the packet sender thread */
      pthread_t m_tPacketSenderThread;

      /** This mutex protects the concurrent access to the data to send */
      pthread_mutex_t m_tDataMutex;
   };
}
#endif
