#include <raylib.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <cmath>
#include <algorithm>
#include <map>
#include <fstream>
#include "gui.h"
#include "Object.h"
#include "Player.h"
#include "Planet.h"

const float Plastic = 2200;
const float Gold = 19320;
const int Camera_width = 1280;
const int Camera_height = 960;
const double gravity = 0.001;

float GetDistance(Vector2 vec1, Vector2 vec2) {
	float distance = sqrt(std::pow((vec1.x - vec2.x), 2) + std::pow((vec1.y - vec2.y), 2));
	return distance;
}

Vector2 GetVector(Vector2 vec1, Vector2 vec2) {
	float d = GetDistance(vec1, vec2);
	if (d < 0.00001) {
		return { 0, 0 };
	}
	float x = vec1.x - vec2.x;
	float y = vec1.y - vec2.y;
	float dx = x / d;
	float dy = y / d;
	return { dx, dy };
}

std::vector<Orbit*>Orbits;


Player sun({ 320, 360 }, 30, YELLOW, 10000);

std::vector<Object>Objects;
std::vector<Planet>Planets;


void CreateObject(Vector2 vec, float w, float h, Color color, float m) {
	Objects.emplace_back(vec, w, h, color, m);
}
void CreateOrbit(Orbit*& orbit, float radius, float mass, float distance, Color color, Planet* pointer) {
	orbit = new Orbit(radius, mass, distance, color, pointer);
	Orbits.push_back(orbit);
}

void Draw_Objects() {
	for (auto& o : Objects) {
		DrawRectangle(o.GetVector().x, o.GetVector().y, o.GetScaleWidht(), o.GetScaleHeight(), o.GetColor());
	}
}


void Gravity_init() {
	if (Objects.empty()) return;
	for (auto& s : Objects) {
		float distance = GetDistance(sun.GetPos(), s.GetCenter());
		if (distance < 0.0000001f) {
			distance = 0.000001f;
		}

		float F = gravity * s.GetMass() * sun.GetMass() / (distance * distance);
		Vector2 vec = GetVector(s.GetVector(), sun.GetPos());
		s.Move(-vec.x * F, -vec.y * F);
	}

}

void CreateMoreObjects(int i, Camera2D* cam) {
	if (i == 0) return;
	Vector2 vec = GetMousePosition();
	Vector2 World = GetScreenToWorld2D(vec, *cam);
	CreateObject({ World.x + (5 * i), World.y + (5 * i) }, 3, 3, WHITE, 100);
	CreateMoreObjects(i - 1, cam);
}
void CreateMoreMoreObjects(int i, Camera2D* cam) {
	if (i == 0) return;
	Vector2 vec = GetMousePosition();
	Vector2 World = GetScreenToWorld2D(vec, *cam);
	CreateObject({ World.x / 2, World.y / 2 }, 3, 3, WHITE, 100);
	CreateMoreObjects(i - 1, cam);
}

void CreateMoreMoreMoreObjects(int i, Camera2D* cam) {
	if (i == 0) return;
	Vector2 vec = GetMousePosition();
	Vector2 World = GetScreenToWorld2D(vec, *cam);
	CreateObject({ World.x / 4096, World.y / 4096 }, 3, 3, WHITE, 100);
	CreateMoreObjects(i - 1, cam);

}

int CreatePlanet(Vector2 pos, float radius, float mass, float temperature, std::string name) {
	Planets.emplace_back(pos, radius, temperature, mass, name);
	return Planets.size() - 1;
}

void DeletePlanet(Planet*& planet) {
	delete planet;
	planet = nullptr;
}

double zoom = 1;
float speed = 100 * GetFrameTime();
float dx, dy;

void RenderPlanets() {
	for (auto& p : Planets) {
		DrawCircle(p.GetPosition().x, p.GetPosition().y, p.GetRadius(), p.GetColor());
		for (auto& o : Orbits) {
			DrawCircle(o->GetPosition().x, o->GetPosition().y, o->GetRadius(), o->GetColor());
			o->Gravity_init();
		}
	}
}

struct Tree {
	Planet* Data;
	Tree* Left;
	Tree* Right;
	Tree(Planet* d) : Left(nullptr), Right(nullptr), Data(d) {}
	~Tree() {
		delete Left;
		delete Right;
	}

};
void push(Planet* p, Tree*& t) {
	if (t == nullptr) {
		t = new Tree(p);
	}
	if (p == nullptr) {
		return;
	}
	if (p->GetRadius() < t->Data->GetRadius()) {
		if (t->Left == nullptr) {
			t->Left = new Tree(p);
		}
		else {
			push(p, t->Left);
		}

	}
	else if (p->GetRadius() > t->Data->GetRadius()) {
		if (t->Right == nullptr) {
			t->Right = new Tree(p);
		}
		else {
			push(p, t->Right);
		}
	}
}

Tree* tree;

std::vector<Planet*>Top;

