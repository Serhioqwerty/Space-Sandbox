#pragma once
#include <raylib.h>

extern const double gravity;

class Orbit;

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