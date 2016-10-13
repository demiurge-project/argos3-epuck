#ifndef BLOB_DETECTOR_H
#define BLOB_DETECTOR_H

#include <argos3/plugins/robots/e-puck/real_robot/vision/extended_configurable_resource.h>
#include <argos3/plugins/robots/e-puck/real_robot/vision/image.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/core/utility/math/range.h>

#include <vector>
#include <map>

namespace argos {

   class CBlobDetector : public CExtendedConfigurableResource {

   public:

      /**
       * A simple bit set.
       * Internally, the bits are stored in an array of bytes.
       * The bits are in little endian (least significant bit first) order.
       */
      class CBitSet {

      public:

         /**
          * Class constructor.
          * @param un_size How many bits to store.
          */
         CBitSet(size_t un_size);

         /**
          * Class destructor.
          */
         ~CBitSet();

         /**
          * Conversion operator.
          * Used to stream the content of this bit set onto the network.
          */
         operator UInt8*() {
            return m_punBuffer;
         }

         /**
          * Returns the bit at the given index.
          * @param un_idx The index of the wanted bit.
          * @return The bit at the given index.
          */
         inline bool Get(size_t un_idx) const {
            return (m_punBuffer[un_idx / 8] & (1 << (un_idx % 8)));
         }

         /**
          * Returns the bit at the given index.
          * @param un_idx The index of the wanted bit.
          * @param b_value The value to set.
          * @return The bit at the given index.
          */
         inline void Set(size_t un_idx,
                         bool b_value) {
            m_punBuffer[un_idx / 8] |= (b_value << (un_idx % 8));
         }

         /**
          * Sets all bits to zero.
          */
         void Reset();

         /**
          * Returns the number of bits in the set.
          * @returns The number of bits in the set.
          */
         inline size_t Size() {
            return m_unSize;
         }

         /**
          * Returns the number of bytes in the set storage.
          * @returns The number of bytes in the set storage.
          */
         inline size_t StorageSize() {
            return m_unStorageSize;
         }

      private:

         /** Number of bits in the structure */
         size_t m_unSize;

         /** Number of bytes used to store the bits */
         size_t m_unStorageSize;

         /** Buffer that stores the bits */
         UInt8* m_punBuffer;

      };

      struct SBlob {
         UInt32 CenterI;
         UInt32 CenterJ;
         UInt32 Area;

         SBlob();
      };

      struct SDetectionData {
          /**
          * For a description of the HSV format go http://en.wikipedia.org/wiki/HSL_and_HSV .
          */
         bool Enabled;
         CColor Color;
         /**
          * Why @brief HueMin and @brief HueMax are not wrapped as a CRange [HueMin,HueMax] ?
          * Because, since Hue is an angle, HueMin can be greater than HueMax (e.g., to represent
          * the range [300°,10°]). Note that CRange forbids min>max.
         **/
         UInt8 HueMin;
         UInt8 HueMax;
         CRange<UInt8> SaturationRange;
         CRange<UInt8> ValueRange;
         CRange<UInt32> AreaRange;
         size_t ImageSize;
         UInt16* Labels;
         UInt16 LabelCounter;
         UInt16 LabelParentListSize;
         UInt16* LabelParentList;
         CBitSet* DetectedPixels;
         std::map<UInt16, SBlob> LabelBlobs;

         SDetectionData(bool Enabled,const CColor& c_color,
                        UInt8 c_hue_min, UInt8 c_hue_max,
                        const CRange<UInt8>& c_saturation_range,
                        const CRange<UInt8>& c_value_range,
                        const CRange<UInt32>& c_area_range,
                        UInt32 un_image_width,
                        UInt32 un_image_height,
                        bool b_create_phase_images);

         ~SDetectionData();

         void Reset();

         void NewLabel();
         inline bool isPixelAccepted(SHSV888Pixel& sPixel);


      };

   public:

      CBlobDetector(UInt32 un_image_width,
                    UInt32 un_image_height,
                    bool b_create_phase_images = false);
      ~CBlobDetector();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset() {}
      virtual void Destroy() {}

      virtual void SaveXMLConfiguration(const std::string& str_path);

      void Detect(const CImage<SHSV888Pixel>& c_image);

      void AddDetector(bool c_enabled, const CColor& c_color,
                             const UInt8 c_hue_min,
                             const UInt8 c_hue_max,
                             const CRange<UInt8>& c_saturation_range,
                             const CRange<UInt8>& c_value_range,
                             const CRange<UInt32>& c_area_range);

      inline const std::vector<SDetectionData*>& GetDetectors() const {
         return m_vecDetectors;
      }

      inline std::vector<SDetectionData*>& GetDetectors() {
         return m_vecDetectors;
      }

   private:

      UInt32 m_unImageWidth;
      UInt32 m_unImageHeight;
      bool m_bCreatePhaseImages;
      std::vector<SDetectionData*> m_vecDetectors;
   public:
      static const size_t u_color_number;
      static const CColor colors[];
   };

}

#endif
