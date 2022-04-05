#include "piece.h"
#include <cmath>
#include <iostream>

Piece::Piece(int grid_width, int grid_height)
	: grid_width(grid_width),
	grid_height(grid_height),
	horiz_mov_key_timer(SDL_GetTicks()),
	vert_mov_key_timer(SDL_GetTicks())
{
	CreatePiece(grid_width);
}

void Piece::CreatePiece(int grid_width)
{
	std::ifstream stream(pieces_file);
	stream >> json_content;
	Uint32 index = RandomizePiece(json_content.size());
	auto& object_it = json_content[index];
	for (auto& line : object_it.items()) {
		if (line.key() == "cells") {
			for (auto& e : line.value()) {
				pair<short, short> block_pos = e;
				base_pos.push_back(block_pos);
				block_pos.first += grid_width / 2;
				piece_pos.emplace_back(std::move(block_pos));
				std::cout << line.key() << "\t" << line.value() << std::endl;
			}
		}
		else if (line.key() == "can_rotate") {
			can_rotate = line.value();
		}
	}
}

Uint32 Piece::RandomizePiece(int pieces_count)
{
	std::random_device rnd;
	std::mt19937 gnrt(rnd());
	std::uniform_int_distribution<uint32_t> distrib(0, pieces_count - 1);
	return distrib(gnrt);
}

void Piece::MoveVertical() {
	auto prev_pos = piece_pos;
	auto now = SDL_GetTicks();
	auto time_diff = (now - vert_mov_key_timer);
	if (prev_dir != direction || time_diff >= vert_mov_time_threshold) {
		float temp_speed_x = speed_x;
		if (time_diff < vert_mov_time_threshold) {
			vert_mov_key_timer = SDL_GetTicks();
		}
		switch (direction) {
		case Direction::kDown:
			piece_pos[0].second += speed_y * 10.0f;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].second = piece_pos[0].second + base_pos[i].second;
			}
			prev_dir = direction;
			direction = Direction::kNull;
			break;
		default:
			vert_mov_key_timer = SDL_GetTicks();
		}
	}
	piece_pos[0].second += speed_y;
	for (int i = 1; i < piece_pos.size(); i++) {
		piece_pos[i].second = piece_pos[0].second + base_pos[i].second;
	}
	PreventOffScreenMovement(prev_pos, base_pos);
}

void Piece::MoveHorizontal() {
	auto prev_pos = piece_pos;
	auto now = SDL_GetTicks();
	auto time_diff = (now - horiz_mov_key_timer);
	if (prev_dir != direction || time_diff >= horiz_mov_time_threshold) {
		float temp_speed_x = speed_x;
		if (time_diff < horiz_mov_time_threshold) {
			horiz_mov_key_timer = SDL_GetTicks();
		}
		else {
			temp_speed_x = speed_x / 2.5;
		}
		switch (direction) {
		case Direction::kLeft:
			piece_pos[0].first -= temp_speed_x;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].first = piece_pos[0].first + base_pos[i].first;
			}
			break;
		case Direction::kRight:
			piece_pos[0].first += temp_speed_x;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].first = piece_pos[0].first + base_pos[i].first;
			}
			break;
		default:
			horiz_mov_key_timer = SDL_GetTicks();
		}
	}
	if (direction != Direction::kUp && direction != Direction::kDown) {
		prev_dir = direction;
		direction = Direction::kNull;
	}
	PreventOffScreenMovement(prev_pos, base_pos);
}

bool Piece::TryRotate(vector<pair<float, float>>& original_pos)
{
	if (can_rotate && direction == Direction::kUp && prev_dir != Direction::kUp) {
		prev_dir = Direction::kUp;
		vector<pair<float, float>> temp_base_pos = base_pos;
		Rotate(temp_base_pos, RotationDirection::Clockwise);
		if (PreventOffScreenMovement(original_pos, temp_base_pos)) {
			base_pos = temp_base_pos;
		}
		else {
			return true;
		}
	}
	return false;
}

void Piece::Rotate(vector<pair<float, float>> p_base_pos, RotationDirection rot_dir)
{
	for (int i = 1; i < base_pos.size(); i++) {
		if (rot_dir == RotationDirection::Clockwise) {
			base_pos[i].first = p_base_pos[i].second * -1;
			base_pos[i].second = p_base_pos[i].first;
		}
		else {
			base_pos[i].first = p_base_pos[i].second;
			base_pos[i].second = p_base_pos[i].first * -1;
		}
		piece_pos[i].first = piece_pos[0].first + base_pos[i].first;
		piece_pos[i].second = piece_pos[0].second + base_pos[i].second;
	}
}

bool Piece::PreventOffScreenMovement(vector<pair<float, float>>& original_pos, vector<pair<float, float>>& base_pos)
{
	for (int i = 0; i < piece_pos.size(); i++) {
		if (piece_pos[i].second > grid_height) {
			piece_pos[0].first = original_pos[0].first;
			piece_pos[0].second = original_pos[0].second;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].first = piece_pos[0].first + base_pos[i].first;
				piece_pos[i].second = piece_pos[0].second + base_pos[i].second;
			}
			this->is_falling = false;
			return true;
		}
		else if (piece_pos[i].first > grid_width || piece_pos[i].first < 0.0) {
			piece_pos[0].first = original_pos[0].first;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].first = piece_pos[0].first + base_pos[i].first;
			}
			return true;
		}
	}
	return false;
}
