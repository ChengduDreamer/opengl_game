#include "plane.h"
#include "element_factory.h"
#include "game_context.h"
namespace yk {
	Plane::Plane(const std::string& img_relative_path, const std::string& vs_path, const std::string& fs_path, float x, float y, float width, float height) :
	Object(img_relative_path, vs_path, fs_path, x, y, width, height) {
		EElementType element_type_ = EElementType::kPlane;
	}
	Plane::~Plane() {
	
	}
	void Plane::LaunchMissile() {
		yk::Position plane_pos = this->GetCurrentHeadPosition();
		float missile_width = 0.017f;
		float missile_height = 0.035f;
		std::shared_ptr<yk::Missile> missile_ptr = yk::ElementFactory::CreateMissile("image/missile_1.png", "shader/hero_b_1.vs", "shader/hero_b_1.fs", plane_pos.x - missile_width / 2, 
			camp_type_ == ECampType::kOur ? plane_pos.y + missile_height : plane_pos.y, missile_width, missile_height);

		switch (camp_type_)
		{
		case yk::ECampType::kOur:
			missile_ptr->SetDirection(static_cast<uint8_t>(yk::EDirection::kU));
			yk::GameContext::GetInstance()->AddOurMissileObject(missile_ptr);
			break;
		case yk::ECampType::kEnemy:
			missile_ptr->SetDirection(static_cast<uint8_t>(yk::EDirection::kD));
			yk::GameContext::GetInstance()->AddEnemyMissileObject(missile_ptr);
			break;
		}
	}
}