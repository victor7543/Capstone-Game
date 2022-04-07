#include <iostream>
#include <math.h>
#include "game.h"
#include "SDL.h"

using std::make_unique;
using std::size_t;

Game::Game(size_t grid_width, size_t grid_height)
	: _grid_height(grid_height),
	_grid_width(grid_width),
	controlled_piece(make_unique<Piece>(grid_width, grid_height))
{
	SDL_Init(SDL_INIT_AUDIO);
}

void Game::Run(Controller const& controller, Renderer& renderer,
	size_t target_frame_duration) {
	Uint32 title_timestamp = SDL_GetTicks();
	Uint32 frame_start;
	Uint32 frame_end;
	Uint32 frame_duration;
	int frame_count = 0;
	bool running = true;

	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8* wavBuffer;

	SDL_LoadWAV("Tetris_theme.wav", &wavSpec, &wavBuffer, &wavLength);
	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	SDL_PauseAudioDevice(deviceId, 0);

	if (TTF_Init() < 0) {
		std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
	}
	while (running) {
		if (SDL_GetQueuedAudioSize(deviceId) == 0) {
			success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
		}
		frame_start = SDL_GetTicks();

		// Input, Update, Render - the main game loop.
		controller.HandleInput(running, *controlled_piece);
		Update();
		renderer.Render(filled_cells, *controlled_piece, score, is_game_over);

		frame_end = SDL_GetTicks();

		// Keep track of how long each loop through the input/update/render cycle
		// takes.
		frame_count++;
		frame_duration = frame_end - frame_start;

		// After every second, update the window title.
		if (frame_end - title_timestamp >= 1000) {
			renderer.UpdateWindowTitle(score, frame_count);
			frame_count = 0;
			title_timestamp = frame_end;
		}

		// If the time for this frame is too small (i.e. frame_duration is
		// smaller than the target ms_per_frame), delay the loop to
		// achieve the correct frame rate.
		if (frame_duration < target_frame_duration) {
			SDL_Delay(target_frame_duration - frame_duration);
		}
	}
	SDL_CloseAudioDevice(deviceId);
	SDL_FreeWAV(wavBuffer);
	TTF_Quit();
}

void Game::Update() {
	piece_cells = vector<SDL_Point>();
	for (auto& pos : controlled_piece->piece_pos) {
		piece_cells.emplace_back(std::move(SDL_Point(
			static_cast<int>(pos.first),
			static_cast<int>(pos.second)
		)));
	}
	if (!controlled_piece->is_falling) {
		for (auto cell : piece_cells) {
			if (cell.y <= 0) {
				is_game_over = true;
				break;
			}
		}
		if (!is_game_over) {
			filled_cells.insert(std::end(filled_cells), std::begin(piece_cells), std::end(piece_cells));
			VerifyCompletedRows();
			controlled_piece = make_unique<Piece>(_grid_width, _grid_height);
		}
	}
	else if (!is_game_over) {
		prev_pos = controlled_piece->piece_pos;
		prev_piece_cells = piece_cells;
		controlled_piece->MoveVertical();
		DetectCollision(false);
		controlled_piece->MoveHorizontal();
		DetectCollision(false);
		bool just_rotated = controlled_piece->TryRotate(controlled_piece->piece_pos);
		if (just_rotated) {
			DetectCollision(just_rotated);
		}
	}
}

void Game::DetectCollision(bool just_rotated)
{
	auto& piece = controlled_piece->piece_pos;
	for (int i = 0; i < piece.size(); i++) {
		controlled_piece->prev_block_pos = piece[i];
		SDL_Point current_cell{
			static_cast<int>(piece[i].first),
			static_cast<int>(piece[i].second)
		};
		for (auto const& filled_cell : filled_cells) {
			if (current_cell.x != prev_piece_cells[i].x || current_cell.y != prev_piece_cells[i].y) { // The piece has changed cells since last update
				if (current_cell.x == filled_cell.x && current_cell.y == filled_cell.y) { // The piece is intersecting with a filled cell.
					if (just_rotated) {
						controlled_piece->Rotate(controlled_piece->base_pos, Piece::RotationDirection::Counter_Clockwise); // The piece is just going back to it's previous rotation, which is a valid position.
						return;
					}
					if (current_cell.x == prev_piece_cells[i].x) {
						piece[0] = prev_pos[0];
						for (int j = 1; j < piece.size(); j++) {
							piece[j].first = piece[0].first + controlled_piece->base_pos[j].first;
							piece[j].second = piece[0].second + controlled_piece->base_pos[j].second;
						}
						controlled_piece->is_falling = false;
						return;
					}
					piece[0].first = prev_pos[0].first;
					for (int j = 1; j < piece.size(); j++) {
						piece[j].first = piece[0].first + controlled_piece->base_pos[j].first;
					}
					return;
				}
			}
		}
	}
}

void Game::VerifyCompletedRows()
{
	vector<SDL_Point> temp_filled_cells = filled_cells;
	int deleted_cells = 0;
	int sequence = 0;
	for (int i = 20; i >= 0; i--) {
		for (int j = 1; j <= temp_filled_cells.size(); j++) {
			if (temp_filled_cells[j-1].y == i) {
				temp_filled_cells.erase(temp_filled_cells.begin() + j-1);
				deleted_cells++;
				j--;
			}
			else if (temp_filled_cells[j - 1].y < i) {
				temp_filled_cells[j - 1].y++;
			}
		}
		if (deleted_cells >= 10) {
			filled_cells = temp_filled_cells;
			score += 100 * pow(2, sequence);
			sequence += 1;
			i++;
		}
		else { 
			temp_filled_cells = filled_cells;
		}
		deleted_cells = 0;
	}
	sequence = 0;
}