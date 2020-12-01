Multithreaded Pacman Game
=========================

Implement a multithreaded version of the arcade video game [Pacman](https://en.wikipedia.org/wiki/Pac-Man). This version will be a
Computer vs Human game. Each enemy will be independent and the number of enemies is configurable. Below you can see the general
requirements for the enemies and human player (pacman) interation.

![Pacman](pacman.png)

Technical Requirements
----------------------
- The game's maze layout can be static.
- The `pacman` gamer must be controlled by the user.
- Enemies are autonomous entities that will move a random way.
- Enemies and pacman should respect the layout limits and walls.
- Enemies number can be configured on game's start.
- Each enemy's behaviour will be implemented as a separated thread.
- Enemies and pacman threads must use the same map or game layout data structure resource.
- Display obtained pacman's scores.
- Pacman loses when an enemy touches it.
- Pacman wins the game when it has taken all coins in the map.


Architecture
-------------

Multithreaded Pacman created with graphical interface and implemented in C, using the libraries SDL (SDL2 image and SDL2 ttf) and OpenMP for GUI and user input and use of threads, respectively. 

Usage
-------------
For the usage of the Multithreaded Pacman Videogame is the classic W,A,S,D keys from the keyboard that will control Up, Left, Down and Right, respectevely.

<img src="controls.jpg" width="40%">.


Structures
-------------

The different structures (structs) used on the Multithreaded Pacman Videogame are as follows:

### Cell
| Attibutes                  |   DataType   | Description                                                                      |
|----------------------------|--------------|----------------------------------------------------------------------------------|
|     isPath                 |     bool     | xxxxxxxxxxxxxxxxxxxxxx                                                           |
|     resType                |     int      | xxxxxxxxxxxxxxxxxxxxxx                                                           |
|     hasPoints              |     bool     |xxxxxxxxxxxxxxxxxxxxxx                                                            |


### NPC
| Attibutes                  |   DataType   | Description                                                                      |
|----------------------------|--------------|----------------------------------------------------------------------------------|
|     isAlive                |     bool     | xxxxxxxxxxxxxxxxxxxxxx                                                           |
|     isEdible               |     bool     | xxxxxxxxxxxxxxxxxxxxxx                                                           |
|     direction              |     int      |xxxxxxxxxxxxxxxxxxxxxx                                                            |
|         x                  |     double   |xxxxxxxxxxxxxxxxxxxxxx                                                            |
|         y                  |     double   |xxxxxxxxxxxxxxxxxxxxxx                                                            |

Functions
-------------
`setDirectionPlayerRender(): void` hi
`keyboardHandler(const char *): void`
`initPlayerResources(): void`
`initMap(): void`
`initNPCS(): void`
`getMod(double,double): double`


