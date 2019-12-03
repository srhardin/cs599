/* Start Header -------------------------------------------------------
Copyright (C) 2019
DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute
of Technology is prohibited.
Language:<C++>
Platform:<windows 10>
Project:<GAM 541, Void Engine>
Author:<Wang Zhongqiu>
-End Header --------------------------------------------------------*/
#include "pch.h"
#include "System/CollisionSys.h"
#include "Entity.h"
#define EPSILON 0.00001f
#define PHYSICS_ROTATION
#define OLD_TRIGGER
Contact::Contact() :
	position(0),
	normal(0),
	depth(0),
	isTrigger(false)
{
}

Contact::~Contact()
{
}

bool CheckCollisionCircleToCircle(C_World& world, C_Collider* collider1, C_Collider* collider2, std::vector<Contact*> &Contacts);

bool CheckCollisionCircleToPolygon(C_World& world, C_Collider* circl1, C_Collider* pShape2, std::vector<Contact*> &Contacts);
bool CheckCollisionPolygonToCircle(C_World& world, C_Collider* circl1, C_Collider* pShape2, std::vector<Contact*> &Contacts);
bool CheckCollisionPolygonToPolygon(C_World& world, C_Collider* collider1, C_Collider* collider2, std::vector<Contact*> &Contacts);

C_CollisionSystem::C_CollisionSystem() {
	CollisionFunctions[C_Collider::CIRCLE][C_Collider::CIRCLE] = CheckCollisionCircleToCircle;
	CollisionFunctions[C_Collider::BOX][C_Collider::CIRCLE] = CheckCollisionPolygonToCircle;
	CollisionFunctions[C_Collider::CIRCLE][C_Collider::BOX] = CheckCollisionCircleToPolygon;
	CollisionFunctions[C_Collider::POLYGON][C_Collider::CIRCLE] = CheckCollisionPolygonToCircle;
	CollisionFunctions[C_Collider::CIRCLE][C_Collider::POLYGON] = CheckCollisionCircleToPolygon;
	CollisionFunctions[C_Collider::POLYGON][C_Collider::POLYGON] = CheckCollisionPolygonToPolygon;

	/*CollisionFunctions[C_Collider::CIRCLE][C_Collider::AABB] = CheckCollisionCircleAABB;
	CollisionFunctions[C_Collider::AABB][C_Collider::CIRCLE] = CheckCollisionAABBCircle;*/

}

C_CollisionSystem::~C_CollisionSystem()
{
}

void C_CollisionSystem::Register(C_World & worldname, std::vector<int>& entityList)
{
	if (entityList.size() == 0) return;

	for (auto i : entityList)
	{
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER)).any())
		{
			S_ColliderRegister colliderMem;
			colliderMem.collider = worldname.mEntities[i].mComponents[E_CP_CIRCLE_COLLIDER];
			colliderMem.type = C_Collider::CIRCLE;
			colliderMem.transform = worldname.mEntities[i].mComponents[E_CP_TRANSFORM];

			mColliderList.insert(colliderMem);
		}
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_BOX_COLLIDER)).any())
		{
			S_ColliderRegister colliderMem;
			colliderMem.collider = worldname.mEntities[i].mComponents[E_CP_BOX_COLLIDER];
			colliderMem.type = C_Collider::POLYGON;
			colliderMem.transform = worldname.mEntities[i].mComponents[E_CP_TRANSFORM];

			mColliderList.insert(colliderMem);
		}
	}

}

void C_CollisionSystem::Unregister(C_World & worldname, std::vector<int>& entityList)
{
	for (auto i : entityList)
	{
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER)).any())
		{
			S_ColliderRegister colliderMem;
			colliderMem.collider = worldname.mEntities[i].mComponents[E_CP_CIRCLE_COLLIDER];
			colliderMem.type = C_Collider::CIRCLE;
			colliderMem.transform = worldname.mEntities[i].mComponents[E_CP_TRANSFORM];
			mColliderList.erase(colliderMem);
		}
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_BOX_COLLIDER)).any())
		{
			S_ColliderRegister colliderMem;
			colliderMem.collider = worldname.mEntities[i].mComponents[E_CP_BOX_COLLIDER];
			colliderMem.type = C_Collider::POLYGON;
			colliderMem.transform = worldname.mEntities[i].mComponents[E_CP_TRANSFORM];
			assert(colliderMem.transform != -1);
			mColliderList.erase(colliderMem);
		}
	}
}



