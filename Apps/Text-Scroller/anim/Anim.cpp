#include "Anim.h"
#include "Ledlib/Util/Strings.h"
#include "Ledlib/Log.h"

using namespace Ledlib;

Anim::Anim(){}
Anim::~Anim(){}

void Anim::SetText(const std::string& text, Ledlib::BaseFont* font){
    this->text = text;
    this->text = Strings::ReplaceAll(this->text, "\n", " ");
    this->text = Strings::Trim(this->text);
    this->text = Strings::RemoveMultipleWhitespaces(this->text);
    this->font = font;
    Log(LOG_DEBUG, "Anim", iLog << "Text set to: " << this->text);
    this->OnTextChanged();
}
void Anim::OnStart(){

}
void Anim::OnExit(){

}
void Anim::OnUpdate(){

}
void Anim::OnRender(){

}
void Anim::OnTextChanged(){

}


