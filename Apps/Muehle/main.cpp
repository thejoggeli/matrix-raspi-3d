#include <iostream>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <list>

#include "Ledlib/Config.h"
#include "Ledlib/Log.h"
#include "Ledlib/Time.h"
#include "Ledlib/Util/Lodepng.h"
#include "Ledlib/Util/Timer.h"
#include "Ledlib/Util/FileHelper.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Events/Event.h"
#include "Ledlib/Events/EventManager.h"
#include "Ledlib/Remote/ServerMessage.h"
#include "Ledlib/Remote/ServerManager.h"
#include "Ledlib2d/App.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Resources/Font.h"

#include "muehlecolor.h"
#include "muehleline.h"
#include "muehlefield.h"

using namespace std;
using namespace Ledlib;

static MuehleField grid[7][7];
static std::vector<MuehleLine> lines;

class MuehleApp : public App {
	void OnSetup() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
    void OnExit() override;
    static void SetMuehleColor(MuehleField& field, MuehleColor color);
    // static void WriteChatMessage(int clientId, const string& str);
};
void MuehleApp::OnSetup() {
    // EventManager::SubscribeMessage("chat", this, &MuehleApp::OnMessageChat);
    ResourceManager::LoadFontBdf("font-1", "4x6.bdf");
    for(int x = 0; x < 7; x++){
        for(int y = 0; y < 7; y++){
            grid[x][y].x = x;
            grid[x][y].y = y;
        }
    }
    auto add_line = [&](MuehleField& f1, MuehleField& f2, MuehleField& f3){
        MuehleLine line;
        line.fields[0] = &f1;
        line.fields[1] = &f2;
        line.fields[2] = &f3;
        f1.exists = f2.exists = f3.exists = true;
        lines.push_back(line);
        MuehleLine* linePtr = &lines[lines.size()-1];
        line.fields[0]->AddLine(linePtr);
        line.fields[1]->AddLine(linePtr);
        line.fields[2]->AddLine(linePtr);
    };
    // outer square
    add_line(grid[0][0], grid[0][3], grid[0][6]);
    add_line(grid[6][0], grid[6][3], grid[6][6]);
    add_line(grid[0][0], grid[3][0], grid[6][0]);
    add_line(grid[0][6], grid[3][6], grid[6][6]);
    // middle square
    add_line(grid[1][1], grid[3][1], grid[5][1]);
    add_line(grid[1][5], grid[3][5], grid[5][5]);
    add_line(grid[5][1], grid[5][3], grid[5][5]);
    add_line(grid[1][1], grid[1][3], grid[1][5]);
    // inner square
    add_line(grid[2][2], grid[3][2], grid[4][2]);
    add_line(grid[2][4], grid[3][4], grid[4][4]);
    add_line(grid[2][2], grid[2][3], grid[2][4]);
    add_line(grid[4][2], grid[4][3], grid[4][4]);
    // bridges
    add_line(grid[3][0], grid[3][1], grid[3][2]);
    add_line(grid[3][6], grid[3][5], grid[3][4]);
    add_line(grid[0][3], grid[1][3], grid[2][3]);
    add_line(grid[6][3], grid[5][3], grid[4][3]);
    // test stones
    SetMuehleColor(grid[0][0], MuehleColor::Black);
    SetMuehleColor(grid[1][1], MuehleColor::White);
    SetMuehleColor(grid[1][3], MuehleColor::Black);
}
void MuehleApp::OnStart(){

}
void MuehleApp::OnUpdate() {

}
void MuehleApp::OnRender() {

    Gfx::Save();
    Gfx::SetFont(ResourceManager::GetFont("font-1"));
    Gfx::SetTextPosition(TextAlign::Left, TextBaseline::Bottom);
    Gfx::antiKerning = true;
    Gfx::textCharacterSpacing = 0;

    Gfx::SetDrawColor(ColorRgb::WHITE);
    for(auto& line: lines){
        float x0 = line.fields[0]->x*4.0f - 7.0f*4.0f/2.0f + 1;
        float y0 = line.fields[0]->y*4.0f - 7.0f*4.0f/2.0f + 1;
        float x1 = line.fields[2]->x*4.0f - 7.0f*4.0f/2.0f + 1;
        float y1 = line.fields[2]->y*4.0f - 7.0f*4.0f/2.0f + 1;
        Gfx::StrokeLine(x0, y0, x1, y1);
    }

    for(int x = 0; x < 7; x++){
        for(int y = 0; y < 7; y++){
            MuehleField& field = grid[x][y];

            if(!field.exists){
                continue;
            }
            float xpos = x*4.0f - 7.0f*4.0f/2.0f;
            float ypos = y*4.0f - 7.0f*4.0f/2.0f;
            if(field.color == MuehleColor::None){
                Gfx::SetDrawColor(ColorRgb::WHITE);
                Gfx::FillRect(xpos+1, ypos+1, 1, 1);
            } else {
                switch(field.color){
                case MuehleColor::White:
                    Gfx::SetDrawColor(ColorRgb::RED);
                    break;
                case MuehleColor::Black:
                    Gfx::SetDrawColor(ColorRgb::GREEN);
                    break;
                default:
                    Gfx::SetDrawColor(ColorRgb::WHITE);
                    break;
                }
                Gfx::FillRect(xpos+1, ypos+1, 3, 3);
                Gfx::SetDrawColor(ColorRgb::BLACK);
                Gfx::FillRect(xpos+1, ypos+1, 1, 1);
            }

            // Gfx::StrokeRect(0, 0, 32, 32);

        }
    }

    Gfx::Restore();

}
void MuehleApp::OnExit(){

}
void MuehleApp::SetMuehleColor(MuehleField &field, MuehleColor color){
    field.color = color;
}

int main(){
    App::Run(new MuehleApp());
	return EXIT_SUCCESS;
}
