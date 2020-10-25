#pragma once

#include "Ledlib2d/App.h"

#include "anim/Anim.h"
#include "anim/AnimSW.h"

class MyApp : public Ledlib::App {
public:

    std::unique_ptr<Anim> anim;

    MyApp();
    void OnSetup() override;
    void OnStart() override;
    void OnUpdate() override;
    void OnRender() override;
    void OnExit() override;
};

