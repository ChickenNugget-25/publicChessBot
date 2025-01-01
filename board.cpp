#include "board.h"
#include "move.h"
#include "magics.h"
// #include "magicConstants.h"
#include <map>

using namespace godot;

// https://analog-hors.github.io/site/magic-bitboards/
// https://essays.jwatzman.org/essays/chess-move-generation-with-magic-bitboards.html

// const u64 rookMagics[];
// const u64 bishopMagics[];

// 7 * 64 * 8 bytes
// = 3,584 bytes
// = 3.5 kB

// const u64 pawnMovesWhite[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x101, 0x202, 0x404, 0x808, 0x1010, 0x2020, 0x4040, 0x8080, 0x10100, 0x20200, 0x40400, 0x80800, 0x101000, 0x202000, 0x404000, 0x808000, 0x1010000, 0x2020000, 0x4040000, 0x8080000, 0x10100000, 0x20200000, 0x40400000, 0x80800000, 0x101000000, 0x202000000, 0x404000000, 0x808000000, 0x1010000000, 0x2020000000, 0x4040000000, 0x8080000000, 0x10100000000, 0x20200000000, 0x40400000000, 0x80800000000, 0x101000000000, 0x202000000000, 0x404000000000, 0x808000000000, 0x1010000000000, 0x2020000000000, 0x4040000000000, 0x8080000000000, 0x10100000000000, 0x20200000000000, 0x40400000000000, 0x80800000000000};
// const u64 pawnMovesBlack[] = {0x10100, 0x20200, 0x40400, 0x80800, 0x101000, 0x202000, 0x404000, 0x808000, 0x1010000, 0x2020000, 0x4040000, 0x8080000, 0x10100000, 0x20200000, 0x40400000, 0x80800000, 0x101000000, 0x202000000, 0x404000000, 0x808000000, 0x1010000000, 0x2020000000, 0x4040000000, 0x8080000000, 0x10100000000, 0x20200000000, 0x40400000000, 0x80800000000, 0x101000000000, 0x202000000000, 0x404000000000, 0x808000000000, 0x1010000000000, 0x2020000000000, 0x4040000000000, 0x8080000000000, 0x10100000000000, 0x20200000000000, 0x40400000000000, 0x80800000000000, 0x101000000000000, 0x202000000000000, 0x404000000000000, 0x808000000000000, 0x1010000000000000, 0x2020000000000000, 0x4040000000000000, 0x8080000000000000, 0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; // layout 1
const u64 pawnMovesBlack[] = {0x10100, 0x20200, 0x40400, 0x80800, 0x101000, 0x202000, 0x404000, 0x808000, 0x1010000, 0x2020000, 0x4040000, 0x8080000, 0x10100000, 0x20200000, 0x40400000, 0x80800000, 0x101000000, 0x202000000, 0x404000000, 0x808000000, 0x1010000000, 0x2020000000, 0x4040000000, 0x8080000000, 0x10100000000, 0x20200000000, 0x40400000000, 0x80800000000, 0x101000000000, 0x202000000000, 0x404000000000, 0x808000000000, 0x1010000000000, 0x2020000000000, 0x4040000000000, 0x8080000000000, 0x10100000000000, 0x20200000000000, 0x40400000000000, 0x80800000000000, 0x101000000000000, 0x202000000000000, 0x404000000000000, 0x808000000000000, 0x1010000000000000, 0x2020000000000000, 0x4040000000000000, 0x8080000000000000, 0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}; // layout 1
const u64 pawnAttacksWhite[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x5, 0xa, 0x14, 0x28, 0x50, 0xa0, 0x40, 0x200, 0x500, 0xa00, 0x1400, 0x2800, 0x5000, 0xa000, 0x4000, 0x20000, 0x50000, 0xa0000, 0x140000, 0x280000, 0x500000, 0xa00000, 0x400000, 0x2000000, 0x5000000, 0xa000000, 0x14000000, 0x28000000, 0x50000000, 0xa0000000, 0x40000000, 0x200000000, 0x500000000, 0xa00000000, 0x1400000000, 0x2800000000, 0x5000000000, 0xa000000000, 0x4000000000, 0x20000000000, 0x50000000000, 0xa0000000000, 0x140000000000, 0x280000000000, 0x500000000000, 0xa00000000000, 0x400000000000, 0x2000000000000, 0x5000000000000, 0xa000000000000, 0x14000000000000, 0x28000000000000, 0x50000000000000, 0xa0000000000000, 0x40000000000000};
const u64 pawnAttacksBlack[] = {0x200, 0x500, 0xa00, 0x1400, 0x2800, 0x5000, 0xa000, 0x4000, 0x20000, 0x50000, 0xa0000, 0x140000, 0x280000, 0x500000, 0xa00000, 0x400000, 0x2000000, 0x5000000, 0xa000000, 0x14000000, 0x28000000, 0x50000000, 0xa0000000, 0x40000000, 0x200000000, 0x500000000, 0xa00000000, 0x1400000000, 0x2800000000, 0x5000000000, 0xa000000000, 0x4000000000, 0x20000000000, 0x50000000000, 0xa0000000000, 0x140000000000, 0x280000000000, 0x500000000000, 0xa00000000000, 0x400000000000, 0x2000000000000, 0x5000000000000, 0xa000000000000, 0x14000000000000, 0x28000000000000, 0x50000000000000, 0xa0000000000000, 0x40000000000000, 0x200000000000000, 0x500000000000000, 0xa00000000000000, 0x1400000000000000, 0x2800000000000000, 0x5000000000000000, 0xa000000000000000, 0x4000000000000000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
// const u64 pawnMoves[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8000000000000000, 0x4000000000000000, 0x2000000000000000, 0x1000000000000000, 0x800000000000000, 0x400000000000000, 0x200000000000000, 0x100000000000000, 0x80000000000000, 0x40000000000000, 0x20000000000000, 0x10000000000000, 0x8000000000000, 0x4000000000000, 0x2000000000000, 0x1000000000000, 0x800000000000, 0x400000000000, 0x200000000000, 0x100000000000, 0x80000000000, 0x40000000000, 0x20000000000, 0x10000000000, 0x8000000000, 0x4000000000, 0x2000000000, 0x1000000000, 0x800000000, 0x400000000, 0x200000000, 0x100000000, 0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x8000000, 0x4000000, 0x2000000, 0x1000000, 0x800000, 0x400000, 0x200000, 0x100000, 0x80000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000, 0x2000, 0x1000, 0x800, 0x400, 0x200, 0x100};
// const u64 pawnAttacks[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x5, 0xa, 0x14, 0x28, 0x50, 0xa0, 0x40, 0x200, 0x500, 0xa00, 0x1400, 0x2800, 0x5000, 0xa000, 0x4000, 0x20000, 0x50000, 0xa0000, 0x140000, 0x280000, 0x500000, 0xa00000, 0x400000, 0x2000000, 0x5000000, 0xa000000, 0x14000000, 0x28000000, 0x50000000, 0xa0000000, 0x40000000, 0x200000000, 0x500000000, 0xa00000000, 0x1400000000, 0x2800000000, 0x5000000000, 0xa000000000, 0x4000000000, 0x20000000000, 0x50000000000, 0xa0000000000, 0x140000000000, 0x280000000000, 0x500000000000, 0xa00000000000, 0x400000000000, 0x2000000000000, 0x5000000000000, 0xa000000000000, 0x14000000000000, 0x28000000000000, 0x50000000000000, 0xa0000000000000, 0x40000000000000};
const u64 knightMoves[] = {0x20400, 0x50800, 0xa1100, 0x142200, 0x284400, 0x508800, 0xa01000, 0x402000, 0x2040004, 0x5080008, 0xa110011, 0x14220022, 0x28440044, 0x50880088, 0xa0100010, 0x40200020, 0x204000402, 0x508000805, 0xa1100110a, 0x1422002214, 0x2844004428, 0x5088008850, 0xa0100010a0, 0x4020002040, 0x20400040200, 0x50800080500, 0xa1100110a00, 0x142200221400, 0x284400442800, 0x508800885000, 0xa0100010a000, 0x402000204000, 0x2040004020000, 0x5080008050000, 0xa1100110a0000, 0x14220022140000, 0x28440044280000, 0x50880088500000, 0xa0100010a00000, 0x40200020400000, 0x204000402000000, 0x508000805000000, 0xa1100110a000000, 0x1422002214000000, 0x2844004428000000, 0x5088008850000000, 0xa0100010a0000000, 0x4020002040000000, 0x400040200000000, 0x800080500000000, 0x1100110a00000000, 0x2200221400000000, 0x4400442800000000, 0x8800885000000000, 0x100010a000000000, 0x2000204000000000, 0x4020000000000, 0x8050000000000, 0x110a0000000000, 0x22140000000000, 0x44280000000000, 0x88500000000000, 0x10a00000000000, 0x20400000000000}; // layout 1
// const u64 knightMoves[] = {0x20400000000000, 0x10a00000000000, 0x88500000000000, 0x44280000000000, 0x22140000000000, 0x110a0000000000, 0x8050000000000, 0x4020000000000, 0x2000204000000000, 0x100010a000000000, 0x8800885000000000, 0x4400442800000000, 0x2200221400000000, 0x1100110a00000000, 0x800080500000000, 0x400040200000000, 0x4020002040000000, 0xa0100010a0000000, 0x5088008850000000, 0x2844004428000000, 0x1422002214000000, 0xa1100110a000000, 0x508000805000000, 0x204000402000000, 0x40200020400000, 0xa0100010a00000, 0x50880088500000, 0x28440044280000, 0x14220022140000, 0xa1100110a0000, 0x5080008050000, 0x2040004020000, 0x402000204000, 0xa0100010a000, 0x508800885000, 0x284400442800, 0x142200221400, 0xa1100110a00, 0x50800080500, 0x20400040200, 0x4020002040, 0xa0100010a0, 0x5088008850, 0x2844004428, 0x1422002214, 0xa1100110a, 0x508000805, 0x204000402, 0x40200020, 0xa0100010, 0x50880088, 0x28440044, 0x14220022, 0xa110011, 0x5080008, 0x2040004, 0x402000, 0xa01000, 0x508800, 0x284400, 0x142200, 0xa1100, 0x50800, 0x20400};
// const u64 bishopMoves[] = {0x8040201008040200, 0x80402010080500, 0x804020110a00, 0x8041221400, 0x182442800, 0x10204885000, 0x102040810a000, 0x102040810204000, 0x4020100804020002, 0x8040201008050005, 0x804020110a000a, 0x804122140014, 0x18244280028, 0x1020488500050, 0x102040810a000a0, 0x204081020400040, 0x2010080402000204, 0x4020100805000508, 0x804020110a000a11, 0x80412214001422, 0x1824428002844, 0x102048850005088, 0x2040810a000a010, 0x408102040004020, 0x1008040200020408, 0x2010080500050810, 0x4020110a000a1120, 0x8041221400142241, 0x182442800284482, 0x204885000508804, 0x40810a000a01008, 0x810204000402010, 0x804020002040810, 0x1008050005081020, 0x20110a000a112040, 0x4122140014224180, 0x8244280028448201, 0x488500050880402, 0x810a000a0100804, 0x1020400040201008, 0x402000204081020, 0x805000508102040, 0x110a000a11204080, 0x2214001422418000, 0x4428002844820100, 0x8850005088040201, 0x10a000a010080402, 0x2040004020100804, 0x200020408102040, 0x500050810204080, 0xa000a1120408000, 0x1400142241800000, 0x2800284482010000, 0x5000508804020100, 0xa000a01008040201, 0x4000402010080402, 0x2040810204080, 0x5081020408000, 0xa112040800000, 0x14224180000000, 0x28448201000000, 0x50880402010000, 0xa0100804020100, 0x40201008040201}; // layout 1
// const u64 bishopMoves[] = {0x40201008040201, 0xa0100804020100, 0x50880402010000, 0x28448201000000, 0x14224180000000, 0xa112040800000, 0x5081020408000, 0x2040810204080, 0x4000402010080402, 0xa000a01008040201, 0x5000508804020100, 0x2800284482010000, 0x1400142241800000, 0xa000a1120408000, 0x500050810204080, 0x200020408102040, 0x2040004020100804, 0x10a000a010080402, 0x8850005088040201, 0x4428002844820100, 0x2214001422418000, 0x110a000a11204080, 0x805000508102040, 0x402000204081020, 0x1020400040201008, 0x810a000a0100804, 0x488500050880402, 0x8244280028448201, 0x4122140014224180, 0x20110a000a112040, 0x1008050005081020, 0x804020002040810, 0x810204000402010, 0x40810a000a01008, 0x204885000508804, 0x182442800284482, 0x8041221400142241, 0x4020110a000a1120, 0x2010080500050810, 0x1008040200020408, 0x408102040004020, 0x2040810a000a010, 0x102048850005088, 0x1824428002844, 0x80412214001422, 0x804020110a000a11, 0x4020100805000508, 0x2010080402000204, 0x204081020400040, 0x102040810a000a0, 0x1020488500050, 0x18244280028, 0x804122140014, 0x804020110a000a, 0x8040201008050005, 0x4020100804020002, 0x102040810204000, 0x102040810a000, 0x10204885000, 0x182442800, 0x8041221400, 0x804020110a00, 0x80402010080500, 0x8040201008040200};
// const u64 rookMoves[] = {0x1010101010101fe, 0x2020202020202fd, 0x4040404040404fb, 0x8080808080808f7, 0x10101010101010ef, 0x20202020202020df, 0x40404040404040bf, 0x808080808080807f, 0x10101010101fe01, 0x20202020202fd02, 0x40404040404fb04, 0x80808080808f708, 0x101010101010ef10, 0x202020202020df20, 0x404040404040bf40, 0x8080808080807f80, 0x101010101fe0101, 0x202020202fd0202, 0x404040404fb0404, 0x808080808f70808, 0x1010101010ef1010, 0x2020202020df2020, 0x4040404040bf4040, 0x80808080807f8080, 0x1010101fe010101, 0x2020202fd020202, 0x4040404fb040404, 0x8080808f7080808, 0x10101010ef101010, 0x20202020df202020, 0x40404040bf404040, 0x808080807f808080, 0x10101fe01010101, 0x20202fd02020202, 0x40404fb04040404, 0x80808f708080808, 0x101010ef10101010, 0x202020df20202020, 0x404040bf40404040, 0x8080807f80808080, 0x101fe0101010101, 0x202fd0202020202, 0x404fb0404040404, 0x808f70808080808, 0x1010ef1010101010, 0x2020df2020202020, 0x4040bf4040404040, 0x80807f8080808080, 0x1fe010101010101, 0x2fd020202020202, 0x4fb040404040404, 0x8f7080808080808, 0x10ef101010101010, 0x20df202020202020, 0x40bf404040404040, 0x807f808080808080, 0xfe01010101010101, 0xfd02020202020202, 0xfb04040404040404, 0xf708080808080808, 0xef10101010101010, 0xdf20202020202020, 0xbf40404040404040, 0x7f80808080808080}; // layout 1
// const u64 rookMoves[] = {0x7f80808080808080, 0xbf40404040404040, 0xdf20202020202020, 0xef10101010101010, 0xf708080808080808, 0xfb04040404040404, 0xfd02020202020202, 0xfe01010101010101, 0x807f808080808080, 0x40bf404040404040, 0x20df202020202020, 0x10ef101010101010, 0x8f7080808080808, 0x4fb040404040404, 0x2fd020202020202, 0x1fe010101010101, 0x80807f8080808080, 0x4040bf4040404040, 0x2020df2020202020, 0x1010ef1010101010, 0x808f70808080808, 0x404fb0404040404, 0x202fd0202020202, 0x101fe0101010101, 0x8080807f80808080, 0x404040bf40404040, 0x202020df20202020, 0x101010ef10101010, 0x80808f708080808, 0x40404fb04040404, 0x20202fd02020202, 0x10101fe01010101, 0x808080807f808080, 0x40404040bf404040, 0x20202020df202020, 0x10101010ef101010, 0x8080808f7080808, 0x4040404fb040404, 0x2020202fd020202, 0x1010101fe010101, 0x80808080807f8080, 0x4040404040bf4040, 0x2020202020df2020, 0x1010101010ef1010, 0x808080808f70808, 0x404040404fb0404, 0x202020202fd0202, 0x101010101fe0101, 0x8080808080807f80, 0x404040404040bf40, 0x202020202020df20, 0x101010101010ef10, 0x80808080808f708, 0x40404040404fb04, 0x20202020202fd02, 0x10101010101fe01, 0x808080808080807f, 0x40404040404040bf, 0x20202020202020df, 0x10101010101010ef, 0x8080808080808f7, 0x4040404040404fb, 0x2020202020202fd, 0x1010101010101fe};
// const u64 queenMoves[] = {0x81412111090503fe, 0x2824222120a07fd, 0x404844424150efb, 0x8080888492a1cf7, 0x10101011925438ef, 0x2020212224a870df, 0x404142444850e0bf, 0x8182848890a0c07f, 0x412111090503fe03, 0x824222120a07fd07, 0x4844424150efb0e, 0x80888492a1cf71c, 0x101011925438ef38, 0x20212224a870df70, 0x4142444850e0bfe0, 0x82848890a0c07fc0, 0x2111090503fe0305, 0x4222120a07fd070a, 0x844424150efb0e15, 0x888492a1cf71c2a, 0x1011925438ef3854, 0x212224a870df70a8, 0x42444850e0bfe050, 0x848890a0c07fc0a0, 0x11090503fe030509, 0x22120a07fd070a12, 0x4424150efb0e1524, 0x88492a1cf71c2a49, 0x11925438ef385492, 0x2224a870df70a824, 0x444850e0bfe05048, 0x8890a0c07fc0a090, 0x90503fe03050911, 0x120a07fd070a1222, 0x24150efb0e152444, 0x492a1cf71c2a4988, 0x925438ef38549211, 0x24a870df70a82422, 0x4850e0bfe0504844, 0x90a0c07fc0a09088, 0x503fe0305091121, 0xa07fd070a122242, 0x150efb0e15244484, 0x2a1cf71c2a498808, 0x5438ef3854921110, 0xa870df70a8242221, 0x50e0bfe050484442, 0xa0c07fc0a0908884, 0x3fe030509112141, 0x7fd070a12224282, 0xefb0e1524448404, 0x1cf71c2a49880808, 0x38ef385492111010, 0x70df70a824222120, 0xe0bfe05048444241, 0xc07fc0a090888482, 0xfe03050911214181, 0xfd070a1222428202, 0xfb0e152444840404, 0xf71c2a4988080808, 0xef38549211101010, 0xdf70a82422212020, 0xbfe0504844424140, 0x7fc0a09088848281}; // layout 1
// const u64 queenMoves[] = {0x7fc0a09088848281, 0xbfe0504844424140, 0xdf70a82422212020, 0xef38549211101010, 0xf71c2a4988080808, 0xfb0e152444840404, 0xfd070a1222428202, 0xfe03050911214181, 0xc07fc0a090888482, 0xe0bfe05048444241, 0x70df70a824222120, 0x38ef385492111010, 0x1cf71c2a49880808, 0xefb0e1524448404, 0x7fd070a12224282, 0x3fe030509112141, 0xa0c07fc0a0908884, 0x50e0bfe050484442, 0xa870df70a8242221, 0x5438ef3854921110, 0x2a1cf71c2a498808, 0x150efb0e15244484, 0xa07fd070a122242, 0x503fe0305091121, 0x90a0c07fc0a09088, 0x4850e0bfe0504844, 0x24a870df70a82422, 0x925438ef38549211, 0x492a1cf71c2a4988, 0x24150efb0e152444, 0x120a07fd070a1222, 0x90503fe03050911, 0x8890a0c07fc0a090, 0x444850e0bfe05048, 0x2224a870df70a824, 0x11925438ef385492, 0x88492a1cf71c2a49, 0x4424150efb0e1524, 0x22120a07fd070a12, 0x11090503fe030509, 0x848890a0c07fc0a0, 0x42444850e0bfe050, 0x212224a870df70a8, 0x1011925438ef3854, 0x888492a1cf71c2a, 0x844424150efb0e15, 0x4222120a07fd070a, 0x2111090503fe0305, 0x82848890a0c07fc0, 0x4142444850e0bfe0, 0x20212224a870df70, 0x101011925438ef38, 0x80888492a1cf71c, 0x4844424150efb0e, 0x824222120a07fd07, 0x412111090503fe03, 0x8182848890a0c07f, 0x404142444850e0bf, 0x2020212224a870df, 0x10101011925438ef, 0x8080888492a1cf7, 0x404844424150efb, 0x2824222120a07fd, 0x81412111090503fe};
const u64 kingMoves[] = {0x302, 0x705, 0xe0a, 0x1c14, 0x3828, 0x7050, 0xe0a0, 0xc040, 0x30203, 0x70507, 0xe0a0e, 0x1c141c, 0x382838, 0x705070, 0xe0a0e0, 0xc040c0, 0x3020300, 0x7050700, 0xe0a0e00, 0x1c141c00, 0x38283800, 0x70507000, 0xe0a0e000, 0xc040c000, 0x302030000, 0x705070000, 0xe0a0e0000, 0x1c141c0000, 0x3828380000, 0x7050700000, 0xe0a0e00000, 0xc040c00000, 0x30203000000, 0x70507000000, 0xe0a0e000000, 0x1c141c000000, 0x382838000000, 0x705070000000, 0xe0a0e0000000, 0xc040c0000000, 0x3020300000000, 0x7050700000000, 0xe0a0e00000000, 0x1c141c00000000, 0x38283800000000, 0x70507000000000, 0xe0a0e000000000, 0xc040c000000000, 0x302030000000000, 0x705070000000000, 0xe0a0e0000000000, 0x1c141c0000000000, 0x3828380000000000, 0x7050700000000000, 0xe0a0e00000000000, 0xc040c00000000000, 0x203000000000000, 0x507000000000000, 0xa0e000000000000, 0x141c000000000000, 0x2838000000000000, 0x5070000000000000, 0xa0e0000000000000, 0x40c0000000000000};
// const u64 kingMoves[] = {0xc0c0000000000000, 0xe0e0000000000000, 0x7070000000000000, 0x3838000000000000, 0x1c1c000000000000, 0xe0e000000000000, 0x707000000000000, 0x303000000000000, 0xc0c0c00000000000, 0xe0e0e00000000000, 0x7070700000000000, 0x3838380000000000, 0x1c1c1c0000000000, 0xe0e0e0000000000, 0x707070000000000, 0x303030000000000, 0xc0c0c000000000, 0xe0e0e000000000, 0x70707000000000, 0x38383800000000, 0x1c1c1c00000000, 0xe0e0e00000000, 0x7070700000000, 0x3030300000000, 0xc0c0c0000000, 0xe0e0e0000000, 0x707070000000, 0x383838000000, 0x1c1c1c000000, 0xe0e0e000000, 0x70707000000, 0x30303000000, 0xc0c0c00000, 0xe0e0e00000, 0x7070700000, 0x3838380000, 0x1c1c1c0000, 0xe0e0e0000, 0x707070000, 0x303030000, 0xc0c0c000, 0xe0e0e000, 0x70707000, 0x38383800, 0x1c1c1c00, 0xe0e0e00, 0x7070700, 0x3030300, 0xc0c0c0, 0xe0e0e0, 0x707070, 0x383838, 0x1c1c1c, 0xe0e0e, 0x70707, 0x30303, 0xc0c0, 0xe0e0, 0x7070, 0x3838, 0x1c1c, 0xe0e, 0x707, 0x303};
const u64 sides = 0xff818181818181ff;

Magic rookMagics[64];
Magic bishopMagics[64];

std::map<int, u64> rookMoves[64];
std::map<int, u64> bishopMoves[64];

void prettyMove(char* str, int from, int to) {
    str[0] = 'a' + (from % 8);
    str[1] = '1' + (7 - int(from / 8));
    str[2] = 'a' + (to % 8);
    str[3] = '1' + (7 - int(to / 8));
    str[4] = '\0';
    return;
}

u64 shift_by(u64 x, int shiftAmount) {
    if (shiftAmount < 0) {
        return x >> abs(shiftAmount);
    }
    return x << shiftAmount;
}

u64 to_bitshift(int x) {

    if (abs(x) > 63) {
        return 0;
    }

    if (x < 0) {
        return u64(1) >> abs(x);
    }
    return u64(1) << x;
}

u64 to_bitshift(u64 data, int x) {

    if (abs(x) > 63) {
        return 0;
    }

    if (x < 0) {
        return data >> abs(x);
    }
    return data << x;
}

u64 same_row_bitshift(int x, int add) {

    if (int(x / 8) != int((x+add) / 8)) {
        return 0;
    }

    else {
        return to_bitshift(x + add);
    }
}

u64 cord_to_bit_shift(Vector2i vec2) {
    return u64(1) << (vec2.x + (vec2.y * 8));
}

u64 bits_between(int left, int right) {
    return ~(to_bitshift(left + 1) - 1) & (to_bitshift(right) - 1);
}

// https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating#By_180_degrees_-_Bit-Reversal
u64 rotate180 (u64 x) {
   const u64 h1 = u64 (0x5555555555555555);
   const u64 h2 = u64 (0x3333333333333333);
   const u64 h4 = u64 (0x0F0F0F0F0F0F0F0F);
   const u64 v1 = u64 (0x00FF00FF00FF00FF);
   const u64 v2 = u64 (0x0000FFFF0000FFFF);
   x = ((x >>  1) & h1) | ((x & h1) <<  1);
   x = ((x >>  2) & h2) | ((x & h2) <<  2);
   x = ((x >>  4) & h4) | ((x & h4) <<  4);
   x = ((x >>  8) & v1) | ((x & v1) <<  8);
   x = ((x >> 16) & v2) | ((x & v2) << 16);
   x = ( x >> 32)       | ( x       << 32);
   return x;
}

Board::Board() {
    ParseFenString("uuuuuuuu/uuuuuuuu/8/8/8/8/UUUUUUUU/UUUUUUUU w 0 0 - 0 1", rand() & 1);
    // getMagics(rookMoves, rookMagics, bishopMoves, bishopMagics);
    // printAll(rookMoves, rookMagics, bishopMoves, bishopMagics);
    // makeTables(rookMoves, rookMagics, bishopMoves, bishopMagics);
} 

u64 Board::get_bitboard(int index) const {
    return bitboards[index];
}

u64 Board::get_enemy_bitboard() const {
    return enemyBitboard;
}

u64 Board::get_all_pieces_bitboard() const {
    return allPiecesBoard;
}

bool Board::getGameOver() const {
    return gameOver;
}

void Board::_bind_methods() {
    ClassDB::bind_method(D_METHOD("helloWorld"), &Board::helloWorld);
    ClassDB::bind_method(D_METHOD("get_bitboard", "index"), &Board::get_bitboard);
    ClassDB::bind_method(D_METHOD("get_enemy_bitboard"), &Board::get_enemy_bitboard);
    ClassDB::bind_method(D_METHOD("get_all_pieces_bitboard"), &Board::get_all_pieces_bitboard);
    ClassDB::bind_method(D_METHOD("get_friendly_bitboard"), &Board::GetFriendlyBB);
    ClassDB::bind_method(D_METHOD("get_is_white"), &Board::get_is_white);
    ClassDB::bind_method(D_METHOD("GenerateLegalMovesOnePieceBitboard", "from"), &Board::GenerateLegalMovesOnePieceBitboard);
    ClassDB::bind_method(D_METHOD("GetIsWhiteTurn"), &Board::getIsWhiteTurn);
    ADD_SIGNAL(MethodInfo("board_changed"));
    ADD_SIGNAL(MethodInfo("wait_frame"));
}

bool Board::getIsWhiteTurn() const {
    return isWhiteTurn;
}

bool Board::get_is_white() const {
    return isWhite;
}

u64 Board::GetFriendlyBB() {

    //  white turn  player white    is enemy BB
    //      0           0                  0
    //      0           1                  1
    //      1           0                  1
    //      1           1                  0

    
    if (isWhiteTurn ^ isWhite) {
        // enemy BB
        return enemyBitboard;
    }
    
    else {
        // player BB
        return allPiecesBoard & ~enemyBitboard;
    }
}

// Used for seeing if a move is legal or not (does it place the player in check)?
bool Board::LastMoveLegal() {
    // called after a move

    u64 attkBB = GeneratePseudoMovesBitboard(true);
    return !(attkBB & (bitboards[KING] & (allPiecesBoard & ~GetFriendlyBB())));
}

// Used for seeing if a move puts a player in check
bool Board::CurrentPlayerinCheck() {
    //if (reAttackBitboard) {
        isWhiteTurn = !isWhiteTurn;
        attackBitboard = 0;
        attackBitboard = GeneratePseudoMovesBitboard(true);
        isWhiteTurn = !isWhiteTurn;
    //}

    // return attackBitboard & bitboards[KING];
    return attackBitboard & (bitboards[KING] & GetFriendlyBB());
    
}

void Board::pushAndMove(Move move) {

    int originalType = 0;

    if (move.isCapture()) {

        int to = move.get_to();

        for (int i = PAWN; i < SIZE; i++) {
            if (bitboards[i] & to_bitshift(to)) {
                originalType = i + 1;
            }
        }
    }

    SuperMove superMove(move, halfMoves, enPassant, originalType, moved & to_bitshift(move.get_from()));
    stack.push(superMove);
    Make(move);
}

Move Board::CreateMove(int from, int to, int promotion) const {

    u64 fromShift = to_bitshift(from);
    u64 toShift = to_bitshift(to);


    int flags = 0;

    // is a pawn?
    if (bitboards[PAWN] & to_bitshift(from)) {
        // promotion
        if (int(to / 8) == 0 | int(to / 8) == 7) {
            flags |= 0x8;
            flags |= promotion - KNIGHT;
        }

        // double pawn push
        if (abs(to - from) == 16) {
            flags |= 0x1;
        }

        // is enPassant capture
        if (to == enPassant) {
            flags |= 0x5;
        }
    }

    // is castle

    else if (bitboards[KING] & to_bitshift(from) && abs(to - from) == 2) {
        flags |= 0x2;

        if (to - from < 0) {
            flags |= 0x1;
        }
    }
    // is capture
    if (toShift & allPiecesBoard) {
        flags |= 0x4;
        if (to_bitshift(to) & moved) {
            flags |= 0x2;
        }
    }

    return Move(from, to, flags);
}

bool Board::isPlayer() const {
    return !(isWhiteTurn ^ isWhite);
}

int Board::isDraw() {
    // draw by repetition
    if (false) {
        return DRAW_BY_REPITION;
    }
    // draw by fifty-move rule
    if (halfMoves > 100) {
        return DRAW_BY_FIFTY_MOVE_RULE;
    }
    // insufficient material (only one piece/king on both sides)
    u64 notEnemyBB = allPiecesBoard & ~enemyBitboard;

    if (((notEnemyBB - 1) & notEnemyBB) == 0 && ((enemyBitboard - 1) & enemyBitboard) == 0) {
        return DRAW_BY_INSUFFICIENT_MATERIAL;
    }

    return NO_DRAW;

}

int Board::isGameOver() {

    int result = isDraw();
    if (result != NO_DRAW) {
        return false;
    }

    // this is always called after a move
    // and because of that only the player whose turn it is has the possibility to be in checkmate

    // is current move player in checkmate?
    Vector<Move> legalMoves = GenerateLegalMoves();
    if (legalMoves.is_empty()) {
        return true;
    }

    // is other player in checkmate?
    // isWhiteTurn = !isWhiteTurn;
    // legalMoves = GenerateLegalMoves();
    // isWhiteTurn = !isWhiteTurn;
    // if (legalMoves.is_empty()) {
    //     return true;
    // }

    return false;
    
}

void Board::Make(Move move) {

    halfMoves++;

    u64 from = to_bitshift(move.get_from());
    u64 to   = to_bitshift(move.get_to());

    if (bitboards[PAWN] & from) {
        halfMoves = 0;
    }

    if (move.isDoublePawnPush()) {
        // this feels wrong but it works, like it shouldn't have to be minus
        enPassant = move.get_from() + (isPlayer() ? NORTH: SOUTH);
    }
    else {
        enPassant = -1;
    }

    SimpleMake(move); 

    isInCheck = CurrentPlayerinCheck();
    gameOver = isGameOver();
}

void Board::UnMake() {
    
    SuperMove superMove = stack.pop();

    hasMoved = superMove.get_hadMoved();
    halfMoves = superMove.get_halfMoves();
    enPassant = superMove.get_enPassant();
    originalType = superMove.get_captured() - 1;

    u64 from = to_bitshift(superMove.get_from());
    u64 to = to_bitshift(superMove.get_to());
    bool isEnemy = from & enemyBitboard;

    SimpleUnMake(superMove);
    CurrentPlayerinCheck();
    gameOver = false;
}

void Board::SimpleMake(Move move) {

    isWhiteTurn = !isWhiteTurn;
    if (isWhiteTurn) {
        fullMoves++;
    }
    
    u64 from = to_bitshift(move.get_from());
    u64 to = to_bitshift(move.get_to());
    bool isEnemy = from & enemyBitboard;

    originalType = -1;

    if (move.get_is_enpassant()) {
        u64 attackedPawn;
        if (isEnemy) {
            attackedPawn = to_bitshift(to, NORTH);
        }
        else {
            attackedPawn = to_bitshift(to, SOUTH);
            enemyBitboard ^= attackedPawn;
        }

        bitboards[PAWN] ^= attackedPawn;
        allPiecesBoard ^= attackedPawn;
        moved ^= attackedPawn; // because the pawn should have already moved
    }
    else if (move.isCapture()) {
        for (int i = PAWN; i < SIZE; i++) {
            if (bitboards[i] & to) {
                originalType = i;
                break;
            }
        }

        bitboards[originalType] ^= to;
        allPiecesBoard ^= to;

        if (!isEnemy) {
            enemyBitboard ^= to;
        }
    }

    if (move.isCastle()) {
        u64 rookPos; // Original Pos
        u64 newRookPos; // New Pos
        if (move.isQueenSideCastle()) {
            rookPos = to_bitshift(to, WEST * 2); // to + (WEST * 2) made an overflow bug
            newRookPos = to_bitshift(to, EAST);
        }
        else {
            rookPos = to_bitshift(to, EAST);
            newRookPos = to_bitshift(to, WEST);
        }

        bitboards[ROOK] ^= rookPos;
        bitboards[ROOK] |= newRookPos;

        allPiecesBoard ^= rookPos;
        allPiecesBoard |= newRookPos;

        // moved ^= rookPos;
        // wouldn't have moved, since it's a castle
        moved |= newRookPos;

        if (isEnemy) {
            enemyBitboard ^= rookPos;
            enemyBitboard |= newRookPos;
        }
    }

    if (move.isPromotion()) {
        // feels a bit slow because we set it just to unset it
        // system("echo 'promoted' > ~/Desktop/godot-debug/promotion.txt");
        bitboards[move.getPromotion()] |= from;
        bitboards[PAWN] ^= from;
    }

    hasMoved = moved & from;
    moved &= ~from;
    moved |= to;

    // from -> to
    for (int pieceType = PAWN; pieceType < SIZE; pieceType++) {
        if (bitboards[pieceType] & from) {
            bitboards[pieceType] ^= from;
            bitboards[pieceType] |= to;

            allPiecesBoard ^= from;
            allPiecesBoard |= to;

            if (isEnemy) {
                enemyBitboard ^= from;
                enemyBitboard |= to;
            }

            return;
        }
    }

}

void Board::SimpleUnMake(Move move) {

    isWhiteTurn = !isWhiteTurn;
    if (!isWhiteTurn) {
        fullMoves--;
    }
    
    u64 from = to_bitshift(move.get_from());
    u64 to = to_bitshift(move.get_to());
    bool isEnemy = to & enemyBitboard;

    if (hasMoved) {
        moved |= from;
    }

    moved ^= to;

    // bitboards[QUEEN] |= from;

    // to -> from
    for (int pieceType = PAWN; pieceType < SIZE; pieceType++) {
        if (bitboards[pieceType] & to) {
            bitboards[pieceType] ^= to;
            bitboards[pieceType] |= from;

            allPiecesBoard ^= to;
            allPiecesBoard |= from;

            if (isEnemy) {
                enemyBitboard ^= to;
                enemyBitboard |= from;
            }
            break;
        }
    }


    if (move.get_is_enpassant()) {
        u64 attackedPawn;
        if (isEnemy) {
            attackedPawn = to_bitshift(to, NORTH);
        }
        else {
            attackedPawn = to_bitshift(to, SOUTH);
            enemyBitboard |= attackedPawn;
        }

        bitboards[PAWN] |= attackedPawn;
        allPiecesBoard |= attackedPawn;

        moved |= attackedPawn;
    }

    else if (move.isCapture()) {
        bitboards[originalType] |= to;
        allPiecesBoard |= to;

        if (!isEnemy) {
            enemyBitboard |= to;
        }

        if (move.hadCaptureMoved()) {
            moved |= to;
        }
    }

    if (move.isCastle()) {
        u64 rookPos; // the one it is currently
        u64 lastRookPos; // The one it was, the one we're reverting to
        if (move.isQueenSideCastle()) {
            lastRookPos = to_bitshift(to, WEST * 2); // to + (WEST * 2) made an overflow bug
            rookPos = to_bitshift(to, EAST);
        }
        else {
            lastRookPos = to_bitshift(to, EAST);
            rookPos = to_bitshift(to, WEST);
        }

        bitboards[ROOK] ^= rookPos;
        bitboards[ROOK] |= lastRookPos;

        allPiecesBoard ^= rookPos;
        allPiecesBoard |= lastRookPos;

        moved ^= rookPos;
        // moved |= lastRookPos; 
        // wouldn't have moved, since it's a castle

        if (isEnemy) {
            enemyBitboard ^= rookPos;
            enemyBitboard |= lastRookPos;
        }
    }

    if (move.isPromotion()) {
        bitboards[move.getPromotion()] ^= from;
        bitboards[PAWN] |= from;
    }
}

u64 Board::getEnPassant() {
    return to_bitshift(enPassant);
}

u64 Board::PseudoMoves(int index, int type, bool isForAttkBB) {

    u64 friendlyBB = GetFriendlyBB();
    u64 moveTemp;
    u64 move = 0;
    int hashIndex;

    switch (type) {
        case PAWN:

            if (!isForAttkBB) {

                if (isWhiteTurn) {
                    // move = pawnMovesWhite[index];
                    move = rotate180(pawnMovesBlack[63 - index]);
                }
                else {
                    // move = pawnMovesBlack[index];
                    move = pawnMovesBlack[index];
                }

                // if we have moved or there's a piece blocking us (on either square) then erase double push as a possible move
                // if (move & allPiecesBoard && moved & to_bitshift(index)) {
                //     move |= move &= ~to_bitshift(index + (isPlayer() ? (NORTH * 2): (SOUTH * 2)));
                // }
                if (move & allPiecesBoard || moved & to_bitshift(index)) {
                    move &= ~to_bitshift(index + (isPlayer() ? (NORTH * 2): (SOUTH * 2)));
                }

                move &= ~allPiecesBoard;
            }

            if (isWhiteTurn) {
                // moveTemp = pawnAttacksWhite[index];
                moveTemp = same_row_bitshift(index + NORTH, EAST) | same_row_bitshift(index + NORTH, WEST);
            }
            else {
                // moveTemp = pawnMovesBlack[index];
                moveTemp = same_row_bitshift(index + SOUTH, EAST) | same_row_bitshift(index + SOUTH, WEST);
            }

            if (isForAttkBB) {
                return moveTemp;
            }
            else {
                move |= moveTemp & (allPiecesBoard | to_bitshift(enPassant));
            }

            return move & ~friendlyBB;
        case KNIGHT:
            return knightMoves[index] & ~friendlyBB;
        case BISHOP:
            // hashIndex = magicIndex(bishopMagics[index], allPiecesBoard);
            // return bishopMoves[index][hashIndex] & ~friendlyBB;
            return getBishopMoves(index, allPiecesBoard) & ~friendlyBB;
            // return bishopMoves[index] & ~friendlyBB;
            // return bishopMovesBlocked(index, allPiecesBoard) & ~friendlyBB;
        case ROOK:
            // hashIndex = magicIndex(rookMagics[index], allPiecesBoard);
            // return rookMoves[index][hashIndex] & ~friendlyBB;
            return getRookMoves(index, allPiecesBoard) & ~friendlyBB;
            // return rookMoves[index] & ~friendlyBB;
            // return rookMovesBlocked(index, allPiecesBoard) & ~friendlyBB;
        case QUEEN:
            // hashIndex = magicIndex(rookMagics[index], allPiecesBoard);
            // move |= rookMoves[index][hashIndex];
            
            // hashIndex = magicIndex(bishopMagics[index], allPiecesBoard);
            // move |= bishopMoves[index][hashIndex];
            return (getBishopMoves(index, allPiecesBoard) | getRookMoves(index, allPiecesBoard)) & ~friendlyBB;

            // return move & ~friendlyBB;
            // return queenMoves[index] & ~friendlyBB;
            // return (bishopMovesBlocked(index, allPiecesBoard) | rookMovesBlocked(index, allPiecesBoard)) & ~friendlyBB;
        case KING:

            move = kingMoves[index];

            //   0 1 2 3 4 5 6 7
            // 0 R N B Q K B N R
            // 1 P P P P P P P P
            // 2 . . . . . . . .
            // 3 . . . . . . . .
            // 4 . . . . . . . .
            // 5 . . . . . . . .
            // 6 P P P P P P P P
            // 7 R N B Q K B N R

            // ~((1 << (L + 1)) - 1) & ((1 << R) - 1)

            if (isForAttkBB) {
                return move & ~friendlyBB;
            }

            if (~moved & to_bitshift(index) && !isInCheck) {
                
                // KING SIDE CASTLE
                if (same_row_bitshift(index, EAST * 3) & bitboards[ROOK] & ~moved && !(bits_between(index, index + (EAST * 3)) & allPiecesBoard)) {
                    move |= to_bitshift(index + (EAST * 2));
                }
                
                // QUEEN SIDE CASTLE
                if (same_row_bitshift(index, WEST * 4) & bitboards[ROOK] & ~moved && !(bits_between(index + (WEST * 4), index) & allPiecesBoard)) {
                    move |= to_bitshift(index + (WEST * 2));
                }
            }

            return move & ~friendlyBB & ~attackBitboard;
    }

    return 0;
}

u64 Board::PseudoMoves(int index) {
    u64 shiftIndex = to_bitshift(index);
    for (int i = 0; i < SIZE; i++) {
        if (shiftIndex & bitboards[i]) {
            return PseudoMoves(index, i);
        }
    }

    return 0;
}

u64 Board::GeneratePseudoMovesBitboard(bool isForAttkBB) {

    u64 result = 0;
    u64 friendlyBB = GetFriendlyBB();

    for (int pieceType = PAWN; pieceType < SIZE; pieceType++) {

        u64 bitboardCopy = get_bitboard(pieceType) & friendlyBB;

        int index = 0;
        while (bitboardCopy > 0) {

            if (bitboardCopy & 1) {
                result |= PseudoMoves(index, pieceType, isForAttkBB);
            }

            index++;
            bitboardCopy >>= 1;
        }
    }

    return result;
}

Vector<Move> Board::GeneratePseudoMoves() {

    Vector<Move> moveVector = Vector<Move>();
    u64 friendlyBB = GetFriendlyBB();

    for (int pieceType = PAWN; pieceType < SIZE; pieceType++) {
        u64 bitboardCopy = get_bitboard(pieceType) & friendlyBB;
        int index = 0;
        while (bitboardCopy > 0) {

            if (bitboardCopy & 1) {
                u64 moves = PseudoMoves(index, pieceType);

                int mv_index = 0;
                while (moves > 0) {

                    if (moves & 1) {

                        Move move = CreateMove(index, mv_index);

                        if (move.isPromotion()) {
                            for (int i = KNIGHT; i < QUEEN; i++) {
                                moveVector.append(CreateMove(index, mv_index, i));
                            }
                        }

                        moveVector.append(CreateMove(index, mv_index));
                    }

                    mv_index++;
                    moves >>= 1;
                }
            }

            index++;
            bitboardCopy >>= 1;
        }
    }
    return moveVector;
}

Vector<Move> Board::GenerateLegalMoves() {
    Vector<Move> PseudoMoves = GeneratePseudoMoves();

    int index = 0;
    while (index < PseudoMoves.size()) {

        SimpleMake(PseudoMoves[index]);

        bool lastMoveLegal = LastMoveLegal();
        
        SimpleUnMake(PseudoMoves[index]);

        if (!lastMoveLegal) {
            PseudoMoves.remove_at(index);
            continue;
        }

        index++;
    }

    return PseudoMoves; // LegalMoves
}

// GODOT-ONLY FUNCTION! WILL BREAK STUFF IN C++!
u64 Board::GenerateLegalMovesOnePieceBitboard(int from) {

    u64 moves = PseudoMoves(from);

    u64 result = 0;

    int to = 0;
    while (moves) {
        if (moves & 1) {

            Move move = CreateMove(from, to);

            SimpleMake(move);

            if (LastMoveLegal()) {
                result |= to_bitshift(to);
            }
            // result |= to_bitshift(to);

            SimpleUnMake(move);
            
        }

        to++;
        moves >>= 1;
    }

    return result;
}

// Testing:

// https://www.chessprogramming.org/Perft
u64 Board::Perft(int depth, bool root) {

    Vector<Move> moveList;
    char str[200];
    u64 nodes = 0;
    u64 cnt = 0;

    if (depth == 0) {

        if (gameOver) {
            checkMates++;
        }
        if (isInCheck) {
            checks++;
        }

        if (stack.peek().isCapture()) {
            captures++;
        }

        if (stack.peek().get_is_enpassant()) {
            enPassants++;
        }

        if (stack.peek().isPromotion()) {
            promotions++;
        }

        if (stack.peek().isCastle()) {
            castles++;
        }

        return u64(1);
    }

    moveList = GeneratePseudoMoves();

    for (int i = 0; i < moveList.size(); i++) {

        // if (moveList[i].isCapture()) {
        //     printState("perft/curState");
        // }

        pushAndMove(moveList[i]);

        // emit_signal("board_changed");

        if (LastMoveLegal()) {

            // emit_signal("board_changed");
            // for (int i = 0; i < 60; i++) {
            //     emit_signal("wait_frame");
            // }
            cnt = Perft(depth - 1, false);
            nodes += cnt;

            if (root) {
                char mv[5];
                prettyMove(mv, moveList[i].get_from(), moveList[i].get_to());
                snprintf(str, 200, "echo '%s: %llu' >> ~/Desktop/godot-debug/perft-mid.txt", mv, cnt);
                system(str);
            }
        }

        UnMake();
        // emit_signal("board_changed");
        // for (int i = 0; i < 60; i++) {
        //     emit_signal("wait_frame");
        // }
        // emit_signal("board_changed");


    }

    return nodes;
}

void Board::doPerft(int maxDepth) {

    system("rm ~/Desktop/godot-debug/perft.txt");

    char str[200];
    // snprintf(str, 200, "echo 'Depth: %d: %llu, Captures: %llu' >> ~/Desktop/godot-debug/perft.txt", maxDepth, Perft(maxDepth), captures);
    // system(str);

    for (int depth = maxDepth; depth <= maxDepth; depth++) {
        snprintf(str, 200, "echo 'Depth %d: %llu, Captures: %llu, En-Passants: %llu, Castles: %llu, Promotions: %llu, Checks: %llu, Checkmates: %llu' >> ~/Desktop/godot-debug/perft.txt", depth, Perft(depth), captures, enPassants, castles, promotions, checks, checkMates);
        system(str);
        captures = 0;
        checks = 0;
        checkMates = 0;
    }
}

// Zobrist Hashing
u64 Board::pseudoRandom() {
    next = (next * 1209318102) + 21381712389;
    return next;
}

u64 Board::getZobristNumberFromPiece(int pieceType, int index, bool isBlack) {
    switch (pieceType) {
        case PAWN:
            return pseudoRandomNumbers[PIECE_IS_PAWN + index + (isBlack ? PIECE_IS_BLACK: 0)];
        case KNIGHT:
            return pseudoRandomNumbers[PIECE_IS_KNIGHT + index + (isBlack ? PIECE_IS_BLACK: 0)];
        case BISHOP:
            return pseudoRandomNumbers[PIECE_IS_BISHOP + index + (isBlack ? PIECE_IS_BLACK: 0)];
        case ROOK:
            return pseudoRandomNumbers[PIECE_IS_ROOK + index + (isBlack ? PIECE_IS_BLACK: 0)];
        case QUEEN:
            return pseudoRandomNumbers[PIECE_IS_QUEEN + index + (isBlack ? PIECE_IS_BLACK: 0)];
        case KING:
            return pseudoRandomNumbers[PIECE_IS_KING + index + (isBlack ? PIECE_IS_BLACK: 0)];
        default:
            return 0;
    }
}

u64 Board::zobristHash() {
    u64 result = 0;
    for (int i = PAWN; i < SIZE; i++) {
        u64 boardCopy = get_bitboard(i);
        int index = 0;

        while (boardCopy > 0) {
            if (boardCopy & 1) {
                result ^= getZobristNumberFromPiece(i, index, (1 << index) & enemyBitboard);
            }

            index++;
            boardCopy >>= 1;
        }
    }

    if (!isWhiteTurn) {
        result ^= pseudoRandomNumbers[BLACK_TO_MOVE];
    }

    for (int player1 = 0; player1 < 2; player1++) {
        for (int i = 0; i < 16; i++) {
            u64 sqr = 1 << i;
            if (player1) {
                sqr <<= (6*8);
            }

            if (sqr & ~moved) {
                result ^= pseudoRandomNumbers[CASTLING_RIGHTS + (i * (player1 ? 2: 1))];
            }
        }
    }

    if (enPassant != -1) {
        result ^= pseudoRandomNumbers[EN_PASSANT + (enPassant % 8)];
    }

    return result;
}

Board::~Board() {
    // delete bitboards;
}