void C_CollisionSystem::Reset()
{
	if (mContacts.size() > 0) {
		for (auto c : mContacts)
			delete c;
		mContacts.clear();
	}
}


bool C_CollisionSystem::CheckCollisionAndGenerateContact(C_World& world, C_Collider * collider1, unsigned type1, C_Collider* collider2, unsigned type2) {
	
	return CollisionFunctions[type1][type2](world, collider1, collider2, mContacts);
}

void C_CollisionSystem::Update(C_World &worldname, float deltaTime)
{
	Reset();
	for (auto iter1 = mColliderList.begin(); iter1 != mColliderList.end(); ++iter1) {
		if ((*iter1).type == C_Collider::CIRCLE)
		{
			worldname.mCircleColliders[(*iter1).collider].collideObjects.clear();
			worldname.mCircleColliders[(*iter1).collider].center = worldname.mTransforms[(*iter1).transform].WorldPos;

		}
		else if ((*iter1).type == C_Collider::POLYGON)
		{
			worldname.mPolygonColliders[(*iter1).collider].collideObjects.clear();
			worldname.mPolygonColliders[(*iter1).collider].center = worldname.mTransforms[(*iter1).transform].WorldPos;
			worldname.mPolygonColliders[(*iter1).collider].quatanion = worldname.mTransforms[(*iter1).transform].WorldQuaternion;

		}

	}

	for (auto iter1 = mColliderList.begin(); iter1 != mColliderList.end(); ++iter1)
	{
		C_Collider *collider1 = nullptr;
		if ((*iter1).type == C_Collider::CIRCLE)
		{
			collider1 = &worldname.mCircleColliders[(*iter1).collider];
		}
		else if ((*iter1).type == C_Collider::POLYGON)
		{
			collider1 = &worldname.mPolygonColliders[(*iter1).collider];
		}
		if (!collider1->needCollide)
		{
			continue;
		}
		auto iter2 = iter1;

		for (++iter2; iter2 != mColliderList.end(); ++iter2)
		{
			C_Collider *collider2 = nullptr;
			if ((*iter2).type == C_Collider::CIRCLE)
			{
				collider2 = &worldname.mCircleColliders[(*iter2).collider];
			}
			else if ((*iter2).type == C_Collider::POLYGON)
			{
				collider2 = &worldname.mPolygonColliders[(*iter2).collider];
			}

			if (!collider2->needCollide)
			{
				continue;
			}

			if ((collider1->mLayer & collider2->notCollideLayer) != 0 || (collider1->notCollideLayer & collider2->mLayer) != 0  ||((collider1->mBody == collider2->mBody)&& collider1->mBody!=-1)	)		{
				continue;
			}
			//if (worldname.mCircleColliders[(*iter1).collider].ownerID== 2&&worldname.mCircleColliders[(*iter2).collider].ownerID== 5)
			//{
			//	printf("asteroid: x:%f , y£º%f \n", worldname.mCircleColliders[(*iter1).collider].center.x, worldname.mCircleColliders[(*iter1).collider].center.y);

			//	//printf("player: x:%f , y£º%f \n", worldname.mCircleColliders[(*iter2).collider].center.x, worldname.mCircleColliders[(*iter2).collider].center.y);
			//}
			//CheckCollisionAndGenerateContact(pShape1, vec2(Pos1X, Pos1Y), pShape2, vec2(Pos2X, Pos2Y), (*iter1), (*iter2));
			CheckCollisionAndGenerateContact(worldname, collider1, (*iter1).type, collider2, (*iter2).type);
		}
	}
}

