# Tic Tac Toe Robot
Developed a Tic Tac Toe robot using Lego EV3 and C, automating gameplay through sensor integration and algorithmic decision-making.

Introduing a Tic Tac Toe robot capable of playing against a human by optimizing both mechanical and software design for efficiency. The robot interprets player moves, manages piece storage, tracks the game board in memory, places pieces accurately, and determines game outcomes—win, lose, or draw—using custom algorithms. The game is built using a Minimax algorithm with Alpha-Beta Pruning to ensure a tie or a win every time a user plays. Developed for final MTE 100 project.

# Installation
1. Install [RobotC](https://www.robotc.net/) (only works on Windows computers)
2. Copy the code from [main.c] (https://github.com/nuthanan06/Tic-Tac-Toe-Robot/blob/main/main.c)
3. Download the firmware code onto your robot, robot should be configured and built similarily to the video.
4. Configure values depending on distances in [main.c](https://github.com/nuthanan06/Tic-Tac-Toe-Robot/blob/main/main.c).

# Video Demonstration

The user will press the EV3 Brick when they have made their move, and the robot will scan the entirety of the board, and make its move by sliding a piece from the dropper onto the plate, moving to the correct index, rotating the wall down, and moving the plate back to its original position to drop the piece. The process repeats until a winner is determined.

https://github.com/user-attachments/assets/d3532f4b-e34a-4fed-8cad-58be6136ac16

# Collaborators
Special thanks to Nuthanan Tharmarajah, Clark Dimond, James Kurian, Ayan Kabir Chowdhury for developing the project. 

