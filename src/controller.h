#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "piece.h"

class Controller {
 public:
  bool HandleInput(bool &running, Piece &piece, bool &is_game_over) const;

 private:
  void ChangeDirection(Piece &piece, Piece::Direction input) const;
};

#endif