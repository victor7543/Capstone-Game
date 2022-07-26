#include "piece.h"
#include <iostream>
#include <fstream>
#include <random>

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
	stream >> json;
	Uint32 index = RandomizePiece(json.size());
	auto& piece_data = json[index];
	short init_offset = 0;
	for (auto& line : piece_data.items()) {
		if (line.key() == "init_offset") {
			init_offset = line.value();
		}
		else if (line.key() == "positions") {
			short start_y_pos = 0;
			for (auto& json_cell_coords : line.value()) {
				pair<short, short> cell_coords = json_cell_coords;
				basic_shape.push_back(cell_coords);
				cell_coords.first += grid_width / 2;
				cell_coords.second += init_offset;
				current_pos.emplace_back(std::move(cell_coords));
			}
		}
		else if (line.key() == "can_rotate") {
			can_rotate = line.value();
		}
		else if (line.key() == "color") {
			color = line.value();
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
	auto prev_pos = current_pos;
	auto now = SDL_GetTicks();
	auto time_diff = (now - vert_mov_key_timer);
	if (acceleration_key_held == false || time_diff >= vert_mov_time_threshold) {
		float temp_speed_x = speed_x;
		if (time_diff < vert_mov_time_threshold) {
			vert_mov_key_timer = SDL_GetTicks();
		}
		switch (vert_direction) {
		case Direction::kDown:
			current_pos[0].second += speed_y * 10.0f;
			for (int i = 1; i < current_pos.size(); i++) {
				current_pos[i].second = current_pos[0].second + basic_shape[i].second;
			}
			vert_direction = Direction::kNull;
			break;
		default:
			vert_mov_key_timer = SDL_GetTicks();
		}
	}
	current_pos[0].second += speed_y;
	for (int i = 1; i < current_pos.size(); i++) {
		current_pos[i].second = current_pos[0].second + basic_shape[i].second;
	}
	WindowBorderCollision(prev_pos);
}

void Piece::MoveHorizontal() {
	auto prev_pos = current_pos;
	auto now = SDL_GetTicks();
	auto time_diff = (now - horiz_mov_key_timer);
	if (horiz_key_held == false || time_diff >= horiz_mov_time_threshold) {
		float temp_speed_x = speed_x;
		if (time_diff < horiz_mov_time_threshold) {
			horiz_mov_key_timer = SDL_GetTicks();
		}
		else {
			temp_speed_x = speed_x / 2.5;
		}
		switch (horiz_direction) {
		case Direction::kLeft:
			current_pos[0].first -= temp_speed_x;
			for (int i = 1; i < current_pos.size(); i++) {
				current_pos[i].first = current_pos[0].first + basic_shape[i].first;
			}
			break;
		case Direction::kRight:
			current_pos[0].first += temp_speed_x;
			for (int i = 1; i < current_pos.size(); i++) {
				current_pos[i].first = current_pos[0].first + basic_shape[i].first;
			}
			break;
		default:
			horiz_mov_key_timer = SDL_GetTicks();
		}
	}
	horiz_direction = Direction::kNull;
	WindowBorderCollision(prev_pos);
}

void Piece::TryRotate()
{
	if (can_rotate && rotate_key_held && is_rotated == false) {
		vector<pair<float, float>> unrotated_shape = basic_shape;
		Rotate(RotationDirection::Clockwise);
		rotate_key_held = false;
		if (WindowBorderCollision()) {
			basic_shape = unrotated_shape;
		}
		is_rotated = true;
	}
}

void Piece::Rotate(RotationDirection const &rot_dir)
{
	auto unrotated_shape = basic_shape;
	for (int i = 1; i < basic_shape.size(); i++) {
		if (rot_dir == RotationDirection::Clockwise) {
			basic_shape[i].first = unrotated_shape[i].second * -1;
			basic_shape[i].second = unrotated_shape[i].first;
		}
		else {
			basic_shape[i].first = unrotated_shape[i].second;
			basic_shape[i].second = unrotated_shape[i].first * -1;
		}
		current_pos[i].first = current_pos[0].first + basic_shape[i].first;
		current_pos[i].second = current_pos[0].second + basic_shape[i].second;
	}
}

bool Piece::WindowBorderCollision(vector<pair<float, float>> const& prev_pos)
{
	for (int i = 0; i < current_pos.size(); i++) {
		if (current_pos[i].second > grid_height) {
			current_pos[0].first = prev_pos[0].first;
			current_pos[0].second = prev_pos[0].second;
			for (int i = 1; i < current_pos.size(); i++) {
				current_pos[i].first = current_pos[0].first + basic_shape[i].first;
				current_pos[i].second = current_pos[0].second + basic_shape[i].second;
			}
			this->is_falling = false;
			return true;
		}
		else if (current_pos[i].first >= grid_width || current_pos[i].first < 0.0) {
			current_pos[0].first = prev_pos[0].first;
			for (int i = 1; i < current_pos.size(); i++) {
				current_pos[i].first = current_pos[0].first + basic_shape[i].first;
			}
			return true;
		}
	}
	return false;
}

bool Piece::WindowBorderCollision()
{
	for (int i = 0; i < current_pos.size(); i++) {
		if (current_pos[i].second > grid_height || current_pos[i].second < 0 || current_pos[i].first >= grid_width || current_pos[i].first < 0.0) {
			Rotate(RotationDirection::Counter_Clockwise);
			return true;
		}
	}
	return false;
}
