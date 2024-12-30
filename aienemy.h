#ifndef AIENEMY_H
#define AIENEMY_H

#include "board.h"

#include <godot_cpp/classes/node.hpp>

namespace godot {
    class AIEnemy : public Node {
        GDCLASS(AIEnemy, Node)

        private:
        Ref<Board> board;
        PVLine pvline;
        bool accurateLine;

        protected:
        static void _bind_methods();

        public:
        // Godot Functions:
        void _ready() override;
        void player_moves(int from, int to);
        void player_promotion(int from, int to, int promotion);
        void make_random();
        void make_loudest();
        void reset_board(String QFenString, bool isWhite);
        Ref<Board> get_board();
        bool isPromotion(int from, int to);
        void unmake();

        void doSignal();

        void make_move();
        void iterativeDeepening(int maxDepth);
        int PVAlphaBeta(int depth, int alpha, int beta, PVLine* pline);
        int alphaBeta(int alpha, int beta, int dephtleft, bool isWhite);
        int alphaBetaSoft(int alpha, int beta, int depthleft);
        int alphaBetaHard(int alpha, int beta, int depthleft);
        int Quiesce(int alpha, int beta);
        int evaluate();

        // PERFT STUFF
        u64 perftCount = 0;
        u64 captures = 0;

        int perftFrame = 0;
        int depthIndex = 0;
        int depth = 0;
        bool isMaking = true;
        bool isPositive = true;
        bool lastMoveCapture = false;
        void PerftFrame();
        static const int maxDepth = 4;
        Vector<Move> moves[maxDepth];
        int moveIndecies[maxDepth];
        // PERFT STUFF
        
        AIEnemy();
        ~AIEnemy();
    };
}

#endif