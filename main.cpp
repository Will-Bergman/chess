#include <iostream>

#include <SFML/Graphics.hpp>
#include "SFML\Window.hpp"
#include "SFML\System.hpp"

using namespace sf;
using namespace std;

typedef Vector2f fpoint;
typedef Vector2i ipoint;

#define WIDTH 1280
#define HEIGHT 720

#define SIZE 640

#define ROWS 8
#define COLS 8

#define SQUARE_W SIZE/ROWS
#define SQUARE_H SIZE/COLS 

#define SQUARE1 Vector2f(40, 40 + SIZE - SQUARE_H)

#define W_PAWN 1
#define B_PAWN -1
#define W_KNIGHT 2
#define B_KNIGHT -2
#define W_BISH 3
#define B_BISH -3
#define W_ROOK 5
#define B_ROOK -5
#define W_QUEEN 6
#define B_QUEEN -6
#define W_KING 7
#define B_KING -7

#include "Drawing.cpp"

bool isLegal(int board[], int piece, int p_square, int other_piece, int t_square, bool turn);

 int getSquare(ipoint mouse_pos, vector<RectangleShape> squares) {

    int col = 0, row = 0;
    if (mouse_pos.x < 40 || mouse_pos.x > 40 + SIZE || mouse_pos.y < 40 || mouse_pos.y > 40 + SIZE) {
        return -1;
    }
    for (int i = 0; i < COLS; i++) {
        if (mouse_pos.x >= squares[i].getPosition().x && mouse_pos.x < squares[i].getPosition().x + SQUARE_W)
            col = i;
    }
    for (int i = 0; i < ROWS; i++) {
        if (mouse_pos.y >= squares[i*8].getPosition().y && mouse_pos.y < squares[i*8].getPosition().y + SQUARE_H)
            row = i;
    }

    return (ROWS - row - 1) * ROWS + col;
    
}

 int getKingSquare(int board[], bool side) {

     if (side) {
         for (int i = 0; i < ROWS * COLS; i++) {
             if (board[i] == -7)
                 return i;
         }
     }
     else {
         for (int i = 0; i < ROWS * COLS; i++) {
             if (board[i] == 7)
                 return i;
         }
     }
     return 0;

 }

 fpoint getSquarePosition(int square) {

     return fpoint(SQUARE1.x + SQUARE_W * (square % 8), SQUARE1.y - SQUARE_H * (square / 8));

 }

 bool isSameRow(int p_square, int t_square) {

     if (p_square / 8 == t_square / 8)
         return true;
     else
         return false;

 }

 bool sameSide(int p_square, int t_square) {

     if ((p_square > 0 && t_square > 0) || (p_square < 0 && t_square < 0))
         return true;
     else
         return false;

 }

 bool otherSide(int p_square, int t_square) {

     if ((p_square > 0 && t_square < 0) || (p_square < 0 && t_square > 0))
         return true;
     else
         return false;

 }
 
 bool checkRowBlocked(int board[], int p_square, int t_square, int col_dif) {

     bool result = true, enemy_found = false;

     if (t_square > p_square) {

         for (int i = 0; i < col_dif; i++) {

             if (sameSide(board[p_square], board[p_square + (i + 1)]) || enemy_found) {
                 result = false;
             }
             if (otherSide(board[p_square], board[p_square + (i + 1)]))
                 enemy_found = true;
         }
     }
     else {
         for (int i = 0; i < col_dif; i++) {

             if (sameSide(board[p_square], board[p_square - (i + 1)]) || enemy_found) {
                 result = false;
             }
             if (otherSide(board[p_square], board[p_square - (i + 1)]))
                 enemy_found = true;
         }
     }

     return result;

 }

 bool checkColBlocked(int board[], int p_square, int t_square, int row_dif) {

     bool result = true, enemy_found = false;

     if (t_square > p_square) {
         for (int i = 0; i < row_dif; i++) {

             if (sameSide(board[p_square], board[p_square + 8 * (i + 1)]) || enemy_found)
                 result = false;
             if (otherSide(board[p_square], board[p_square + 8 * (i + 1)]))
                 enemy_found = true;
         }
     }
     else {
         for (int i = 0; i < row_dif; i++) {

             if (sameSide(board[p_square], board[p_square - 8*(i + 1)]) || enemy_found)
                 result = false;
             if (otherSide(board[p_square], board[p_square - 8 * (i + 1)]))
                 enemy_found = true;
         }
     }

     return result;

 }

 bool checkDiagBlocked(int board[], int p_square, int t_square, int row_dif, int col_dif) {

     bool result = true, enemy_found = false;

     if (row_dif == col_dif && row_dif > 0) {
         for (int i = 0; i < abs(row_dif); i++) {
             if (sameSide(board[p_square], board[p_square + 9 * (i + 1)]) || enemy_found)
                 result = false;
             if (otherSide(board[p_square], board[p_square + 9 * (i + 1)]))
                 enemy_found = true;
         }
     }
     else if (row_dif == col_dif && row_dif < 0) {
         for (int i = 0; i < abs(row_dif); i++) {
             if (sameSide(board[p_square], board[p_square - 9 * (i + 1)]) || enemy_found)
                 result = false;
             if (otherSide(board[p_square], board[p_square - 9 * (i + 1)]))
                 enemy_found = true;
         }
     }
     else if (row_dif != col_dif && row_dif > 0) {
         for (int i = 0; i < abs(row_dif); i++) {
             if (sameSide(board[p_square], board[p_square + 7 * (i + 1)]) || enemy_found)
                 result = false;
             if (otherSide(board[p_square], board[p_square + 7 * (i + 1)]))
                 enemy_found = true;
         }
     }
     else {
         for (int i = 0; i < abs(row_dif); i++) {
             if (sameSide(board[p_square], board[p_square - 7 * (i + 1)]) || enemy_found)
                 result = false;
             if (otherSide(board[p_square], board[p_square - 7 * (i + 1)]))
                 enemy_found = true;
         }
     }

     return result;

 }

 bool isDefended(int board[], int p_square, int t_square, bool turn) {

     bool result = false;

     int temp_board[ROWS * COLS];
     for (int i = 0; i < ROWS * COLS; i++) {
         temp_board[i] = board[i];
     }
     temp_board[p_square] = 0;
     if (turn)
         temp_board[t_square] = 7;
     else
         temp_board[t_square] = -7;
     
     int other_king = getKingSquare(temp_board, turn);


     for (int i = 0; i < ROWS * COLS; i++) {
             if (abs(temp_board[i]) != 7) {
                 if (isLegal(temp_board, temp_board[i], i, temp_board[t_square], t_square, !turn)) {
                     result = true;
                     break;
                 }
                 if ((other_king - 1 == t_square) || (other_king + 1 == t_square) ||
                     (other_king - 9 == t_square) || (other_king - 8 == t_square) ||
                     (other_king - 7 == t_square) || (other_king + 7 == t_square) ||
                     (other_king + 8 == t_square) || (other_king + 9 == t_square)) {
                     result = true;
                     break;
                 }
             }
     }

     return result;

 }

 bool isActuallyLegal(int board[], int p_square, int t_square, bool turn) {

     bool result = true;
     int this_king;   
     this_king = getKingSquare(board, !turn);

     int next_board[ROWS * COLS];
     for (int i = 0; i < ROWS * COLS; i++) {
         next_board[i] = board[i];
     }
     next_board[p_square] = 0;
     next_board[t_square] = board[p_square];

     if (!isLegal(board, board[p_square], p_square, board[t_square], t_square, turn)) {
         result = false;
     }
     for (int i = 0; i < ROWS * COLS; i++) {
         if (isLegal(next_board, next_board[i], i, next_board[this_king], this_king, !turn))
             result = false;
     }

     return result;

 }

 bool isLegal(int board[], int piece, int p_square, int other_piece, int t_square, bool turn) {
     
     int square_dif = t_square - p_square;
     int row_dif = t_square / 8 - p_square / 8;
     int col_dif = t_square % 8 - p_square % 8;

     if (!sameSide(piece, other_piece)) {
         if (piece == 1 && turn) {
             if (p_square / 8 == 1) {
                 if (t_square == p_square + 8 && other_piece == 0)
                     return true;
                 else if (t_square == p_square + 16 && other_piece == 0 && board[p_square + 8] == 0)
                     return true;
                 else if ((t_square == p_square + 7 || t_square == p_square + 9) && (t_square / 8) - (p_square / 8) == 1 && other_piece < 0)
                     return true;
                 else
                     return false;
             }
             else {
                 if (t_square == p_square + 8 && other_piece == 0)
                     return true;
                 else if ((t_square == p_square + 7 || t_square == p_square + 9) && (t_square / 8) - (p_square / 8) == 1 && other_piece < 0)
                     return true;
                 else
                     return false;
             }
         }
         else if (piece == -1 && !turn) {
             if (p_square / 8 == 6) {
                 if (t_square == p_square - 8 && other_piece == 0)
                     return true;
                 else if (t_square == p_square - 16 && other_piece == 0 && board[p_square - 8] == 0)
                     return true;
                 else if ((t_square == p_square - 7 || t_square == p_square - 9) && (t_square / 8) - (p_square / 8) == -1 && other_piece > 0)
                     return true;
                 else
                     return false;
             }
             else {
                 if (t_square == p_square - 8 && other_piece == 0)
                     return true;
                 else if ((t_square == p_square - 7 || t_square == p_square - 9) && (t_square / 8) - (p_square / 8) == -1 && other_piece > 0)
                     return true;
                 else
                     return false;
             }
         }
         else if ((piece == 2 && turn) || (piece == -2 && !turn)) {
             if (abs(row_dif) + abs(col_dif) == 3 && row_dif != 0 && col_dif != 0)
                 return true;
             else
                 return false;
         }
         else if ((piece == 3 && turn) || (piece == -3 && !turn)) {
             if (abs(row_dif) == abs(col_dif))
                 if (checkDiagBlocked(board, p_square, t_square, row_dif, col_dif))
                     return true;
                 else
                     return false;
             else
                 return false;


         }
         else if ((piece == 5 && turn) || (piece == -5 && !turn)) {
             if ((abs(row_dif) > 0 && col_dif == 0))

                 if (checkColBlocked(board, p_square, t_square, abs(row_dif)))
                     return true;
                 else
                     return false;
             else if ((abs(col_dif) > 0 && row_dif == 0))
                 if (checkRowBlocked(board, p_square, t_square, abs(col_dif)))
                     return true;
                 else
                     return false;
             else
                 return false;
         }
         else if ((piece == 6 && turn) || (piece == -6 && !turn)) {

             if ((abs(row_dif) > 0 && col_dif == 0))
                 if (checkColBlocked(board, p_square, t_square, abs(row_dif)))
                     return true;
                 else
                     return false;
             else if ((abs(col_dif) > 0 && row_dif == 0))
                 if (checkRowBlocked(board, p_square, t_square, abs(col_dif)))
                     return true;
                 else
                     return false;
             else if (abs(row_dif) == abs(col_dif))
                 if (checkDiagBlocked(board, p_square, t_square, row_dif, col_dif))
                     return true;
                 else
                     return false;
             else
                 return false;


         }
         else if ((piece == 7 && turn) || (piece == -7 && !turn)) {

             if ((abs(row_dif) + abs(col_dif) == 2 && abs(row_dif) == abs(col_dif)) || (abs(col_dif) + abs(row_dif) == 1)) {
                 if (!isDefended(board, p_square, t_square, turn))
                     return true;
                 else
                     return false;
             }
             else {
                 return false;
             }

         }
         else {
             return false;
         }
     }
     else {
         return false;
     }

 }

 bool isLegalinCheck(int board[], int piece, int p_square, int other_piece, int t_square, bool turn, int square_attacked) {

     // Check if legal in general
     if (isLegal(board, piece, p_square, other_piece, t_square, turn)) {
         // scan next board if move would be made
         int next_board[ROWS * COLS] = {};
         for (int i = 0; i < ROWS * COLS; i++) {
             next_board[i] = board[i];
         }
         next_board[p_square] = 0;
         next_board[t_square] = board[p_square];
        // If king square is in check still
        int this_king = getKingSquare(board,!turn);
        if (isLegal(next_board, next_board[square_attacked], square_attacked, board[this_king], this_king, !turn))
            // Not legal
            return false;
        else
            // Legal
            return true;
     }
     else
         return false;

 }

