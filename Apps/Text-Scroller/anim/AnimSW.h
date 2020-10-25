#pragma once

#include "Anim.h"

namespace Ledlib {
    class Bitmap;
}

class AnimSW : public Anim {
private:
    std::vector<std::string> rows;
    std::shared_ptr<Ledlib::Bitmap> bitmap;
    int row_width = 64;
public:
    AnimSW();

    void OnStart();
    void OnExit();
    void OnUpdate();
    void OnRender();
    void OnTextChanged();
};

