#pragma once

#include"RuntimePrefix.h"
#include "LightningEngineGL.h"
#include "Math/Vector2.h"
#include "Math/Vector4.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "Utils/SmartPtr.h"
#include "GL20/Texture2D.h"

namespace LightningEngine
{
	enum UniformType
	{
		kUniformTypeMatri3x3 = 1,
		kUniformTypeMatri4x4,
		kUniformTypeSampler2D,
		kUniformTypeSamplerCube,
		kUniformTypeFloat,
		kUniformTypeIntArray,
		kUniformTypeVec2,
		kUniformTypeVec3,
		kUniformTypeVec4,
		kUniformTypeVector4Array,
		kUniformTypeCount
	};

	class MaterialProperty
	{
	public:
		MaterialProperty(UniformType type) : valueType_(type), 
			location_(-1), 
			name_(64){}

		virtual ~MaterialProperty() {}
		virtual MaterialProperty* Clone() = 0;

	public:
		UniformType valueType_;
		GLint location_;
		FixedString name_;

	};


	class TextureUnitProperty : public MaterialProperty
	{
	public:
		TextureUnitProperty(UniformType type) : 
			MaterialProperty(type),
			slot_(-1), value_(nullptr)
		{}

		virtual MaterialProperty* Clone();

	public:
		GLint slot_;
		SmartPtr<TextureUnit> value_;
	};


	class UniformFloatProperty : public MaterialProperty
	{
	public:
		UniformFloatProperty(UniformType type = kUniformTypeFloat) : 
			MaterialProperty(type),
			value_(0.0f) {}

		virtual MaterialProperty* Clone();

	public:
		float value_;
	};


	class UniformVector2Property : public MaterialProperty
	{
	public:
		UniformVector2Property(UniformType type = kUniformTypeVec2) : 
			MaterialProperty(type),
			value_(0.0f, 0.f) {}

		virtual MaterialProperty* Clone();

	public:
		Vector2 value_;
	};


	class UniformVector3Property : public MaterialProperty
	{
	public:
		UniformVector3Property(UniformType type = kUniformTypeVec3) :
			MaterialProperty(type),
			value_(0.0f, 0.f, 0.0f) {}

		virtual MaterialProperty* Clone();

	public:
		Vector3 value_;
	};


	class UniformVector4Property : public MaterialProperty
	{
	public:
		UniformVector4Property(UniformType type = kUniformTypeVec4) : 
			MaterialProperty(type),
			value_(0.0f, 0.f, 0.0f, 0.0f) {}

		virtual MaterialProperty* Clone();

	public:
		Vector4 value_;
	};


	class UniformIntArrayProperty : public MaterialProperty
	{
	public:
		UniformIntArrayProperty(UniformType type = kUniformTypeIntArray) : 
			MaterialProperty(type),
			value_(nullptr), size_(0) {}

		virtual MaterialProperty* Clone();

	public:
		int size_;
		int* value_;
	};


	class UniformVector4ArrayProperty : public MaterialProperty
	{
	public:
		UniformVector4ArrayProperty(UniformType type = kUniformTypeVector4Array) :
			MaterialProperty(type),
			value_(nullptr), size_(0) {}

		virtual MaterialProperty* Clone();

	public:
		int size_;
		float* value_;
	};


	class UniformMatri4x4Property : public MaterialProperty
	{
	public:
		UniformMatri4x4Property(UniformType type = kUniformTypeMatri4x4) :
			MaterialProperty(type) {}

		virtual MaterialProperty* Clone();

	public:
		Matrix4x4 value_;
	};


	class UniformMatri3x3Property : public MaterialProperty
	{
	public:
		UniformMatri3x3Property(UniformType type = kUniformTypeMatri3x3) :
			MaterialProperty(type) {}

		virtual MaterialProperty* Clone();

	public:
		Matrix3x3 value_;
	};

	struct AttributeDefine
	{
		Int32 attributeSlot;
		Int32 dataOffset;
		GLenum dataType;
		UInt8 unitCount_;
		GLboolean normalized;
		UInt16 dataStride;

		AttributeDefine() : attributeSlot(-1), dataOffset(0), normalized(GL_FALSE) {}
	};
}