#include "../../GameMain.h"
#include <math.h>

void GameMain::SetAnimation(AXIS Axis ) {

	if (this->prevLevel_ != -1) {
		int limit = this->DASH_CHAIN_LIMIT_[this->prevLevel_];
		if (this->chainCount_ >= limit) {
			this->nextAxis_.clear();
			this->playerInvincibleFrame_ = 0;
			this->prevLevel_ = -1;
			this->chainCount_ = 0;
			this->cantDashFrame_ = DASH_COOLFRAME;
			cloud_z = 20.0f;
			bezier_animation = 0.0f;
			for (int i = 0; i < 20; i++)
			{
				//床の初期位置
				cloud[i]->SetPosition(0.0f, -300.0, i*400.0f);

				//床の初期位置
				BG_[i]->SetPosition(500.0f, cloud_y, i*400.0f);

				//床の初期位置
				BG_2[i]->SetPosition(-500.0f, cloud_y, i*400.0f);
			}

			return;

		}
		else {

			this->nextAxis_.pop_front();

		}

	}	

	this->playerAnimeManager_.SetCompletedMethod([this]() {

		if (this->nextAxis_.size() > 0) {
			if (this->nextAxis_.front() != NONE) {
				this->SetAnimation(this->nextAxis_.front());
				return;
			}
		}

		//保険（はぁと）
		{
			this->nextAxis_.clear();
			this->playerInvincibleFrame_ = 0;
			this->cantDashFrame_ = DASH_COOLFRAME;
			//this->prevPlaytrack_ = -1;
			this->prevLevel_ = -1;
			this->chainCount_ = 0;
			cloud_z = 20.0f;
			bezier_animation = 0.0f;
			for (int i = 0; i < 20; i++)
			{
				//床の初期位置
				cloud[i]->SetPosition(0.0f, -300.0, i*400.0f);

				//床の初期位置
				BG_[i]->SetPosition(500.0f, cloud_y, i*400.0f);

				//床の初期位置
				BG_2[i]->SetPosition(-500.0f, cloud_y, i*400.0f);
			}
		}
	
	});

	this->chainCount_++;
	this->playerInvincibleFrame_ = INVINCIBLE_MAXFRAME;

	if (this->prevLevel_ == -1) this->prevLevel_ = this->transGauge_.GetNowLevel();
	
	int trackNum = this->prevLevel_;

	std::function<void(float,float,float)> method;


	switch (Axis) {

	case LEFT:

		//左 
		    roll_se->Play();
			trackNum += 0;
			player_pos = Player->GetPosition();
			player_r_pos = Player->GetRotation();
			player_angle = 0.0f;
			method = [this](float speed, float nowTime, float animeLength) {
				
			player_x -= 1.5f;
			camera_r_y += 0.5f;
			if (camera_r_y >= 40.0)
			{
				camera_r_y = 40.0f;
			}

				if (this->prevLevel_ == 2) {

					player_x = player_pos.x - 30.0f + MathHelper_Cos(player_angle) * 30.0f;
					player_y = player_pos.y - 0.0f + MathHelper_Sin(player_angle) * 30.0f;

					player_angle = nowTime / animeLength * 360;

					camera_r_y += 0.2f;
					if (camera_r_y >= 40.0)
					{
						camera_r_y = 40.0f;
					}

				}

			};

			break;

	case RIGHT:
		//右

		    roll_se->Play();
			trackNum += 3;
			player_pos = Player->GetPosition();
			player_r_pos = Player->GetRotation();
			player_angle = 0.0f;
			method = [this](float speed, float nowTime, float animeLength) {
			
				
			player_x += 1.5f;
			camera_r_y -= 0.5f;
			if (camera_r_y <= -40.0)
			{
				camera_r_y = -40.0f;
			}


				if (this->prevLevel_ == 2)
				{
					player_x = player_pos.x + 20.0 + MathHelper_Cos(player_angle) * -20.0;
					player_y = player_pos.y + 0.0 + MathHelper_Sin(player_angle) * -20.0;

					player_angle = nowTime / animeLength * -360;

					camera_r_y -= 0.2f;
					if (camera_r_y <= -40.0)
					{
						camera_r_y = -40.0f;
					}

				}
			};

		break;

	case UP:

		trackNum += 6;
		player_pos = Player->GetPosition();
		player_r_pos = Player->GetRotation();
		bezier_animation = 0.0f;
		method = [&](float speed, float nowTime, float animeLength) {
			player_z += 1.5f;

			if (this->prevLevel_ == 2)
			{

				float t = nowTime / animeLength;

				Vector3 bezier_ = Vector3_Bezier(Vector3(player_pos.x, player_pos.y, player_pos.z),
					Vector3(player_pos.x, player_pos.y + 30, player_pos.z + 50),
					Vector3(player_pos.x, player_pos.y + 30, player_pos.z + 100),
					Vector3(player_pos.x, player_pos.y, player_pos.z + 150), t);

				player_y = bezier_.y;
				player_z = bezier_.z;

				player_z += 1.5f;

			}
		};

		break;

	case DOWN:

		trackNum += 12;
		BD_se->Play();
		method = [this](float speed, float nowTime, float animeLength) {
			player_z -= 1.0f * speed;

		};

		break;

	case OVER:

		trackNum += 9;
		player_pos = Player->GetPosition();
		player_r_pos = Player->GetRotation();
		bezier_animation = 0.0f;
		method = [this](float speed, float nowTime, float animeLength) {


			if (this->prevLevel_ == 2)
			{

				float t = nowTime / animeLength;

				Vector3 bezier_ = Vector3_Bezier(Vector3(player_pos.x, player_pos.y, player_pos.z),
					Vector3(player_pos.x, player_pos.y + 30, player_pos.z + 50),
					Vector3(player_pos.x, player_pos.y + 30, player_pos.z + 100),
					Vector3(player_pos.x, player_pos.y, player_pos.z + 150), t);

				player_y = bezier_.y;
				player_z = bezier_.z;
			}

		};

		break;

	
	}

	TrackSettingValues trackOptions = this->TRACK_OPTION_VALUES_[trackNum];

	this->playerAnimeManager_.Play(trackOptions.track_,trackOptions.speed_,trackOptions.endTime_,method);

}