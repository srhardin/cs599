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
#include "System/PhysicsSys.h"
#include "Component/Transform.h"
#include "System/CollisionSys.h"
#include "Entity.h"
#include "Enums.h"
#include<thread>
#include<sstream>
#define FRECTION
#define PHY_ROTATION
//#define WANG
#define PHYSYS_THREAD_NUMBER 0
#define DEGREE_TO_RADIAN (3.1415927f/180.0f)
#define EPSILEN 0.000001f
const vec3 X = vec3(1, 0, 0);
const vec3 Y = vec3(0, 1, 0);
const vec3 Z = vec3(0, 0, 1);



unsigned SearchTopBody(C_World& worldName, unsigned child) {

	unsigned parentBody = -1;


	if (worldName.mTransforms[GetComponent(worldName, child, E_CP_TRANSFORM)].parentID == -1)
	{
		parentBody = GetComponent(worldName, child, E_CP_BODY);
		return parentBody;
	}
	else {

		parentBody = SearchTopBody(worldName, worldName.mTransforms[GetComponent(worldName, child, E_CP_TRANSFORM)].parentID);

	}

	if (parentBody == -1)
	{
		parentBody = GetComponent(worldName, child, E_CP_BODY);
	}

	return parentBody;

};

unsigned AttachOne(C_World& worldName, unsigned parent, unsigned child, unsigned attachID) {
	if (parent == -1)
	{
		parent = child;
	}
	bool childHasBody = HasComponent(worldName, child, E_CPM_BODY);
	bool parentHasBody = HasComponent(worldName, parent, E_CPM_BODY);

	if (childHasBody)
	{
		worldName.mBodies[worldName.mEntities[child].mComponents[E_CP_BODY]].enable = true;
	}
	if (HasComponent(worldName, parent, E_CPM_CIRCLE_COLLIDER))
	{
		attachID = worldName.mCircleColliders[worldName.mEntities[parent].mComponents[E_CP_CIRCLE_COLLIDER]].mBody;
	}
	else if (HasComponent(worldName, parent, E_CPM_BOX_COLLIDER))
	{
		attachID = worldName.mPolygonColliders[worldName.mEntities[parent].mComponents[E_CP_BOX_COLLIDER]].mBody;
	}
	else
	{
		attachID = worldName.mEntities[parent].mComponents[E_CP_BODY];
	}



	if (attachID == -1)
	{
		if (parentHasBody)
		{
				attachID = worldName.mEntities[parent].mComponents[E_CP_BODY];
		}
		else if (childHasBody) {
			attachID = worldName.mEntities[child].mComponents[E_CP_BODY];
		}
	}
	if (parent == child)
	{
		if ((worldName.mEntities[child].entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER)).any())
		{
			worldName.mCircleColliders[worldName.mEntities[child].mComponents[E_CP_CIRCLE_COLLIDER]].mBody = attachID;
		}
		else if ((worldName.mEntities[child].entityMask & std::bitset<64>(E_CPM_BOX_COLLIDER)).any())
		{
			worldName.mPolygonColliders[worldName.mEntities[child].mComponents[E_CP_BOX_COLLIDER]].mBody = attachID;
		}
		return attachID;
	}
	if ((worldName.mEntities[child].entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER)).any() || (worldName.mEntities[child].entityMask & std::bitset<64>(E_CPM_BOX_COLLIDER)).any())
	{
		C_Collider* collider = nullptr;
		//get collider
		if ((worldName.mEntities[child].entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER)).any())
		{
			collider = &worldName.mCircleColliders[worldName.mEntities[child].mComponents[E_CP_CIRCLE_COLLIDER]];
		}
		else
		{
			collider = &worldName.mPolygonColliders[worldName.mEntities[child].mComponents[E_CP_BOX_COLLIDER]];
		}

		//if parent has body?
		if (parentHasBody && childHasBody)
		{
			worldName.mBodies[worldName.mEntities[child].mComponents[E_CP_BODY]].enable = false;
		}
		if (collider != nullptr)
		{
			collider->mBody = attachID;
		}
	}

	return attachID;
}

