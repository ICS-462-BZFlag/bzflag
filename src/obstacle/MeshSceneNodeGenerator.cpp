/* bzflag
 * Copyright (c) 1993-2010 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <math.h>
#include "vectors.h"
#include "MeshSceneNodeGenerator.h"
#include "MeshObstacle.h"
#include "MeshFace.h"
#include "bzfgl.h"
#include "MeshDrawInfo.h"
#include "MeshSceneNode.h"
#include "MeshPolySceneNode.h"
#include "MeshFragSceneNode.h"
#include "OccluderSceneNode.h"
#include "DynamicColor.h"
#include "TextureMatrix.h"
#include "TextureManager.h"
#include "OpenGLMaterial.h"
#include "StateDatabase.h"
#include "BZDBCache.h"


//
// MeshSceneNodeGenerator
//

MeshSceneNodeGenerator::MeshSceneNodeGenerator(const MeshObstacle* _mesh) {
  mesh = _mesh;
  currentNode = 0;
  returnOccluders = false;
  setupOccluders();
  const MeshDrawInfo* drawInfo = mesh->getDrawInfo();
  useDrawInfo = (drawInfo != NULL) && drawInfo->isValid();
  if (!useDrawInfo) {
    setupFacesAndFrags();
  }
  return;
}


MeshSceneNodeGenerator::~MeshSceneNodeGenerator() {
  // do nothing
  return;
}


void MeshSceneNodeGenerator::setupOccluders() {
  // This is wasteful, only need separate
  // occluders if the face is invisible or
  // has been grouped into a mesh fragment.
  // If this is done, don't forget to make
  // the resulting combo sceneNode into an
  // occluder.
  const int faceCount = mesh->getFaceCount();
  for (int i = 0; i < faceCount; i++) {
    const MeshFace* face = mesh->getFace(i);
    const BzMaterial* bzmat = face->getMaterial();
    if (bzmat->getOccluder()) {
      OccluderSceneNode* onode = new OccluderSceneNode(face);
      occluders.push_back(onode);
    }
  }
}


static bool translucentMaterial(const BzMaterial* mat) {
  // translucent texture?
  TextureManager& tm = TextureManager::instance();
  int faceTexture = -1;
  if (mat->getTextureCount() > 0) {
    const std::string& texname = mat->getTextureLocal(0);
    if (texname.size() > 0) {
      faceTexture = tm.getTextureID(texname);
      if (faceTexture >= 0) {
        const ImageInfo& imageInfo = tm.getInfo(faceTexture);
        if (imageInfo.alpha && mat->getUseTextureAlpha(0)) {
          return true;
        }
      }
    }
  }

  // translucent color?
  bool translucentColor = false;
  const DynamicColor* dyncol = DYNCOLORMGR.getColor(mat->getDynamicColor());
  if (dyncol == NULL) {
    if (mat->getDiffuse()[3] != 1.0f) {
      translucentColor = true;
    }
  }
  else if (dyncol->canHaveAlpha()) {
    translucentColor = true;
  }

  // is the color used?
  if (translucentColor) {
    if (((faceTexture >= 0) && mat->getUseColorOnTexture(0)) ||
        (faceTexture < 0)) {
      // modulate with the color if asked to, or
      // if the specified texture was not available
      return true;
    }
  }

  return false;
}


static bool groundClippedFace(const MeshFace* face) {
  const fvec4& plane = face->getPlane();
  if (plane.z < -0.9f) {
    // plane is facing downwards
    const Extents& exts = face->getExtents();
    if (exts.maxs.z < 0.001) {
      // plane is on or below the ground, ditch it
      return true;
    }
  }
  return false;
}


static int sortByMaterial(const void* a, const void* b) {
  const MeshFace* faceA = *((const MeshFace**)a);
  const MeshFace* faceB = *((const MeshFace**)b);
  const bool noClusterA = faceA->noClusters();
  const bool noClusterB = faceB->noClusters();

  if (noClusterA && !noClusterB) {
    return -1;
  }
  if (noClusterB && !noClusterA) {
    return +1;
  }

  if (faceA->getMaterial() > faceB->getMaterial()) {
    return +1;
  }
  else {
    return -1;
  }
}

void MeshSceneNodeGenerator::setupFacesAndFrags() {
  const int faceCount = mesh->getFaceCount();

  // NOTE: this is where MeshClusters start being called
  //       MeshFragments. it would be good to rename all
  //       of the MeshFragment files and classes to match
  //       with the MeshCluster naming convention.

  // only using regular MeshFaces?
  const bool noMeshClusters = BZDB.isTrue("noMeshClusters");
  if (mesh->noClusters() || noMeshClusters || !BZDBCache::zbuffer) {
    for (int i = 0; i < faceCount; i++) {
      const MeshFace* face = mesh->getFace(i);
      MeshNode mn;
      mn.isFace = true;
      mn.faces.push_back(face);
      nodes.push_back(mn);
    }
    return; // bail out
  }

  // build up a list of faces and fragments
  const MeshFace** sortList = new const MeshFace*[faceCount];

  // clip ground faces, and then sort the face list by material
  int count = 0;
  for (int i = 0; i < faceCount; i++) {
    const MeshFace* face = mesh->getFace(i);
    if (!groundClippedFace(face)) {
      sortList[count] = face;
      count++;
    }
  }
  qsort(sortList, count, sizeof(MeshFace*), sortByMaterial);

  // make the faces and fragments
  int first = 0;
  while (first < count) {
    const MeshFace* firstFace = sortList[first];
    const BzMaterial* firstMat = firstFace->getMaterial();

    // see if this face needs to be drawn individually
    if (firstFace->noClusters() ||
        (translucentMaterial(firstMat) &&
         !firstMat->getNoSorting() && !firstMat->getGroupAlpha())) {
      MeshNode mn;
      mn.isFace = true;
      mn.faces.push_back(firstFace);
      nodes.push_back(mn);
      first++;
      continue;
    }

    // collate similar materials
    int last = first + 1;
    while (last < count) {
      const MeshFace* lastFace = sortList[last];
      const BzMaterial* lastMat = lastFace->getMaterial();
      if (lastMat != firstMat) {
        break;
      }
      last++;
    }

    // make a face for singles, and a fragment otherwise
    if ((last - first) == 1) {
      MeshNode mn;
      mn.isFace = true;
      mn.faces.push_back(firstFace);
      nodes.push_back(mn);
    }
    else {
      MeshNode mn;
      mn.isFace = false;
      for (int i = first; i < last; i++) {
        mn.faces.push_back(sortList[i]);
      }
      nodes.push_back(mn);
    }

    first = last;
  }

  delete[] sortList;

  return;
}


WallSceneNode* MeshSceneNodeGenerator::getNextNode(bool /*lod*/) {
  const MeshNode* mn;
  const MeshFace* face;
  const BzMaterial* mat;

  // divert for Occluders
  if (returnOccluders) {
    if (currentNode < (int)occluders.size()) {
      currentNode++;
      return (WallSceneNode*)occluders[currentNode - 1];
    }
    else {
      return NULL;
    }
  }

  // divert for the MeshSceneNode
  const MeshDrawInfo* drawInfo = mesh->getDrawInfo();
  if (useDrawInfo) {
    if (drawInfo->isInvisible()) {
      if (occluders.size() <= 0) {
        return NULL;
      }
      else {
        currentNode = 1;
        returnOccluders = true;
        return (WallSceneNode*)occluders[0];
      }
    }
    else {
      currentNode = 0;
      returnOccluders = true;
      return (WallSceneNode*)(new MeshSceneNode(mesh));
    }
  }

  // remove any faces or frags that will not be displayed
  // also, return NULL if we are at the end of the face list
  while (true) {

    if (currentNode >= (int)nodes.size()) {
      // start sending out the occluders
      returnOccluders = true;
      if (occluders.size() > 0) {
        currentNode = 1;
        return (WallSceneNode*)occluders[0];
      }
      else {
        return NULL;
      }
    }

    mn = &nodes[currentNode];
    if (mn->isFace) {
      face = mn->faces[0];
      mat = face->getMaterial();
    }
    else {
      face = NULL;
      mat = mn->faces[0]->getMaterial();
    }

    if (mat->isInvisible()) {
      currentNode++;
      continue;
    }

    if (mn->isFace && groundClippedFace(face)) {
      currentNode++;
      continue;
    }

    break; // break the loop if we haven't used 'continue'
  }

  WallSceneNode* node;
  if (mn->isFace) {
    node = getMeshPolySceneNode(face);
  }
  else {
    const MeshFace** faces = new const MeshFace*[mn->faces.size()];
    for (int i = 0; i < (int)mn->faces.size(); i++) {
      faces[i] = mn->faces[i];
    }
    // the MeshFragSceneNode will delete the faces
    node = new MeshFragSceneNode(mn->faces.size(), faces);
  }

  setupNodeMaterial(node, mat);

  currentNode++;

  return node;
}


