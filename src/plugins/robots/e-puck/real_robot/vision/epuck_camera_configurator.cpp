#include "epuck_camera_configurator.h"
#include "client_server_protocol.h"

#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
namespace argos {

/****************************************/
/****************************************/

static const SInt32 LISTEN_PORT = 6969;

/****************************************/
/****************************************/

CEPuckCameraConfigurator::CEPuckCameraConfigurator(const std::string& str_conf_file) :
    m_strConfFileName(str_conf_file),
    m_pcImageGrabber(NULL),
    m_pcBlobDetector(NULL),
    m_pcDistanceConverter(NULL) {
    try{
        /*
             Check if the XML file exists..
            If it does not exist, create it with default parameters*/
        if(!( access( m_strConfFileName.c_str(), F_OK ) != -1 )){
            LOGERR<<"[WARNING] File invalid or not found. Using default values.\n";
            std::ofstream s_file;
            s_file.open(m_strConfFileName.c_str());
            s_file<<"<?xml version=\"1.0\" ?>"<<"\n";
            s_file<<"<camera-configuration>"<<"\n";
            s_file<<"<img_grabber center_x=\"176\" center_y=\"144\" width=\"352\" height=\"288\" radius=\"140\" gain=\"28\" exposure=\"0\" />"<<"\n";
            for (size_t i = 0; i < CBlobDetector::u_color_number; ++i) {
                s_file<<"<"<<CBlobDetector::colors[i]<<" enabled=\"false\" hue_min=\"0\" hue_max=\"180\" sat_min=\"0\" sat_max=\"255\" value_min=\"0\" value_max=\"255\" area_min=\"0\" area_max=\"100000\" />"<<"\n";
            }
            s_file<<"<pixel_to_cm_conversion alpha=\"0.13228\" beta=\"1.05702\" one_cm=\"0.0\" one_pixel=\"0.0\" two_cm=\"0.0\" two_pixel=\"0.0\">"<<"\n";
            s_file<<"</camera-configuration>"<<"\n";
            s_file.close();
        }
        /*
          * Load and parse the XML configuration file
          */
        ticpp::Document tConf(m_strConfFileName);
        tConf.LoadFile();

        /*
          * Initialize the image grabber, the detectors and the converter
          */
        m_pcImageGrabber = new CEPuckImageGrabber;
        m_pcImageGrabber->Init(*tConf.FirstChildElement());

        m_pcBlobDetector = new CBlobDetector(m_pcImageGrabber->GetImageWidth(),
                                             m_pcImageGrabber->GetImageHeight(),
                                             true);
        m_pcBlobDetector->Init(*tConf.FirstChildElement());
        
        m_pcDistanceConverter = new CDistanceConverter();
        m_pcDistanceConverter->Init(*tConf.FirstChildElement());
        /*
          * Initialize image buffer
          */
        m_pcHSVImage = new CImage<SHSV888Pixel>(m_pcImageGrabber->GetImageWidth(),
                                                m_pcImageGrabber->GetImageHeight());
    }
    catch(std::exception& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error initializing the camera configurator", ex);
    }
}

/****************************************/
/****************************************/

CEPuckCameraConfigurator::~CEPuckCameraConfigurator() {
    m_pcImageGrabber->Destroy();
    m_pcBlobDetector->Destroy();
    m_pcDistanceConverter->Destroy();
    delete m_pcImageGrabber;
    delete m_pcBlobDetector;
    delete m_pcDistanceConverter;
}

/****************************************/
/****************************************/

void CEPuckCameraConfigurator::Execute() {
    LOG << "[INFO] Waiting for connections on port " << LISTEN_PORT << std::endl;
    LOG.Flush();
    m_cListenSocket.Listen(LISTEN_PORT, 1);
    m_cListenSocket.Accept(m_cCommandSocket);
    LOG << "[INFO] Client connected" << std::endl;
    LOG.Flush();
    CommandLoop();
}

/****************************************/
/****************************************/

void CEPuckCameraConfigurator::CommandLoop() {
    UInt8 unCommand;
    while(1) {
        try {
            /* Receive command */
            if(m_cCommandSocket.ReceiveBuffer(&unCommand, 1)) {
                /* Command received, execute it */
                ExecuteCommand(unCommand);
            }
            else {
                /* Client disconnected normally */
                LOG << "[INFO] Client disconnected." << std::endl;
                LOG.Flush();
                return;
            }
        }
        catch(CARGoSException& ex) {
            /* Error */
            THROW_ARGOSEXCEPTION_NESTED("Can't receive command from client" , ex);
        }
    }
}

/****************************************/
/****************************************/

void CEPuckCameraConfigurator::ExecuteCommand(UInt8 un_command) {
    switch(un_command) {
    case COMMAND_GET_ATTRIBUTES:
        GetAttributes();
        break;
    case COMMAND_SET_ATTRIBUTES:
        SetAttributes();
        break;
    case COMMAND_REFRESH:
        Refresh();
        break;
    case COMMAND_SAVE_CONFIGURATION:
        SaveConfiguration();
        break;
    default:
        LOGERR << "Wrong command " << un_command << " received" << std::endl;
        LOGERR.Flush();
    }
}

/****************************************/
/****************************************/

void CEPuckCameraConfigurator::GetAttributes() {
    try {
        /*
          * Fill in the data buffer
          */
        CByteArray cBuffer;
        /* Image grabber */
        cBuffer << m_pcImageGrabber->GetImageWidth();
        cBuffer << m_pcImageGrabber->GetImageHeight();
        cBuffer << m_pcImageGrabber->GetImageCenterX();
        cBuffer << m_pcImageGrabber->GetImageCenterY();
        cBuffer << m_pcImageGrabber->GetImageRadius();
        cBuffer << m_pcImageGrabber->GetGain();
        cBuffer << m_pcImageGrabber->GetExposure();
        /* Blob detector */
        const std::vector<CBlobDetector::SDetectionData*>& cDetectors = m_pcBlobDetector->GetDetectors();
        cBuffer << static_cast<UInt32>(cDetectors.size());
        for(size_t i = 0; i < cDetectors.size(); ++i) {

            cBuffer << static_cast<UInt8>(cDetectors[i]->Enabled);
            cBuffer << cDetectors[i]->Color.GetRed();
            cBuffer << cDetectors[i]->Color.GetGreen();
            cBuffer << cDetectors[i]->Color.GetBlue();
            cBuffer << cDetectors[i]->HueMin;
            cBuffer << cDetectors[i]->HueMax;
            cBuffer << cDetectors[i]->SaturationRange.GetMin();
            cBuffer << cDetectors[i]->SaturationRange.GetMax();
            cBuffer << cDetectors[i]->ValueRange.GetMin();
            cBuffer << cDetectors[i]->ValueRange.GetMax();
            cBuffer << cDetectors[i]->AreaRange.GetMin();
            cBuffer << cDetectors[i]->AreaRange.GetMax();
        }
        /* Distance converter */
        cBuffer << m_pcDistanceConverter->GetFirstDistanceCm();
        cBuffer << m_pcDistanceConverter->GetFirstDistancePixel();
        cBuffer << m_pcDistanceConverter->GetSecondDistanceCm();
        cBuffer << m_pcDistanceConverter->GetSecondDistancePixel();
        cBuffer << m_pcDistanceConverter->GetAlphaVariable();
        cBuffer << m_pcDistanceConverter->GetBetaVariable();
        /* Send the data */
        m_cCommandSocket.SendByteArray(cBuffer);
        LOG << "[INFO] Attributes sent to client (total: " << cBuffer.Size() << " bytes)." << std::endl;
        LOG.Flush();
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error sending attributes to client", ex);
    }
}

/****************************************/
/****************************************/

void CEPuckCameraConfigurator::SetAttributes() {
    try {
        /*
          * Receive the data
          */
        CByteArray cBuffer;
        if(! m_cCommandSocket.ReceiveByteArray(cBuffer)) {
            THROW_ARGOSEXCEPTION("Connection closed by client");
        }
        LOG << "[INFO] Attributes received from client (total: " << cBuffer.Size() << " bytes)" << std::endl;



        /*
          * Configure image grabber and filters
          */
        /* Image grabber */
        m_pcImageGrabber->SetImageCenterX(cBuffer.PopFront<UInt32>());
        m_pcImageGrabber->SetImageCenterY(cBuffer.PopFront<UInt32>());
        m_pcImageGrabber->SetImageRadius(cBuffer.PopFront<UInt32>());
        m_pcImageGrabber->SetGain(cBuffer.PopFront<SInt32>());
        m_pcImageGrabber->SetExposure(cBuffer.PopFront<SInt32>());
        /* Blob detector */
        const std::vector<CBlobDetector::SDetectionData*>& cDetectors = m_pcBlobDetector->GetDetectors();
        size_t unDetectorsNum = cBuffer.PopFront<UInt32>();
        if(unDetectorsNum != cDetectors.size()) {
            THROW_ARGOSEXCEPTION("Expected configuration for " << cDetectors.size() << " detectors, received only " << unDetectorsNum);
        }
        for(size_t i = 0; i < unDetectorsNum; ++i) {
            cDetectors[i]->Enabled=cBuffer.PopFront<UInt8>();
            UInt8 r,g,b;
            r=cBuffer.PopFront<UInt8>();
            g=cBuffer.PopFront<UInt8>();
            b=cBuffer.PopFront<UInt8>();
            cDetectors[i]->Color.Set(r,g,b);
            UInt8 min,max;
            min=cBuffer.PopFront<UInt8>();
            max=cBuffer.PopFront<UInt8>();
            cDetectors[i]->HueMin=min;
            cDetectors[i]->HueMax=max;
            min=cBuffer.PopFront<UInt8>();
            max=cBuffer.PopFront<UInt8>();
            cDetectors[i]->SaturationRange.Set(min,max);
            min=cBuffer.PopFront<UInt8>();
            max=cBuffer.PopFront<UInt8>();
            cDetectors[i]->ValueRange.Set(min,max);
            UInt32 un_area_min=cBuffer.PopFront<UInt32>();
            UInt32 un_area_max=cBuffer.PopFront<UInt32>();
            cDetectors[i]->AreaRange.Set(un_area_min,un_area_max);
        }
        /* Distance converter */
        m_pcDistanceConverter->SetFirstDistanceCm(cBuffer.PopFront<Real>());
		m_pcDistanceConverter->SetFirstDistancePixel(cBuffer.PopFront<Real>());
		m_pcDistanceConverter->SetSecondDistanceCm(cBuffer.PopFront<Real>());
		m_pcDistanceConverter->SetSecondDistancePixel(cBuffer.PopFront<Real>());
		m_pcDistanceConverter->SetAlphaVariable(cBuffer.PopFront<Real>());
		m_pcDistanceConverter->SetBetaVariable(cBuffer.PopFront<Real>());
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error receiving attributes from client", ex);
    }

}


/****************************************/
/****************************************/

void CEPuckCameraConfigurator::Refresh() {

    try {
        size_t unTotalDataSent = 0;
        /* Grab image from camera */
        m_pcImageGrabber->GrabFrame(*m_pcHSVImage);
        /* Apply blob detectors */
        m_pcBlobDetector->Detect(*m_pcHSVImage);
        /* Send image to client */
        m_cCommandSocket.SendByteArray(m_pcHSVImage->GetData());
        unTotalDataSent += m_pcHSVImage->GetData().Size() + sizeof(UInt32);
        /*
          * Send detector data
          */
        const std::vector<CBlobDetector::SDetectionData*>& cDetectors = m_pcBlobDetector->GetDetectors();
        /* Send number of detectors */
        UInt32 unDetectorNum = cDetectors.size();
        m_cCommandSocket.SendBuffer(reinterpret_cast<UInt8*>(&unDetectorNum),
                                    sizeof(unDetectorNum));
        unTotalDataSent += sizeof(unDetectorNum);
        UInt32 unBitSetStorageSize;
        UInt32 unBlobListSize;
        for(size_t i = 0; i < unDetectorNum; ++i) {
            /* Send size of image after color detector */
            unBitSetStorageSize = cDetectors[i]->DetectedPixels->StorageSize();
            m_cCommandSocket.SendBuffer(reinterpret_cast<UInt8*>(&unBitSetStorageSize),
                                        sizeof(unBitSetStorageSize));
            unTotalDataSent += sizeof(unBitSetStorageSize);
            /* Send data of image after color detector */
            m_cCommandSocket.SendBuffer(*(cDetectors[i]->DetectedPixels),
                                        unBitSetStorageSize);
            unTotalDataSent += unBitSetStorageSize;
            /* Send size of blob list */
            unBlobListSize = cDetectors[i]->LabelBlobs.size();
            m_cCommandSocket.SendBuffer(reinterpret_cast<UInt8*>(&unBlobListSize),
                                        sizeof(unBlobListSize));
            unTotalDataSent += sizeof(unBlobListSize);
            /* Send blob data */
            for(std::map<UInt16, CBlobDetector::SBlob>::iterator it = cDetectors[i]->LabelBlobs.begin();
                it != cDetectors[i]->LabelBlobs.end();
                ++it) {
                /* Send blob center I */
                m_cCommandSocket.SendBuffer(reinterpret_cast<UInt8*>(&(it->second.CenterI)),
                                            sizeof(UInt32));
                /* Send blob center J */
                m_cCommandSocket.SendBuffer(reinterpret_cast<UInt8*>(&(it->second.CenterJ)),
                                            sizeof(UInt32));
                /* Send blob area */
                m_cCommandSocket.SendBuffer(reinterpret_cast<UInt8*>(&(it->second.Area)),
                                            sizeof(UInt32));
                unTotalDataSent += sizeof(UInt32) * 3;
            }
        }
        LOG << "[INFO] Data sent to client (total: " << unTotalDataSent << " bytes)." << std::endl;
        LOG.Flush();
    }
    catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error sending image to client", ex);
    }
}

/****************************************/
/****************************************/

void CEPuckCameraConfigurator::SaveConfiguration() {
    try {
        m_pcImageGrabber->SaveXMLConfiguration(m_strConfFileName);
        m_pcBlobDetector->SaveXMLConfiguration(m_strConfFileName);
        m_pcDistanceConverter->SaveXMLConfiguration(m_strConfFileName);
    } catch (CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error saving the configuration", ex);
    }
}

/****************************************/
/****************************************/

}

int main(int n_argc, char** pch_argv) {
    if(n_argc != 2) {
        argos::LOGERR << "[FATAL] " << pch_argv[0] << " <conf_file>" << std::endl;
        argos::LOGERR.Flush();
        return 1;
    }
    try {
        argos::CEPuckCameraConfigurator cECC(pch_argv[1]);
        cECC.Execute();
    }
    catch(argos::CARGoSException& ex) {
        argos::LOGERR << ex.what() << std::endl;
        argos::LOG.Flush();
        argos::LOGERR.Flush();
        return 1;
    }
    return 0;
}