void Attach(C_World& worldName, unsigned parent, unsigned child, unsigned attachID) {
	unsigned Body = AttachOne(worldName, parent, child, attachID);
	for (auto i = worldName.mTransforms[GetComponent(worldName, child, E_CP_TRANSFORM)].childrenID.begin(); i != worldName.mTransforms[GetComponent(worldName, child, E_CP_TRANSFORM)].childrenID.end(); ++i)
	{
		Body = AttachOne(worldName, child, *i, Body);
	}
}


void Unattach(C_World& worldName, unsigned myID) {
	if ((worldName.mEntities[myID].entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER)).any())
	{
		worldName.mCircleColliders[worldName.mEntities[myID].mComponents[E_CP_CIRCLE_COLLIDER]].mBody = -1;
	}
	else if ((worldName.mEntities[myID].entityMask & std::bitset<64>(E_CPM_BOX_COLLIDER)).any())
	{
		worldName.mPolygonColliders[worldName.mEntities[myID].mComponents[E_CP_BOX_COLLIDER]].mBody = -1;
	}
	Attach(worldName, -1, myID);
}


//void UpdatePos(C_World &worldName, unsigned ChildID, unsigned parentID) {
//
//	worldName.mTransforms[ChildID].WorldPos = worldName.mTransforms[parentID].WorldPos + worldName.mTransforms[ChildID].LocalPos;
//	worldName.mTransforms[ChildID].WorldRotation = worldName.mTransforms[parentID].WorldRotation + worldName.mTransforms[ChildID].Rotation;
//	worldName.mTransforms[ChildID].TransMatrix = translate(mat4(1.0f), worldName.mTransforms[ChildID].WorldPos)
//		*rotate(mat4(1.0f), worldName.mTransforms[ChildID].WorldRotation.y, Y)
//		*rotate(mat4(1.0f), worldName.mTransforms[ChildID].WorldRotation.x, vec3(1, 0, 0))
//		*rotate(mat4(1.0f), worldName.mTransforms[ChildID].WorldRotation.z, vec3(0, 0, 1))
//		*scale(mat4(1.0f), worldName.mTransforms[ChildID].Scale);
//	if (worldName.mTransforms[ChildID].childrenID.size() == 0)
//	{
//		return;
//	}
//	for (auto i = worldName.mTransforms[ChildID].childrenID.begin(); i != worldName.mTransforms[ChildID].childrenID.end(); ++i)
//	{
//		UpdatePos(worldName, *i, ChildID);
//	}
//}

//
//
//C_PhySysUpdateThread::C_PhySysUpdateThread() {
//	std::thread(run);
//
//}
//
//void C_PhySysUpdateThread::Start() {
//	if (!run)
//	{
//		return;
//	}
//
//
//}


C_PhysicsSystem::C_PhysicsSystem(C_CollisionSystem* collisionSys) :stopPara(30.f), angularStopPara(10.0f), percent(0.8f), slop(0.01f)//, isThreadRun(PHYSYS_THREAD_NUMBER)
{
	CollisionSystem = collisionSys;
	list = &CollisionSystem->mContacts;
}
C_PhysicsSystem::~C_PhysicsSystem()
{

}

