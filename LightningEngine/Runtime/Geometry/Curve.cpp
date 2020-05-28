#include "Curve.h"
#include "Math/Vector3.h"

namespace LightningEngine
{
	void Curve::AddKeyPoint(int idx, Vector3 * keyPoint)
	{
		if (keyPoints_.find(idx) == keyPoints_.end())
		{
			keyPoints_.insert(std::pair<int, Vector3*>(idx, keyPoint));
		}
	}

	void Curve::SetKeyPointData(int idx, float x, float y, float z)
	{
		std::map<int, Vector3*>::iterator iter = keyPoints_.find(idx);
		if (iter != keyPoints_.end())
		{
			Vector3* vecPoint = iter->second;
			vecPoint->Set(x, y, z);
		}
	}

	int Curve::PrevKeyPointFrameIdx(int idx)
	{
		int prev = 0;
		std::map<int, Vector3*>::iterator iter = keyPoints_.begin();
		for (; iter != keyPoints_.end(); ++iter)
		{
			if (iter->first >= idx)
			{
				return prev;
			}

			prev = iter->first;
		}

		return prev;
	}

	int Curve::NextKeyPointFrameIdx(int idx)
	{
		std::map<int, Vector3*>::iterator iter = keyPoints_.begin();
		for (; iter != keyPoints_.end(); ++iter)
		{
			if (iter->first == idx)
			{
				++iter;

				if (iter != keyPoints_.end())
				{
					return iter->first;
				}

				return -1;
			}
		}
		return -1;
	}

	int Curve::GetIndexOf(int idx)
	{
		int ret = 0;
		std::map<int, Vector3*>::iterator iter = keyPoints_.begin();
		for (; iter != keyPoints_.end(); ++iter)
		{
			if (iter->first == idx)
			{
				break;
			}

			ret++;
		}
		return ret;
	}

	void Curve::ModityKeyPointKey(int original, int current)
	{
		std::map<int, Vector3*>::iterator iter = keyPoints_.find(original);
		if (iter != keyPoints_.end())
		{
			Vector3* vecPoint = iter->second;
			keyPoints_.erase(iter);
			AddKeyPoint(current, vecPoint);
		}
	}

	void Curve::DeleteFrame(int idx)
	{
		std::map<int, Vector3*>::iterator iter = keyPoints_.find(idx);
		if (iter != keyPoints_.end())
		{
			keyPoints_.erase(iter);
		}
	}

	void Curve::ReleaseKeyPoints()
	{
		for (std::map<int, Vector3*>::iterator iter = keyPoints_.begin(); 
			iter != keyPoints_.end(); ++iter)
		{
			delete iter->second;
		}

		keyPoints_.clear();
	}
}