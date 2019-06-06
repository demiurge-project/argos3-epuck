#include "image.h"

#include <argos3/core/utility/math/general.h>

#include <cmath>
#include <cstdio>

namespace argos {

/****************************************/
/****************************************/

void HSV888toRGB888(size_t un_width,
                    size_t un_height,
                    UInt8* pun_rgb,
                    const UInt8* pun_hsv) {
    /* HSV variables to help conversion */
    static Real fHue, fSaturation, fValue;
    /* Other variables to help conversion */
    static Real fPrimary, fSecondary, fTertiary;
    for(size_t j = 0; j < un_height; ++j) {
        for(size_t i = 0; i < un_width; ++i) {
            /* Normalize hue in [0,360], saturation in [0,1], and value in [0,1] */
            //fHue        = pun_hsv[0] * 2.0f;
            fHue        = pun_hsv[0] * 2.0f;
            fSaturation = static_cast<Real>(pun_hsv[1]) / 255.0f;
            fValue      = static_cast<Real>(pun_hsv[2]) / 255.0f;
            /* Calculate support variables */
            fPrimary    = fSaturation * fValue;
            fSecondary  = fPrimary * (1.0f - Abs(fmod(fHue / 60.0f, 2.0f) - 1.0f));
            fTertiary   = fValue - fPrimary;
            /* Set RGB values */
            if(fHue < 60.0f) {
                pun_rgb[0] = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
                pun_rgb[1] = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
                pun_rgb[2] = static_cast<UInt8>(255.0f *  fTertiary);
            }
            else if(fHue < 120.0f) {
                pun_rgb[0] = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
                pun_rgb[1] = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
                pun_rgb[2] = static_cast<UInt8>(255.0f *  fTertiary);
            }
            else if(fHue < 180.0f) {
                pun_rgb[0] = static_cast<UInt8>(255.0f *  fTertiary);
                pun_rgb[1] = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
                pun_rgb[2] = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
            }
            else if(fHue < 240.0f) {
                pun_rgb[0] = static_cast<UInt8>(255.0f *  fTertiary);
                pun_rgb[1] = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
                pun_rgb[2] = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
            }
            else if(fHue < 300.0f) {
                pun_rgb[0] = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
                pun_rgb[1] = static_cast<UInt8>(255.0f *  fTertiary);
                pun_rgb[2] = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
            }
            else {
                pun_rgb[0] = static_cast<UInt8>(255.0f * (fTertiary + fPrimary));
                pun_rgb[1] = static_cast<UInt8>(255.0f *  fTertiary);
                pun_rgb[2] = static_cast<UInt8>(255.0f * (fTertiary + fSecondary));
            }
            /* Go to next pixel */
            pun_hsv += 3;
            pun_rgb += 3;
        }
    }
}

/****************************************/
/****************************************/

#define CLIP(x) ( (x)>=0xFF ? 0xFF : ( (x) <= 0x00 ? 0x00 : (x) ) )

void YUYVtoRGB888(int width,
                  int height,
                  unsigned char *src,
                  unsigned char *dst) {
    int line, column;
    unsigned char *py, *pu, *pv;
    unsigned char *tmp = dst;
    /* In this format each four bytes is two pixels. Each four bytes is two Y's, a Cb and a Cr.
         Each Y goes to one of the pixels, and the Cb and Cr belong to both pixels. */
    py = src;
    pu = src + 1;
    pv = src + 3;
    for ( line = 0; line < height; ++line ) {
        for ( column = 0; column < width; ++column ) {
            *tmp++ = CLIP ( *py + ( *pu-128 ) + ( ( *pu-128 ) >> 1 ) + ( ( *pu-128 ) >> 2 ) + ( ( *pu-128 ) >> 6 ) );
            *tmp++ = CLIP ( *py - ( ( ( *pu-128 ) >> 2 ) + ( ( *pu-128 ) >> 4 ) + ( ( *pu-128 ) >> 5 ) ) - ( ( ( *pv-128 ) >> 1 ) + ( ( *pv-128 ) >> 3 ) + ( ( *pv-128 ) >> 4 ) + ( ( *pv-128 ) >> 5 ) ) );
            *tmp++ = CLIP ( *py + ( *pv-128 ) + ( ( *pv-128 ) >> 2 ) + ( ( *pv-128 ) >> 3 ) + ( ( *pv-128 ) >> 5 ) );
            // increase py every time
            py += 2;
            // increase pu, pv every second time
            if ( ( column & 1 ) ==1 ) {
                pu += 4;
                pv += 4;
            }
        }
    }
}

/****************************************/
/****************************************/


void ARGOS2_YUYVtoHSV888(size_t un_width,
                         size_t un_height,
                         UInt8 *pun_hsv,
                         const UInt8 *pun_yuyv,
                         size_t un_center_x,
                         size_t un_center_y,
                         size_t un_radius) {
    size_t line, column;
    const UInt8 *py, *pu, *pv;
    UInt8 *tmp = pun_hsv;
    /* In this format each four bytes is two pixels. Each four bytes is two Y's, a Cb and a Cr.
         Each Y goes to one of the pixels, and the Cb and Cr belong to both pixels. */
    py = pun_yuyv;
    pu = pun_yuyv + 1;
    pv = pun_yuyv + 3;
    UInt8 r,g,b;
    SInt32 h;
    UInt8 s;
    UInt8 v;
    UInt8 max,min,delta;
    UInt32 unSquareRadius = un_radius * un_radius;
    for ( line = 0; line < un_height; ++line ) {
        for ( column = 0; column < un_width; ++column ) {
            if(Square(column - un_center_x) + Square(line - un_center_y) < unSquareRadius) {
                b = CLIP ( *py + ( *pu-128 ) + ( ( *pu-128 ) >> 1 ) + ( ( *pu-128 ) >> 2 ) + ( ( *pu-128 ) >> 6 ) );
                g = CLIP ( *py - ( ( ( *pu-128 ) >> 2 ) + ( ( *pu-128 ) >> 4 ) + ( ( *pu-128 ) >> 5 ) ) - ( ( ( *pv-128 ) >> 1 ) + ( ( *pv-128 ) >> 3 ) + ( ( *pv-128 ) >> 4 ) + ( ( *pv-128 ) >> 5 ) ) );
                r = CLIP ( *py + ( *pv-128 ) + ( ( *pv-128 ) >> 2 ) + ( ( *pv-128 ) >> 3 ) + ( ( *pv-128 ) >> 5 ) );
                //RGB--> HSV
                if(r<g)min=r; else min=g;
                if(b<min)min=b;
                if(r>g)max=r; else max=g;
                if(b>max)max=b;

                v = max;                // v, 0..255

                delta = max - min;                      // 0..255, < v

                if( max != min ){
                    s = (delta)*255 / max;        // s, 0..255

                    if( r == max ){
                        h = (g - b)*60/delta;        // between yellow & magenta

                    }else if( g == max )
                        h = 120 + (b - r)*60/delta;    // between cyan & yellow
                    else{
                        h = 240 + (r - g)*60/delta;    // between magenta & cyan
                    }
                    if( h < 0 ){
                        h += 360;
                    }
                }else {
                    // r = g = b = 0        // s = 0, v is undefined
                    s = 0;
                    h = 0;

                }

                *tmp++=(UInt8)(h/2);
                *tmp++=s;
                *tmp++=v;
                // increase py every time
                py += 2;
                // increase pu, pv every second time
                if ( ( column & 1 ) ==1 ) {
                    pu += 4;
                    pv += 4;
                }
            }else{
                *tmp++=0;
                *tmp++=0;
                *tmp++=0;
                py += 2;
                // increase pu, pv every second time
                if ( ( column & 1 ) ==1 ) {
                    pu += 4;
                    pv += 4;
                }
            }
        }
    }
}

/****************************************/
/****************************************/

#define UNITCLAMP(v) v = v > 1.0f ? 1.0f : (v < 0.0f ? 0.0f : v);

/*
    * Used formulas at:
    * http://en.wikipedia.org/wiki/YUV#Y.27UV444_to_RGB888_conversion
    * http://docs.opencv.org/modules/imgproc/doc/miscellaneous_transformations.html#cvtcolor
    */
#define YUYV_TO_HSV(Y_IDX)                                                 \
    fRed   = (pYUYVBlock[Y_IDX] + 1.403f * (pYUYVBlock[3] - 128.0f)) / 255.0f; \
    UNITCLAMP(fRed);                                                     \
    fGreen = (pYUYVBlock[Y_IDX] - 0.714f * (pYUYVBlock[3] - 128.0f) - 0.344f * (pYUYVBlock[1] - 128.0f)) / 255.0f; \
    UNITCLAMP(fGreen);                                                   \
    fBlue  = (pYUYVBlock[Y_IDX] + 1.773f * (pYUYVBlock[1] - 128.0f)) / 255.0f; \
    UNITCLAMP(fBlue);                                                    \
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
    pHSVBlock[0] = fHue / 2.0f;                                       \
    pHSVBlock[1] = 255.0f * fSaturation;                              \
    pHSVBlock[2] = 255.0f * fValue;                                   \
}

