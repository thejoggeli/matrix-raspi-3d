#pragma once

#include <vector>
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/Timer.h"
#include <memory>

using namespace Ledlib;

class Piece;

struct GridSlot {
    bool free = true;
    bool collapsing = false;
	ColorRgb color = ColorRgb::WHITE;
};

class Grid {
public:
    float tickDuration = 0.5f;
	int collapseLength = 0;
	std::shared_ptr<Piece> currentPiece = nullptr;
	std::shared_ptr<Piece> nextPiece = nullptr;
	std::vector<std::vector<GridSlot>> slots;
    Timer tickTimer;
    Timer collapseTimer;
	Timer fastMoveTimer;
	Timer sideMoveTimer;
    int width;
	int height;
	Grid();
    void Start();
    void Update();
    void Render();
    void Setup(unsigned int w, unsigned int h);
	bool IsPieceBlocked(Piece& piece, int xdir, int ydir);
	bool SettlePiece(Piece& piece);
    void NextPiece();
    bool FindCollapseRows();
    void CollapseRows();
};

