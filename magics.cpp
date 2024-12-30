#include "board.h"
#include "magics.h"
#include <stdio.h>

// https://analog-hors.github.io/site/magic-bitboards/

typedef unsigned long long u64;
typedef unsigned char u8;

using namespace godot;

const u64 constBishopMoves[] = {0x8040201008040200, 0x80402010080500, 0x804020110a00, 0x8041221400, 0x182442800, 0x10204885000, 0x102040810a000, 0x102040810204000, 0x4020100804020002, 0x8040201008050005, 0x804020110a000a, 0x804122140014, 0x18244280028, 0x1020488500050, 0x102040810a000a0, 0x204081020400040, 0x2010080402000204, 0x4020100805000508, 0x804020110a000a11, 0x80412214001422, 0x1824428002844, 0x102048850005088, 0x2040810a000a010, 0x408102040004020, 0x1008040200020408, 0x2010080500050810, 0x4020110a000a1120, 0x8041221400142241, 0x182442800284482, 0x204885000508804, 0x40810a000a01008, 0x810204000402010, 0x804020002040810, 0x1008050005081020, 0x20110a000a112040, 0x4122140014224180, 0x8244280028448201, 0x488500050880402, 0x810a000a0100804, 0x1020400040201008, 0x402000204081020, 0x805000508102040, 0x110a000a11204080, 0x2214001422418000, 0x4428002844820100, 0x8850005088040201, 0x10a000a010080402, 0x2040004020100804, 0x200020408102040, 0x500050810204080, 0xa000a1120408000, 0x1400142241800000, 0x2800284482010000, 0x5000508804020100, 0xa000a01008040201, 0x4000402010080402, 0x2040810204080, 0x5081020408000, 0xa112040800000, 0x14224180000000, 0x28448201000000, 0x50880402010000, 0xa0100804020100, 0x40201008040201};
const u64 constRookMoves[] = {0x1010101010101fe, 0x2020202020202fd, 0x4040404040404fb, 0x8080808080808f7, 0x10101010101010ef, 0x20202020202020df, 0x40404040404040bf, 0x808080808080807f, 0x10101010101fe01, 0x20202020202fd02, 0x40404040404fb04, 0x80808080808f708, 0x101010101010ef10, 0x202020202020df20, 0x404040404040bf40, 0x8080808080807f80, 0x101010101fe0101, 0x202020202fd0202, 0x404040404fb0404, 0x808080808f70808, 0x1010101010ef1010, 0x2020202020df2020, 0x4040404040bf4040, 0x80808080807f8080, 0x1010101fe010101, 0x2020202fd020202, 0x4040404fb040404, 0x8080808f7080808, 0x10101010ef101010, 0x20202020df202020, 0x40404040bf404040, 0x808080807f808080, 0x10101fe01010101, 0x20202fd02020202, 0x40404fb04040404, 0x80808f708080808, 0x101010ef10101010, 0x202020df20202020, 0x404040bf40404040, 0x8080807f80808080, 0x101fe0101010101, 0x202fd0202020202, 0x404fb0404040404, 0x808f70808080808, 0x1010ef1010101010, 0x2020df2020202020, 0x4040bf4040404040, 0x80807f8080808080, 0x1fe010101010101, 0x2fd020202020202, 0x4fb040404040404, 0x8f7080808080808, 0x10ef101010101010, 0x20df202020202020, 0x40bf404040404040, 0x807f808080808080, 0xfe01010101010101, 0xfd02020202020202, 0xfb04040404040404, 0xf708080808080808, 0xef10101010101010, 0xdf20202020202020, 0xbf40404040404040, 0x7f80808080808080};
const u64 AFile = 0x8080808080808080;
const u64 HFile = 0x0101010101010101;
const u64 TopRank = 0xff;
const u64 BottomRank = 0xff00000000000000;

u64 next = 0;

// Magic rookMagics[64];
// Magic bishopMagics[64];

// std::map<int, u64> rookMoves[64];
// std::map<int, u64> bishopMoves[64];

u64 random_u64() {
    // rand is 31 bits?
    // 31 + 31 = 62
    // next = (next * 2032011209) + 0x89f78962d;
    // return next;
    return u64( u64(arc4random()) << 32) | arc4random();
    // return u64( u64(random()) << 32 ) | ( random() );
    // return u64( (u64(rand() & 0x3) << 62) | (u64(rand()) << 31) | rand() );
}

