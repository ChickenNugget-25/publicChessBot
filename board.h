#ifndef BOARD_H
#define BOARD_H

#include "move.h"
#include "transposition.h"
// #include "magicConstants.h"

// #include <cstdint>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

typedef u_int64_t u64;

enum {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    SIZE
};

enum {
    NORTH = -8,
    EAST = 1,
    SOUTH = 8,
    WEST = -1
};

enum {
    NO_DRAW = 0,
    DRAW_BY_REPITION,
    DRAW_BY_FIFTY_MOVE_RULE,
    DRAW_BY_INSUFFICIENT_MATERIAL
};

namespace godot {
    class Board : public RefCounted {
        GDCLASS(Board, RefCounted);

        protected:
        static void _bind_methods();

        public:

        // Stack<SuperMove> stack;
        // Zobrist hashing
        u64 pseudoRandomNumbers[937]; // different number for my chess variant
        u64 pseudoRandom();
        u64 next = 0;
        u64 getZobristNumberFromPiece(int piece, int index, bool isBlack);
        u64 zobristHash();

        Stack stack;
        int originalType;
        bool hasMoved;

        u64 bitboards[SIZE];
        u64 allPiecesBoard;
        u64 enemyBitboard;
        u64 attackBitboard;
        u64 moved;
        u64 currentZorbistHash;
        int enPassant;

        // DEBUG:
        Callable signal;
        u64 captures = 0;
        u64 checks = 0;
        u64 checkMates = 0;
        u64 enPassants = 0;
        u64 promotions = 0;
        u64 castles = 0;
        // END DEBUG

        bool isWhite;
        bool isWhiteTurn;

        bool isInCheck;
        bool gameOver;

        int fullMoves;
        int halfMoves; // fifty move counter

        Vector<int> pieceVecPlayer[6]; // Player's piece vector, index 0
        Vector<int> pieceVecEnemy[6];  // Enemy's piece vector, index 1
        int currentAxis = REAL;

        // DEBUG:
        void printState(String filename);
        void printMove(String filename, Move move);
        void printSuperMove(String filename, SuperMove superMove);
        u64 Perft(int depth, bool root = true);
        void doPerft(int maxDepth);

        // Godot Functions:
        String helloWorld();
        u64 get_bitboard(int index) const;
        u64 get_enemy_bitboard() const;
        u64 get_all_pieces_bitboard() const;
        bool get_is_white() const;
        u64 GenerateLegalMovesOnePieceBitboard(int from);
        
        // Move Helpers:
        bool getIsWhiteTurn() const;
        bool isPlayer() const;
        Move CreateMove(int from, int to, int promotion=QUEEN) const;
        u64 GetFriendlyBB();
        u64 getEnPassant();
        bool CurrentPlayerinCheck(); // current player in check?
        bool LastMoveLegal(); // other (not current) player in check?
        int isGameOver();
        int isDraw();
        bool getGameOver() const;

        // Move Generation:
        u64 PseudoMoves(int index);
        u64 PseudoMoves(int index, int type, bool isForAttkBB = false);
        Vector<Move> GeneratePseudoMoves();
        u64 GeneratePseudoMovesBitboard(bool isForAttkBB = false);

        // Move Validation:
        void SimpleMake(Move move);
        void SimpleUnMake(Move move);
        Vector<Move> GenerateLegalMoves();

        // Move Functions:
        void pushAndMove(Move move);
        void Make(Move move);
        void Make(SuperMove move);
        void UnMake();
        
        Board();
        ~Board();
    };
}

#endif