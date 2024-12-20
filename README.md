# Frogger Game

A simple Frogger game written in C, featuring plants that shoot and crocodiles to dodge. The game uses the `ncurses` and `pthread` libraries for rendering and multithreading.

## Requirements

To run this project, ensure you have the following tools and libraries installed:

- GCC (C compiler)
- GNU Make
- `ncurses` library
- `pthread` library

### Install Required Libraries (Linux)

Run the following commands to install the necessary dependencies:

"""bash
sudo apt-get update
sudo apt-get install build-essential libncurses5-dev libncursesw5-dev"""

## How to run (Linux)

- Clone the repository
  git clone https://github.com/GloriaRagnedda/frogger.git
  cd frogger

- Compile the project
  make compile

- To link the object files and create the executable:
  make linking

- Run the game
  ./Frogger