MeshPolySceneNode* MeshSceneNodeGenerator::getMeshPolySceneNode(const MeshFace* face) {
  int i;

  // vertices
  const int vertexCount = face->getVertexCount();
  fvec3Array vertices(vertexCount);
  for (i = 0; i < vertexCount; i++) {
    vertices[i] = face->getVertex(i);
  }

  // normals
  int normalCount = 0;
  if (face->useNormals()) {
    normalCount = vertexCount;
  }
  fvec3Array normals(normalCount);
  for (i = 0; i < normalCount; i++) {
    normals[i] = face->getNormal(i);
  }

  // texcoords
  fvec2Array texcoords(vertexCount);
  if (face->useTexcoords()) {
    for (i = 0; i < vertexCount; i++) {
      texcoords[i] = face->getTexcoord(i);
    }
  }
  else {
    const fvec2& autoScale = face->getMaterial()->getTextureAutoScale(0);
    makeTexcoords(autoScale, face->getPlane(), vertices, texcoords);
  }

  bool noRadar = false;
  bool noShadow = false;
  const BzMaterial* bzmat = face->getMaterial();
  if (bzmat != NULL) {
    noRadar  = bzmat->getNoRadar();
    noShadow = bzmat->getNoShadowCast(); // FIXME
  }
  MeshPolySceneNode* node =
    new MeshPolySceneNode(face->getPlane(), noRadar, noShadow,
                          vertices, normals, texcoords);

  return node;
}


