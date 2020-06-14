#include "VertexData.h"

namespace LightningEngine
{
	static const Int8 TypeVertexData3D = sizeof(VertexDataFull);

	VertexData::VertexData() : 
		data_(nullptr), 
		count_(0), 
		bufferSize_(0),
		type_(TypeVertexData3D) 
	{}

	VertexData::~VertexData()
	{
		if (nullptr != data_)
		{
			delete[]data_;
		}
	}

	void VertexData::SetBufferSize(int cnt)
	{
		count_ = cnt;
		bufferSize_ = count_ * type_;
		data_ = new VertexDataFull[cnt];
	}
}