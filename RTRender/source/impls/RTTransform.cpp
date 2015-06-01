#include "impls\RTTransform.h"


namespace RTRender
{

	CTransform::CTransform():
		mTransform(CMatrix4x4::mIdentity), mInvTransform(CMatrix4x4::mIdentity)
	{
	}

	CTransform::CTransform(const CMatrix4x4& mat) :
		mTransform(mat), mInvTransform(mat)
	{
		mInvTransform.Inverse();
	}

	CTransform::CTransform(const CMatrix4x4& mat, const CMatrix4x4& invMat) :
		mTransform(mat), mInvTransform(invMat)
	{
	}

	CTransform::~CTransform()
	{
	}

	void CTransform::Inverse()
	{
		CMatrix4x4 tmp(mTransform);

		mTransform = mInvTransform;
		mInvTransform = tmp;
	}

	CTransform CTransform::Translate(const CVector3& translationVec)
	{
		CMatrix4x4 T(1.0, 0.0, 0.0, translationVec.x,
					 0.0, 1.0, 0.0, translationVec.y,
			         0.0, 0.0, 1.0, translationVec.z,
			         0.0, 0.0, 0.0,              1.0);

		CMatrix4x4 invT(1.0, 0.0, 0.0, -translationVec.x,
			            0.0, 1.0, 0.0, -translationVec.y,
			            0.0, 0.0, 1.0, -translationVec.z,
			            0.0, 0.0, 0.0, 1.0);

		return CTransform(T, invT);
	}

	CTransform CTransform::Scale(const CVector3& scalingVec)
	{
		CMatrix4x4 S(scalingVec.x, 0.0, 0.0, 0.0,
			         0.0, scalingVec.y, 0.0, 0.0,
			         0.0, 0.0, scalingVec.z, 0.0,
			         0.0, 0.0, 0.0,          1.0);

		CMatrix4x4 invS(1.0 / scalingVec.x, 0.0, 0.0, 0.0,
			            0.0, 1.0 / scalingVec.y, 0.0, 0.0,
			            0.0, 0.0, 1.0 / scalingVec.z, 0.0,
			            0.0, 0.0, 0.0,                1.0);

		return CTransform(S, invS);
	}

	CTransform CTransform::RotateX(const F& angle)
	{
		F sinx = sin(CMath::Deg2Rad(angle));
		F cosx = cos(CMath::Deg2Rad(angle));

		CMatrix4x4 RX(1.0, 0.0,  0.0,   0.0,
			          0.0, cosx, -sinx, 0.0,
					  0.0, sinx, cosx,  0.0,
					  0.0, 0.0,  0.0,   1.0);
		
		CMatrix4x4 invRX(RX);
		invRX.Transpose();

		return CTransform(RX, invRX);
	}

	CTransform CTransform::RotateY(const F& angle)
	{
		F sinx = sin(CMath::Deg2Rad(angle));
		F cosx = cos(CMath::Deg2Rad(angle));

		CMatrix4x4 RY(cosx, 0.0,  sinx, 0.0,
			          0.0,  1.0,  0.0,  0.0,
			          -sinx, 0.0, cosx, 0.0,
			          0.0,   0.0, 0.0,  1.0);

		CMatrix4x4 invRY(RY);
		invRY.Transpose();

		return CTransform(RY, invRY);
	}

	CTransform CTransform::RotateZ(const F& angle)
	{
		F sinx = sin(CMath::Deg2Rad(angle));
		F cosx = cos(CMath::Deg2Rad(angle));

		CMatrix4x4 RZ(cosx, -sinx, 0.0, 0.0,
					  sinx, cosx,  0.0, 0.0,
					  0.0,  0.0,   1.0, 0.0,
					  0.0,  0.0,   0.0, 1.0);

		CMatrix4x4 invRZ(RZ);
		invRZ.Transpose();

		return CTransform(RZ, invRZ);
	}

	CPoint CTransform::operator() (const CPoint& point) const
	{
		F x = point.x, y = point.y, z = point.z;

		F xp = mTransform.m[0][0] * x + mTransform.m[0][1] * y + mTransform.m[0][2] * z + mTransform.m[0][3];
		F yp = mTransform.m[1][0] * x + mTransform.m[1][1] * y + mTransform.m[1][2] * z + mTransform.m[1][3];
		F zp = mTransform.m[2][0] * x + mTransform.m[2][1] * y + mTransform.m[2][2] * z + mTransform.m[2][3];
		F wp = mTransform.m[3][0] * x + mTransform.m[3][1] * y + mTransform.m[3][2] * z + mTransform.m[3][3];

		if (wp == 1.0)
			return CPoint(xp, yp, zp);

		F invW = 1.0 / wp;

		return CPoint(xp * invW, yp * invW, zp * invW);
	}

	CVector3 CTransform::operator() (const CVector3& vec) const
	{
		F x = vec.x, y = vec.y, z = vec.z;

		return CVector3(mTransform.m[0][0] * x + mTransform.m[0][1] * y + mTransform.m[0][2] * z,
			            mTransform.m[1][0] * x + mTransform.m[1][1] * y + mTransform.m[1][2] * z,
			            mTransform.m[2][0] * x + mTransform.m[2][1] * y + mTransform.m[2][2] * z);
	}

	CNormal CTransform::operator() (const CNormal& n) const
	{
		F x = n.x, y = n.y, z = n.z;

		return CNormal(mInvTransform.m[0][0] * x + mInvTransform.m[1][0] * y + mInvTransform.m[2][0] * z,
			           mInvTransform.m[0][1] * x + mInvTransform.m[1][1] * y + mInvTransform.m[2][1] * z,
			           mInvTransform.m[0][2] * x + mInvTransform.m[1][2] * y + mInvTransform.m[2][2] * z);
	}

	CRay CTransform::operator() (const CRay& ray) const
	{
		CPoint to = (*this)(ray.origin);
		CVector3 td = (*this)(ray.dir);

		return CRay(to, td);
	}

	CBoundingBox CTransform::operator() (const CBoundingBox& bbox) const
	{
		const CTransform& M = *this;

		CBoundingBox result(M(CPoint(bbox.min.x, bbox.min.y, bbox.min.z)), M(CPoint(bbox.max.x, bbox.min.y, bbox.min.z)));
		result = result.Union(M(CPoint(bbox.min.x, bbox.max.y, bbox.min.z)));
		result = result.Union(M(CPoint(bbox.min.x, bbox.min.y, bbox.max.z)));
		result = result.Union(M(CPoint(bbox.min.x, bbox.max.y, bbox.max.z)));
		result = result.Union(M(CPoint(bbox.max.x, bbox.max.y, bbox.min.z)));
		result = result.Union(M(CPoint(bbox.max.x, bbox.min.y, bbox.max.z)));
		result = result.Union(M(CPoint(bbox.max.x, bbox.max.y, bbox.max.z)));

		return result;
	}

	CTransform CTransform::operator* (const CTransform& transf) const
	{
		CTransform composedTransform;

		composedTransform.mTransform = mTransform * transf.mTransform;

		composedTransform.mInvTransform = composedTransform.mTransform;
		composedTransform.mInvTransform.Inverse();

		return composedTransform;
	}
}