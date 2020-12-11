#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "BacchusEditor.h"
#include "ModuleInput.h"
#include "ModuleSceneManager.h"
#include "ModuleRenderer3D.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "ResourceMesh.h"

#include "mmgr/mmgr.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	camera = new ComponentCamera(nullptr);
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init(json config)
{
	App->renderer3D->SetActiveCamera(camera);
	App->renderer3D->SetCullingCamera(camera);

	return true;
}

bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	camera->frustum.pos = { 0.0f,1.0f,-5.0f };

	reference = camera->frustum.pos;

	camera->update_projection = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	delete camera;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->GetAppState() == AppState::EDITOR)
	{
		speed = 10.0f * dt;
		float3 newPos(0, 0, 0);

		//SHIFT: Speed 2x
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed *= 2.0f;

		if (!App->bacchuseditor->IsMouseCaptured() && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			//WASD: Free movement

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += camera->frustum.front * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= camera->frustum.front * speed;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= camera->frustum.WorldRight() * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += camera->frustum.WorldRight() * speed;

			//RIGHT CLICK: Look Around
			LookAround(speed, camera->frustum.pos);
		}

		camera->frustum.pos += newPos;
		reference += newPos;

		//MOUSE WHEEL PRESS: Pan
		if (App->input->GetMouseButton(2) == KEY_REPEAT)
		{
			Pan(speed);
		}

		//MOUSE WHEEL UP/DOWN: Zoom
		if (!App->bacchuseditor->IsMouseCaptured() && abs(App->input->GetMouseZ()) > 0)
		{
			Zoom(speed);
		}

		//ALT + RIGHT CLICK: Orbit
		if (!App->bacchuseditor->IsMouseCaptured() && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			LookAround(speed, reference);
		}

		//F: Focus on geometry
		if (!App->bacchuseditor->IsKeyboardCaptured() && App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			FrameObject(App->scene_manager->GetSelectedGameObjects());
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::LookAround(float speed, float3 reference)
{
	float dx = -App->input->GetMouseXMotion() * speed;
	float dy = -App->input->GetMouseYMotion() * speed;

	math::Quat rotationX = math::Quat::RotateAxisAngle(float3::unitY, dx * DEGTORAD);
	math::Quat rotationY = math::Quat::RotateAxisAngle(camera->frustum.WorldRight(), dy * DEGTORAD);
	math::Quat finalRotation = rotationX * rotationY;

	camera->frustum.up = finalRotation.Mul(camera->frustum.up).Normalized();
	camera->frustum.front = finalRotation.Mul(camera->frustum.front).Normalized();

	float distance = (camera->frustum.pos - reference).Length();
	camera->frustum.pos = reference + (-camera->frustum.front * distance);
	
	if (!reference.Equals(this->reference))
		this->reference = camera->frustum.pos + camera->frustum.front * (camera->frustum.pos).Length();
}

void ModuleCamera3D::Pan(float speed)
{
	int dx = -App->input->GetMouseXMotion();
	int dy = App->input->GetMouseYMotion();
	float factor = abs(camera->frustum.pos.y) / 100.0f;
	
	if (factor < 0.5f)
		factor = 0.5f;

	if (dx != 0)
	{
		camera->frustum.pos += speed * camera->frustum.WorldRight() * dx * factor;
		reference += speed * camera->frustum.WorldRight() * dx * factor;
	}

	if (dy != 0)
	{
		camera->frustum.pos += speed * camera->frustum.up * dy * factor;
		reference += speed * camera->frustum.up * dy * factor;
	}
}

void ModuleCamera3D::Zoom(float speed)
{
	float factor = abs(camera->frustum.pos.y);
	if (factor < 1.0f)
	{
		factor = 1.0f;
	}

	int mouse_wheel = App->input->GetMouseZ();
	float3 Movement = camera->frustum.front * mouse_wheel * speed * factor;
	camera->frustum.pos += Movement;
}

void ModuleCamera3D::FrameObject(GameObject* GO)
{
	if (GO)
	{
		ComponentTransform* transform = GO->GetComponent<ComponentTransform>(Component::ComponentType::Transform);
		ComponentMesh* mesh = GO->GetComponent<ComponentMesh>(Component::ComponentType::Mesh);

		if (mesh && mesh->resource_mesh)
		{
			AABB aabb;
			aabb.SetNegativeInfinity();
			aabb.Enclose(mesh->resource_mesh->Vertices, mesh->resource_mesh->VerticesSize);
			aabb.Translate(transform->GetPosition());
			float3 center = aabb.CenterPoint();

			reference.x = center.x;
			reference.y = center.y;
			reference.z = center.z;

			float3 Movement = camera->frustum.front * (2 * mesh->GetAABB().HalfDiagonal().Length());
			camera->frustum.pos = reference - Movement;
		
		}
	}
	
}