#include "impls\RTMath.h"


namespace RTRender
{
	/*!
		CMath
		*/

	const F CMath::mEpsilon = 1e-3;
	const F CMath::mInfinityF = 1e+10;
	const F CMath::mPi = 3.14159265359;
	const F CMath::mInvPi = 0.31830988618;

	F CMath::Deg2Rad(const F& degree)
	{
		return (mPi * degree) / 180.0;
	}

	bool CMath::SolveQuadricEquation(const F& a, const F& b, const F& c, F& r0, F& r1)
	{
		F d = b * b - 4.0 * a * c;

		if (d <= 0.0)
			return false;
		else if (d > 0.0)
		{
			F sqrtD = sqrt(d);

			F q;

			if (b < 0)
				q = -0.5 * (b - sqrtD);
			else
				q = -0.5 * (b + sqrtD);

			r0 = q / a;
			r1 = q / c;

			if (r0 > r1)
			{
				F t = r0;
				r0 = r1;
				r1 = t;
			}
		}

		return true;
	}

	/*!
		CVector3
		*/

	CVector3::CVector3() :
		x(0.0), y(0.0), z(0.0)
	{
	}

	CVector3::CVector3(F x, F y, F z) :
		x(x), y(y), z(z)
	{
	}

	CVector3::CVector3(const CVector3& vecRef) :
		x(vecRef.x), y(vecRef.y), z(vecRef.z)
	{
	}

	CVector3::~CVector3()
	{
	}

	CVector3 CVector3::operator- (const CVector3& vec)
	{
		return CVector3(x - vec.x, y - vec.y, z - vec.z);
	}

	CVector3 CVector3::operator- (const CNormal& n)
	{
		return CVector3(x - n.x, y - n.y, z - n.z);
	}

	CVector3 CVector3::operator+ (const CVector3& vec)
	{
		return CVector3(x + vec.x, y + vec.y, z + vec.z);
	}

	CVector3 CVector3::operator+ (const CNormal& n) const
	{
		return CVector3(x + n.x, y + n.y, z + n.z);
	}

	CVector3 CVector3::operator* (const F& scalar) const
	{
		return CVector3(x * scalar, y * scalar, z * scalar);
	}

