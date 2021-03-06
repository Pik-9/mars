/*
 *  Copyright 2011, 2012, DFKI GmbH Robotics Innovation Center
 *
 *  This file is part of the MARS simulation framework.
 *
 *  MARS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  MARS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with MARS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * OSGNodeStruct.cpp
 *
 *  Created on: 19.04.2011
 *      Author: daniel
 */

#include "OSGNodeStruct.h"
#include "../3d_objects/CubeDrawObject.h"
#include "../3d_objects/SphereDrawObject.h"
#include "../3d_objects/CylinderDrawObject.h"
#include "../3d_objects/CapsuleDrawObject.h"
#include "../3d_objects/PlaneDrawObject.h"
#include "../3d_objects/TerrainDrawObject.h"
#include "../3d_objects/LoadDrawObject.h"

#include <mars/interfaces/MaterialData.h>
#include <mars/utils/Vector.h>

#include <stdexcept>
#include <cstdlib>
#include <cstdio>

#include <osg/ComputeBoundsVisitor>

namespace mars {
  namespace graphics {

    using mars::interfaces::NodeData;
    using mars::interfaces::LightData;
    using mars::interfaces::MaterialData;
    using mars::utils::Vector;

    OSGNodeStruct::OSGNodeStruct(std::vector<LightData*> &lightList,
                                 const NodeData &node, bool isPreview,
                                 unsigned long id,
                                 bool useMARSShader, bool useFog,
                                 bool useNoise, bool drawLineLaser,
                                 bool marsShadow)
      : osg::Group(), drawObject_(NULL), id_(id), isPreview_(isPreview) {
      if (node.filename.compare("PRIMITIVE") == 0) {
        switch(NodeData::typeFromString(node.origName.c_str())) {
        case mars::interfaces::NODE_TYPE_BOX: {
          if(node.visual_size != Vector(0.0, 0.0, 0.0)) {
            drawObject_ = new CubeDrawObject(node.visual_size);
          }
          else {
            drawObject_ = new CubeDrawObject(node.ext);
          }
          break;
        }
        case mars::interfaces::NODE_TYPE_SPHERE: {
          if(node.visual_size != Vector(0.0, 0.0, 0.0)) {
            drawObject_ = new SphereDrawObject(node.visual_size.x());
          }
          else {
            drawObject_ = new SphereDrawObject(node.ext.x());
          }
          break;
        }
        case mars::interfaces::NODE_TYPE_REFERENCE: {
#warning add here an coordinate system item
          //For now until we have an real coordinate system
          drawObject_ = new SphereDrawObject(0.2);
          break;
        }
        case mars::interfaces::NODE_TYPE_MESH:
        case mars::interfaces::NODE_TYPE_CYLINDER: 
          if(node.visual_size != Vector(0.0, 0.0, 0.0)) {
            drawObject_ = new CylinderDrawObject(node.visual_size.x(),
                                                 node.visual_size.y());
          }
          else {
            drawObject_ = new CylinderDrawObject(node.ext.x(), node.ext.y());
          }
          break;
          break;
        case mars::interfaces::NODE_TYPE_CAPSULE: {
          if(node.visual_size != Vector(0.0, 0.0, 0.0)) {
            drawObject_ = new CapsuleDrawObject(node.visual_size.x(),
                                                node.visual_size.y());
          }
          else {
            drawObject_ = new CapsuleDrawObject(node.ext.x(), node.ext.y());
          }
          break;
        }
        case mars::interfaces::NODE_TYPE_PLANE: {
          if(node.visual_size != Vector(0.0, 0.0, 0.0)) {
            drawObject_ = new PlaneDrawObject(node.visual_size);
          }
          else {
            drawObject_ = new PlaneDrawObject(node.ext);
          }
          break;
        }
        default:
          fprintf(stderr,"Cannot find primitive type: %i(%s), at %s:%i\n",
                  NodeData::typeFromString(node.origName.c_str()),
                  node.origName.c_str(), __FILE__, __LINE__);
          throw std::runtime_error("unknown primitive type");
        }
        drawObject_->setUseMARSShader(useMARSShader);
        drawObject_->createObject(id, Vector(0.0, 0.0, 0.0));//node.pivot);
      } else if (node.physicMode == mars::interfaces::NODE_TYPE_TERRAIN) {
        // we have a heightfield
        if (!node.terrain->pixelData) {
          node.terrain->pixelData = (double*)calloc(
                                                    (node.terrain->width*node.terrain->height), sizeof(double));
          //QImage image(QString::fromStdString(snode->filename));
          int r = 0, g = 0, b = 0;
          int count = 0;
          for (int y=0; y<node.terrain->height; y++) {
            for (int x=0; x<node.terrain->width; x++) {
              //convert to greyscale by common used scale
              node.terrain->pixelData[count++] = ((r*0.3+g*0.59+b*0.11)/255.0);
            }
          }
        }
        drawObject_ = new TerrainDrawObject(node.terrain);
        drawObject_->setUseMARSShader(useMARSShader);
        drawObject_->createObject(id, Vector(node.terrain->targetWidth*0.5,
                                             node.terrain->targetHeight*0.5, 0.0));
      } else { // we have to load the node from an import file
        LoadDrawObjectInfo info;
        info.fileName = node.filename;
        info.objectName = node.origName;
        drawObject_ = new LoadDrawObject(info, node.ext);
        drawObject_->setUseMARSShader(useMARSShader);
        drawObject_->createObject(id, node.pivot);
        drawObject_->setScale(node.visual_scale);
        if(node.visual_size != Vector(0.0, 0.0, 0.0)) {
          drawObject_->setScaledSize(node.visual_size);
        }
      }

      drawObject_->setPosition(node.pos + node.rot * node.visual_offset_pos);
      drawObject_->setQuaternion(node.rot * node.visual_offset_rot);

      MaterialData ms = node.material;

      if(isPreview) {
        ms.transparency = 0.8;
      }

      drawObject_->setMaterial(ms, useFog, useNoise, drawLineLaser, marsShadow);

      if(!isPreview) {
        drawObject_->updateShader(lightList, false, node.shaderSources);
      }
    }

