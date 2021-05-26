#pragma once
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>

struct Pendulum
{
	float cordLength = 45.0f;
	float angle = 100.0f * DEG2RAD;
	Vector2 position = { cordLength * sin(angle), cordLength * cos(angle) };
	float velocity = 0.0f;
	float acceleration = 0.0f;
	float mass = 0.01f;
	float radius = 3.0f;
	Color color = RED;
};