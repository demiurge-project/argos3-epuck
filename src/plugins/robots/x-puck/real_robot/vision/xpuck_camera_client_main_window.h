#ifndef XPUCK_CAMERA_CLIENT_MAIN_WINDOW_H
#define XPUCK_CAMERA_CLIENT_MAIN_WINDOW_H

namespace argos {
   class CXPuckCameraClientImageGrabberConfigurator;
   class CXPuckCameraClientBlobDetectorConfigurator;
   class CXPuckCameraClientDistanceConverterConfigurator;
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

   class CXPuckCameraClientMainWindow : public QMainWindow {

      Q_OBJECT

   public:

      CXPuckCameraClientMainWindow(const QString& str_hostname,
                                   quint16 un_port);
      virtual ~CXPuckCameraClientMainWindow() {}

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
      CXPuckCameraClientImageGrabberConfigurator* m_pcImageGrabberConf;
      CXPuckCameraClientBlobDetectorConfigurator* m_pcBlobDetectorConf;
      CXPuckCameraClientDistanceConverterConfigurator* m_pcDistanceConverterConf;

      QTimer* m_pcRefreshTimer;

   };

}

#endif