bool inBounds(Vector2i vec2) {
    // return !(vec2.x > 7 || vec2.y > 7 || vec2.x < 0 || vec2.y < 0);
    return (vec2.x <= 7 && vec2.x >= 0 && vec2.y <= 7 && vec2.y >= 0);
}

u64 vecToShift(Vector2i vec2) {
    return u64(1) << (vec2.x + (vec2.y * 8));
}

u64 loopOOB(Vector2i movePos, Vector2i inc, u64 subset) {
    u64 moves = 0;
    movePos += inc;

    while (inBounds(movePos)) {
        moves |= vecToShift(movePos);

        if (subset & vecToShift(movePos)) {
            break;
        }

        movePos += inc;
    }

    return moves;
} 

u64 rookMovesBlocked(int index, u64 subset) {
    
    u64 moves = 0;
    Vector2i pos = Vector2i(index % 8, int(index / 8));
    
    Vector2i inc;

    for (int i = -1; i < 2; i+=2) {
        inc = Vector2i(i, 0);
        moves |= loopOOB(pos, inc, subset);
        
        inc = Vector2i(0, i);
        moves |= loopOOB(pos, inc, subset);
    }

    return moves;
}

u64 bishopMovesBlocked(int index, u64 subset) {
    
    u64 moves = 0;
    Vector2i pos = Vector2i(index % 8, int(index / 8));

    Vector2i inc;

    for (int i = -1; i < 2; i+=2) {
        for (int j = -1; j < 2; j+=2) {
            inc = Vector2i(i, j);
            moves |= loopOOB(pos, inc, subset);
        }
    }

    return moves;

}

u64 magicIndex(const Magic magic, u64 blockers) {
    u64 actualBlockers = blockers & magic.mask;
    u64 hash = actualBlockers * magic.magic;
    return (hash >> magic.indexBits);
}

std::map<int, u64> tryMakeTable(bool isRook, int index, Magic magic) {
    
    std::map<int, u64> table = std::map<int, u64>();

    u64 subset = 0;
    do {

        subset = (subset - magic.mask) & magic.mask;
        u64 realMoves;

        if (isRook) {
            realMoves = rookMovesBlocked(index, subset);
        }
        else {
            realMoves = bishopMovesBlocked(index, subset);
        }

        int magicIndexI = magicIndex(magic, subset);

        if (table.find(magicIndexI) == table.end()) {
            table[magicIndexI] = realMoves;
        }
        else if (table[magicIndexI] != realMoves) {
            // Collision!
            return std::map<int, u64>();
        }
    }
    while (subset != 0);

    return table;
    
}

void find_magic(bool isRook, int index, std::map<int, u64>* moves, Magic* magics) {

    u64 mask;
    Vector2i vec2 = Vector2i(index % 8, int(index / 8));

    if (isRook) {
        mask = constRookMoves[index];
    }
    else {
        mask = constBishopMoves[index];
    }

    if (vec2.x != 0) {
        mask &= ~HFile;
    }
    if (vec2.x != 7) {
        mask &= ~AFile;
    }
    if (vec2.y != 0) {
        mask &= ~TopRank;
    }
    if (vec2.y != 7) {
        mask &= ~BottomRank;
    }

    u64 maskCopy = mask;
    int bits = 0;

    while (maskCopy) {
        maskCopy &= maskCopy - 1;
        bits++;
    }

    Magic magic;
    magic.mask = mask;
    magic.indexBits = 64 - bits;

    while (true) {
        magic.magic = random_u64() & random_u64() & random_u64();

        // Vector<u64> table = tryMakeTable(isRook, index, magic);
        std::map<int, u64> table = tryMakeTable(isRook, index, magic);

        if (!table.empty()) {
            moves[index] = table;
            magics[index] = magic;
            return;
        }
    }
} 

