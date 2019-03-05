#include "MainApplication.h"
#include "Urho3D/Scene/Scene.h"
#include "Urho3D/Graphics/Camera.h"
#include "Urho3D/Graphics/Renderer.h"
#include "Urho3D/Graphics/Octree.h"
#include "Urho3D/Graphics/Model.h"
#include "Urho3D/Graphics/StaticModel.h"
#include "Urho3D/Graphics/Material.h"
#include "Urho3D/Graphics/Skybox.h"
#include "Urho3D/Graphics/RenderPath.h"
#include "Urho3D/Resource/ResourceCache.h"
#include "Urho3D/Graphics/Zone.h"
#include "Urho3D/Core/Timer.h"
#include "Ledext/Urho/AppManager.h"

MainApplication::MainApplication(Context* context) : App (context){}

void MainApplication::OnStart(){

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	scene = AppManager::GetScene();

	Node* zoneNode = scene->CreateChild("Zone");
	Zone* zone = zoneNode->CreateComponent<Zone>();
	zone->SetBoundingBox(BoundingBox(-10000.0f, 10000.0f));
	zone->SetAmbientColor(Color(0.5f,0.5f,0.5f));
	zone->SetFogColor(Color(0,0,0,1));

	Node* lightNode = scene->CreateChild();
	lightNode->SetDirection(Vector3::FORWARD);
	lightNode->Yaw(50);     // horizontal
	lightNode->Pitch(10);   // vertical
	Light* light=lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);
	light->SetBrightness(1.0f);
	light->SetColor(Color(1,1,1,1));
	light->SetCastShadows(true);

	boxNode = scene->CreateChild("Box");
	boxNode->SetPosition(Vector3(0,0,20));
	boxNode->SetScale(Vector3(10,10,10));

	Material* material = new Material(context_);
	material->SetFillMode(FillMode::FILL_SOLID);
	material->SetShaderParameter("MatDiffColor", Color(1,0,1));

/*	for(int x=-30;x<30;x+=3){
		for(int z=0;z<60;z+=3){
			Node* boxNode_ = scene->CreateChild("Box");
			boxNode_->SetPosition(Vector3(x,-3,z));
			boxNode_->SetScale(Vector3(2,2,2));
			StaticModel* boxObject=boxNode_->CreateComponent<StaticModel>();
			boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
			boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
			boxObject->SetCastShadows(true);
		}
	} */

	StaticModel* boxObject = boxNode->CreateComponent<StaticModel>();
	boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
//	boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
	boxObject->SetMaterial(material);

/*	Node* skyNode=scene->CreateChild("Sky");
	skyNode->SetScale(500.0f); // The scale actually does not matter
	Skybox* skybox=skyNode->CreateComponent<Skybox>();
	skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml")); */

	Camera* camera = AppManager::GetCamera();
	cameraNode = camera->GetNode();
	cameraNode->SetPosition(Vector3(0,10,0));
	cameraNode->LookAt(boxNode->GetPosition()-Vector3(0,1,0), Vector3::UP);
	camera->SetFarClip(2000);

}

void MainApplication::OnUpdate(){
	float timestep = GetSubsystem<Time>()->GetTimeStep();
	Quaternion quat;
	quat.FromEulerAngles(0, timestep*90.0f, 0);
	boxNode->Rotate(quat);
}