void C_PhysicsSystem::UpdateWorldPosition(C_World &worldName) {
	for (auto tr : transfromList)
	{
		if (worldName.mTransforms[tr].parentID == -1)		//always update from the family's root
		{
			std::queue<unsigned> myChildren;
			// update root's position
			worldName.mTransforms[tr].WorldQuaternion = worldName.mTransforms[tr].LocalQuaternion;
			worldName.mTransforms[tr].WorldPos = worldName.mTransforms[tr].LocalPos;
			worldName.mTransforms[tr].WorldScale = worldName.mTransforms[tr].Scale;
			worldName.mTransforms[tr].TransMatrix = translate(mat4(1.0f), worldName.mTransforms[tr].WorldPos)
				*QuaternionToRotationMatrix(worldName.mTransforms[tr].WorldQuaternion)
				*scale(mat4(1.0f), worldName.mTransforms[tr].WorldScale);

			vector<int> badData;
			for (auto child : worldName.mTransforms[tr].childrenID)
			{
#ifdef WANG
				UpdatePos(worldName, child, tr);
			}
#endif // WANG

#ifndef WANG
				if (worldName.mTransforms[child].parentID != -1)
					myChildren.push(child);
				else
					badData.push_back(child);

			}
			for (auto d : badData)
			{
				worldName.mTransforms[tr].childrenID.erase(d);
			}
		// update children's position
			while (!myChildren.empty())
			{
				// get the front child
				unsigned nextCh = myChildren.front();
				myChildren.pop();
				// push its children
				badData.clear();
				for (auto ch : worldName.mTransforms[nextCh].childrenID)
				{
					if (worldName.mTransforms[ch].parentID != -1)
						myChildren.push(ch);
					else
						badData.push_back(ch);	
				}
				for (auto d : badData)
				{
					worldName.mTransforms[nextCh].childrenID.erase(d);
				}
				unsigned pID = worldName.mTransforms[nextCh].parentID;
				worldName.mTransforms[nextCh].WorldPos = vec3(worldName.mTransforms[pID].TransMatrix * vec4(worldName.mTransforms[nextCh].LocalPos, 1.0f));
				// get quaternion
				worldName.mTransforms[nextCh].WorldScale = worldName.mTransforms[pID].WorldScale * worldName.mTransforms[nextCh].Scale;
				worldName.mTransforms[nextCh].WorldQuaternion = QuaterMulti(worldName.mTransforms[pID].WorldQuaternion, worldName.mTransforms[nextCh].LocalQuaternion);
				worldName.mTransforms[nextCh].TransMatrix = translate(mat4(1.0f), worldName.mTransforms[nextCh].WorldPos)
					*QuaternionToRotationMatrix(worldName.mTransforms[nextCh].WorldQuaternion)
					*scale(mat4(1.0f), worldName.mTransforms[nextCh].WorldScale);
			}
#endif // !WANG

		}
	}
}

void C_PhysicsSystem::IntegrateVelocity(C_World &worldName, float deltaTime) {
	for (auto bd : mBodyList) {
		if (worldName.mBodies[bd.body].invMass == 0.0f)
		{
			continue;
		}
		if (!worldName.mBodies[bd.body].enable)
		{
			continue;
		}
		
		//pos
		worldName.mTransforms[bd.transform].LocalPos += worldName.mBodies[bd.body].velocity*deltaTime;
#ifdef PHY_ROTATION

		if (worldName.mBodies[bd.body].angularVelocity.z > 4* PI)
		{
			worldName.mBodies[bd.body].angularVelocity.z = 4 * PI;
		}
		else if (worldName.mBodies[bd.body].angularVelocity.z< -4* PI)
		{
			worldName.mBodies[bd.body].angularVelocity.z = -4 * PI;
		}

		vec4 q = GetQuater(vec4(0.0f, 0.0f, 1.0f, worldName.mBodies[bd.body].angularVelocity.z*deltaTime));

		vec4 &mQ = worldName.mTransforms[bd.transform].LocalQuaternion;
		mQ = QuaterMulti(q, mQ);
#endif
		//IntegrateForce(worldName, deltaTime);
	}
}

void C_PhysicsSystem::IntegrateForce(C_World &worldName, float deltaTime) {

	for (auto bd : mBodyList) {
		if (!worldName.mBodies[bd.body].enable)
		{
			continue;
		}
		if (worldName.mBodies[bd.body].invMass == 0.0f)
		{
			continue;
		}

		if (!worldName.mBodies[bd.body].kinematic) {
			worldName.mBodies[bd.body].totalForce.y -= worldName.gravity;
			worldName.mBodies[bd.body].accelaration = worldName.mBodies[bd.body].totalForce * worldName.mBodies[bd.body].invMass;
			worldName.mBodies[bd.body].velocity += worldName.mBodies[bd.body].accelaration*deltaTime;
			worldName.mBodies[bd.body].velocity *= worldName.mBodies[bd.body].velFriction;
		}
#ifdef PHY_ROTATION

		if (!worldName.mBodies[bd.body].kinematic) {
			worldName.mBodies[bd.body].angularVelocity += worldName.mBodies[bd.body].torque*worldName.mBodies[bd.body].invMass*deltaTime;
			worldName.mBodies[bd.body].angularVelocity *= worldName.mBodies[bd.body].velFriction;
		}


#endif //PHY_ROTATION
	}
}

