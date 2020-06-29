#include "Camera.h"
#include "Debugger/Debugger.h"

namespace LightningEngine
{
	Camera::Camera() : fieldOfView_(45.0f),
		nearClip_(0.1f),
		farClip_(1000.0f),
		cameraMode_(PerspectiveMode),
		//cullingFlag_(GameObjectLayer_Default),
		depth_(0),
		clearFlag_(kClearModeClearAll),
		bEnable_(true),
		orthoModeSize_(100.0f),
		aspect_(0.0f),
		bOffScreen_(false),
		fbo_(nullptr),
		bRenderDepth_(false),
		depthFBO_(nullptr)
	{
		clearColor_.Set(41, 77, 121,  255);
	}

	Camera::~Camera()
	{
		if (bOffScreen_)
		{
			GetSceneManager()->RemoveOffScreenCamera(this);
		}
		else
		{
			GetSceneManager()->RemoveCamera(this);
		}

		if (fbo_)
		{
			delete fbo_;
		}

		if (depthFBO_)
		{
			delete depthFBO_;
		}
	}

	void Camera::ClearBackground()
	{
		if (kClearModeSkyBox == clearFlag_ || 
			kClearModeClearAll == clearFlag_)
		{
			OGL_CALL(glClearColor(clearColor_.r_ / 255.0f, 
				clearColor_.g_ / 255.0f, 
				clearColor_.b_ / 255.0f,
				clearColor_.a_ / 255.0f));
			OGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		}
		else if (kClearModeSolidColor == clearFlag_)
		{
			OGL_CALL(glClearColor(clearColor_.r_ / 255.0f,
				clearColor_.g_ / 255.0f,
				clearColor_.b_ / 255.0f,
				clearColor_.a_ / 255.0f));
			OGL_CALL(glClear(GL_COLOR_BUFFER_BIT));
		}
		else if (kClearModeDepthOnly == clearFlag_)
		{
			OGL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
		}
	}

	void Camera::LookAt(Vector3 eye, Vector3 lookAt, Vector3 up)
	{
		viewMatrix_.LookAt(eye, lookAt, up);
	}

	void Camera::SetProjection(float fovy, float aspect, float zNear, float zFar)
	{
		projectionMatrix_.SetPerspective(fovy, aspect, zNear, zFar);
	}

	void Camera::OnViewPortSizeChange(int width, int height)
	{
		viewWidth_ = (float)width;
		viewHeight_ = (float)height;
		aspect_ = viewWidth_ / viewHeight_;

		if (PerspectiveMode == cameraMode_)
		{
			SetProjection(fieldOfView_, aspect_, nearClip_, farClip_);
		}
		else if (OrthoMode == cameraMode_)
		{
			orthoModeSize_ = width;
			resolutionWidth_ = orthoModeSize_;
			resolutionHeight_ = resolutionWidth_ / aspect_;
			projectionMatrix_.SetOrtho(-resolutionWidth_ / 2.0f,
				resolutionWidth_ / 2.0f, 
				-resolutionHeight_ / 2.0f,
				resolutionHeight_ / 2.0f,
				nearClip_,
				farClip_);
		}

		if (owner_)
		{
			owner_->OnViewportChanged(viewWidth_, viewHeight_);
		}
	}

	void Camera::SetClearColor(UInt8 r, UInt8 g, UInt8 b, UInt8 a)
	{
		clearColor_.r_ = r;
		clearColor_.g_ = g;
		clearColor_.b_ = b;
		clearColor_.a_ = a;
	}

	bool Camera::GetViewRect(Rect<float>& rect)
	{
		if (OrthoMode == cameraMode_)
		{
			rect.left_ = -resolutionWidth_ / 2.0f;
			rect.width_ = resolutionWidth_;
			rect.bottom_ = -resolutionHeight_ / 2.0f;
			rect.height_ = resolutionHeight_;
			return true;
		}

		return false;
	}

	void Camera::ScreenCoordToOpenGLCoord(float & x, float & y)
	{
		float full_resolution_width = GetSceneManager()->fullResolutionWidth_;
		float full_resolution_height = GetSceneManager()->fullResolutionHeight_;
		float scale = viewWidth_ / full_resolution_width;
		x = scale * (x - full_resolution_width / 2.0f);
		y = scale * (full_resolution_height / 2.0f - y);
	}

