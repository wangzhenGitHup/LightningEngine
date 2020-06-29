#pragma once

#include "RuntimePrefix.h"
#include "Allocator/TLSFAllocator.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector3.h"
#include "Base/Component.h"
#include "Math/Color.h"
#include "Geometry/Plane.h"
#include "Math/Rect.h"
#include "Geometry/Ray.h"
#include "GL20/FBO.h"
#include "LightningEngineGL.h"

namespace LightningEngine
{
	enum FrustumPlane
	{
		kFrustumPlaneLeft,
		kFrustumPlaneRight,
		kFrustumPlaneBottom,
		kFrustumPlaneTop,
		kFrustumPlaneNear,
		kFrustumPlaneFar,
		kFrustumPlaneCount,
	};

	struct CullingParamerters
	{
		Plane cullingPlanes[kFrustumPlaneCount];
	};

	class Camera : public Component
	{
	public:
		enum ClearMode
		{
			kClearModeSkyBox = 1,
			kClearModeSolidColor = 2,
			kClearModeDepthOnly = 3,
			kClearModeDontClear = 4,
			kClearModeClearAll = 5
		};

		enum CameraMode
		{
			PerspectiveMode = 1,
			OrthoMode = 2,
		};

	public:
		Camera();
		virtual ~Camera();
		void ClearBackground();
		void LookAt(Vector3 eye, Vector3 lookAt, Vector3 up);
		void SetProjection(float fovy, float aspect, float zNear, float zFar);
		void OnViewPortSizeChange(int width, int height);
		void SetClearColor(UInt8 r, UInt8 g, UInt8 b, UInt8 a);
		bool GetViewRect(Rect<float>& rect);
		void ScreenCoordToOpenGLCoord(float& x, float& y);
		int GetScreenShot(unsigned char** pixel, int& width, int& height);
		virtual void Update(float deltaTime);

		static bool SortCameras(const Camera* a, const Camera* b)
		{
			return a->depth_ < b->depth_;
		}

		DECLEAR_ENGINE_CLASS(Camera)
	public:
		CullingParamerters cullingParameters_;
		Matrix4x4 projectionMatrix_;
		Matrix4x4 viewMatrix_;
		float depth_;
		float orthoModeSize_;
		ClearMode clearFlag_;
		float resolutionWidth_;
		float resolutionHeight_;
		int viewWidth_;
		int viewHeight_;
		CameraMode cameraMode_;
		UInt32 cullingFlag_;
		Color4B clearColor_;
		float fieldOfView_;
		float aspect_;
		float nearClip_;
		float farClip_;
		FBO* fbo_;
		FBO* depthFBO_;
		bool bOffScreen_;
		bool bRenderDepth_;

	};

	void ExtractProjectionPlanes(const Matrix4x4& finalMat, Plane* outPlanes);
}