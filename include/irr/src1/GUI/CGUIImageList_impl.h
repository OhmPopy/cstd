// This file is part of the "Irrlicht Engine".
// written by Reinhard Ostermeier, reinhard@nospam.r-ostermeier.de
// modified by Thomas Alten
//#include "CGUIImageList.h"
//! constructor
CGUIImageList::CGUIImageList(IVideoDriver* driver)
  :  Driver(driver),
     Texture(0),
     ImageCount(0),
     ImageSize(0, 0),
     ImagesPerRow(0),
     UseAlphaChannel(false)
{
#ifdef _DEBUG
  setDebugName("CGUIImageList");
#endif
  if (Driver) {
    Driver->grab();
  }
}
//! destructor
CGUIImageList::~CGUIImageList()
{
  if (Driver) {
    Driver->drop();
  }
  if (Texture) {
    Texture->drop();
  }
}
//! Creates the image list from texture.
bool CGUIImageList::createImageList(ITexture* texture,
    dimension2d<s32> imageSize,
    bool useAlphaChannel)
{
  if (!texture) {
    return false;
  }
  Texture = texture;
  Texture->grab();
  ImageSize = imageSize;
  ImagesPerRow = Texture->getSize().Width / ImageSize.Width;
  ImageCount = ImagesPerRow * Texture->getSize().Height / ImageSize.Height;
  UseAlphaChannel = useAlphaChannel;
  return true;
}
//! Draws an image and clips it to the specified rectangle if wanted
void CGUIImageList::draw(s32 index, const position2di& destPos,
    const IRECT* clip /*= 0*/)
{
  IRECT sourceRect;
  if (!Driver || index < 0 || index >= ImageCount) {
    return;
  }
  sourceRect.UpperLeftCorner.X = (index % ImagesPerRow) * ImageSize.Width;
  sourceRect.UpperLeftCorner.Y = (index / ImagesPerRow) * ImageSize.Height;
  sourceRect.LowerRightCorner.X = sourceRect.UpperLeftCorner.X + ImageSize.Width;
  sourceRect.LowerRightCorner.Y = sourceRect.UpperLeftCorner.Y + ImageSize.Height;
  Driver->draw2DImage(Texture, destPos, sourceRect, clip,
      SColor(255, 255, 255, 255), UseAlphaChannel);
}

