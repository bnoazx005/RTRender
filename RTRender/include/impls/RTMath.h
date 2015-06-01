#ifndef CVECTOR3_H
#define CVECTOR3_H


#include <math.h>
#include "RTTypes.h"


namespace RTRender
{
	class CVector3;
	class CNormal;
	class CPoint;
	class CRay;
	class CMatrix3x3;
	class CMatrix4x4;


	class CMath
	{
		public:
			static const F mEpsilon;
			static const F mInfinityF;
			static const F mPi;
			static const F mInvPi;
		public:
			template<class T>
				static T Min(T a, T b)
				{
					return (a < b) ? a : b;
				}

			static inline F Minf(F a, F b)
			{
				return (a + mEpsilon < b) ? a : b;
			}

			static inline F Maxf(F a, F b)
			{
				return (a + mEpsilon > b) ? a : b;
			}

			static F Deg2Rad(const F& degree);
			static bool SolveQuadricEquation(const F& a, const F& b, const F& c, F& r0, F& r1);
	};

	class CVector3
	{
		public:
			CVector3();
			CVector3(F x, F y, F z);
			CVector3(const CVector3& vecRef);
			~CVector3();

			CVector3 operator- (const CVector3& vec);
			CVector3 operator- (const CNormal& n);
			CVector3 operator- () const;
			CVector3 operator+ (const CVector3& vec);
			CVector3 operator+ (const CNormal& n) const;
			CVector3 operator* (const F& scalar) const;
			
			void operator*= (const F& scalar);

			bool operator== (const CVector3& vec);
			bool operator!= (const CVector3& vec);

			void Normalize();
			void Invert();

			F Length() const;		
			F SquaredLength() const;

			CVector3 CrossProduct(const CVector3& vec);
			F DotProduct(const CVector3& vec) const;
			F DotProduct(const CNormal& n) const;
		public:
			F x, y, z;

			static const CVector3 mUp;
			static const CVector3 mRight;
			static const CVector3 mForward;
			static const CVector3 mInfinite;
	};

	class CNormal
	{
		public:
			CNormal();
			CNormal(F x, F y, F z);
			CNormal(const CNormal& normal);
			explicit CNormal(const CVector3& vec);
			~CNormal();

			CNormal operator- (const CNormal& n) const;
			CNormal operator+ (const CNormal& n) const;
			CNormal operator* (const F& scalar) const;

			bool operator== (const CNormal& n) const;
			bool operator!= (const CNormal& n) const;
			
			F Length() const;
			F SquaredLength() const;
		public:
			F x, y, z;
	};

	class CPoint
	{
		public:
			CPoint();
			CPoint(F x, F y, F z);
			CPoint(const CPoint& point);
			~CPoint();

			CPoint operator+ (const CVector3& vec) const;
			CVector3 operator- (const CPoint& p) const;
			CPoint operator- (const CVector3& vec) const;

			F operator() (const unsigned char& axisIndex) const;

			F DistanceTo(const CPoint& p) const;
			F SquaredDistanceTo(const CPoint& p) const;
		public:
			F x, y, z;
	};

	class CRay
	{
		public:
			CRay();
			CRay(const CPoint& origin, const CVector3& direction);
			CRay(const CRay& rayRef);
			~CRay();
		public:
			CPoint origin;
			CVector3 dir;
	};

	class CMatrix3x3
	{
		public:
			CMatrix3x3();

			CMatrix3x3(const CMatrix3x3& mat);

			CMatrix3x3(F m00, F m01, F m02,
					   F m10, F m11, F m12,
					   F m20, F m21, F m22);

			CMatrix3x3(const CVector3& v0, const CVector3& v1, const CVector3& v2);

			~CMatrix3x3();

			void Transpose();

			void Inverse();

			F Det() const;

			F operator() (const int& i, const int& j) const;
			CMatrix3x3 operator* (const CMatrix3x3& mat);
			CVector3 operator() (const CVector3& vec);
			CMatrix3x3 operator* (const F& value);
		public:
			static const CMatrix3x3 mIdentity;
		private:
			F m[3][3];
	};

	class CMatrix4x4
	{
		public:
			CMatrix4x4();

			CMatrix4x4(const CMatrix4x4& mat);

			CMatrix4x4(F m00, F m01, F m02, F m03,
				       F m10, F m11, F m12, F m13,
				       F m20, F m21, F m22, F m23,
					   F m30, F m31, F m32, F m33);

			~CMatrix4x4();

			void Transpose();

			void Inverse();

			F Det() const;

			F operator() (const int& i, const int& j) const;
			CMatrix4x4 operator* (const CMatrix4x4& mat) const;
			CMatrix4x4 operator* (const F& value);
		public:
			static const CMatrix4x4 mIdentity;
		public:
			F m[4][4];
	};
}

#endif