#ifndef MAGICS_H
#define MAGICS_H

#include <map>

typedef unsigned char u8;
using namespace godot;

struct Magic {
    u64 mask;
    u64 magic;
    u8 indexBits;
};

void makeTables(std::map<int, u64>* rookMoves, Magic* rookMagics, std::map<int, u64>* bishopMoves, Magic* bishopMagics);
void getMagics(std::map<int, u64>* rookMoves, Magic* rookMagics, std::map<int, u64>* bishopMoves, Magic* bishopMagics);
void printAll(std::map<int, u64>* rookMoves, Magic* rookMagics, std::map<int, u64>* bishopMoves, Magic* bishopMagics);
u64 magicIndex(const Magic magic, u64 blockers);
u64 rookMovesBlocked(int index, u64 blockers);
u64 bishopMovesBlocked(int index, u64 blockers);
bool inBounds(Vector2i vec2);

#endif