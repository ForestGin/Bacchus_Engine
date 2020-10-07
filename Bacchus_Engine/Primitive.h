
#pragma once

#include "Color.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const math::float3& u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	math::float4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class PrimiCube : public Primitive
{
public :
	PrimiCube();
	PrimiCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
	void Size(float x, float y, float z);
public:
	math::float3 size;
};

// ============================================
class PrimiSphere : public Primitive
{
public:
	PrimiSphere();
	PrimiSphere(float radius);
public:
	float radius;
};

// ============================================
class PrimiCylinder : public Primitive
{
public:
	PrimiCylinder();
	PrimiCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class PrimiLine : public Primitive
{
public:
	PrimiLine();
	PrimiLine(float x, float y, float z);
	void InnerRender() const;
public:
	math::float3 origin;
	math::float3 destination;
};

// ============================================
class PrimiPlane : public Primitive
{
public:
	PrimiPlane();
	PrimiPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	math::float3 normal;
	float constant;
};