bool CheckCollisionCircleToCircle(C_World& world, C_Collider* collider1,
	C_Collider* collider2, std::vector<Contact*> &Contacts)
{
	//Intersection
	bool Intersection = false;

	vec3 aPos = collider1->center;
	vec3 bPos = collider2->center;

	C_CircleCollider *circle1 = static_cast<C_CircleCollider*>(collider1);
	C_CircleCollider *circle2 = static_cast<C_CircleCollider*>(collider2);

	// Calculate translational vector, which is normal
	vec3 normal3 = bPos - aPos;
	vec2 normal(normal3.x, normal3.y);
	float dist_sqr = glm::dot(normal, normal);
	float radius = circle1->mRadius + circle2->mRadius;

	// Not in contact
	if (dist_sqr >= radius * radius)
	{
		return Intersection;
	}
	Intersection = true;
	float distance = std::sqrt(dist_sqr);

	Contact *pNewContact = new Contact();
	pNewContact->isTrigger = (circle1->isTrigger || circle2->isTrigger);
	pNewContact->bodys[0] = collider1->mBody;

	pNewContact->colliderTransforms[0] = world.mEntities[collider1->ownerID].mComponents[E_CP_TRANSFORM];
	pNewContact->bodys[1] = collider2->mBody;
	pNewContact->colliderTransforms[1] = world.mEntities[collider2->ownerID].mComponents[E_CP_TRANSFORM];

	if (distance == 0.0f)
	{
		pNewContact->depth = circle1->mRadius;
		pNewContact->normal = vec3(1.0f, 0.0f, 0.0f);
		pNewContact->position = aPos;
	}
	else
	{
		pNewContact->depth = radius - distance;
		pNewContact->normal = Vec2ToVec3(normal) / distance;
		pNewContact->position = pNewContact->normal * circle1->mRadius + aPos;
	}

#ifndef PHYSICS_ROTATION
	pNewContact->bodys[0] = -1;
	pNewContact->bodys[1] = -1;
#endif


#ifdef  OLD_TRIGGER
	circle1->collideObjects.insert(circle2->ownerID);
	circle2->collideObjects.insert(circle1->ownerID);
#endif //  OLD_TRIGGER

#ifndef OLD_TRIGGER
	if (!circle1->isTrigger)
	{
		circle2->collideObjects.insert(circle1->ownerID);
	}
	if (!circle2->isTrigger)
	{
		circle1->collideObjects.insert(circle2->ownerID);
	}
#endif // !OLD_TRIGGER
	Contacts.push_back(pNewContact);
	return Intersection;
}




struct S_Segment {
	float minValue;
	float maxValue;
	S_Segment() :minValue(FLT_MAX), maxValue(FLT_MIN) {}
};

struct S_Penetration {
	unsigned pointID;
	float penetration;
	S_Penetration() :penetration(-FLT_MAX) {};
};

bool BiasGreaterThan(float a, float b)
{
	const float k_biasRelative = 0.95f;
	const float k_biasAbsolute = 0.01f;
	return a >= b * k_biasRelative + a * k_biasAbsolute;
}




vec2 GetSupportPoint(const vec2& dir, const C_Polygon * polygon)
{
	float bestProjection = -FLT_MAX;
	vec2 bestVertex;

	for (int i = 0; i < polygon->points.size(); ++i)
	{
		vec2 v = Vec3ToVec2(polygon->points[i]);
		float projection = glm::dot(v, dir);

		if (projection > bestProjection)
		{
			bestVertex = v;
			bestProjection = projection;
		}
	}

	return bestVertex;
}


S_Penetration FindLeastPenetration(const C_Polygon * polygon1, const C_Polygon * polygon2) {
	S_Penetration result;
	for (int i = 0; i < polygon1->points.size(); ++i) {
		// Retrieve a face normal from polygon1, translate to world space
		vec4 normalt = RotationVector(polygon1->quatanion, polygon1->normals[i]);
		//transform face normal into polygon2's model space
		mat4 polygon2RotationMatrixTranspose = glm::transpose(QuaternionToRotationMatrix(polygon2->quatanion));
		vec4 normal = polygon2RotationMatrixTranspose * normalt;
		vec2 n = vec2(normal.x, normal.y);
		//get support point from polygon2 along -n
		vec2 s = GetSupportPoint(-n, polygon2);

		//transform polygon1's vertex into polygon2's model space
		vec2 v = Vec3ToVec2(polygon1->points[i]);
		vec4 vWorld = RotationVector(polygon1->quatanion, Vec2ToVec3(v)) + vec4(polygon1->center.x, polygon1->center.y, 0.0f, 0.0f);
		vec2 polygon2Center = Vec3ToVec2(polygon2->center);
		vWorld -= Vec2ToVec4(polygon2Center);
		vWorld = polygon2RotationMatrixTranspose * vWorld;
		v = vec2(vWorld.x, vWorld.y);
		//compute penetration distance in polygon2's model space
		float dis = glm::dot(n, s - v);

		if (dis > result.penetration)
		{
			result.penetration = dis;
			result.pointID = i;
		}
	}
	return result;
}

