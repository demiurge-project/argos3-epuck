#include "epuck_image_grabber.h"

#include <argos3/core/utility/configuration/tinyxml/ticpp.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace argos {

/****************************************/
/****************************************/

const std::string CONFIGURATION_TAG              = "e-puck";
const std::string CONFIGURATION_TAG_GAIN         = "gain";
const std::string CONFIGURATION_TAG_EXPOSURE     = "exposure";
const std::string CONFIGURATION_TAG_CENTER_X     = "center_x";
const std::string CONFIGURATION_TAG_CENTER_Y     = "center_y";
const std::string CONFIGURATION_TAG_IMAGE_WIDTH  = "image_width";
const std::string CONFIGURATION_TAG_IMAGE_HEIGHT = "image_height";

const std::string CAMERA_DEVICE_NAME = "/dev/video1";

/****************************************/
/****************************************/

static int XIOCTL(int   n_fd,
                  int   n_request,
                  void* p_arg) {
    int nRetVal;
    do {
        nRetVal = ::ioctl(n_fd, n_request, p_arg);
    }
    while(nRetVal == -1 &&
          errno == EINTR);
    return nRetVal;
}

/****************************************/
/****************************************/

CEPuckImageGrabber::CEPuckImageGrabber() :
    m_unImageWidth(352),
    m_unImageHeight(288),
    m_nDeviceFile(-1),
    m_unImageCenterX(182),
    m_unImageCenterY(144),
    m_unImageRadius(140),
    m_unImageGain(26),
    m_unImageExposure(0){
}

/****************************************/
/****************************************/

void CEPuckImageGrabber::Init(TConfigurationNode& t_tree) {
    TConfigurationNode tGroundNode = GetNode(t_tree, "img_grabber");
    GetNodeAttributeOrDefault(tGroundNode, "center_x", m_unImageCenterX, m_unImageCenterX);
    GetNodeAttributeOrDefault(tGroundNode, "center_y",m_unImageCenterY,m_unImageCenterY);
    GetNodeAttributeOrDefault(tGroundNode, "width",m_unImageWidth,m_unImageWidth);
    GetNodeAttributeOrDefault(tGroundNode, "height",m_unImageHeight,m_unImageHeight);
    GetNodeAttributeOrDefault(tGroundNode, "radius",m_unImageRadius,m_unImageRadius);
    GetNodeAttributeOrDefault(tGroundNode, "gain",m_unImageGain,m_unImageGain);
    GetNodeAttributeOrDefault(tGroundNode, "exposure",m_unImageExposure,m_unImageExposure);
    /* Open device and set file descriptor */
    m_nDeviceFile = ::open(CAMERA_DEVICE_NAME.c_str(),
                           O_RDWR | O_NONBLOCK,
                           0);
    if(m_nDeviceFile < 0) {
        THROW_ARGOSEXCEPTION("Can't open camera device file: " << ::strerror(errno));
    }
    /* Initialize video-for-linux-related stuff */
    V4LInit();
}

/****************************************/
/****************************************/

void CEPuckImageGrabber::Destroy() {
    /* Destroy video-for-linux-related stuff */
    V4LDestroy();
    /* Close device file */
    if(::close(m_nDeviceFile) == -1) {
        THROW_ARGOSEXCEPTION("Can't close camera device file: " << ::strerror(errno));
    }
}

/****************************************/
/****************************************/

void CEPuckImageGrabber::SaveXMLConfiguration(const std::string& str_path) {
    /*
     * Load and parse the XML configuration file
     */
    ticpp::Document tDocument(str_path);
    tDocument.LoadFile();
    ticpp::Element tConf = *tDocument.FirstChildElement();
    ticpp::Element tConfElement("img_grabber");
    SetNodeAttribute(tConfElement,"center_x",m_unImageCenterX);
    SetNodeAttribute(tConfElement,"center_y",m_unImageCenterY);
    SetNodeAttribute(tConfElement,"width",m_unImageWidth);
    SetNodeAttribute(tConfElement,"height",m_unImageHeight);
    SetNodeAttribute(tConfElement,"radius",m_unImageRadius);
    SetNodeAttribute(tConfElement,"gain",m_unImageGain);
    SetNodeAttribute(tConfElement,"exposure",m_unImageExposure);
    tConf.ReplaceChild(tConf.FirstChildElement("img_grabber"),tConfElement);
    tDocument.SaveFile(str_path);


}

