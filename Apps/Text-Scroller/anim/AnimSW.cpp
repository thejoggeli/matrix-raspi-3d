#include "AnimSW.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/AppCamera.h"
#include "Ledlib/Util/Strings.h"
#include "Ledlib/Log.h"
#include "Ledlib2d/Resources/Font.h"
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib/Time.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace Ledlib;

AnimSW::AnimSW(){
    row_width = 64;
}

void AnimSW::OnStart(){

}
void AnimSW::OnExit(){

}
void AnimSW::OnUpdate(){

}
void AnimSW::OnRender(){
    Gfx::SetProjectionMatrix(glm::perspective(60.0f, Gfx::width/Gfx::height, 0.1f, 100000.0f));
    Gfx::Save();
    Gfx::Scale(-1, -1, 1);
    Gfx::Translate(0, -0.5, -1.5);
    Gfx::Rotate(-0.25f, glm::vec3(1, 0, 0));
    Gfx::Translate(0, sin(Time::sinceStart)*10.0f, 0);
    Gfx::DrawBitmap(bitmap.get(), 0, 0);
    Gfx::Restore();
    // Gfx::DrawBitmap(bitmap.get(), 0, 5);
}
void AnimSW::OnTextChanged(){
    rows.clear();

    auto words = Strings::Split(this->text, " ");

    Gfx::SetFont(font);
    Gfx::antiKerning = true;
    Gfx::textCharacterSpacing = 0;

    std::string row = "";
    for(int i = 0; i < words.size(); i++){
        std::string tmp = Strings::Trim(row + " " + words[i]);
        int w = font->GetTextWidth(tmp, Gfx::textCharacterSpacing, Gfx::antiKerning);
        if(w > row_width){
            if(row == ""){
                rows.push_back(tmp);
                row = "";
            } else {
                rows.push_back(row);
                row = words[i];
            }
        } else {
            row = tmp;
        }
    }
    if(row != ""){
        rows.push_back(row);
    }

    int padding = 4;
    int spacing = 0;
    int h = font->glyphHeight * rows.size() + spacing * (rows.size()-1) + padding*2;
    bitmap = Bitmap::CreateRenderable(row_width + padding*2, h);

    Gfx::Save();
    Gfx::SetRenderTarget(bitmap.get());
    Gfx::ClearTransform();
    Gfx::Scale(1.0f*Gfx::width/bitmap->width, 1.0f*Gfx::height/bitmap->height);
    Gfx::Translate(0, bitmap->height/2.0f);
    Gfx::SetTextColor(1, 1, 0);
    Gfx::SetTextPosition(TextAlign::Center, TextBaseline::Middle);
    Gfx::Clear(0, 0, 0);
    int y = -padding - font->glyphHeight/2;
    for(auto& row: rows){
        Gfx::DrawText(row, 0, y);
        y -= font->glyphHeight;
        y -= spacing;
    }
    Gfx::SetRenderTarget(nullptr);
    Gfx::Restore();

}

