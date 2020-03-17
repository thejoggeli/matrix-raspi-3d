#include "Ball.h"
#include "Paddle.h"
#include "Ledlib/Time.h"
#include "Ledlib/Log.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Physics/BoxCollider.h"
#include "Ledlib2d/Physics/CollisionData.h"

Ball::Ball(){}

void Ball::OnStart(){
	SetPosition(0, 0);
	velocity = glm::vec2(-30.0f, 10.0f);
	bounds.Set(Gfx::left-5, Gfx::bottom+1, Gfx::right+5, Gfx::top-1);
	std::shared_ptr<BoxCollider> collider = Collider::Create<BoxCollider>("ball");
	collider->SetSize(radius*2, radius*2);
	SetCollider(collider);
	oldPositionTimer.Start(0.02f);
	velocityGain = 1.0f;
}

void Ball::OnUpdate(){
	oldPosition = position;
	bool respawn = false;
	if(oldPositionTimer.IsFinished()){
		oldPositionTimer.Restart(true);
		oldPositions.insert(oldPositions.begin(), position);
		while(oldPositions.size() > 6){
			oldPositions.pop_back();
		}
	}
	Translate(glm::vec3(velocity * Time::deltaTime, 0));
	if(position.x > bounds.w+radius){
		respawn = true;
	} else if(position.x < bounds.x-radius){
		respawn = true;
	}
	if(position.y > bounds.h-radius){
		SetPosition(position.x, bounds.h-radius);
		velocity.y = -velocity.y;
	} else if(position.y < bounds.y+radius){
		SetPosition(position.x, bounds.y+radius);
		velocity.y = -velocity.y;
	}
	if(respawn){
		SetPosition(0, 0);
		velocity.x = velocity.x > 0 ? -30 : 30;
		if(fabsf(velocity.y) > fabsf(velocity.x)) velocity.y = velocity.x;
		oldPositions.clear();
		oldPositionTimer.Restart(true);
	}
	velocity += glm::normalize(velocity) * velocityGain * Time::deltaTime;
}

void Ball::OnRender(){
	ColorRgb color = ColorRgb::YELLOW;
	const float maxc = 120.0f;
	const float minc = 40.0f;
	color.g = Numbers::Clamp(0.0f, 255.0f, maxc*255.0f/minc-glm::length(velocity)*255.0f/minc);
//	color.g = Numbers::Clamp(0.0f, 255.0f, (maxc-velocity.GetLength())*255.0f/minc);
	Gfx::SetDrawColor(color);
	Gfx::FillRect(0, 0, radius*2, radius*2);
/*	Color color = Color::YELLOW;
	color.a = 255-32;
	for(auto const& v: oldPositions){
		color.a -= 32;
		Gfx::SetDrawColor(color);
		Gfx::FillRect(v.x-radius, v.y-radius, radius*2, radius*2);
	} */
}

void Ball::OnCollision(const CollisionData &data){
	Log("collision");
	if(data.b->group == "paddle"){
		std::shared_ptr<Paddle> paddle = std::static_pointer_cast<Paddle>(data.b->GetEntity());
		glm::vec2 oldv = velocity;
		glm::vec2 padd_inp_pos = (paddle->oldPosition+glm::vec2(paddle->position))/2.0f;
		glm::vec2 ball_inp_pos = (oldPosition+glm::vec2(position))/2.0f;
		float padd_radius = paddle->width/2.0f;
		float padd_offset = paddle->height/2.0f - padd_radius;
		if(ball_inp_pos.y > padd_inp_pos.y+padd_offset || ball_inp_pos.y < padd_inp_pos.y-padd_offset){
			// corner collision
			glm::vec2 center = padd_inp_pos;
			if(ball_inp_pos.y > padd_inp_pos.y+padd_offset){
				center.y += padd_offset;
			} else {
				center.y -= padd_offset;
			}
			glm::vec2 dir = glm::normalize(ball_inp_pos - center);
			glm::vec2 nv = dir * glm::length(oldv);
			glm::vec2 pv = (glm::vec2(paddle->position) - paddle->oldPosition)/Time::deltaTime;
			nv.y += pv.y * 0.5f;
			velocity = nv;
		} else {
			// frontal collision
			velocity.x = -velocity.x;
			glm::vec2 pv = (glm::vec2(paddle->position) - paddle->oldPosition)/Time::deltaTime;
			float dvy = pv.y - velocity.y;
			velocity.y += dvy * 0.2f;
		}
		// prevent low x-velocity
		if(velocity.x > -1 && velocity.x <= 0) velocity.x = -1;
		else if(velocity.x < 1 && velocity.x >= 0) velocity.x = 1;
		// move out
		Translate(glm::vec3(velocity, 0)*Time::deltaTime);
	}
}
