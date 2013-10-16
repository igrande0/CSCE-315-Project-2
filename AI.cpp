//Artificial Intelligence

/* We have to at the minimum implement the MIN-MAX algorithm
http://en.wikipedia.org/wiki/Minimax */

/* Possible Tile Values
  ___ ___ ___ ___ ___ ___ ___ ___ 
1| 99| -8|  8|  6|  6|  8| -8| 99|
2| -8|-24| -4| -3| -3| -4|-24| -8|
3|  8| -4|  7|  4|  4|  7| -4|  8|
4|  6| -3|  4|  0|  0|  4| -3|  6|
5|  6| -3|  4|  0|  0|  4| -3|  6|
6|  8| -4|  7|  4|  4|  7| -4|  8|
7| -8|-24| -4| -3| -3| -4|-24| -8|
8| 99| -8|  8|  6|  6|  8| -8| 99|
   a   b   c   d   e   f   g   h
*/


/* I believe this is how it works.
It does something like chose the maximum value that will give your opponent a minimum value
*/
class char_value{
  char status;
  int value;
};

Position max_move(vector<vector<char_value>> BOARD){
  Position position;
  int highest_value = INT_MIN;
  for(unsigned int i=0; i<available_moves.size(); i++){
    int temp_value = min_move(board_if_move_is_applied);
    if(temp_value > highest_value){
      position.row = available_moves[i].row;
      position.column = available_moves[i].column;
      highest_value = temp_value;
    }
  }
  return position;
}

Position min_move(vector<vector<char_value>> BOARD){
  Position position;
  int lowest_value = INT_MAX;
  for(unsigned int i=0; i<available_moves.size(); i++){
    int temp_value = max_move(board_if_move_is_applied);
    if(temp_value > highest_value){
      position.row = available_moves[i].row;
      position.column = available_moves[i].column;
      lowest_value = temp_value;
    }
  }
  return position;
}
