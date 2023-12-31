#include "XLCamera.h"

XLCamera::XLCamera()
	: mPosition(0.0f, 1.0f, -2.0f),
	mRight(1.0f, 0.0f, 0.0f),
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f),
	mEnableOrtho(true)
{
	SetLens(0.4f * 3.14f, 1.0f, 0.0001f, 1000.0f);
	//SetOrthoFrustum(6.0f, 0.0001f, 1000.0f);
	//Pitch(-30);
}

XLCamera::~XLCamera()
{

}

XMVECTOR XLCamera::GetPositionXM() const
{
	return XMLoadFloat3(&mPosition);
}

XMFLOAT3 XLCamera::GetPosition() const
{
	return mPosition;
}

void XLCamera::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void XLCamera::SetPosition(const XMFLOAT3& v)
{
	mPosition = v;
}

XMVECTOR XLCamera::GetRightXM() const
{
	return XMLoadFloat3(&mRight);
}

XMFLOAT3 XLCamera::GetRight() const
{
	return mRight;
}

XMVECTOR XLCamera::GetUpXM() const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 XLCamera::GetUp() const
{
	return mUp;
}

XMVECTOR XLCamera::GetLookXM() const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 XLCamera::GetLook() const
{
	return mLook;
}

float XLCamera::GetNearZ() const
{
	return mNearZ;
}

float XLCamera::GetFarZ() const
{
	return mFarZ;
}

float XLCamera::GetAspect() const
{
	return mAspect;
}

float XLCamera::GetFovY() const
{
	return mFovY;
}

float XLCamera::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

float XLCamera::GetNearWindowWidth() const
{
	return mAspect * mNearWindowHeight;
}

float XLCamera::GetNearWindowHeight() const
{
	return mNearWindowHeight;
}

float XLCamera::GetFarWindowWidth() const
{
	return mAspect * mFarWindowHeight;
}

float XLCamera::GetFarWindowHeight() const
{
	return mFarWindowHeight;
}

void XLCamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	mEnableOrtho = false;

	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	//XMMATRIX P = XMMatrixOrthographicLH(2.0f, 2.0f, mNearZ, mFarZ);

	XMStoreFloat4x4(&mProj, P);
}

void XLCamera::SetOrthoFrustum(float size, float zn, float zf)
{
	mEnableOrtho = true;

	mOrthoFrustumSize = size;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	XMMATRIX P = XMMatrixOrthographicLH(size, size, mNearZ, mFarZ);

	XMStoreFloat4x4(&mProj, P);
}

void XLCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	// 시선벡터와 월드업을 외적하면 right벡터가 나오고
	// 다시 그 right벡터와 시선벡터를 외적하면 내 up벡터가 나옴
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void XLCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

XMMATRIX XLCamera::View() const
{
	return XMLoadFloat4x4(&mView);
}

XMMATRIX XLCamera::Proj() const
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX XLCamera::ViewProj() const
{
	return XMMatrixMultiply(View(), Proj());
}

void XLCamera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

void XLCamera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));

	if (mEnableOrtho)
	{
		mOrthoFrustumSize -= d;
		SetOrthoFrustum(mOrthoFrustumSize, mNearZ, mFarZ);
	}
}

void XLCamera::WorldUpDown(float d)
{
	// mPosition += d * mUp
	XMVECTOR scale = XMVectorReplicate(d);
	XMVECTOR up = XMLoadFloat3(&mUp);
	XMVECTOR position = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(scale, up, position));
}

void XLCamera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void XLCamera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void XLCamera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&mPosition);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	mView(0, 0) = mRight.x;
	mView(1, 0) = mRight.y;
	mView(2, 0) = mRight.z;
	mView(3, 0) = x;

	mView(0, 1) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(2, 1) = mUp.z;
	mView(3, 1) = y;

	mView(0, 2) = mLook.x;
	mView(1, 2) = mLook.y;
	mView(2, 2) = mLook.z;
	mView(3, 2) = z;

	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;
}
