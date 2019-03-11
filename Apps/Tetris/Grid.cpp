#include "Grid.h"
#include "Piece.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Time.h"
#include "Ledlib/Math/Numbers.h"

Grid::Grid(){}

void Grid::Setup(unsigned int w, unsigned int h){
	currentPiece = nullptr;
	nextPiece = nullptr;
	width = w;
	height = h;
	nextPiece = std::make_shared<Piece>();
	nextPiece->SetPieceType(Piece::GetRandomPieceType());
	NextPiece();
	slots.clear();
	for(unsigned int x = 0; x < w; x++){
		slots.push_back(std::vector<GridSlot>());
		for(unsigned int y = 0; y < h; y++){
			slots[x].push_back(GridSlot());
		}
	}
	collapseLength = 0;
}
void Grid::Start(){
	tickTimer.Start(tickDuration);
}
void Grid::Update(){
	if(currentPiece){
		if(ClientManager::OnKeyDown(KeyCode::A) || ClientManager::OnKeyDown(KeyCode::Up)){
			currentPiece->Rotate(true);
			if(IsPieceBlocked(*currentPiece, 0, 0)){
				currentPiece->Rotate(false);
			}
		}
		if(ClientManager::OnKeyDown(KeyCode::B)){
			currentPiece->Rotate(false);
			if(IsPieceBlocked(*currentPiece, 0, 0)){
				currentPiece->Rotate(true);
			}
		}
		bool moveLeftFlag = false;;
		bool moveRightFlag = false;;
		if(ClientManager::OnKeyDown(KeyCode::Left)){
			sideMoveTimer.Reset();
			moveLeftFlag = true;
		}
		if(ClientManager::OnKeyDown(KeyCode::Right)){
			sideMoveTimer.Reset();
			moveRightFlag = true;
		}
		if(moveLeftFlag || ClientManager::IsKeyDown(KeyCode::Left)){
			if(sideMoveTimer.IsFinished() && !IsPieceBlocked(*currentPiece, -1, 0)){
				currentPiece->x -= 1;
				sideMoveTimer.Start(tickDuration/4.0f);
			}
		}
		if(moveRightFlag || ClientManager::IsKeyDown(KeyCode::Right)){
			if(sideMoveTimer.IsFinished() && !IsPieceBlocked(*currentPiece, 1, 0)){
				currentPiece->x += 1;
				sideMoveTimer.Start(tickDuration/4.0f);
			}
		}
		if(ClientManager::OnKeyDown(KeyCode::Down)){
			fastMoveTimer.Reset();
		}
		if(ClientManager::IsKeyDown(KeyCode::Down)){
			if(fastMoveTimer.IsFinished() && !IsPieceBlocked(*currentPiece, 0, 1)){
				currentPiece->y += 1;
				fastMoveTimer.Start(tickDuration/32.0f);
				tickTimer.Start(tickDuration/2.0f);
			}
		}
	}
	if(collapseLength > 0){
		if(collapseTimer.IsFinished()){
			CollapseRows();
			if(!FindCollapseRows()){
				tickTimer.Start(tickDuration/2);
			}
		}
	} else if(tickTimer.IsFinished()){
		if(currentPiece){
			if(IsPieceBlocked(*currentPiece, 0, 1)){
				// piece cannot fall
				if(!SettlePiece(*currentPiece)){
					// game over
				}
				tickTimer.Reset();
			} else {
				// piece can fall
				currentPiece->y += 1;
				tickTimer.Start(tickDuration);
			}
		} else {
			NextPiece();
			tickTimer.Start(tickDuration);
		}
	}
}
void Grid::Render(){
	Gfx::SetDrawColor(ColorRgb::BLACK);
	for(unsigned int y = 0; y < height; y++){
		int yy = y;
		if(slots[0][y].collapsing){
			ColorHsl hsl = ColorHsl(0, 1, 0.75f);
			hsl.h = hsl.WrapHue(Time::sinceStart*4.0f);
			ColorRgb color = hsl.GetRgb();
			color.a = Numbers::Mod(collapseTimer.GetAbsolutePassedTime(), 0.2f) < 0.1f ? 255 : 0;
			Gfx::SetDrawColor(color);
			for(unsigned int x = 0; x < width; x++){
				int xx = x;
				Gfx::FillRect(xx+0.5f, yy+0.5f, 1, 1);
			}
		} else {
			for(unsigned int x = 0; x < width; x++){
				if(slots[x][y].free) continue;
				int xx = x;
				Gfx::SetDrawColor(slots[x][y].color);
				Gfx::FillRect(xx+0.5f, yy+0.5f, 1, 1);
			}
		}
	}
	if(currentPiece){
		bool blocked = IsPieceBlocked(*currentPiece, 0, 1);
		if(blocked){
			Gfx::SetDrawColor(ColorRgb::WHITE);
		} else {
			Gfx::SetDrawColor(currentPiece->color);
		}
		for(unsigned int x = 0; x < currentPiece->size; x++){
			for(unsigned int y = 0; y < currentPiece->size; y++){
				if(currentPiece->slots[x][y] == 0) continue;
				int xx = (currentPiece->x+x);
				int yy = (currentPiece->y+y);
				Gfx::FillRect(xx+0.5f, yy+0.5f, 1, 1);
			}
		}
	}
}
bool Grid::IsPieceBlocked(Piece& piece, int xdir, int ydir){
	for(unsigned int x = 0; x < piece.size; x++){
		for(unsigned int y = 0; y < piece.size; y++){
			if(piece.slots[x][y] == 0) continue;
			int xx = piece.x + x + xdir;
			int yy = piece.y + y + ydir;
			if(yy < 0) continue;
			if(xx < 0 || xx >= width || yy >= height) return true;
			if(!slots[xx][yy].free){
				return true;
			}
		}
	}
	return false;
}
bool Grid::SettlePiece(Piece& piece){
	for(unsigned int x = 0; x < piece.size; x++){
		for(unsigned int y = 0; y < piece.size; y++){
			if(piece.slots[x][y] == 0) continue;
			int xx = piece.x + x;
			int yy = piece.y + y;
			if(yy < 0) return false;
			if(xx < 0 || xx >= width || y >= height) continue;
			slots[xx][yy].free = false;
			slots[xx][yy].color = piece.color;
		}
	}
	currentPiece = nullptr;
	FindCollapseRows();
}
void Grid::NextPiece(){
	currentPiece = nextPiece;
	currentPiece->x = width/2 - currentPiece->size/2;
	currentPiece->y = -currentPiece->bounds.bottom-1;
	nextPiece = std::make_shared<Piece>();
	nextPiece->SetPieceType(Piece::GetRandomPieceType());
}
bool Grid::FindCollapseRows(){
	collapseLength = 0;
	for(int y = 0; y < height; y++){
		bool full = true;
		for(int x = 0; x < width; x++){
			if(slots[x][y].free){
				full = false;
				break;
			}
		}
		if(full){
			collapseLength++;
			for(int x = 0; x < width; x++){
				slots[x][y].collapsing = true;
			}
		}
	}
	collapseTimer.Start(tickDuration*2);
	return collapseLength > 0;
}
void Grid::CollapseRows(){
	collapseLength = 0;
	for(int y = height-1; y >= 0; y--){
		bool collapse = slots[0][y].collapsing;
		if(collapse){
			for(int j = y; j >= 1; j--){
				for(int x = 0; x < width; x++){
					slots[x][j] = slots[x][j-1];
				}
			}
			for(int x = 0; x < width; x++){
				slots[x][0] = GridSlot();
			}
			y++;
		}
	}
}