	int Camera::GetScreenShot(unsigned char ** pixel, int & width, int & height)
	{
		if (fbo_)
		{
			width = fbo_->width_;
			height = fbo_->height_;
			int size = width * height * 4;
			*pixel = new unsigned char[size];
			unsigned char* tmpPixel = *pixel;
			fbo_->Switch();
			glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, tmpPixel);
			fbo_->Restore();

			for (int i = 0; i < size; i += 4)
			{
				unsigned char p = tmpPixel[i];
				tmpPixel[i] = tmpPixel[i + 2];
				tmpPixel[i + 2] = p;
			}

			return 4;
		}
		width = 256;
		height = 256;
		int size = width * height * 4;
		*pixel = new unsigned char[size];
		unsigned char* tmpPixel = *pixel;
		memset(tmpPixel, 0, size);
		return 4;
	}

	void Camera::Update(float deltaTime)
	{
		if (bPositionChanged_)
		{
			const Matrix4x4& world_mat = owner_->GetWorldMatrix();
			Vector3 pos(world_mat.data_[12],
				world_mat.data_[13],
				world_mat.data_[14]);

			Vector3 up(world_mat.data_[4],
				world_mat.data_[5],
				world_mat.data_[6]);

			Vector3 target(-world_mat.data_[8],
				-world_mat.data_[9],
				-world_mat.data_[10]);
			
			Vector3 center = pos + target;
			viewMatrix_.LookAt(pos, center, up);
			bPositionChanged_ = false;
		}
	}

	void ExtractProjectionPlanes(const Matrix4x4 & finalMat, Plane * outPlanes)
	{
		float tmpVec[4];
		float otherVec[4];
		tmpVec[0] = finalMat.Get(3, 0); //mat41
		tmpVec[1] = finalMat.Get(3, 1); //mat42
		tmpVec[2] = finalMat.Get(3, 2); //mat43
		tmpVec[3] = finalMat.Get(3, 3); //mat44

		otherVec[0] = finalMat.Get(0, 0); //mat11
		otherVec[1] = finalMat.Get(0, 1); //mat12
		otherVec[2] = finalMat.Get(0, 2); //mat13
		otherVec[3] = finalMat.Get(0, 3); //mat14

		//Left
		outPlanes[kFrustumPlaneLeft].SetABCD(otherVec[0] + tmpVec[0],
			otherVec[1] + tmpVec[1],
			otherVec[2] + tmpVec[2],
			otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneLeft].Normalize();

		//Right
		outPlanes[kFrustumPlaneRight].SetABCD(-otherVec[0] + tmpVec[0],
			-otherVec[1] + tmpVec[1],
			-otherVec[2] + tmpVec[2],
			-otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneRight].Normalize();

		//bottom
		otherVec[0] = finalMat.Get(1, 0); //mat21
		otherVec[1] = finalMat.Get(1, 1); //mat22
		otherVec[2] = finalMat.Get(1, 2); //mat23
		otherVec[3] = finalMat.Get(1, 3); //mat24
		outPlanes[kFrustumPlaneBottom].SetABCD(otherVec[0] + tmpVec[0],
			otherVec[1] + tmpVec[1],
			otherVec[2] + tmpVec[2],
			otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneBottom].Normalize();

		//Top
		outPlanes[kFrustumPlaneTop].SetABCD(-otherVec[0] + tmpVec[0],
			-otherVec[1] + tmpVec[1],
			-otherVec[2] + tmpVec[2],
			-otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneTop].Normalize();

		//Near
		otherVec[0] = finalMat.Get(2, 0); //mat31
		otherVec[1] = finalMat.Get(2, 1); //mat32
		otherVec[2] = finalMat.Get(2, 2); //mat33
		otherVec[2] = finalMat.Get(2, 3); //mat34

		outPlanes[kFrustumPlaneNear].SetABCD(otherVec[0] + tmpVec[0],
			otherVec[1] + tmpVec[1],
			otherVec[2] + tmpVec[2],
			otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneNear].Normalize();

		//Far
		outPlanes[kFrustumPlaneFar].SetABCD(-otherVec[0] + tmpVec[0],
			-otherVec[1] + tmpVec[1],
			-otherVec[2] + tmpVec[2],
			-otherVec[3] + tmpVec[3]);
		outPlanes[kFrustumPlaneFar].Normalize();

	}
}