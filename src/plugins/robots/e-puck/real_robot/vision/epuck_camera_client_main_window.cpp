#include "epuck_camera_client_main_window.h"
#include "epuck_camera_client_image_grabber_configurator.h"
#include "epuck_camera_client_blob_detector_configurator.h"
#include "epuck_camera_client_distance_converter_configurator.h"
#include "blob_detector.h"

#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/general.h>
#include <argos3/core/utility/logging/argos_log.h>

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QStatusBar>
#include <QToolBar>
#include <QTimer>
#include <QThread>
#include <QMessageBox>
#include <cmath>
#include <unistd.h>

namespace argos {

/****************************************/
/****************************************/

static const int NETWORK_TIMEOUT = 5000;
static const int REFRESH_PERIOD = 500;

/****************************************/
/****************************************/

CEPuckCameraClientMainWindow::CEPuckCameraClientMainWindow(const QString& str_hostname,
                                                           quint16 un_port) {
    m_cSocket.Connect(str_hostname.toStdString(), un_port);
    CreateActions();
    CreateToolBar();
    CreateImageWidgets();
    SetStatusMsg("Ready");
}

/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::closeEvent(QCloseEvent* pc_event) {
}

/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::CreateActions() {
    /* Save configuration */
    m_pcActionSaveConfiguration = new QAction(tr("Save &Configuration"), this);
    connect(m_pcActionSaveConfiguration, SIGNAL(triggered()),
            this, SLOT(SaveConfiguration()));
    m_pcActionPause = new QAction(tr("&Pause"), this);
    m_pcActionPause->setCheckable(true);
    connect(m_pcActionPause, SIGNAL(changed()),
            this, SLOT(PauseChecked()));
}

/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::CreateToolBar() {
    m_pcToolBar = addToolBar(tr("Commands"));
    m_pcToolBar->addAction(m_pcActionSaveConfiguration);
    m_pcToolBar->addAction(m_pcActionPause);
}

/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::CreateImageWidgets() {
    /* Create widget to contain subwidgets */
    QWidget* pcCentralWidget = new QWidget();
    /* Send request to get attributes */
    SendCommand(argos::COMMAND_GET_ATTRIBUTES);
    /* Receive data and configure widgets */
    CByteArray cBuffer;
    m_cSocket.ReceiveByteArray(cBuffer);
    /* Create image grabber */
    m_pcImageGrabberConf = new CEPuckCameraClientImageGrabberConfigurator(pcCentralWidget);
    m_pcImageGrabberConf->SetImageWidth(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetImageHeight(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetImageCenterX(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetImageCenterY(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetImageRadius(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetGain(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetExposure(cBuffer.PopFront<UInt32>());
    /* Create blob detector configurator */
    m_pcBlobDetectorConf = new CEPuckCameraClientBlobDetectorConfigurator(pcCentralWidget);
    UInt32 unNumDetectors = cBuffer.PopFront<UInt32>();
    for(size_t i = 0; i < unNumDetectors; ++i) {
        CEPuckCameraClientBlobDetectorConfiguratorTab* pcDetector = new CEPuckCameraClientBlobDetectorConfiguratorTab();
        pcDetector->SetImageWidth(m_pcImageGrabberConf->GetImageWidth());
        pcDetector->SetImageHeight(m_pcImageGrabberConf->GetImageHeight());
        pcDetector->setColorEnabled(cBuffer.PopFront<UInt8>());
        UInt8 r,g,b;
        r=cBuffer.PopFront<UInt8>();
        g=cBuffer.PopFront<UInt8>();
        b=cBuffer.PopFront<UInt8>();
        pcDetector->SetDetectorColor(CColor(r,g,b));
        pcDetector->SetHueRangeMin(cBuffer.PopFront<UInt8>());
        pcDetector->SetHueRangeMax(cBuffer.PopFront<UInt8>());
        pcDetector->SetSaturationRangeMin(cBuffer.PopFront<UInt8>());
        pcDetector->SetSaturationRangeMax(cBuffer.PopFront<UInt8>());
        pcDetector->SetValueRangeMin(cBuffer.PopFront<UInt8>());
        pcDetector->SetValueRangeMax(cBuffer.PopFront<UInt8>());
        pcDetector->SetAreaRangeMin(cBuffer.PopFront<UInt32>());
        pcDetector->SetAreaRangeMax(cBuffer.PopFront<UInt32>());
        m_pcBlobDetectorConf->AddDetector(pcDetector);
    }
    /* Create distance converter configurator */
	m_pcDistanceConverterConf = new CEPuckCameraClientDistanceConverterConfigurator(pcCentralWidget);
	m_pcDistanceConverterConf->SetFirstDistanceCm(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetFirstDistancePixel(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetSecondDistanceCm(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetSecondDistancePixel(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetAlphaVariable(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetBetaVariable(cBuffer.PopFront<Real>());
	
    
    QHBoxLayout* pcLayout = new QHBoxLayout();
    
    QGroupBox* verticalBox = new QGroupBox();
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_pcImageGrabberConf);
    vLayout->addWidget(m_pcDistanceConverterConf);
    verticalBox->setLayout(vLayout);
    
    pcLayout->addWidget(verticalBox);
    pcLayout->addWidget(m_pcBlobDetectorConf);
    
    pcCentralWidget->setLayout(pcLayout);
    setCentralWidget(pcCentralWidget);
    Refresh();


    m_pcRefreshTimer = new QTimer(this);
    m_pcRefreshTimer->setInterval(REFRESH_PERIOD);
    connect(m_pcRefreshTimer, SIGNAL(timeout()), this, SLOT(RefreshTimerTimeout()));
    m_pcRefreshTimer->start();
}

/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::SendCommand(argos::ECameraConfiguratorCommand e_command) {
    argos::UInt8 punCommand = e_command;
    m_cSocket.SendBuffer(&punCommand, 1);
}

/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::SetStatusMsg(const QString& str_message) {
    statusBar()->showMessage(str_message, 2000);
}

/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::GetAttributes() {
    /* Send request to get attributes */
    SendCommand(argos::COMMAND_GET_ATTRIBUTES);
    /*
       * Receive data and configure widgets
       */
    CByteArray cBuffer;
    m_cSocket.ReceiveByteArray(cBuffer);
    printf("Array received %d\n",cBuffer.Size());
    /* Image grabber */
    m_pcImageGrabberConf->SetImageWidth(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetImageHeight(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetImageCenterX(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetImageCenterY(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetImageRadius(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetGain(cBuffer.PopFront<UInt32>());
    m_pcImageGrabberConf->SetExposure(cBuffer.PopFront<UInt32>());
    /* Blob detector */
    UInt32 unNumDetectors = cBuffer.PopFront<UInt32>();
    for(size_t i = 0; i < unNumDetectors; ++i) {
        CEPuckCameraClientBlobDetectorConfiguratorTab* pcDetector = m_pcBlobDetectorConf->GetDetector(i);
        pcDetector->SetImageWidth(m_pcImageGrabberConf->GetImageWidth());
        pcDetector->SetImageHeight(m_pcImageGrabberConf->GetImageHeight());

	pcDetector->SetImageCenterX(m_pcImageGrabberConf->GetImageCenterX());
	pcDetector->SetImageCenterY(m_pcImageGrabberConf->GetImageCenterY());

        UInt8 enabled;
        enabled= cBuffer.PopFront<UInt8>();
        printf("EN %d\n ",enabled);
        UInt8 r,g,b;
        r=cBuffer.PopFront<UInt8>();
        g=cBuffer.PopFront<UInt8>();
        b=cBuffer.PopFront<UInt8>();
        pcDetector->SetDetectorColor(
                    CColor(r,g,b));
        pcDetector->SetHueRangeMin(cBuffer.PopFront<UInt8>());
        pcDetector->SetHueRangeMax(cBuffer.PopFront<UInt8>());
        pcDetector->SetSaturationRangeMin(cBuffer.PopFront<UInt8>());
        pcDetector->SetSaturationRangeMax(cBuffer.PopFront<UInt8>());
        pcDetector->SetValueRangeMin(cBuffer.PopFront<UInt8>());
        pcDetector->SetValueRangeMax(cBuffer.PopFront<UInt8>());
        pcDetector->SetAreaRangeMin(cBuffer.PopFront<UInt32>());
        pcDetector->SetAreaRangeMax(cBuffer.PopFront<UInt32>());
    }
    /* Distance converter */
    m_pcDistanceConverterConf->SetFirstDistanceCm(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetFirstDistancePixel(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetSecondDistanceCm(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetSecondDistancePixel(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetAlphaVariable(cBuffer.PopFront<Real>());
	m_pcDistanceConverterConf->SetBetaVariable(cBuffer.PopFront<Real>());
}

/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::SetAttributes() {
    //Get the values from the GUI components and send'em to the server
    try {
        /* Send request to set attributes */
        SendCommand(argos::COMMAND_SET_ATTRIBUTES);
        /*
          * Fill in the data buffer
          */
        CByteArray cBuffer;
        /* Image grabber parameters*/
        cBuffer << m_pcImageGrabberConf->GetImageCenterX();
        cBuffer << m_pcImageGrabberConf->GetImageCenterY();
        cBuffer << m_pcImageGrabberConf->GetImageRadius();
        cBuffer << m_pcImageGrabberConf->GetGain();
        cBuffer << m_pcImageGrabberConf->GetExposure();
        /* Blob detector */
        //const std::vector<CBlobDetector::SDetectionData*>& cDetectors = m_pcBlobDetectorConf->GetDetector();

        size_t un_size=m_pcBlobDetectorConf->count();
        cBuffer << static_cast<UInt32>(un_size);
        for(size_t i = 0; i < un_size; ++i) {
            CEPuckCameraClientBlobDetectorConfiguratorTab* pcDetector = m_pcBlobDetectorConf->GetDetector(i);
            cBuffer << static_cast<UInt8>(pcDetector->ColorEnabled());
            cBuffer << pcDetector->GetDetectorColor().GetRed();
            cBuffer << pcDetector->GetDetectorColor().GetGreen();
            cBuffer << pcDetector->GetDetectorColor().GetBlue();

            cBuffer << pcDetector->GetHueRangeMin();
            cBuffer << pcDetector->GetHueRangeMax();

            cBuffer << pcDetector->GetSaturationRangeMin();
            cBuffer <<pcDetector->GetSaturationRangeMax();

            cBuffer << pcDetector->GetValueRangeMin();
            cBuffer <<pcDetector->GetValueRangeMax();

            cBuffer <<pcDetector->GetAreaRangeMin();
            cBuffer <<pcDetector->GetAreaRangeMax();
        }
        /* Distance converter */
        cBuffer << m_pcDistanceConverterConf->GetFirstDistanceCm();
        cBuffer << m_pcDistanceConverterConf->GetFirstDistancePixel();
        cBuffer << m_pcDistanceConverterConf->GetSecondDistanceCm();
        cBuffer << m_pcDistanceConverterConf->GetSecondDistancePixel();
        cBuffer << m_pcDistanceConverterConf->GetAlphaVariable();
        cBuffer << m_pcDistanceConverterConf->GetBetaVariable();
        
        /* Send the data */
        m_cSocket.SendByteArray(cBuffer);
        LOG << "[INFO] Attributes sent to client (total: " << cBuffer.Size() << " bytes)." << std::endl;
        LOG.Flush();
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error sending attributes to client", ex);
    }
}
/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::PauseChecked() {
    if(m_pcActionPause->isChecked()){
        m_pcBlobDetectorConf->EnablePause(true);

    }else{
        m_pcBlobDetectorConf->EnablePause(false);

    }
}

/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::RefreshTimerTimeout() {
    if(!m_pcActionPause->isChecked()){
    m_pcRefreshTimer->stop();
    SetAttributes();
    Refresh();
    m_pcRefreshTimer->start();
    }
}


/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::Refresh() {

    /* Send command to server */
    SendCommand(argos::COMMAND_REFRESH);
    /*
       * Receive data
       */
    CByteArray cBuffer;
    /* Receive raw image data */
    m_cSocket.ReceiveByteArray(cBuffer);
    m_pcImageGrabberConf->SetImageData(cBuffer);
    /* Receive blob detector data */
    UInt32 unBitSetStorageSize;
    UInt32 unBlobListSize;
    UInt32 unNumDetectors;
    UInt32 unBlobCenterI, unBlobCenterJ, unBlobArea;
    m_cSocket.ReceiveBuffer(reinterpret_cast<UInt8*>(&unNumDetectors),
                            sizeof(unNumDetectors));
    for(size_t i = 0; i < unNumDetectors; ++i) {
        CEPuckCameraClientBlobDetectorConfiguratorTab* pcDetector = m_pcBlobDetectorConf->GetDetector(i);
        /* Receive size of image after color detector */
        m_cSocket.ReceiveBuffer(reinterpret_cast<UInt8*>(&unBitSetStorageSize),
                                sizeof(unBitSetStorageSize));
        /* Receive data of image after color detector */
        UInt8* unBitSet = new UInt8[unBitSetStorageSize];
        m_cSocket.ReceiveBuffer(unBitSet,
                                unBitSetStorageSize);
        /* Set data and clear buffer */
        //pcDetector->SetPixelFilterData(unBitSet, unBitSetStorageSize);
        pcDetector->SetImagesData(cBuffer,unBitSet,unBitSetStorageSize);
        delete[] unBitSet;
        /* Receive size of blob list */
        m_cSocket.ReceiveBuffer(reinterpret_cast<UInt8*>(&unBlobListSize),
                                sizeof(unBlobListSize));
        /* Receive blob data */
        pcDetector->ClearBlobData();
        for(size_t j = 0; j < unBlobListSize; ++j) {
            /* Receive blob center I */
            m_cSocket.ReceiveBuffer(reinterpret_cast<UInt8*>(&unBlobCenterI),
                                    sizeof(unBlobCenterI));
            /* Receive blob center J */
            m_cSocket.ReceiveBuffer(reinterpret_cast<UInt8*>(&unBlobCenterJ),
                                    sizeof(unBlobCenterJ));
            /* Receive blob area */
            m_cSocket.ReceiveBuffer(reinterpret_cast<UInt8*>(&unBlobArea),
                                    sizeof(unBlobArea));
            /* Add blob data */
            pcDetector->AddBlobData(unBlobCenterI, unBlobCenterJ, unBlobArea);

	    pcDetector->SetImageCenterX(m_pcImageGrabberConf->GetImageCenterX());
	    pcDetector->SetImageCenterY(m_pcImageGrabberConf->GetImageCenterY());
        }
    }

}


/****************************************/
/****************************************/

void CEPuckCameraClientMainWindow::SaveConfiguration() {
    m_pcRefreshTimer->stop();
    SetAttributes();
    SendCommand(argos::COMMAND_SAVE_CONFIGURATION);
    QMessageBox msgBox;
    msgBox.setText("Configuration Saved");
    msgBox.exec();
    m_pcRefreshTimer->start();
}

/****************************************/
/****************************************/

}

int main(int n_argc, char** pch_argv) {
    if(n_argc != 3) {
        qCritical("Hostname and port expected");
        return 1;
    }
    QApplication cApp(n_argc, pch_argv);
    argos::CEPuckCameraClientMainWindow cMainWindow(pch_argv[1],
            QString(pch_argv[2]).toInt());
    cMainWindow.show();
    return cApp.exec();
}
