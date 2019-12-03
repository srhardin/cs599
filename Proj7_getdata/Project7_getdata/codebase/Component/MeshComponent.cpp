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
#include"pch.h"
#include"Component/MeshComponent.h"


C_MeshComponent::C_MeshComponent():C_Component(), mName("") {

}

C_MeshComponent::~C_MeshComponent() {

}
void C_MeshComponent::Serialize(S_JsonValue & val) {
	mName = val["name"].asString();
}

void C_MeshComponent::Initialize() {
	mName = "";

}