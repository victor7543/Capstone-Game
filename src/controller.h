#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "piece.h"

class Controller {
 public:
	bool HandleInput(bool &running, Piece &piece, bool &is_game_over) const;
private:
	mutable bool rotate_key_already_pressed = false;
	mutable bool horiz_key_already_pressed = false;
	mutable bool acceleration_key_already_pressed = false;
};

#endif