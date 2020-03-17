#include "Piece.h"
#include "Ledlib/Math/Numbers.h"

Piece::Piece(){}

void Piece::SetPieceType(PieceType type){
	switch(type){
	case PieceType::I:
		SetSize(4);
		slots[2][0] = 1;
		slots[2][1] = 1;
		slots[2][2] = 1;
		slots[2][3] = 1;
		color = ColorRgb::YELLOW;
		break;
	case PieceType::J:
		SetSize(3);
		slots[1][0] = 1;
		slots[1][1] = 1;
		slots[1][2] = 1;
		slots[0][2] = 1;
		color = ColorRgb::BLUE;
		break;
	case PieceType::L:
		SetSize(3);
		slots[1][0] = 1;
		slots[1][1] = 1;
		slots[1][2] = 1;
		slots[2][2] = 1;
		color = ColorRgb::MAGENTA;
		break;
	case PieceType::O:
		SetSize(2);
		slots[0][0] = 1;
		slots[0][1] = 1;
		slots[1][0] = 1;
		slots[1][1] = 1;
		color = ColorRgb::GREEN;
		break;
	case PieceType::T:
		SetSize(3);
		slots[1][0] = 1;
		slots[0][1] = 1;
		slots[1][1] = 1;
		slots[2][1] = 1;
		color = ColorRgb::RED;
		break;
	case PieceType::Z:
		SetSize(3);
		slots[0][0] = 1;
		slots[1][0] = 1;
		slots[1][1] = 1;
		slots[2][1] = 1;
		color = ColorRgb::CYAN;
		break;
	case PieceType::S:
		SetSize(3);
		slots[1][0] = 1;
		slots[2][0] = 1;
		slots[0][1] = 1;
		slots[1][1] = 1;
		color = ColorRgb(0.5f, 0, 1.0f);
		break;
	}
	RecalcBounds();
}
PieceType Piece::GetRandomPieceType(){
	int rng = Numbers::Random(0, 7);
	switch(rng){
	case 0: return PieceType::I;
	case 1: return PieceType::J;
	case 2: return PieceType::L;
	case 3: return PieceType::S;
	case 4: return PieceType::Z;
	case 5: return PieceType::O;
	case 6: return PieceType::T;
	default: return PieceType::I;
	}
}
void Piece::SetSize(unsigned int s){
	size = s;
	slots.clear();
	for(unsigned int x = 0; x < s; x++){
		slots.push_back(std::vector<int>());
		for(unsigned int y = 0; y < s; y++){
			slots[x].push_back(0);
		}
	}
}
void Piece::RecalcBounds(){
	bounds.right = bounds.bottom = 0;
	bounds.top = bounds.left = size;
	for(unsigned int x = 0; x < size; x++){
		for(unsigned int y = 0; y < size; y++){
			if(slots[x][y] == 0) continue;
			if(x < bounds.left) bounds.left = x;
			if(y < bounds.top) bounds.top = y;
			if(x > bounds.right) bounds.right = x;
			if(y > bounds.bottom) bounds.bottom = y;
		}
	}
}
void Piece::Rotate(bool ccw){
	std::vector<std::vector<int>> rotationSlots = slots;
	for(int x = 0; x < size; x++){
		for(int y = 0; y < size; y++){
			rotationSlots[x][y] = 0;
		}
	}
	float off = static_cast<float>(size)/2.0f-0.5f;
	for(int x = 0; x < size; x++){
		for(int y = 0; y < size; y++){
			if(slots[x][y] == 0) continue;
			float tx = static_cast<float>(x)-off;
			float ty = static_cast<float>(y)-off;
			float fx = ccw ? +ty : -ty;
			float fy = ccw ? -tx : +tx;
			int ix = static_cast<int>(fx+off+0.1f);
			int iy = static_cast<int>(fy+off+0.1f);
			rotationSlots[ix][iy] = 1;
		}
	}
	slots = rotationSlots;
	RecalcBounds();
}