//void FindIncidentFace(vec2* vList, C_Polygon* referencePolygon, C_Polygon* incidentPolygon, unsigned int referenceIndex) {
//	vec2 referenceNormal = referencePolygon->normals[referenceIndex];
//
//	vec4 referenceNormalVec4 = RotationVector(referencePolygon->quatanion, vec4(referenceNormal.x, referenceNormal.y, 0.0f, 0.0f));
//	unsigned incidentIndex = 0;
//	float minDot = FLT_MAX;
//	for (int i = 0; i < incidentPolygon->points.size(); ++i)
//	{
//		float dot = glm::dot(referenceNormalVec4, vec4(incidentPolygon->normals[i].x, incidentPolygon->normals[i].y, 0.0f, 0.0f));
//		if (dot < minDot)
//		{
//			minDot = dot;
//			incidentIndex = i;
//		}
//	}
//
//	vec4 incidentRo = RotationVector(incidentPolygon->quatanion, Vec2ToVec4(incidentPolygon->points[incidentIndex]));
//
//	vList[0] = Vec4ToVec2(incidentRo) + incidentPolygon->center;
//	incidentIndex = incidentIndex - 1 < 0 ? incidentPolygon->points.size() : incidentIndex - 1;
//	incidentRo = RotationVector(incidentPolygon->quatanion, Vec2ToVec4(incidentPolygon->points[incidentIndex]));
//	vList[1] = Vec4ToVec2(incidentRo) + incidentPolygon->center;
//	//incidentFace = incidentFace + 1 >= (int32)IncPoly->m_vertexCount ? 0 : incidentFace + 1;
//	//vList[1] = IncPoly->u * IncPoly->m_vertices[incidentFace] + IncPoly->body->position;
//
//}

