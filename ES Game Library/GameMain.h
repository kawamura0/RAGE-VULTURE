
#pragma once

#include "ESGLib.h"
#include "GameScene/GameScene.hpp"
#include <random>
#include "../ES Game Library/Myclass/Ggauge.h"
#include "../ES Game Library/Myclass/FujiFilm/TransGauge.h"
#include "../ES Game Library/Myclass/FujiFilm/PlayerAnimation.h"


//�v���C���[�̖��G���ԁi�t���[���P�ʁj
#define INVINCIBLE_MAXFRAME 180;
//�v���C���[�̋O���s���ԁi�t���[���P�ʁj
#define DASH_COOLFRAME 30
//���G���̓����蔻��͉��{�ɂ��邩
#define INVINCIBLE_COLLISION_MULTIPLAYER 3.0;
//�~�T�C���ő吶����
#define MISSILE_MAX 30


class GameMain : public CGameScene {
public:
	GameMain() : DefaultFont(GraphicsDevice.CreateDefaultFont())
	{
//		ContentRootDirectory(_T("Content"));
	}

	virtual ~GameMain()
	{
#ifdef _INC_SQUIRREL
		Squirrel.ReleaseAllScripts();
#endif
#ifdef _INC_NUI
		NUI.ReleaseAllKinects();
#endif
#ifdef _INC_LIVE2D
		Live2D.ReleaseAllModels();
#endif
#ifdef _INC_EFFEKSEER
		Effekseer.ReleaseAllEffects();
#endif
#ifdef _INC_DIRECT2D
		Direct2D.ReleaseAllResources();
#endif
		MediaManager.ReleaseAllMedia();

		SoundDevice.ReleaseAllMusics();
		SoundDevice.ReleaseAllSounds();

		GraphicsDevice.ReleaseAllRenderTargets();
		GraphicsDevice.ReleaseAllStateBlocks();
		GraphicsDevice.ReleaseAllFonts();
		GraphicsDevice.ReleaseAllSprites();
		GraphicsDevice.ReleaseAllAnimationModels();
		GraphicsDevice.ReleaseAllModels();
		GraphicsDevice.ReleaseAllVertexBuffers();
		GraphicsDevice.ReleaseAllEffects();

		Finalize();
	}

public:
	virtual bool Initialize();

	virtual int  Update();
	virtual void Draw();

private:
	void Finalize();
	FONT DefaultFont; //�@�m�[�}���t�H���g
	FONT Big_Font; //�r�b�O�t�H���g

private:
	// �ϐ��錾
	enum
	{
		FLASHING_TIME = 60,
		FLASHING_SYUKI = 6,
		PLYER_INVINCIBLE_TIME = 60,
		SCLORL_MAX = 20,
		STAGE_MAX = 3,
		BEAM_MAX = 16,
		TEKI_MAX = 9,
		TEKI_POS_MAX = 16,
		SMOKE_MAX = 100,
		G_POS_MAX = 80,

	};
	
	//���[�r�[
	MEDIA game_play_movie;
	int   game_play_movie_time;
	int   game_play_movie_state;

	CAMERA camera; //�J����

	//�v���C���[
	ANIMATIONMODEL Player;
	ANIMATIONMODEL player;
	int            player_flashing;//�_��

	//�X�J�C�h�[��
	MODEL  skydome; 

	//�_
	MODEL  cloud[SCLORL_MAX];
	float  cloud_y = -600.0f;
	float  cloud_z = -30.0f; 

	//���R
	MODEL  BG_[SCLORL_MAX];  
	MODEL  BG_2[SCLORL_MAX]; 

	//�V�[��
	SPRITE title; //�^�C�g��
	
	SPRITE description; //�������

	//�Z���N�g
	SPRITE select_bg;//�Z���N�g�w�i   
	SPRITE select_cursor; //�Z���N�g�J�[�\��

	//�X�e�[�W�{�^��
	SPRITE stage_button[STAGE_MAX];
	float  stage_button_x[STAGE_MAX];
	float  stage_button_y[STAGE_MAX];

	float  select_transparent;//�����x

	float  select_move_x = 190.0f;
	float  select_move_y = 430.0f;

	int    select_state;

	int    transparency_state;//�����x

	SPRITE clear; //�N���A

	SPRITE over; //�I�[�o�[
	
	//�t�F�[�h
	SPRITE fade; 
	float  faded_out;

	int game_seane = TITLE_SEANE; //�V�[���J��

	int scene_count;

	//����
	SPRITE time_number; //�������ԕ���
	float  time_frame; //���Ԑݒ�l
	
	//�x��
	SPRITE warning;//�x��
	float  warning_transparent;//�����x
	int    warning_state;

	//�g�����X�Q�[�W
	SPRITE trans_frame; //�g�����X�g
	SPRITE trans_gage; //�g�����X�Q�[�W