/****************************************/
/****************************************/

void MaskedYUYVtoHSV888_SLOW(size_t un_width,
                                   size_t un_height,
                                   UInt8* pun_hsv,
                                   const UInt8* pun_yuyv,
                                   size_t un_center_x,
                                   size_t un_center_y,
                                   size_t un_radius) {
    /* RGB variables to help conversion */
    static Real fRed, fGreen, fBlue;
    /* HSV variables to help conversion */
    static Real fHue, fSaturation, fValue, fMinRGB, fDiff;
    /*
       * Pointer to each YUYV block
       * pYUYVBlock[0] -> Y0
       * pYUYVBlock[1] -> Cb
       * pYUYVBlock[2] -> Y1
       * pYUYVBlock[3] -> Cr
       */
    const UInt8* pYUYVBlock = pun_yuyv;
    /*
       * Pointer to each HSV block
       * pHSVBlock[0] = hue
       * pHSVBlock[1] = saturation
       * pHSVBlock[2] = value
       */
    UInt8* pHSVBlock = pun_hsv;
    UInt32 unSquareRadius = un_radius * un_radius;
    for(size_t j = 0; j < un_height; ++j) {
        for(size_t i = 0; i < un_width / 2; ++i) {
            if(Square(i+i - un_center_x) + Square(j - un_center_y) < unSquareRadius) {
                /* First pixel in the YUYV block -> RGB */
                YUYV_TO_HSV(0);
                /* Go to the next HSV block */
                pHSVBlock += 3;
                /* Second pixel in the YUYV block -> RGB */
                YUYV_TO_HSV(2);
                /* Go to the next HSV block */
                pHSVBlock += 3;
                /* Go to next YUYV block */
                pYUYVBlock += 4;
            }else{
                pHSVBlock[0] = 0;
                pHSVBlock[1] = 0;
                pHSVBlock[2] = 0;
                pHSVBlock[3] = 0;
                pHSVBlock[4] = 0;
                pHSVBlock[5] = 0;
                /* Go to the next blocks */
                pHSVBlock += 6;
                pYUYVBlock += 4;
            }
        }
    }
}

