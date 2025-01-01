#include "magicConstants.h"

u64 magicIndexIN(const Magic magic, u64 blockers) {
    u64 actualBlockers = blockers & magic.mask;
    u64 hash = actualBlockers * magic.magic;
    return (hash >> magic.indexBits);
}

u64 getBishopMoves(int index, u64 allPiecesBoard) {
	return bishopMoves[index][magicIndexIN(bishopMagics[index], allPiecesBoard)];
}

u64 getRookMoves(int index, u64 allPiecesBoard) {
    return rookMoves[index][magicIndexIN(rookMagics[index], allPiecesBoard)];
}