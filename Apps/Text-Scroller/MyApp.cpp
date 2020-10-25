#include "MyApp.h"
#include "Ledlib2d/Resources/ResourceManager.h"

using namespace Ledlib;

MyApp::MyApp(){}

void MyApp::OnSetup(){
    anim = std::make_unique<AnimSW>();
    ResourceManager::LoadFontBdf("font-1", "7x13B.bdf");
    ResourceManager::LoadFontBdf("font-2", "5x8.bdf");
    std::string text = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic";
    anim->SetText(text, ResourceManager::GetFont("font-2"));
}
void MyApp::OnStart(){
    anim->OnStart();
}
void MyApp::OnUpdate(){
    anim->OnUpdate();
}
void MyApp::OnRender(){
    anim->OnRender();
}
void MyApp::OnExit(){
    anim->OnExit();
}
