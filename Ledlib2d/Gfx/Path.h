#pragma once

namespace Ledlib {
namespace Gfx {

void BeginPath();
void LineTo(float x, float y, float z = 0);
void ClosePath();
void FillPath();
void StrokePath();

}
}