void printAll(std::map<int, u64>* rookMoves, Magic* rookMagics, std::map<int, u64>* bishopMoves, Magic* bishopMagics) {

    char str[100];

    snprintf(str, 100, "echo 'bishopMagics = {' > ~/Desktop/godot-debug/magics.txt");
    system(str);

    for (int i = 0; i < 64; i++) {
        if (i == 63) {
            snprintf(str, 100, "echo '\n\t{ %llu, %llu, %d }' >> ~/Desktop/godot-debug/magics.txt", bishopMagics[i].mask, bishopMagics[i].magic, bishopMagics[i].indexBits);
            system(str);
            break;
        }
        snprintf(str, 100, "echo '\n\t{ %llu, %llu, %d },' >> ~/Desktop/godot-debug/magics.txt", bishopMagics[i].mask, bishopMagics[i].magic, bishopMagics[i].indexBits);
        system(str);
    }

    snprintf(str, 100, "echo '}\n\nrookMagics = {' >> ~/Desktop/godot-debug/magics.txt");
    system(str);

    for (int i = 0; i < 64; i++) {
        if (i == 63) {
            snprintf(str, 100, "echo '\n\t{ %llu, %llu, %d }' >> ~/Desktop/godot-debug/magics.txt", bishopMagics[i].mask, bishopMagics[i].magic, bishopMagics[i].indexBits);
            system(str);
            break;
        }
        snprintf(str, 100, "echo '\n\t{ %llu, %llu, %d },' >> ~/Desktop/godot-debug/magics.txt", bishopMagics[i].mask, bishopMagics[i].magic, bishopMagics[i].indexBits);
        system(str);
    }

    snprintf(str, 100, "echo '}\n\nbishopMoves = {' >> ~/Desktop/godot-debug/magics.txt");
    system(str);

    for (int i = 0; i < 64; i++) {
        snprintf(str, 100, "echo '\n\t{' >> ~/Desktop/godot-debug/magics.txt");
        system(str);

        std::map<int, u64> dic = bishopMoves[i];

        for (int j = 0; j < dic.size(); j++) {
            if (j == dic.size() - 1) {
                snprintf(str, 100, "echo '\n\t\t%llu' >> ~/Desktop/godot-debug/magics.txt", dic[j]);
                system(str);
                break;
            }
            snprintf(str, 100, "echo '\n\t\t%llu,' >> ~/Desktop/godot-debug/magics.txt", dic[j]);
            system(str);
        }

        if (i == 63) { 
            snprintf(str, 100, "echo '\n},' >> ~/Desktop/godot-debug/magics.txt");
            system(str);
            break;
        }

        snprintf(str, 100, "echo '\n},' >> ~/Desktop/godot-debug/magics.txt");
        system(str);
    }

    snprintf(str, 100, "echo '\n}\n\nrookMoves = {' >> ~/Desktop/godot-debug/magics.txt");
    system(str);

    for (int i = 0; i < 64; i++) {
        snprintf(str, 100, "echo '\n\t{' >> ~/Desktop/godot-debug/magics.txt");
        system(str);

        std::map<int, u64> dic = rookMoves[i];

        for (int j = 0; j < dic.size(); j++) {
            if (j == dic.size() - 1) {
                snprintf(str, 100, "echo '\n\t\t%llu' >> ~/Desktop/godot-debug/magics.txt", dic[j]);
                system(str);
                break;
            }
            snprintf(str, 100, "echo '\n\t\t%llu,' >> ~/Desktop/godot-debug/magics.txt", dic[j]);
            system(str);
        }

        if (i == 63) {
            snprintf(str, 100, "echo '\n}' >> ~/Desktop/godot-debug/magics.txt");
            system(str);
            break;
        } 
        snprintf(str, 100, "echo '\n},' >> ~/Desktop/godot-debug/magics.txt");
        system(str);
    }

    snprintf(str, 100, "echo '\n}\n' >> ~/Desktop/godot-debug/magics.txt");
    system(str);
    
}

void getMagics(std::map<int, u64>* rookMoves, Magic* rookMagics, std::map<int, u64>* bishopMoves, Magic* bishopMagics) {

    for (int i = 0; i < 64; i++) {
        find_magic(true, i, rookMoves, rookMagics);
    }

    for (int i = 0; i < 64; i++) {
        find_magic(false, i, bishopMoves, bishopMagics);
    }
}

void makeTables(std::map<int, u64>* rookMoves, Magic* rookMagics, std::map<int, u64>* bishopMoves, Magic* bishopMagics) {
    for (int i = 0; i < 64; i++) {
        // isRook
        std::map<int, u64> table = tryMakeTable(true, i, rookMagics[i]);
        if (!table.empty()) {
            rookMoves[i] = table;
        }
        else {
            system("echo 'something broke' > ~/Desktop/godot-debug/error.txt");
        }

        // isBishop
        table = tryMakeTable(false, i, bishopMagics[i]);
        if (!table.empty()) {
            bishopMoves[i] = table;
        }
    }
}