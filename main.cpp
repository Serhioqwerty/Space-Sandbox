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

const float Plastic = 2200;
const float Gold = 19320;
const double gravity = 0.001;
const int Camera_width = 1280;
const int Camera_height = 960;

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

class Object {
private:
	Vector2 vec;
	Vector2 center;
	float w, h, mass;
	Color color;
public:
	Object(Vector2 v, float w, float h, Color c, float m) : vec(v), w(w), h(h), color(c), mass(m) {
		this->center = { this->vec.x + this->w / 2, this->vec.y + this->h / 2 };
	}
	void Move(float dx, float dy) {
		this->vec.x += dx;
		this->center.x += dx;
		this->vec.y += dy;
		this->center.y += dy;
	}
	Vector2 GetVector() {
		return this->vec;
	}
	Vector2 GetCenter() {
		return this->center;
	}
	float GetScaleWidht() {
		return this->w;
	}
	float GetScaleHeight() {
		return this->h;
	}
	Color GetColor() {
		return this->color;
	}
	void SetPosition(float x, float y) {
		this->vec.x = x;
		this->vec.y = y;
		this->center = { this->vec.x + this->w / 2, this->vec.y + this->h / 2 };

	}
	float GetMass() {
		return this->mass;
	}
};



class Player {
private:
	Vector2 pos;
	float radius;
	Color color;
	float mass;
public:
	Player(Vector2 pos, float radius, Color color, float mass) {
		this->mass = mass;
		this->pos = pos;
		this->color = color;
		this->radius = radius;
	}
	void Move(float dx, float dy) {
		this->pos.x += dx;
		this->pos.y += dy;
	}

	Vector2 GetPos() {
		return this->pos;
	}
	void SetColor(Color color) {
		this->color = color;
	}
	void SetRadius(float n) {
		this->radius = n;
	}
	float GetRadius() {
		return this->radius;
	}
	Color GetColor() {
		return this->color;
	}
	void SetPos(float x, float y) {
		this->pos.x = x;
		this->pos.y = y;
	}
	float GetMass() {
		return this->mass;
	}
	void SetMass(float m) {
		this->mass = m;
	}
};

class Orbit;

std::vector<Orbit*>Orbits;

class Planet {
private:
	Vector2 position;
	float radius;
	Color color;
	float temperature;
	float mass;

public:
	std::string name;
	Orbit* orbit;
	Planet(Vector2 position, float radius, float t, float m, std::string name) : orbit(nullptr) {
		this->position = position;
		this->temperature = t;
		this->radius = radius;
		this->mass = m;
		this->name = name;
		if (temperature >= 2000) {
			this->color = RED;
		}
		else if (temperature >= 3500) {
			this->color = ORANGE;
		}
		else if (temperature >= 5000) {
			this->color = YELLOW;
		}
		else if (temperature >= 7500) {
			this->color = WHITE;
		}
		else if (temperature >= 10000) {
			this->color = { 0, 220, 255 };
		}
		else if (temperature >= 30000) {
			this->color = BLUE;
		}
	}
	~Planet() {
		delete orbit;
	}

	void ChangeColor() {
		if (temperature >= 2000) {
			this->color = RED;
		}
		else if (temperature >= 3500) {
			this->color = ORANGE;
		}
		else if (temperature >= 5000) {
			this->color = YELLOW;
		}
		else if (temperature >= 7500) {
			this->color = WHITE;
		}
		else if (temperature >= 10000) {
			this->color = { 0, 220, 255 };
		}
		else if (temperature >= 30000) {
			this->color = BLUE;
		}
	}
	Vector2 GetPosition() {
		return this->position;
	}
	Color GetColor() {
		return this->color;
	}
	float GetTemperature() {
		return this->temperature;
	}
	float GetMass() {
		return this->mass;
	}
	void SetTemperature(float t) {
		this->temperature = t;
		ChangeColor();
	}
	void SetPos(Vector2 pos) {
		this->position = pos;
	}
	void Move(float dx, float dy) {
		this->position.x += dx;
		this->position.y += dy;
	}
	float GetRadius() {
		return this->radius;
	}



};

class Orbit {

private:
	float radius;
	Vector2 center;
	Vector2 position;
	float mass;
	float distance;
	Color color;
	Planet* planet;
	float speed;
	float angle = 0;
public:
	
	Orbit(float radius, float mass, float distance, Color color, Planet* pointer) {
		if (pointer == nullptr) {
			std::cout << "Fatal error: pointer if nullptr";
			getchar();
			abort();
		}
		this->planet = pointer;
		this->radius = radius;
		this->center = pointer->GetPosition();
		this->mass = mass;
		this->distance = distance;
		this->color = color;
		this->position.x = this->center.x;
		this->position.y = this->center.y + this->planet->GetRadius() + this->radius + this->distance;
	}
	~Orbit() {

	}
	Vector2 GetPosition() {
		return this->position;
	}
	float GetRadius() {
		return this->radius;
	}
	float GetDistance() {
		return this->distance;
	}
	Color GetColor() {
		return this->color;
	}
	void Gravity_init() {
		float D = sqrt((planet->GetPosition().x - this->position.x) * (planet->GetPosition().x - this->position.x) + (planet->GetPosition().y - this->position.y) * (planet->GetPosition().y - this->position.y));
		float F = gravity * this->mass * this->planet->GetMass() / (D * D);
		float Radius = this->distance + this->radius + planet->GetRadius();
		angle += F * GetFrameTime();
		this->position.x = planet->GetPosition().x + cosf(angle) * Radius;
		this->position.y = planet->GetPosition().y + sinf(angle) * Radius;
	}


};



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