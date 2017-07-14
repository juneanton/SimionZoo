#include "stdafx.h"
#include "matrix44.h"


Matrix44::Matrix44()
{
}


Matrix44::~Matrix44()
{
}

Matrix44 Matrix44::identity()
{
	Matrix44 m;
	m.set(0, 0, 1.0); m.set(0, 1, 0.0); m.set(0, 2, 0.0); m.set(0, 3, 0.0);
	m.set(1, 0, 0.0); m.set(1, 1, 1.0); m.set(1, 2, 0.0); m.set(1, 3, 0.0);
	m.set(2, 0, 0.0); m.set(2, 1, 0.0); m.set(2, 2, 1.0); m.set(2, 3, 0.0);
	m.set(3, 0, 0.0); m.set(3, 1, 0.0); m.set(3, 2, 0.0); m.set(3, 3, 1.0);
	return m;
}

Matrix44 Matrix44::rotationMatrix(Quaternion& quat)
{
	Matrix44 m;
	double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = quat.x() + quat.x();  y2 = quat.y() + quat.y();  z2 = quat.z() + quat.z();
	xx = quat.x() * x2;       xy = quat.x() * y2;       xz = quat.x() * z2;
	yy = quat.y() * y2;       yz = quat.y() * z2;       zz = quat.z() * z2;
	wx = quat.w() * x2;       wy = quat.w() * y2;       wz = quat.w() * z2;
	
	m.set(0, 0, 1.0 - (yy + zz));
	m.set(0, 1, xy - wz);
	m.set(0, 2, xz + wy);
	m.set(0, 3, 0.0);
	m.set(1, 0, xy + wz);
	m.set(1, 1, 1.0 - (xx + zz));
	m.set(1, 2, yz - wx);
	m.set(1, 3, 0.0);
	m.set(2, 0, xz - wy);
	m.set(2, 1, yz + wx);
	m.set(2, 2, 1.0 - (xx + yy));
	m.set(2, 3, 0.0);
	m.set(3, 0, 0.0);
	m.set(3, 1, 0.0);
	m.set(3, 2, 0.0);
	m.set(3, 3, 1.0);
	return m;
}

Matrix44 Matrix44::translationMatrix(Vector3D& translation)
{
	Matrix44 m;
	m.set(0, 0, 1.0);
	m.set(0, 1, 0.0);
	m.set(0, 2, 0.0);
	m.set(0, 3, 0.0);
	m.set(1, 0, 0.0);
	m.set(1, 1, 1.0);
	m.set(1, 2, 0.0);
	m.set(1, 3, 0.0);
	m.set(2, 0, 0.0);
	m.set(2, 1, 0.0);
	m.set(2, 2, 1.0);
	m.set(2, 3, 0.0);
	m.set(3, 0, translation.x());
	m.set(3, 1, translation.y());
	m.set(3, 2, translation.z());
	m.set(3, 3, 1.0);
	return m;
}
Matrix44 Matrix44::scaleMatrix(Vector3D& scale)
{
	Matrix44 m;
	m.set(0, 0, scale.x());
	m.set(0, 1, 0.0);
	m.set(0, 2, 0.0);
	m.set(0, 3, 0.0);
	m.set(1, 0, 0.0);
	m.set(1, 1, scale.y());
	m.set(1, 2, 0.0);
	m.set(1, 3, 0.0);
	m.set(2, 0, 0.0);
	m.set(2, 1, 0.0);
	m.set(2, 2, scale.z());
	m.set(2, 3, 0.0);
	m.set(3, 0, 0.0);
	m.set(3, 1, 0.0);
	m.set(3, 2, 0.0);
	m.set(3, 3, 1.0);
	return m;

}

