// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_TRIANGLE_SELECTOR_H_INCLUDED__
#define __I_TRIANGLE_SELECTOR_H_INCLUDED__
//#include "triangle3d.h"
//#include "line3d.h"
class ISceneNode;
//! Interface to return triangles with specific properties.
/** Every ISceneNode may have a triangle selector, available with
ISceneNode::getTriangleScelector() or ISceneManager::createTriangleSelector.
This is used for doing collision detection: For example if you know, that a
collision may have happened in the area between (1,1,1) and (10,10,10), you
can get all triangles of the scene node in this area with the
ITriangleSelector easily and check every triangle if it collided. */
class ITriangleSelector
{
public:
  //! Get amount of all available triangles in this selector
  s32 getTriangleCount() = 0;
  //! Gets the triangles for one associated node.
  /**
  This returns all triangles for one scene node associated with this
  selector.  If there is more than one scene node associated (e.g. for
  an IMetaTriangleSelector) this this function may be called multiple
  times to retrieve all triangles.
  \param triangles Array where the resulting triangles will be
  written to.
  \param arraySize Size of the target array.
  \param outTriangleCount: Amount of triangles which have been written
  into the array.
  \param transform Pointer to matrix for transforming the triangles
  before they are returned. Useful for example to scale all triangles
  down into an ellipsoid space. If this pointer is null, no
  transformation will be done. */
  void getTriangles(triangle3df* triangles, s32 arraySize,
      s32& outTriangleCount, const matrix4* transform = 0) = 0;
  //! Gets the triangles for one associated node which may lie within a specific bounding box.
  /**
  This returns all triangles for one scene node associated with this
  selector.  If there is more than one scene node associated (e.g. for
  an IMetaTriangleSelector) this this function may be called multiple
  times to retrieve all triangles.
  This method will return at least the triangles that intersect the box,
  but may return other triangles as well.
  \param triangles Array where the resulting triangles will be written
  to.
  \param arraySize Size of the target array.
  \param outTriangleCount Amount of triangles which have been written
  into the array.
  \param box Only triangles which are in this axis aligned bounding box
  will be written into the array.
  \param transform Pointer to matrix for transforming the triangles
  before they are returned. Useful for example to scale all triangles
  down into an ellipsoid space. If this pointer is null, no
  transformation will be done. */
  void getTriangles(triangle3df* triangles, s32 arraySize,
      s32& outTriangleCount, const F3AABBOX& box,
      const matrix4* transform = 0) = 0;
  //! Gets the triangles for one associated node which have or may have contact with a 3d line.
  /**
  This returns all triangles for one scene node associated with this
  selector.  If there is more than one scene node associated (e.g. for
  an IMetaTriangleSelector) this this function may be called multiple
  times to retrieve all triangles.
  Please note that unoptimized triangle selectors also may return
  triangles which are not in contact at all with the 3d line.
  \param triangles Array where the resulting triangles will be written
  to.
  \param arraySize Size of the target array.
  \param outTriangleCount Amount of triangles which have been written
  into the array.
  \param line Only triangles which may be in contact with this 3d line
  will be written into the array.
  \param transform Pointer to matrix for transforming the triangles
  before they are returned. Useful for example to scale all triangles
  down into an ellipsoid space. If this pointer is null, no
  transformation will be done. */
  void getTriangles(triangle3df* triangles, s32 arraySize,
      s32& outTriangleCount, const F3LINE& line,
      const matrix4* transform = 0) = 0;
  //! Get scene node associated with a given triangle.
  /**
  This allows to find which scene node (potentially of several) is
  associated with a specific triangle.
  \param triangleIndex: the index of the triangle for which you want to find
  the associated scene node.
  \return The scene node associated with that triangle.
  */
  ISceneNode* getSceneNodeForTriangle(u32 triangleIndex) = 0;
  //! Get number of TriangleSelectors that are part of this one
  /** Only useful for MetaTriangleSelector, others return 1
  */
  u32 getSelectorCount() = 0;
  //! Get TriangleSelector based on index based on getSelectorCount
  /** Only useful for MetaTriangleSelector, others return 'this' or 0
  */
  ITriangleSelector* getSelector(u32 index) = 0;
  //! Get TriangleSelector based on index based on getSelectorCount
  /** Only useful for MetaTriangleSelector, others return 'this' or 0
  */
  const ITriangleSelector* getSelector(u32 index) = 0;
};
#endif