void C_PhysicsSystem::ResolveCollision(C_World& world)
{

	// Calculate relative velocity
	for (auto ele : *list)
	{
		if (ele->isTrigger)
		{
			continue;
		}
		unsigned firstBodyID = ele->bodys[0];
		unsigned secondBodyID = ele->bodys[1];

		
		if (firstBodyID != -1 && secondBodyID != -1 && firstBodyID!= secondBodyID) {
			int firstID = world.mBodies[firstBodyID].ownerID;   //ele->colliderTransforms[0];//actually collider component ID
			int secondID = world.mBodies[secondBodyID].ownerID; //ele->colliderTransforms[1];

			vec3 ra = world.mTransforms[firstID].WorldPos;
			ra.z = 0.0f;
			vec3 rb = world.mTransforms[secondID].WorldPos;
			rb.z = 0.0f;
#ifdef PHY_ROTATION
			vec3 rv = world.mBodies[secondBodyID].velocity + glm::cross(world.mBodies[secondBodyID].angularVelocity, rb) - world.mBodies[firstBodyID].velocity - glm::cross(world.mBodies[firstBodyID].angularVelocity, ra);
			rv.z = 0.0f;
#endif
#ifndef PHY_ROTATION
			vec3 rv = world.mBodies[secondBodyID].mVel - world.mBodies[firstBodyID].mVel;
#endif // !PHY_ROTATION


			// Calculate relative velocity in terms of the normal direction
			float velAlongNormal = glm::dot(ele->normal, rv);

			// Do not resolve if velocities are separating
			if (velAlongNormal > 0)
				continue;

			vec3 raCrossN = glm::cross(ra, ele->normal);
			vec3 rbCrossN = glm::cross(rb, ele->normal);
#ifdef PHY_ROTATION
			float invMassSum = world.mBodies[firstBodyID].invMass + world.mBodies[secondBodyID].invMass + glm::dot(raCrossN, raCrossN) * world.mBodies[firstBodyID].invMass + glm::dot(rbCrossN, rbCrossN)* world.mBodies[secondBodyID].invMass;
#endif //PHY_ROTATION
#ifndef PHY_ROTATION
			float invMassSum = world.mBodies[firstBodyID].mInvMass + world.mBodies[secondBodyID].mInvMass;
#endif //PHY_ROTATION
			// Calculate restitution
			float e = std::fmin(world.mBodies[firstBodyID].restitution, world.mBodies[secondBodyID].restitution);

			// Calculate impulse scalar
			float j = -(1.0f + e) * velAlongNormal;
			j /= invMassSum;

			// Apply impulse
			vec3 impulse = j * ele->normal;

			world.mBodies[firstBodyID].velocity += world.mBodies[firstBodyID].invMass * (-impulse);
#ifdef PHY_ROTATION
			world.mBodies[firstBodyID].angularVelocity += world.mBodies[firstBodyID].invMass * glm::cross(ra, -impulse);
#endif //PHY_ROTATION

			world.mBodies[secondBodyID].velocity += world.mBodies[secondBodyID].invMass * impulse;
#ifdef PHY_ROTATION
			world.mBodies[secondBodyID].angularVelocity += world.mBodies[secondBodyID].invMass * glm::cross(rb, impulse);
#endif //PHY_ROTATION

#ifdef FRECTION
			//resolve frection
#ifdef PHY_ROTATION
			rv = world.mBodies[secondBodyID].velocity + glm::cross(world.mBodies[secondBodyID].angularVelocity, rb) - world.mBodies[firstBodyID].velocity - glm::cross(world.mBodies[firstBodyID].angularVelocity, ra);
#endif
#ifndef PHY_ROTATION
			rv = world.mBodies[secondBodyID].mVel - world.mBodies[firstBodyID].mVel;
#endif // !PHY_ROTATION
			// Solve for the tangent vector
			vec3 tangent = rv - glm::dot(rv, ele->normal) *  ele->normal;
			glm::normalize(tangent);

			// Solve for magnitude to apply along the friction vector
			float jt = -glm::dot(rv, tangent);
			jt /= invMassSum;
			if (jt < EPSILEN)
				return;
			// Use to approximate mu given friction coefficients of each body
			float mu = sqrtf(world.mBodies[firstBodyID].staticFriction*world.mBodies[firstBodyID].staticFriction + world.mBodies[secondBodyID].staticFriction*world.mBodies[secondBodyID].staticFriction);

			// Clamp magnitude of friction and create impulse vector
			vec3 frictionImpulse;
			if (abs(jt) < j * mu) {
				frictionImpulse = jt * tangent;
			}
			else
			{
				float dynamicFriction = sqrtf(world.mBodies[firstBodyID].dynamicFriction*world.mBodies[firstBodyID].dynamicFriction + world.mBodies[secondBodyID].dynamicFriction*world.mBodies[secondBodyID].dynamicFriction);
				frictionImpulse = -j * tangent * dynamicFriction;
			}

			// Apply
			world.mBodies[firstBodyID].velocity -= world.mBodies[firstBodyID].invMass * frictionImpulse;
#ifdef PHY_ROTATION
			world.mBodies[firstBodyID].angularVelocity -= world.mBodies[firstBodyID].invMass * glm::cross(ra, frictionImpulse);
#endif //PHY_ROTATION

			world.mBodies[secondBodyID].velocity += world.mBodies[secondBodyID].invMass * frictionImpulse;
#ifdef PHY_ROTATION
			world.mBodies[secondBodyID].angularVelocity += world.mBodies[secondBodyID].invMass * glm::cross(rb, frictionImpulse);
#endif //PHY_ROTATION

#endif // FRECTION
		}
		else if ((firstBodyID == -1 && secondBodyID == -1)|| firstBodyID == secondBodyID) {
			continue;
		}
		else
		{
			if (firstBodyID != -1)
			{
				int firstID = world.mBodies[firstBodyID].ownerID;   //ele->colliderTransforms[0];//actually collider component ID

				vec3 ra = world.mTransforms[firstID].WorldPos;
				ra.z = 0.0f;
				int firstBodyID = world.mEntities[firstID].mComponents[E_CP_BODY];
				//int secondBodyID = world.mEntities[secondID].mComponents[E_CP_BODY];

				vec3 rv = -world.mBodies[firstBodyID].velocity - glm::cross(world.mBodies[firstBodyID].angularVelocity, ra);

				// Calculate relative velocity in terms of the normal direction
				float velAlongNormal = glm::dot(ele->normal, rv);

				// Do not resolve if velocities are separating
				if (velAlongNormal > 0)
					continue;
				vec3 raCrossN = glm::cross(ra, ele->normal);
				// Calculate restitution
				float e = world.mBodies[firstBodyID].restitution;

				// Calculate impulse scalar
				float j = -(1 + e) * velAlongNormal;
				float invMassSum = world.mBodies[firstBodyID].invMass + glm::dot(raCrossN, raCrossN) * world.mBodies[firstBodyID].invMass;
				j /= invMassSum;
				//j = sqrtf(j);
				// Apply impulse
				vec3 impulse = j * ele->normal;
				world.mBodies[firstBodyID].velocity -= world.mBodies[firstBodyID].invMass * impulse;

#ifdef FRECTION
				//resolve frection
#ifdef PHY_ROTATION
				rv = - world.mBodies[firstBodyID].velocity;
#endif
#ifndef PHY_ROTATION
				rv = - world.mBodies[firstBodyID].mVel;
#endif // !PHY_ROTATION
				// Solve for the tangent vector
				vec3 tangent = rv - glm::dot(rv, ele->normal) *  ele->normal;
				glm::normalize(tangent);

				// Solve for magnitude to apply along the friction vector
				float jt = -glm::dot(rv, tangent);
				jt /= world.mBodies[firstBodyID].invMass;
				if (jt < EPSILEN)
					return;
				// Use to approximate mu given friction coefficients of each body
				float mu = world.mBodies[firstBodyID].staticFriction;

				// Clamp magnitude of friction and create impulse vector
				vec3 frictionImpulse;
				if (abs(jt) < j * mu) {
					frictionImpulse = jt * tangent;
				}
				else
				{
					float dynamicFriction = world.mBodies[firstBodyID].dynamicFriction;
					frictionImpulse = -j * tangent * dynamicFriction;
				}

				// Apply
				world.mBodies[firstBodyID].velocity -= world.mBodies[firstBodyID].invMass * frictionImpulse;
#ifdef PHY_ROTATION
				world.mBodies[firstBodyID].angularVelocity -= world.mBodies[firstBodyID].invMass * glm::cross(ra, frictionImpulse);
#endif //PHY_ROTATION
#endif // FRECTION

			}
			else
			{
				int secondID = world.mBodies[secondBodyID].ownerID;
				vec3 rb = world.mTransforms[secondID].WorldPos;
				rb.z = 0.0f;
				vec3 rv = world.mBodies[secondBodyID].velocity + glm::cross(world.mBodies[secondBodyID].angularVelocity, rb);

				// Calculate relative velocity in terms of the normal direction
				float velAlongNormal = glm::dot(ele->normal, rv);

				// Do not resolve if velocities are separating
				if (velAlongNormal > 0)
					continue;
				vec3 rbCrossN = glm::cross(rb, ele->normal);
				// Calculate restitution
				float e = world.mBodies[secondBodyID].restitution;

					// Calculate impulse scalar
				float j = -(1 + e) * velAlongNormal;
				float invMassSum = world.mBodies[secondBodyID].invMass + glm::dot(rbCrossN, rbCrossN) * world.mBodies[secondBodyID].invMass;
				j /= invMassSum;
				//j = sqrtf(j);
				// Apply impulse
				vec3 impulse = j * ele->normal;
				world.mBodies[secondBodyID].velocity += world.mBodies[secondBodyID].invMass * impulse;

#ifdef FRECTION
				//resolve frection
#ifdef PHY_ROTATION
				rv = world.mBodies[secondBodyID].velocity;
#endif
#ifndef PHY_ROTATION
				rv = world.mBodies[secondBodyID].mVel;
#endif // !PHY_ROTATION
				// Solve for the tangent vector
				vec3 tangent = rv - glm::dot(rv, ele->normal) *  ele->normal;
				glm::normalize(tangent);

				// Solve for magnitude to apply along the friction vector
				float jt = -glm::dot(rv, tangent);
				jt /= world.mBodies[secondBodyID].invMass;
				if (jt < EPSILEN)
					return;
				// Use to approximate mu given friction coefficients of each body
				float mu = world.mBodies[secondBodyID].staticFriction;

				// Clamp magnitude of friction and create impulse vector
				vec3 frictionImpulse;
				if (abs(jt) < j * mu) {
					frictionImpulse = jt * tangent;
				}
				else
				{
					float dynamicFriction = world.mBodies[secondBodyID].dynamicFriction;
					frictionImpulse = -j * tangent * dynamicFriction;
				}

				// Apply
				world.mBodies[secondBodyID].velocity += world.mBodies[secondBodyID].invMass * frictionImpulse;
#ifdef PHY_ROTATION
				world.mBodies[secondBodyID].angularVelocity += world.mBodies[secondBodyID].invMass * glm::cross(rb, frictionImpulse);
#endif //PHY_ROTATION
#endif // FRECTION

			}
		}
	}
}

