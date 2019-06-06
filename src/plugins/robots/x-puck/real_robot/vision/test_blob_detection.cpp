#include "blob_detector.h"
#include <cstdio>
#include <cmath>
#include <argos3/core/utility/math/general.h>


using namespace argos;

static size_t IMAGE_WIDTH  = 7;
static size_t IMAGE_HEIGHT = 5;


void PrintImage(UInt8* c_img) {
   fprintf(stdout, "-- IMAGE RGB --\n");
   for(size_t r = 0; r < IMAGE_HEIGHT; ++r) {
      for(size_t c = 0; c < IMAGE_WIDTH; ++c) {
         fprintf(stdout, "  %.2d,%.2d,%.2d",
                 c_img[0],
                 c_img[1],
                 c_img[2]);
          c_img += 3;
      }
      fprintf(stdout, "\n");
   }
   fprintf(stdout, "\n");
}

void PrintImage(const CImage<SHSV888Pixel>& c_img) {
   fprintf(stdout, "-- IMAGE HSV--\n");
   for(size_t r = 0; r < IMAGE_HEIGHT; ++r) {
      for(size_t c = 0; c < IMAGE_WIDTH; ++c) {
         fprintf(stdout, "  %.2d,%.2d,%.2d",
                 c_img(c,r).Hue,
                 c_img(c,r).Saturation,
                 c_img(c,r).Value);
      }
      fprintf(stdout, "\n");
   }
   fprintf(stdout, "\n");
}

void PrintDetectorData(const CBlobDetector& c_detector) {
   const std::vector<CBlobDetector::SDetectionData*>& cDetectorData = c_detector.GetDetectors();
   fprintf(stdout, "-- FILTERED PIXELS --\n");
   for(size_t r = 0; r < IMAGE_HEIGHT; ++r) {
      for(size_t c = 0; c < IMAGE_WIDTH; ++c) {
         fprintf(stdout, "  %d",
                 cDetectorData[0]->DetectedPixels->Get(r * IMAGE_WIDTH + c));
      }
      fprintf(stdout, "\n");
   }
   fprintf(stdout, "\n");
   fprintf(stdout, "-- FIRST PASS LABELS --\n");
   for(size_t r = 0; r < IMAGE_HEIGHT; ++r) {
      for(size_t c = 0; c < IMAGE_WIDTH; ++c) {
         fprintf(stdout, "  %d",
                 cDetectorData[0]->Labels[r * IMAGE_WIDTH + c]);
      }
      fprintf(stdout, "\n");
   }
   fprintf(stdout, "\n");
   fprintf(stdout, "-- BLOB LIST --\n");
   for(std::map<UInt16, CBlobDetector::SBlob>::iterator it = cDetectorData[0]->LabelBlobs.begin();
       it != cDetectorData[0]->LabelBlobs.end();
       ++it) {
      fprintf(stdout, "%d   <%d, %d>, %d\n",
              it->first,
              it->second.CenterI,
              it->second.CenterJ,
              it->second.Area);
   }
   fprintf(stdout, "\n");
}

int main() {
   CImage<SHSV888Pixel> img(IMAGE_WIDTH, IMAGE_HEIGHT);
//   SHSV888Pixel bg(0, 0, 0);
   SHSV888Pixel bg(0, 60, 60);
   for(size_t r = 0; r < IMAGE_HEIGHT; ++r) {
      for(size_t c = 0; c < IMAGE_WIDTH; ++c) {
         img(c,r) = bg;
      }
   }

   PrintImage(img);
   CByteArray  m_cRGBImageData;
   m_cRGBImageData.Resize(IMAGE_WIDTH*IMAGE_HEIGHT);

    HSV888toRGB888(IMAGE_WIDTH,IMAGE_HEIGHT,
                   m_cRGBImageData.ToCArray(), img.GetData().ToCArray());
    PrintImage(m_cRGBImageData.ToCArray());
   PrintImage(img);


   //
   Real fRed,fGreen,fBlue;
   fRed   = 0.882352941f ;                                                     \
   fGreen = 0.823529412f;                                                   \
   fBlue  = 0.117647059f;
   UInt8 value,hue,saturation;\
   Real fValue,fSaturation,fMinRGB,fDiff,fHue;
   fValue = Max(fRed, Max(fGreen, fBlue));                              \
   fMinRGB = Min(fRed, Min(fGreen, fBlue));                             \
   fHue = 0.0f;                                                         \
   fSaturation = 0.0f;                                                  \
   if(fValue > 0.0f) {                                                  \
      fDiff = fValue - fMinRGB;                                         \
      fSaturation = fDiff / fValue;                                     \
      if(fValue == fRed) fHue = 60.0f * (fGreen - fBlue) / fDiff;       \
      if(fValue == fGreen) fHue = 120.0f + 60.0f * (fBlue - fRed) / fDiff; \
      if(fValue == fBlue) fHue = 240.0f + 60.0f * (fRed - fGreen) / fDiff; \
      if(fHue < 0.0f) fHue += 360.0f;                                   \
     hue = fHue / 2.0f;                                       \
     saturation = 255.0f * fSaturation;                              \
     value = 255.0f * fValue;                                   \
     fprintf(stdout,"HSV %3d %3d %3d\n",hue,saturation,value);

     fHue        = hue * 2.0f;
         fSaturation = static_cast<Real>(saturation) / 255.0f;
         fValue      = static_cast<Real>(value) / 255.0f;
         /* Calculate support variables */
         Real fPrimary    = fSaturation * fValue;
         Real fSecondary  = fPrimary * (1.0f - Abs(fmod(fHue / 60.0f, 2.0f) - 1.0f));
         Real fTertiary   = fValue - fPrimary;

         UInt8 converted_r,converted_g,converted_b;
         /* Set RGB values */
         if(fHue < 60.0f) {
           converted_r = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
           converted_g = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
           converted_b = static_cast<UInt8>(255.0f *  fTertiary);
         }
         else if(fHue < 120.0f) {
            converted_r = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
           converted_g = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
           converted_b = static_cast<UInt8>(255.0f *  fTertiary);
         }
         else if(fHue < 180.0f) {
            converted_r = static_cast<UInt8>(255.0f *  fTertiary);
           converted_g = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
           converted_b = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
         }
         else if(fHue < 240.0f) {
            converted_r = static_cast<UInt8>(255.0f *  fTertiary);
            converted_g = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
          converted_b   = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
         }
         else if(fHue < 300.0f) {
            converted_r = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
            converted_g = static_cast<UInt8>(255.0f *  fTertiary);
          converted_b   = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
         }
         else {
            converted_r = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
           converted_g = static_cast<UInt8>(255.0f *  fTertiary);
          converted_b = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
         }

          fprintf(stdout,"RGB %3d %3d %3d\n",converted_r,converted_g,converted_b);
   }





   //
   CBlobDetector bd(IMAGE_WIDTH, IMAGE_HEIGHT, true);
   bd.AddDetector(true,CColor::RED,
                        0,
                        255,
                        CRange<UInt8>(0,255),
                        CRange<UInt8>(0,255),
                        CRange<UInt32>(0,1000));
   bd.Detect(img);
   PrintDetectorData(bd);
   bd.Detect(img);
   PrintDetectorData(bd);
}
