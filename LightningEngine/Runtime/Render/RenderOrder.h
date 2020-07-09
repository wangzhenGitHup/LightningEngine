#pragma once

#include "Base/Component.h"

namespace LightningEngine
{
	class RenderUnit : public DoubleLinkedList
	{
	public:
		GameObject* gameObject_;
		int index_;
		int offset_;
	};

	class RenderOrder : public Component
	{
	public:
		RenderOrder();
		virtual ~RenderOrder();
		virtual void Update(float deltaTime);
		virtual void Render(RenderQueue* render);
		DECLEAR_ENGINE_CLASS(RenderOrder)

	public:
		RenderUnit* renderUnit_;

	};
}