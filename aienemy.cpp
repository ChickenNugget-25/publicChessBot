#include "aienemy.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

enum {
    PAWN_SCORE = 10,
    KNIGHT_SCORE = 32,
    BISHOP_SCORE = 30,
    ROOK_SCORE = 50,
    QUEEN_SCORE = 90,
    KING_SCORE = 100,
};

void AIEnemy::_bind_methods() {
    ClassDB::bind_method(D_METHOD("player_moves", "from", "to"), &AIEnemy::player_moves);
    ClassDB::bind_method(D_METHOD("player_moves_promotion", "from", "to", "promotion"), &AIEnemy::player_promotion);
    ClassDB::bind_method(D_METHOD("make_random"), &AIEnemy::make_random);
    ClassDB::bind_method(D_METHOD("reset_board"), &AIEnemy::reset_board);
    ClassDB::bind_method(D_METHOD("get_board"), &AIEnemy::get_board);
    ClassDB::bind_method(D_METHOD("make_loudest"), &AIEnemy::make_loudest);
    ClassDB::bind_method(D_METHOD("perft_frame"), &AIEnemy::PerftFrame);
    ClassDB::bind_method(D_METHOD("is_promotion", "from", "to"), &AIEnemy::isPromotion);
    ClassDB::bind_method(D_METHOD("unmake"), &AIEnemy::unmake);
    ADD_SIGNAL(MethodInfo("perft_done", PropertyInfo(Variant::INT, "depth"), PropertyInfo(Variant::INT, "count"), PropertyInfo(Variant::INT, "captures")));
    ADD_SIGNAL(MethodInfo("message", PropertyInfo(Variant::STRING, "msg")));
}

int max(int x, int y) {
    if (x > y) {
        return x;
    }
    return y;
}

int min(int x, int y) {
    if (x > y) {
        return y;
    }
    return x;
}

void AIEnemy::_ready() {}

void AIEnemy::unmake() {
    board->UnMake();
}

void AIEnemy::reset_board(String QFenString, bool isWhite) {
    board->ParseQFenString(QFenString, isWhite);
}

void AIEnemy::player_promotion(int from, int to, int promotion) {
    Move testMove = board->CreateMove(from, to, promotion);
    board->printMove("lastMove", testMove);
    board->printState("lastState");
    // board->Make(testMove);
    board->pushAndMove(testMove);

    board->printState("newState");
    // board->UnMake();
    // board->printState("afterUnmake");
    // make_random();
}

void AIEnemy::player_moves(int from, int to) {
    player_promotion(from, to, -1);
}

void AIEnemy::make_random() {

    Vector<Move> moves = board->GenerateLegalMoves();
    
    if (moves.is_empty()) {
        return;
    }

    Move lastMove = moves[rand() % moves.size()];
    board->Make(lastMove);
}

// makes the highest flagged move
void AIEnemy::make_loudest() {
    int biggestIndex = 0, biggestFlag = 0;
    Vector<Move> moves = board->GenerateLegalMoves();

    if (moves.is_empty()) {
        return;
    }

    board->Make(moves[biggestIndex]);
}

void AIEnemy::make_move() {
    // iterativeDeepening(2);
    PVAlphaBeta(2, INT32_MIN, INT32_MAX, &pvline);
    board->Make(pvline.argmove[0]);
}

bool AIEnemy::isPromotion(int from, int to) {
    Move move = board->CreateMove(from, to);
    return move.isPromotion();
}

void AIEnemy::iterativeDeepening(int maxDepth) {
    for (int i = 1; i < maxDepth; i++) {
        PVAlphaBeta(i, INT32_MIN, INT32_MAX, &pvline);
    }
}

Ref<Board> AIEnemy::get_board() {
    if (board.is_valid()) {
        return board;
    }

    return 0;
}

AIEnemy::AIEnemy() {
    board.instantiate();
    board->ParseFenString("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", true);
}

// https://www.chessprogramming.org/Quiescence_Search
int AIEnemy::Quiesce( int alpha, int beta ) {
    int stand_pat = evaluate();

    if ( stand_pat >= beta ) {
        return beta;
    }
    if ( alpha < stand_pat ) {
        alpha = stand_pat;
    }

    int score;

    Vector<Move> legalMoves = board->GenerateLegalMoves();

    for (int i = 0; i < legalMoves.size(); i++) {
        if (!legalMoves[i].isCapture()) {
            continue;
        }

        board->pushAndMove(legalMoves[i]);
        score = -Quiesce( -beta, -alpha );
        board->UnMake();

        if ( score >= beta ) {
            return beta;
        }
        if ( score > alpha ) {
            alpha = score;
        }
    }

    return alpha;
}

