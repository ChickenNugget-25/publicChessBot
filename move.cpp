#include "move.h"

void Move::set_data(const short data) {
    this->data = data;
}

short Move::get_data() const {
    return data;
}

int Move::get_from() const {
    return data & 0x3f;
}

int Move::get_to() const {
    return (data & (0x3f << 6)) >> 6;
}

int Move::get_flags() const {
    return (data & (0xf << 12)) >> 12;
}



bool Move::get_is_enpassant() const {
    return get_flags() == 5;
    // return (get_flags() & 0x4) && (get_flags() & 0x1);
}

bool Move::isCastle() const {
    return get_flags() == 3 || get_flags() == 2;
    // return !(get_flags() & 0x8) && (get_flags() & 0x2);
}

// Only call if we know it's a castle
bool Move::isQueenSideCastle() const {
    return get_flags() & 1;
}

bool Move::isCapture() const {
    return get_flags() & 0x4;
   // return data & (0x4 << 12);
}

bool Move::hadCaptureMoved() const {
    return get_flags() & 0x2;
}

bool Move::isPromotion() const {
    return get_flags() & 0x8;
}

int Move::getPromotion() const {
    return (get_flags() & 0x3) + 1;
}

bool Move::isDoublePawnPush() const {
    return get_flags() == 1;
}

Move::Move() {
    data = -1;
}

Move::Move(int from, int to, int flags) {    
    data = from | (to << 6) | (flags << 12);
}

Move::~Move() {

}

SuperMove::SuperMove() {
    extra = -1;
    hadMoved = false;
}

SuperMove::SuperMove(Move move, int halfMoves, int enPassant, int originalType, bool hadMoved) {
    set_data(move.get_data());

    extra = (halfMoves & 0x7f) | ((enPassant & 0x3f) << 7) | ((originalType & 0x7) << 13);
    this->hadMoved = hadMoved;
}

bool SuperMove::get_hadMoved() const {
    return hadMoved;
}

int SuperMove::get_halfMoves() const {
    // 0111 1111
    return extra & 0x7f;
}

int SuperMove::get_enPassant() const {
    // 0011 1111
    // 0001 1111 1000 0000
    return (extra & (0x3f << 7)) >> 7;
}

int SuperMove::get_captured() const {
    // 0111
    // 1110 0000 0000 0000
    return (extra & (0x7 << 13)) >> 13;
}

// https://www.reddit.com/r/Cplusplus/comments/jdeo9t/how_do_stacks_work_in_c/

// template <typename T> Stack<T>::Stack() {
//     top = -1;
// }

Stack::Stack() {
    top = -1;
}

bool Stack::is_empty() {
    return top == -1;
}

void Stack::push(SuperMove sprMove) {
    top++;
    // static_assert(top <= 31);
    buffer[top] = sprMove;
}

SuperMove Stack::peek() {
    // static_assert(top >= 0);
    return buffer[top];
}

SuperMove Stack::pop() {
    top--;
    return buffer[top + 1];
    // static_assert(top >= 0);
    // return buffer[top + 1];
}