#include <iostream>
#include <memory>
#include <math.h>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <vld.h> 
#include "gdge.h"

static const int MAX_LIGHT_COUNT = 16;

const Uint8* keystates = SDL_GetKeyboardState(NULL);

int main(int argc, char** argv)
{
	Display display(1280, 720, "Game");

	Renderer renderer(&display);
	ResourceManager resManager;
	Scene scene;

	scene.AddComponent(new PhysicsWorldComponent(btVector3(0, -10, 0)));

	Texture* texture = resManager.LoadResourceFromFile<Texture>("./res/ground.jpg");
	Texture* sTex = resManager.LoadResourceFromFile<Texture>("./res/sTex.png");

	Material* defM = new Material(texture);
	resManager.AddResource(defM);
	Material* sMat = new Material(sTex, glm::vec4(1, 0, 0, 0), 30, 70);
	resManager.AddResource(sMat);

	Texture fontTex("./res/font_small.png", false);
	Font font("./res/font_small.fnt", fontTex, 512, 64);

	Mesh* groundMesh = MeshCreator::GetPlaneMesh(1024, 1024, 100);
	resManager.AddResource(groundMesh);
	Mesh* cubeMesh = MeshCreator::GetCubeMesh(1, 1, 1);
	resManager.AddResource(cubeMesh);
	Mesh* sphereMesh = resManager.LoadResourceFromFile<Mesh>("./res/sphere.obj");

	btCollisionShape* sphereShape = new btSphereShape(1);

	Object* groundObject = scene.CreateObject();
	groundObject->AddComponent(new TransformComponent(Transform()));
	groundObject->GetComponent<TransformComponent>()->GetTransform()->SetPos(glm::vec3(0, -5, 0));
	groundObject->GetComponent<TransformComponent>()->GetTransform()->UpdateTransform();
	groundObject->AddComponent(new MeshComponent(groundMesh));
	groundObject->AddComponent(new MaterialComponent(defM));
	groundObject->AddComponent(new RigidBodyComponent(groundObject->GetComponent<MeshComponent>()->GetMesh()->GetCollisionShape(), groundObject->GetComponent<TransformComponent>()->GetTransform(), 0, glm::vec3(0, 0, 0)));
	scene.GetComponent<PhysicsWorldComponent>()->GetDynamicsWorld()->addRigidBody(groundObject->GetComponent<RigidBodyComponent>()->GetRigidBody());
	groundObject->GetComponent<RigidBodyComponent>()->GetRigidBody()->setRestitution(0.9);
	groundObject->GetComponent<RigidBodyComponent>()->GetRigidBody()->setFriction(0.15);


	Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, -5.0f), 70.0f, display.GetAspect(), 0.1f, 1000.0f);

	scene.SetCamera(camera);

	for (int i = 0; i < 2; i++)
	{
		Object* cube = scene.CreateObject();
		cube->AddComponent(new MeshComponent(cubeMesh));
		cube->AddComponent(new TransformComponent(Transform()));
		cube->AddComponent(new RigidBodyComponent(cube->GetComponent<MeshComponent>()->GetMesh()->GetCollisionShape(), cube->GetComponent<TransformComponent>()->GetTransform(), 10, glm::vec3(0, 0, 0)));
		scene.GetComponent<PhysicsWorldComponent>()->GetDynamicsWorld()->addRigidBody(cube->GetComponent<RigidBodyComponent>()->GetRigidBody());
		cube->AddComponent(new MaterialComponent(defM));
	}

	Object* dCube = scene.CreateObject();
	dCube->AddComponent(new MeshComponent(cubeMesh));
	dCube->AddComponent(new TransformComponent(Transform()));
	dCube->GetComponent<TransformComponent>()->GetTransform()->SetPos(glm::vec3(0, 20, 0));
	dCube->GetComponent<TransformComponent>()->GetTransform()->UpdateTransform();
	dCube->AddComponent(new MaterialComponent(defM));
	dCube->AddComponent(new RigidBodyComponent(dCube->GetComponent<MeshComponent>()->GetMesh()->GetCollisionShape(), dCube->GetComponent<TransformComponent>()->GetTransform(), 10, glm::vec3(0, 0, 0)));
	scene.GetComponent<PhysicsWorldComponent>()->GetDynamicsWorld()->addRigidBody(dCube->GetComponent<RigidBodyComponent>()->GetRigidBody());

	Object* light1 = scene.CreateObject();
	light1->AddComponent(new LightComponent(glm::vec3(0.0, 1.0, 8.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), -32.0, 1.0, 32.0, 1.0, 0.1, 0.000001));
	light1->GetComponent<LightComponent>()->GetLight()->SetShadow(new Shadow());

	Object* light2 = scene.CreateObject();
	light2->AddComponent(new LightComponent(glm::vec3(23.0, 1.0, 8.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), -32.0, 1.0, 32.0, 1.0, 0.1, 0.000001));
	light2->GetComponent<LightComponent>()->GetLight()->SetShadow(new Shadow());

	Object* light3 = scene.CreateObject();
	light3->AddComponent(new LightComponent(glm::vec3(-23.0, 1.0, -8.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.7, 0.9), -32.0, 1.0, 32.0, 1.0, 0.1, 0.000001));
    light3->GetComponent<LightComponent>()->GetLight()->SetShadow(new Shadow());









	SDL_Event e;
	bool isRunning = true;

	glm::vec3 pos(0, 0, 0);
	glm::vec3 forward(0, 0, 0);
	glm::vec2 mouseMove(0, 0);
	glm::vec3 cubePos(5.5, -4, 2.7);
	float curSpeed = 0;
	float moveSpeed = 0.7;
	float yaw = 0;
	float pitch = 0;

	bool pauseFrustum = false;


	double lastTime = SDL_GetTicks();
	double timeBehind = 0.0;
	double targetTimeStep = 1000.0 / 60.0;
	
	std::string fpsst = ("FPS: " + std::to_string((int)*display.GetFPS()));
	std::string objectst = ("OBJECTS: " + std::to_string((int)scene.GetObjects()->size()) + " / 8192");
	std::string helloguysL = "qwertyuiopasdfghjklzxcvbnm1234567890.,!?{}\/-+";
	std::string frustumL = "FRUSTUM PAUSED!";
	Label fps(&fpsst, glm::vec4(1, 1, 1, 1), &font, &display, -0.95f, 0.9f, 16);
	Label objects(&objectst, glm::vec4(1, 1, 1, 1), &font, &display, -0.95f, 0.8f, 16);
	Label fr(&frustumL, glm::vec4(1, 0, 0, 1), &font, &display, -0.95f, 0.7f, 16);
	Label hello(&helloguysL, glm::vec4(1, 1, 1, 1), &font, &display, -0.95f, 0.6f, 16);

	std::cout << "RAND: " << rand() << std::endl;

	while (isRunning)
	{

		display.UpdateFPSCounter();

		timeBehind += SDL_GetTicks() - lastTime;
		lastTime = SDL_GetTicks();
		fpsst = ("FPS: " + std::to_string((int)*display.GetFPS()));
		objectst = ("OBJECTS: " + std::to_string((int)scene.GetObjects()->size()) + " / 8192");
		//LOGIC
		while (timeBehind >= targetTimeStep)
		{

			//CAMERA AND EVENTS
			mouseMove.x = 0;
			mouseMove.y = 0;

			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_MOUSEMOTION:
					mouseMove.x = -e.motion.xrel;
					mouseMove.y = -e.motion.yrel;
					break;
				case SDL_MOUSEBUTTONDOWN:
					SDL_SetRelativeMouseMode(SDL_TRUE);
					break;
				case SDL_MOUSEBUTTONUP:
					if (e.button.button == SDL_BUTTON_MIDDLE)
					{
						Object* cube = scene.CreateObject();
						cube->AddComponent(new MeshComponent(sphereMesh));
						cube->AddComponent(new TransformComponent(Transform()));
						cube->AddComponent(new MaterialComponent(sMat));

						cube->GetComponent<TransformComponent>()->GetTransform()->SetPos(glm::vec3(scene.GetCamera()->GetPos()->x, scene.GetCamera()->GetPos()->y, scene.GetCamera()->GetPos()->z));
						cube->GetComponent<TransformComponent>()->GetTransform()->UpdateTransform();
					}
					break;
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE)
						SDL_SetRelativeMouseMode(SDL_FALSE);
					break;
				case SDL_KEYUP:
					if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_s)
						curSpeed = 0;
					if (e.key.keysym.sym == SDLK_c)
						std::cout << "Camera at: x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << std::endl;
					if (e.key.keysym.sym == SDLK_f)
						if (pauseFrustum) pauseFrustum = false;
						else pauseFrustum = true;
					break;
				case SDL_QUIT:
					isRunning = false;
					break;

				default:
					break;
				}
			}

			if (keystates[SDL_SCANCODE_W])
				if (curSpeed < moveSpeed) curSpeed += 0.1;
			if (keystates[SDL_SCANCODE_S])
				if (curSpeed > -moveSpeed) curSpeed -= 0.1;

			
			if (keystates[SDL_SCANCODE_UP])
				cubePos.z -= 0.1;
			if (keystates[SDL_SCANCODE_DOWN])
				cubePos.z += 0.1;
			if (keystates[SDL_SCANCODE_LEFT])
				cubePos.x -= 0.1;
			if (keystates[SDL_SCANCODE_RIGHT])
				cubePos.x += 0.1;

			if (keystates[SDL_SCANCODE_SPACE])
			{
				Object* cube = scene.CreateObject();

				cube->AddComponent(new MeshComponent(sphereMesh));
				cube->AddComponent(new TransformComponent(Transform()));
				cube->AddComponent(new MaterialComponent(sMat));

				cube->GetComponent<TransformComponent>()->GetTransform()->SetPos(glm::vec3(scene.GetCamera()->GetPos()->x, scene.GetCamera()->GetPos()->y, scene.GetCamera()->GetPos()->z));
				cube->GetComponent<TransformComponent>()->GetTransform()->UpdateTransform();

				cube->AddComponent(new RigidBodyComponent(sphereShape, cube->GetComponent<TransformComponent>()->GetTransform(), 50, glm::vec3(0, 0, 0)));
				scene.GetComponent<PhysicsWorldComponent>()->GetDynamicsWorld()->addRigidBody(cube->GetComponent<RigidBodyComponent>()->GetRigidBody());
				cube->GetComponent<RigidBodyComponent>()->GetRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
				cube->GetComponent<RigidBodyComponent>()->GetRigidBody()->setRestitution(0.6);
				cube->GetComponent<RigidBodyComponent>()->GetRigidBody()->setFriction(0.85);
			}

			if (SDL_GetRelativeMouseMode())
			{
				yaw += mouseMove.x / 4;
				pitch += mouseMove.y / 4;
			}

			if (pitch >= 89) pitch = 89;
			if (pitch <= -89) pitch = -89;

			forward.x = sin(yaw * M_PI / 180.0);
			forward.z = cos(yaw * M_PI / 180.0);
			forward.y = tan(pitch * M_PI / 180.0);

			pos.x += curSpeed * forward.x;
			pos.y += curSpeed * forward.y;
			pos.z += curSpeed * forward.z;

			scene.GetCamera()->SetPosition(pos);
			scene.GetCamera()->SetForward(glm::normalize(forward));

			//GAME LOGICS

			if (!pauseFrustum)
				camera->GetFrustum()->CalculateFrustum(scene.GetCamera()->GetProjectionMatrix(), scene.GetCamera()->GetViewMatrix());

			scene.Update();

			timeBehind -= targetTimeStep;
		}

		

		renderer.RenderScene(&scene);


		//RENDER GUI
		//Label(std::string* text, float x, float y, glm::vec4* color, Font* font, Display* display, float size, float gap = 0);


		//renderer.RenderString(&fps);
		//renderer.RenderString(&objects);
		//renderer.RenderString(&hello);
		//renderer.RenderString((std::string) ("FPS: " + std::to_string((int) *display.GetFPS())), -0.95, 0.9, 16, 0, glm::vec4(1, 1, 1, 1), font);
		
		//renderer.RenderString((std::string) ("OBJECTS: " + std::to_string((int) scene.GetObjects()->size()) + " / 8192" ), -0.95, 0.8, 16, 0, glm::vec4(1, 1, 1, 1), font);

		if (pauseFrustum)
			renderer.RenderString(&fr);

		//renderer.RenderString((std::string) ("Hello guys, its me !"), 0.5, 0.6, 16, 0, glm::vec4(1, 1, 1, 1), font);

		display.Update();
		SDL_Delay(0);
	}

	scene.ClearScene();

	delete sphereShape;

	delete camera;

	return 0;
}