	//HP
	Ggauge hp;
	float  hp_gauge;
	Vector3 hp_pos;
	int hp_scene; //�̗͑J��

	//�v���C���[
	float player_x; //�v���C���[��x���W
	float player_y; //�v���C���[��y���W
	float player_z; //�v���C���[��z���W

	float player_r_x; //�v���C���[�̉�]x���W
	float player_r_y; //�v���C���[�̉�]y���W
	float player_r_z; //�v���C���[�̉�]z���W

	float player_move_time; //�����܂ł̎���

	float bezier_animation = 0.0f;
	int   bezier_anime_state = 0;

	float player_angle = 0.0f;//�v���C���[�̊p�x

	int   player_invincible_time;//�v���C���[���G����

	int   anime_state; //�A�j���[�V�����������Ȃ����邽�߂̑J��

	int   p_s; //�����J��

	//�J����
	float camera_x; //�J������x���W
	float camera_y; //�J������y���W
	float camera_z; //�J������z���W

	float camera_r_x; //�J�����̉�]x���W
	float camera_r_y; //�J�����̉�]y���W
	float camera_r_z; //�J�����̉�]z���W

	float camera_angle; //�J�����̎���

	float pad_paramater;//�p�b�h�X�e�[�^�X

	//�e
	float shadow_deps; //�e�̔Z��
	float shadow_z; //�e��z���W
	
	//����
	int time_state; //���ԑJ��

	int first_; //��̈�
	int second_; //�\�̈�
//	int third_; //�S�̈�

	int select_move_state = 0;
	int fade_out_state = 0;

	int a = 0;
	MUSIC bgm_; //���C��BGM
	MUSIC bgm2;
	MUSIC bgm3;
	MUSIC select_bgm; //�Z���N�gBGM
	SOUND se_; //���C���ւ̑J�ډ� 
	SOUND select_se; //�Z���N�g�J��
	SOUND engine_se; //�G���W����
	SOUND BD_se; //������
	SOUND damege_se; //�_���[�W��
	SOUND dead_se; //���S��
	SOUND missile_se; //�~�T�C����
	SOUND flash_se; //�r�[����
	SOUND up_se;
	SOUND roll_se;
	Vector3 player_pos;
	Vector3 player_r_pos;
	Vector3 warning_pos[MISSILE_MAX];

	//�G�r�[��
	MODEL beam_[BEAM_MAX];

	int beam_state[BEAM_MAX]; //�e�J��

	float beam_x[BEAM_MAX]; //�e��x���W
	float beam_y[BEAM_MAX]; //�e��y���W
	float beam_z[BEAM_MAX]; //�e��z���W

	//�G
	MODEL teki[TEKI_MAX]; 

	float teki_flag[TEKI_MAX];
	
	int teki_state[TEKI_MAX]; //�G�J��

	float teki_pos_x[TEKI_POS_MAX]; //�G��x���W
	float teki_pos_y[TEKI_POS_MAX]; //�G��y���W
	float teki_pos_z[TEKI_POS_MAX]; //�G��z���W

	Vector3 t_pos;

	//�~�T�C��
	MODEL  enemy_missile[MISSILE_MAX];

	float  bezier_t[MISSILE_MAX];

	Vector3  enemy_get_position;

	Vector3  bezier_position;
	Vector3  bezier_storege[MISSILE_MAX];
	Vector3  bezier_storege2[MISSILE_MAX];
	Vector3  bezier_storege3[MISSILE_MAX];
	Vector3  bezier_storege4[MISSILE_MAX];
	
	int   missile_flag[MISSILE_MAX];
	
	int   enemy_shot_state[MISSILE_MAX]; // 0 1:�ҋ@���@2:�o����

	float homing_random[MISSILE_MAX];//�z�[�~���O

	//��
	
	MODEL  smoke[SMOKE_MAX];
	Vector3 smoke_position[MISSILE_MAX];
	float  smoke_losttime[SMOKE_MAX];//���̏����鎞��

	//����
	MODEL  bom[MISSILE_MAX];
	float  bom_scale[MISSILE_MAX];
	float  bom_con[MISSILE_MAX];
	int    bom_state[MISSILE_MAX];
	Vector3   bom_position[MISSILE_MAX];

	Vector3 camera_get_position;//�J�����̍��W�擾

	//�G�l�~�[
	MODEL enemy; 
	int   enemy_state;
	Vector3 enemy_position;
	
	int enemy_count;
	Vector3 enemy_miisile_position;

	Vector3   player_get_position[MISSILE_MAX];//�v���C���[�̍��W�擾

	//�v���C���[�����蔻��
	MODEL player_collision;
	OrientedBoundingBox player_obb;
	Quaternion player_qt;

