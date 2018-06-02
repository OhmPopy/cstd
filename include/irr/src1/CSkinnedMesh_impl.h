// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifdef _IRR_COMPILE_WITH_SKINNED_MESH_SUPPORT_
//#include "CSkinnedMesh.h"
//#include "CBoneSceneNode.h"
//#include "IAnimatedMeshSceneNode.h"
//#include "os.h"
//! constructor
CSkinnedMesh::CSkinnedMesh()
  : SkinningBuffers(0), AnimationFrames(0.f), FramesPerSecond(25.f),
    LastAnimatedFrame(-1), SkinnedLastFrame(false),
    InterpolationMode(EIM_LINEAR),
    HasAnimation(false), PreparedForSkinning(false),
    AnimateNormals(true), HardwareSkinning(false)
{
#ifdef _DEBUG
  setDebugName("CSkinnedMesh");
#endif
  SkinningBuffers = &LocalBuffers;
}
//! destructor
CSkinnedMesh::~CSkinnedMesh()
{
  int i;
  for (i = 0; i < AllJoints_size; ++i) {
    delete AllJoints[i];
  }
  for (int j = 0; j < LocalBuffers_size; ++j) {
    if (LocalBuffers[j]) {
      LocalBuffers[j]->drop();
    }
  }
}
//! returns the amount of frames in milliseconds.
//! If the amount is 1, it is a static (=non animated) mesh.
u32 CSkinnedMesh::getFrameCount() const
{
  return floor32(AnimationFrames);
}
//! Gets the default animation speed of the animated mesh.
/** \return Amount of frames per second. If the amount is 0, it is a static, non animated mesh. */
f32 CSkinnedMesh::getAnimationSpeed() const
{
  return FramesPerSecond;
}
//! Gets the frame count of the animated mesh.
/** \param fps Frames per second to play the animation with. If the amount is 0, it is not animated.
The actual speed is set in the scene node the mesh is instantiated in.*/
void CSkinnedMesh::setAnimationSpeed(f32 fps)
{
  FramesPerSecond = fps;
}
//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
IMesh* CSkinnedMesh::getMesh(s32 frame, s32 detailLevel, s32 startFrameLoop, s32 endFrameLoop)
{
  //animate(frame,startFrameLoop, endFrameLoop);
  if (frame == -1) {
    return this;
  }
  animateMesh((f32)frame, 1.0f);
  skinMesh();
  return this;
}
//--------------------------------------------------------------------------
//      Keyframe Animation
//--------------------------------------------------------------------------
//! Animates this mesh's joints based on frame input
//! blend: {0-old position, 1-New position}
void CSkinnedMesh::animateMesh(f32 frame, f32 blend)
{
  if (!HasAnimation || LastAnimatedFrame == frame) {
    return;
  }
  LastAnimatedFrame = frame;
  SkinnedLastFrame = false;
  if (blend <= 0.f) {
    return;  //No need to animate
  }
  int i;
  for (i = 0; i < AllJoints_size; ++i) {
    //The joints can be animated here with no input from their
    //parents, but for setAnimationMode extra checks are needed
    //to their parents
    SJoint* joint = AllJoints[i];
    const float3 oldPosition = joint->Animatedposition;
    const float3 oldScale = joint->Animatedscale;
    const quaternion oldRotation = joint->Animatedrotation;
    float3 position = oldPosition;
    float3 scale = oldScale;
    quaternion rotation = oldRotation;
    getFrameData(frame, joint,
        position, joint->positionHint,
        scale, joint->scaleHint,
        rotation, joint->rotationHint);
    if (blend == 1.0f) {
      //No blending needed
      joint->Animatedposition = position;
      joint->Animatedscale = scale;
      joint->Animatedrotation = rotation;
    }
    else {
      //Blend animation
      joint->Animatedposition = lerp(oldPosition, position, blend);
      joint->Animatedscale = lerp(oldScale, scale, blend);
      joint->Animatedrotation.slerp(oldRotation, rotation, blend);
    }
  }
  //Note:
  //LocalAnimatedMatrix needs to be built at some point, but this function may be called lots of times for
  //one render (to play two animations at the same time) LocalAnimatedMatrix only needs to be built once.
  //a call to buildAllLocalAnimatedMatrices is needed before skinning the mesh, and before the user gets the joints to move
  //----------------
  // Temp!
  buildAllLocalAnimatedMatrices();
  //-----------------
  updateBoundingBox();
}
void CSkinnedMesh::buildAllLocalAnimatedMatrices()
{
  int i;
  for (i = 0; i < AllJoints_size; ++i) {
    SJoint* joint = AllJoints[i];
    //Could be faster:
    if (joint->UseAnimationFrom &&
        (joint->UseAnimationFrom->PositionKeys_size ||
            joint->UseAnimationFrom->ScaleKeys_size ||
            joint->UseAnimationFrom->RotationKeys_size)) {
      joint->GlobalSkinningSpace = false;
      // IRR_TEST_BROKEN_QUATERNION_USE: TODO - switched to getMatrix_transposed instead of getMatrix for downward compatibility.
      //                   Not tested so far if this was correct or wrong before quaternion fix!
      joint->Animatedrotation.getMatrix_transposed(joint->LocalAnimatedMatrix);
      // --- joint->LocalAnimatedMatrix *= joint->Animatedrotation.getMatrix() ---
      f32* m1 = joint->LocalAnimatedMatrix;
      float3 Pos = joint->Animatedposition;
      m1[0] += Pos.X * m1[3];
      m1[1] += Pos.Y * m1[3];
      m1[2] += Pos.Z * m1[3];
      m1[4] += Pos.X * m1[7];
      m1[5] += Pos.Y * m1[7];
      m1[6] += Pos.Z * m1[7];
      m1[8] += Pos.X * m1[11];
      m1[9] += Pos.Y * m1[11];
      m1[10] += Pos.Z * m1[11];
      m1[12] += Pos.X * m1[15];
      m1[13] += Pos.Y * m1[15];
      m1[14] += Pos.Z * m1[15];
      // -----------------------------------
      if (joint->ScaleKeys_size) {
        /*
        matrix4 scaleMatrix;
        scaleMatrix.setScale(joint->Animatedscale);
        joint->LocalAnimatedMatrix *= scaleMatrix;
        */
        // -------- joint->LocalAnimatedMatrix *= scaleMatrix -----------------
        matrix4& mat = joint->LocalAnimatedMatrix;
        mat[0] *= joint->Animatedscale.X;
        mat[1] *= joint->Animatedscale.X;
        mat[2] *= joint->Animatedscale.X;
        mat[3] *= joint->Animatedscale.X;
        mat[4] *= joint->Animatedscale.Y;
        mat[5] *= joint->Animatedscale.Y;
        mat[6] *= joint->Animatedscale.Y;
        mat[7] *= joint->Animatedscale.Y;
        mat[8] *= joint->Animatedscale.Z;
        mat[9] *= joint->Animatedscale.Z;
        mat[10] *= joint->Animatedscale.Z;
        mat[11] *= joint->Animatedscale.Z;
        // -----------------------------------
      }
    }
    else {
      joint->LocalAnimatedMatrix = joint->LocalMatrix;
    }
  }
  SkinnedLastFrame = false;
}
void CSkinnedMesh::buildAllGlobalAnimatedMatrices(SJoint* joint, SJoint* parentJoint)
{
  if (!joint) {
    int i;
    for (i = 0; i < RootJoints_size; ++i) {
      buildAllGlobalAnimatedMatrices(RootJoints[i], 0);
    }
    return;
  }
  else {
    // Find global matrix...
    if (!parentJoint || joint->GlobalSkinningSpace) {
      joint->GlobalAnimatedMatrix = joint->LocalAnimatedMatrix;
    }
    else {
      joint->GlobalAnimatedMatrix = parentJoint->GlobalAnimatedMatrix * joint->LocalAnimatedMatrix;
    }
  }
  for (int j = 0; j < joint->Children_size; ++j) {
    buildAllGlobalAnimatedMatrices(joint->Children[j], joint);
  }
}
void CSkinnedMesh::getFrameData(f32 frame, SJoint* joint,
    float3 position, s32& positionHint,
    float3 scale, s32& scaleHint,
    quaternion& rotation, s32& rotationHint)
{
  s32 foundPositionIndex = -1;
  s32 foundScaleIndex = -1;
  s32 foundRotationIndex = -1;
  if (joint->UseAnimationFrom) {
    const SPositionKey* PositionKeys = joint->UseAnimationFrom->PositionKeys;
    const SScaleKey* ScaleKeys = joint->UseAnimationFrom->ScaleKeys;
    const SRotationKey > & RotationKeys = joint->UseAnimationFrom->RotationKeys;
    if (PositionKeys_size) {
      foundPositionIndex = -1;
      //Test the Hints...
      if (positionHint >= 0 && (u32)positionHint < PositionKeys_size) {
        //check this hint
        if (positionHint > 0 && PositionKeys[positionHint].frame >= frame && PositionKeys[positionHint - 1].frame < frame) {
          foundPositionIndex = positionHint;
        }
        else if (positionHint + 1 < (s32)PositionKeys_size) {
          //check the next index
          if (PositionKeys[positionHint + 1].frame >= frame &&
              PositionKeys[positionHint + 0].frame < frame) {
            positionHint++;
            foundPositionIndex = positionHint;
          }
        }
      }
      //The hint test failed, do a full scan...
      if (foundPositionIndex == -1) {
        int i;
        for (i = 0; i < PositionKeys_size; ++i) {
          if (PositionKeys[i].frame >= frame) { //Keys should to be sorted by frame
            foundPositionIndex = i;
            positionHint = i;
            break;
          }
        }
      }
      //Do interpolation...
      if (foundPositionIndex != -1) {
        if (InterpolationMode == EIM_CONSTANT || foundPositionIndex == 0) {
          position = PositionKeys[foundPositionIndex].position;
        }
        else if (InterpolationMode == EIM_LINEAR) {
          const SPositionKey& KeyA = PositionKeys[foundPositionIndex];
          const SPositionKey& KeyB = PositionKeys[foundPositionIndex - 1];
          const f32 fd1 = frame - KeyA.frame;
          const f32 fd2 = KeyB.frame - frame;
          position = ((KeyB.position - KeyA.position) / (fd1 + fd2)) * fd1 + KeyA.position;
        }
      }
    }
    //------------------------------------------------------------
    if (ScaleKeys_size) {
      foundScaleIndex = -1;
      //Test the Hints...
      if (scaleHint >= 0 && (u32)scaleHint < ScaleKeys_size) {
        //check this hint
        if (scaleHint > 0 && ScaleKeys[scaleHint].frame >= frame && ScaleKeys[scaleHint - 1].frame < frame) {
          foundScaleIndex = scaleHint;
        }
        else if (scaleHint + 1 < (s32)ScaleKeys_size) {
          //check the next index
          if (ScaleKeys[scaleHint + 1].frame >= frame &&
              ScaleKeys[scaleHint + 0].frame < frame) {
            scaleHint++;
            foundScaleIndex = scaleHint;
          }
        }
      }
      //The hint test failed, do a full scan...
      if (foundScaleIndex == -1) {
        int i;
        for (i = 0; i < ScaleKeys_size; ++i) {
          if (ScaleKeys[i].frame >= frame) { //Keys should to be sorted by frame
            foundScaleIndex = i;
            scaleHint = i;
            break;
          }
        }
      }
      //Do interpolation...
      if (foundScaleIndex != -1) {
        if (InterpolationMode == EIM_CONSTANT || foundScaleIndex == 0) {
          scale = ScaleKeys[foundScaleIndex].scale;
        }
        else if (InterpolationMode == EIM_LINEAR) {
          const SScaleKey& KeyA = ScaleKeys[foundScaleIndex];
          const SScaleKey& KeyB = ScaleKeys[foundScaleIndex - 1];
          const f32 fd1 = frame - KeyA.frame;
          const f32 fd2 = KeyB.frame - frame;
          scale = ((KeyB.scale - KeyA.scale) / (fd1 + fd2)) * fd1 + KeyA.scale;
        }
      }
    }
    //-------------------------------------------------------------
    if (RotationKeys_size) {
      foundRotationIndex = -1;
      //Test the Hints...
      if (rotationHint >= 0 && (u32)rotationHint < RotationKeys_size) {
        //check this hint
        if (rotationHint > 0 && RotationKeys[rotationHint].frame >= frame && RotationKeys[rotationHint - 1].frame < frame) {
          foundRotationIndex = rotationHint;
        }
        else if (rotationHint + 1 < (s32)RotationKeys_size) {
          //check the next index
          if (RotationKeys[rotationHint + 1].frame >= frame &&
              RotationKeys[rotationHint + 0].frame < frame) {
            rotationHint++;
            foundRotationIndex = rotationHint;
          }
        }
      }
      //The hint test failed, do a full scan...
      if (foundRotationIndex == -1) {
        int i;
        for (i = 0; i < RotationKeys_size; ++i) {
          if (RotationKeys[i].frame >= frame) { //Keys should be sorted by frame
            foundRotationIndex = i;
            rotationHint = i;
            break;
          }
        }
      }
      //Do interpolation...
      if (foundRotationIndex != -1) {
        if (InterpolationMode == EIM_CONSTANT || foundRotationIndex == 0) {
          rotation = RotationKeys[foundRotationIndex].rotation;
        }
        else if (InterpolationMode == EIM_LINEAR) {
          const SRotationKey& KeyA = RotationKeys[foundRotationIndex];
          const SRotationKey& KeyB = RotationKeys[foundRotationIndex - 1];
          const f32 fd1 = frame - KeyA.frame;
          const f32 fd2 = KeyB.frame - frame;
          const f32 t = fd1 / (fd1 + fd2);
          /*
          f32 t = 0;
          if (KeyA.frame!=KeyB.frame)
            t = (frame-KeyA.frame) / (KeyB.frame - KeyA.frame);
          */
          rotation.slerp(KeyA.rotation, KeyB.rotation, t);
        }
      }
    }
  }
}
//--------------------------------------------------------------------------
//        Software Skinning
//--------------------------------------------------------------------------
//! Preforms a software skin on this mesh based of joint positions
void CSkinnedMesh::skinMesh()
{
  if (!HasAnimation || SkinnedLastFrame) {
    return;
  }
  //----------------
  // This is marked as "Temp!".  A shiny dubloon to whomever can tell me why.
  buildAllGlobalAnimatedMatrices();
  //-----------------
  SkinnedLastFrame = true;
  if (!HardwareSkinning) {
    //Software skin....
    u32 i;
    //rigid animation
    for (i = 0; i < AllJoints_size; ++i) {
      for (int j = 0; j < AllJoints[i]->AttachedMeshes_size; ++j) {
        SSkinMeshBuffer* Buffer = (*SkinningBuffers)[ AllJoints[i]->AttachedMeshes[j] ];
        Buffer->Transformation = AllJoints[i]->GlobalAnimatedMatrix;
      }
    }
    //clear skinning helper array
    for (i = 0; i < Vertices_Moved_size; ++i)
      for (int j = 0; j < Vertices_Moved[i]_size; ++j) {
        Vertices_Moved[i][j] = false;
      }
    //skin starting with the root joints
    for (i = 0; i < RootJoints_size; ++i) {
      skinJoint(RootJoints[i], 0);
    }
    for (i = 0; i < SkinningBuffers->size(); ++i) {
      (*SkinningBuffers)[i]->setDirty(EBT_VERTEX);
    }
  }
  updateBoundingBox();
}
void CSkinnedMesh::skinJoint(SJoint* joint, SJoint* parentJoint)
{
  if (joint->Weights_size) {
    //Find this joints pull on vertices...
    matrix4 jointVertexPull(matrix4::EM4CONST_NOTHING);
    jointVertexPull.setbyproduct(joint->GlobalAnimatedMatrix, joint->GlobalInversedMatrix);
    float3 thisVertexMove, thisNormalMove;
    SSkinMeshBuffer* > & buffersUsed = *SkinningBuffers;
    //Skin Vertices Positions and Normals...
    int i;
    for (i = 0; i < joint->Weights_size; ++i) {
      SWeight& weight = joint->Weights[i];
      // Pull this vertex...
      jointVertexPull.transformVect(thisVertexMove, weight.StaticPos);
      if (AnimateNormals) {
        jointVertexPull.rotateVect(thisNormalMove, weight.StaticNormal);
      }
      if (!(*(weight.Moved))) {
        *(weight.Moved) = true;
        buffersUsed[weight.buffer_id]->getVertex(weight.vertex_id)->Pos = thisVertexMove * weight.strength;
        if (AnimateNormals) {
          buffersUsed[weight.buffer_id]->getVertex(weight.vertex_id)->Normal = thisNormalMove * weight.strength;
        }
        //*(weight._Pos) = thisVertexMove * weight.strength;
      }
      else {
        buffersUsed[weight.buffer_id]->getVertex(weight.vertex_id)->Pos += thisVertexMove * weight.strength;
        if (AnimateNormals) {
          buffersUsed[weight.buffer_id]->getVertex(weight.vertex_id)->Normal += thisNormalMove * weight.strength;
        }
        //*(weight._Pos) += thisVertexMove * weight.strength;
      }
      buffersUsed[weight.buffer_id]->boundingBoxNeedsRecalculated();
    }
  }
  //Skin all children
  for (int j = 0; j < joint->Children_size; ++j) {
    skinJoint(joint->Children[j], joint);
  }
}
E_ANIMATED_MESH_TYPE CSkinnedMesh::getMeshType() const
{
  return EAMT_SKINNED;
}
//! Gets joint count.
u32 CSkinnedMesh::getJointCount() const
{
  return AllJoints_size;
}
//! Gets the name of a joint.
const c8* CSkinnedMesh::getJointName(u32 number) const
{
  if (number >= AllJoints_size) {
    return 0;
  }
  return AllJoints[number]->Name;
}
//! Gets a joint number from its name
s32 CSkinnedMesh::getJointNumber(const c8* name) const
{
  int i;
  for (i = 0; i < AllJoints_size; ++i) {
    if (AllJoints[i]->Name == name) {
      return i;
    }
  }
  return -1;
}
//! returns amount of mesh buffers.
u32 CSkinnedMesh::getMeshBufferCount() const
{
  return LocalBuffers_size;
}
//! returns pointer to a mesh buffer
IMeshBuffer* CSkinnedMesh::getMeshBuffer(u32 nr) const
{
  if (nr < LocalBuffers_size) {
    return LocalBuffers[nr];
  }
  else {
    return 0;
  }
}
//! Returns pointer to a mesh buffer which fits a material
IMeshBuffer* CSkinnedMesh::getMeshBuffer(const SMaterial& material) const
{
  int i;
  for (i = 0; i < LocalBuffers_size; ++i) {
    if (LocalBuffers[i]->getMaterial() == material) {
      return LocalBuffers[i];
    }
  }
  return 0;
}
//! returns an axis aligned bounding box
const F3AABBOX& CSkinnedMesh::getBoundingBox() const
{
  return BoundingBox;
}
//! set user axis aligned bounding box
void CSkinnedMesh::setBoundingBox(const aabbox3df& box)
{
  BoundingBox = box;
}
//! sets a flag of all contained materials to a new value
void CSkinnedMesh::setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue)
{
  int i;
  for (i = 0; i < LocalBuffers_size; ++i) {
    LocalBuffers[i]->Material.setFlag(flag, newvalue);
  }
}
//! set the hardware mapping hint, for driver
void CSkinnedMesh::setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint,
    E_BUFFER_TYPE buffer)
{
  int i;
  for (i = 0; i < LocalBuffers_size; ++i) {
    LocalBuffers[i]->setHardwareMappingHint(newMappingHint, buffer);
  }
}
//! flags the meshbuffer as changed, reloads hardware buffers
void CSkinnedMesh::setDirty(E_BUFFER_TYPE buffer)
{
  int i;
  for (i = 0; i < LocalBuffers_size; ++i) {
    LocalBuffers[i]->setDirty(buffer);
  }
}
//! uses animation from another mesh
bool CSkinnedMesh::useAnimationFrom(const ISkinnedMesh* mesh)
{
  bool unmatched = false;
  int i;
  for (i = 0; i < AllJoints_size; ++i) {
    SJoint* joint = AllJoints[i];
    joint->UseAnimationFrom = 0;
    if (joint->Name == "") {
      unmatched = true;
    }
    else {
      for (int j = 0; j < mesh->getAllJoints().size(); ++j) {
        SJoint* otherJoint = mesh->getAllJoints()[j];
        if (joint->Name == otherJoint->Name) {
          joint->UseAnimationFrom = otherJoint;
        }
      }
      if (!joint->UseAnimationFrom) {
        unmatched = true;
      }
    }
  }
  checkForAnimation();
  return !unmatched;
}
//!Update Normals when Animating
//!False= Don't animate them, faster
//!True= Update normals (default)
void CSkinnedMesh::updateNormalsWhenAnimating(bool on)
{
  AnimateNormals = on;
}
//!Sets Interpolation Mode
void CSkinnedMesh::setInterpolationMode(E_INTERPOLATION_MODE mode)
{
  InterpolationMode = mode;
}
SSkinMeshBuffer* > & CSkinnedMesh::getMeshBuffers()
{
  return LocalBuffers;
}
CSkinnedMesh::SJoint* > & CSkinnedMesh::getAllJoints()
{
  return AllJoints;
}
const CSkinnedMesh::SJoint* > & CSkinnedMesh::getAllJoints() const
{
  return AllJoints;
}
//! (This feature is not implementated in irrlicht yet)
bool CSkinnedMesh::setHardwareSkinning(bool on)
{
  if (HardwareSkinning != on) {
    if (on) {
      //set mesh to static pose...
      int i;
      for (i = 0; i < AllJoints_size; ++i) {
        SJoint* joint = AllJoints[i];
        for (int j = 0; j < joint->Weights_size; ++j) {
          const u16 buffer_id = joint->Weights[j].buffer_id;
          const u32 vertex_id = joint->Weights[j].vertex_id;
          LocalBuffers[buffer_id]->getVertex(vertex_id)->Pos = joint->Weights[j].StaticPos;
          LocalBuffers[buffer_id]->getVertex(vertex_id)->Normal = joint->Weights[j].StaticNormal;
          LocalBuffers[buffer_id]->boundingBoxNeedsRecalculated();
        }
      }
    }
    HardwareSkinning = on;
  }
  return HardwareSkinning;
}
void CSkinnedMesh::calculateGlobalMatrices(SJoint* joint, SJoint* parentJoint)
{
  if (!joint && parentJoint) { // bit of protection from endless loops
    return;
  }
  //Go through the root bones
  if (!joint) {
    int i;
    for (i = 0; i < RootJoints_size; ++i) {
      calculateGlobalMatrices(RootJoints[i], 0);
    }
    return;
  }
  if (!parentJoint) {
    joint->GlobalMatrix = joint->LocalMatrix;
  }
  else {
    joint->GlobalMatrix = parentJoint->GlobalMatrix * joint->LocalMatrix;
  }
  joint->LocalAnimatedMatrix = joint->LocalMatrix;
  joint->GlobalAnimatedMatrix = joint->GlobalMatrix;
  if (joint->GlobalInversedMatrix.isIdentity()) { //might be pre calculated
    joint->GlobalInversedMatrix = joint->GlobalMatrix;
    joint->GlobalInversedMatrix.makeInverse(); // slow
  }
  for (int j = 0; j < joint->Children_size; ++j) {
    calculateGlobalMatrices(joint->Children[j], joint);
  }
  SkinnedLastFrame = false;
}
void CSkinnedMesh::checkForAnimation()
{
  u32 i, j;
  //Check for animation...
  HasAnimation = false;
  for (i = 0; i < AllJoints_size; ++i) {
    if (AllJoints[i]->UseAnimationFrom) {
      if (AllJoints[i]->UseAnimationFrom->PositionKeys_size ||
          AllJoints[i]->UseAnimationFrom->ScaleKeys_size ||
          AllJoints[i]->UseAnimationFrom->RotationKeys_size) {
        HasAnimation = true;
      }
    }
  }
  //meshes with weights, are still counted as animated for ragdolls, etc
  if (!HasAnimation) {
    for (i = 0; i < AllJoints_size; ++i) {
      if (AllJoints[i]->Weights_size) {
        HasAnimation = true;
      }
    }
  }
  if (HasAnimation) {
    //--- Find the length of the animation ---
    AnimationFrames = 0;
    for (i = 0; i < AllJoints_size; ++i) {
      if (AllJoints[i]->UseAnimationFrom) {
        if (AllJoints[i]->UseAnimationFrom->PositionKeys_size)
          if (AllJoints[i]->UseAnimationFrom->PositionKeys.getLast().frame > AnimationFrames) {
            AnimationFrames = AllJoints[i]->UseAnimationFrom->PositionKeys.getLast().frame;
          }
        if (AllJoints[i]->UseAnimationFrom->ScaleKeys_size)
          if (AllJoints[i]->UseAnimationFrom->ScaleKeys.getLast().frame > AnimationFrames) {
            AnimationFrames = AllJoints[i]->UseAnimationFrom->ScaleKeys.getLast().frame;
          }
        if (AllJoints[i]->UseAnimationFrom->RotationKeys_size)
          if (AllJoints[i]->UseAnimationFrom->RotationKeys.getLast().frame > AnimationFrames) {
            AnimationFrames = AllJoints[i]->UseAnimationFrom->RotationKeys.getLast().frame;
          }
      }
    }
  }
  if (HasAnimation && !PreparedForSkinning) {
    PreparedForSkinning = true;
    //check for bugs:
    for (i = 0; i < AllJoints_size; ++i) {
      SJoint* joint = AllJoints[i];
      for (j = 0; j < joint->Weights_size; ++j) {
        const u16 buffer_id = joint->Weights[j].buffer_id;
        const u32 vertex_id = joint->Weights[j].vertex_id;
        //check for invalid ids
        if (buffer_id >= LocalBuffers_size) {
          os::Printer::log("Skinned Mesh: Weight buffer id too large", ELL_WARNING);
          joint->Weights[j].buffer_id = joint->Weights[j].vertex_id = 0;
        }
        else if (vertex_id >= LocalBuffers[buffer_id]->getVertexCount()) {
          os::Printer::log("Skinned Mesh: Weight vertex id too large", ELL_WARNING);
          joint->Weights[j].buffer_id = joint->Weights[j].vertex_id = 0;
        }
      }
    }
    //An array used in skinning
    for (i = 0; i < Vertices_Moved_size; ++i)
      for (j = 0; j < Vertices_Moved[i]_size; ++j) {
        Vertices_Moved[i][j] = false;
      }
    // For skinning: cache weight values for speed
    for (i = 0; i < AllJoints_size; ++i) {
      SJoint* joint = AllJoints[i];
      for (j = 0; j < joint->Weights_size; ++j) {
        const u16 buffer_id = joint->Weights[j].buffer_id;
        const u32 vertex_id = joint->Weights[j].vertex_id;
        joint->Weights[j].Moved = &Vertices_Moved[buffer_id] [vertex_id];
        joint->Weights[j].StaticPos = LocalBuffers[buffer_id]->getVertex(vertex_id)->Pos;
        joint->Weights[j].StaticNormal = LocalBuffers[buffer_id]->getVertex(vertex_id)->Normal;
        //joint->Weights[j]._Pos=&Buffers[buffer_id]->getVertex(vertex_id)->Pos;
      }
    }
    // normalize weights
    normalizeWeights();
  }
  SkinnedLastFrame = false;
}
//! called by loader after populating with mesh and bone data
void CSkinnedMesh::finalize()
{
  u32 i;
  // Make sure we recalc the next frame
  LastAnimatedFrame = -1;
  SkinnedLastFrame = false;
  //calculate bounding box
  for (i = 0; i < LocalBuffers_size; ++i) {
    LocalBuffers[i]->recalculateBoundingBox();
  }
  if (AllJoints_size || RootJoints_size) {
    // populate AllJoints or RootJoints, depending on which is empty
    if (!RootJoints_size) {
      for (int CheckingIdx = 0; CheckingIdx < AllJoints_size; ++CheckingIdx) {
        bool foundParent = false;
        for (i = 0; i < AllJoints_size; ++i) {
          for (int n = 0; n < AllJoints[i]->Children_size; ++n) {
            if (AllJoints[i]->Children[n] == AllJoints[CheckingIdx]) {
              foundParent = true;
            }
          }
        }
        if (!foundParent) {
          RootJoints.push_back(AllJoints[CheckingIdx]);
        }
      }
    }
    else {
      AllJoints = RootJoints;
    }
  }
  for (i = 0; i < AllJoints_size; ++i) {
    AllJoints[i]->UseAnimationFrom = AllJoints[i];
  }
  //Set array sizes...
  for (i = 0; i < LocalBuffers_size; ++i) {
    Vertices_Moved.push_back(bool > ());
    Vertices_Moved[i].set_used(LocalBuffers[i]->getVertexCount());
  }
  //Todo: optimise keys here...
  checkForAnimation();
  if (HasAnimation) {
    //--- optimize and check keyframes ---
    for (i = 0; i < AllJoints_size; ++i) {
      SPositionKey* PositionKeys = AllJoints[i]->PositionKeys;
      SScaleKey* ScaleKeys = AllJoints[i]->ScaleKeys;
      SRotationKey > & RotationKeys = AllJoints[i]->RotationKeys;
      if (PositionKeys_size > 2) {
        for (int j = 0; j < PositionKeys_size - 2; ++j) {
          if (PositionKeys[j].position == PositionKeys[j + 1].position && PositionKeys[j + 1].position == PositionKeys[j + 2].position) {
            PositionKeys.erase(j + 1); //the middle key is unneeded
            --j;
          }
        }
      }
      if (PositionKeys_size > 1) {
        for (int j = 0; j < PositionKeys_size - 1; ++j) {
          if (PositionKeys[j].frame >= PositionKeys[j + 1].frame) { //bad frame, unneed and may cause problems
            PositionKeys.erase(j + 1);
            --j;
          }
        }
      }
      if (ScaleKeys_size > 2) {
        for (int j = 0; j < ScaleKeys_size - 2; ++j) {
          if (ScaleKeys[j].scale == ScaleKeys[j + 1].scale && ScaleKeys[j + 1].scale == ScaleKeys[j + 2].scale) {
            ScaleKeys.erase(j + 1); //the middle key is unneeded
            --j;
          }
        }
      }
      if (ScaleKeys_size > 1) {
        for (int j = 0; j < ScaleKeys_size - 1; ++j) {
          if (ScaleKeys[j].frame >= ScaleKeys[j + 1].frame) { //bad frame, unneed and may cause problems
            ScaleKeys.erase(j + 1);
            --j;
          }
        }
      }
      if (RotationKeys_size > 2) {
        for (int j = 0; j < RotationKeys_size - 2; ++j) {
          if (RotationKeys[j].rotation == RotationKeys[j + 1].rotation && RotationKeys[j + 1].rotation == RotationKeys[j + 2].rotation) {
            RotationKeys.erase(j + 1); //the middle key is unneeded
            --j;
          }
        }
      }
      if (RotationKeys_size > 1) {
        for (int j = 0; j < RotationKeys_size - 1; ++j) {
          if (RotationKeys[j].frame >= RotationKeys[j + 1].frame) { //bad frame, unneed and may cause problems
            RotationKeys.erase(j + 1);
            --j;
          }
        }
      }
      //Fill empty keyframe areas
      if (PositionKeys_size) {
        SPositionKey* Key;
        Key = &PositionKeys[0]; //getFirst
        if (Key->frame != 0) {
          PositionKeys.push_front(*Key);
          Key = &PositionKeys[0]; //getFirst
          Key->frame = 0;
        }
        Key = &PositionKeys.getLast();
        if (Key->frame != AnimationFrames) {
          PositionKeys.push_back(*Key);
          Key = &PositionKeys.getLast();
          Key->frame = AnimationFrames;
        }
      }
      if (ScaleKeys_size) {
        SScaleKey* Key;
        Key = &ScaleKeys[0]; //getFirst
        if (Key->frame != 0) {
          ScaleKeys.push_front(*Key);
          Key = &ScaleKeys[0]; //getFirst
          Key->frame = 0;
        }
        Key = &ScaleKeys.getLast();
        if (Key->frame != AnimationFrames) {
          ScaleKeys.push_back(*Key);
          Key = &ScaleKeys.getLast();
          Key->frame = AnimationFrames;
        }
      }
      if (RotationKeys_size) {
        SRotationKey* Key;
        Key = &RotationKeys[0]; //getFirst
        if (Key->frame != 0) {
          RotationKeys.push_front(*Key);
          Key = &RotationKeys[0]; //getFirst
          Key->frame = 0;
        }
        Key = &RotationKeys.getLast();
        if (Key->frame != AnimationFrames) {
          RotationKeys.push_back(*Key);
          Key = &RotationKeys.getLast();
          Key->frame = AnimationFrames;
        }
      }
    }
  }
  //Needed for animation and skinning...
  calculateGlobalMatrices(0, 0);
  //animateMesh(0, 1);
  //buildAllLocalAnimatedMatrices();
  //buildAllGlobalAnimatedMatrices();
  //rigid animation for non animated meshes
  for (i = 0; i < AllJoints_size; ++i) {
    for (int j = 0; j < AllJoints[i]->AttachedMeshes_size; ++j) {
      SSkinMeshBuffer* Buffer = (*SkinningBuffers)[ AllJoints[i]->AttachedMeshes[j] ];
      Buffer->Transformation = AllJoints[i]->GlobalAnimatedMatrix;
    }
  }
  //calculate bounding box
  if (LocalBuffers.empty()) {
    BoundingBox.reset(0, 0, 0);
  }
  else {
    aabbox3df bb(LocalBuffers[0]->BoundingBox);
    LocalBuffers[0]->Transformation.transformBoxEx(bb);
    BoundingBox.reset(bb);
    for (int j = 1; j < LocalBuffers_size; ++j) {
      bb = LocalBuffers[j]->BoundingBox;
      LocalBuffers[j]->Transformation.transformBoxEx(bb);
      BoundingBox.addInternalBox(bb);
    }
  }
}
void CSkinnedMesh::updateBoundingBox(void)
{
  if (!SkinningBuffers) {
    return;
  }
  SSkinMeshBuffer* > & buffer = *SkinningBuffers;
  BoundingBox.reset(0, 0, 0);
  if (!buffer.empty()) {
    for (int j = 0; j < buffer_size; ++j) {
      buffer[j]->recalculateBoundingBox();
      aabbox3df bb = buffer[j]->BoundingBox;
      buffer[j]->Transformation.transformBoxEx(bb);
      BoundingBox.addInternalBox(bb);
    }
  }
}
SSkinMeshBuffer* CSkinnedMesh::addMeshBuffer()
{
  SSkinMeshBuffer* buffer = new SSkinMeshBuffer();
  LocalBuffers.push_back(buffer);
  return buffer;
}
CSkinnedMesh::SJoint* CSkinnedMesh::addJoint(SJoint* parent)
{
  SJoint* joint = new SJoint;
  AllJoints.push_back(joint);
  if (!parent) {
    //Add root joints to array in finalize()
  }
  else {
    //Set parent (Be careful of the mesh loader also setting the parent)
    parent->Children.push_back(joint);
  }
  return joint;
}
CSkinnedMesh::SPositionKey* CSkinnedMesh::addPositionKey(SJoint* joint)
{
  if (!joint) {
    return 0;
  }
  joint->PositionKeys.push_back(SPositionKey());
  return &joint->PositionKeys.getLast();
}
CSkinnedMesh::SScaleKey* CSkinnedMesh::addScaleKey(SJoint* joint)
{
  if (!joint) {
    return 0;
  }
  joint->ScaleKeys.push_back(SScaleKey());
  return &joint->ScaleKeys.getLast();
}
CSkinnedMesh::SRotationKey* CSkinnedMesh::addRotationKey(SJoint* joint)
{
  if (!joint) {
    return 0;
  }
  joint->RotationKeys.push_back(SRotationKey());
  return &joint->RotationKeys.getLast();
}
CSkinnedMesh::SWeight* CSkinnedMesh::addWeight(SJoint* joint)
{
  if (!joint) {
    return 0;
  }
  joint->Weights.push_back(SWeight());
  return &joint->Weights.getLast();
}
bool CSkinnedMesh::isStatic()
{
  return !HasAnimation;
}
void CSkinnedMesh::normalizeWeights()
{
  // note: unsure if weights ids are going to be used.
  // Normalise the weights on bones....
  u32 i, j;
  f32 > > verticesTotalWeight;
  verticesTotalWeight.reallocate(LocalBuffers_size);
  for (i = 0; i < LocalBuffers_size; ++i) {
    verticesTotalWeight.push_back(f32 > ());
    verticesTotalWeight[i].set_used(LocalBuffers[i]->getVertexCount());
  }
  for (i = 0; i < verticesTotalWeight_size; ++i)
    for (j = 0; j < verticesTotalWeight[i]_size; ++j) {
      verticesTotalWeight[i][j] = 0;
    }
  for (i = 0; i < AllJoints_size; ++i) {
    SJoint* joint = AllJoints[i];
    for (j = 0; j < joint->Weights_size; ++j) {
      if (joint->Weights[j].strength <= 0) { //Check for invalid weights
        joint->Weights.erase(j);
        --j;
      }
      else {
        verticesTotalWeight[joint->Weights[j].buffer_id] [joint->Weights[j].vertex_id] += joint->Weights[j].strength;
      }
    }
  }
  for (i = 0; i < AllJoints_size; ++i) {
    SJoint* joint = AllJoints[i];
    for (j = 0; j < joint->Weights_size; ++j) {
      const f32 total = verticesTotalWeight[joint->Weights[j].buffer_id] [joint->Weights[j].vertex_id];
      if (total != 0 && total != 1) {
        joint->Weights[j].strength /= total;
      }
    }
  }
}
void CSkinnedMesh::recoverJointsFromMesh(IBoneSceneNode* > & jointChildSceneNodes)
{
  int i;
  for (i = 0; i < AllJoints_size; ++i) {
    IBoneSceneNode* node = jointChildSceneNodes[i];
    SJoint* joint = AllJoints[i];
    node->setPosition(joint->LocalAnimatedMatrix.getTranslation());
    node->setRotation(joint->LocalAnimatedMatrix.getRotationDegrees());
    node->setScale(joint->LocalAnimatedMatrix.getScale());
    node->positionHint = joint->positionHint;
    node->scaleHint = joint->scaleHint;
    node->rotationHint = joint->rotationHint;
    node->updateAbsolutePosition();
  }
}
void CSkinnedMesh::transferJointsToMesh(const IBoneSceneNode* > & jointChildSceneNodes)
{
  int i;
  for (i = 0; i < AllJoints_size; ++i) {
    const IBoneSceneNode* const node = jointChildSceneNodes[i];
    SJoint* joint = AllJoints[i];
    joint->LocalAnimatedMatrix.setRotationDegrees(node->getRotation());
    joint->LocalAnimatedMatrix.setTranslation(node->getPosition());
    joint->LocalAnimatedMatrix *= matrix4().setScale(node->getScale());
    joint->positionHint = node->positionHint;
    joint->scaleHint = node->scaleHint;
    joint->rotationHint = node->rotationHint;
    joint->GlobalSkinningSpace = (node->getSkinningSpace() == EBSS_GLOBAL);
  }
  // Make sure we recalc the next frame
  LastAnimatedFrame = -1;
  SkinnedLastFrame = false;
}
void CSkinnedMesh::transferOnlyJointsHintsToMesh(const IBoneSceneNode* > & jointChildSceneNodes)
{
  int i;
  for (i = 0; i < AllJoints_size; ++i) {
    const IBoneSceneNode* const node = jointChildSceneNodes[i];
    SJoint* joint = AllJoints[i];
    joint->positionHint = node->positionHint;
    joint->scaleHint = node->scaleHint;
    joint->rotationHint = node->rotationHint;
  }
  SkinnedLastFrame = false;
}
void CSkinnedMesh::addJoints(IBoneSceneNode* > & jointChildSceneNodes,
    IAnimatedMeshSceneNode* node, ISceneManager* smgr)
{
  //Create new joints
  int i;
  for (i = 0; i < AllJoints_size; ++i) {
    jointChildSceneNodes.push_back(new CBoneSceneNode(0, smgr, 0, i, AllJoints[i]->Name));
  }
  //Match up parents
  for (i = 0; i < jointChildSceneNodes_size; ++i) {
    const SJoint* const joint = AllJoints[i]; //should be fine
    s32 parentID = -1;
    for (int j = 0; (parentID == -1) && (j < AllJoints_size); ++j) {
      if (i != j) {
        const SJoint* const parentTest = AllJoints[j];
        for (int n = 0; n < parentTest->Children_size; ++n) {
          if (parentTest->Children[n] == joint) {
            parentID = j;
            break;
          }
        }
      }
    }
    IBoneSceneNode* bone = jointChildSceneNodes[i];
    if (parentID != -1) {
      bone->setParent(jointChildSceneNodes[parentID]);
    }
    else {
      bone->setParent(node);
    }
    bone->drop();
  }
  SkinnedLastFrame = false;
}
void CSkinnedMesh::convertMeshToTangents()
{
  // now calculate tangents
  for (int b = 0; b < LocalBuffers_size; ++b) {
    if (LocalBuffers[b]) {
      LocalBuffers[b]->convertToTangents();
      const s32 idxCnt = LocalBuffers[b]->getIndexCount();
      u16* idx = LocalBuffers[b]->getIndices();
      S3DVertex* v =
          (S3DVertex*)LocalBuffers[b]->getVertices();
      for (s32 i = 0; i < idxCnt; i += 3) {
        calculateTangents(
            v[idx[i + 0]].Normal,
            v[idx[i + 0]].Tangent,
            v[idx[i + 0]].Binormal,
            v[idx[i + 0]].Pos,
            v[idx[i + 1]].Pos,
            v[idx[i + 2]].Pos,
            v[idx[i + 0]].TCoords,
            v[idx[i + 1]].TCoords,
            v[idx[i + 2]].TCoords);
        calculateTangents(
            v[idx[i + 1]].Normal,
            v[idx[i + 1]].Tangent,
            v[idx[i + 1]].Binormal,
            v[idx[i + 1]].Pos,
            v[idx[i + 2]].Pos,
            v[idx[i + 0]].Pos,
            v[idx[i + 1]].TCoords,
            v[idx[i + 2]].TCoords,
            v[idx[i + 0]].TCoords);
        calculateTangents(
            v[idx[i + 2]].Normal,
            v[idx[i + 2]].Tangent,
            v[idx[i + 2]].Binormal,
            v[idx[i + 2]].Pos,
            v[idx[i + 0]].Pos,
            v[idx[i + 1]].Pos,
            v[idx[i + 2]].TCoords,
            v[idx[i + 0]].TCoords,
            v[idx[i + 1]].TCoords);
      }
    }
  }
}
void CSkinnedMesh::calculateTangents(
    float3 normal,
    float3 tangent,
    float3 binormal,
    float3 vt1, float3 vt2, float3 vt3, // vertices
    vector2df& tc1, vector2df& tc2, vector2df& tc3) // texture coords
{
  float3 v1 = vt1 - vt2;
  float3 v2 = vt3 - vt1;
  normal = v2.crossProduct(v1);
  normal.normalize();
  // binormal
  f32 deltaX1 = tc1.X - tc2.X;
  f32 deltaX2 = tc3.X - tc1.X;
  binormal = (v1 * deltaX2) - (v2 * deltaX1);
  binormal.normalize();
  // tangent
  f32 deltaY1 = tc1.Y - tc2.Y;
  f32 deltaY2 = tc3.Y - tc1.Y;
  tangent = (v1 * deltaY2) - (v2 * deltaY1);
  tangent.normalize();
  // adjust
  float3 txb = tangent.crossProduct(binormal);
  if (txb.dotProduct(normal) < 0.0f) {
    tangent *= -1.0f;
    binormal *= -1.0f;
  }
}
#endif // _IRR_COMPILE_WITH_SKINNED_MESH_SUPPORT_
