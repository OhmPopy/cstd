// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_D3D8_PARALLAX_MAPMATERIAL_RENDERER_H_INCLUDED__
#define __C_D3D8_PARALLAX_MAPMATERIAL_RENDERER_H_INCLUDED__
#ifdef _IRR_WINDOWS_API_
#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
//#include <d3d8.h>
//#include "CD3D8ShaderMaterialRenderer.h"
//#include "IShaderConstantSetCallBack.h"
//! Renderer for parallax maps
class CD3D8ParallaxMapRenderer : public CD3D8ShaderMaterialRenderer, IShaderConstantSetCallBack
{
public:
  CD3D8ParallaxMapRenderer(
      IDirect3DDevice8* d3ddev, IVideoDriver* driver,
      s32& outMaterialTypeNr, IMaterialRenderer* baseMaterial);
  ~CD3D8ParallaxMapRenderer();
  //! Called by the engine when the vertex and/or pixel shader constants for an
  //! material renderer should be set.
  void OnSetConstants(IMaterialRendererServices* services, s32 userData);
  bool OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype);
  void OnSetMaterial(const SMaterial& material) { }
  void OnSetMaterial(const SMaterial& material,
      const SMaterial& lastMaterial,
      bool resetAllRenderstates, IMaterialRendererServices* services);
  //! Returns the render capability of the material.
  s32 getRenderCapability() const;
private:
  //! stores if this shader compiled the shaders and is
  //! allowed to delete them again. D3D8 lacks reference counting
  //! support for shaders.
  bool CompiledShaders;
  f32 CurrentScale;
};
#endif
#endif
#endif