/****************************************/
/****************************************/

void YUYVtoHSV888_SLOW_NOBUG(size_t un_width,
                             size_t un_height,
                             UInt8* pun_hsv,
                             const UInt8* pun_yuyv) {
    /* RGB variables to help conversion */
    static Real fRed, fGreen, fBlue;
    /* HSV variables to help conversion */
    static Real fHue, fSaturation, fValue, fMinRGB, fDiff;
    /*
       * Pointer to each YUYV block
       * pYUYVBlock[0] -> Y0
       * pYUYVBlock[1] -> Cb
       * pYUYVBlock[2] -> Y1
       * pYUYVBlock[3] -> Cr
       */
    const UInt8* pYUYVBlock = pun_yuyv;
    /*
       * Pointer to each HSV block
       * pHSVBlock[0] = hue
       * pHSVBlock[1] = saturation
       * pHSVBlock[2] = value
       */
    UInt8* pHSVBlock = pun_hsv;
    for(size_t j = 0; j < un_height; ++j) {
        for(size_t i = 0; i < un_width / 2; ++i) {

            /* First pixel in the YUYV block -> RGB */
            YUYV_TO_HSV(0);
            /* Go to the next HSV block */
            pHSVBlock += 3;
            /* Second pixel in the YUYV block -> RGB */
            YUYV_TO_HSV(2);
            /* Go to the next HSV block */
            pHSVBlock += 3;
            /* Go to next YUYV block */
            pYUYVBlock += 4;
        }
    }
}

/****************************************/
/****************************************/

#define BYTE_CLAMP(v) v = (v > 255) ? 255 : ((v < 0) ? 0 : v);

