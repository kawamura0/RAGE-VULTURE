
#pragma once

#include "ESGLib.h"
#include "GameScene/GameScene.hpp"
#include <random>
#include "../ES Game Library/Myclass/Ggauge.h"
#include "../ES Game Library/Myclass/FujiFilm/TransGauge.h"
#include "../ES Game Library/Myclass/FujiFilm/PlayerAnimation.h"


//プレイヤーの無敵時間（フレーム単位）
#define INVINCIBLE_MAXFRAME 180;
//プレイヤーの軌道不可時間（フレーム単位）
#define DASH_COOLFRAME 30
//無敵中の当たり判定は何倍にするか
#define INVINCIBLE_COLLISION_MULTIPLAYER 3.0;
//ミサイル最大生成数
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
	FONT DefaultFont; //　ノーマルフォント
	FONT Big_Font; //ビッグフォント

private:
	// 変数宣言
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
	
	//ムービー
	MEDIA game_play_movie;
	int   game_play_movie_time;
	int   game_play_movie_state;

	CAMERA camera; //カメラ

	//プレイヤー
	ANIMATIONMODEL Player;
	ANIMATIONMODEL player;
	int            player_flashing;//点滅

	//スカイドーム
	MODEL  skydome; 

	//雲
	MODEL  cloud[SCLORL_MAX];
	float  cloud_y = -600.0f;
	float  cloud_z = -30.0f; 

	//横崖
	MODEL  BG_[SCLORL_MAX];  
	MODEL  BG_2[SCLORL_MAX]; 

	//シーン
	SPRITE title; //タイトル
	
	SPRITE description; //操作説明

	//セレクト
	SPRITE select_bg;//セレクト背景   
	SPRITE select_cursor; //セレクトカーソル

	//ステージボタン
	SPRITE stage_button[STAGE_MAX];
	float  stage_button_x[STAGE_MAX];
	float  stage_button_y[STAGE_MAX];

	float  select_transparent;//透明度

	float  select_move_x = 190.0f;
	float  select_move_y = 430.0f;

	int    select_state;

	int    transparency_state;//透明度

	SPRITE clear; //クリア

	SPRITE over; //オーバー
	
	//フェード
	SPRITE fade; 
	float  faded_out;

	int game_seane = TITLE_SEANE; //シーン遷移

	int scene_count;

	//時間
	SPRITE time_number; //制限時間文字
	float  time_frame; //時間設定値
	
	//警告
	SPRITE warning;//警告
	float  warning_transparent;//透明度
	int    warning_state;

	//トランスゲージ
	SPRITE trans_frame; //トランス枠
	SPRITE trans_gage; //トランスゲージ

	//HP
	Ggauge hp;
	float  hp_gauge;
	Vector3 hp_pos;
	int hp_scene; //体力遷移

	//プレイヤー
	float player_x; //プレイヤーのx座標
	float player_y; //プレイヤーのy座標
	float player_z; //プレイヤーのz座標

	float player_r_x; //プレイヤーの回転x座標
	float player_r_y; //プレイヤーの回転y座標
	float player_r_z; //プレイヤーの回転z座標

	float player_move_time; //動きまでの時間

	float bezier_animation = 0.0f;
	int   bezier_anime_state = 0;

	float player_angle = 0.0f;//プレイヤーの角度

	int   player_invincible_time;//プレイヤー無敵時間

	int   anime_state; //アニメーション時動かなくするための遷移

	int   p_s; //自分遷移

	//カメラ
	float camera_x; //カメラのx座標
	float camera_y; //カメラのy座標
	float camera_z; //カメラのz座標

	float camera_r_x; //カメラの回転x座標
	float camera_r_y; //カメラの回転y座標
	float camera_r_z; //カメラの回転z座標

	float camera_angle; //カメラの視野

	float pad_paramater;//パッドステータス

	//影
	float shadow_deps; //影の濃さ
	float shadow_z; //影のz座標
	
	//時間
	int time_state; //時間遷移

	int first_; //一の位
	int second_; //十の位