    void OSGNodeStruct::edit(const NodeData &node, bool resize) {
      osg::ComputeBoundsVisitor cbbv;
      osg::BoundingBox bb = cbbv.getBoundingBox();
      osg::Quat oquat;
      Vector ex;

      // get old size, create bounding box to get center point
      drawObject_->getObject()->accept(cbbv);
      // compute bounding box has to be done in this way
      if(fabs(bb.xMax()) > fabs(bb.xMin())) {
        ex.x() = fabs(bb.xMax() - bb.xMin());
      } else {
        ex.x() = fabs(bb.xMin() - bb.xMax());
      }
      if(fabs(bb.yMax()) > fabs(bb.yMin())) {
        ex.y() = fabs(bb.yMax() - bb.yMin());
      } else {
        ex.y() = fabs(bb.yMin() - bb.yMax());
      }
      if(fabs(bb.zMax()) > fabs(bb.zMin())) {
        ex.z() = fabs(bb.zMax() - bb.zMin());
      } else {
        ex.z() = fabs(bb.zMin() - bb.zMax());
      }

      //compute scale factor
      double scaleX = (ex.x() != 0) ? node.visual_size.x() / ex.x() : 1;
      double scaleY = (ex.y() != 0) ? node.visual_size.y() / ex.y() : 1;
      double scaleZ = (ex.z() != 0) ? node.visual_size.z() / ex.z() : 1;

      //resize
      if (resize) {
        drawObject_->getScaleMatrix()->setMatrix(osg::Matrix::scale(scaleX, scaleY, scaleZ));
        drawObject_->getTransform()->setPivotPoint(osg::Vec3(
                                                             node.pivot.x()*scaleX, node.pivot.y()*scaleY, node.pivot.z()*scaleZ));
      }
      drawObject_->getTransform()->setPosition(osg::Vec3(
                                                         node.pos.x()+node.visual_offset_pos.x(),
                                                         node.pos.y()+node.visual_offset_pos.y(),
                                                         node.pos.z()+node.visual_offset_pos.z()));
      oquat.set(node.rot.x(), node.rot.y(), node.rot.z(), node.rot.w());
      drawObject_->getTransform()->setAttitude(oquat);
    }

  } // end of namespace graphics 
} // end of namespace mars

