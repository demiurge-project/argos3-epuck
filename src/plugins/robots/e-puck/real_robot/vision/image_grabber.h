#ifndef IMAGE_GRABBER_H
#define IMAGE_GRABBER_H

#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/extended_configurable_resource.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/image.h>

namespace argos {

   /**
    * Defines the generic interface that each image acquisition class must implement.
    */
   class CImageGrabber : public CExtendedConfigurableResource {

   public:

      virtual ~CImageGrabber(){}

      /**
       * Grabs a frame from a device and fills the input argument with the data.
       * @param image The image to fill.
       */
      virtual void GrabFrame(CImage<SHSV888Pixel>& c_image) = 0;

      /**
       * Return the image width in pixel
       * @return The image width
       */
      virtual size_t GetImageWidth() const = 0;

      /**
       * Returns the image heigth in pixel
       * @return The image heigth
       */
      virtual size_t GetImageHeight() const = 0;

   };

}

#endif
