CSCE-315-Project-2
==================

A Reversi game developed in C++. Game mechanics and server code are complete but need to be fully tested and debugged.

To test the game mechanics in isolation:
* Run "Reversi".
* Wait for the test game to complete.

To test the server/game mechanics as a unit:
* Run "Server" in an isolated terminal or in the background ("Server&" in the terminal).
* Run "Client" in the foreground.
* Initially enter "WHITE" or "BLACK" to choose color.
* Enter "DISPLAY" at any time to display the game state.
* Enter a move (e.g. "d3") to make a move. Server will make a random move after every move received from the client.
