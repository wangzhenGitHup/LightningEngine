#include "DrawCall.h"

namespace LightningEngine
{
	DrawCall::DrawCall() : vbo_(nullptr), 
		ebo_(nullptr), 
		material_(nullptr),
		camera_(nullptr),
		customComponenet_(nullptr),
		gameObject_(nullptr)
		
	{
		distance2Camera_ = 0.0f;
	}
	DrawCall::~DrawCall()
	{
		Clear();
	}

	void DrawCall::Clear()
	{
		DrawCall* pNext = Next<DrawCall>();
		if (nullptr != pNext)
		{
			delete pNext;
			pNext->ClearNext();
		}

		if (nullptr != material_)
		{
			ebo_ = nullptr;
			vbo_ = nullptr;
			material_ = nullptr;
			camera_ = nullptr;
		}
	}

	void DrawCall::Rendering()
	{
		if (nullptr != material_)
		{
			ForwardRendering();
		}

		if (nullptr != GetNext())
		{
			Next<DrawCall>()->Rendering();
		}
	}

	void DrawCall::RenderingOpaque()
	{
		if (nullptr != material_)
		{
			if (material->renderingQueue_ < kRenderingQueueTransparent)
			{
				ForwardRendering();
			}
		}

		if (nullptr != GetNext())
		{
			Next<DrawCall>()->RenderingOpaque();
		}
	}

	void DrawCall::RenderingTransparent()
	{
		if (nullptr != material_)
		{
			if (material_->renderingQueue_ >= kRenderingQueueTransparent) {
				ForwardRendering();
			}
		}

		if (nullptr != GetNext())
		{
			Next<DrawCall>()->RenderingTransparent();
		}
	}

	void DrawCall::SetUpDepthMap(RenderPass * pass)
	{
		GameObject* pObj = (GameObject)gameObject_;
		if (material_->bReceivedShadow && camera_->bRenderDepth)
		{
			if (nullptr != Light::s_mainLight &&
				nullptr != pObj)
			{
				if ((pObj->layer_ && Light::s_mainLight->cullingMask_) != 0)
				{
					pass->SetTextureProperty(UniformDefines::Sampler_DepthMap_NameInShader,
						camera_->depthFBO_->projection_.data_);
					pass->SetMatrix4Property(UniformDefines::Matrix_MainLightProjection_NameInShader,
						Light::s_mainLight->projection_.data_);
					pass->SetMatrix4Property(UniformDefines::Matrix_MainLightView_NameInShader,
						Light::s_mainLight->view_.data_);
					pass->SetVec4Property(UniformDefines::Vec4_MainLightPosition_NameInShader,
						Light::s_mainLight->position_.v_);
				}
			}
		}
	}

	void DrawCall::SetUpCamera(RenderPass * pass)
	{
		if (nullptr != pass->GetVec4Property(UniformDefines::Vec4_CameraWorldPos_NameInShader))
		{
			const Matrix4x4 & world_matrix = camera_->owner_->GetWorldMatrix();
			pass->SetVec4Property(UniformDefines::Vec4_CameraWorldPos_NameInShader, world_matrix.data_[12], world_matrix.data_[13], world_matrix.data_[14], 1.0f);
		}
	}

