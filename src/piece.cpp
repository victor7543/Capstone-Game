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
    for (auto& object_it : json_content.items()) {
        for (auto& line : object_it.value().items()) {
            vector<std::string> keys{ "block1", "block2", "block3", "block4" };
            for (auto& key : keys) {
                if (key == line.key()) {
                    pair<short, short> block_pos = line.value();
                    init_value.push_back(block_pos);
                    block_pos.first += grid_width / 2;
                    piece_pos.emplace_back(std::move(block_pos));
                    std::cout << line.key() << "\t" << line.value() << std::endl;
                }
            }
        }
        if (!piece_pos.empty())
            break;
    }
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
                prev_dir = Direction::kUp;
                break;
            case Direction::kDown:
                piece_pos[0].second += speed_y * 10.0f;
                for (int i = 1; i < piece_pos.size(); i++) {
                    piece_pos[i].second = piece_pos[0].second + init_value[i].second;
                }
                prev_dir = Direction::kDown;
                break;
            case Direction::kLeft:
                piece_pos[0].first -= temp_speed_x;
                for (int i = 1; i < piece_pos.size(); i++) {
                    piece_pos[i].first = piece_pos[0].first + init_value[i].first;
                }
                prev_dir = Direction::kLeft;
                break;
            case Direction::kRight:
                piece_pos[0].first += temp_speed_x;
                for (int i = 1; i < piece_pos.size(); i++) {
                    piece_pos[i].first = piece_pos[0].first + init_value[i].first;
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
        piece_pos[i].second = piece_pos[0].second + init_value[i].second;
    }
    PreventOffScreenMovement(prev_piece);
}

void Piece::PreventOffScreenMovement(vector<pair<float, float>> &prev_piece)
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
                piece_pos[i].first = piece_pos[0].first + init_value[i].first;
                piece_pos[i].second = piece_pos[0].second + init_value[i].second;
            }
            break;
        }
        else if (revert_position_x) {
            piece_pos[0].first = prev_piece[0].first;
            for (int i = 1; i < piece_pos.size(); i++) {
                piece_pos[i].first = piece_pos[0].first + init_value[i].first;
            }
            break;
        }
    }
}
