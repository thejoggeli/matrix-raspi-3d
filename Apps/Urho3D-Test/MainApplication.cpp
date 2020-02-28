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
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Log.h"
#include "Ledlib/Math/Numbers.h"

static float brightness = 1.0f;
static float rotx = 0,  roty = 0;
static Node* lightNode;
static Light* light;

MainApplication::MainApplication(Context* context) : Ledlib3dApplication (context){}

void MainApplication::OnSetup(){

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	scene = GetScene();

	Log("number 1");

	Node* zoneNode = scene->CreateChild("Zone");
	Zone* zone = zoneNode->CreateComponent<Zone>();
	zone->SetBoundingBox(BoundingBox(-10000.0f, 10000.0f));
	zone->SetAmbientColor(Color(0.25f,0.25f,0.25f));
	zone->SetFogColor(Color(0,0,0,1));

	Log("number 2");

	lightNode = scene->CreateChild();
	lightNode->SetDirection(Vector3::FORWARD);
	lightNode->Yaw(50);     // horizontal
	lightNode->Pitch(10);   // vertical
	light=lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);
	light->SetBrightness(1.0f);
	light->SetColor(Color(1,1,1,1));
	light->SetCastShadows(true);

	boxNode = scene->CreateChild("Box");
	boxNode->SetPosition(Vector3(0,0,20));
	boxNode->SetScale(Vector3(10,10,10));

	Material* material = new Material(context_);
	material->SetFillMode(FillMode::FILL_SOLID);
	material->SetShaderParameter("MatDiffColor", Color(1,0.1f,1));

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

	Camera* camera = GetCamera();
	cameraNode = camera->GetNode();
	cameraNode->SetPosition(Vector3(0,10,0));
	cameraNode->LookAt(boxNode->GetPosition()-Vector3(0,1,0), Vector3::UP);
	camera->SetFarClip(2000);

}

void MainApplication::OnStart(){

}

void MainApplication::OnUpdate(){
	float timestep = GetSubsystem<Time>()->GetTimeStep();
	if(ClientManager::IsKeyDown(KeyCode::Up)){
		brightness = Numbers::Clamp(0.0f, 2.0f, brightness + timestep);
		light->SetBrightness(brightness);
	}
	if(ClientManager::IsKeyDown(KeyCode::Down)){
		brightness = Numbers::Clamp(0.0f, 2.0f, brightness - timestep);
		light->SetBrightness(brightness);
	}
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			roty += joy.x * timestep * 360.0f;
			rotx += joy.y * timestep * 360.0f;
			Log(iLog << rotx << " / " << roty);
		//	Log(iLog << joy.x << " / " << joy.y);
			break;
		}
	}
	Quaternion quat;
	quat.FromEulerAngles(rotx, roty, 0);
	boxNode->SetRotation(quat);
}
