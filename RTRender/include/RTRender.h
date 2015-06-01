#ifndef RTRENDER_H
#define RTRENDER_H

// .\include\interfaces
#include "interfaces\RTImage.h"
#include "interfaces\RTLight.h"
#include "interfaces\RTSceneObject.h"
#include "interfaces\RTModelLoader.h"
#include "interfaces\RTTracer.h"
#include "interfaces\RTCamera.h"

// .\include\impls
#include "impls\RTPerspectiveCamera.h"
#include "impls\RTMath.h"
#include "impls\RTColor.h"
#include "impls\RTSphere.h"
#include "impls\RTPlane.h"
#include "impls\RTPointLight.h"
#include "impls\RTBoundingBox.h"
#include "impls\RTTriangle.h"
#include "impls\RTOBJLoader.h"
#include "impls\RTTransform.h"
#include "impls\RTMesh.h"
#include "impls\RTInstance.h"
#include "impls\RTRayTracer.h"
#include "impls\RTBVHTree.h"
#include "impls\RTScene.h"
#include "impls\RTLog.h"
#include "impls\RTMPIRayTracer.h"
#include "impls\RTOMPRayTracer.h"
#include "impls\RTApplication.h"
#include "impls\RTConfig.h"
#include "impls\RTTGAImage.h"
#include "impls\RTPPMImage.h"

#include "RTTypes.h"

#endif