	void CVector3::operator*= (const F& scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	bool CVector3::operator== (const CVector3& vec)
	{
		F eps = CMath::mEpsilon;

		if (abs(x - vec.x) <= eps && abs(y - vec.y) <= eps && abs(z - vec.z) <= eps)
			return true;
		else
			return false;
	}

	bool CVector3::operator!= (const CVector3& vec)
	{
		F eps = CMath::mEpsilon;

		if (abs(x - vec.x) > eps || abs(y - vec.y) > eps || abs(z - vec.z) > eps)
			return true;
		else
			return false;
	}

	CVector3 CVector3::operator- () const
	{
		return CVector3(-x, -y, -z);
	}

	void CVector3::Normalize()
	{
		F mag = 1.0 / sqrt(x * x + y * y + z * z);

		x *= mag;
		y *= mag;
		z *= mag;
	}

	void CVector3::Invert()
	{
		x *= -1.0;
		y *= -1.0;
		y *= -1.0;
	}

	F CVector3::Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	F CVector3::SquaredLength() const
	{
		return (x * x + y * y + z * z);
	}

	CVector3 CVector3::CrossProduct(const CVector3& vec)
	{
		return CVector3(y * vec.z - z * vec.y,
		                z * vec.x - x * vec.z,
		                x * vec.y - y * vec.x);
	}

	F CVector3::DotProduct(const CVector3& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	F CVector3::DotProduct(const CNormal& n) const
	{
		return x * n.x + y * n.y + z * n.z;
	}

	const CVector3 CVector3::mForward = CVector3(0.0, 0.0, 1.0);
	const CVector3 CVector3::mRight = CVector3(1.0, 0.0, 0.0);
	const CVector3 CVector3::mUp = CVector3(0.0, 1.0, 0.0);
	const CVector3 CVector3::mInfinite = CVector3(1e+10, 1e+10, 1e+10);

	/*!
		CNormal
		*/

	CNormal::CNormal() :
		x(0.0), y(0.0), z(0.0)
	{
	}

	CNormal::CNormal(F x, F y, F z) :
		x(x), y(y), z(z)
	{
		F invLength = 1.0 / sqrt(x * x + y * y + z * z);

		x *= invLength;
		y *= invLength;
		z *= invLength;
	}

	CNormal::CNormal(const CNormal& normal) :
		x(normal.x), y(normal.y), z(normal.z)
	{
	}

	CNormal::CNormal(const CVector3& vec) :
		x(vec.x), y(vec.y), z(vec.z)
	{
		F invLength = 1.0 / sqrt(x * x + y * y + z * z);

		x *= invLength;
		y *= invLength;
		z *= invLength;
	}

	CNormal::~CNormal()
	{
	}

	CNormal CNormal::operator- (const CNormal& n) const
	{
		return CNormal(x - n.x, y - n.y, z - n.z);
	}

	CNormal CNormal::operator+ (const CNormal& n) const
	{
		return CNormal(x + n.x, y + n.y, z + n.z);
	}

	CNormal CNormal::operator* (const F& scalar) const
	{
		return CNormal(x * scalar, y * scalar, z * scalar);
	}

	bool CNormal::operator== (const CNormal& n) const
	{
		F eps = CMath::mEpsilon;

		if (abs(x - n.x) <= eps && abs(y - n.y) <= eps && abs(z - n.z) <= eps)
			return true;
		else
			return false;
	}

	bool CNormal::operator!= (const CNormal& n) const
	{
		F eps = CMath::mEpsilon;

		if (abs(x - n.x) > eps || abs(y - n.y) > eps || abs(z - n.z) > eps)
			return true;
		else
			return false;
	}

	F CNormal::Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	F CNormal::SquaredLength() const
	{
		return (x * x + y * y + z * z);
	}

	/*!
		CPoint
	*/

	CPoint::CPoint() :
		x(0.0), y(0.0), z(0.0)
	{
	}

	CPoint::CPoint(F x, F y, F z) :
		x(x), y(y), z(z)
	{
	}

	CPoint::CPoint(const CPoint& point) :
		x(point.x), y(point.y), z(point.z)
	{
	}

	CPoint::~CPoint()
	{
	}

	CPoint CPoint::operator+ (const CVector3& vec) const
	{
		return CPoint(x + vec.x, y + vec.y, z + vec.z);
	}

	CVector3 CPoint::operator- (const CPoint& p) const
	{
		return CVector3(x - p.x, y - p.y, z - p.z);
	}

	CPoint CPoint::operator- (const CVector3& vec) const
	{
		return CPoint(x - vec.x, y - vec.y, z - vec.z);
	}

	F CPoint::operator() (const unsigned char& axisIndex) const
	{
		if (axisIndex == 0)
			return x;
		else if (axisIndex == 1)
			return y;
		else
			return z;
	}

	F CPoint::DistanceTo(const CPoint& p) const
	{
		return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z));
	}

