#pragma once

#include <string>
#include "Ledlib2d/Entity.h"

namespace Ledlib {
    class BaseFont;
}

class Anim {
protected:
    std::string text;
    Ledlib::BaseFont* font;
public:
    Anim();
    virtual ~Anim();
    void SetText(const std::string& text, Ledlib::BaseFont* font);
    virtual void OnStart();
    virtual void OnExit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnTextChanged();
};

