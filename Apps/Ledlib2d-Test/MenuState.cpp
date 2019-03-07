#include "MenuState.h"
#include "TestEntity.h"

#include "Ledlib2d/Scene/Scene.h"

void MenuState::OnStart(){
	GetScene()->CreateEntity<TestEntity>();
}
