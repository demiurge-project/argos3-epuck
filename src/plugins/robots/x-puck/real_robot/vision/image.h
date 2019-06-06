#ifndef IMAGE_H
#define IMAGE_H

#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/datatypes/datatypes.h>

namespace argos {

   /**
    * RGB888 pixel
    */
   struct SRGB888Pixel {
      UInt8 Red;
      UInt8 Green;
      UInt8 Blue;
   };

   /**
    * HSV888 pixel
    */
   struct SHSV888Pixel {
      UInt8 Hue;
      UInt8 Saturation;
      UInt8 Value;

      SHSV888Pixel() :
         Hue(0),
         Saturation(0),
         Value(0) {}

      SHSV888Pixel(UInt8 un_hue,
                   UInt8 un_saturation,
                   UInt8 un_value) :
         Hue(un_hue),
         Saturation(un_saturation),
         Value(un_value) {}
   };

   /**
    * Image type
    */
   template<typename T> class CImage {
   public:
      CImage(UInt32 un_width,
             UInt32 un_height) :
         m_unWidth(un_width),
         m_unHeight(un_height),
         m_cData(un_width * un_height * sizeof(T)) {}

      ~CImage() {}

      inline UInt32 GetWidth() const {
         return m_unWidth;
      }

      inline UInt32 GetHeight() const {
         return m_unHeight;
      }

      inline CByteArray& GetData() {
         return m_cData;
      }

      inline const CByteArray& GetData() const {
         return m_cData;
      }

      inline const T& operator()(UInt32 un_col, UInt32 un_row) const {
         return
            *reinterpret_cast<const T*>(
               m_cData.ToCArray() + ((un_row * m_unWidth + un_col) * sizeof(T)));
      }

      inline T& operator()(UInt32 un_col, UInt32 un_row) {
         return
            *reinterpret_cast<T*>(
               m_cData.ToCArray() + ((un_row * m_unWidth + un_col) * sizeof(T)));
      }

   private:

      UInt32 m_unWidth;

      UInt32 m_unHeight;

      CByteArray m_cData;
   };

   extern "C" {

      void HSV888toRGB888(size_t un_width,
                          size_t un_height,
                          UInt8* pun_rgb,
                          const UInt8* pun_hsv);

      void MaskedYUYVtoHSV888_SLOW(size_t un_width,
                        size_t un_height,
                        UInt8* pun_hsv,
                        const UInt8* pun_yuyv, size_t un_center_x, size_t un_center_y, size_t un_radius);

      void MaskedYUYVtoHSV888_Fast_BUG(size_t un_width,
                              size_t un_height,
                              UInt8* pun_hsv,
                              const UInt8* pun_yuyv,
                              size_t un_center_x,
                              size_t un_center_y,
                              size_t un_radius);
      void YUYVtoHSV888_SLOW_NOBUG(size_t un_width,
                              size_t un_height,
                              UInt8* pun_hsv,
                              const UInt8* pun_yuyv);

      void YUYVtoRGB888(int width,
                        int height,
                        unsigned char *src,
                        unsigned char *dst);
      void ARGOS2_YUYVtoHSV888(size_t un_width,
                               size_t un_height,
                               UInt8 *pun_hsv,
                               const UInt8 *pun_yuyv,
                               size_t un_center_x,
                               size_t un_center_y,
                               size_t un_radius);

   }

}

#endif
