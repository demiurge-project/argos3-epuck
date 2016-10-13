#include "image.h"

using namespace argos;

static const size_t IMAGE_WIDTH    = 10;
static const size_t IMAGE_HEIGHT   = 10;
static const size_t RGB_IMAGE_SIZE = 3 * IMAGE_WIDTH * IMAGE_HEIGHT;
static const size_t HSV_IMAGE_SIZE = 3 * IMAGE_WIDTH * IMAGE_HEIGHT;

int main() {
   /* Create image buffers */
   UInt8* punOriginalRGB = new UInt8[RGB_IMAGE_SIZE];
   UInt8* punMiddleHSV   = new UInt8[RGB_IMAGE_SIZE];
   UInt8* punFinalRGB    = new UInt8[RGB_IMAGE_SIZE];
   /* Destroy image buffers */
   delete[] punOriginalRGB;
   delete[] punMiddleHSV;
   delete[] punFinalRGB;
}
