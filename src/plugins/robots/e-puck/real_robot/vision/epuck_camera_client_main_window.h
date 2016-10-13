#ifndef EPUCK_CAMERA_CLIENT_MAIN_WINDOW_H
#define EPUCK_CAMERA_CLIENT_MAIN_WINDOW_H

namespace argos {
   class CEPuckCameraClientImageGrabberConfigurator;
   class CEPuckCameraClientBlobDetectorConfigurator;
   class CEPuckCameraClientDistanceConverterConfigurator;
}

#include "client_server_protocol.h"
#include <argos3/core/utility/networking/tcp_socket.h>
#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


class QAction;
class QToolBar;

namespace argos {

   class CEPuckCameraClientMainWindow : public QMainWindow {

      Q_OBJECT

   public:

      CEPuckCameraClientMainWindow(const QString& str_hostname,
                                   quint16 un_port);
      virtual ~CEPuckCameraClientMainWindow() {}

   private:

      void closeEvent(QCloseEvent* pc_event);

      void CreateActions();
      void CreateToolBar();
      void CreateImageWidgets();

      void SendCommand(argos::ECameraConfiguratorCommand e_command);
      void SetStatusMsg(const QString& str_message);

   private slots:

      void GetAttributes();
      void SetAttributes();
      void Refresh();
      void SaveConfiguration();

      void RefreshTimerTimeout();
      void PauseChecked();
   private:

      pthread_t m_tRefresh;

      argos::CTCPSocket m_cSocket;
      QToolBar*         m_pcToolBar;
      QAction*          m_pcActionSaveConfiguration;
      QAction*          m_pcActionPause;
      CEPuckCameraClientImageGrabberConfigurator* m_pcImageGrabberConf;
      CEPuckCameraClientBlobDetectorConfigurator* m_pcBlobDetectorConf;
      CEPuckCameraClientDistanceConverterConfigurator* m_pcDistanceConverterConf;

      QTimer* m_pcRefreshTimer;

   };

}

#endif
