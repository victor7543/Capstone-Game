#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "shape.h"

class Controller {
 public:
  void HandleInput(bool &running, Shape &shape) const;

 private:
  void ChangeDirection(Shape &shape, Shape::Direction input) const;
};

#endif