void C_PhysicsSystem::CorrectPos(C_World& world) {

	for (auto ele : *list)
	{
		if (ele->isTrigger)
		{
			continue;
		}
		unsigned firstBodyID = ele->bodys[0];
		unsigned secondBodyID = ele->bodys[1];
		if (firstBodyID == -1 || secondBodyID == -1 || firstBodyID == secondBodyID)
		{
			continue;
		}
		int firstID = world.GetComponent(world.mBodies[firstBodyID].ownerID, E_CP_TRANSFORM); // body component ID
		int secondID = world.GetComponent(world.mBodies[secondBodyID].ownerID, E_CP_TRANSFORM);
		if (firstBodyID != -1 && secondBodyID != -1) {

			//positional correction
			//vec3 correction = std::fmaxf(0, ele->depth - slop) / (world.mBodies[firstBodyID].invMass + world.mBodies[secondBodyID].invMass) * percent * ele->normal;
			vec3 correction =  ele->depth/ (world.mBodies[firstBodyID].invMass + world.mBodies[secondBodyID].invMass) * percent * ele->normal;
			world.mTransforms[world.mEntities[firstID].mComponents[E_CP_TRANSFORM]].LocalPos -= world.mBodies[firstBodyID].invMass * correction;
			world.mTransforms[world.mEntities[secondID].mComponents[E_CP_TRANSFORM]].LocalPos += world.mBodies[secondBodyID].invMass * correction;

		}

	}

}

