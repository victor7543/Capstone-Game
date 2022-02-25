#include "piece.h"
#include <cmath>
#include <iostream>

Piece::Piece(int grid_width, int grid_height)
	: grid_width(grid_width),
	grid_height(grid_height),
	key_pressed_timer(SDL_GetTicks())
{
	CreatePiece(grid_width);
}

void Piece::CreatePiece(int grid_width)
{
	std::ifstream stream(pieces_file);
	stream >> json_content;
	Uint32 index = RandomizePiece(json_content.size());
	auto &object_it = json_content[index];
	for (auto& line : object_it.items()) {
		std::string key = "block";
		if (key == line.key()) {
			for (auto &e : line.value()) {
				pair<short, short> block_pos = e;
				initial_pos.push_back(block_pos);
				block_pos.first += grid_width / 2;
				piece_pos.emplace_back(std::move(block_pos));
				std::cout << line.key() << "\t" << line.value() << std::endl;
			}
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


void Piece::Move() {
	now = SDL_GetTicks();
	time_diff = (now - key_pressed_timer);
	auto prev_piece = piece_pos;
	if (prev_dir != direction || time_diff >= time_threshold) {
		float temp_speed_x = speed_x;
		if (time_diff < time_threshold) {
			key_pressed_timer = SDL_GetTicks();
		}
		else {
			temp_speed_x = speed_x / 5.0;
		}
		switch (direction) {
		case Direction::kUp:
			RotatePiece();
			prev_dir = Direction::kUp;
			break;
		case Direction::kDown:
			piece_pos[0].second += speed_y * 10.0f;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].second = piece_pos[0].second + initial_pos[i].second;
			}
			prev_dir = Direction::kDown;
			break;
		case Direction::kLeft:
			piece_pos[0].first -= temp_speed_x;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].first = piece_pos[0].first + initial_pos[i].first;
			}
			prev_dir = Direction::kLeft;
			break;
		case Direction::kRight:
			piece_pos[0].first += temp_speed_x;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].first = piece_pos[0].first + initial_pos[i].first;
			}
			prev_dir = Direction::kRight;
			break;
		default:
			key_pressed_timer = SDL_GetTicks();
			prev_dir = Direction::kNeutral;
		}
	}
	direction = Direction::kNeutral;
	piece_pos[0].second += speed_y;
	for (int i = 1; i < piece_pos.size(); i++) {
		piece_pos[i].second = piece_pos[0].second + initial_pos[i].second;
	}
	PreventOffScreenMovement(prev_piece);
}

void Piece::RotatePiece()
{
	vector<pair<float, float>> temp_initial_pos = initial_pos;
	for (int i = 1; i < initial_pos.size(); i++) {
		initial_pos[i].first = temp_initial_pos[i].second * -1;
		initial_pos[i].second = temp_initial_pos[i].first;
	}
	for (int i = 1; i < piece_pos.size(); i++) {
		piece_pos[i].first = piece_pos[0].first + initial_pos[i].first;
		piece_pos[i].second = piece_pos[0].second + initial_pos[i].second;
	}
}

void Piece::PreventOffScreenMovement(vector<pair<float, float>>& prev_piece)
{
	bool revert_position = false;
	bool revert_position_x = false;
	for (int i = 0; i < piece_pos.size(); i++) {
		if (piece_pos[i].first > grid_width) {
			revert_position_x = true;
		}
		if (piece_pos[i].first < 0.0) {
			revert_position_x = true;
		}
		if (piece_pos[i].second > grid_height) {
			revert_position = true;
			this->is_falling = false;
		}
		if (revert_position) {
			piece_pos[0].first = prev_piece[0].first;
			piece_pos[0].second = prev_piece[0].second;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].first = piece_pos[0].first + initial_pos[i].first;
				piece_pos[i].second = piece_pos[0].second + initial_pos[i].second;
			}
			break;
		}
		else if (revert_position_x) {
			piece_pos[0].first = prev_piece[0].first;
			for (int i = 1; i < piece_pos.size(); i++) {
				piece_pos[i].first = piece_pos[0].first + initial_pos[i].first;
			}
			break;
		}
	}
}