#define FAST_YUYV_TO_HSV(Y_IDX)                                         \
    unY = pYUYVBlock[Y_IDX];                                             \
    unRed = unY + unCr + (unCr >> 2) + (unCr >> 3) + (unCr >> 5);        \
    BYTE_CLAMP(unRed);                                                     \
    unGreen = unY - ((unCb >> 2) + (unCb >> 4) + (unCb >> 5)) - ((unCr >> 1) + (unCr >> 3) + (unCr >> 4) + (unCr >> 5)); \
    BYTE_CLAMP(unGreen);                                                   \
    unBlue = unY + unCb + (unCb >> 1) + (unCb >> 2) + (unCb >> 6);       \
    BYTE_CLAMP(unBlue);                                                    \
    pHSVBlock[2] = (unRed > unGreen) ? ((unBlue > unRed) ? unBlue : unRed) : ((unBlue > unGreen) ? unBlue : unGreen); \
    if(pHSVBlock[2] == 0) {                                                  \
    pHSVBlock[0] = 0;                                                 \
    pHSVBlock[1] = 0;                                                 \
}                                                                    \
    else {                                                               \
    unMinRGB = (unRed < unGreen) ? ((unBlue < unRed) ? unBlue : unRed) : ((unBlue < unGreen) ? unBlue : unGreen); \
    unDiff = pHSVBlock[2] - unMinRGB;                                     \
    if(unDiff == 0) {                                                 \
    pHSVBlock[0] = 0;                                              \
    pHSVBlock[1] = 0;                                              \
}                                                                 \
    else {                                                            \
    pHSVBlock[1] = 255 * unDiff / pHSVBlock[2];                    \
    BYTE_CLAMP(pHSVBlock[1]);                                      \
    if(pHSVBlock[2] == unRed)                                      \
    /*pHSVBlock[0] = 43 * (unGreen - unBlue) / unDiff; */           \
    unHue = 60 * (unGreen - unBlue) / unDiff;            \
    else if(pHSVBlock[2] == unGreen){                               \
    /*pHSVBlock[0] = 85 + 43 * (unBlue - unRed) / unDiff;*/         \
    unHue = 120 + 60 * (unBlue - unRed) / unDiff;         \
}else{                                                           \
     /*pHSVBlock[0] = 171 + 43 * (unRed - unGreen) / unDiff;*/       \
   unHue = 240 + 60 * (unRed - unGreen) / unDiff;              \
}                                                              \
   /* BYTE_CLAMP(pHSVBlock[0]);*/                                      \
    if( unHue < 0 ){               \
      unHue += 360;                                      \
    }                                                                 \
     unHue/=2;                                \
     pHSVBlock[0]=unHue;\
} \
} \

void MaskedYUYVtoHSV888_Fast_BUG(size_t un_width,
                                 size_t un_height,
                                 UInt8* pun_hsv,
                                 const UInt8* pun_yuyv,
                                 size_t un_center_x,
                                 size_t un_center_y,
                                 size_t un_radius) {

    /* YUYV variables to help conversion */
    static SInt32 unY, unCr, unCb;
    /* RGB variables to help conversion */
    static SInt32 unRed, unGreen, unBlue;
    /* HSV variables to help conversion */
    static SInt32 unMinRGB, unDiff,unHue;
    /*
       * Pointer to each YUYV block
       * pYUYVBlock[0] -> Y0
       * pYUYVBlock[1] -> Cb
       * pYUYVBlock[2] -> Y1
       * pYUYVBlock[3] -> Cr
       */
    const UInt8* pYUYVBlock = pun_yuyv;
    /*
       * Pointer to each HSV block
       * pHSVBlock[0] = hue
       * pHSVBlock[1] = saturation
       * pHSVBlock[2] = value
       */
    UInt8* pHSVBlock = pun_hsv;
    UInt32 unSquareRadius = un_radius * un_radius;
    for(size_t j = 0; j < un_height; ++j) {
        for(size_t i = 0; i < un_width / 2; ++i) {
            if(Square(i+i - un_center_x) + Square(j - un_center_y) < unSquareRadius) {
                /* Fix Cr and Cb */
                unCr = pYUYVBlock[1] - 128;
                unCb = pYUYVBlock[3] - 128;
                /* First pixel in the YUYV block -> RGB */
                FAST_YUYV_TO_HSV(0);
                /* Go to the next HSV block */
                pHSVBlock += 3;
                /* Second pixel in the YUYV block -> RGB */
                FAST_YUYV_TO_HSV(2);
                /* Go to the next HSV block */
                pHSVBlock += 3;
                /* Go to next YUYV block */
                pYUYVBlock += 4;
            }
            else {
                pHSVBlock[0] = 0;
                pHSVBlock[1] = 0;
                pHSVBlock[2] = 0;
                pHSVBlock[3] = 0;
                pHSVBlock[4] = 0;
                pHSVBlock[5] = 0;
                /* Go to the next blocks */
                pHSVBlock += 6;
                pYUYVBlock += 4;
            }
        }
    }
}

/****************************************/
/****************************************/

}
