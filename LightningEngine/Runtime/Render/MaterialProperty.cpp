#include "MaterialProperty.h"
#include "Allocator/TLSFAllocator.h"


namespace LightningEngine
{
	MaterialProperty * TextureUnitProperty::Clone()
	{
		TextureUnitProperty* pRet = new(kMemMaterialId)TextureUnitProperty(value_);
		pRet->name_ = name_.text_;
		pRet->location_ = location_;
		pRet->valueType_ = valueType_;
		pRet->value_ = value_;
		pRet->slot_ = slot_;
		return pRet;
	}

	MaterialProperty * UniformFloatProperty::Clone()
	{
		UniformFloatProperty* pRet = new(kMemMaterialId)UniformFloatProperty;
		pRet->location_ = location_;
		pRet->valueType_ = valueType_;
		pRet->value_ = value_;
		return pRet;
	}

	MaterialProperty * UniformVector2Property::Clone()
	{
		UniformVector2Property* pRet = new(kMemMaterialId)UniformVector2Property;
		pRet->location_ = location_;
		pRet->valueType_ = valueType_;
		pRet->value_ = value_;
		return pRet;
	}

	MaterialProperty * UniformVector3Property::Clone()
	{
		UniformVector3Property* pRet = new(kMemMaterialId)UniformVector3Property;
		pRet->location_ = location_;
		pRet->valueType_ = valueType_;
		pRet->value_ = value_;
		return pRet;
	}

	MaterialProperty * UniformVector4Property::Clone()
	{
		UniformVector4Property* pRet = new(kMemMaterialId)UniformVector4Property;
		pRet->location_ = location_;
		pRet->valueType_ = valueType_;
		pRet->value_ = value_;
		return pRet;
	}

	MaterialProperty * UniformIntArrayProperty::Clone()
	{
		UniformIntArrayProperty* pRet = new(kMemMaterialId)UniformIntArrayProperty;
		pRet->location_ = location_;
		pRet->valueType_ = valueType_;
		pRet->value_ = value_;
		return pRet;
	}

	MaterialProperty * UniformVector4ArrayProperty::Clone()
	{
		UniformVector4ArrayProperty* pRet = new(kMemMaterialId)UniformVector4ArrayProperty;
		pRet->location_ = location_;
		pRet->valueType_ = valueType_;
		pRet->value_ = value_;
		pRet->size_ = size_;
		return pRet;
	}

	MaterialProperty * UniformMatri4x4Property::Clone()
	{
		UniformMatri4x4Property* pRet = new(kMemMaterialId)UniformMatri4x4Property;
		pRet->location_ = location_;
		pRet->valueType_ = valueType_;
		pRet->value_ = value_;
		return pRet;
	}

	MaterialProperty * UniformMatri3x3Property::Clone()
	{
		UniformMatri3x3Property* pRet = new(kMemMaterialId)UniformMatri3x3Property;
		pRet->location_ = location_;
		pRet->valueType_ = valueType_;
		memcpy(pRet->value_.data_, value_.data_, sizeof(float) * 16);
		return pRet;
	}
}