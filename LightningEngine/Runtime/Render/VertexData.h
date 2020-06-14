#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Math/Color.h"

namespace LightningEngine
{
	enum VertexDataType
	{
		kVertexDataTypeFull,
		kVertexDataTypeES20Full,
		kVertexDataTypeES30Full,
		kVertexDataTypeCount
	};

	struct VertexDataFull
	{
		Vector4 vertex_;
		Vector4 texcoord0_;
		Vector4 texcoord1_;
		Vector4 normal_;
		Vector4 tangent_;
	};

	//openGL ES2.0 Max Attribute Count 8
	struct VextexDataES20Full
	{
		Vector4 vertex_;
		Vector4 texcoord0_;
		Vector4 texcoord1_;
		Vector4 texcoord2_;
		Vector4 texcoord3_;
		Vector4 texcoord4_;
		Vector4 normal_;
		Vector4 tangent_;
	};

	//openGL ES3.0 Max Attribute Count 16
	struct VextexDataES30Full
	{
		Vector4 vertex_;
		Vector4 texcoord0_;
		Vector4 texcoord1_;
		Vector4 texcoord2_;
		Vector4 texcoord3_;
		Vector4 texcoord4_;
		Vector4 texcoord5_;
		Vector4 texcoord6_;
		Vector4 texcoord7_;
		Vector4 texcoord8_;
		Vector4 texcoord9_;
		Vector4 texcoord10_;
		Vector4 texcoord11_;
		Vector4 texcoord12_;
		Vector4 normal_;
		Vector4 tangent_;
	};

	class VertexData
	{
	public:
		VertexData();
		~VertexData();

		void SetBufferSize(int cnt);

		template<class T>
		T* GetBuffer()
		{
			return (T*)data_;
		}

		Int32 type_;
		Int32 count_;
		Int32 bufferSize_;
		Any data_;
	};
}