void C_PhysicsSystem::ClearForce(C_World& worldName) {
	for (auto bd : mBodyList) {
		if (worldName.mBodies[bd.body].enable == false)
		{
			continue;
		}
		if (worldName.mBodies[bd.body].invMass == 0.0f)
		{
			continue;
		}
		// reset the force
		//float Vnorm = glm::length(worldName.mBodies[bd.body].velocity);
		//// resistance = Fw=Cw¡¤v^2
		//if (!worldName.mBodies[bd.body].kinematic)
		//	worldName.mBodies[bd.body].totalForce = -stopPara * worldName.miu*worldName.mBodies[bd.body].velocity*Vnorm;
#ifdef PHY_ROTATION
		//Vnorm = glm::length(worldName.mBodies[bd.body].angularVelocity);
		//// resistance = Fw=Cw¡¤v^2
		//if (!worldName.mBodies[bd.body].kinematic)
		//	worldName.mBodies[bd.body].torque = -angularStopPara * worldName.miu*worldName.mBodies[bd.body].angularVelocity*Vnorm;
		worldName.mBodies[bd.body].totalForce = vec3(0.0f);
		worldName.mBodies[bd.body].torque = vec3(0.0f);
#endif
	}

}

void C_PhysicsSystem::Update(C_World &worldname, float deltaTime)
{
	deltaTime = deltaTime < 0.04f ? deltaTime : 0.03f;
	IntegrateForce(worldname, deltaTime);
	IntegrateVelocity(worldname, deltaTime);
	ClearForce(worldname);
	UpdateWorldPosition(worldname);
	CollisionSystem->Update(worldname, 0.0f);
	ResolveCollision(worldname);
	CorrectPos(worldname);
}

