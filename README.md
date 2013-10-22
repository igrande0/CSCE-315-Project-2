CSCE-315-Project-2
==================

A Reversi game developed in C++.

To test the game mechanics in isolation:
* Run "Reversi".
* Wait for the test game to complete.

To test the AI in isolation:
* Run "AI".
* Wait for the MEDIUM vs. HARD game to complete (HARD is white, MEDIUM is black)

To test the server/game mechanics as a unit:
* Run "Server" in an isolated terminal or in the background ("Server&" in the terminal).
* Run "Client" in the foreground.
* Initially enter "WHITE" or "BLACK" to choose color.
* Enter "EASY", "MEDIUM", or "HARD" to choose AI difficulty.
* Enter "DISPLAY" at any time to display the game state.
* Enter a move (e.g. "d3") to make a move. The AI will make a move after every move received from the client.
* Enter "UNDO" at any time to revert to the last move sent.
