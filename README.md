# Tetris game for the Capstone project

Made in C++ using the SDL2 library.

## How it works
Just like classic Tetris, there are 7 types of pieces that keep descending from the top of the screen in random order.
The player controls each piece as they fall, trying to complete lines in order to score points and prevent the pieces from
reaching the top of the screen. Each line completed will earn the player 100 points and make that line disappear, and if the player manages to complete multiple lines at once there is a score multiplier. The pieces can collide with each other and will pile up until they reach the top of the screen. When that happens the game is over.

## Code structure
The main function will create a Renderer, a Controller and a Game object, and then call Run() from the Game object, passing the other objects as arguments. The Run function will initialize the audio and start the game loop, which will keep calling the HandleInput, Update and Render functions until the application finishes.

The Game class contains an instance of Piece, which is the representation of the piece the player is currently controlling, and the Piece class has functions that allow it to move. Those functions will be called from Update() in the Game class, but the piece can only be moved if one of the movement buttons is pressed. This is controlled by HandleInput in the Controller class by setting a few flags in the Piece reference it received as parameter. After the piece is moved or rotated in any direction, the Update() function will detect collisions and revert the piece to it's previous position if necessary, but the functions responsible for detecting collisions with the borders of the viewport are present in the Piece class, which is also responsible for calling them after the piece moves or rotates. After a piece reaches the bottom of the viewport or collides vertically with another piece, it will be destroyed and it's position will be added to a vector that holds the positions from all previous pieces. That vector will then be used by CheckCompletedRows() called from Update() to check for completed rows, which will eliminate their positions from the vector and increment the player's score. After a piece is destroyed, another one will be created in Update() using the Piece constructor. This constructor reads from a json file containing all the information about each piece type (shape, color, can rotate, starting position offset) using the nlohmann json library https://github.com/nlohmann/json, and then selects a random piece using a random number generator.
If one of the previous pieces is above the upper limit of the viewport than the game is over and no code from Update() will execute until the player presses another key, which will allow the game loop to call ResetGame() and start over the game.
The Render class consist mostly of functions responsible for rendering the piece, the previous pieces that can no longer be moved, and text to inform the score and when the game is over. It will draw the controlled piece with the color specified in the json file and the previous pieces with a white color, and all pieces have a black border. The rendering itself is done using SDL functions.

## File structure
All classes have their own file and have their declarations in a header file and the definitions in a cpp file.
main.cpp has the main function and a few constants that determine the size os the viewport and the grid.
game.h and game.cpp have have the Game class.
piece.h and piece.cpp have the Piece class.
controller.h and controller.cpp have the Controller class.
renderer.h and renderer.cpp have the Renderer class.
pieces.json has the specifications for each Tetris piece.
Tetris_theme.wav is the audio file.
## How the Project satisfies the rubric
### Compiling and Testing
The project compiles and runs without errors.
### The project demonstrates an understanding of C++ functions and control structures 
The code is organized into functions and makes use of while loops, for loops, if conditions and switch statements
(game.cpp line 162, game.cpp line 87, game.cpp line 34, piece.cpp line 62, piece.cpp 93, controller.cpp line 8, piece.cpp line 22).
### The project reads data from a file and process the data, or the program writes data to a file.
The project read from a json file to get the pieces specifications (piece.cpp line 17).
### The project accepts user input and processes the input.
The project uses user input to control the piece (controller.cpp line 8).
### The project uses Object Oriented Programming techniques.
The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks (all header files).
### Classes use appropriate access specifiers for class members.
All class data members are explicitly specified as public, protected, or private (all header files).
### Class constructors utilize member initialization lists.
All class members that are set to argument values are initialized through member initialization lists (piece.cpp line 6, game.cpp line 11, renderer.cpp line 6).
### Overloaded functions allow the same function to operate on different parameters.
The WindowBorderCollision() function was overloaded with different signatures for the same function name (piece.h line 41).

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Capstone-Game`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
