#ifndef EPUCK_IMAGE_GRABBER_H
#define EPUCK_IMAGE_GRABBER_H

#include <argos3/plugins/robots/e-puck/real_robot/vision/image_grabber.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/vector2.h>

namespace argos {

class CEPuckImageGrabber : public CImageGrabber {

public:

    CEPuckImageGrabber();
    virtual ~CEPuckImageGrabber() {}

    virtual void Init(TConfigurationNode& t_tree);
    virtual void Reset() {}
    virtual void Destroy();

    virtual void SaveXMLConfiguration(const std::string& str_path);

    virtual void GrabFrame(CImage<SHSV888Pixel>& c_image);

    virtual size_t GetImageWidth() const;
    virtual size_t GetImageHeight() const;

    inline UInt32 GetGain() const {
        return m_unImageGain;
    }

    void SetGain(UInt32 un_gain);

    inline UInt32 GetExposure() const {
        return m_unImageExposure;
    }

    void SetExposure(UInt32 un_exposure);

    inline UInt32 GetImageCenterX() const {
        return m_unImageCenterX;
    }

    inline UInt32 GetImageCenterY() const {
        return m_unImageCenterY;
    }

    inline void SetImageCenterX(UInt32 un_center_x) {
        m_unImageCenterX = un_center_x;
    }

    inline void SetImageCenterY(UInt32 un_center_y) {
        m_unImageCenterY = un_center_y;
    }

    inline UInt32 GetImageRadius() const {
        return m_unImageRadius;
    }

    inline void SetImageRadius(UInt32 un_radius) {
        m_unImageRadius = un_radius;
    }

private:

    /* Initialize V4L stuff */
    void V4LInit();

    /* Destroy V4L stuff */
    void V4LDestroy();

    /* Call V4L to grab a frame */
    void V4LGrabFrame(CImage<SHSV888Pixel>& c_image);

private:

    /* Image buffer type */
    struct SImageBuffer {
        UInt8*  Data;
        size_t Size;
    };

private:

    /* Camera image width */
    UInt32 m_unImageWidth;

    /* Camera image height */
    UInt32 m_unImageHeight;

    /* Device file descriptor */
    SInt32 m_nDeviceFile;

    /* Image center */
    UInt32 m_unImageCenterX;
    UInt32 m_unImageCenterY;

    /* Radius of the filtered area */
    UInt32 m_unImageRadius;

    /* Gain */
    UInt32 m_unImageGain;
    /* Exposure */
    UInt32 m_unImageExposure;

    SImageBuffer* m_psImgBuffers;

};

}
#endif
