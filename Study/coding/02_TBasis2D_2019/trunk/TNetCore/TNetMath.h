#pragma once
#include <math.h>

namespace TBASIS_SERVER_LIB
{
	static const float T_EPSILON = 0.001f;

	struct float2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};
	struct TPoint : float2
	{
		TPoint() = default;
		TPoint(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		float* get()
		{
			return v;
		}
		void operator = (float* arr)
		{
			x = arr[0];
			y = arr[1];
		}
		float& operator [] (int index)
		{
			if (index == 0) return x;
			return y;			
		}
		bool operator == (TPoint& p)
		{
			if (fabs(x - p.x) < T_EPSILON)
			{
				if (fabs(y - p.y) < T_EPSILON)
				{
					return true;
				}
			}
			return false;
		}
		bool operator != (TPoint& p)
		{
			if (fabs(x - p.x) < T_EPSILON)
			{
				if (fabs(y - p.y) < T_EPSILON)
				{
					return false;
				}
			}
			return true;
		}
		TPoint operator * (float fValue)
		{
			TPoint ret;
			ret.x = x * fValue;
			ret.y = y * fValue;
			return ret;
		}
		TPoint operator + (TPoint& p)
		{
			TPoint ret;
			ret.x = x + p.x;
			ret.y = y + p.y;
			return ret;
		}
		TPoint operator + (TPoint p)
		{
			TPoint ret;
			ret.x = x + p.x;
			ret.y = y + p.y;
			return ret;
		}
		TPoint operator - (TPoint& p)
		{
			TPoint ret;
			ret.x = x - p.x;
			ret.y = y - p.y;
			return ret;
		}
		float Length()
		{
			return sqrt(x*x + y * y);
		}

		TPoint Normalize()
		{
			float fD = Length();
			if (fD != 0.0f)
			{
				x /= fD;
				y /= fD;
			}
			return TPoint(x, y);
		}
	};
}