void C_PhysicsSystem::Register(C_World &worldname, std::vector<int> &entityList)
{
	for (auto i : entityList)
	{
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_BODY)).any())
		{
			S_PhysicsRegister newphyMem = { worldname.mEntities[i].mComponents[E_CP_BODY] ,
										  worldname.mEntities[i].mComponents[E_CP_TRANSFORM] };
			mBodyList.insert(newphyMem);
		}
		transfromList.insert(worldname.mEntities[i].mComponents[E_CP_TRANSFORM]);

		Attach(worldname, worldname.mTransforms[worldname.mEntities[i].mComponents[E_CP_TRANSFORM]].parentID, i);
	}
}

void C_PhysicsSystem::Unregister(C_World &worldname, std::vector<int> &entityList)
{
	for (auto i : entityList)
	{
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_BODY)).any())
		{
			S_PhysicsRegister delphyMem = { worldname.mEntities[i].mComponents[E_CP_BODY] ,
										  worldname.mEntities[i].mComponents[E_CP_TRANSFORM] };
			mBodyList.erase(delphyMem);
		}
		transfromList.erase(worldname.mEntities[i].mComponents[E_CP_TRANSFORM]);
		Unattach(worldname, i);
	}
}

void SeparatingAxisTheorem(C_World &worldname, unsigned A, unsigned B) {




}
