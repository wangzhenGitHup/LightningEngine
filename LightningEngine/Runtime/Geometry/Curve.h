#pragma once

#include <map>

namespace LightningEngine
{
	class Vector3;

	class Curve
	{
	public:
		void AddKeyPoint(int idx, Vector3* keyPoint);
		void SetKeyPointData(int idx, float x, float y, float z);
		int PrevKeyPointFrameIdx(int idx);
		int NextKeyPointFrameIdx(int idx);
		int GetIndexOf(int idx);
		void ModityKeyPointKey(int original, int current);
		void DeleteFrame(int idx);
		void ReleaseKeyPoints();
		std::map<int, Vector3*>& GetKeyPoints() { return keyPoints_; }

	protected:
		std::map<int, Vector3*> keyPoints_;
		
	};
}