#ifndef MOVE_H
#define MOVE_H

#ifndef u64
typedef  unsigned long long u64;
#endif

enum {
    STACK_MAX = 16
};

class Move {

    // 2 bytes

    private:
    short data;

    // Regular Move: 
    // from     (6) (0-5)
    // to       (6) (6-11)
    // flags    (4) (12-15)
    // promotion, capture, special 0, special 1
    // special 0 && capture ? special 0 == captured_moved

    public:

    void set_data(const short data);
    short get_data() const;
    int get_from() const;
    int get_to() const;
    int get_flags() const;

    bool isCapture() const;
    bool hadCaptureMoved() const;
    bool isCastle() const;
    bool isQueenSideCastle() const;
    bool isPromotion() const;
    bool isDoublePawnPush() const;

    bool get_is_enpassant() const;
    int getPromotion() const;

    Move();
    Move(int from, int to, int flags);
    ~Move();
};

class SuperMove : public Move {

    // extra (2) + data (2) + hadMoved (1/8)
    // = 6.125 bytes

    short extra;
    // halfMoves        (7) (0-6)
    // enPassant        (6) (7-12)

    // captured + 1     (3) (13-15)
    // the piece that was captured ^

    bool hadMoved;
    // hadMoved         (1) 
    public:

    bool get_hadMoved() const;
    int get_halfMoves() const;
    int get_enPassant() const;
    int get_captured() const;

    SuperMove();
    SuperMove(Move move, int halfMoves, int enPassant, int originalType, bool hadMoved);
};

// template <typename T> class Stack {

//     // 16 ply (8 moves)
//     T buffer[16];
//     // where data is
//     unsigned int top;

//     public:
//     Stack();

//     bool is_empty();
//     void push(T sprMove);
//     T peek();
//     T pop();

// };

class Stack {
    // 16 ply (8 moves)
    SuperMove buffer[STACK_MAX];
    
    public:
    unsigned int top;
    Stack();

    bool is_empty();
    void push(SuperMove sprMove);
    SuperMove peek();
    SuperMove pop();
};

// https://www.chessprogramming.org/Principal_Variation
struct PVLine {
    int cmove;
    Move argmove[STACK_MAX];
};

#endif