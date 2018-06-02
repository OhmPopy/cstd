// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_BILLBOARD_SCENE_NODE_H_INCLUDED__
#define __I_BILLBOARD_SCENE_NODE_H_INCLUDED__
//#include "ISceneNode.h"
//! A billboard scene node.
/** A billboard is like a 3d sprite: A 2d element,
which always looks to the camera. It is usually used for explosions, fire,
lensflares, particles and things like that.
*/
class IBillboardSceneNode : public ISceneNode
{
public:
  //! Constructor
  IBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      const float3 position = float3(0, 0, 0))
    : ISceneNode(parent, mgr, id, position) {}
  //! Sets the size of the billboard, making it rectangular.
  void setSize(const dimension2df& size) = 0;
  //! Sets the size of the billboard with independent widths of the bottom and top edges.
  /** \param[in] height The height of the billboard.
  \param[in] bottomEdgeWidth The width of the bottom edge of the billboard.
  \param[in] topEdgeWidth The width of the top edge of the billboard.
  */
  void setSize(f32 height, f32 bottomEdgeWidth, f32 topEdgeWidth) = 0;
  //! Returns the size of the billboard.
  /** This will return the width of the bottom edge of the billboard.
  Use getWidths() to retrieve the bottom and top edges independently.
  \return Size of the billboard.
  */
  const dimension2df& getSize() = 0;
  //! Gets the size of the the billboard and handles independent top and bottom edge widths correctly.
  /** \param[out] height The height of the billboard.
  \param[out] bottomEdgeWidth The width of the bottom edge of the billboard.
  \param[out] topEdgeWidth The width of the top edge of the billboard.
  */
  void getSize(f32& height, f32& bottomEdgeWidth, f32& topEdgeWidth) = 0;
  //! Set the color of all vertices of the billboard
  /** \param[in] overallColor Color to set */
  void setColor(const SColor& overallColor) = 0;
  //! Set the color of the top and bottom vertices of the billboard
  /** \param[in] topColor Color to set the top vertices
  \param[in] bottomColor Color to set the bottom vertices */
  void setColor(const SColor& topColor,
      const SColor& bottomColor) = 0;
  //! Gets the color of the top and bottom vertices of the billboard
  /** \param[out] topColor Stores the color of the top vertices
  \param[out] bottomColor Stores the color of the bottom vertices */
  void getColor(SColor& topColor,
      SColor& bottomColor) = 0;
};
#endif