	F CPoint::SquaredDistanceTo(const CPoint& p) const
	{
		return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z);
	}

	/*!
		CRay
	*/

	CRay::CRay() :
		origin(), dir()
	{
	}

	CRay::CRay(const CPoint& origin, const CVector3& direction) :
		origin(origin), dir(direction)
	{
	}

	CRay::CRay(const CRay& rayRef) :
		origin(rayRef.origin), dir(rayRef.dir)
	{
	}

	CRay::~CRay()
	{
	}

	/*!
		CMatrix3x3
	*/

	CMatrix3x3::CMatrix3x3()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m[i][j] = 0.0;
	}

	CMatrix3x3::CMatrix3x3(const CMatrix3x3& mat)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m[i][j] = mat.m[i][j];
	}

	CMatrix3x3::CMatrix3x3(F m00, F m01, F m02,
						   F m10, F m11, F m12,
		                   F m20, F m21, F m22)					
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
	}

	CMatrix3x3::~CMatrix3x3()
	{
	}

	F CMatrix3x3::operator() (const int& i, const int& j) const
	{
		if (i >= 0 && i < 3 && j >= 0 && j < 3)
			return m[i][j];

		return CMath::mInfinityF;
	}

	F CMatrix3x3::Det() const
	{
		return	m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
				m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]) +
				m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);
	}

	void CMatrix3x3::Transpose()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = i; j < 3; j++)
			{
				F hold = m[i][j];
				m[i][j] = m[j][i];
				m[j][i] = hold;
			}
		}
	}

	void CMatrix3x3::Inverse()
	{
		F det = (m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
				 m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]) +
			     m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]));

		if (det < CMath::mEpsilon)
			return;

		F invDet = 1.0 / det;

		F umTransp[3][3];

	//	umTransp[0][0] 
	}

	CMatrix3x3 CMatrix3x3::operator* (const F& value)
	{
		CMatrix3x3 multiplyResult;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				multiplyResult.m[i][j] *= value;

		return multiplyResult;
	}

	CMatrix3x3 CMatrix3x3::operator* (const CMatrix3x3& mat)
	{
		CMatrix3x3 multiplyResult;

		multiplyResult.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0];
		multiplyResult.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][2];
		multiplyResult.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2];

		multiplyResult.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0];
		multiplyResult.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][2];
		multiplyResult.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2];

		multiplyResult.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0];
		multiplyResult.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][2];
		multiplyResult.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2];

		return multiplyResult;
	}

	CVector3 CMatrix3x3::operator() (const CVector3& vec)
	{
		CVector3 multiplyResult;

		multiplyResult.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z;
		multiplyResult.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z;
		multiplyResult.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z;

		return multiplyResult;
	}

	const CMatrix3x3 CMatrix3x3::mIdentity = CMatrix3x3(1.0, 0.0, 0.0,
													    0.0, 1.0, 0.0,
														0.0, 0.0, 1.0);

	/*!
		CMatrix4x4
	*/

	CMatrix4x4::CMatrix4x4() 
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = 0.0;
	}

	CMatrix4x4::CMatrix4x4(const CMatrix4x4& mat)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = mat.m[i][j];
	}

	CMatrix4x4::CMatrix4x4(F m00, F m01, F m02, F m03,
		                   F m10, F m11, F m12, F m13,
		                   F m20, F m21, F m22, F m23,
		                   F m30, F m31, F m32, F m33)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	CMatrix4x4::~CMatrix4x4()
	{
	}

	void CMatrix4x4::Transpose()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = i; j < 4; j++)
			{
				F hold = m[i][j];
				m[i][j] = m[j][i];
				m[j][i] = hold;
			}
		}
	}

	//there are some scary things below!  

	void CMatrix4x4::Inverse()
	{
		F det = m[0][0] * (m[1][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) -
			               m[1][2] * (m[2][1] * m[3][3] - m[3][1] * m[1][3]) +
			               m[1][3] * (m[2][1] * m[3][2] - m[3][1] * m[3][2])) -
			    m[0][1] * (m[1][0] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) -
			               m[1][2] * (m[2][0] * m[3][0] - m[3][0] * m[2][3]) +
			               m[1][3] * (m[2][0] * m[3][2] - m[3][0] * m[2][2])) +
			    m[0][2] * (m[1][0] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) -
			               m[1][1] * (m[2][0] * m[3][0] - m[3][0] * m[2][3]) +
			               m[1][3] * (m[2][0] * m[3][1] - m[3][0] * m[2][1])) -
			    m[0][3] * (m[1][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]) -
			               m[1][1] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]) +
			               m[1][2] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]));

		if (det  < CMath::mEpsilon) // there is no an inverse matrix 
			return;

		F invDet = 1.0 / det;

		F u[4][4];

		u[0][0] = m[1][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) -
			      m[1][2] * (m[2][1] * m[3][3] - m[3][1] * m[1][3]) +
			      m[1][3] * (m[2][1] * m[3][2] - m[3][1] * m[3][2]);

		u[0][1] = -(m[1][0] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) -
			        m[1][2] * (m[2][0] * m[3][0] - m[3][0] * m[2][3]) +
			        m[1][3] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]));

		u[0][2] = m[1][0] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) -
				  m[1][1] * (m[2][0] * m[3][0] - m[3][0] * m[2][3]) +
				  m[1][3] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]);

		u[0][3] = -(m[1][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]) -
			        m[1][1] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]) +
			        m[1][2] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]));

		u[1][0] = -(m[0][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) -
			        m[0][2] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) +
			        m[0][3] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]));

		u[1][1] = m[0][0] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) -
			      m[0][2] * (m[2][0] * m[3][3] - m[3][0] * m[2][3]) +
			      m[0][3] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]);

		u[1][2] = -(m[0][0] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) -
			        m[0][1] * (m[2][0] * m[3][3] - m[3][0] * m[2][3]) +
			        m[0][3] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]));

		u[1][3] = m[0][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]) -
			      m[0][1] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]) +
			      m[0][2] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]);

		u[2][0] = m[0][1] * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) -
			      m[0][2] * (m[1][1] * m[3][3] - m[3][1] * m[1][3]) +
			      m[0][3] * (m[1][1] * m[3][2] - m[3][1] * m[1][2]);

		u[2][1] = -(m[0][0] * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) -
			        m[0][2] * (m[1][0] * m[3][3] - m[3][0] * m[1][3]) +
			        m[0][3] * (m[1][0] * m[3][2] - m[3][0] * m[1][2]));

		u[2][2] = m[0][0] * (m[1][1] * m[3][3] - m[3][1] * m[1][3]) -
			      m[0][1] * (m[1][0] * m[3][3] - m[3][0] * m[1][3]) +
			      m[0][3] * (m[1][0] * m[3][1] - m[3][0] * m[1][1]);

		u[2][3] = -(m[0][0] * (m[1][1] * m[3][2] - m[3][1] * m[1][2]) -
			        m[0][1] * (m[1][0] * m[3][2] - m[3][0] * m[1][2]) +
			        m[0][2] * (m[1][0] * m[3][1] - m[3][0] * m[1][1]));

		u[3][0] = -(m[0][1] * (m[1][2] * m[2][3] - m[2][2] * m[1][3]) -
			        m[0][2] * (m[1][1] * m[2][3] - m[2][1] * m[1][3]) +
			        m[0][3] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]));

		u[3][1] = m[0][0] * (m[1][2] * m[2][3] - m[2][2] * m[1][3]) -
			      m[0][2] * (m[1][0] * m[2][3] - m[2][0] * m[1][3]) +
			      m[0][3] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]);

		u[3][2] = -(m[0][0] * (m[1][1] * m[2][3] - m[2][1] * m[1][3]) -
			        m[0][1] * (m[1][0] * m[2][3] - m[2][0] * m[1][3]) +
			        m[0][3] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]));

		u[3][3] = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
			      m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]) +
			      m[0][3] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[j][i] = u[i][j] * invDet;
	}

	F CMatrix4x4::Det() const
	{
		return m[0][0] * (m[1][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) -
			              m[1][2] * (m[2][1] * m[3][3] - m[3][1] * m[1][3]) +
			              m[1][3] * (m[2][1] * m[3][2] - m[3][1] * m[3][2])) -
			   m[0][1] * (m[1][0] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) -
			              m[1][2] * (m[2][0] * m[3][0] - m[3][0] * m[2][3]) +
			              m[1][3] * (m[2][0] * m[3][2] - m[3][0] * m[2][2])) +
			   m[0][2] * (m[1][0] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) -
			              m[1][1] * (m[2][0] * m[3][0] - m[3][0] * m[2][3]) +
			              m[1][3] * (m[2][0] * m[3][1] - m[3][0] * m[2][1])) -
			   m[0][3] * (m[1][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]) -
			              m[1][1] * (m[2][0] * m[3][2] - m[3][0] * m[2][2]) +
			              m[1][2] * (m[2][0] * m[3][1] - m[3][0] * m[2][1]));
	}

	F CMatrix4x4::operator() (const int& i, const int& j) const
	{
		if (i >= 0 && i < 4 && j >= 0 && j < 4)
			return m[i][j];

		return CMath::mInfinityF;
	}

	CMatrix4x4 CMatrix4x4::operator* (const CMatrix4x4& mat) const
	{
		CMatrix4x4 multiplyResult;

		multiplyResult.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
		multiplyResult.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
		multiplyResult.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
		multiplyResult.m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];

		multiplyResult.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
		multiplyResult.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
		multiplyResult.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
		multiplyResult.m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];

		multiplyResult.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
		multiplyResult.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
		multiplyResult.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
		multiplyResult.m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];

		multiplyResult.m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
		multiplyResult.m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
		multiplyResult.m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
		multiplyResult.m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];

		return multiplyResult;
	}

	CMatrix4x4 CMatrix4x4::operator* (const F& value)
	{
		CMatrix4x4 multiplyResult;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				multiplyResult.m[i][j] *= value;

		return multiplyResult;
	}

	const CMatrix4x4 CMatrix4x4::mIdentity = CMatrix4x4(1.0, 0.0, 0.0, 0.0,
													    0.0, 1.0, 0.0, 0.0,
													    0.0, 0.0, 1.0, 0.0,
													    0.0, 0.0, 0.0, 1.0);
}