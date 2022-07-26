#include <iostream>
#include "game.h"
#include "controller.h"
#include "renderer.h"
#include "piece.h"
#include <SDL_ttf.h>

using std::make_unique;
using std::size_t;

Game::Game(size_t grid_width, size_t grid_height)
	: _grid_height(grid_height),
	_grid_width(grid_width),
	controlled_piece(make_unique<Piece>(grid_width, grid_height))
{
	SDL_Init(SDL_INIT_AUDIO);
}

void Game::Run(Controller const&controller, Renderer& renderer,
	size_t target_frame_duration) {
	Uint32 title_timestamp = SDL_GetTicks();
	Uint32 frame_start;
	Uint32 frame_end;
	Uint32 frame_duration;
	int frame_count = 0;
	bool running = true;
	SDL_AudioSpec wavSpec;{}


	if (TTF_Init() < 0) {
		std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
	}
	int audio_queue_success = InitAudio(wavSpec);
	while (running) {
		if (SDL_GetQueuedAudioSize(deviceId) == 0 && audio_queue_success == 0) {  // this is necessary to keep the audio playing.
			audio_queue_success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
		}
		frame_start = SDL_GetTicks();

		// Input, Update, Render - the main game loop.
		if (controller.HandleInput(running, *controlled_piece, game_over)) {
			ResetGame();
		}
		Update();
		renderer.Render(filled_cells, *controlled_piece, score, game_over);

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

bool Game::InitAudio(SDL_AudioSpec& wavSpec)
{
	SDL_LoadWAV("Tetris_theme.wav", &wavSpec, &wavBuffer, &wavLength);
	deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	if (success == 0) {
		SDL_PauseAudioDevice(deviceId, 0);
	}
	return success;
}

void Game::Update() {
	if (!game_over) {
		piece_cells = vector<SDL_Point>();
		for (auto& pos : controlled_piece->current_pos) {
			piece_cells.emplace_back(std::move(SDL_Point(
				static_cast<int>(pos.first),
				static_cast<int>(pos.second)
			)));
		}
		if (!controlled_piece->is_falling) {
			filled_cells.insert(std::end(filled_cells), std::begin(piece_cells), std::end(piece_cells)); // inserts the cells of the controlled piece into 
			CheckCompletedRows();                                                                        // filled cells vector.
			if (CheckGameOver()) {
				return;
			}
			controlled_piece = make_unique<Piece>(_grid_width, _grid_height);
		}
		prev_pos = controlled_piece->current_pos;
		prev_piece_cells = piece_cells;
		controlled_piece->MoveVertical();
		ValidateVerticalMovement();
		controlled_piece->MoveHorizontal();
		ValidateHorizontalMovement();
		controlled_piece->TryRotate();
		if (controlled_piece->is_rotated) {
			ValidateRotation();
		}
	}
}

void Game::ValidateVerticalMovement()
{
	if (DetectCollisions()) {
		controlled_piece->current_pos[0] = prev_pos[0];
		for (int j = 1; j < controlled_piece->current_pos.size(); j++) {
			controlled_piece->current_pos[j].first = controlled_piece->current_pos[0].first + controlled_piece->basic_shape[j].first;
			controlled_piece->current_pos[j].second = controlled_piece->current_pos[0].second + controlled_piece->basic_shape[j].second;
		}
		controlled_piece->is_falling = false;
	}
}

void Game::ValidateHorizontalMovement()
{
	if (DetectCollisions()) {
		controlled_piece->current_pos[0].first = prev_pos[0].first;
		for (int j = 1; j < controlled_piece->current_pos.size(); j++) {
			controlled_piece->current_pos[j].first = controlled_piece->current_pos[0].first + controlled_piece->basic_shape[j].first;
		}
	}
}

void Game::ValidateRotation()
{
	if (DetectCollisions()) {
		controlled_piece->Rotate(Piece::RotationDirection::Counter_Clockwise); // The piece is just going back to it's previous rotation, which is a valid position.
	}
}

bool Game::DetectCollisions()
{
	for (int i = 0; i < controlled_piece->current_pos.size(); i++) {
		controlled_piece->prev_block_pos = controlled_piece->current_pos[i];
		current_cell = {
			static_cast<int>(controlled_piece->current_pos[i].first),
			static_cast<int>(controlled_piece->current_pos[i].second)
		};
		for (auto const& filled_cell : filled_cells) {
			if (current_cell.x != prev_piece_cells[i].x || current_cell.y != prev_piece_cells[i].y) { // The piece has changed cells since last update
				if (current_cell.x == filled_cell.x && current_cell.y == filled_cell.y) { // The piece is intersecting with a filled cell.
					return true;
				}
			}
		}
	}
	return false;
}

void Game::CheckCompletedRows() // makes the completed row disappear and the rows above fall down to fill the empty space.
{
	vector<SDL_Point> filled_cells_temp = filled_cells;
	int deleted_cells = 0;
	int sequence = 0;
	for (int i = 20; i >= 0; i--) { // for each row
		for (int j = 1; j <= filled_cells_temp.size(); j++) {
			if (filled_cells_temp[j-1].y == i) { // every cell present in the current row will be deleted from the temporary vector,
				filled_cells_temp.erase(filled_cells_temp.begin() + j-1); // but that vector will only become the main one if a full row was deleted
				deleted_cells++;
				j--;
			}
			else if (filled_cells_temp[j-1].y < i) {
				filled_cells_temp[j-1].y++;
			}
		}
		if (deleted_cells >= 10) {
			filled_cells = filled_cells_temp;
			score += 100 * pow(2, sequence); // the score is increased if you manage to complete a row, 
			sequence += 1;					 // but the increase is bigger if you manage to complete multiple rows at once.
			i++;
		}
		else { 
			filled_cells_temp = filled_cells; //if there was no complete row the temp vector is discarded.
		}
		deleted_cells = 0;
	}
	sequence = 0;
}

bool Game::CheckGameOver()
{
	for (SDL_Point& cell : piece_cells) {
		if (cell.y <= 0) {
			game_over = true;
			return true;
		}
	}
	return false;
}

void Game::ResetGame()
{
	filled_cells = vector<SDL_Point>();
	score = 0;
	game_over = false;
	controlled_piece = make_unique<Piece>(_grid_width, _grid_height);
}