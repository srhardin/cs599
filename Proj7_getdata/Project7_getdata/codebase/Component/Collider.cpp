/* Start Header -------------------------------------------------------
Copyright (C) 2019
DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute
of Technology is prohibited.
Language:<C++>
Platform:<windows 10>
Project:<GAM 541, Void Engine>
Author:<Wang Zhongqiu, Li Chunyan>
-End Header --------------------------------------------------------*/
#include "pch.h"
#include "Component/Collider.h"

unsigned C_Collider::pos = 0;
std::unordered_map<std::string, unsigned> C_Collider::maskMap;


C_Collider::C_Collider(ShapeType type) :mType(type), center(0), centerOffset(0), needCollide(true), isTrigger(false), mLayer(1), notCollideLayer(0), mBody(-1) {};

C_CircleCollider::C_CircleCollider() :C_Collider(CIRCLE), mRadius(0.0f) {}

C_CircleCollider::~C_CircleCollider() {}

void C_CircleCollider::Serialize(S_JsonValue & value)
{
	std::vector<std::string> names = value.GetMemberNames();
	for (std::string &itr : names)
	{
		if (itr == "mRadius")
		{
			mRadius = value[itr].asFloat();
		}
		else if (itr == "isTrigger")
			isTrigger = value[itr].asBool();

		else if (itr == "notCollideLayer")
		{
			S_JsonValue notCollideLayerVal = value[itr];

			for (int i = 0; i < notCollideLayerVal.size(); ++i)
			{
				std::string maskName = notCollideLayerVal[i].asString();
				if (maskName == "all")
				{
					notCollideLayer = UINT_MAX;
					break;
				}
				if (maskMap[maskName] == 0)
				{
					++pos;
					maskMap[maskName] = 1 << pos;
				}
				if ((notCollideLayer &maskMap[maskName]) != 0)
				{
					continue;
				}
				notCollideLayer += maskMap[maskName];
			}

		}
		else if (itr == "layer")
		{
			std::string mLayerName = value[itr].asString();
			if (maskMap[mLayerName] == 0)
			{
				++pos;
				maskMap[mLayerName] = 1 << pos;
			}
			mLayer = maskMap[mLayerName];
		}
	}
}

void C_CircleCollider::Initialize()
{
	centerOffset = vec3(0);
	isTrigger = false;
	mLayer = 1; 
	notCollideLayer = 0;
	mBody = -1;
	mRadius = 0.0f;
	center = vec3(0.0f);
}

C_Polygon::C_Polygon() :C_Collider(POLYGON), points(0), normals(0), quatanion(0) {};

C_Polygon::C_Polygon(ShapeType mType) :C_Collider(mType), points(0), normals(0), quatanion(0) {};

/*Serialize Polygon
  
							       normal[i]
								     ^
								      |
                  point[i+1]------------------------------------point[i]
							 |													  |
							 |													  |
							 |------------------------------------|
*/
void C_Polygon::Serialize(S_JsonValue &value) {
	std::vector<std::string> names = value.GetMemberNames();
	float width = 0.0f;
	float height = 0.0f;
	for (std::string & attribute : names) {
		if (attribute == "width")
		{
			width = value[attribute].asFloat();
			points.resize(4);
			normals.resize(4);
		}else if (attribute == "needCollide")
		{
			needCollide = value[attribute].asBool();
		}
		else if (attribute == "height")
		{
			height = value[attribute].asFloat();
		}
		else if(attribute == "points")
		{
			points.resize(value[attribute].size());
			normals.resize(value[attribute].size());
		}else if (attribute == "notCollideLayer")
		{
			S_JsonValue notCollideLayerVal = value[attribute];

			for (int i = 0; i < notCollideLayerVal.size(); ++i)
			{
				std::string maskName = notCollideLayerVal[i].asString();
				if (maskName == "all")
				{
					notCollideLayer = UINT_MAX;
					break;
				}
				if (maskMap[maskName] == 0)
				{
					++pos;
					maskMap[maskName] = 1 << pos;
				}
				if ((notCollideLayer &maskMap[maskName]) != 0)
				{
					continue;
				}
				notCollideLayer += maskMap[maskName];
			}

		}
		else if (attribute == "layer")
		{
			std::string mLayerName = value[attribute].asString();
			if (maskMap[mLayerName] == 0)
			{
				++pos;
				maskMap[mLayerName] = 1 << pos;
			}
			mLayer = maskMap[mLayerName];
		}
		
	}
	if (width != 0.0f)
	{
		points[0] = vec3(width / 2.0f, height / 2.0f, 0.0f);
		points[1] = vec3(-width / 2.0f, height / 2.0f,0.0f);
		points[2] = vec3(-width / 2.0f, -height / 2.0f, 0.0f);
		points[3] = vec3(width / 2.0f, -height / 2.0f, 0.0f);
		mType = BOX;
		mWidth = width;
		mHeight = height;
	}
	else {
		for (int i = 0; i < value["points"].size(); ++i)
		{
			points[i] = value["points"][i].asVec3();
		}
	}
	for (int i = 0; i < points.size(); ++i)
	{
		if (i == points.size()-1)
		{
			vec2 side = glm::normalize( points[0] - points[i]);
		normals[i] = vec3(side.y, -side.x, 0.0f);
		continue;
		}
		vec2 side = glm::normalize( points[i+1] - points[i]);
		normals[i] = vec3(side.y, -side.x, 0.0f);
	}
}

void C_Polygon::Initialize()
{
	centerOffset = vec3(0);
	isTrigger = false;
	mLayer = 1;
	notCollideLayer = 0;
	mBody = -1;
	center = vec3(0.0f);
	points.clear();
	normals.clear();
	quatanion = vec4(0);
}



void C_Polygon::SetWidthAndHeight(float width, float height) {
	mWidth = width;
	mHeight = height;
	
	points[0] = vec3(width / 2.0f, height / 2.0f, 0.0f);
	points[1] = vec3(-width / 2.0f, height / 2.0f, 0.0f);
	points[2] = vec3(-width / 2.0f, -height / 2.0f, 0.0f);
	points[3] = vec3(width / 2.0f, -height / 2.0f, 0.0f);


}