std::ofstream file("top_planet.txt");

void inOrderPrint(Tree* root) {
	if (root == nullptr) {
		file.close();
		return;
	}
	inOrderPrint(root->Left);
	Top.push_back(root->Data);
	inOrderPrint(root->Right);
}



int main() {
	srand(static_cast<unsigned int>(time(nullptr)));
	InitWindow(Camera_width, Camera_height, "Space Sandbox");

	SetTargetFPS(60);

	Camera2D camera = { 0 };
	camera.target = sun.GetPos();
	//camera.zoom = 1;
	camera.offset = { Camera_width / 2, Camera_height / 2 };

	int first_planet = CreatePlanet({ 11000, 200 }, 10000, 5000000, 4000, "Earth");
	int second_planet = CreatePlanet({ 1100000, 200 }, 1000000, 500000000, 4000, "Jupiter");
	int Universe = CreatePlanet({ 110000000, 200 }, 100000000, 50000000000, 12000, "Milki_way");
	int Multi_Universe = CreatePlanet({ 10e9, 10e9 }, 1000000000, 5000000000000, 12000, "Universe");
	CreateOrbit(Planets[first_planet].orbit, 2000, 5000, 8000, RED, &Planets[first_planet]);
	CreateOrbit(Planets[first_planet].orbit, 1000, 400, 14000, YELLOW, &Planets[first_planet]);
	std::string message;
	Window jopa({ 0, 0 }, Camera_width, 100, { 130, 130, 130, 128 }, WHITE, message, {0, 0}, 20, flag::ALL);

	tree = new Tree(&Planets[second_planet]);
	for (auto& p : Planets) {
		push(&p, tree);
	}
	inOrderPrint(tree);
	for (auto& t : Top) {
		std::string str = "Planet: " + t->name + "\tRadius: " + std::to_string(t->GetRadius()) + "\n";
		file << str.c_str();
	}
	std::cout << Top[0]->GetRadius() << std::endl;
	file.close();
	while (!WindowShouldClose()) {
		Vector2 vec = GetMousePosition();
		Vector2 worldPos = GetScreenToWorld2D(vec, camera);
		if (IsKeyDown(KEY_UP)) {
			dy = -speed;
		}
		else if (IsKeyDown(KEY_DOWN)) {
			dy = speed;
		}
		else {
			dy = 0;
		}
		if (IsKeyDown(KEY_RIGHT)) {
			dx = speed;
		}
		else if (IsKeyDown(KEY_LEFT)) {
			dx = -speed;
		}
		else {
			dx = 0;
		}
		sun.Move(dx, dy);
		camera.target = sun.GetPos();
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {

			CreateObject(worldPos, 3, 3, WHITE, 100);
		}
		if (IsKeyDown(KEY_Q)) {

			CreateMoreObjects(10, &camera);
		}
		if (IsKeyDown(KEY_E)) {

			CreateMoreMoreObjects(100, &camera);
		}
		if (IsKeyDown(KEY_T)) {
			CreateMoreMoreMoreObjects(10000, &camera);
		}
		if (IsKeyDown(KEY_C)) {
			Objects.clear();
		}

		if (IsKeyPressed(KEY_O)) {
			zoom -= 0.1;
			if (zoom < 0.1) {
				zoom -= 0.0001;
			}
			if (zoom < 0.0000001) {
				zoom = 0.0000001;
			}
		}
		else if (IsKeyPressed(KEY_K)) {
			zoom -= 0.0001;
			if (zoom < 0.000001) {
				zoom = 0.0000001;
			}
		}
		else if (IsKeyPressed(KEY_M)) {
			zoom -= 0.00001;
			if (zoom < 0.000001) {
				zoom = 0.0000001;
			}
		}
		else if (IsKeyDown(KEY_P)) {
			zoom += 0.01;
			if (zoom > 1) {
				zoom += 0.0001;
			}
		}
		if (IsKeyDown(KEY_Z)) {
			speed = 1000 * GetFrameTime();
		}
		else if (IsKeyDown(KEY_V)) {
			speed = 10000 * GetFrameTime();
		}
		else if (IsKeyDown(KEY_B)) {
			speed = 10e5 * GetFrameTime();
		}
		else {
			speed = 100 * GetFrameTime();
		}

		camera.zoom = zoom;



		BeginDrawing();


		ClearBackground(BLACK);

		BeginMode2D(camera);
		Gravity_init();
		Draw_Objects();
		DrawCircle(sun.GetPos().x, sun.GetPos().y, sun.GetRadius(), sun.GetColor());
		RenderPlanets();
		EndMode2D();



		float planets = Objects.size();
		message = "Objects: " + std::to_string(planets) + "\nX: " + std::to_string(sun.GetPos().x) + "\tY: " + std::to_string(sun.GetPos().y) + "\nZoom: " + std::to_string(zoom);

		jopa.Render();

		EndDrawing();
	}
}