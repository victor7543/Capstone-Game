#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "piece.h"

class Controller {
 public:
  void HandleInput(bool &running, Piece &piece) const;

 private:
  void ChangeDirection(Piece &piece, Piece::Direction input) const;
};

#endif