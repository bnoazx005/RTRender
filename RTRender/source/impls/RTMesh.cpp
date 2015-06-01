#include "impls\RTMesh.h"


namespace RTRender
{
	/*!
		CMeshTriangle
	*/

	CMeshTriangle::CMeshTriangle(const TMeshBuffer* mesh, const unsigned int& i0, const unsigned int& i1, const unsigned int& i2,
		                         const TMaterial* material, const CColor& color) :
	     ISceneObject(material, color), mI0(i0), mI1(i1), mI2(i2), mpMesh(mesh)
	{
		CPoint p0(mpMesh->mVertices[mI0]);
		CPoint p1(mpMesh->mVertices[mI1]);
		CPoint p2(mpMesh->mVertices[mI2]);

		CVector3 u = p1 - p0;
		CVector3 v = p2 - p0;

		mNormal = CNormal(u.CrossProduct(v));
	}

	CMeshTriangle::~CMeshTriangle()
	{
	}

	bool CMeshTriangle::Hit(const CRay& ray, F& t, THitInfo& info) const
	{
		CRay castedRay = ray;

		CPoint p0(mpMesh->mVertices[mI0]);
		CPoint p1(mpMesh->mVertices[mI1]);
		CPoint p2(mpMesh->mVertices[mI2]);

		F a = p0.x - p1.x, b = p0.x - p2.x, c = castedRay.dir.x, d = p0.x - castedRay.origin.x;
		F e = p0.y - p1.y, f = p0.y - p2.y, g = castedRay.dir.y, h = p0.y - castedRay.origin.y;
		F i = p0.z - p1.z, j = p0.z - p2.z, k = castedRay.dir.z, l = p0.z - castedRay.origin.z;

		F m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
		F q = g * i - e * k, s = e * j - f * i;

		F invDenom = 1.0 / (a * m + b * q + c * s);

		F e1 = d * m - b * n - c * p;
		F beta = e1 * invDenom;

		if (beta < 0.0)
			return (false);

		F r = r = e * l - h * i;
		F e2 = a * n + d * q + c * r;
		F gamma = e2 * invDenom;

		if (gamma < 0.0)
			return false;

		if (beta + gamma > 1.0)
			return false;

		F e3 = a * p - b * r + d * s;
		F t0 = e3 * invDenom;

		if (t0 < CMath::mEpsilon)
			return false;

		t = t0;

		info.mNormal = mNormal;
		info.mRay = castedRay;
		info.mColor = mColor;
		info.mPoint = castedRay.origin + castedRay.dir * t;
		info.mpObject = this;
		info.mpMaterial = mpMaterial;

		return true;
	}

	CBoundingBox CMeshTriangle::GetBoundingBox() const
	{
		CBoundingBox triBBox(mpMesh->mVertices[mI0], mpMesh->mVertices[mI1]);
		triBBox = triBBox.Union(mpMesh->mVertices[mI2]);

		return triBBox;
	}

	bool CMeshTriangle::IsInsideBBox(const CBoundingBox& bbox) const
	{
		if (bbox.Inside(mpMesh->mVertices[mI0]) || bbox.Inside(mpMesh->mVertices[mI1]) || bbox.Inside(mpMesh->mVertices[mI2]))
			return true;

		return false;
	}

	/*!
		CSmoothMeshTriangle
	*/

	CSmoothMeshTriangle::CSmoothMeshTriangle(const TMeshBuffer* mesh, const unsigned int& i0, const unsigned int& i1, const unsigned int& i2,
		                                     const TMaterial* material, const CColor& color) :
		CMeshTriangle(mesh, i0, i1, i2, material, color)
	{
	}

	CSmoothMeshTriangle::~CSmoothMeshTriangle()
	{
	}

	bool CSmoothMeshTriangle::Hit(const CRay& ray, F& t, THitInfo& info) const
	{
		CRay castedRay = ray;

		CPoint p0(mpMesh->mVertices[mI0]);
		CPoint p1(mpMesh->mVertices[mI1]);
		CPoint p2(mpMesh->mVertices[mI2]);

		F a = p0.x - p1.x, b = p0.x - p2.x, c = castedRay.dir.x, d = p0.x - castedRay.origin.x;
		F e = p0.y - p1.y, f = p0.y - p2.y, g = castedRay.dir.y, h = p0.y - castedRay.origin.y;
		F i = p0.z - p1.z, j = p0.z - p2.z, k = castedRay.dir.z, l = p0.z - castedRay.origin.z;

		F m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
		F q = g * i - e * k, s = e * j - f * i;

		F invDenom = 1.0 / (a * m + b * q + c * s);

		F e1 = d * m - b * n - c * p;
		F beta = e1 * invDenom;

		if (beta < 0.0)
			return (false);

		F r = r = e * l - h * i;
		F e2 = a * n + d * q + c * r;
		F gamma = e2 * invDenom;

		if (gamma < 0.0)
			return false;

		if (beta + gamma > 1.0)
			return false;

		F e3 = a * p - b * r + d * s;
		F t0 = e3 * invDenom;

		if (t0 < CMath::mEpsilon)
			return false;

		t = t0;

		info.mNormal = interpolateNormal(beta, gamma);
		info.mRay = castedRay;
		info.mColor = mColor;
		info.mPoint = castedRay.origin + castedRay.dir * t;
		info.mpObject = this;
		info.mpMaterial = mpMaterial;

		return true;
	}

	CNormal CSmoothMeshTriangle::interpolateNormal(const F& b, const F& g) const
	{
		return CNormal(mpMesh->mNormals[mI0] * (1.0 - b - g) + mpMesh->mNormals[mI1] * b + mpMesh->mNormals[mI2] * g);
	}
}