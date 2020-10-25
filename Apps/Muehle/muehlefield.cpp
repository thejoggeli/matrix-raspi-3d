#include "muehlefield.h"
#include <algorithm>

MuehleField::MuehleField(){}

void MuehleField::AddLine(MuehleLine* line){
    if(std::find(lines.begin(), lines.end(), line) != lines.end()){
        // already contains
    } else {
        lines.push_back(line);
    }
}