/****************************************/
/****************************************/

void CEPuckImageGrabber::GrabFrame(CImage<SHSV888Pixel>& c_image) {
    /* Initialize the files to monitor */
    fd_set tMonitoredFiles;
    FD_ZERO(&tMonitoredFiles);
    FD_SET(m_nDeviceFile, &tMonitoredFiles);
    /* Initialize the monitoring timeout */
    timeval tMonitoringTimeout;
    tMonitoringTimeout.tv_sec = 5;
    tMonitoringTimeout.tv_usec = 0;
    /*
       * Wait for the device file to be ready for non-blocking access
       * It could happen that waiting exceeds the timeout
       * In this case, we perform another cycle
       */

    SInt32 nWaitingResult = 0;
    while(nWaitingResult == 0) {
        nWaitingResult = ::select(m_nDeviceFile + 1, /* The +1 is wanted by select()'s interface */
                                  &tMonitoredFiles,
                                  NULL,
                                  NULL,
                                  &tMonitoringTimeout);
        if(nWaitingResult > 0) {
            V4LGrabFrame(c_image);
        }
        else if(nWaitingResult < 0) {
            THROW_ARGOSEXCEPTION("Error waiting for the camera device file to be ready for access: " << ::strerror(errno));
        }
        else { /* nWaitingResult == 0 */
            LOGERR << "[WARNING] Timeout expired while waiting for the camera device file to be ready for access" << std::endl;
            LOGERR.Flush();
            return;
        }
    }
}

/****************************************/
/****************************************/

size_t CEPuckImageGrabber::GetImageWidth() const {
    return m_unImageWidth;
}

/****************************************/
/****************************************/

size_t CEPuckImageGrabber::GetImageHeight() const {
    return m_unImageHeight;
}

/****************************************/
/****************************************/

