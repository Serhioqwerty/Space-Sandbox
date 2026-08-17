#pragma once
#include <raylib.h>


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
