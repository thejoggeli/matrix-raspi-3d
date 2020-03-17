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
#include "Urho3D/Graphics/Graphics.h"
#include "Urho3D/Graphics/Texture2D.h"
#include "Urho3D/Graphics/Technique.h"
#include "Urho3D/Core/Timer.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Log.h"
#include "Ledlib/Math/Numbers.h"

static float brightness = 1.0f;
static Node* playerNode;
static Node* lightNode;
static Light* light;
static float speed = 0.1f;
static float height = 1.2f;

MainApplication::MainApplication(Context* context) : Ledlib3dApplication (context){

}

void MainApplication::OnSetup(){

	ResourceCache* cache = GetSubsystem<ResourceCache>();

	scene = GetScene();

	Node* zoneNode = scene->CreateChild("Zone");
	Zone* zone = zoneNode->CreateComponent<Zone>();
	zone->SetBoundingBox(BoundingBox(-10000.0f, 10000.0f));
	zone->SetAmbientColor(Color(0.25f,0.25f,0.25f));
	zone->SetFogColor(Color(0,0,0,1));

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
	boxNode->SetPosition(Vector3(0,0,0));
	boxNode->SetScale(Vector3(2,2,2));

	Texture* texture = cache->GetResource<Texture2D>("Textures/Earth/earth.jpg");

	Technique* diff = cache->GetResource<Technique>("Techniques/Diff.xml");

	Material* material = new Material(context_);
	material->SetFillMode(FillMode::FILL_SOLID);
	material->SetTechnique(0, diff);
//	material->SetShaderParameter("MatDiffColor", Color(1,0.1f,1));
	material->SetTexture(TextureUnit::TU_DIFFUSE, texture);

	StaticModel* sphereObject = boxNode->CreateComponent<StaticModel>();
	sphereObject->SetModel(cache->GetResource<Model>("Models/Sphere.mdl"));
//	sphereObject->SetMaterial(cache->GetResource<Material>("Materials/Earth.xml"));
	sphereObject->SetMaterial(material);

	playerNode = scene->CreateChild("Player");
	Camera* camera = GetCamera();
	cameraNode = camera->GetNode();
	cameraNode->SetParent(playerNode);

	playerNode->SetPosition(Vector3(0, height, 0));

	Quaternion q;
	q.FromAngleAxis(40, Vector3(1,0,0));
	cameraNode->Rotate(q);
	camera->SetFarClip(2000);

}

void MainApplication::OnStart(){

}

void MainApplication::OnUpdate(){
	float dt = GetSubsystem<Time>()->GetTimeStep();

	if(ClientManager::IsKeyDown(KeyCode::Left)){
		Quaternion rot;
		Vector3 up = (playerNode->GetPosition() - boxNode->GetPosition()).Normalized();
		rot.FromAngleAxis(-dt*45, up);
		playerNode->Rotate(rot);
	}
	if(ClientManager::IsKeyDown(KeyCode::Right)){
		Quaternion rot;
		Vector3 up = (playerNode->GetPosition() - boxNode->GetPosition()).Normalized();
		rot.FromAngleAxis(dt*45, up);
		playerNode->Rotate(rot);
	}

	// move forward
	playerNode->Translate(Vector3(0.0f, 0.0f, dt*speed));

	// keep height
	Vector3 up = (playerNode->GetPosition() - boxNode->GetPosition()).Normalized();
	playerNode->SetPosition(boxNode->GetPosition() + up*height);

	Vector3 playerUp = playerNode->GetRotation() * Vector3::UP;
	Vector3 normalUp = (playerNode->GetPosition() - boxNode->GetPosition()).Normalized();

	Quaternion dq;
	dq.FromRotationTo(playerUp, normalUp);

	playerNode->Rotate(dq);

/*	Quaternion playerRotation = playerNode->GetRotation();
	playerNode->SetPosition(playerRotation * Vector3::UP * height);
	playerNode->LookAt()
	playerNode->LookAt(Quaternion */

/*	if(ClientManager::IsKeyDown(KeyCode::Up)){
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
			break;
		}
	}
	Quaternion quat;
	quat.FromEulerAngles(rotx, roty, 0);
	boxNode->SetRotation(quat); */
}