int main()
{

    // Board
    int board[64] = {

         5, 2, 3, 6, 7, 3, 2, 5,
         1, 1, 1, 1, 1, 1, 1, 1,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
        -1,-1,-1,-1,-1,-1,-1,-1,
        -5,-2,-3,-6,-7,-3,-2,-5

    };

    int legals[64] = {

         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0

    };
    
    bool turn = true, whiteCanCastle = true, blackCanCastle = true;
    bool inCheck = false;
    int square_attacked = -1;

    // Game Window
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Chess Game", Style::Default);
    window.setFramerateLimit(60);

    // Game Mouse
    Mouse mouse;
    bool alrPressed = false;

    // Game Border
    RectangleShape border(fpoint(SIZE, SIZE));
    border.setPosition(fpoint(40.f, 40.f));
    border.setFillColor(Color(0, 0, 0, 100));
    border.setOutlineThickness(20.f);
    border.setOutlineColor(Color(26, 3, 3, 255));
    
    // Highlighted Square
    RectangleShape highlighted_rec(fpoint(SQUARE_W, SQUARE_H));
    highlighted_rec.setPosition(SQUARE1);
    highlighted_rec.setFillColor(Color(255, 0, 0, 0));
    int selected;
    bool highlighted = false;

    // Game Grid
    vector<RectangleShape> squares(ROWS * COLS);
    for (int i = 0; i < squares.size(); i++) {
        squares[i].setSize(fpoint(SQUARE_W, SQUARE_H));
    }

    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            
            squares[8 * i + j].setPosition(fpoint(40.f + (float) (j * SQUARE_W), 40.f + (float) (i * SQUARE_H)));
            if((i+j) % 2 == 0)
                squares[8 * i + j].setFillColor(Color(186, 154, 115, 255));
            else
                squares[8 * i + j].setFillColor(Color(59, 48, 36, 255));

        }
    }

    // Load Game Pieces
    Texture w_pawn, b_pawn, w_knight, b_knight, w_bish, b_bish, w_rook, b_rook, w_queen, b_queen, w_king, b_king;
    w_pawn.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\W_Pawn.png");
    b_pawn.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\B_Pawn.png");
    w_knight.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\W_Knight.png");
    b_knight.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\B_Knight.png");
    w_bish.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\W_Bish.png");
    b_bish.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\B_Bish.png");
    w_rook.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\W_Rook.png");
    b_rook.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\B_Rook.png");
    w_queen.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\W_Queen.png");
    b_queen.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\B_Queen.png");
    w_king.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\W_King.png");
    b_king.loadFromFile("C:\\Users\\willi\\source\\repos\\Chess_Game_1.1\\B_King.png");


    // Sprite Grid
    vector<Sprite> sprites(ROWS * COLS);
    for (int i = 0; i < sprites.size(); i++) {

        sprites[i].setPosition(SQUARE1.x + (i % 8) * SQUARE_W, SQUARE1.y - (i / 8) * SQUARE_H);
        sprites[i].setScale((float)SQUARE_W / (float)w_pawn.getSize().x, (float)SQUARE_H / (float)w_pawn.getSize().x);


        switch (board[i]) {
        case(1):
            sprites[i].setTexture(w_pawn);
            break;
        case(2):
            sprites[i].setTexture(w_knight);
            break;
        case(3):
            sprites[i].setTexture(w_bish);
            break;
        case(5):
            sprites[i].setTexture(w_rook);
            break;
        case(6):
            sprites[i].setTexture(w_queen);
            break;
        case(7):
            sprites[i].setTexture(w_king);
            break;
        case(-1):
            sprites[i].setTexture(b_pawn);
            break;
        case(-2):
            sprites[i].setTexture(b_knight);
            break;
        case(-3):
            sprites[i].setTexture(b_bish);
            break;
        case(-5):
            sprites[i].setTexture(b_rook);
            break;
        case(-6):
            sprites[i].setTexture(b_queen);
            break;
        case(-7):
            sprites[i].setTexture(b_king);
            break;

        }

    }

    // Legal Moves Grid
    vector<CircleShape> legal_moves(ROWS * COLS);
    for (int i = 0; i < legal_moves.size(); i++) {
        legal_moves[i].setRadius(20);
        legal_moves[i].setFillColor(Color(0, 0, 0, 0));
        legal_moves[i].setPosition(SQUARE1.x + (i%8) * SQUARE_W + 0.25 * SQUARE_W, SQUARE1.y - (i/8) * SQUARE_H + 0.25 * SQUARE_H);
    }

    // Loop
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            
        }
        //----------------------------------------------------------------------------------------------------------
        // Update --------------------------------------------------------------------------------------------------
        ipoint mouse_pos = mouse.getPosition(window);
        if (!alrPressed && Mouse::isButtonPressed(Mouse::Left)) {

            // Find square clicked on (if any)
            int square_clicked_on = getSquare(mouse_pos, squares);
            if (square_clicked_on != -1) {

                // Check if there is a square already selected
                if (!highlighted) {

                    legals[square_clicked_on] = 2;
                    selected = square_clicked_on;
                    // set position of highlight
                    if ((board[square_clicked_on] > 0 && turn) || (board[square_clicked_on] < 0 && !turn)) {
                        highlighted_rec.setPosition(getSquarePosition(square_clicked_on));
                        highlighted_rec.setFillColor(Color(255, 0, 0, 100));
                        highlighted = true;
                    }
                    else {
                        highlighted_rec.setFillColor(Color(255, 0, 0, 0));
                        highlighted = false;
                    }

                   // Find legal moves
                    cout << (abs(board[selected] == 7));
                    if (!inCheck) {
;                        for (int i = 0; i < squares.size(); i++) {
                            if (!isActuallyLegal(board, square_clicked_on, i, turn))
                                legals[i] = 0;
                            if (isActuallyLegal(board, square_clicked_on, i, turn))
                                legals[i] = 1;
                        }
                    }
                    else if (inCheck && abs(board[selected]) == 7) {
                        for (int i = 0; i < squares.size(); i++) {
                            if (!isLegal(board, board[square_clicked_on], square_clicked_on, board[i], i, turn))
                                legals[i] = 0;
                            if (isLegal(board, board[square_clicked_on], square_clicked_on, board[i], i, turn))
                                legals[i] = 1;
                        }
                    }
                    else {
                        for (int i = 0; i < squares.size(); i++) {
                            if (!isActuallyLegal(board, square_clicked_on, i, turn))
                                legals[i] = 0;
                            else
                                legals[i] = 1;
                        }
                    }
                    // Show legal moves
                    for (int i = 0; i < squares.size(); i++) {
                        if (legals[i] == 1)
                            legal_moves[i].setFillColor(Color(0, 0, 0, 180));
                        if (legals[i] == 0)
                            legal_moves[i].setFillColor(Color(0, 0, 0, 0));

                    }

                }
                else {

                    highlighted_rec.setFillColor(Color(255, 0, 0, 0));
                    highlighted = false;
                    for (int i = 0; i < squares.size(); i++) {
                        legals[i] = 0;
                        legal_moves[i].setFillColor(Color(0, 0, 0, 0));
                    }

                    // Make move if legal
                    if (!inCheck) {
                        if (isActuallyLegal(board, selected, square_clicked_on, turn)) {
                            // Change board
                            int temp = board[selected];
                            board[selected] = 0;
                            board[square_clicked_on] = temp;

                            // Check if in check
                            int other_king = getKingSquare(board, turn);
                            if (isLegal(board, board[square_clicked_on], square_clicked_on, board[other_king], other_king, turn)) {
                                inCheck = true;
                                square_attacked = square_clicked_on;
                            }
                            else {
                                inCheck = false;
                            }

                            // Swap Turn
                            turn = !turn;

                            // Rescan Sprites
                            for (int i = 0; i < ROWS * COLS; i++) {

                                switch (board[i]) {
                                case(1):
                                    sprites[i].setTexture(w_pawn);
                                    break;
                                case(2):
                                    sprites[i].setTexture(w_knight);
                                    break;
                                case(3):
                                    sprites[i].setTexture(w_bish);
                                    break;
                                case(5):
                                    sprites[i].setTexture(w_rook);
                                    break;
                                case(6):
                                    sprites[i].setTexture(w_queen);
                                    break;
                                case(7):
                                    sprites[i].setTexture(w_king);
                                    break;
                                case(-1):
                                    sprites[i].setTexture(b_pawn);
                                    break;
                                case(-2):
                                    sprites[i].setTexture(b_knight);
                                    break;
                                case(-3):
                                    sprites[i].setTexture(b_bish);
                                    break;
                                case(-5):
                                    sprites[i].setTexture(b_rook);
                                    break;
                                case(-6):
                                    sprites[i].setTexture(b_queen);
                                    break;
                                case(-7):
                                    sprites[i].setTexture(b_king);
                                    break;

                                }

                            }
                        }
                    }
                    else if (inCheck && abs(board[selected]) == 7) {
                        if (isLegal(board, board[selected], selected, board[square_clicked_on], square_clicked_on, turn)) {

                            // Change board
                            int temp = board[selected];
                            board[selected] = 0;
                            board[square_clicked_on] = temp;

                            // Check if in check
                            int other_king = getKingSquare(board, turn);
                            if (isLegal(board, board[square_clicked_on], square_clicked_on, board[other_king], other_king, turn)) {
                                inCheck = true;
                                square_attacked = square_clicked_on;
                            }
                            else
                                inCheck = false;

                            // Swap Turn
                            turn = !turn;

                            // Rescan Sprites
                            for (int i = 0; i < ROWS * COLS; i++) {

                                switch (board[i]) {
                                case(1):
                                    sprites[i].setTexture(w_pawn);
                                    break;
                                case(2):
                                    sprites[i].setTexture(w_knight);
                                    break;
                                case(3):
                                    sprites[i].setTexture(w_bish);
                                    break;
                                case(5):
                                    sprites[i].setTexture(w_rook);
                                    break;
                                case(6):
                                    sprites[i].setTexture(w_queen);
                                    break;
                                case(7):
                                    sprites[i].setTexture(w_king);
                                    break;
                                case(-1):
                                    sprites[i].setTexture(b_pawn);
                                    break;
                                case(-2):
                                    sprites[i].setTexture(b_knight);
                                    break;
                                case(-3):
                                    sprites[i].setTexture(b_bish);
                                    break;
                                case(-5):
                                    sprites[i].setTexture(b_rook);
                                    break;
                                case(-6):
                                    sprites[i].setTexture(b_queen);
                                    break;
                                case(-7):
                                    sprites[i].setTexture(b_king);
                                    break;

                                }

                            }

                        }
                    }
                    else {
                        if (isActuallyLegal(board, selected, square_clicked_on, turn)) {

                            // Change board
                            int temp = board[selected];
                            board[selected] = 0;
                            board[square_clicked_on] = temp;

                            // Check if in check
                            int other_king = getKingSquare(board, turn);
                            if (isLegal(board, board[square_clicked_on], square_clicked_on, board[other_king], other_king, turn)) {
                                inCheck = true;
                                square_attacked = square_clicked_on;
                            }
                            else
                                inCheck = false;

                            // Swap Turn
                            turn = !turn;

                            // Rescan Sprites
                            for (int i = 0; i < ROWS * COLS; i++) {

                                switch (board[i]) {
                                case(1):
                                    sprites[i].setTexture(w_pawn);
                                    break;
                                case(2):
                                    sprites[i].setTexture(w_knight);
                                    break;
                                case(3):
                                    sprites[i].setTexture(w_bish);
                                    break;
                                case(5):
                                    sprites[i].setTexture(w_rook);
                                    break;
                                case(6):
                                    sprites[i].setTexture(w_queen);
                                    break;
                                case(7):
                                    sprites[i].setTexture(w_king);
                                    break;
                                case(-1):
                                    sprites[i].setTexture(b_pawn);
                                    break;
                                case(-2):
                                    sprites[i].setTexture(b_knight);
                                    break;
                                case(-3):
                                    sprites[i].setTexture(b_bish);
                                    break;
                                case(-5):
                                    sprites[i].setTexture(b_rook);
                                    break;
                                case(-6):
                                    sprites[i].setTexture(b_queen);
                                    break;
                                case(-7):
                                    sprites[i].setTexture(b_king);
                                    break;

                                }

                            }

                        }

                    }

                }

            }

            // Toggle boolean
            alrPressed = true;
        }
        if (alrPressed && !Mouse::isButtonPressed(Mouse::Left)) {


            // Toggle boolean
            alrPressed = false;
        }
        //--------------------------------------------------------------------------------------
        // Draw --------------------------------------------------------------------------------
        window.clear(Color(51, 48, 47, 255));

        window.draw(border);

        for (int i = 0; i < squares.size(); i++) {
            window.draw(squares[i]);
        }

        window.draw(highlighted_rec);

        for (int i = 0; i < sprites.size(); i++) {
            if(board[i] != 0)
                window.draw(sprites[i]);
        }
        for (int i = 0; i < squares.size(); i++) {
            window.draw(legal_moves[i]);
        }

        window.display();
        //----------------------------------------------------------------------------------------
    }

    return 0;

}
