/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_device.h>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 * @author Carlo Pinciroli <cpinciro@ulb.ac.be>
 */

#ifndef REAL_EPUCK_I2C_DEVICE_H
#define REAL_EPUCK_I2C_DEVICE_H

#include <argos3/core/utility/datatypes/datatypes.h>

namespace argos {

   /** The type of the device stream */
   typedef int TI2CDeviceStream;

   class CRealEPuckI2CDevice {

   public:

      virtual ~CRealEPuckI2CDevice() {}

      /* Initializes the I2C device management. */
      static void Init();

      /**
       * Opens the stream for the wanted device.
       * @param n_address The address of the device.
       * @return A stream for the wanted device.
       * @throws CARGoSException if an error occurs
       */
      TI2CDeviceStream OpenDevice(SInt16 n_address);

      /**
       * Closes the stream for the wanted device.
       * @param t_stream The device stream, which must have been opened beforehand
       * @throws CARGoSException if an error occurs
       */
      void CloseDevice(TI2CDeviceStream t_stream);

      /**
       * Reads a register from the device stream.
       * @param t_stream The device stream, which must have been opened beforehand
       * @param n_register The id of the wanted register
       * @param n_count How many bytes to read; can be either 1 or 2
       * @throws CARGoSException if an error occurs
       * @see OpenDevice
       */
      UInt16 ReadRegister(TI2CDeviceStream t_stream,
                          SInt8 n_register,
                          SInt16 n_count);

      /**
       * Writes a value into a register.
       * @param t_stream The device stream, which must have been opened beforehand
       * @param n_register The id of the wanted register
       * @param n_value The value to write
       * @throws CARGoSException if an error occurs
       * @see OpenDevice
       */
      void WriteRegister(TI2CDeviceStream t_stream,
                         SInt8 n_register,
                         SInt8 n_value);

      /**
       * Writes data onto the device stream.
       * @param t_stream The device stream, which must have been opened beforehand
       * @param n_buffer The buffer containing the data
       * @param n_count How many bytes to write
       * @throws CARGoSException if an error occurs
       * @see OpenDevice
       */
      void WriteData(TI2CDeviceStream t_stream,
                     const SInt8* n_buffer,
                     SInt16 n_count);
 static void ThreadCleanup();
   private:
      /** This mutex protects the concurrent access to read/write i2c operations */
      static pthread_mutex_t m_tIOMutex;

   };

}

#endif
