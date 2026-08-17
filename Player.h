#pragma once

#include <raylib.h>

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