void MeshSceneNodeGenerator::setupNodeMaterial(WallSceneNode* node,
                                               const BzMaterial* mat) {
  // cheat a little
  ((BzMaterial*)mat)->setReference();

  node->setBzMaterial(mat);

  node->setRadarSpecial(mat->getRadarSpecial());
}


bool MeshSceneNodeGenerator::makeTexcoords(const fvec2& autoScale,
                                           const fvec4& plane,
                                           const fvec3Array& vertices,
                                           fvec2Array& texcoords) {
  const float defScale = 1.0f / 8.0f;
  const float sScale = (autoScale.s == 0.0f) ? defScale : 1.0f / autoScale.s;
  const float tScale = (autoScale.t == 0.0f) ? defScale : 1.0f / autoScale.t;

  fvec3 x = fvec3(vertices[1]) - fvec3(vertices[0]);
  fvec3 y = fvec3::cross(plane.xyz(), x);

  if (!fvec3::normalize(x) ||
      !fvec3::normalize(y)) {
    return false;
  }

  const bool horizontal = fabsf(plane[2]) > 0.999f;

  const int count = vertices.getSize();
  for (int i = 0; i < count; i++) {
    const fvec3& v = vertices[i];
    const fvec3 delta = fvec3(v) - vertices[0];
    const fvec2 nh = fvec2(plane.x, plane.y).normalize();
    const float vs = 1.0f / sqrtf(1.0f - (plane.z * plane.z));

    if (sScale < 0.0f) {
      texcoords[i].s = -sScale * fvec3::dot(delta, x);
    }
    else {
      if (horizontal) {
        texcoords[i].s = sScale * v.x;
      }
      else {
        texcoords[i].s = sScale * ((nh.x * v.y) - (nh.y * v.x));
      }
    }

    if (tScale < 0.0f) {
      texcoords[i].t = -tScale * fvec3::dot(delta, y);
    }
    else {
      if (horizontal) {
        texcoords[i].t = tScale * v.y;
      }
      else {
        texcoords[i].t = tScale * (v.z * vs);
      }
    }
  }

  return true;
}


// Local Variables: ***
// mode: C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: nil ***
// End: ***
// ex: shiftwidth=2 tabstop=8
