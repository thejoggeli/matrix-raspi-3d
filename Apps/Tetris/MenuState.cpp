#include "MenuState.h"
#include "GameState.h"
#include "Ledlib/Util/ClientJoiner.h"
#include "Ledlib/Util/ClientJoinerSlot.h"
#include "Ledlib2d/Game.h"

using namespace Ledlib;

void MenuState::OnStart(){
	ClientJoiner::Init(4, false);
}

void MenuState::OnUpdate(){
	ClientJoiner::Update();
	if(ClientJoiner::IsEveryoneReady() && ClientJoiner::GetNumTakenSlots() > 0){
		auto gameState = GetGame()->SetState<GameState>();
		for(auto slot: ClientJoiner::GetTakenSlots()){
			if(!slot->GetClient()) continue;
			gameState->AddPlayer(slot->GetClient());;
		}
	}

}
void MenuState::OnRender(){}
