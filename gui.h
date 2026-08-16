#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <variant>

extern enum class flag { ALL, TEXT, OBJECT, NONE };
extern enum class flag2 {BUTTON, STATIC};



class Window {
private:
	flag Type;
	flag2 Class;
	Vector2 position;
	Vector2 position_center;
	float Width, Height;
	Color color;
	Color color_text;
	std::string& text;
	int font_size;
	Vector2 position_text;
public:
	Window(Vector2 pos, float w, float h, Color c, Color ct, std::string& t, Vector2 pos_t, int font_size, flag type) : position(pos), Width(w), Height(h), color(c), text(t), color_text(ct), font_size(font_size), position_text(pos_t) {
		this->position_center.x = this->position.x + this->Width / 2;
		this->position_center.y = this->position.y + this->Height / 2;
		this->Type = type;
	}
	~Window() {
		this->Type == flag::NONE;
	}
	void Render() {
		
		if (this->Type == flag::ALL) {
			DrawRectangle(this->position.x, this->position.y, this->Width, this->Height, this->color);
			DrawText(this->text.c_str(), this->position.x + this->position_text.x, this->position.y + this->position_text.y, this->font_size, this->color_text);
		}
		else if (this->Type == flag::TEXT) {
			DrawText(this->text.c_str(), this->position.x + this->position_text.x, this->position.y + this->position_text.y, this->font_size, this->color_text);
		}
		else if (this->Type == flag::OBJECT) {
			DrawRectangle(this->position.x, this->position.y, this->Width, this->Height, this->color);
		}
		else if (this->Type == flag::NONE) {
			return;
		}
	}
};

