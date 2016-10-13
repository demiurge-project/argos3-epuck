#ifndef EPUCK_CAMERA_CONFIGURATION_H
#define EPUCK_CAMERA_CONFIGURATION_H

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/networking/tcp_socket.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/epuck_image_grabber.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/blob_detector.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/distance_converter.h>
#include <string>

namespace argos {

   class CEPuckCameraConfigurator {

   public:

      CEPuckCameraConfigurator(const std::string& str_conf_file);

      ~CEPuckCameraConfigurator();

      void Execute();

   private:

      void CommandLoop();
      void ExecuteCommand(UInt8 un_command);

      void GetAttributes();
      void SetAttributes();
      void Refresh();
      void SaveConfiguration();

      SInt32 SendData(const UInt8* pun_buffer,
                      ssize_t n_size);

      SInt32 ReceiveData(UInt8* pun_buffer,
                         ssize_t n_size);

   private:

      std::string m_strConfFileName;

      CEPuckImageGrabber* m_pcImageGrabber;
      CBlobDetector*      m_pcBlobDetector;
      CDistanceConverter* m_pcDistanceConverter;

      CTCPSocket m_cListenSocket;
      CTCPSocket m_cCommandSocket;

      CImage<SHSV888Pixel>* m_pcHSVImage;
   };

}

#endif