//	int third_; //百の位

	int select_move_state = 0;
	int fade_out_state = 0;

	int a = 0;
	MUSIC bgm_; //メインBGM
	MUSIC bgm2;
	MUSIC bgm3;
	MUSIC select_bgm; //セレクトBGM
	SOUND se_; //メインへの遷移音 
	SOUND select_se; //セレクト遷移
	SOUND engine_se; //エンジン音
	SOUND BD_se; //加速音
	SOUND damege_se; //ダメージ音
	SOUND dead_se; //死亡音
	SOUND missile_se; //ミサイル音
	SOUND flash_se; //ビーム音
	SOUND up_se;
	SOUND roll_se;
	Vector3 player_pos;
	Vector3 player_r_pos;
	Vector3 warning_pos[MISSILE_MAX];

	//敵ビーム
	MODEL beam_[BEAM_MAX];

	int beam_state[BEAM_MAX]; //弾遷移

	float beam_x[BEAM_MAX]; //弾のx座標
	float beam_y[BEAM_MAX]; //弾のy座標
	float beam_z[BEAM_MAX]; //弾のz座標

	//敵
	MODEL teki[TEKI_MAX]; 

	float teki_flag[TEKI_MAX];
	
	int teki_state[TEKI_MAX]; //敵遷移

	float teki_pos_x[TEKI_POS_MAX]; //敵のx座標
	float teki_pos_y[TEKI_POS_MAX]; //敵のy座標
	float teki_pos_z[TEKI_POS_MAX]; //敵のz座標

	Vector3 t_pos;

	//ミサイル
	MODEL  enemy_missile[MISSILE_MAX];

	float  bezier_t[MISSILE_MAX];

	Vector3  enemy_get_position;

	Vector3  bezier_position;
	Vector3  bezier_storege[MISSILE_MAX];
	Vector3  bezier_storege2[MISSILE_MAX];
	Vector3  bezier_storege3[MISSILE_MAX];
	Vector3  bezier_storege4[MISSILE_MAX];
	
	int   missile_flag[MISSILE_MAX];
	
	int   enemy_shot_state[MISSILE_MAX]; // 0 1:待機中　2:出現中

	float homing_random[MISSILE_MAX];//ホーミング

	//煙
	
	MODEL  smoke[SMOKE_MAX];
	Vector3 smoke_position[MISSILE_MAX];
	float  smoke_losttime[SMOKE_MAX];//煙の消える時間

	//爆発
	MODEL  bom[MISSILE_MAX];
	float  bom_scale[MISSILE_MAX];
	float  bom_con[MISSILE_MAX];
	int    bom_state[MISSILE_MAX];
	Vector3   bom_position[MISSILE_MAX];

	Vector3 camera_get_position;//カメラの座標取得

	//エネミー
	MODEL enemy; 
	int   enemy_state;
	Vector3 enemy_position;
	
	int enemy_count;
	Vector3 enemy_miisile_position;

	Vector3   player_get_position[MISSILE_MAX];//プレイヤーの座標取得

	//プレイヤー当たり判定
	MODEL player_collision;
	OrientedBoundingBox player_obb;
	Quaternion player_qt;

	//ミサイル当たり判定
	MODEL missile_collision[MISSILE_MAX];
	OrientedBoundingBox missile_obb[MISSILE_MAX];
	Quaternion missile_qt[MISSILE_MAX];

	//敵
	MODEL teki_collsion[TEKI_MAX];
	OrientedBoundingBox teki_obb[TEKI_MAX];
	Quaternion teki_qt[TEKI_MAX];

	//敵ビーム当たり判定
	MODEL beam_collsion[BEAM_MAX];
	OrientedBoundingBox beam_obb[BEAM_MAX];
	Quaternion beam_qt[BEAM_MAX];

	//エネミー当たり判定
	MODEL enemy_collsion;
	OrientedBoundingBox enemy_obb;
	Quaternion enemy_qt;

	// ランダム変数	
	random_device seed;
	mt19937 random;
	uniform_real_distribution<float> dist;


	///////////////////////fujifujiちゃん
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

	//トランスゲージ
	FujiFilm::TransGauge transGauge_;
	//ダッシュ中に当たったミサイルどれか
	std::vector<MODEL> hittingMissile_;

	//アニメ管理
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

	///////////////////////fujifujiちゃん


	// 関数宣言
	void Bg_Update(); //背景処理
	void Select_Draw();
	void Select_Update();
	void Fade_Out_Update();
	void Fade_Out_Draw();
	void Title_Update(); //タイトルの処理
	void Title_Draw(); //タイトルの描画
	void Description_Draw();
	void Clear_Update(); //クリアの処理
	void Clear_Draw(); //クリアの描画
	void Over_Update(); //オーバーの処理
	void Over_Draw(); //オーバーの描画
	void Playre_Initialize();
	void Player_Update(); //自分の処理
	void Player_Draw(); //自分の描画
	void Time_Update(); //タイムの処理
	void Time_Draw(); //タイムの描画

	void Ui_Initialize();

	void Sclorl_Initialize();
	void Sclorl_Update(); //スクロールの処理
	
	void G_Initialize();
	void G_Draw(); //トランス描画
	void Enemy_Initialize();
	void Enemy_Update();
	void Enemy_Draw(); //敵描画
	void Enemy_Shot_Initialize();
	void Enemy_Shot_Draw(); //弾描画
	void Enemy_Shot_Update(); //弾処理

	void All_Initialize();
	void Missile_Initialize();
	void Missile_Update();

	void SetBezierToEnemyMissile(int missilNum, int dx, int dy, int dz);//何番のミサイルか　どの方向に移動（+右 -左）どの方向に移動（+上 -下）

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

	//キーボード＆パッド宣言
	KeyboardState key_state;
	KeyboardBuffer key_buffer; 
	GamePadState pad_state; 
	GamePadBuffer pad_buffer; 

	//シーン名設定
	enum GameSeane
	{
		TITLE_SEANE, //タイトル
		MOVE,
		SETUMEI,
		SELECT_SEANE, //セレクト
		TUTORIAL,
		STAGE1,
		STAGE2,
		CLEAR_SEANE, //クリア
		OVER_SEANE //オーバー
	};


};
 