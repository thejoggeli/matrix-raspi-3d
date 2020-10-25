#pragma once

#include <vector>
#include "muehlecolor.h"

class MuehleLine;

class MuehleField {
public:

    MuehleColor color = MuehleColor::None;
    bool exists = false;
    int x, y;
    std::vector<MuehleLine*> lines;

    MuehleField();
    void AddLine(MuehleLine* line);
};

