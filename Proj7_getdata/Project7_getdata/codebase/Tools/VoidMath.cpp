/* Start Header -------------------------------------------------------
Copyright (C) 2019
DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute
of Technology is prohibited.
Language:<C++>
Platform:<windows 10>
Project:<GAM 541, Void Engine>
Author:<Li Chunayan>
-End Header --------------------------------------------------------*/
#include "pch.h"
#include "Tools/VoidMath.h"



//--------------transform functions------------
mat4 translate(const mat4 &m, const vec3 &v)
{
	return glm::translate(m, v);
}

mat4 scale(const mat4 &m, const vec3 &v)
{
	return glm::scale(m, v);
}

mat4 rotate(const mat4 &m, float angle, const vec3 &axis)
{
	
	return glm::rotate(m, angle, axis);
}

mat4 perspective(float fovy, float aspect, float _near, float _far)
{

	return glm::perspective(fovy, aspect, _near, _far);
}

mat4 lookAt(vec3 const &eye, vec3 const &center, vec3 const &up)
{
	return glm::lookAt(eye, center, up);
}

float radians(float degree)
{
	return glm::radians(degree);
}

vec3 normalize(vec3 a)
{
	return glm::normalize(a);
}

float distance(float a, float b)
{
	return glm::distance(a, b);
}

float distance(vec2 a, vec2 b)
{
	
	return glm::distance(a, b);
}

vec4 GetQuater(const vec4 &r)
{
	float s = sinf(r.w / 2);
	return vec4(s*r.x, s*r.y, s*r.z, cosf(r.w / 2));
}

vec4 conjugate(const vec4 &q)
{
	return vec4(-q.x, -q.y, -q.z, q.w);
}

vec4 QuaterMulti(const vec4 &q1, const vec4 &q2)
{
	return vec4(q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
		q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
		q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,
		q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
}

vec4 RotationVector(const vec4 &q, const vec4 &v)
{
	return QuaterMulti(QuaterMulti(q, v), conjugate(q));
}

vec4 RotationVector(const vec4 &q, const vec3 &v)
{
	vec4 vv = Vec3ToVec4(v);
	return QuaterMulti(QuaterMulti(q, vv), conjugate(q));




}


vec4 RotationVector(const quat &q, const vec3 &v)
{
	
	return q * Vec3ToVec4(v);
}




mat4 QuaternionToRotationMatrix(const vec4 &q)
{
	mat4 result = mat4(1.0f);
	result[0][0] = 1 - 2 * q.y*q.y - 2 * q.z*q.z;
	result[1][0] = 2 * q.x*q.y - 2 * q.w*q.z;
	result[2][0] = 2 * q.x*q.z + 2 * q.w*q.y;
	result[0][1] = 2 * q.x*q.y + 2 * q.w*q.z;
	result[1][1] = 1 - 2 * q.x*q.x - 2 * q.z*q.z;
	result[2][1] = 2 * q.y*q.z - 2 * q.w*q.x;
	result[0][2] = 2 * q.x*q.z - 2 * q.w*q.y;
	result[1][2] = 2 * q.y*q.z + 2 * q.w*q.x;
	result[2][2] = 1 - 2 * q.x*q.x - 2 * q.y*q.y;
	return result;
}


mat4 QuaternionToRotationMatrix(const quat &q) {

	 return glm::toMat4(q);
}


vec4 Slerp(const vec4 &qStart, const vec4 &qEnd, float t)
{
	float theata = acosf(dot(qStart,qEnd));
	if (theata < 0.001f) return qEnd;
	if (theata < 0.1f)
	{
		return (1 - t)*qStart + t * qEnd;
	}
	return (sinf((1 - t)*theata) / sinf(theata))*qStart + (sinf(t*theata) / sinf(theata))*qEnd;
}

float RadiusToDegree(float r)
{
	return (r*180.0f/PI);
}
float DegreeToRadius(float d)
{
	return (d*PI/180.0f);
}


vec4 Vec2ToVec4(const vec2& v) {
	return vec4(v.x,v.y,0.0f,0.0f);
}


vec2 Vec4ToVec2(const vec4& v) {
	return vec2(v.x,v.y);
}



vec3 Vec2ToVec3(const vec2& v) {

	return vec3(v.x,v.y,0.0f);
}
vec2 Vec3ToVec2(const vec3& v) {
return vec2(v.x,v.y);
}

vec3 Vec4ToVec3(const vec4& v) {

	return vec3(v.x, v.y, v.z);
}
vec4 Vec3ToVec4(const vec3& v) {
	return vec4(v.x, v.y,v.z,0);
}