	void DrawCall::SetUpOnePassLights(RenderPass * pass)
	{
		GameObject* pObj = (GameObject*)gameObject_;
		memset(Light::s_lightEnableMask, 0, sizeof(int) * 8);
		auto iter = Light::s_lights.begin();
		auto iterEnd = Light::s_lights.end();
		int idx = 0;
		for (; iter != iterEnd; ++iter)
		{
			Light* pLight = *iter;
			if (nullptr != pLight->owner_ &&
				!pLight->owner_->IsEnabled())
			{
				continue;
			}

			if ((pLight->cullingMask_ & pObj->layer_) != 0)
			{
				Light::s_lightEnabledMask[idx] = 1;
				Light::s_lightPos[idx] = pLight->position_;
				Light::s_lightAmbient[idx] = pLight->ambientColor_;
				Light::s_lightDiffuse[idx] = pLight->diffuseColor_;
				Light::s_lightSpecular[idx] = pLight->specularColor_;
				Light::s_lightSetting[idx] = pLight->setting_;
				Light::s_lightSetting1[idx] = pLight->setting1_;
			}

			idx++;
		}

		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightPos_NameInShader, 8, Light::s_lightPos[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightAmbient_NameInShader, 8, Light::s_lightAmbient[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightDiffuse_NameInShader, 8, Light::s_lightDiffuse[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightSpecular_NameInShader, 8, Light::s_lightSpecular[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightSetting_NameInShader, 8, Light::s_lightSetting[0].v);
		pass->SetVec4ArrayProperty(UniformDefines::Vec4Array_LightSetting1_NameInShader, 8, Light::s_lightSetting1[0].v);
		pass->SetIntArrayProperty(UniformDefines::IntArray_LightEnabled_NameInShader, 8, Light::s_lightEnableMask);
	}

	void DrawCall::SetUpLight(RenderPass * pass, Light * light)
	{
		pass->SetVec4Property(UniformDefines::Vec4_LightAmbient_NameInShader,
			light->ambientColor_.v_);
		pass->SetVec4Property(UniformDefines::Vec4_LightDiffuse_NameInShader,
			light->diffuseColor_.v_);
		pass->SetVec4Property(UniformDefines::Vec4_LightSpecular_NameInShader,
			light->specularColor_.v_);
		pass->SetVec4Property(UniformDefines::Vec4_LightPos_NameInShader,
			light->position_.v_);
		pass->SetVec4Property(UniformDefines::Vec4_LightSetting_NameInShader,
			light->setting_.v_);
		pass->SetVec4Property(UniformDefines::Vec4_LightSetting1_NameInShader,
			light->setting1_.v_);
	}

	void DrawCall::RenderNoLit(RenderPass * pass)
	{
		pass->ActiveRenderState();
		pass->UpdateUniforms(camera_);
		pass->SetupStencilBuffer();
		if (ebo_ == nullptr || ebo_->mIndexCount == 0) 
		{
			vbo_->Draw(pass->renderState_.primitiveType_);
		}
		else 
		{
			ebo_->Draw(pass->renderState_.primitiveType_);
		}
		pass->RestoreStencilBuffer();
	}

	void DrawCall::RenderOnePass(RenderPass * pass)
	{
		SetUpDepthMap(pass);
		SetUpOnePassLights(pass);
		SetUpCamera(pass);
		pass->ActiveRenderState();
		pass->UpdateUniforms(camera_);
		pass->SetupStencilBuffer();

		if (ebo_ == nullptr || ebo_->indexCount_ == 0) 
		{
			vbo_->Draw(pass->renderState_.primitiveType_);
		}
		else 
		{
			ebo_->Draw(pass->renderState_.primitiveType_);
		}
		pass->RestoreStencilBuffer();
	}

	void DrawCall::RenderForwardBasePass(RenderPass * pass)
	{
		SetUpDepthMap(pass);
		GameObject* pObj = (GameObject*)gameObject_;
		if (nullptr != Light::s_mainLight &&
			(pObj->layer_ & Light::s_mainLight->cullingMask_) != 0)
		{
			if (nullptr != Light::s_mainLight->owner_ &&
				!Light::s_mainLight->owner_->IsEnabled()) 
			{
			}
			else
			{
				SetUpLight(pass, Light::s_mainLight);
			}
		}
		SetUpCamera(pass);
		pass->ActiveRenderState();
		pass->SetupStencilBuffer();
		pass->UpdateUniforms(camera_);
		if (nullptr == ebo_ || ebo_->indexCount_ == 0)
		{
			vbo_->Draw(pass->renderState_.primitiveType_);
		}
		else
		{
			ebo_->Draw(pass->renderState_.primitiveType_);
		}
		pass->RestoreStencilBuffer();
	}

	void DrawCall::RenderForwardAdditivePass(RenderPass * pass)
	{
		SetUpDepthMap(pass);
		SetUpCamera(pass);

		GameObject*pObj = (GameObject*)gameObject_;

		for (auto iter = Light::s_lights.begin(); 
			iter != Light::s_lights.end(); 
			++iter)
		{
			Light* pLight = *iter;
			if (nullptr != pLight->owner_ &&
				!pLight->owne_->IsEnabled())
			{
				continue;
			}

			if (pLight != Light::s_mainLight &&
				(pObj->layer_ & pLight->cullingMask_) != 0)
			{
				SetUpLight(pass, current_light);
				pass->ActiveRenderState();
				pass->SetupStencilBuffer();
				pass->UpdateUniforms(camera_);
				if (nullptr == ebo_ || ebo_->indexCount_ == 0)
				{
					vbo_->Draw(pass->renderState_.primitiveType_);
				}
				else 
				{
					ebo_->Draw(pass->renderState_.primitiveType_);
				}
				pass->RestoreStencilBuffer();
			}
		}
	}

	void DrawCall::ForwardRendering()
	{
		vbo_->Active();
		for (auto iter = material_->renderPasses_.begin();
			iter != material_->renderPasses_.end();
			++iter)
		{
			RenderPass* pass = *iter;
			switch (pass->mCatagory) 
			{
			case kRenderPassCatagoryNoLit:
				RenderNoLit(pass);
				break;

			case kRenderPassCatagoryOnePass:
				RenderOnePass(pass);
				break;

			case kRenderPassCatagoryForwardBase:
				RenderForwardBasePass(pass);
				break;

			case kRenderPassCatagoryForwardAdd:
				RenderForwardAdditivePass(pass);
				break;
			}
		}
	}

	void DrawCall::RenderingDepth()
	{
		if (nullptr != camera_->depthMaterial_ && 
			nullptr != vbo_)
		{
			vbo_->Active();
			RenderPass*pass = camera_->depthMaterial_->GetPass(0);
			pass->SetMatrix4Property(UniformDefines::Matrix_M_NameInShader, 
				material_->GetMatrix4Property(UniformDefines::Matrix_M_NameInShader)->value_.data_);
			pass->ActiveRenderState();
			pass->UpdateUniforms(camera_);
			if (nullptr != Light::mMainLight)
			{
				camera_->depthMaterial_->SetMatrix4Property(UniformDefines::Matrix_MainLightProjection_NameInShader,
					Light::s_mainLight->projection_.data_);

				camera_->depthMaterial_->SetMatrix4Property(UniformDefines::Matrix_MainLightView_NameInShader, 
					Light::s_mainLight->view_.data_);

				camera_->depthMateria_l->SetVec4Property(UniformDefines::Vec4_MainLightPosition_NameInShader, 
					Light::s_mainLight->position_.v_);
			}
			if (nullptr == ebo_ || ebo_->indexCount_ == 0)
			{
				vbo_->Draw(pass->renderState_.primitiveType_);
			}
			else
			{
				ebo_->Draw(pass->renderState_.primitiveType_);
			}
		}

		if (GetNext() != nullptr) 
		{
			Next<DrawCall>()->RenderingDepth();
		}
	}

	static void AppendSortedRenderQueue(DrawCall*head, DrawCall*new_node) 
	{
		DrawCall* current_node = head;
		DrawCall* prev_node = current_node;

		while (nullptr != current_node)
		{
			if (new_node->distance2Camera_ <= current_node->distance2Camera_) 
			{
				new_node->InsertAfter(prev_node);
				prev_node = nullptr;
				break;
			}

			prev_node = current_node;
			current_node = current_node->Next<DrawCall>();
		}
		if (prev_node != nullptr) 
		{
			prev_node->Append(new_node);
		}
	}

	void RenderQueue::AppendOpaqueDrawCall(DrawCall * dc)
	{
		dc->camera_ = camera_;
		if (nullptr == opaqueDrawCall_)
		{
			opaqueDrawCall_ = dc;
		}
		else
		{
			AppendSortedRenderQueue(opaqueDrawCall_, dc);
		}
	}

	void RenderQueue::AppendTransparentDrawCall(DrawCall * dc)
	{
		dc->camera_ = camera_;
		if (nullptr == transparentDrawCall_)
		{
			transparentDrawCall_ = dc;
		}
		else
		{
			AppendSortedRenderQueue(transparentDrawCall_, dc);
		}
	}

	void RenderQueue::AppendDepthDrawCall(DrawCall * dc)
	{
		dc->camera_ = camera_;
		if (nullptr == depthDrawCall_) 
		{
			depthDrawCall_ = dc;
		}
		else
		{
			AppendSortedRenderQueue(depthDrawCall_, dc);
		}
	}

	void RenderQueue::RenderOpaque()
	{
		DrawCall* current_node = opaqueDrawCall_;
		while (nullptr != current_node)
		{
			if (nullptr != current_node->material_)
			{
				current_node->ForwardRendering();
			}
			current_node = current_node->Next<DrawCall>();
		}
	}

	void RenderQueue::RenderTransparent()
	{
		DrawCall* current_node = lastTransparentDrawCall_;
		while (nullptr != current_node)
		{
			if (nullptr != current_node->material_)
			{
				current_node->ForwardRendering();
			}
			current_node = current_node->Prev<DrawCall>();
		}
	}

	void RenderQueue::RenderUI()
	{
		DrawCall* current_node = uiDrawCall_;
		while (nullptr != current_node)
		{
			if (nullptr != current_node->material_)
			{
				current_node->ForwardRendering();
			}
			current_node = current_node->Next<DrawCall>();
		}
	}

	void RenderQueue::RenderDepth()
	{
		DrawCall* current_node = depthDrawCall_;
		while (nullptr != current_node)
		{
			if (nullptr != current_node->material_)
			{
				current_node->RenderingDepth();
			}
			current_node = current_node->Next<DrawCall>();
		}
	}
}