void CEPuckImageGrabber::V4LInit() {
    /* Query device capabilities */
    v4l2_capability tCapability;
    if(XIOCTL(m_nDeviceFile, VIDIOC_QUERYCAP, &tCapability) == -1) {
        THROW_ARGOSEXCEPTION("VIDIOC_QUERYCAP: " << ::strerror(errno));
    }
    /* Setup image cropping */
    v4l2_cropcap tCropCapability;
    v4l2_crop tCrop;
    ::memset(&tCropCapability, 0, sizeof(tCropCapability));
    tCropCapability.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(XIOCTL(m_nDeviceFile,
              VIDIOC_CROPCAP,
              &tCropCapability) == 0) {
        tCrop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        tCrop.c = tCropCapability.defrect;
        XIOCTL(m_nDeviceFile, VIDIOC_S_CROP, &tCrop);
    }
    /* Setup image format */
    v4l2_format tFormat;
    ::memset(&tFormat, 0, sizeof(tFormat));
    tFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tFormat.fmt.pix.width = m_unImageWidth;
    tFormat.fmt.pix.height = m_unImageHeight;
    tFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    tFormat.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if(XIOCTL(m_nDeviceFile,
              VIDIOC_S_FMT,
              &tFormat) == -1) {
        THROW_ARGOSEXCEPTION("VIDIOC_S_TFORMAT: " << ::strerror(errno));
    }
    /* VIDIOC_S_FMT may change width and height */
    if(m_unImageWidth != tFormat.fmt.pix.width) {
        m_unImageWidth = tFormat.fmt.pix.width;
        LOGERR << "[WARNING] Image width set to " << m_unImageWidth << " by device." << std::endl;
    }
    if(m_unImageHeight != tFormat.fmt.pix.height) {
        m_unImageWidth = tFormat.fmt.pix.height;
        LOGERR << "[WARNING] Image height set to " << m_unImageHeight << " by device." << std::endl;
    }
    /*
       * An older version of the camera driver was setting the image buffer
       * to an incorrect (i.e., smaller) size than necessary.
       * Here we check whether the bug is present, and fix the buffer size
       * in case it is.
       */
    /* Check that bytesperline is correct */
    UInt32 unMinSize = tFormat.fmt.pix.width * 2;
    if(tFormat.fmt.pix.bytesperline < unMinSize) {
        LOGERR << "[WARNING] Driver set wrong value for fmt.pix.bytesperline: "
               << tFormat.fmt.pix.bytesperline
               << ". ";
        tFormat.fmt.pix.bytesperline = unMinSize;
        LOGERR << "Fixed with correct value "
               << tFormat.fmt.pix.bytesperline
               << "."
               << std::endl;
    }
    unMinSize = tFormat.fmt.pix.bytesperline * tFormat.fmt.pix.height;
    if(tFormat.fmt.pix.sizeimage < unMinSize) {
        LOGERR << "[WARNING] Driver set wrong value for fmt.pix.sizeimage: "
               << tFormat.fmt.pix.sizeimage
               << ". ";
        tFormat.fmt.pix.sizeimage = unMinSize;
        LOGERR << "Fixed with correct value "
               << tFormat.fmt.pix.sizeimage
               << "."
               << std::endl;
    }
    /* Request device buffers to the driver */
    v4l2_requestbuffers tReqBuffers;
    ::memset(&tReqBuffers, 0, sizeof(tReqBuffers));
    tReqBuffers.count = 2;
    tReqBuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tReqBuffers.memory = V4L2_MEMORY_MMAP;
    if(XIOCTL(m_nDeviceFile,
              VIDIOC_REQBUFS,
              &tReqBuffers) == -1) {
        THROW_ARGOSEXCEPTION("VIDIOC_REQBUFS: " << ::strerror(errno));
    }
    if(tReqBuffers.count != 2) {
        THROW_ARGOSEXCEPTION("Requested 2 buffers to the driver, obtained " << tReqBuffers.count);
    }
    /* Allocate space for buffers */
    m_psImgBuffers = new SImageBuffer[2];
    /* Assign a memory-mapped file to each buffer */
    struct v4l2_buffer tBuffer;
    for(size_t i = 0; i < 2; ++i) {
        ::memset(&tBuffer, 0, sizeof(tBuffer));
        tBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        tBuffer.memory = V4L2_MEMORY_MMAP;
        tBuffer.index = i;
        if(XIOCTL(m_nDeviceFile,
                  VIDIOC_QUERYBUF,
                  &tBuffer) == -1) {
            THROW_ARGOSEXCEPTION("VIDIOC_QUERYBUF [" << i << "]: " << ::strerror(errno));
        }
        m_psImgBuffers[i].Size = tBuffer.length;
        m_psImgBuffers[i].Data = reinterpret_cast<UInt8*>(
                    ::mmap(NULL /* start anywhere */,
                           tBuffer.length,
                           PROT_READ | PROT_WRITE,
                           MAP_SHARED /* recommended */,
                           m_nDeviceFile,
                           tBuffer.m.offset));
        if(m_psImgBuffers[i].Data == MAP_FAILED) {
            THROW_ARGOSEXCEPTION("Can't create the memory-mapped file #" << i);
        }
    }
    /* Enqueue the capturing buffer in the driver's incoming queue */
    for(size_t i = 0; i < 2; ++i) {
        ::memset(&tBuffer, 0, sizeof(tBuffer));
        tBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        tBuffer.memory = V4L2_MEMORY_MMAP;
        tBuffer.index = i;
        if(XIOCTL(m_nDeviceFile,
                  VIDIOC_QBUF,
                  &tBuffer) == -1) {
            THROW_ARGOSEXCEPTION("VIDIOC_QBUF: " << ::strerror(errno));
        }
    }
    /* Start capturing */
    v4l2_buf_type tType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(XIOCTL(m_nDeviceFile,
              VIDIOC_STREAMON,
              &tType) == -1) {
        THROW_ARGOSEXCEPTION("VIDIOC_STREAMON: " << ::strerror(errno));
    }
}

