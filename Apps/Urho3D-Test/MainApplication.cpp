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

MainApplication::MainApplication(Context * context) : Urho3DApplication (context){

}

void MainApplication::Start(){

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	scene = new Scene(context_);
	scene->CreateComponent<Octree>();

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

	box = scene->CreateChild("Box");
	box->SetPosition(Vector3(0,0,20));
	box->SetScale(Vector3(10,10,10));

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

	StaticModel* boxObject = box->CreateComponent<StaticModel>();
	boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
//	boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
	boxObject->SetMaterial(material);

/*	Node* skyNode=scene->CreateChild("Sky");
	skyNode->SetScale(500.0f); // The scale actually does not matter
	Skybox* skybox=skyNode->CreateComponent<Skybox>();
	skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml")); */

	camera = scene->CreateChild("Camera");
	camera->SetPosition(Vector3(0,10,0));
	camera->LookAt(box->GetPosition()-Vector3(0,1,0), Vector3::UP);
	Camera* c = camera->CreateComponent<Camera>();
	c->SetFarClip(2000);

	// Now we setup the viewport. Of course, you can have more than one!
	Renderer* renderer = GetSubsystem<Renderer>();
	SharedPtr<Viewport> viewport(new Viewport(context_, scene, camera->GetComponent<Camera>()));
	renderer->SetViewport(0, viewport);

	Urho3DApplication::Start();
}

void MainApplication::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData){
	Urho3DApplication::HandleUpdate(eventType, eventData);
	float timestep = GetSubsystem<Time>()->GetTimeStep();
	Quaternion quat;
	quat.FromEulerAngles(0, timestep*90.0f, 0);
	box->Rotate(quat);
}
