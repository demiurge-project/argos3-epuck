/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_range_and_bearing_actuator.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_RANGE_AND_BEARING_ACTUATOR_H
#define REAL_EPUCK_RANGE_AND_BEARING_ACTUATOR_H

namespace argos {
   class CRealEPuckRangeAndBearingActuator;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_actuator.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_actuator.h>

namespace argos {
   class CRealEPuckRangeAndBearingActuator : virtual public CCI_EPuckRangeAndBearingActuator,
                                             virtual public CRealEPuckI2CActuator {

   public:

      CRealEPuckRangeAndBearingActuator();
      virtual ~CRealEPuckRangeAndBearingActuator();

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
