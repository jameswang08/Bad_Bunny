Project for CS31, focused on working with arrays, pointers, and memory managment
*Everything is in one file because we had yet to learn how to create multi-file projects

Project Spec:

An evil wizard has produced many clones of the killer rabbit of Caerbannog and has trapped you in an enclosure with them. Fortunately, you have a supply of poisoned baby carrots.

Well, that's the scenario for a new video game under development. Your assignment is to complete the prototype that uses character-based graphics.

If you execute this Windows program or this Mac program or this Linux program, you will see the player (indicated by @) in a rectangular arena filled with rabbits (usually indicated by R). At each turn, the user will select an action for the player to take: either move one step or drop a poisoned carrot without moving. The player will take the action, and then each rabbit will move one step in a random direction. If a rabbit moves onto the grid point occupied by the player, the player dies from the rabbit's attack. (If the player moves to a grid point occupied by a rabbit, the player is attacked and dies, so that would be a dumb move.)

If a rabbit lands on a grid point with a poisoned carrot, it eats the carrot. The first time a rabbit eats a poisoned carrot, it slows down: it doesn't move on its next turn, but it moves on the turn after that, then on the next turn it doesn't move, then it moves on the turn after that, etc., moving only every other turn. The second time a rabbit eats a poisoned carrot, it dies. (If a poisoned rabbit moves to a grid point with both the player and a poisoned carrot, it eats the carrot and just before it dies, it attacks the player, so both die.)

This smaller Windows version or Mac version or Linux version of the game may help you see the operation of the game more clearly.

At each turn the player may take one of these actions:

Move one step north, east, south, or west, and do not drop a poisoned carrot. If the player attempts to move out of the arena (e.g., south, when on the bottom row), the player does not move, and does not drop a vial. If the player moves to a grid point currently occupied by a rabbit, the player dies.
Do not move, but attempt to drop a poisoned carrot. If there is already a poisoned carrot at that grid point, no additional carrot is dropped; a grid point may have at most one poisoned carrot. The player has an unlimited supply of poisoned carrots.
The game allows the user to select the player's action: n/e/s/w for movement, or c for dropping a poisoned carrot. The user may also just hit enter to have the computer select the player's move.

After the player moves, it's the rabbits' turn. Each rabbit has an opportunity to move. A rabbit that has previously eaten a poisoned carrot will not move if it attempted to move on the previous turn. Otherwise, it will pick a random direction (north, east, south, west) with equal probability. The rabbit moves one step in that direction if it can; if the rabbit attempts to move off the grid, however, it does not move (but this still counts as a poisoned rabbit's attempt to move, so it won't move on the next turn). More than one rabbit may occupy the same grid point; in that case, instead of R, the display will show a digit character indicating the number of rabbits at that point (where 9 indicates 9 or more).

If after a rabbit moves, it occupies the same grid point as the player (whether or not there's a poisoned carrot at that point), the player dies. If the rabbit lands on a grid point with a poisoned carrot on it, it eats the carrot (and the carrot is no longer part of the game). If this is the second poisoned carrot the rabbit has eaten, it dies. If more than one rabbit lands on a spot that started the turn with a poisoned carrot on it, only one of them eats the poisoned carrot.

The CS 31 assignment was to complete a C++ program skeleton to produce a program that implements the described behavior. For CS 32 Project 1, we will give you a correct solution to the CS 31 assignment. The program defines four classes that represent the four kinds of objects this program works with: Game, Arena, Rabbit, and Player. Details of the interface to these classes are in the program code, but here are the essential responsibilities of each class:

Game

To create a Game, you specify a number of rows and columns and the number of rabbits to start with. The Game object creates an appropriately sized Arena and populates it with the Player and the Rabbits.
A game may be played.
Arena

When an Arena object of a particular size is created, it has no positions occupied by rabbits or the player. In the Arena coordinate system, row 1, column 1 is the upper-left-most position that can be occupied by a rabbit or the player. If an Arena created with 10 rows and 20 columns, then the lower-right-most position that could be occupied would be row 10, column 20.
You may tell an Arena object to put a poisoned carrot at a particular position.
You may ask an Arena object whether there's a poisoned carrot at a particular position.
You may tell an Arena object to create a Rabbit at a particular position.
You may tell an Arena object to create a Player at a particular position.
You may tell an Arena object to have all the rabbits in it make their move.
You may ask an Arena object its size, how many rabbits are at a particular position, and how many rabbits altogether are in the Arena.
You may ask an Arena object for access to its player.
An Arena object may be displayed on the screen, showing the locations of the rabbits, the player, and the poisoned carrots, along with other status information.
Player

A Player is created at some position (using the Arena coordinate system, where row 1, column 1 is the upper-left-most position, etc.).
You may tell a Player to move in a direction or to drop a poisoned carrot.
You may tell a Player that it has died.
You may ask a Player for its position and its alive/dead status.
Rabbit

A Rabbit is created at some position.
You may tell a Rabbit to move.
You may ask a Rabbit object for its position and its alive/dead status.
CS 31 students were told:

Complete the implementation in accordance with the description of the game. You are allowed to make whatever changes you want to the private parts of the classes: You may add or remove private data members or private member functions, or change their types. You must not make any deletions, additions, or changes to the public interface of any of these classes — we're depending on them staying the same so that we can test your programs. You can and will, of course, make changes to the implementations of public member functions, since the callers of the function wouldn't have to change any of the code they write to call the function. You must not declare any public data members, nor use any global variables whose values may change during execution; global constants are OK. You may add additional functions that are not members of any class. The word friend must not appear in your program.

Any member functions you implement must never put an object into an invalid state, one that will cause a problem later on. (For example, bad things could come from placing a rabbit outside the arena.) Any function that has a reasonable way of indicating failure through its return value should do so. Constructors pose a special difficulty because they can't return a value. If a constructor can't do its job, we have it write an error message and exit the program with failure by calling exit(1);. (We haven't learned about throwing an exception to signal constructor failure.)
