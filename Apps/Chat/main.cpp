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
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Resources/Font.h"

using namespace std;
using namespace Ledlib;

class ChatMessage {
public:
    string str;
    int clientId;
    BaseFont* font;
    ColorRgb color = ColorRgb(1.0, 1.0, 1.0);
    float x = 0.0;
    float xdir = 1.0;
    float xtimer = 0.0;
    float width = 0.0;
};

static list<std::shared_ptr<ChatMessage>> chatMessages;

class ChatApp : public App {
	void OnSetup() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
    void OnExit() override;
    static void WriteChatMessage(int clientId, const string& str);
    static void OnMessageChat(void* obj, MessageEvent& message);
};
void ChatApp::OnSetup() {
    EventManager::SubscribeMessage("chat", this, &ChatApp::OnMessageChat);
    ResourceManager::LoadFontBdf("font-1", "4x6.bdf");
    WriteChatMessage(0, "hallo chat");
    WriteChatMessage(0, "test test test test test test test ");
}
void ChatApp::OnStart(){

}
void ChatApp::OnUpdate() {

}
void ChatApp::OnRender() {

    int y = Gfx::bottom;

    Gfx::SetTextPosition(TextAlign::Left, TextBaseline::Bottom);
    Gfx::antiKerning = true;
    Gfx::textCharacterSpacing = 0;

    for (auto rit = chatMessages.rbegin(); rit != chatMessages.rend(); ++rit){
        ChatMessage* chatMessage = (*rit).get();
        BaseFont* font = chatMessage->font;
        Gfx::SetFont(font);
        Gfx::SetTextColor(chatMessage->color);
        Gfx::DrawText(chatMessage->str, Gfx::left, y);
        y += font->glyphHeight;
        if(chatMessage->color.a < 1.0){
            chatMessage->color.a = Numbers::Max(0.0, chatMessage->color.a + Time::deltaTime);
        }
        if(chatMessage->width > Gfx::width){
            if(chatMessage->xtimer > 0.0f){
                chatMessage->xtimer -= Time::deltaTime;
                if(chatMessage->xtimer <= 0.0){
                    chatMessage->xtimer = 0.0;
                }
            } else {
                chatMessage->x += chatMessage->xdir * Time::deltaTime * 25.0f;
                if(chatMessage->x <= 0.0){
                    chatMessage->x = 0.0;
                    chatMessage->xtimer = 1.0;
                    chatMessage->xdir = 1.0;
                } else if(chatMessage->x > Gfx::width){
                    chatMessage->x = Gfx::width;
                    chatMessage->xtimer = 1.0;
                    chatMessage->xdir = -1.0;
                }
            }
        }

    }
}
void ChatApp::OnExit(){

}
void ChatApp::WriteChatMessage(int clientId, const string& str){
    std::shared_ptr<ChatMessage> chatMessage = make_shared<ChatMessage>();
    chatMessage->str = str;
    chatMessage->clientId = clientId;
    chatMessage->font = ResourceManager::GetFont("font-1");
    chatMessage->color = ColorHsl::HslToRgb(Numbers::Random(0.0f, 1.0f), 1.0f, 0.5f);
    chatMessage->color.a = 0.0;
    chatMessage->width = chatMessage->font->GetTextWidth(chatMessage->str, 0, true);
    chatMessages.push_back(chatMessage);
    if(chatMessages.size() > 10){
        chatMessages.pop_front();
    }

}
void ChatApp::OnMessageChat(void *obj, MessageEvent &message){
    shared_ptr<ChatMessage> chatMessage = std::make_shared<ChatMessage>();
    ChatApp::WriteChatMessage(message.clientId, message.GetParam(0));
}


int main(){
    App::Run(new ChatApp());
	return EXIT_SUCCESS;
}
