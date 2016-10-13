/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_range_and_bearing_sensor.h>
 *
 * @brief This file provides the implementation of the epuck range and
 * bearing sensor.
 *
 * The real robot measures the power of the received signal. This
 * distance is converted in Distance using the function
 * Max(exp(m_fExpA + m_fExpB * fPower), 0.0);
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#ifndef REAL_EPUCK_RANGE_AND_BEARING_SENSOR_H
#define REAL_EPUCK_RANGE_AND_BEARING_SENSOR_H

namespace argos {
   class CRealEPuckRangeAndBearingSensor;
}

#include <argos3/core/utility/math/range.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_range_and_bearing_sensor.h>
#include <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_sensor.h>

#include <pthread.h>
#include <list>

namespace argos {

   class CRealEPuckRangeAndBearingSensor : virtual public CCI_EPuckRangeAndBearingSensor,
                                           virtual public CRealEPuckI2CSensor {

   public:

      CRealEPuckRangeAndBearingSensor();
      ~CRealEPuckRangeAndBearingSensor();

      void Init(TConfigurationNode& t_node);

      virtual void ReceiveData();

   private:

      void Calibrate(const std::string& str_conf_file);

      void MessageFetcherThread();

      static void* StartThread(void* pt_arg);

   private:
     Real m_fExpA,m_fExpB,m_expK; //The conversion between POWER (from the hw) and DISTANCE is done using the function DISTANCE(POWER)=exp(a+ b* POWER)
      /** Device stream to interact with the range and bearing sensor */
      TI2CDeviceStream m_tDeviceStream;

      /** Flag that stores whether the sensor is calibrated */
      bool m_bCalibrated;

      /** Handle to the message fetcher thread */
      pthread_t m_tMessageFetcherThread;

      /** This mutex protects the concurrent access to the packet queue */
      pthread_mutex_t m_tPacketQueueMutex;

      /**
       * This is a list of packets read by the message fetcher thread.
       * The packets in this list get read by ReceiveData() every time step.
       * After each ReceiveData(), the list gets cleared.
       * @see MessageFetcherThread
       * @see ReceiveData
       */
      std::list<CCI_EPuckRangeAndBearingSensor::SReceivedPacket*> m_tQueuedPackets;
   };
}
#endif
