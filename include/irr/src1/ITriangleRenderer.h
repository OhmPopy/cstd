// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_TRIANGLE_RENDERER_H_INCLUDED__
#define __I_TRIANGLE_RENDERER_H_INCLUDED__
//#include "S2DVertex.h"
//#include "IZBuffer.h"
class IImage;
#if 0
enum ETriangleRenderer {
  ETR_FLAT = 0,
  ETR_FLAT_WIRE,
  ETR_GOURAUD,
  ETR_GOURAUD_WIRE,
  ETR_TEXTURE_FLAT,
  ETR_TEXTURE_FLAT_WIRE,
  ETR_TEXTURE_GOURAUD,
  ETR_TEXTURE_GOURAUD_WIRE,
  ETR_TEXTURE_GOURAUD_NOZ,
  ETR_TEXTURE_GOURAUD_ADD,
  ETR_COUNT
};
#else
enum {ETR_COUNT = ETR_TEXTURE_GOURAUD_ADD + 1};
typedef EBurningFFShader ETriangleRenderer;
#endif
class ITriangleRenderer
{
public:
  //! sets a render target
  void setRenderTarget(IImage* surface, const IRECT& viewPort) = 0;
  //! en or disables the backface culling
  void setBackfaceCulling(bool enabled = true) = 0;
  //! sets the Texture
  void setTexture(IImage* texture) = 0;
  //! draws an indexed triangle list
  void drawIndexedTriangleList(S2DVertex* vertices, s32 vertexCount, const u16* indexList, s32 triangleCount) = 0;
};
ITriangleRenderer* createTriangleRendererTextureGouraud(IZBuffer* zbuffer);
ITriangleRenderer* createTriangleRendererTextureGouraudWire(IZBuffer* zbuffer);
ITriangleRenderer* createTriangleRendererGouraud(IZBuffer* zbuffer);
ITriangleRenderer* createTriangleRendererGouraudWire(IZBuffer* zbuffer);
ITriangleRenderer* createTriangleRendererTextureFlat(IZBuffer* zbuffer);
ITriangleRenderer* createTriangleRendererTextureFlatWire(IZBuffer* zbuffer);
ITriangleRenderer* createTriangleRendererFlat(IZBuffer* zbuffer);
ITriangleRenderer* createTriangleRendererFlatWire(IZBuffer* zbuffer);
ITriangleRenderer* createTriangleRendererTextureGouraudNoZ();
ITriangleRenderer* createTriangleRendererTextureGouraudAdd(IZBuffer* zbuffer);
#endif