	//�~�T�C�������蔻��
	MODEL missile_collision[MISSILE_MAX];
	OrientedBoundingBox missile_obb[MISSILE_MAX];
	Quaternion missile_qt[MISSILE_MAX];

	//�G
	MODEL teki_collsion[TEKI_MAX];
	OrientedBoundingBox teki_obb[TEKI_MAX];
	Quaternion teki_qt[TEKI_MAX];

	//�G�r�[�������蔻��
	MODEL beam_collsion[BEAM_MAX];
	OrientedBoundingBox beam_obb[BEAM_MAX];
	Quaternion beam_qt[BEAM_MAX];

	//�G�l�~�[�����蔻��
	MODEL enemy_collsion;
	OrientedBoundingBox enemy_obb;
	Quaternion enemy_qt;

	// �����_���ϐ�	
	random_device seed;
	mt19937 random;
	uniform_real_distribution<float> dist;


	///////////////////////fujifuji�����
	enum AXIS {

		NONE,
		UP,
		RIGHT,
		DOWN,
		LEFT,
		OVER

	};
	void SetAnimation(AXIS leftStickAxis);
	AXIS GetLeftStickAxis();

	std::list<AXIS> nextAxis_;

	void UpdateCollisionToHittingMissile();

	//�g�����X�Q�[�W
	FujiFilm::TransGauge transGauge_;
	//�_�b�V�����ɓ��������~�T�C���ǂꂩ
	std::vector<MODEL> hittingMissile_;

	//�A�j���Ǘ�
	FujiFilm::PlayerAnimation playerAnimeManager_;

	struct TrackSettingValues {

		int track_;
		float speed_;
		float endTime_;

		TrackSettingValues(int track, float speed, float endTime) {

			this->track_ = track;
			this->speed_ = speed;
			this->endTime_ = endTime;

		}

	};

	static const std::vector<TrackSettingValues> TRACK_OPTION_VALUES_;
	static const std::vector<int> DASH_CHAIN_LIMIT_;

	int playerInvincibleFrame_;
	int cantDashFrame_;
	int prevLevel_;
	int chainCount_;

	///////////////////////fujifuji�����


	// �֐��錾
	void Bg_Update(); //�w�i����
	void Select_Draw();
	void Select_Update();
	void Fade_Out_Update();
	void Fade_Out_Draw();
	void Title_Update(); //�^�C�g���̏���
	void Title_Draw(); //�^�C�g���̕`��
	void Description_Draw();
	void Clear_Update(); //�N���A�̏���
	void Clear_Draw(); //�N���A�̕`��
	void Over_Update(); //�I�[�o�[�̏���
	void Over_Draw(); //�I�[�o�[�̕`��
	void Playre_Initialize();
	void Player_Update(); //�����̏���
	void Player_Draw(); //�����̕`��
	void Time_Update(); //�^�C���̏���
	void Time_Draw(); //�^�C���̕`��

	void Ui_Initialize();

	void Sclorl_Initialize();
	void Sclorl_Update(); //�X�N���[���̏���
	
	void G_Initialize();
	void G_Draw(); //�g�����X�`��
	void Enemy_Initialize();
	void Enemy_Update();
	void Enemy_Draw(); //�G�`��
	void Enemy_Shot_Initialize();
	void Enemy_Shot_Draw(); //�e�`��
	void Enemy_Shot_Update(); //�e����

	void All_Initialize();
	void Missile_Initialize();
	void Missile_Update();

	void SetBezierToEnemyMissile(int missilNum, int dx, int dy, int dz);//���Ԃ̃~�T�C�����@�ǂ̕����Ɉړ��i+�E -���j�ǂ̕����Ɉړ��i+�� -���j

	void Tutorial_Initialize();
	void Tutorial_Update();
	void Tutorial_Teki_Intialize();
	void Tutorial_Teki_Update();
	void Tutorial_Time_Intialize();
	void Tutorial_Time_Update();

	void Stage1_Intialize();
	void Stage1_Update();
	void Stage1_Teki_Intialize();
	void Stage1_Teki_Update();
	void Stage1_Time_Intialize();
	void Stage1_Time_Update();

	void Stage2_Initialize();
	void Stage2_Update();
	void Stage2_Teki_Intialize();
	void Stage2_Teki_Update();
	void Stage2_Time_Intialize();
	void Stage2_Time_Update();

	//�L�[�{�[�h���p�b�h�錾
	KeyboardState key_state;
	KeyboardBuffer key_buffer; 
	GamePadState pad_state; 
	GamePadBuffer pad_buffer; 

	//�V�[�����ݒ�
	enum GameSeane
	{
		TITLE_SEANE, //�^�C�g��
		MOVE,
		SETUMEI,
		SELECT_SEANE, //�Z���N�g
		TUTORIAL,
		STAGE1,
		STAGE2,
		CLEAR_SEANE, //�N���A
		OVER_SEANE //�I�[�o�[
	};


};
 