#ifndef CONTROLLER_H
#define CONTROLLER_H

class Piece;

class Controller {
 public:
	bool HandleInput(bool &running, Piece &piece, bool &game_over) const;
private:
	mutable bool rotate_key_already_pressed = false;
	mutable bool horiz_key_already_pressed = false;
	mutable bool acceleration_key_already_pressed = false;
};

#endif