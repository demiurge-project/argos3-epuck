/**
 * @file <argos3/plugins/robots/e-puck/real_robot/real_epuck_i2c_device.cpp>
 *
 * @author Lorenzo Garattoni - <lgaratto@ulb.ac.be>
 */

#include "real_epuck_i2c_device.h"

#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h> /* for I2C_SLAVE */
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

namespace argos {

pthread_mutex_t CRealEPuckI2CDevice::m_tIOMutex;
UInt8 maxRetries = 10;

/****************************************/
/****************************************/

void CRealEPuckI2CDevice::Init() {
    ::pthread_mutex_init(&m_tIOMutex, NULL);
}

/****************************************/
/****************************************/
void CRealEPuckI2CDevice::ThreadCleanup(){


    pthread_mutex_unlock(&m_tIOMutex);


}
/****************************************/
/****************************************/

TI2CDeviceStream CRealEPuckI2CDevice::OpenDevice(SInt16 n_address) {


    pthread_mutex_lock(&m_tIOMutex);

    TI2CDeviceStream tStream = ::open("/dev/i2c-3", O_RDWR);
    if(tStream == -1) {

        pthread_mutex_unlock(&m_tIOMutex);

        THROW_ARGOSEXCEPTION("Unable to open the I2C device stream: " << ::strerror(errno));
    }
    if(::ioctl(tStream, I2C_SLAVE, n_address) == -1) {

        pthread_mutex_unlock(&m_tIOMutex);

        THROW_ARGOSEXCEPTION("Unable to configure the I2C device stream: " << ::strerror(errno));
    }

    pthread_mutex_unlock(&m_tIOMutex);


    return tStream;
}

/****************************************/
/****************************************/

void CRealEPuckI2CDevice::CloseDevice(TI2CDeviceStream t_stream) {


    pthread_mutex_lock(&m_tIOMutex);


    if(::close(t_stream)) {

        pthread_mutex_unlock(&m_tIOMutex);

        THROW_ARGOSEXCEPTION("Unable to close the I2C device stream: " << ::strerror(errno));
    }

    pthread_mutex_unlock(&m_tIOMutex);



}

/****************************************/
/****************************************/

UInt16 CRealEPuckI2CDevice::ReadRegister(TI2CDeviceStream t_stream,
                                         SInt8 n_register,
                                         SInt16 n_count) {


    pthread_mutex_lock(&m_tIOMutex);

    UInt8 unData[2] = {0,0};
    UInt8 nRetries = 0;

    if(n_count < 1 || n_count > 2) {

        pthread_mutex_unlock(&m_tIOMutex);

        LOGERR << "CRealEPuckI2CDevice::ReadRegister()"
               << "can only read 1 or 2 bytes from device stream; requested "
               << n_count;
    }

    while (nRetries < maxRetries && ::write(t_stream, &n_register, 1) != 1) {nRetries++;}
    if (nRetries == maxRetries) {

        pthread_mutex_unlock(&m_tIOMutex);

        LOGERR << "CRealEPuckI2CDevice::ReadRegister(): "
               << "Unable to send data to the I2C device stream: "
               << ::strerror(errno);
    }

    nRetries = 0;
    while (nRetries < maxRetries && ::read(t_stream, &unData, n_count) != n_count) {nRetries++;}

    if(nRetries == maxRetries) {

        pthread_mutex_unlock(&m_tIOMutex);

        LOG << "CRealEPuckI2CDevice::ReadRegister(): "
            << "Unable to read data from the I2C device stream: "
            << ::strerror(errno);
    }

    pthread_mutex_unlock(&m_tIOMutex);


    return (unData[1] << 8) + unData[0];
}

/****************************************/
/****************************************/

void CRealEPuckI2CDevice::WriteRegister(TI2CDeviceStream t_stream,
                                        SInt8 n_register,
                                        SInt8 n_value) {

    SInt8 unData[2];
    unData[0] = n_register;
    unData[1] = n_value;
    WriteData(t_stream, &unData[0], 2);

}

/****************************************/
/****************************************/

void CRealEPuckI2CDevice::WriteData(TI2CDeviceStream t_stream,
                                    const SInt8* n_buffer,
                                    SInt16 n_count) {


    pthread_mutex_lock(&m_tIOMutex);

    ssize_t nWritten = 0;
    UInt32 unRetries = 0;
    size_t unLeftToWrite = n_count;
    SInt8* pnCurrentPos = (SInt8*) n_buffer;
    do {
        nWritten = ::write(t_stream, pnCurrentPos, unLeftToWrite);
        if (nWritten < 0 && unRetries > maxRetries) {

            pthread_mutex_unlock(&m_tIOMutex);

            LOGERR << "CRealEPuckI2CDevice::WriteData(): "
                   << "Unable to send data to the I2C device stream: "
                   << ::strerror(errno);
        } else if (nWritten < 0){
            ++unRetries;
        } else {
            unLeftToWrite -= nWritten;
            pnCurrentPos += nWritten;
        }
    } while (unLeftToWrite != 0);

    pthread_mutex_unlock(&m_tIOMutex);


}

/****************************************/
/****************************************/

}
