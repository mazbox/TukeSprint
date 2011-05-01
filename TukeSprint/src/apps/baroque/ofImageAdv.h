
#include "ofImage.h"

class ofImageAdv: public ofImage {
public:
//------------------------------------
void mirror(bool horizontal, bool vertical){
   if(width>0) flipPixels(myPixels, horizontal, vertical);

   update();
}

//------------------------------------
void rotate(float angle){
   rotatePixels(myPixels, angle);

   tex.clear();
   if (bUseTexture == true){
      tex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
   }

   update();
}

void flipPixels(ofPixels &pix, bool horizontal, bool vertical){
   if(!horizontal && !vertical)
      return;

   FIBITMAP * bmp               = getBmpFromPixels(pix);
   bool horSuccess = false, vertSuccess = false;

   if(horizontal)
      horSuccess = FreeImage_FlipHorizontal(bmp);
   if(vertical)
      vertSuccess = FreeImage_FlipVertical(bmp);

   if(horSuccess || vertSuccess)
      putBmpIntoPixels(bmp, pix);

   if (bmp != NULL)            FreeImage_Unload(bmp);
}

//----------------------------------------------------
void rotatePixels(ofPixels &pix, float angle){
   if(angle == 0.0)
      return;

   FIBITMAP * bmp               = getBmpFromPixels(pix);
   FIBITMAP * convertedBmp         = NULL;

   convertedBmp = FreeImage_RotateClassic(bmp, angle);
   putBmpIntoPixels(convertedBmp, pix);

   if (bmp != NULL)            FreeImage_Unload(bmp);
   if (convertedBmp != NULL)      FreeImage_Unload(convertedBmp);
}
};
