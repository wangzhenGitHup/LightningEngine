#pragma once

#include "Utils/SmartPtr.h"
#include "Utils/LinkedList.h"
#include "LightningEngineGL.h"
#include "GL20/VBO.h"
#include "GL20/EBO.h"

namespace LightningEngine
{
	class Camera;
	class CustomComponent;
	class RenderPass;
	class GameObject;
	class Material;
	class Light;
	
	class Front2BackDoubleLinkedList : public DoubleLinkedList
	{
	public:
		float distance2Camera_;
	};

	class DrawCall : public Front2BackDoubleLinkedList
	{
	public:
		DrawCall();
		~DrawCall();
		void Clear();
		void Rendering();
		void RenderingOpaque();
		void RenderingTransparent();
		void SetUpDepthMap(RenderPass* pass);
		void SetUpCamera(RenderPass* pass);
		void SetUpOnePassLights(RenderPass* pass);
		void SetUpLight(RenderPass* pass, Light* light);
		void RenderNoLit(RenderPass* pass);
		void RenderOnePass(RenderPass* pass);
		void RenderForwardBasePass(RenderPass* pass);
		void RenderForwardAdditivePass(RenderPass* pass);
		void ForwardRendering();
		void RenderingDepth();

	public:
		Material* material_;
		Camera camera_;
		VBO* vbo_;
		EBO* ebo_;
		GameObject gameObject_;
		CustomComponent customComponenet_;
	};

	class RenderQueue
	{
	public:
		RenderQueue(Camera* camera) : camera_(camera),
			opaqueDrawCall_(nullptr), 
			transparentDrawCall_(nullptr), 
			lastTransparentDrawCall_(nullptr), 
			uiDrawCall_(nullptr), 
			lastUIDDrawCall_(nullptr), 
			depthDrawCall_(nullptr){}
		
		void AppendOpaqueDrawCall(DrawCall* dc);
		void AppendTransparentDrawCall(DrawCall* dc);
		//void AppendUIDrawCall(DrawCall* dc);
		void AppendDepthDrawCall(DrawCall* dc);
		void RenderOpaque();
		void RenderTransparent();
		void RenderUI();
		void RenderDepth();

	public:
		Camera* camera_;
		DrawCall* opaqueDrawCall_;
		DrawCall* transparentDrawCall_;
		DrawCall* lastTransparentDrawCall_;
		DrawCall* uiDrawCall_;
		DrawCall* lastUIDDrawCall_;
		DrawCall* depthDrawCall_;

	};
}