int pieceToScore(int pieceType) {
    switch (pieceType) {
        case PAWN:
            return PAWN_SCORE;
        case KNIGHT:
            return KNIGHT_SCORE;
        case BISHOP:
            return BISHOP_SCORE;
        case ROOK:
            return ROOK_SCORE;
        case QUEEN:
            return QUEEN_SCORE;
        case KING:
            return KING_SCORE;
        case UNKNOWN:
            return UNKNOWN_SCORE;
        default:
            return 0;
    }
}

int AIEnemy::evaluate() {
    int whiteScore = 0;
    int blackScore = 0;
    for (int i = PAWN; i < SIZE; i++) {
        u64 curBB = board->get_bitboard(i);
        while (curBB) {
            if (((curBB - 1) & curBB) & board->enemyBitboard) {
                blackScore += pieceToScore(i);
            }
            else {
                whiteScore += pieceToScore(i);
            }
            curBB &= curBB - 1;
        }
    }

    return whiteScore - blackScore;
}

// https://www.chessprogramming.org/Principal_Variation
int AIEnemy::PVAlphaBeta(int depth, int alpha, int beta, PVLine* pline) {


    PVLine line;
    if (depth == 0) {
        pline->cmove = 0;
        return evaluate();
    }

    Vector<Move> legalMoves = board->GenerateLegalMoves();
    for (int i = 0; i < legalMoves.size(); i++) {
        board->pushAndMove(legalMoves[i]);
        board->printMove("alphabeta/move_" + itos(i), legalMoves[i]);
        int val = -PVAlphaBeta(depth - 1, -beta, -alpha, &line);
        board->UnMake();

        if (val >= beta) return beta;
        if (val > alpha) {
            alpha = val;
            pline->argmove[0] = legalMoves[i];
            memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(Move));
            pline->cmove = line.cmove + 1;
        }
    }

    board->printState("alphabeta/" + itos(depth) + ":" + itos(alpha));

    return alpha;
}

int AIEnemy::alphaBeta( int alpha, int beta, int depthleft, bool isWhite ) {
    if (depthleft == 0 || board->getGameOver()) {
        return evaluate();
    }

    Vector<Move> moves = board->GenerateLegalMoves();
    
    if (isWhite) {
        int maxEval = -INFINITY;
        int eval;
        for (int i = 0; i < moves.size(); i++) {
            board->pushAndMove(moves[i]);
            eval = alphaBeta( depthleft - 1, alpha, beta, false );
            board->UnMake();
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);

            if (beta <= alpha) {
                break;
            }

            return maxEval;

        }
    }
    else {
        int minEval = INFINITY;
        int eval;
        for (int i = 0; i < moves.size(); i++) {
            board->pushAndMove(moves[i]);
            eval = alphaBeta( depthleft - 1, alpha, beta, true );
            board->UnMake();
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }

        return minEval;
    }

    return 0;
}

// https://www.chessprogramming.org/Alpha-Beta
int AIEnemy::alphaBetaSoft( int alpha, int beta, int depthleft ) { 
    if ( depthleft == 0 ) return Quiesce( alpha, beta );
    int bestValue = -INFINITY; // I have no idea what this variable is

    int score;
    Vector<Move> moves = board->GenerateLegalMoves();
    for (int i = 0; i < moves.size(); i++) {
        score = -alphaBetaSoft( -beta, -alpha, depthleft - 1);
        if ( score > bestValue ) {
            bestValue = score;
            if ( score > alpha ) {
                alpha = score;
            }
        }
        if ( score >= beta ) {
            return bestValue;
        }

    }

    return bestValue;
}

int AIEnemy::alphaBetaHard( int alpha, int beta, int depthleft ) {
    if ( depthleft == 0 ) return Quiesce( alpha, beta );
    int score;
    Vector<Move> moves = board->GenerateLegalMoves();

    for (int i = 0; i < moves.size(); i++) {
        score = -alphaBetaHard( -beta, -alpha, depthleft - 1 );
        if ( score >= beta ) {
            return beta;
        }
        if ( score > alpha ) {
            alpha = score;
        }

        return alpha;
    }

    return 0;
}

AIEnemy::~AIEnemy() {}