/****************************************/
/****************************************/

void CEPuckImageGrabber::V4LDestroy() {
    /* Stop capturing */
    v4l2_buf_type tType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(XIOCTL(m_nDeviceFile,
              VIDIOC_STREAMOFF,
              &tType) == -1) {
        THROW_ARGOSEXCEPTION("VIDIOC_STREAMOFF: " << ::strerror(errno));
    }
    /* Dispose of buffers */
    for(size_t i = 0; i < 2; ++i) {
        if(::munmap(m_psImgBuffers[i].Data,
                    m_psImgBuffers[i].Size) == -1) {
            THROW_ARGOSEXCEPTION("Can't release the memory-mapped file #" << i);
        }
    }
    delete m_psImgBuffers;
}

/****************************************/
/****************************************/

void CEPuckImageGrabber::V4LGrabFrame(CImage<SHSV888Pixel>& c_image) {
    /* Setup the image buffer */
    v4l2_buffer tBuffer;
    ::memset(&tBuffer, 0, sizeof(tBuffer));
    tBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    tBuffer.memory = V4L2_MEMORY_MMAP;
    /* Request the driver to access the filled image buffer */
    if((XIOCTL(m_nDeviceFile,
               VIDIOC_DQBUF,
               &tBuffer) == -1) &&
            (errno != EIO)) {
        THROW_ARGOSEXCEPTION("VIDIOC_DQBUF: " << ::strerror(errno));
    }

    /*FAST BUT WITH A BUG*/
    //          MaskedYUYVtoHSV888_Fast_BUG(m_unImageWidth,
    //                             m_unImageHeight,
    //                             c_image.GetData().ToCArray(),
    //                             m_psImgBuffers[tBuffer.index].Data,
    //                             m_unImageCenterX,
    //                             m_unImageCenterY,
    //                             m_unImageRadius);
    /*ARGOS 2 FUNCTION*/
    ARGOS2_YUYVtoHSV888(m_unImageWidth,
                        m_unImageHeight,
                        c_image.GetData().ToCArray(),
                        m_psImgBuffers[tBuffer.index].Data,
            m_unImageCenterX,
            m_unImageCenterY,
            m_unImageRadius);
    if(XIOCTL(m_nDeviceFile,
              VIDIOC_QBUF,
              &tBuffer) == -1) {
        THROW_ARGOSEXCEPTION("VIDIOC_DQBUF: " << ::strerror(errno));
    }
}

/****************************************/
/****************************************/

void CEPuckImageGrabber::SetGain(UInt32 un_gain) {
    if(m_unImageGain==un_gain){
        return;
    }
    m_unImageGain = un_gain;
    struct v4l2_control c_ctrl;
    memset(&c_ctrl, 0, sizeof(c_ctrl));
    c_ctrl.id = V4L2_CID_GAIN;
    c_ctrl.value = un_gain;
    if(XIOCTL(m_nDeviceFile,VIDIOC_S_CTRL, &c_ctrl) == -1) {
        THROW_ARGOSEXCEPTION("Set Gain Failed: " << ::strerror(errno));
    }
}
/****************************************/
/****************************************/

void CEPuckImageGrabber::SetExposure(UInt32 un_exposure) {

    m_unImageExposure = un_exposure;
    /*The camera of the epuck does not support the set of the exposure.
        The code below contains the commands to set it. It is commented
     because it'd throw an exception due to invalid argument*/

    /*struct v4l2_control c_ctrl;
       memset(&c_ctrl, 0, sizeof(c_ctrl));
       c_ctrl.id = V4L2_CID_EXPOSURE;
       c_ctrl.value = un_exposure;
       if(XIOCTL(m_nDeviceFile,VIDIOC_S_CTRL, &c_ctrl) == -1) {
           THROW_ARGOSEXCEPTION("Set Exposure Failed: " << ::strerror(errno));
       }*/
}
/****************************************/
/****************************************/
} /* namespace argos */