Matrix44 Matrix44::operator*(Matrix44& mat) const
{
	Matrix44 result;
	result.set(0, 0, get(0, 0)*mat.get(0, 0) + get(1, 0)*mat.get(0, 1) + get(2, 0)*mat.get(0, 2) + get(3, 0)*mat.get(0, 3));
	result.set(1, 0, get(0, 0)*mat.get(1, 0) + get(1, 0)*mat.get(1, 1) + get(2, 0)*mat.get(1, 2) + get(3, 0)*mat.get(1, 3));
	result.set(2, 0, get(0, 0)*mat.get(2, 0) + get(1, 0)*mat.get(2, 1) + get(2, 0)*mat.get(2, 2) + get(3, 0)*mat.get(2, 3));
	result.set(3, 0, get(0, 0)*mat.get(3, 0) + get(1, 0)*mat.get(3, 1) + get(2, 0)*mat.get(3, 2) + get(3, 0)*mat.get(3, 3));

	result.set(0, 1, get(0, 1)*mat.get(0, 0) + get(1, 1)*mat.get(0, 1) + get(2, 1)*mat.get(0, 2) + get(3, 1)*mat.get(0, 3));
	result.set(1, 1, get(0, 1)*mat.get(1, 0) + get(1, 1)*mat.get(1, 1) + get(2, 1)*mat.get(1, 2) + get(3, 1)*mat.get(1, 3));
	result.set(2, 1, get(0, 1)*mat.get(2, 0) + get(1, 1)*mat.get(2, 1) + get(2, 1)*mat.get(2, 2) + get(3, 1)*mat.get(2, 3));
	result.set(3, 1, get(0, 1)*mat.get(3, 0) + get(1, 1)*mat.get(3, 1) + get(2, 1)*mat.get(3, 2) + get(3, 1)*mat.get(3, 3));

	result.set(0, 2, get(0, 2)*mat.get(0, 0) + get(1, 2)*mat.get(0, 1) + get(2, 2)*mat.get(0, 2) + get(3, 2)*mat.get(0, 3));
	result.set(1, 2, get(0, 2)*mat.get(1, 0) + get(1, 2)*mat.get(1, 1) + get(2, 2)*mat.get(1, 2) + get(3, 2)*mat.get(1, 3));
	result.set(2, 2, get(0, 2)*mat.get(2, 0) + get(1, 2)*mat.get(2, 1) + get(2, 2)*mat.get(2, 2) + get(3, 2)*mat.get(2, 3));
	result.set(3, 2, get(0, 2)*mat.get(3, 0) + get(1, 2)*mat.get(3, 1) + get(2, 2)*mat.get(3, 2) + get(3, 2)*mat.get(3, 3));

	result.set(0, 3, get(0, 3)*mat.get(0, 0) + get(1, 3)*mat.get(0, 1) + get(2, 3)*mat.get(0, 2) + get(3, 3)*mat.get(0, 3));
	result.set(1, 3, get(0, 3)*mat.get(1, 0) + get(1, 3)*mat.get(1, 1) + get(2, 3)*mat.get(1, 2) + get(3, 3)*mat.get(1, 3));
	result.set(2, 3, get(0, 3)*mat.get(2, 0) + get(1, 3)*mat.get(2, 1) + get(2, 3)*mat.get(2, 2) + get(3, 3)*mat.get(2, 3));
	result.set(3, 3, get(0, 3)*mat.get(3, 0) + get(1, 3)*mat.get(3, 1) + get(2, 3)*mat.get(3, 2) + get(3, 3)*mat.get(3, 3));

	return result;
}



Point3D Matrix44::operator*(Point3D& v) const
{
	Point3D result;
	result.setX(get(0, 0)*v.x() + get(1, 0)*v.y() + get(2, 0)*v.z() + get(3, 0));
	result.setY(get(0, 1)*v.x() + get(1, 1)*v.y() + get(2, 1)*v.z() + get(3, 1));
	result.setZ(get(0, 2)*v.x() + get(1, 2)*v.y() + get(2, 2)*v.z() + get(3, 2));
	return result;
}

Vector3D Matrix44::operator*(Vector3D& v) const
{
	Vector3D result;
	result.setX(get(0, 0)*v.x() + get(1, 0)*v.y() + get(2, 0)*v.z());
	result.setY(get(0, 1)*v.x() + get(1, 1)*v.y() + get(2, 1)*v.z());
	result.setZ(get(0, 2)*v.x() + get(1, 2)*v.y() + get(2, 2)*v.z());
	return result;
}
double* Matrix44::asArray()
{
	return _values;
}

double Matrix44::get(int col, int row) const
{
	if (col >= 0 && col < 4 && row>=0 && row < 4)
	{
		return _values[col*4 + row];
	}
	return 0.0;
}

void Matrix44::set(int col, int row, double value)
{
	if (col >= 0 && col < 4 && row>=0 && row < 4)
	{
		_values[col*4 + row]= value;
	}
}

void Matrix44::flipYZAxis()
{
	//https://www.gamedev.net/topic/537664-handedness/#entry4480987
	double tmp;
	//swap 2nd and 3rd row vectors
	for (int i = 0; i < 4; ++i)
	{
		tmp = get(i, 1);
		set(i, 1, get(i, 2));
		set(i, 2, tmp);
	}
	//swap 2nd and 3rd column vectors
	for (int i = 0; i < 4; ++i)
	{
		tmp = get(1, i);
		set(1, i, get(2, i));
		set(2, i, tmp);
	}
}