bool CheckCollisionCircleToPolygon(C_World& world, C_Collider * collider1, C_Collider * collider2, std::vector<Contact*>& Contacts) {

	C_CircleCollider* circle = static_cast<C_CircleCollider*>(collider1);
	C_Polygon* polygon = static_cast<C_Polygon*>(collider2);

//my way


	vec2 center2 = Vec3ToVec2(circle->center);
	vec2 plygonCenter = Vec3ToVec2(polygon->center);
	vec3 polygonToCircle = circle->center - polygon->center;
	polygonToCircle.z = 0;
	//transform circle center to Polygon model space
	vec3 center = Vec4ToVec3(glm::transpose(QuaternionToRotationMatrix(polygon->quatanion)) * Vec3ToVec4(polygonToCircle));


	vec2 v;
	vec2 polygon2circle = Vec3ToVec2(center);
	float max = FLT_MIN;
	int maxNormalID = 0;
	int maxPoint = 0;
	vec2  polygon2circleUnit = glm::normalize(polygon2circle);
	for (int i = 0; i < polygon->points.size(); ++i)
	{

		vec2 currentCorner = Vec3ToVec2(polygon->points[i]);
		v = currentCorner;
		float projection = glm::dot(v, polygon2circleUnit);
		if (max < projection)
		{
			max = projection;
			maxNormalID = i;
			maxPoint = i;
		}


	}
	float dis = glm::length(polygon2circle);
	if (dis - max - circle->mRadius > 0)
	{
		return false;
	}
	//get another points
	vec3 v0 = polygon->points[maxPoint];
	unsigned i1;
	unsigned i2 = maxPoint - 1 >= 0 ? maxPoint - 1 : polygon->points.size() - 1;


	vec3 normal1 = polygon->normals[maxNormalID];
	vec3 normal2 = polygon->normals[i2];
	normal1 = RotationVector(polygon->quatanion, normal1);
	normal2 = RotationVector(polygon->quatanion, normal2);
	float dot1 = glm::dot(center - v0, normal1);
	float dot2 = glm::dot(center - v0, normal2);


	vec3 normal;
	if (dot1 > dot2)
	{
		normal = normal1;
	}
	else if (dot1 < dot2)
	{
		normal = normal2;
		maxNormalID = i2;

	}
	else {
		Contact *pNewContact = new Contact();
		pNewContact->depth = circle->mRadius + max - dis;
		vec3 n = v0 - center;
		n = Vec4ToVec3(RotationVector(polygon->quatanion, n));
		n = glm::normalize(n);
		pNewContact->normal = n;
		v0 = Vec4ToVec3(RotationVector(polygon->quatanion, v0)) + polygon->center;
		v0.z = 0.0f;
		pNewContact->position = v0;

		pNewContact->isTrigger = (circle->isTrigger || polygon->isTrigger);
		pNewContact->bodys[0] = circle->mBody;
		pNewContact->bodys[1] = polygon->mBody;
		pNewContact->colliderTransforms[0] = world.mEntities[circle->ownerID].mComponents[E_CP_TRANSFORM];
		pNewContact->colliderTransforms[1] = world.mEntities[polygon->ownerID].mComponents[E_CP_TRANSFORM];
#ifndef PHYSICS_ROTATION
		pNewContact->bodys[0] = -1;
		pNewContact->bodys[1] = -1;
#endif
		Contacts.push_back(pNewContact);
#ifdef  OLD_TRIGGER
		circle->collideObjects.insert(polygon->ownerID);
		polygon->collideObjects.insert(circle->ownerID);
#endif //  OLD_TRIGGER

#ifndef OLD_TRIGGER
		if (!circle->isTrigger)
		{
			polygon->collideObjects.insert(circle->ownerID);
		}
		if (!polygon->isTrigger)
		{
			circle->collideObjects.insert(polygon->ownerID);
		}
#endif // !OLD_TRIGGER

		
		return true;
	}

	vec3 v1 = polygon->points[maxNormalID];
	i1 = maxNormalID + 1 == polygon->points.size() ? 0 : maxNormalID + 1;
	vec3 v2 = polygon->points[i1];

	vec3 d1 = center - v1;
	vec3 d2 = center - v2;
	vec3 d3 = v2 - v1;
	 dot1 = glm::dot(center - v1, v2 - v1);
	 dot2 = glm::dot(center - v2, v1 - v2);


		// Closest to v1
	if (dot1 <= 0.0f)
	{
		if (glm::dot(center- v1, center - v1) > circle->mRadius * circle->mRadius)
			return false;

		Contact *pNewContact = new Contact();
		pNewContact->depth = circle->mRadius + max - dis;
		vec3 n = v1 - center;
		n = Vec4ToVec3(RotationVector( polygon->quatanion,n));
		n= glm::normalize(n);
		pNewContact->normal = n;
		v1 = Vec4ToVec3(RotationVector(polygon->quatanion, v1)) + polygon->center;
		v1.z = 0.0f;
		pNewContact->position = v1;

		pNewContact->isTrigger = (circle->isTrigger || polygon->isTrigger);
		pNewContact->bodys[0] = circle->mBody;
		pNewContact->bodys[1] = polygon->mBody;
		pNewContact->colliderTransforms[0] = world.mEntities[circle->ownerID].mComponents[E_CP_TRANSFORM];
		pNewContact->colliderTransforms[1] = world.mEntities[polygon->ownerID].mComponents[E_CP_TRANSFORM];
#ifndef PHYSICS_ROTATION
		pNewContact->bodys[0] = -1;
		pNewContact->bodys[1] = -1;
#endif

		Contacts.push_back(pNewContact);
	}

	// Closest to v2
	else if (dot2 <= 0.0f)
	{
		if (glm::dot(center - v2, center - v2) > circle->mRadius  * circle->mRadius)
			return false;
		Contact *pNewContact = new Contact();
		pNewContact->depth = circle->mRadius + max - dis;
		vec3 n = v2 - center;
		n = Vec4ToVec3(RotationVector(polygon->quatanion, n));
		n = glm::normalize(n);
		pNewContact->normal = n;
		v2 = Vec4ToVec3(RotationVector(polygon->quatanion, v2)) + polygon->center;
		v2.z = 0.0f;
		pNewContact->position = v2;

		pNewContact->isTrigger = (circle->isTrigger || polygon->isTrigger);
		pNewContact->bodys[0] = circle->mBody;
		pNewContact->bodys[1] = polygon->mBody;
		pNewContact->colliderTransforms[0] = world.mEntities[circle->ownerID].mComponents[E_CP_TRANSFORM];
		pNewContact->colliderTransforms[1] = world.mEntities[polygon->ownerID].mComponents[E_CP_TRANSFORM];
#ifndef PHYSICS_ROTATION
		pNewContact->bodys[0] = -1;
		pNewContact->bodys[1] = -1;
#endif

		Contacts.push_back(pNewContact);
	}

	// Closest to face
	else
	{		
		vec3 n = polygon->normals[maxNormalID];
		if (glm::dot(center - v1, n) >circle->mRadius)
			return false;
		Contact *pNewContact = new Contact();
		pNewContact->depth = circle->mRadius + max - dis;
		n = RotationVector(polygon->quatanion, n);
		pNewContact->normal = -glm::normalize(n);
		pNewContact->position = pNewContact->normal * circle->mRadius + circle->center;
		
		pNewContact->isTrigger = (circle->isTrigger || polygon->isTrigger);
		
		pNewContact->bodys[0] = circle->mBody;
		pNewContact->bodys[1] = polygon->mBody;
		pNewContact->colliderTransforms[0] = world.mEntities[circle->ownerID].mComponents[E_CP_TRANSFORM];
		pNewContact->colliderTransforms[1] = world.mEntities[polygon->ownerID].mComponents[E_CP_TRANSFORM];
#ifndef PHYSICS_ROTATION
		pNewContact->bodys[0] = -1;
		pNewContact->bodys[1] = -1;
#endif
		Contacts.push_back(pNewContact);
	}
#ifdef  OLD_TRIGGER
	circle->collideObjects.insert(polygon->ownerID);
	polygon->collideObjects.insert(circle->ownerID);
#endif //  OLD_TRIGGER

#ifndef OLD_TRIGGER
	if (!circle->isTrigger)
	{
		polygon->collideObjects.insert(circle->ownerID);
	}
	if (!polygon->isTrigger)
	{
		circle->collideObjects.insert(polygon->ownerID);
	}
#endif // !OLD_TRIGGER
	return true;
}

