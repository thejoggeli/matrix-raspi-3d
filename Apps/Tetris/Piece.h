#pragma once

#include <vector>
#include "Ledlib/Util/ColorRgb.h"

using namespace Ledlib;

enum PieceType {
    I, O, T, L, J, Z, S
};

class Piece {
public:
	ColorRgb color;
    std::vector<std::vector<int>> slots;
    int x;
    int y;
	struct Bounds {
        int top; int bottom; int right; int left;
	} bounds;
    unsigned int size;
	Piece();
	void SetPieceType(PieceType type);
	static PieceType GetRandomPieceType();
    void Rotate(bool ccw = false);
    void SetSize(unsigned int s);
	void RecalcBounds();
};