bool CheckCollisionPolygonToCircle(C_World& world, C_Collider * collider1, C_Collider * collider2, std::vector<Contact*>& Contacts) {
	return CheckCollisionCircleToPolygon(world, collider2, collider1, Contacts);

}
//
bool CheckCollisionPolygonToPolygon(C_World& world, C_Collider* collider1, C_Collider* collider2, std::vector<Contact*> &Contacts) {
	C_Polygon* polygon1 = static_cast<C_Polygon*>(collider1);
	C_Polygon* polygon2 = static_cast<C_Polygon*>(collider2);
	/*if (polygon1->ownerID == 23&& polygon2->ownerID == 29)
	{
		int aaa = 1;
	}*/

 // Check for a separating axis with collider1's face planes
	S_Penetration penetrationA = FindLeastPenetration(polygon1, polygon2);
	if (penetrationA.penetration >= 0.0f)
		return false;

	// Check for a separating axis with collider2's face planes

	S_Penetration penetrationB = FindLeastPenetration(polygon2, polygon1);
	if (penetrationB.penetration >= 0.0f)
		return false;
	


	Contact *pNewContact = new Contact();
	pNewContact->depth = 0;
	pNewContact->isTrigger = true;//(collider1->isTrigger || polygon2->isTrigger);
	pNewContact->bodys[0] = collider1->mBody;
	pNewContact->bodys[1] = polygon2->mBody;
	pNewContact->colliderTransforms[0] = world.mEntities[collider1->ownerID].mComponents[E_CP_TRANSFORM];
	pNewContact->colliderTransforms[1] = world.mEntities[polygon2->ownerID].mComponents[E_CP_TRANSFORM];
	Contacts.push_back(pNewContact);

#ifdef  OLD_TRIGGER
	polygon1->collideObjects.insert(polygon2->ownerID);
	polygon2->collideObjects.insert(polygon1->ownerID);
#endif //  OLD_TRIGGER

#ifndef OLD_TRIGGER
	if (!polygon1->isTrigger)
	{
		polygon2->collideObjects.insert(polygon1->ownerID);
	}
	if (!polygon2->isTrigger)
	{
		polygon1->collideObjects.insert(polygon2->ownerID);
	}
#endif // !OLD_TRIGGER
	
	return true;

}

