// #include "Extension\DirectX11\DXGraphics11.hpp"
#include "StdAfx.h"
#include "GameMain.h"
#include <functional>
#include <math.h>

#include "../ES Game Library/Myclass/MethodTimer.h"
#include "../ES Game Library/Myclass/ReadFireTimeMap.h"

/// <summary>
/// Allows the game to perform any initialization it needs to before starting to run.
/// This is where it can query for any required services and load all of your content.
/// Initialize will enumerate through any components and initialize them as well.
/// </summary>


const std::vector<GameMain::TrackSettingValues> GameMain::TRACK_OPTION_VALUES_ = {

//�Đ��g���b�N�@�X�s�[�h�@�A�j���̎���
//�ŏ��̐�����-1�ɂ���ƍĐ����܂���B
//������
{ TrackSettingValues(4,4.0f,4.0f) },
{ TrackSettingValues(4,4.0f,4.0f) },
{ TrackSettingValues(4,4.0f,4.0f) },
//�����E
{ TrackSettingValues(1,4.0f,4.0f) },
{ TrackSettingValues(1,4.0f,4.0f) },
{ TrackSettingValues(1,4.0f,4.0f) },
//���
{ TrackSettingValues(7,4.0f,2.5f) },    
{ TrackSettingValues(7,4.0f,2.5f) },
{ TrackSettingValues(7,4.0f,2.5f) },

{ TrackSettingValues(10,4.0f,4.0f) },
{ TrackSettingValues(10,4.0f,4.0f) },
{ TrackSettingValues(10,4.0f,4.0f) },

//�O
{ TrackSettingValues(-1,2.0f,3.0f)},
{ TrackSettingValues(-1,2.0f,4.0f) },
{ TrackSettingValues(-1,2.0f,5.0f) },

};

//-1�Ŗ������@���x�����Ƃɉ��A���܂ŋO���\��
const std::vector<int> GameMain::DASH_CHAIN_LIMIT_ = {

	1,
	1,
	1
};


//�����l
bool GameMain::Initialize()
{
	// TODO: Add your initialization logic here
	WindowTitle(_T("RAGE VULTURE"));

	GraphicsDevice.SetRenderState(NormalizeNormals_Enable);
	MediaManager.Attach(GraphicsDevice);

	//���C�g
	Light light_;
	
	light_.Type = Light_Directional; // �f�B���N�V���i�����C�g
	light_.Diffuse = Color(1.0f, 1.0f, 1.0f); // �f�B�t���[�Y�F
	light_.Ambient = Color(1.0f, 1.0f, 1.0f); // �X�y�L�����[�F
	light_.Specular = Color(1.0f, 1.0f, 1.0f); // �A���r�G���g�F
	light_.Direction = Vector3_Down; // ���C�g�̕����x�N�g��

	// //���C�g�̓ǂݍ���
	GraphicsDevice.SetLight(light_);

	// �r���[�|�[�g���擾
	Viewport   view = GraphicsDevice.GetViewport();

	// �J�����ݒ�
	camera->SetView(Vector3(0.0f, 80.0f, camera_z), Vector3(camera_r_x, camera_r_y, camera_r_z));
	camera->SetPerspectiveFieldOfView(60.0f, (float)view.Width, (float)view.Height, 1.0f, 10000.0f);
	GraphicsDevice.SetCamera(camera);
	GraphicsDevice.SetRenderState(CullMode_None);

	//�_
	GraphicsDevice.CreateSharedModelFromFile(_T("Ground/ground.x"), cloud, SCLORL_MAX); 
	GraphicsDevice.CreateSharedModelFromFile(_T("B/canyon.X"), BG_, SCLORL_MAX); 
	GraphicsDevice.CreateSharedModelFromFile(_T("G/canyon.X"), BG_2, SCLORL_MAX);
		
	SPRITE cloud_texture = GraphicsDevice.CreateSpriteFromFile(_T("Ground/�_�C.png"));

	for (int i = 0; i < SCLORL_MAX; i++)
	{
		cloud[i]->SetScale(2.0f, 1.0f, 2.0f);
		
		cloud[i]->SetTexture(*cloud_texture);

		//��
		BG_[i]->SetScale(1.0f, 10.0f, 1.0f); //���̑傫��
		BG_2[i]->SetScale(1.0f, 10.0f, 1.0f); //���̑傫��

		//�}�e���A��
		Material mtrl(cloud[i]);
		mtrl.Emissive = Color(1.0f, 1.0f, 1.0f); //�G�~�b�V�u�ݒ�
		mtrl.Diffuse = Color(0.5f, 0.5f, 0.5f);
		mtrl.Ambient = Color(0.25f, 0.25f, 0.25f);
		cloud[i]->SetMaterial(mtrl);

		BG_[i]->SetMaterial(mtrl);

		BG_2[i]->SetMaterial(mtrl);
	}

	//�X�J�C�h�[��
	skydome = GraphicsDevice.CreateModelFromFile(_T("SkyDome/SkyDome.x"));
	skydome->SetScale(100.0f);

	//�t�H�O
	GraphicsDevice.SetRenderState(Fog_Enable); //�t�H�O��L���ɂ���
	GraphicsDevice.SetRenderState(FogVertexMode_Linear); //���`�t�H�O
	GraphicsDevice.SetRenderState(FogColor(219, 227, 243)); //�t�H�O�̐F
	GraphicsDevice.SetRenderStateF(FogStart, 1000.0f); //�t�H�O�̊J�n�_
	GraphicsDevice.SetRenderStateF(FogEnd, 3000.0f); //�t�H�O�̏I���_

	//����
	game_play_movie = MediaManager.CreateMediaFromFile(_T("move/play_movie.wmv"));

	//�^�C�g��
	title = GraphicsDevice.CreateSpriteFromFile(_T("bg/title.png")); 

	//����
	description = GraphicsDevice.CreateSpriteFromFile(_T("bg/setumei.png")); 

	//�Z���N�g
	select_bg = GraphicsDevice.CreateSpriteFromFile(_T("bg/select.png"));
	stage_button[0] = GraphicsDevice.CreateSpriteFromFile(_T("UI/stage01.png"));
	stage_button[1] = GraphicsDevice.CreateSpriteFromFile(_T("UI/stage02.png"));
	stage_button[2] = GraphicsDevice.CreateSpriteFromFile(_T("UI/stage03.png"));

	//�N���A
	clear = GraphicsDevice.CreateSpriteFromFile(_T("bg/clear.png")); 

	//�I�[�o�[
	over = GraphicsDevice.CreateSpriteFromFile(_T("bg/over.png")); 

	//�t�F�[�h
	fade = GraphicsDevice.CreateSpriteFromFile(_T("bg/fade.png")); 

	time_number = GraphicsDevice.CreateSpriteFromFile(_T("UI/number.png")); // �ԍ�
	
	//�g�����X�Q�[�W
	trans_frame = GraphicsDevice.CreateSpriteFromFile(_T("UI/waku_trans.png")); //�g
	trans_gage = GraphicsDevice.CreateSpriteFromFile(_T("UI/trans.png")); //�Q�[�W

	select_cursor = GraphicsDevice.CreateSpriteFromFile(_T("UI/yajirusi.png"));//�Z���N�g�J�[�\��

	warning = GraphicsDevice.CreateSpriteFromFile(_T("UI/blip.png"));//�x��

	//���y
	bgm_ = SoundDevice.CreateMusicFromFile(_T("music/BGM.wav"));
	bgm2 = SoundDevice.CreateMusicFromFile(_T("music/BGM2.wav"));
	bgm3 = SoundDevice.CreateMusicFromFile(_T("music/BGM3.wav"));
	select_bgm = SoundDevice.CreateMusicFromFile(_T("music/select.wav"));
	engine_se = SoundDevice.CreateSoundFromFile(_T("music/engine.wav"));
	se_ = SoundDevice.CreateSoundFromFile(_T("music/SE_01.wav"));
	select_se = SoundDevice.CreateSoundFromFile(_T("music/cancel.wav"));
	BD_se = SoundDevice.CreateSoundFromFile(_T("music/BD.wav"));
	damege_se = SoundDevice.CreateSoundFromFile(_T("music/damege.wav"));
	dead_se = SoundDevice.CreateSoundFromFile(_T("music/bom.wav"));
	missile_se = SoundDevice.CreateSoundFromFile(_T("music/misile.wav"));
	flash_se = SoundDevice.CreateSoundFromFile(_T("music/flash.wav"));
	up_se = SoundDevice.CreateSoundFromFile(_T("music/up.wav"));
	roll_se = SoundDevice.CreateSoundFromFile(_T("music/roll.wav"));

	player = GraphicsDevice.CreateAnimationModelFromFile(_T("f15C2/left180.X")); 

	Player = GraphicsDevice.CreateAnimationModelFromFile(_T("f15C2/F15C3.X")); 
	
	player_flashing = 0;

	Player->SetScale(0.09f); 
	player->SetScale(0.09f);
	player_obb = Player->GetOBB();
	player_obb.Radius.x *= 0.33333f;
	player_obb.Radius.y *= 0.33333f;
	player_obb.Radius.z *= 0.33333f;
	
	//�v���C���[�̓����蔻��
	SimpleShape shape;
	shape.Type = Shape_Box;
	shape.Width = player_obb.Radius[0] * 2;//��
	shape.Height = player_obb.Radius[1] * 2;//����
	shape.Depth = player_obb.Radius[2] * 2;//���s��

	player_collision = GraphicsDevice.CreateModelFromSimpleShape(shape);

	Material  mtrl4(player_collision);
	mtrl4.Emissive = Color(1.0f, 1.0f, 1.0f);//�G�~�V�b�u�ݒ�
	player_collision->SetMaterial(mtrl4);


	//�G�l�~�[
	enemy = GraphicsDevice.CreateModelFromFile(_T("enemy/mikoto.X"));
	
	enemy->SetScale(0.5);
	enemy->SetPosition(600, 0, 2000);//�G�l�~�[�̏������W
	enemy->SetPosition(-600, 0, 2000);
	enemy_state = 0;

	enemy_obb = enemy->GetOBB();

	//�G�l�~�[�̓����蔻��
	SimpleShape shape_E;
	shape_E.Type = Shape_Box;
	shape_E.Width = enemy_obb.Radius[0] * 2;//��
	shape_E.Height = enemy_obb.Radius[1] * 2;//����
	shape_E.Depth = enemy_obb.Radius[2] * 2;//���s��
	enemy_collsion = GraphicsDevice.CreateModelFromSimpleShape(shape_E);

	Material  mtrl_E(enemy_collsion);
	mtrl_E.Emissive = Color(1.0f, 1.0f, 1.0f);//�G�~�V�b�u�ݒ�
	enemy_collsion->SetMaterial(mtrl_E);//���̐F


	//�~�T�C��
	GraphicsDevice.CreateSharedModelFromFile(_T("AIM120D_Missile/Missile.X"), enemy_missile, MISSILE_MAX);

	//�~�T�C���̓����蔻��
	missile_obb[0] = enemy_missile[0]->GetOBB();
	missile_obb[0].Radius[0] *= 1.0f;//��
	missile_obb[0].Radius[1] *= 1.0f;//����
	missile_obb[0].Radius[2] *= 1.0f;//���s��

	for (int i = 0; i < MISSILE_MAX;i++)
	{
		//enemy_shot_state_[i] = 0;
		//�G�l�~�[�̒e
		//enemy_missile[i]->SetTexture(_T("AIM120D Missile/AIM120-1.png"));
		missile_obb[i] = missile_obb[0];

		SimpleShape shape;
		shape.Type = Shape_Box;
		shape.Width = missile_obb[i].Radius[0] * 2;//��
		shape.Height = missile_obb[i].Radius[1] * 2;//����
		shape.Depth = missile_obb[i].Radius[2] * 2;//���s��
		missile_collision[i] = GraphicsDevice.CreateModelFromSimpleShape(shape);

		Material  mtrl5(missile_collision[i]);
		mtrl5.Emissive = Color(1.0f, 1.0f, 1.0f);//�G�~�V�b�u�ݒ�
		missile_collision[i]->SetMaterial(mtrl5);
	}

	//��
	GraphicsDevice.CreateSharedModelFromFile(_T("smoke/kemuri.X"), smoke, 100);
	SPRITE smoke_texture = GraphicsDevice.CreateSpriteFromFile(_T("smoke/smoke2.png"));

	for (int j = 0; j < SMOKE_MAX;j++)
	{
		smoke[j]->SetTexture(*smoke_texture);
		smoke[j]->SetScale(0.8);
		Material  mtrl2(smoke[j]);
		mtrl2.Emissive = Color(1.0f, 1.0f, 1.0f);//�G�~�V�b�u�ݒ�
		smoke[j]->SetMaterial(mtrl2);//���̐F			
	}

	// �G  0�`2:�퓬�@�@3�`4�A6�`7:�G�l�~�[  5:���
	for (int i = 0; i <= 2; i++)
	{
		teki[i] = GraphicsDevice.CreateModelFromFile(_T("enemy/enemy.X"));//�퓬�@
		teki[i]->SetScale(0.03);
		
	}

	for (int i = 3; i <= 4;i++)
	{
		teki[i] = GraphicsDevice.CreateModelFromFile(_T("enemy/mikoto.x"));//�G�l�~�[
		teki[i]->SetScale(0.08);
	}

	teki[5] = GraphicsDevice.CreateModelFromFile(_T("enemy_ship/enemysip.X"));//���
	teki[5]->SetScale(0.5);

	for (int i = 6; i <= 7; i++)
	{
		teki[i] = GraphicsDevice.CreateModelFromFile(_T("enemy/mikoto.x"));//�G�l�~�[
		teki[i]->SetScale(0.08);
	}

	teki[8] = GraphicsDevice.CreateModelFromFile(_T("enemy_ship/enemysip.X"));//���
	teki[8]->SetScale(1.0);

	for (int i = 0; i < TEKI_MAX;i++)
	{
		Material ml(teki[i]);
		ml.Emissive = Color(0.0f, 0.0f, 0.0f);
		ml.Diffuse = Color(0.5f, 0.5f, 0.5f);
		ml.Ambient = Color(0.25f, 0.25f, 0.25f);
		teki[i]->SetMaterial(ml);

		teki_obb[i] = teki[i]->GetOBB();
		SimpleShape shape_T1;
		shape_T1.Type = Shape_Box;
		shape_T1.Width = teki_obb[i].Radius[0] * 2;//��
		shape_T1.Height = teki_obb[i].Radius[1] * 2;//����
		shape_T1.Depth = teki_obb[i].Radius[2] * 2;//���s��
		teki_collsion[i] = GraphicsDevice.CreateModelFromSimpleShape(shape_T1);

		Material  mtrl4(teki_collsion[i]);
		mtrl4.Emissive = Color(1.0f, 1.0f, 1.0f);//�G�~�V�b�u�ݒ�
		teki_collsion[i]->SetMaterial(mtrl4);
	}

	//�r�[��
	GraphicsDevice.CreateSharedModelFromFile(_T("shot/shot.x"), beam_, 16);
	SPRITE Tamatexture = GraphicsDevice.CreateSpriteFromFile(_T("shot/main_shot.png"));

	for (int i = 0; i < TEKI_POS_MAX; i++)
	{
		beam_[i]->SetTexture(*Tamatexture);
		beam_[i]->SetScale(0.2);

		beam_obb[i] = beam_[i]->GetOBB();
		beam_obb[i].Radius.y *= 10;
		SimpleShape shape_TB;
		shape_TB.Type = Shape_Box;
		shape_TB.Width = beam_obb[i].Radius[0] * 2;//��
		shape_TB.Height = beam_obb[i].Radius[1] * 2;//����
		shape_TB.Depth = beam_obb[i].Radius[2] * 2;//���s��
		beam_collsion[i] = GraphicsDevice.CreateModelFromSimpleShape(shape_TB);

		Material  mtrl_TB(beam_collsion[i]);
		mtrl_TB.Emissive = Color(1.0f, 1.0f, 1.0f);
		beam_collsion[i]->SetMaterial(mtrl_TB);
	}

	//����
	GraphicsDevice.CreateSharedModelFromFile(_T("bom/bom.X"), bom, 30);
	SPRITE Bomtexture = GraphicsDevice.CreateSpriteFromFile(_T("bom/bom.png"));

	for (int i = 0; i < MISSILE_MAX;i++)
	{
		bom[i]->SetTexture(*Bomtexture);
		Material  mtrl_B(bom[i]);
		mtrl_B.Emissive = Color(1.0f, 1.0f, 1.0f);//�G�~�V�b�u�ݒ�
		bom[i]->SetMaterial(mtrl_B);
		bom_scale[i] = 1.2f;
		bom_con[i] = 0;
		bom_state[i] = 0;
	}

	//HP
	Ggauge::LoadGraphic();
	hp.Initialize();

	// ����������
	random = std::mt19937(seed());
	// X��100.0f�ȏ�300.0f�ȉ�����l�𓙊m���Ŕ���������
	dist = uniform_real_distribution<float>(-50.0f, 50.0f);

	//�t�H���g�T�C�Y�ݒ�
	Big_Font = GraphicsDevice.CreateSpriteFont(_T("Caladea"), 35);

	//�Q�[���p�b�h������
	InputDevice.CreateGamePad(1);

	{
		using namespace Fujimura;

		ReadFireTimeMap::missileInstanceMethod_ = SEE_CLASS_MEMBERS NEED_ARGUMENT
		{
			for (int i = 0; i < MISSILE_MAX;i++)
			{
				//���g�p�̃~�T�C������������
				if (enemy_shot_state[i] == 0 && smoke_losttime >= 0)
				{
					//����������
					this->SetBezierToEnemyMissile(i, MathHelper_Random(-20, 20), MathHelper_Random(-3, 1),MathHelper_Random(-2,2));
					//�g�p�i2�j�ɂ���
					enemy_shot_state[i] = 2;
					smoke_losttime[i] = 1.0f;
					return;
				}
			}
		};
		this->playerAnimeManager_.SetModel(this->player);
		FujiFilm::TransGauge::LoadResource();
	}

	return true;
}

/// <summary>
/// Finalize will be called once per game and is the place to release
/// all resource.
/// </summary>

void GameMain::Finalize()
{
	// TODO: Add your finalization logic here
	
	
}

/// <summary>
/// Allows the game to run logic such as updating the world,
/// checking for collisions, gathering input, and playing audio.
/// </summary>
/// <returns>
/// Scene continued value.
/// </returns>

//����0
int GameMain::Update()
{
	// TODO: Add your update logic here

	//�L�[�{�[�h���p�b�h�錾
	key_state = Keyboard->GetState();
	key_buffer = Keyboard->GetBuffer();
	pad_state = GamePad(0)->GetState();
	pad_buffer = GamePad(0)->GetBuffer();

	/*�^�C�g��*/
	if (game_seane == TITLE_SEANE && game_seane != TUTORIAL && game_seane != STAGE1 && game_seane != STAGE2) 
	{
		//���掞�Ԃ̉��Z
		game_play_movie_time++;

		//������ʂւ̑J��
		if (key_buffer.IsPressed(Keys_Enter) || pad_buffer.IsPressed(GamePad_Button10) || pad_buffer.IsPressed(GamePad_Button1) || pad_buffer.IsPressed(GamePad_Button2)
			|| pad_buffer.IsPressed(GamePad_Button3) || pad_buffer.IsPressed(GamePad_Button4) /*||pad_buffer_.IsPressed(GamePad_Button8)*/) {
			//game_scene_ = 1;
			All_Initialize();
			
		}

	}

	//����Đ��֘A
	if (game_play_movie_state == 0 && game_seane == TITLE_SEANE && game_seane != TUTORIAL && game_seane != STAGE1 && game_seane != STAGE2) {
		if (game_play_movie_time >= 300) {
			game_play_movie_state = 1;
			game_play_movie->Play();
			game_play_movie->Replay();
		}
	}

	if (game_play_movie_state == 1 && game_seane == TITLE_SEANE && game_seane != TUTORIAL && game_seane != STAGE1 && game_seane != STAGE2) {
		if (pad_buffer.IsPressed(GamePad_Button1) || pad_buffer.IsPressed(GamePad_Button2)
			|| pad_buffer.IsPressed(GamePad_Button3) || pad_buffer.IsPressed(GamePad_Button4)
			|| key_buffer.IsPressed(Keys_Enter) || pad_buffer.IsPressed(GamePad_Button10)
			|| game_play_movie->IsComplete()) {
			//game_seane = Title_Seane;
			game_play_movie->Replay();
		}
	}
	
	//�^�C�g���̏���
	if (game_seane == TITLE_SEANE)
	{

		Title_Update();

		if (pad_buffer.IsPressed(GamePad_Button1)
			|| pad_buffer.IsPressed(GamePad_Button2)
			|| pad_buffer.IsPressed(GamePad_Button3)
			|| pad_buffer.IsPressed(GamePad_Button4)
			|| pad_buffer.IsPressed(GamePad_Button10)
			|| key_buffer.IsPressed(Keys_Enter))
		{
		
			game_seane = SETUMEI;
		}

		//if (fade_out_state == 1)
		//{
		//	faded_out += 0.01f;
		//	if (faded_out >= 1.0)
		//	{
		//		fade_out_state = 2;
		//		faded_out = 1.0f;
		//		select_bgm->Play();
		//		game_seane = 1;
		//	}
		//}
	}

	else if (game_seane == SETUMEI)
	{
		if (pad_buffer.IsPressed(GamePad_Button1)
			|| pad_buffer.IsPressed(GamePad_Button2)
			|| pad_buffer.IsPressed(GamePad_Button3)
			|| pad_buffer.IsPressed(GamePad_Button4)
			|| pad_buffer.IsPressed(GamePad_Button10)
			|| key_buffer.IsPressed(Keys_Enter))
		{
			select_bgm->Play();
			se_->Play();
			game_seane = SELECT_SEANE;
			All_Initialize();
			select_move_state = 0;
		}
	}


	else if (game_seane == SELECT_SEANE)
	{
		Select_Update();
		//bgm_->Stop();

		if (pad_buffer.IsPressed(GamePad_Button1)
			|| pad_buffer.IsPressed(GamePad_Button2)
			|| pad_buffer.IsPressed(GamePad_Button3)
			|| pad_buffer.IsPressed(GamePad_Button4)
			|| pad_buffer.IsPressed(GamePad_Button10)
			|| key_buffer.IsPressed(Keys_Enter))
		{
			//AllInitialize();
			select_bgm->Stop();
			select_se->Play();
			if (a == 0)
			{
				bgm_->Play();
			}
			if (a == 1)
			{
				bgm2->Play();
			}
			if (a == 2)
			{
				bgm3->Play();
			}
			engine_se->PlayLooping();

		}

		/*	if (fade_out_state == 2)
		{
		faded_out -= 0.01f;
		if (faded_out <= 0.0)
		{
		faded_out = 0.0f;
		if (pad_buffer.IsPressed(GamePad_Button1)
		|| pad_buffer.IsPressed(GamePad_Button2)
		|| pad_buffer.IsPressed(GamePad_Button3)
		|| pad_buffer.IsPressed(GamePad_Button4)
		|| pad_buffer.IsPressed(GamePad_Button10)
		|| key_buffer.IsPressed(Keys_Enter))
		{
		select_bgm->Stop();
		select_se->Play();
		fade_out_state = 3;
		}
		}
		}*/

		/*	if (fade_out_state == 3)
		{
		faded_out += 0.01f;
		if (faded_out >= 1.0)
		{
		faded_out = 1.0f;
		game_seane = 2;
		fade_out_state = 4;
		bgm_->Stop();
		bgm_->Play();
		engine_se->PlayLooping();
		}
		}*/

	}

	//	if (fade_out_state == 5)
	//	{
	//		faded_out += 0.01f;
	//		if (faded_out >= 1.0)
	//		{
	//			faded_out = 1.0f;
	//			fade_out_state = 6;
	//			game_seane = 4;
	//		}
	//	}
	//	if (fade_out_state == 7)
	//	{
	//		faded_out += 0.01f;
	//		if (faded_out >= 1.0)
	//		{
	//			faded_out = 1.0f;
	//			fade_out_state = 8;
	//			game_seane = 3;
	//		}
	//	}
	//}


	else if (game_seane == TUTORIAL)
	{

		//			if (fade_out_state == 4)
		//			{
		//				faded_out -= 0.01f;
		//				if (faded_out <= 0.0)
		//				{
		//					faded_out = 0.0f;
		if (hp_gauge <= 0.0)
		{
			dead_se->Play();
			dead_se->IsPlaying();
			bgm_->Stop();
			bgm2->Stop();
			bgm3->Stop();
			game_seane = OVER_SEANE;
			//						fade_out_state = 5;
		}
		if (time_state == 1)
		{
			game_seane = CLEAR_SEANE;

			//						fade_out_state = 7;
		}

		//				}

		//			}

		{
			using namespace Fujimura;
			METHOD_TIMER.Update();
		}

		Bg_Update();
		Time_Update();
		Player_Update();
		Sclorl_Update();
		Enemy_Update();
		this->Missile_Update();
		Tutorial_Update();
		Enemy_Shot_Update();
		UpdateCollisionToHittingMissile();
		
		GraphicsDevice.SetCamera(camera); //�J�����̎���

		/*if (fade_out_state == 5)
		{
			faded_out += 0.01f;
			if (faded_out >= 1.0)
			{
				faded_out = 1.0f;
				fade_out_state = 6;
				game_seane = 4;
			}
		}

		if (fade_out_state == 7)
		{
			faded_out += 0.01f;
			if (faded_out >= 1.0)
			{
				faded_out = 1.0f;
				fade_out_state = 8;
				game_seane = 3;
			}
		}*/


	}

	else if (game_seane == STAGE1)
	{

		if (hp_gauge <= 0.0)
		{
			dead_se->Play();
			dead_se->IsPlaying();
			game_seane = OVER_SEANE;
			//fade_out_state = 5;
		}
		if (time_state == 1)
		{

			game_seane = CLEAR_SEANE;

			//fade_out_state = 7;
		}

		{
			using namespace Fujimura;
			METHOD_TIMER.Update();
		}

		Bg_Update();
		Time_Update();
		Player_Update();
		Sclorl_Update();
		Enemy_Update();
		Stage1_Update();
		Enemy_Shot_Update();
		this->Missile_Update();

		UpdateCollisionToHittingMissile();

		GraphicsDevice.SetCamera(camera); //�J�����̎���

	}

	else if (game_seane == STAGE2)
	{

		if (hp_gauge <= 0.0)
		{
			dead_se->Play();
			dead_se->IsPlaying();
			game_seane = OVER_SEANE;
			//fade_out_state = 5;
		}
		if (time_state == 1)
		{
			game_seane = CLEAR_SEANE;//�N���A3
			scene_count = 1;
			//fade_out_state = 7;
		}

		{
			using namespace Fujimura;
			METHOD_TIMER.Update();

		}

		Bg_Update(); //�w�i����
		Time_Update(); //���ԏ���
		Player_Update(); //�����̏���
		Sclorl_Update();//�X�N���[���̏���
		Enemy_Update();
		Stage2_Update();
		Enemy_Shot_Update();
		this->Missile_Update();

		UpdateCollisionToHittingMissile();

		GraphicsDevice.SetCamera(camera); //�J�����̎���

	}

	//�N���A�̏���
	else if (game_seane == CLEAR_SEANE)
	{
		//if (fade_out_state == 8)
		//{
		//	faded_out -= 0.01f;
		//	if (faded_out <= 0.0)
		//	{
		//		faded_out = 0.0f;
		//		fade_out_state = 0;
		//	}
		//}

		All_Initialize();
		Clear_Update(); //�N���A����
		GraphicsDevice.SetCamera(camera); //�J�����̎���
	}

	//�I�[�o�[�̏���
	else if (game_seane == OVER_SEANE)
	{
		//if (fade_out_state == 6)
		//{
		//	faded_out -= 0.01f;
		//	if (faded_out <= 0.0)
		//	{
		//		faded_out = 0.0f;
		//		fade_out_state = 0;
		//	}
		//}

		Over_Update(); //�I�[�o�[����
		GraphicsDevice.SetCamera(camera); //�J�����̎���
	}

	return 0;
}

/// <summary>
/// This is called when the game should draw itself.
/// </summary>

//�`��
void GameMain::Draw()
{
	// TODO: Add your drawing code here
	GraphicsDevice.Clear(Color_AliceBlue);

	GraphicsDevice.BeginScene();

	if (game_seane == TUTORIAL)
	{
		GraphicsDevice.SetRenderState(Fog_Enable); //�t�H�O��L���ɂ���

		hp.DrawGauge(hp_gauge);
		hp.SetPosition(hp_pos);

		for (int i = 0; i < SCLORL_MAX; i++)
		{
			Vector3   gpos = cloud[i]->GetPosition();
			Vector3   gpos11 = BG_[i]->GetPosition();
			Vector3   gpos21 = BG_2[i]->GetPosition();
	
			BG_2[i]->SetPosition(gpos21);
			BG_2[i]->Draw();

			BG_[i]->SetPosition(gpos11);
			BG_[i]->Draw();

			for (int j = 0; j < G_POS_MAX;j++)
			{
				gpos.x = -8000.0f + j * 400;
				cloud[i]->SetPosition(gpos);
				cloud[i]->Draw();
			}
		}

		//�X�J�C�h�[���`��
		GraphicsDevice.SetRenderState(Fog_Disable); //�t�H�O�𖳌��ɂ���
		skydome->Draw(); //�X�J�C�h�[��

						  //�G�l�~�[�̒e
		for (int i = 0; i < MISSILE_MAX; i++)
		{
			if (enemy_state == 2)
			{
				if (enemy_shot_state[i] == 2)
				{
					// �~�T�C���`��
					enemy_missile[i]->Draw();
					enemy_missile[i]->SetScale(0.6);
					missile_collision[i]->SetPosition(missile_obb[i].Center);
					missile_collision[i]->SetDirection(missile_qt[i]);
					//					Hako2[i]->Draw();

					//�e�̐���
					GraphicsDevice.BeginShadowRendering();

					//��]�s��̐ݒ�
					Matrix matrix_;

					//���C�g�^�C�v�A���C�g�̈ʒu�A�ǂ����Ƃ炷���A�e�̈ʒu
					matrix_ = Matrix_CreateShadow(Light_Directional, Vector3(0.2f, -1.0f, shadow_z), enemy_missile[i]->GetPosition(),
						Vector3(0.0f, -80.0f, 0.0f), Vector3_Up);

					//���
					for (int t = 0; t < 10; t++)
					{
						//�}�N���@=�@�ł����l�͌����Ă܂�
						float distance = FLT_MAX;
						//���C�@�O���E���h����v���C���[��
						cloud[t]->IntersectRay(enemy_missile[i]->GetPosition(), Vector3_Down, &distance);
						//�e�`��
						enemy_missile[i]->DrawShadow(matrix_, shadow_deps);
					}
					GraphicsDevice.EndShadowRendering();
					missile_collision[i]->SetPosition(missile_obb[i].Center);
					missile_collision[i]->SetDirection(missile_qt[i]);
				}
			}
		}

		Enemy_Draw(); //�G�̕`��

		Player_Draw(); //�����̕`��

		GraphicsDevice.BeginAlphaBlend();

		//���������Ȃ��悤�ɂ���
		GraphicsDevice.SetRenderState(DepthBuffer_Disable);
		GraphicsDevice.SetRenderState(DepthBufferWrite_Disable);

		//�`��@���Z
		GraphicsDevice.SetBlendMode(BlendMode_Add);

		Enemy_Shot_Draw();

		for (int i = 0; i < MISSILE_MAX; i++)
		{
			if (bom_state[i] != 0) 
			{
				bom[i]->SetPosition(bom_position[i]);
				bom[i]->SetScale(bom_scale[i]);
				bom[i]->DrawAlpha(bom_con[i], 12);
			}
		}

		GraphicsDevice.SetBlendMode(BlendMode_Default);

		for (int i = 0;i < MISSILE_MAX;i++) 
		{
			Vector3  mis_front_vec = enemy_missile[i]->GetFrontVector();
			
			if (bezier_t[i] >= 1 || hp_scene >= 1 || enemy_shot_state[i] == 0) 
			{
				smoke_losttime[i] -= 0.025;
			}

			for (int j = 0; j < bezier_t[i] * SMOKE_MAX; j++)
			{
				bezier_position = Vector3_Bezier(bezier_storege[i],
					bezier_storege2[i],
					bezier_storege3[i],
					player_get_position[i], j / 100.0f);

				bezier_position.y += 5;
				bezier_position.x -= 5;

				smoke[i]->SetScale(0.6);
				smoke[i]->SetPosition(bezier_position - mis_front_vec * 45);
				smoke[i]->DrawAlpha(j / 100.0f * smoke_losttime[i], 12);
			}

		}

		GraphicsDevice.SetRenderState(DepthBuffer_Enable);
		GraphicsDevice.SetRenderState(DepthBufferWrite_Enable);


		if (this->playerAnimeManager_.isPlaying())
		{
			player->SetPosition(player_x, player_y, player_z);
			player->SetRotation(player_r_x, player_r_y, player_r_z);
			player->SetScale(0.09f);
			player->AdvanceTime(0.02f);
			player->Draw();
			GraphicsDevice.SetBlendMode(BlendMode_Add);
			player->DrawAlpha(0.0f, DrawAlpha_BothFaces);
			GraphicsDevice.SetRenderState(CullMode_CullCounterClockwiseFace);
		}


		GraphicsDevice.EndAlphaBlend();

		//�x��
		Vector3 warning_pos[MISSILE_MAX];
		for (int i = 0; i < MISSILE_MAX; i++) 
		{
			warning_pos[i] = GraphicsDevice.WorldToScreen(enemy_missile[i]->GetPosition());
			warning_pos[i].x -= 50.0f;
			warning_pos[i].y -= 50.0f;
			warning_pos[i].z = -50.0f;
			if (warning_pos[i].y <= 50.0)
			{
				warning_pos[i].y = 50.0f;
			}
			if (warning_pos[i].y >= 620.0)
			{
				warning_pos[i].y = 620.0f;
			}
			if (warning_pos[i].x <= 50.0)
			{
				warning_pos[i].x = 50.0f;
			}
			if (warning_pos[i].x >= 1180.0)
			{
				warning_pos[i].x = 1180.0f;
			}
		}

	}


	if (game_seane == STAGE1)
	{
		GraphicsDevice.SetRenderState(Fog_Enable); //�t�H�O��L���ɂ���

		hp.DrawGauge(hp_gauge);
		hp.SetPosition(hp_pos);

		for (int i = 0; i < SCLORL_MAX; i++)
		{
			Vector3   gpos = cloud[i]->GetPosition();
			Vector3   gpos11 = BG_[i]->GetPosition();
			Vector3   gpos21 = BG_2[i]->GetPosition();
	
			BG_2[i]->SetPosition(gpos21);
			BG_2[i]->Draw();

			BG_[i]->SetPosition(gpos11);
			BG_[i]->Draw();

			for (int j = 0; j < G_POS_MAX;j++)
			{
				gpos.x = -8000.0f + j * 400;
				cloud[i]->SetPosition(gpos);
				cloud[i]->Draw();
			}
		}

		//�X�J�C�h�[���`��
		GraphicsDevice.SetRenderState(Fog_Disable); //�t�H�O�𖳌��ɂ���
		skydome->Draw(); //�X�J�C�h�[��

		//�G�l�~�[�̒e
		for (int i = 0; i < MISSILE_MAX; i++)
		{
			if (enemy_state == 2)
			{
				if (enemy_shot_state[i] == 2)
				{
					// �~�T�C���`��
					enemy_missile[i]->Draw();
					enemy_missile[i]->SetScale(0.6);
					missile_collision[i]->SetPosition(missile_obb[i].Center);
					missile_collision[i]->SetDirection(missile_qt[i]);
					//Hako2[i]->Draw();

					//�e�̐���
					GraphicsDevice.BeginShadowRendering();

					//��]�s��̐ݒ�
					Matrix matrix_;

					//���C�g�^�C�v�A���C�g�̈ʒu�A�ǂ����Ƃ炷���A�e�̈ʒu
					matrix_ = Matrix_CreateShadow(Light_Directional, Vector3(0.2f, -1.0f, shadow_z), enemy_missile[i]->GetPosition(),
						Vector3(0.0f, -80.0f, 0.0f), Vector3_Up);

					//���
					for (int t = 0; t < 10; t++)
					{
						//�}�N���@=�@�ł����l�͌����Ă܂�
						float distance = FLT_MAX;
						//���C�@�O���E���h����v���C���[��
						cloud[t]->IntersectRay(enemy_missile[i]->GetPosition(), Vector3_Down, &distance);
						//�e�`��
						enemy_missile[i]->DrawShadow(matrix_, shadow_deps);
					}
					GraphicsDevice.EndShadowRendering();
					missile_collision[i]->SetPosition(missile_obb[i].Center);
					missile_collision[i]->SetDirection(missile_qt[i]);
				}
			}
		}

		Enemy_Draw(); //�G�̕`��

		Player_Draw(); //�����̕`��

		GraphicsDevice.BeginAlphaBlend();

		//���������Ȃ��悤�ɂ���
		GraphicsDevice.SetRenderState(DepthBuffer_Disable);
		GraphicsDevice.SetRenderState(DepthBufferWrite_Disable);

		//�`��@���Z
		GraphicsDevice.SetBlendMode(BlendMode_Add);

		Enemy_Shot_Draw();

		//����
		for (int i = 0; i < MISSILE_MAX; i++)
		{
			if (bom_state[i] != 0)
			{
				bom[i]->SetPosition(bom_position[i]);
				bom[i]->SetScale(bom_scale[i]);
				bom[i]->DrawAlpha(bom_con[i], 12);
			}
		}

		GraphicsDevice.SetBlendMode(BlendMode_Default);

		for (int i = 0;i < MISSILE_MAX;i++) 
		{
			Vector3  mis_front_vec = enemy_missile[i]->GetFrontVector();
			if (bezier_t[i] >= 1 || hp_scene >= 1 || enemy_shot_state[i] == 0) 
			{
				smoke_losttime[i] -= 0.025;
			}
			for (int j = 0;j < bezier_t[i] * 100;j++)
			{
				bezier_position = Vector3_Bezier(bezier_storege[i],
					bezier_storege2[i],
					bezier_storege3[i],
					player_get_position[i], j / 100.0f);

				bezier_position.y += 5;
				bezier_position.x -= 5;
				smoke[i]->SetScale(0.6);
				smoke[i]->SetPosition(bezier_position - mis_front_vec * 45);
				smoke[i]->DrawAlpha(j / 100.0f * smoke_losttime[i], 12);
			}
		}

		GraphicsDevice.SetRenderState(DepthBuffer_Enable);
		GraphicsDevice.SetRenderState(DepthBufferWrite_Enable);

		if (this->playerAnimeManager_.isPlaying())
		{
			player->SetPosition(player_x, player_y, player_z);
			player->SetRotation(player_r_x, player_r_y, player_r_z);
			player->SetScale(0.09f);
			player->AdvanceTime(0.02f);
			player->Draw();
			GraphicsDevice.SetBlendMode(BlendMode_Add);
			player->DrawAlpha(0.0f, DrawAlpha_BothFaces);
			GraphicsDevice.SetRenderState(CullMode_CullCounterClockwiseFace);
		}

		GraphicsDevice.EndAlphaBlend();

		Vector3 warning_pos[MISSILE_MAX];
		for (int i = 0; i < MISSILE_MAX; i++) {
			warning_pos[i] = GraphicsDevice.WorldToScreen(enemy_missile[i]->GetPosition());
			warning_pos[i].x -= 50.0f;
			warning_pos[i].y -= 50.0f;
			warning_pos[i].z = -50.0f;
			if (warning_pos[i].y <= 50.0)
			{
				warning_pos[i].y = 50.0f;
			}
			if (warning_pos[i].y >= 620.0)
			{
				warning_pos[i].y = 620.0f;
			}
			if (warning_pos[i].x <= 50.0)
			{
				warning_pos[i].x = 50.0f;
			}
			if (warning_pos[i].x >= 1180.0)
			{
				warning_pos[i].x = 1180.0f;
			}
		}
	}

	if (game_seane == STAGE2)
	{

		GraphicsDevice.SetRenderState(Fog_Enable); //�t�H�O��L���ɂ���

		hp.DrawGauge(hp_gauge);
		hp.SetPosition(hp_pos);


		for (int i = 0; i < SCLORL_MAX; i++)
		{
			Vector3   gpos = cloud[i]->GetPosition();
			Vector3   gpos11 = BG_[i]->GetPosition();
			Vector3   gpos21 = BG_2[i]->GetPosition();
			
			BG_2[i]->SetPosition(gpos21);
			BG_2[i]->Draw();

			BG_[i]->SetPosition(gpos11);
			BG_[i]->Draw();

			for (int j = 0; j < G_POS_MAX;j++)
			{
				gpos.x = -8000.0f + j * 400;
				cloud[i]->SetPosition(gpos);
				cloud[i]->Draw();
			}
		}

		//�X�J�C�h�[���`��
		GraphicsDevice.SetRenderState(Fog_Disable); //�t�H�O�𖳌��ɂ���
		skydome->Draw(); //�X�J�C�h�[��

		//�G�l�~�[�̒e
		for (int i = 0; i < MISSILE_MAX; i++)
		{
			if (enemy_state == 2)
			{
				if (enemy_shot_state[i] == 2)
				{
					// �~�T�C���`��
					enemy_missile[i]->Draw();
					enemy_missile[i]->SetScale(0.6);
					missile_collision[i]->SetPosition(missile_obb[i].Center);
					missile_collision[i]->SetDirection(missile_qt[i]);
					//					Hako2[i]->Draw();

					//�e�̐���
					GraphicsDevice.BeginShadowRendering();

					//��]�s��̐ݒ�
					Matrix matrix_;

					//���C�g�^�C�v�A���C�g�̈ʒu�A�ǂ����Ƃ炷���A�e�̈ʒu
					matrix_ = Matrix_CreateShadow(Light_Directional, Vector3(0.2f, -1.0f, shadow_z), enemy_missile[i]->GetPosition(),
						Vector3(0.0f, -80.0f, 0.0f), Vector3_Up);

					//���
					for (int t = 0; t < 10; t++)
					{
						//�}�N���@=�@�ł����l�͌����Ă܂�
						float distance = FLT_MAX;
						//���C�@�O���E���h����v���C���[��
						cloud[t]->IntersectRay(enemy_missile[i]->GetPosition(), Vector3_Down, &distance);
						//�e�`��
						enemy_missile[i]->DrawShadow(matrix_, shadow_deps);
					}
					GraphicsDevice.EndShadowRendering();
					missile_collision[i]->SetPosition(missile_obb[i].Center);
					missile_collision[i]->SetDirection(missile_qt[i]);
				}
			}
		}

		Enemy_Draw(); //�G�̕`��

		Player_Draw(); //�����̕`��

		GraphicsDevice.BeginAlphaBlend();

		//���������Ȃ��悤�ɂ���
		GraphicsDevice.SetRenderState(DepthBuffer_Disable);
		GraphicsDevice.SetRenderState(DepthBufferWrite_Disable);

		//�`��@���Z
		GraphicsDevice.SetBlendMode(BlendMode_Add);

		Enemy_Shot_Draw();

		for (int i = 0; i < MISSILE_MAX; i++) {
			if (bom_state[i] != 0) {
				bom[i]->SetPosition(bom_position[i]);
				bom[i]->SetScale(bom_scale[i]);
				bom[i]->DrawAlpha(bom_con[i], 12);
			}
		}

		GraphicsDevice.SetBlendMode(BlendMode_Default);

		for (int i = 0;i < MISSILE_MAX;i++) {
			Vector3  mis_front_vec = enemy_missile[i]->GetFrontVector();
			//�~�T�C�������˂���ĂȂ�������
			//if (enemy_shot_state_[i] >= 0) {
			//		if (enemy_shot_state_[i] != 100) {
			if (bezier_t[i] >= 1 || hp_scene >= 1 || enemy_shot_state[i] == 0) {
				smoke_losttime[i] -= 0.025;
			}
			for (int j = 0;j < bezier_t[i] * SMOKE_MAX;j++) {
				bezier_position = Vector3_Bezier(bezier_storege[i],
					bezier_storege2[i],
					bezier_storege3[i],
					player_get_position[i], j / 100.0f);

				bezier_position.y += 5;
				bezier_position.x -= 5;
				smoke[i]->SetScale(0.6);
				smoke[i]->SetPosition(bezier_position - mis_front_vec * 45);
				smoke[i]->DrawAlpha(j / 100.0f * smoke_losttime[i], 12);
			}
			//		}
			//}
		}

		GraphicsDevice.SetRenderState(DepthBuffer_Enable);
		GraphicsDevice.SetRenderState(DepthBufferWrite_Enable);

		if (this->playerAnimeManager_.isPlaying())
		{
			player->SetPosition(player_x, player_y, player_z);
			player->SetRotation(player_r_x, player_r_y, player_r_z);
			player->SetScale(0.09f);
			player->AdvanceTime(0.02f);
			player->Draw();
			GraphicsDevice.SetBlendMode(BlendMode_Add);
			player->DrawAlpha(0.0f, DrawAlpha_BothFaces);
			GraphicsDevice.SetRenderState(CullMode_CullCounterClockwiseFace);
		}

		GraphicsDevice.EndAlphaBlend();
	}
	for (int i = 0; i < MISSILE_MAX; i++) {
		warning_pos[i] = GraphicsDevice.WorldToScreen(enemy_missile[i]->GetPosition());
		warning_pos[i].x -= 50.0f;
		warning_pos[i].y -= 50.0f;
		warning_pos[i].z = -50.0f;
		if (warning_pos[i].y <= 50.0)
		{
			warning_pos[i].y = 50.0f;
		}
		if (warning_pos[i].y >= 620.0)
		{
			warning_pos[i].y = 620.0f;
		}
		if (warning_pos[i].x <= 50.0)
		{
			warning_pos[i].x = 50.0f;
		}
		if (warning_pos[i].x >= 1180.0)
		{
			warning_pos[i].x = 1180.0f;
		}
	}

	SpriteBatch.Begin();

	//	fade_out_Draw();


	//�^�C�g���V�[���̕`��
	if (game_seane == TITLE_SEANE)
	{
		Title_Draw(); //�^�C�g���`��
		if (game_play_movie_state == 1) 
		{
			SpriteBatch.Draw(*game_play_movie, Vector3(0.0f, 0.0f, -2000.0f));
		}
	
	}

	if (game_seane == SETUMEI)
	{
		Description_Draw();
	}

	if (game_seane == SELECT_SEANE)
	{
		Select_Draw();
	}


	if (game_seane == TUTORIAL)
	{
		Time_Draw(); 
		this->transGauge_.Draw(Vector3_Zero);

		for (int i = 0; i < MISSILE_MAX; i++)
		{
			if (enemy_state == 2)
			{
				if (enemy_shot_state[i] == 2)
				{
					if (enemy_shot_state[i] == 2 && enemy_missile[i]->GetPosition().z >= 500)
					{
						SpriteBatch.Draw(*warning, warning_pos[i], warning_transparent);
					}
				}
			}
		}
	}

	if (game_seane == STAGE1)
	{
		Time_Draw();
		this->transGauge_.Draw(Vector3_Zero);

		for (int i = 0; i < MISSILE_MAX; i++)
		{
			if (enemy_state == 2)
			{
				if (enemy_shot_state[i] == 2)
				{
					if (enemy_shot_state[i] == 2 && enemy_missile[i]->GetPosition().z >= 500)
					{
						SpriteBatch.Draw(*warning, warning_pos[i], warning_transparent);
					}
				}
			}
		}
	}

	if (game_seane == STAGE2)
	{
		Time_Draw();
		this->transGauge_.Draw(Vector3_Zero);

		for (int i = 0; i < MISSILE_MAX; i++)
		{
			if (enemy_state == 2)
			{
				if (enemy_shot_state[i] == 2)
				{
					if (enemy_shot_state[i] == 2 && enemy_missile[i]->GetPosition().z >= 500)
					{
						SpriteBatch.Draw(*warning, warning_pos[i], warning_transparent);
					}
				}
			}
		}
	}

	//�N���A�V�[���̕`��
	if (game_seane == CLEAR_SEANE)
	{
		bgm_->Stop();
		bgm2->Stop();
		bgm3->Stop();
		Clear_Draw(); //�N���A
	}

	//�I�[�o�[�V�[���̕`��
	if (game_seane == OVER_SEANE)
	{
		bgm_->Stop();
		bgm2->Stop();
		bgm3->Stop();

		Over_Draw(); //�I�[�o�[
	}

	SpriteBatch.End();

	GraphicsDevice.EndScene();
}

void GameMain::Bg_Update()
{

	if (key_buffer.IsPressed(Keys_P))
	{
		time_state = 1;
	}

	//���Ԃ��O�ɂȂ�����N���A�V�[���֑J��
	if (time_state == 1)
	{
		bgm_->Stop(); //BGM��~
		engine_se->Stop();
		//fade_out_state = 7;
	}
	//�̗͂��O�ɂȂ�����I�[�o�[�V�[���֑J��
	if (hp_gauge <= 0.0)
	{
		bgm_->Stop(); //BGM��~
		engine_se->Stop();
		//fade_out_state = 5;
	}
}

//�^�C�g���̏���
void GameMain::Title_Update()
{
	//�G���^�[�L�[���������烁�C���V�[���֑J��
	if (pad_buffer.IsPressed(GamePad_Button1)
		|| pad_buffer.IsPressed(GamePad_Button2)
		|| pad_buffer.IsPressed(GamePad_Button3)
		|| pad_buffer.IsPressed(GamePad_Button4)
		|| pad_buffer.IsPressed(GamePad_Button10)
		|| key_buffer.IsPressed(Keys_Enter))
	{
		//if (fade_out_state == 0)
		//{
			se_->Play();
			//AllInitialize();
		//}
		//AllInitialize();
	}
		
}

//�^�C�g���̕`��
void GameMain::Title_Draw()
{
	//fade_out_Draw();
	SpriteBatch.Draw(*title, Vector3(0.0f,0.0f, 1000.0f)); //�^�C�g��
}

void GameMain::Description_Draw()
{
	SpriteBatch.Draw(*description, Vector3(0.0f, 0.0f, 1000.0f)); //����
}

void GameMain::Fade_Out_Update()
{

}

void GameMain::Fade_Out_Draw()
{
	//SpriteBatch.Draw(*fade_, Vector3(0.0f, 0.0f, -1000.0f),faded_out);
}

//�Z���N�g�̏���
void GameMain::Select_Update()
{
	if (transparency_state == 0)
	{
		select_transparent = 1.0f;
		if (select_transparent >= 1.0)
		{
			transparency_state = 1;
		}
	}
	if (transparency_state == 1)
	{
		select_transparent -= 0.03f;
		if (select_transparent <= 0.0)
		{
			select_transparent = 0.0f;
			transparency_state = 2;
		}
	}
	if (transparency_state == 2)
	{
		select_transparent += 0.03f;
		if (select_transparent >= 1.0)
		{
			select_transparent = 1.0f;
			transparency_state = 0;
		}
	}


	if (select_move_state == 0)
	{
		select_move_x = 190.0f;
		select_move_y = 200.0f;

		if (pad_state.X2)
		{
			select_move_state = 1;
		}
	}

	if (select_move_state == 1)
	{
		select_move_x += 4.5f;

		if (select_move_x >= 550.0)
		{
			select_move_x = 550.0f;
		}
		if (select_move_y <= 200.0)
		{
			select_move_y = 200.0f;
		}
		if (select_move_x == 550.0 && select_move_y == 200.0)
		{
			if (pad_state.X2)
			{
				select_move_state = 2;
			}
			
		}
	}

	if (select_move_state == 2)
	{
		select_move_x += 4.5f;

		if (select_move_x >= 910.0)
		{
			select_move_x = 910.0f;
		}
		if (select_move_y <= 110.0)
		{
			select_move_y = 110.0f;
		}
		if (pad_state.X2)
		{
			select_move_state = 2;
		}
	}

	//	if (fade_out_state == 0)
	//	{
	//�G���^�[�L�[���������烁�C���V�[���֑J��
	if (pad_buffer.IsPressed(GamePad_Button1)
		|| pad_buffer.IsPressed(GamePad_Button2)
		|| pad_buffer.IsPressed(GamePad_Button3)
		|| pad_buffer.IsPressed(GamePad_Button4)
		|| pad_buffer.IsPressed(GamePad_Button10)
		|| key_buffer.IsPressed(Keys_Enter))
	{
		//			if (fade_out_state == 0)
		//			{


		if (stage_button_x[0] > select_move_x + 252 || stage_button_x[0] + 131 < select_move_x ||
			stage_button_y[0] > select_move_y + 179 || stage_button_y[0] + 150 < select_move_y) {
		}
		else
		{
			game_seane = TUTORIAL;//�`���[�g���A����
			a = 0;
			Tutorial_Initialize();
		}

		if (stage_button_x[1] > select_move_x + 252 || stage_button_x[1] + 131 < select_move_x ||
			stage_button_y[1] > select_move_y + 179 || stage_button_y[1] + 150 < select_move_y) {
		}
		else
		{
			game_seane = STAGE1;//�X�e�[�W1��
			 a = 1;
			Stage1_Intialize();
		}


		if (stage_button_x[2] > select_move_x + 252 || stage_button_x[2] + 131 < select_move_x ||
			stage_button_y[2] > select_move_y + 179 || stage_button_y[2] + 150 < select_move_y) {
		}
		else
		{
			game_seane = STAGE2;//�X�e�[�W2��
			 a = 2;
			Stage2_Initialize();
		}
	}

}

//�Z���N�g�̕`��
void GameMain::Select_Draw()
{
	//fade_out_Draw();
	if (select_state == 0)
	{
		SpriteBatch.Draw(*select_cursor, Vector3(select_move_x, select_move_y, 100.0f), select_transparent); //���
	}
	else if (select_state == 1)
	{
		SpriteBatch.Draw(*select_cursor, Vector3(select_move_x, select_move_y, 100.0f), select_transparent); //���
	}
	else if (select_state == 2)
	{
		SpriteBatch.Draw(*select_cursor, Vector3(select_move_x, select_move_y, 100.0f), select_transparent); //���
	}
	SpriteBatch.Draw(*select_bg, Vector3(0.0f, 0.0f, 1100.0f)); //�Z���N�g
	for (int i = 0; i < 3; i++)
	{
		SpriteBatch.Draw(*stage_button[i], Vector3(stage_button_x[i], stage_button_y[i], 1000.0f)); //�Z���N�g
	}
}

//�N���A�̏���
void GameMain::Clear_Update()
{
	//if (fade_out_state == 0)
	//{
		//�G���^�[�L�[����������^�C�g���V�[���֑J��
		if (pad_buffer.IsPressed(GamePad_Button1)
			|| pad_buffer.IsPressed(GamePad_Button2)
			|| pad_buffer.IsPressed(GamePad_Button3)
			|| pad_buffer.IsPressed(GamePad_Button4)
			|| pad_buffer.IsPressed(GamePad_Button10))
		{

			if (scene_count == 1) {
				game_seane = TITLE_SEANE;
			}
			else {
				game_seane = SELECT_SEANE;
				scene_count = 0;
			}
		}
	//}
}

//�N���A�̕`��
void GameMain::Clear_Draw()
{
	//fade_out_Draw();
	SpriteBatch.Draw(*clear, Vector3(0.0f,0.0f, 1000.0f)); //�N���A
}

//�I�[�o�[�̏���
void GameMain::Over_Update()
{
	//if (fade_out_state == 0)
	//{
		//�G���^�[�L�[����������^�C�g���V�[���֑J��
		if (pad_buffer.IsPressed(GamePad_Button1)
			|| pad_buffer.IsPressed(GamePad_Button2)
			|| pad_buffer.IsPressed(GamePad_Button3)
			|| pad_buffer.IsPressed(GamePad_Button4)
			|| pad_buffer.IsPressed(GamePad_Button10))
		{
			game_seane = TITLE_SEANE;
			//fade_out_state = 0;
		}
	//}
}

//�I�[�o�[�̕`��
void GameMain::Over_Draw()
{
	//fade_out_Draw();
	SpriteBatch.Draw(*over, Vector3(0.0f,0.0f, 1000.0f)); //�I�[�o�[
}

void GameMain::Playre_Initialize() 
{
	camera->SetView(Vector3(camera_x, camera_y, camera_z), Vector3(camera_r_x, camera_r_y, camera_r_z)); //�J���������l
	
	player_x = 0.0f; 
	player_y = 10.0f; 
	player_z = 67.0f; 

	player_r_x = 0.0f;
	player_r_y = 0.0f;
	player_r_z = 0.0f;
	
	camera_x = 0.0f;
	camera_y = 80.0f;
	camera_z = -15.0f; 
	
	camera_r_x = 30.0f; 
	camera_r_y = 0.0f;
	camera_r_z = 0.0f; 

	camera_angle = 45.0f; //�J�����̎���

	anime_state = 0; //�A�j���[�V�����������Ȃ����邽�߂̑J��

	player_move_time = 1; //�����̎���
	
	p_s = 0; //�����J��
}

//�����̏���
void GameMain::Player_Update()
{
	if (this->playerInvincibleFrame_ > 0) this->playerInvincibleFrame_--;
	if (this->cantDashFrame_ > 0) this->cantDashFrame_--;
	this->playerAnimeManager_.Update();

	camera->SetRotation(camera_r_x, camera_r_y, camera_r_z);

	camera->SetPosition(camera_x, camera_y, camera_z);

	if (pad_buffer.IsPressed(GamePad_Button3)|| 
		pad_buffer.IsPressed(GamePad_Button5)||
		pad_buffer.IsPressed(GamePad_Button6)||
		pad_buffer.IsPressed(GamePad_Button7) ||
		pad_buffer.IsPressed(GamePad_Button8) &&
		this->cantDashFrame_ <= 0) {

		AXIS axis = NONE;
		if (pad_buffer.IsPressed(GamePad_Button3)) axis = DOWN;
		if (pad_buffer.IsPressed(GamePad_Button5)) axis = LEFT;
		if (pad_buffer.IsPressed(GamePad_Button6)) axis = RIGHT;
		if (pad_buffer.IsPressed(GamePad_Button7)) axis = UP;
		if (pad_buffer.IsPressed(GamePad_Button8)) axis = OVER;
		if (axis != NONE) {
			if (this->playerAnimeManager_.isPlaying()) {
				this->nextAxis_.push_back(axis);
			}
			else {
				this->SetAnimation(axis);
			}
		}
	}

	if (player_invincible_time > 0) 
	{
		player_invincible_time -= 1;
	}


	//�E�L�[���������Ƃ��ɉE�Ɉړ�
	if (!this->playerAnimeManager_.isPlaying())
	{
		if (p_s == 0)
		{
			if (pad_state.X < Axis_Center || key_state.IsKeyDown(Keys_Left))
			{
				player_move_time = 1;
				player_x += pad_paramater * ((float)pad_state.X / (float)Axis_Max);
				player_r_z += 2.0f;
				camera_r_z += 0.4f;
				camera_r_y += 0.4f;
				if (camera_r_y >= 45.0)
				{
					camera_r_y = 45.0f;
				}
				if (player_r_z >= 60.0)
				{
					player_r_z = 60.0f;
				}
				if (camera_r_z >= 20.0)
				{
					camera_r_z = 20.0f;
				}
			}
			else
				if (player_r_z >= 0.0)
				{
					if (player_r_z >= 0.0)
					{
						player_r_z -= 2.0f;
						camera_r_z -= 0.4f;
					}
					if (player_r_z <= 0.0)
					{
						player_r_z = 0.0f;
					}
					if (camera_r_z <= 0.0)
					{
						camera_r_z = 0.0f;
					}
				}
			if (pad_state.X > Axis_Center)
			{
				player_move_time -= 1.0f;
				if (player_move_time <= 0.0)
				{
					player_move_time = 0.0f;
				}
				if (player_move_time <= 0.0)
				{
					p_s = 1;
				}
			}
		}

		//���L�[���������Ƃ��ɍ��Ɉړ�
		if (p_s == 1)
		{
			if (pad_state.X > Axis_Center)
			{
				player_move_time = 1;
				player_x += pad_paramater * ((float)pad_state.X / (float)Axis_Max);
				player_r_z -= 2.0f;
				camera_r_z -= 0.4f;
				camera_r_y -= 0.4f;
				if (camera_r_y <= -45.0)
				{
					camera_r_y = -45.0f;
				}
				if (player_r_z <= -60.0)
				{
					player_r_z = -60.0f;
				}
				if (camera_r_z <= -20.0)
				{
					camera_r_z = -20.0f;
				}
			}
			else
				if (player_r_z <= 0.0)
				{
					if (player_r_z <= 0.0)
					{
						player_r_z += 2.0f;
						camera_r_z += 0.4f;
					}
					if (player_r_z >= 0.0)
					{
						player_r_z = 0.0f;
					}
					if (camera_r_z >= 0.0)
					{
						camera_r_z = 0.0f;
					}
				}
			if (pad_state.X < Axis_Center)
			{
				player_move_time -= 1.0f;
				if (player_move_time <= 0.0)
				{
					player_move_time = 0.0f;
				}
				if (player_move_time <= 0.0)
				{
					p_s = 0;
				}
			}
		}

		//��L�[���������Ƃ��ɏ�Ɉړ�
		if (pad_state.Y >= Axis_Center)
		{
			player_z -= pad_paramater * ((float)pad_state.Y / (float)Axis_Max);
			
		}

		//���L�[���������Ƃ��ɉ��Ɉړ�
		if (pad_state.Y <= Axis_Center)
		{
			player_z -= pad_paramater * ((float)pad_state.Y / (float)Axis_Max);
		}
	}

	if (key_state.IsKeyDown(Keys_Up))
	{
		camera_angle -= 0.3f;
		camera->SetFieldOfViewY(camera_angle);
	}

	if (key_state.IsKeyDown(Keys_Down))
	{
		camera_angle += 0.3f;
		camera->SetFieldOfViewY(camera_angle);
	}

		//������x���W�̍ő�l����
		if (player_x >= 200.0)
		{
			player_x = 200.0f;
		}

		//������x���W�̍ŏ��l����
		if (player_x <= -200.0)
		{
			player_x = -200.0f;
		}

		//������y���W�̍ő�l����
		if (player_y >= 300.0)
		{
			player_y = 300.0f;
		}

		//������y���W�̍ŏ��l����
		if (player_y <= -200.0)
		{
			player_y = -200.0f;
		}

		//������z���W�̍ő�l����
		if (player_z >= 400.0)
		{
			player_z = 400.0f;
			//camera_z -= 0.5f;
		}

		//������z���W�̍ŏ��l����
		if (player_z <= 45.0)
		{
			player_z = 45.0f;
			//camera_z += 0.5f;
		}

		//�Y�[���C������
		if (camera_angle <= 35.0)
		{
			camera_angle = 35.0f;
		}
		//�Y�[���A�E�g����
		if (camera_angle >= 55.0)
		{
			camera_angle = 55.0f;
		}

	
		if (player_flashing <= 0)
		{
			if (key_buffer.IsPressed(Keys_F))
			{
				player_flashing = 1;
			}
		} else {
			player_flashing--;
			if (player_flashing <= 0)
			{
				player_flashing = 0;
			}
		}

}

void GameMain::Player_Draw()
{
	GraphicsDevice.BeginAlphaBlend();

	Player->SetPosition(player_x, player_y, player_z); //�����ʒu
	Player->SetRotation(player_r_x, player_r_y, player_r_z); //������]�l
	Player->SetScale(0.09f);
	Player->AdvanceTime(0.02f);
	

	if (!player_flashing || (player_flashing % FLASHING_SYUKI <= FLASHING_SYUKI / 2))
	{
		if (!this->playerAnimeManager_.isPlaying())
		{
			Player->Draw();
		}
	}
	player_collision->SetPosition(player_obb.Center);
	//�e�̐���
	GraphicsDevice.BeginShadowRendering();

	//��]�s��̐ݒ�
	Matrix matrix_;
	
	//���C�g�^�C�v�A���C�g�̈ʒu�A�ǂ����Ƃ炷���A�e�̈ʒu
	matrix_ = Matrix_CreateShadow(Light_Directional, Vector3(0.2f, -1.0f,shadow_z), Player->GetPosition(), 
		Vector3(0.0f, -80.0f, 0.0f), Vector3_Up);

	//���
	for (int i = 0; i < 10; i++)
	{
		//�}�N���@=�@�ł����l�͌����Ă܂�
		float distance = FLT_MAX;
		//���C�@�O���E���h����v���C���[��
		cloud[i]->IntersectRay(Player->GetPosition(), Vector3_Down, &distance);
		//�e�`��
		Player->DrawShadow(matrix_, shadow_deps);
	}
	
	if (player_z <= 40.0)
	{
		shadow_z -= 0.01f;
		if (shadow_z <= -1.0)
		{
			shadow_z = -1.0f;
		}
	}
	if (player_z >= 80.0)
	{
		shadow_z += 0.01f;
		if (shadow_z >= 0.2)
		{
			shadow_z = 0.2f;
		}
	}
	if (player_z == 70.0)
	{
		shadow_z = 0.2f;
	}


	GraphicsDevice.EndShadowRendering();


}

//�^�C���̏���
void GameMain::Time_Update()
{
	if (time_state == 0)
	{
		time_frame = time_frame - 0.016;
		if (time_frame <= 0.0)
		{
			time_frame = 0;
			if (time_frame == 0)
			{
				time_state = 1;
			}
		}
	}
	
	first_ = time_frame;
	second_ = time_frame;
	//third_ = time_frame_;


	
}

void GameMain::Time_Draw()
{
	SpriteBatch.DrawString(Big_Font, Vector2(1060.0, 20.0), Color(4,196,116), _T("TIME"));
	SpriteBatch.Draw(*time_number, Vector3(1220.0f, 10.0f, 0.0f), RectWH(first_%10 *40.0, 0.0, 40.0, 41.0));
	SpriteBatch.Draw(*time_number, Vector3(1180.0f, 10.0f, 0.0f), RectWH(second_/10%10 *40.0, 0.0, 40.0, 41.0));
	//SpriteBatch.Draw(*number_, Vector3(1140.0f, 10.0f, 0.0f), RectWH(third_/100 *40.0, 0.0, 40.0, 41.0));
}

void GameMain::Sclorl_Initialize()
{
	for (int i = 0; i < SCLORL_MAX; i++)
	{
		//�_�̏����ʒu
		cloud[i]->SetPosition(-8000.0, -300.0, i * 400.0f);

		BG_[i]->SetPosition(500.0f, cloud_y, i * 400.0f);
		BG_2[i]->SetPosition(-500.0f, cloud_y, i * 400.0f);
	}
	
	cloud_z = 20.0f; //�X�N���[���X�s�[�h
}

//�X�N���[������
void GameMain::Sclorl_Update()
{
	//�X�N���[���X�s�[�h
	for (int i = 0; i < SCLORL_MAX; i++)
	{
		cloud[i]->Move(0.0f, 0.0f, cloud_z);
		BG_[i]->Move(0.0f, 0.0f, cloud_z);
		BG_2[i]->Move(0.0f, 0.0f, cloud_z);
	}
	//���[�v
	Vector3   gpos;
	Vector3   gpos11;
	Vector3   gpos21;
	for (int i = 0; i < SCLORL_MAX; i++)
	{
		gpos = cloud[i]->GetPosition();
		if (gpos.z >= 7600.0f)
		{
			gpos.z = -350.0f;
			cloud[i]->SetPosition(gpos);
		}

		gpos11 = BG_[i]->GetPosition();
		if (gpos11.z >= 7600.0f)
		{
			gpos11.z = -350.0f;
			BG_[i]->SetPosition(gpos11);
		}

		gpos21 = BG_2[i]->GetPosition();
		if (gpos21.z >= 7600.0f)
		{
			gpos21.z = -350.0f;
			BG_2[i]->SetPosition(gpos21);
		}
	}

}

//�G�l�~�[
void GameMain::Enemy_Update()
{
	for (int i = 0; i < MISSILE_MAX;i++)
	{
		//enemy_get_position = enemy->GetPosition();
		camera_get_position = camera->GetPosition();

		//�����蔻��
		enemy_obb.Center = enemy->GetPosition();
		enemy_qt = enemy->GetDirectionQuaternion();

		enemy_obb.SetAxis(enemy_qt);

		if (player_obb.Intersects(enemy_obb))
		{
			hp_gauge -= 0.1f;
			damege_se->Play();
			if (hp_scene >= 9)
			{
				damege_se->Stop();
			}
			bom_position[i] = Player->GetPosition();
			bom_state[i] = 1;
			enemy_state = 0;

		}
		else
		{

		}

		if (enemy_state == 0)
		{
			Vector3 player_getpos = Player->GetPosition();
			enemy->SetPosition(player_getpos.x, player_getpos.y, 2000);
			enemy_count = 240;
			enemy_state = 1;
		}

		if (enemy_state == 1) 
		{
			enemy_count--;
			if (enemy_count >= 0) 
			{
				enemy_state = 2;
			}
		}

		if (enemy_state == 2)
		{
			Vector3 enemy_getpos = enemy->GetPosition();
			if (enemy_getpos.z <= camera_get_position.z - 300)
			{
				enemy_state = 0;
			}
		}
	}
}

void GameMain::Missile_Initialize() 
{
	for (int i = 0; i < MISSILE_MAX;i++) 
	{
		enemy_shot_state[i] = 0;
		bom_state[i] = 0;
		bom_con[i] = 0;
		bezier_t[i] = 0;
	}
}

void GameMain::Missile_Update()
{
	player_obb.Center = Player->GetPosition();
	player_qt = Player->GetDirectionQuaternion();

	OrientedBoundingBox playerObb = player_obb;
	//���G���Ȃ�@�����蔻���傫��
	if (this->playerInvincibleFrame_ > 0) playerObb.Radius *= INVINCIBLE_COLLISION_MULTIPLAYER;

	const float hoseiX = 2.0f;
	const float hoseiY = 5.0f;
	const float hoseiZ = 5.0f;

	const float hosei2X = 2.0f;
	const float hosei2Y = 5.0f;
	const float hosei2Z = 5.0f;


	if (player_invincible_time <= 0)
	{
		for (int i = 0; i < MISSILE_MAX;i++)
		{
			//�x��
			if (warning_state == 0)
			{
				warning_transparent = 1.0f;
				if (warning_transparent >= 1.0)
				{
					warning_state = 1;
				}
			}
			if (warning_state == 1)
			{
				warning_transparent -= 0.03f;
				if (warning_transparent <= 0.0)
				{
					warning_transparent = 0.0f;
					warning_state = 2;
				}
			}
			if (warning_state == 2)
			{
				warning_transparent += 0.03f;
				if (warning_transparent >= 1.0)
				{
					warning_transparent = 1.0f;
					warning_state = 0;
				}
			}

			//�~�T�C�������蔻��
			if (enemy_shot_state[i] == 2) 
			{
				//obb2 �p�̌��_�␳�̍쐬
				Vector3 obb2X = enemy_missile[i]->GetRightVector() * hoseiX;
				Vector3 obb2Y = enemy_missile[i]->GetUpVector() * hoseiY;
				Vector3 obb2Z = enemy_missile[i]->GetFrontVector() * hoseiZ;

				Vector3 obb2hosei = obb2X + obb2Y + obb2Z;

				missile_obb[i].Center = enemy_missile[i]->GetPosition() + obb2hosei;
				missile_qt[i] = enemy_missile[i]->GetDirectionQuaternion();

				missile_obb[i].SetAxis(missile_qt[i]);

				if (player_obb.Intersects(missile_obb[i])) 
				{
					if (this->playerInvincibleFrame_ <= 0) 
					{
						hp_gauge -= 0.1f;
						damege_se->Play();
						if (hp_scene >= 9)
						{
							damege_se->Stop();
						}
						bom_position[i] = Player->GetPosition();
						bom_state[i] = 1;
						enemy_shot_state[i] = 0;
						this->transGauge_.SetScore(0);
						player_invincible_time = PLYER_INVINCIBLE_TIME;
						player_flashing = FLASHING_TIME;
						break;
					}
					else
					{

						//���G���ł���΁AtransGauge�㏸
						auto itr = std::find(this->hittingMissile_.begin(), this->hittingMissile_.end(), enemy_missile[i]);
						if (itr == this->hittingMissile_.end()) {
				
							this->hittingMissile_.push_back(enemy_missile[i]);
							this->transGauge_.SetScore(this->transGauge_.GetNowScore() + 1);
							if (this->transGauge_.GetLevelUpNorm() % this->transGauge_.GetNowScore())
							{
								flash_se->Play();
							}
						}

					}
				}
				else
				{
			
				}
			}
		}
	}

	//����
	for (int i = 0; i < MISSILE_MAX; i++)
	{
		if (bom_state[i] == 1) 
		{
			bom_con[i] += 0.03f;
			bom_scale[i] += 0.02f;
			if (bom_con[i] > 1.0f)
			{
				bom_con[i] = 1.0f;
				bom_state[i] = 2;
			}

			if (bom_scale[i] > 2.2f)
			{
				bom_scale[i] = 2.2f;
			}
		}
		else if (bom_state[i] == 2) 
		{
			bom_con[i] -= 0.045f;
			if (bom_con[i] < 0.0f)
			{
				bom_con[i] = 0;
				bom_scale[i] = 1.2;
				bom_state[i] = 0;
			}
		}

	}

	Vector3 Cam = camera->GetPosition();

	for (int i = 0; i < MISSILE_MAX;i++)
	{
		if (enemy_shot_state[i] == 2) 
		{
		
			//�e�̑��x����
			bezier_t[i] += 0.004f;
			//�z�[�~���O����
			homing_random[i] = MathHelper_Random(4, 7) / 10.0f;
			if (bezier_t[i] < homing_random[i]) {
				player_get_position[i] = Player->GetPosition();
			}

			Vector3 Missile_Get_Position = enemy_missile[i]->GetPosition();
														 
			bezier_position = Vector3_Bezier(bezier_storege[i],
				bezier_storege2[i],
				bezier_storege3[i],
				player_get_position[i], bezier_t[i]);
			enemy_missile[i]->SetPosition(bezier_position);

			Matrix  miss_rot;
			miss_rot = Matrix_CreateLookAt(Missile_Get_Position, bezier_position, Vector3_Up);
			miss_rot = Matrix_Invert(miss_rot);
			enemy_missile[i]->SetDirection(Quaternion_CreateFromRotationMatrix(miss_rot));
		
			smoke_losttime[i] = 1.0f;
			if (bezier_t[i] >= 2)
			{
				enemy_shot_state[i] = 0;
				continue;
			}
			
		}

	}
}

void GameMain::SetBezierToEnemyMissile(int missilNum, int dx, int dy, int dz)
{
	float random_value = dist(random);

	enemy_missile[missilNum]->SetPosition(enemy->GetPosition());//�G�l�~�[�̍��W�ƒe�̍��W�𓝈�
	enemy_missile[missilNum]->SetDirection(180, 0, 0);
	//���������߂�
	Vector3 dir = Player->GetPosition() - enemy->GetPosition();
	bezier_t[missilNum] = 0;

	float strikeout = random_value;

	bezier_storege[missilNum] = enemy->GetPosition();
	bezier_storege[missilNum].y -= 10.0f;

	bezier_storege2[missilNum] = enemy->GetPosition();
	bezier_storege2[missilNum].z += dir.z * 0.3f;
	bezier_storege2[missilNum].y += random_value * dy;
	bezier_storege2[missilNum].x += random_value * dx;

	bezier_storege3[missilNum] = enemy->GetPosition();
	bezier_storege3[missilNum].z += dir.z*0.6f;
	bezier_storege3[missilNum].y += random_value * dy;
	bezier_storege3[missilNum].x += random_value * dx;

	bezier_storege4[missilNum] = Player->GetPosition();

	//	enemy_missile2[i]->SetPosition(enemy[0]->GetPosition() + i * Vector3(-100, 0, -500));//�G�l�~�[�̍��W�ƒe2�̍��W�𓝈�
	//	enemy_missile2[i]->SetDirection(180, 0, 0);
	//���������߂�
	//	Vector3 dir = player_->GetPosition() - enemy[0]->GetPosition();
	//	bezier2_t_[i] = 0;

	//  bezier_storege01_[i] = enemy[0]->GetPosition();
	//  bezier_storege01_[i].y -= 10.0f;

	//	bezier_storege02_[i] = enemy[0]->GetPosition();
	//	bezier_storege02_[i].z += dir.z*0.3f;
	//	bezier_storege02_[i].y -= 50.0f;
	//	bezier_storege02_[i].x += random_value;

	//	bezier_storege03_[i] = enemy[0]->GetPosition();
	//	bezier_storege03_[i].z += dir.z*0.6f;
	//	bezier_storege03_[i].y -= 10.0f;
	//	bezier_storege03_[i].x -= random_value;

	//	bezier_storege04_[i] = player_->GetPosition();
}

void GameMain::UpdateCollisionToHittingMissile()
{

	player_obb.Center = Player->GetPosition() + Vector3(0, 10, 0);
	player_qt = Player->GetDirectionQuaternion();

	OrientedBoundingBox playerObb = player_obb;
	//���G���Ȃ�@�����蔻���傫��
	if (playerInvincibleFrame_ > 0) playerObb.Radius *= INVINCIBLE_COLLISION_MULTIPLAYER;

	const float correction_X = 2.0f;
	const float correction_Y = 5.0f;
	const float correction_Z = 5.0f;

	const float correction2_X = 2.0f;
	const float correction2_Y = 5.0f;
	const float correction2_Z = 5.0f;

	auto itr = this->hittingMissile_.begin();
	while (itr != this->hittingMissile_.end()) {

		OrientedBoundingBox enemyObb = (*itr)->GetOBB();

		Vector3 obb2X = (*itr)->GetRightVector() * correction_X;
		Vector3 obb2Y = (*itr)->GetUpVector() * correction_Y;
		Vector3 obb2Z = (*itr)->GetFrontVector() * correction_Z;

		Vector3 obb2_correction = obb2X + obb2Y + obb2Z;

		enemyObb.Center = (*itr)->GetPosition() + obb2_correction;

		if (!playerObb.Intersects(enemyObb)) {

			itr = this->hittingMissile_.erase(itr);
			continue;

		}

		itr++;

	}

}

void GameMain::G_Initialize()
{
	hp_gauge = 1.0f;
	hp_pos = Vector3(1000.0f, 500.0f, 0.0f);
}

//G�Q�[�W
void GameMain::G_Draw()
{
	SpriteBatch.Draw(*trans_frame, Vector3(0.0f, 0.0f, 0.0f));
	SpriteBatch.Draw(*trans_gage, Vector3(0.0f, 0.0f, -1.0f));
}

//�G
void GameMain::Enemy_Initialize()
{
	for (int i = 0;i < 9;i++)
	{
		teki_state[i] = 0;
	}

	for (int i = 0; i < 9; i++)
	{
		teki_flag[i] = 0;
	}
}

void GameMain::Enemy_Draw()
{
	Vector3 p_pos = Player->GetPosition();

	//�퓬�@
	for (int i = 0; i <= 3; i++)
	{
		if (teki_flag[i] == 1)
		{
			//for (int i = 0; i < 2; i++)
			//{
			teki_pos_y[i] = p_pos.y;
			teki[i]->SetPosition(teki_pos_x[i], teki_pos_y[i], teki_pos_z[i]); //�G�̏����ʒu
			teki[i]->Draw(); //�G�̕`��
			teki_collsion[i]->SetPosition(teki_obb[i].Center);
			teki_collsion[i]->SetDirection(teki_qt[i]);
			//				Hako_T[1]->Draw();
			//			}
		}
	}

	//�G�l�~�[
	for (int i = 3; i < 4; i++)
	{
		if (teki_flag[i] == 1)
		{
			//for (int i = 0; i < 2; i++)
			//{
			teki_pos_y[i] = p_pos.y;
			teki[i]->SetPosition(teki_pos_x[i], teki_pos_y[i], teki_pos_z[i]); //�G�̏����ʒu
			teki[i]->SetRotation(0.0f, 180.f, 0.0f); //�G�̏�����]�l
			teki[i]->Draw(); //�G�̕`��
			teki_collsion[i]->SetPosition(teki_obb[i].Center);
			teki_collsion[i]->SetDirection(teki_qt[i]);
			//	Hako_T[2]->Draw();
			//			}
		}
	}

	if (teki_flag[5] == 1)
	{
		teki_pos_y[5] = p_pos.y;
		teki[5]->SetPosition(teki_pos_x[5], teki_pos_y[5], teki_pos_z[5]); //�G�̏����ʒu
		teki[5]->Draw(); //�G�̕`��
		teki_collsion[5]->SetPosition(teki_obb[5].Center);
		teki_collsion[5]->SetDirection(teki_qt[5]);
		//	Hako_T[2]->Draw();
	}

	for (int i = 6; i <= 7; i++)
	{
		if (teki_flag[i] == 1)
		{
			teki_pos_y[i] = p_pos.y;
			teki[i]->SetPosition(teki_pos_x[i], teki_pos_y[i], teki_pos_z[i]); //�G�̏����ʒu
			teki[i]->SetRotation(0.0f, 180.f, 0.0f); //�G�̏�����]�l
			teki[i]->Draw(); //�G�̕`��
			teki_collsion[i]->SetPosition(teki_obb[i].Center);
			teki_collsion[i]->SetDirection(teki_qt[i]);
		}
	}

	if (teki_flag[8] == 1)
	{
		teki_pos_y[8] = p_pos.y;
		teki[8]->SetPosition(teki_pos_x[8], teki_pos_y[8], teki_pos_z[8]); //�G�̏����ʒu
		teki[8]->Draw(); //�G�̕`��
		teki_collsion[8]->SetPosition(teki_obb[8].Center);
		teki_collsion[8]->SetDirection(teki_qt[8]);
		//	Hako_T[2]->Draw();
	}

	//�e�̐���
	GraphicsDevice.BeginShadowRendering();

	//��]�s��̐ݒ�
	Matrix matrix_[9];

	//���C�g�^�C�v�A���C�g�̈ʒu�A�ǂ����Ƃ炷���A�e�̈ʒu
	for (int i = 0; i < 9; i++)
	{
		if (teki_flag[i] == 1)
		{
			matrix_[i] = Matrix_CreateShadow(Light_Directional, Vector3(0.2f, -1.0f, shadow_z), teki[i]->GetPosition(),
			Vector3(0.0f, -80.0f, 0.0f), Vector3_Up);
		}
	}

	//���
	for (int i = 0; i < 10; i++)
	{
		//�}�N���@=�@�ł����l�͌����Ă܂�
		/*float distance = FLT_MAX;
		float distance2 = FLT_MAX;
		float distance3 = FLT_MAX;
		float distance4 = FLT_MAX;*/

		float distance[9];

		for (int z = 0; z < 9; z++)
		{
			if (teki_flag[z] == 1)
			{
				distance[z] = FLT_MAX;
				cloud[i]->IntersectRay(teki[z]->GetPosition(), Vector3_Down, &distance[z]);
				teki[z]->DrawShadow(matrix_[z], shadow_deps);
			}
		}

	}

	GraphicsDevice.EndShadowRendering();
}

void GameMain::Enemy_Shot_Initialize() {


	Vector3 p_pos = Player->GetPosition();

	for (int i = 0; i < BEAM_MAX; i++)
	{
		beam_y[i] = p_pos.y; //�e��y���W	
	}

	//���퓬�@
	beam_x[0] = teki_pos_x[0] + 20;
	beam_z[0] = teki_pos_z[0];
	beam_x[1] = teki_pos_x[0] - 20;
	beam_z[1] = teki_pos_z[0];

	//�E�퓬�@
	beam_x[2] = teki_pos_x[1] + 20;
	beam_z[2] = teki_pos_z[1];
	beam_x[3] = teki_pos_x[1] - 20;
	beam_z[3] = teki_pos_z[1];

	//�����퓬�@
	beam_x[4] = teki_pos_x[2] + 20;
	beam_z[4] = teki_pos_z[2];
	beam_x[5] = teki_pos_x[2] - 20;
	beam_z[5] = teki_pos_z[2];

	//���G�l�~�[
	beam_x[6] = teki_pos_x[3] + 10;
	beam_z[6] = teki_pos_z[3];
	beam_x[7] = teki_pos_x[3] - 10;
	beam_z[7] = teki_pos_z[3];

	//�E�G�l�~�[
	beam_x[8] = teki_pos_x[4] + 10;
	beam_z[8] = teki_pos_z[4];
	beam_x[9] = teki_pos_x[4] - 10;
	beam_z[9] = teki_pos_z[4];

	//���
	beam_x[10] = teki_pos_x[5] + 10;
	beam_z[10] = teki_pos_z[5];
	beam_x[11] = teki_pos_x[5] - 10;
	beam_z[11] = teki_pos_z[5];

	for (int i = 0; i < 12; i++)
	{
		beam_state[i] = 0;
	}

}

void GameMain::Enemy_Shot_Update()
{
	Vector3 p_pos = Player->GetPosition();

	//�r�[�������蔻��

	if (player_invincible_time <= 0)
	{
		for (int i = 0; i < BEAM_MAX; i++)
		{
			if (beam_state[i] == 1) {
				beam_obb[i].Center = beam_[i]->GetPosition();
				beam_qt[i] = beam_[i]->GetDirectionQuaternion();

				beam_obb[i].SetAxis(beam_qt[i]);
				if (player_obb.Intersects(beam_obb[i])) 
				{
					hp_gauge -= 0.1f;
					damege_se->Play();
					if (hp_scene >= 9)
					{
						damege_se->Stop();
					}
					bom_position[i] = Player->GetPosition();
					bom_state[i] = 1;
					beam_state[i] = 0;
					player_invincible_time = PLYER_INVINCIBLE_TIME;
					player_flashing = FLASHING_TIME;
					break;
				}
				else {

				}
			}
		}
	}

	if (teki_flag[0] == 1)
	{
		if (beam_state[0] == 0)
		{
			beam_x[0] = teki_pos_x[0] + 20;
			beam_z[0] = teki_pos_z[0];
			if (teki_pos_z[0] >= 150.0)
			{
				beam_state[0] = 1;
			}
		}
		if (beam_state[1] == 0)
		{
			beam_x[1] = teki_pos_x[0] - 20;
			beam_z[1] = teki_pos_z[0];
			if (teki_pos_z[0] >= 150.0)
			{
				beam_state[1] = 1;
			}
		}
	}

	if (beam_state[0] == 1)
	{
		beam_z[0] -= 11.0f;
		beam_y[0] = p_pos.y;
		if (beam_z[0] <= -10.0)
		{
			beam_state[0] = 0;
		}
	}

	if (beam_state[1] == 1)
	{
		beam_z[1] -= 11.0f;
		beam_y[1] = p_pos.y;
		if (beam_z[1] <= -10.0)
		{
			beam_state[1] = 0;
		}
	}

	//	}

	//�퓬�@2
	if (teki_flag[1] == 1)
	{
		if (beam_state[2] == 0)
		{
			beam_x[2] = teki_pos_x[1] + 20;
			beam_z[2] = teki_pos_z[1];
			if (teki_pos_z[1] >= 150.0)
			{
				beam_state[2] = 1;
			}
		}
		if (beam_state[3] == 0)
		{
			beam_x[3] = teki_pos_x[1] - 20;
			beam_z[3] = teki_pos_z[1];
			if (teki_pos_z[1] >= 150.0)
			{
				beam_state[3] = 1;
			}
		}
	}

	if (beam_state[2] == 1)
	{
		beam_z[2] -= 11.0f;
		beam_y[2] = p_pos.y;
		if (beam_z[2] <= -10.0)
		{
			beam_state[2] = 0;
		}
	}
	if (beam_state[3] == 1)
	{
		beam_z[3] -= 11.0f;
		beam_y[3] = p_pos.y;
		if (beam_z[3] <= -10.0)
		{
			beam_state[3] = 0;
		}
	}

	//�퓬�@3
	if (teki_flag[2] == 1)
	{
		if (beam_state[4] == 0)
		{
			beam_x[4] = teki_pos_x[2] + 20;
			beam_z[4] = teki_pos_z[2];
			if (teki_pos_z[2] >= 150.0)
			{
				beam_state[4] = 1;
			}
		}
		if (beam_state[5] == 0)
		{
			beam_x[5] = teki_pos_x[2] - 20;
			beam_z[5] = teki_pos_z[2];
			if (teki_pos_z[2] >= 150.0)
			{
				beam_state[5] = 1;
			}
		}
	}

	if (beam_state[4] == 1)
	{
		beam_z[4] -= 11.0f;
		beam_y[4] = p_pos.y;
		if (beam_z[4] <= -10.0)
		{
			beam_state[4] = 0;
		}
	}

	if (beam_state[5] == 1)
	{
		beam_z[5] -= 11.0f;
		beam_y[5] = p_pos.y;
		if (beam_z[5] <= -10.0)
		{
			beam_state[5] = 0;
		}
	}

	//�E�G�l�~�[1
	if (teki_flag[3] == 1)
	{
		if (beam_state[6] == 0)
		{
			beam_x[6] = teki_pos_x[3] + 20;
			beam_z[6] = teki_pos_z[3];
			if (teki_pos_z[3] >= 150.0)
			{
				beam_state[6] = 1;
			}
		}

		if (beam_state[7] == 0)
		{
			beam_x[7] = teki_pos_x[3] - 20;
			beam_z[7] = teki_pos_z[3];
			if (teki_pos_z[3] >= 150.0)
			{
				beam_state[7] = 1;
			}
		}

	}

	if (beam_state[6] == 1)
	{
		beam_z[6] -= 9.0f;
		beam_y[6] = p_pos.y;
		if (beam_z[6] <= -10.0)
		{
			beam_state[6] = 0;
		}
	}

	if (beam_state[7] == 1)
	{
		beam_z[7] -= 9.0f;
		beam_y[7] = p_pos.y;
		if (beam_z[7] <= -10.0)
		{
			beam_state[7] = 0;
		}
	}

	//�E�G�l�~�[2
	if (teki_flag[4] == 1)
	{

		if (beam_state[8] == 0)
		{
			beam_x[8] = teki_pos_x[4] + 20;
			beam_z[8] = teki_pos_z[4];
			if (teki_pos_z[4] >= 150.0)
			{
				beam_state[8] = 1;
			}
		}

		if (beam_state[9] == 0)
		{
			beam_x[9] = teki_pos_x[4] - 20;
			beam_z[9] = teki_pos_z[4];
			if (teki_pos_z[4] >= 150.0)
			{
				beam_state[9] = 1;
			}
		}

	}

	if (beam_state[8] == 1)
	{
		beam_z[8] -= 9.0f;
		beam_y[8] = p_pos.y;
		if (beam_z[8] <= -10.0)
		{
			beam_state[8] = 0;
		}
	}

	if (beam_state[9] == 1)
	{
		beam_z[9] -= 9.0f;
		beam_y[9] = p_pos.y;
		if (beam_z[9] <= -10.0)
		{
			beam_state[9] = 0;
		}
	}

	//���
	if (teki_flag[5] == 1)
	{
		if (beam_state[10] == 0)
		{
			beam_x[10] = teki_pos_x[5] + 10;
			beam_z[10] = teki_pos_z[5];
			if (teki_pos_z[5] >= 150.0)
			{
				beam_state[10] = 1;
			}
		}

		if (beam_state[11] == 0)
		{
			beam_x[11] = teki_pos_x[5] - 10;
			beam_z[11] = teki_pos_z[5];
			if (teki_pos_z[5] >= 150.0)
			{
				beam_state[11] = 1;
			}
		}

	}

	if (beam_state[10] == 1)
	{
		beam_z[10] -= 14;
		beam_y[10] = p_pos.y;
		if (beam_z[10] <= -10.0)
		{
			beam_state[10] = 0;
		}
	}


	if (beam_state[11] == 1)
	{
		beam_z[11] -= 14;
		beam_y[11] = p_pos.y;
		if (beam_z[11] <= -10.0)
		{
			beam_state[11] = 0;
		}
	}

	//���G�l�~�[1
	if (teki_flag[6] == 1)
	{
		if (beam_state[12] == 0)
		{
			beam_x[12] = teki_pos_x[6] + 20;
			beam_z[12] = teki_pos_z[6];
			if (teki_pos_z[6] >= 150.0)
			{
				beam_state[12] = 1;
			}
		}
		if (beam_state[13] == 0)
		{
			beam_x[13] = teki_pos_x[6] - 20;
			beam_z[13] = teki_pos_z[6];
			if (teki_pos_z[6] >= 150.0)
			{
				beam_state[13] = 1;
			}
		}

	}


	if (beam_state[12] == 1)
	{
		beam_z[12] -= 9.0f;
		beam_y[12] = p_pos.y;
		if (beam_z[12] <= -10.0)
		{
			beam_state[12] = 0;
		}
	}

	if (beam_state[13] == 1)
	{
		beam_z[13] -= 9.0f;
		beam_y[13] = p_pos.y;
		if (beam_z[13] <= -10.0)
		{
			beam_state[13] = 0;
		}
	}

	//���G�l�~�[2
	if (teki_flag[7] == 1)
	{
		if (beam_state[14] == 0)
		{
			beam_x[14] = teki_pos_x[7] + 20;
			beam_z[14] = teki_pos_z[7];
			if (teki_pos_z[7] >= 150.0)
			{
				beam_state[14] = 1;
			}
		}

		if (beam_state[15] == 0)
		{
			beam_x[15] = teki_pos_x[7] - 20;
			beam_z[15] = teki_pos_z[7];
			if (teki_pos_z[7] >= 150.0)
			{
				beam_state[15] = 1;
			}
		}

	}

	if (beam_state[14] == 1)
	{
		beam_z[14] -= 9.0f;
		beam_y[14] = p_pos.y;
		if (beam_z[14] <= -10.0)
		{
			beam_state[14] = 0;
		}
	}

	if (beam_state[15] == 1)
	{
		beam_z[15] -= 9.0f;
		beam_y[15] = p_pos.y;
		if (beam_z[15] <= -10.0)
		{
			beam_state[15] = 0;
		}
	}

}
//�e
void GameMain::Enemy_Shot_Draw()
{
	Vector3 p_pos = Player->GetPosition();

	for (int i = 0; i < 6; i++)
	{
		if (teki_flag[i] == 1)
		{
			for (int j = 0; j < 12; j++)
			{
				beam_y[j] = p_pos.y;

				if (beam_state[j] != 0)
				{
					beam_[j]->SetPosition(beam_x[j], beam_y[j], beam_z[j]);
					beam_[j]->DrawAlpha(12);
					beam_collsion[j]->SetPosition(beam_obb[j].Center);
					beam_collsion[j]->SetDirection(beam_qt[j]);
					//				Hako_TB[i]->Draw();
				}
			}

		}
	}

}

void GameMain::Ui_Initialize()
{
	//����
	first_ = 0; //��̈�
	second_ = 0; //�\�̈�
	//third_ = 0; //�S�̈�

	hp_scene = 0; //�̗͑J��
	time_state = 0; //���ԑJ��
	time_frame = 60; //����

	G_Initialize();

	//�x��
	warning_state = 0;
	warning_transparent = 1.0f;//�����x
}

void GameMain::All_Initialize() 
{
	stage_button_x[0] = 100;
	stage_button_x[1] = 500;
	stage_button_x[2] = 900;
	for (int i = 0; i < 3; i++)
	{
		stage_button_y[i] = 190;
	}

	//�X�J�C�h�[��
	skydome->SetPosition(0.0f, -500.0f, 0.0f);

	//���[�r�[
	game_play_movie_state = 0;
	game_play_movie_time = 0;

	scene_count = 0;

	transparency_state = 0;

	select_state = 0;

	select_transparent = 1.0f;//�����x

	//�e
	shadow_deps = 0.08f; //�e�̔Z��
	shadow_z = 0.2f; //�e��z���W
	
	pad_paramater = 1.5; //�p�b�h�X�e�[�^�X

	faded_out = 0.0f;

	Ui_Initialize();
	Playre_Initialize();
	Missile_Initialize();
	Enemy_Initialize();
	Enemy_Shot_Initialize();
	Sclorl_Initialize();
	
	//fade_out_state = 1;
	//faded_out = 0.0f;

	player_flashing = 0;
	
	using namespace Fujimura;

	METHOD_TIMER.Initialize();

	ReadFireTimeMap::Reset();
	//ReadFireTimeMap().Import(_T("fileData.txt"));

	this->transGauge_.Initialize();
	this->playerAnimeManager_.Initialize();
	this->playerInvincibleFrame_ = 0;
	this->cantDashFrame_ = 0;
	this->prevLevel_ = -1;
	this->chainCount_ = 0;
	this->nextAxis_.clear();

}

void GameMain::Tutorial_Initialize()
{
	Tutorial_Teki_Intialize();
	Tutorial_Time_Intialize();
	
	//Fujimura��Y���ăR�s�[���Ă�������
	using namespace Fujimura;
	ReadFireTimeMap().Import(_T("txt/tutorial.txt"));
	hp_gauge = 1.0f;

}
void GameMain::Tutorial_Update()
{
	Tutorial_Teki_Update();
	Tutorial_Time_Update();
}
void GameMain::Tutorial_Teki_Intialize()
{
	Vector3 p_pos = Player->GetPosition();

	// teki�@3:�G�l�~�[�@0:�퓬�@�@5:���

	//�`���[�g���A��
	teki_pos_x[3] = 0;
	teki_pos_y[3] = p_pos.y;
	teki_pos_z[3] = 1000.0f;
	teki_state[3] = 0;
	teki_flag[3] = 0;

	teki_pos_x[8] = p_pos.x;
	teki_pos_y[8] = p_pos.y;
	teki_pos_z[8] = 2000;
	teki_state[8] = 1;
	teki_flag[8] = 0;

	teki_pos_x[5] = 0;
	teki_pos_y[5] = p_pos.y;
	teki_pos_z[5] = 2000;
	teki_state[5] = 1;
	teki_flag[5] = 0;

}
void GameMain::Tutorial_Teki_Update()
{
	//�`���[�g���A��
	Vector3 p_pos = Player->GetPosition();
	////�G�l�~�[	
	if (teki_flag[3] == 1)
	{
		if (player_invincible_time <= 0)
		{
		
			teki_obb[3].Center = teki[3]->GetPosition();
			teki_qt[3] = teki[3]->GetDirectionQuaternion();

			teki_obb[3].SetAxis(teki_qt[3]);
			if (player_obb.Intersects(teki_obb[3])) 
			{
		
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[3] = Player->GetPosition();
				bom_state[3] = 1;
				teki_state[3] = 1;
				teki_flag[3] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else 
			{

			}

			if (teki_state[3] == 0)
			{
				teki_pos_z[3] -= 3.0f;
				if (teki_pos_z[3] >= 1000.0)
				{
					teki_state[3] = 1;
					teki_flag[3] = 0;
				}
			}

			for (int j = 0; j < 2; j++)
			{
				if (teki_state[3] == 1)
				{
					teki_pos_x[3] = 0.0f;
					teki_pos_y[3] = p_pos.y;
					teki_pos_z[3] = 1000.0f;
					if (teki_pos_z[3] <= -10.0)
					{
						beam_state[j] = 0;
						teki_state[3] = 0;

					}
				}
			}
		}

	}

	//�퓬�@	
	if (teki_flag[8] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[8].Center = teki[8]->GetPosition();
			teki_qt[8] = teki[8]->GetDirectionQuaternion();

			teki_obb[8].SetAxis(teki_qt[8]);
			if (player_obb.Intersects(teki_obb[8])) 
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[8] = Player->GetPosition();
				bom_state[8] = 1;
				teki_state[8] = 0;
				teki_flag[8] = 0;

				player_invincible_time = PLYER_INVINCIBLE_TIME;
				//				break;
			}
			else {

			}
		}

		for (int j = 0; j < 2; j++)
		{
			if (teki_state[8] == 1)
			{
				teki_pos_x[8] = 0.0f;
				teki_pos_y[8] = p_pos.y;
				teki_pos_z[8] -= 3.0f;

				if (teki_pos_z[8] < 300)
				{
					teki_state[8] = 2;
				}
			}

			if (teki_state[8] == 2)
			{
				Vector3 t_pos = teki[8]->GetPosition();

				teki_pos_x[8] = t_pos.x;
				teki_pos_y[8] = t_pos.y;
				teki_pos_z[8] -= 3.0f;
				if (teki_pos_z[8] <= -100.0)
				{
					beam_state[8] = 0;
					teki_state[8] = 0;
					teki_flag[8] = 0;
				}
			}
		}
	}

	//���
	if (teki_flag[5] == 1)
	{
		if (player_invincible_time <= 0)
		{
			
			teki_obb[5].Center = teki[5]->GetPosition();
			teki_qt[5] = teki[5]->GetDirectionQuaternion();

			teki_obb[5].SetAxis(teki_qt[5]);
			if (player_obb.Intersects(teki_obb[5]))
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[5] = Player->GetPosition();
				bom_state[5] = 1;
				teki_state[5] = 0;
				teki_flag[5] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				//				break;
			}
			else {

			}
		}

		if (teki_state[5] == 1)
		{
			teki_pos_z[5] -= 1.5f;
			if (teki_pos_z[5] <= 1200)
			{
				teki_pos_z[5] = 1200;

			}
		}

	}
}
void GameMain::Tutorial_Time_Intialize()
{
	time_frame = 60; //����
	time_state = 0;
}
void GameMain::Tutorial_Time_Update()
{
	if ((int)time_frame == 55 && teki_flag[3] == 0)
	{
		teki_flag[3] = 1;//�G�l�~�[
	}

	if ((int)time_frame == 45 && teki_flag[8] == 0)
	{
		teki_flag[8] = 1;//�퓬�@
	}

	if ((int)time_frame == 40 && teki_flag[5] == 0)
	{
		teki_flag[5] = 1;//���
	}
}

void GameMain::Stage1_Intialize()
{
	Stage1_Teki_Intialize();
	Stage1_Time_Intialize();
	using namespace Fujimura;
	ReadFireTimeMap().Import(_T("txt/STAGE1.txt"));
	hp_gauge = 1.0f;
}
void GameMain::Stage1_Update()
{
	Stage1_Teki_Update();
	Stage1_Time_Update();
}
void GameMain::Stage1_Teki_Intialize()
{
	Vector3 p_pos = Player->GetPosition();

	//STAGE1

	//teki  0�`2:�퓬�@  3�`4�A6�`7:�G�l�~�[  5:��� 

	for (int i = 0; i < 8; i++)
	{
		teki_state[i] = 0;
		teki_pos_y[i] = p_pos.y;
	}

	//�E�G�l�~�[
	teki_pos_x[3] = 200.0f;
	teki_pos_z[3] = -200.0f;

	teki_pos_x[4] = 300.0f;
	teki_pos_z[4] = -200.0f;

	//���G�l�~�[
	teki_pos_x[6] = -200.0f;
	teki_pos_z[6] = -200.0f;

	teki_pos_x[7] = -300.0f;
	teki_pos_z[7] = -200.0f;

	//���
	teki_pos_x[5] = 0;
	teki_pos_z[5] = 1500;
	
	//�G�l�~�[
	for (int i = 0; i <= 2; i++)
	{
		teki_pos_x[i] = p_pos.x;
		teki_pos_z[i] = 2000;
	}

}
void GameMain::Stage1_Teki_Update()
{
	//�X�e�[�W1
	Vector3 p_pos = Player->GetPosition();

	//	�E�G�l�~�[1	
	if (teki_flag[3] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[3].Center = teki[3]->GetPosition();
			teki_qt[3] = teki[3]->GetDirectionQuaternion();

			teki_obb[3].SetAxis(teki_qt[3]);
			if (player_obb.Intersects(teki_obb[3])) 
			{
			
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[3] = Player->GetPosition();
				bom_state[3] = 1;
				teki_state[3] = 1;
				teki_flag[3] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else 
			{

			}

			if (teki_state[3] == 0)
			{
				teki_pos_x[3] = 200.0f;
				teki_pos_z[3] = -200;
				if (teki_pos_z[3] <= -200.0)
				{
					teki_state[3] = 1;
				}
			}

			if (teki_state[3] == 1)
			{
				teki_pos_z[3] += 3.0f;
				teki_pos_x[3] -= 0.2f;
				for (int j = 0; j < 2; j++)
				{
					if (teki_pos_z[3] >= 1500.0)
					{
						beam_state[j] = 0;
						teki_state[3] = 0;
						teki_flag[3] = 0;
					}
				}
			}
		}
	}

	//	�E�G�l�~�[2	
	if (teki_flag[4] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[4].Center = teki[4]->GetPosition();
			teki_qt[4] = teki[4]->GetDirectionQuaternion();

			teki_obb[4].SetAxis(teki_qt[4]);
			if (player_obb.Intersects(teki_obb[4]))
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[4] = Player->GetPosition();
				bom_state[4] = 1;
				teki_state[4] = 1;
				teki_flag[4] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else
			{

			}
		}

		if (teki_state[4] == 0)
		{
			teki_pos_x[4] = 300.0f;
			teki_pos_z[4] = -200;
			if (teki_pos_z[4] <= -200.0)
			{
				teki_state[4] = 1;
			}
		}

		if (teki_state[4] == 1)
		{
			teki_pos_z[4] += 3.0f;
			teki_pos_x[4] -= 0.2f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[4] >= 1500.0)
				{
					beam_state[j] = 0;
					teki_state[4] = 0;
					teki_flag[4] = 0;
				}
			}
		}
	}

	//	���G�l�~�[1	
	if (teki_flag[6] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[6].Center = teki[6]->GetPosition();
			teki_qt[6] = teki[6]->GetDirectionQuaternion();

			teki_obb[6].SetAxis(teki_qt[6]);
			if (player_obb.Intersects(teki_obb[6]))
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[6] = Player->GetPosition();
				bom_state[6] = 1;
				teki_state[6] = 1;
				teki_flag[6] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else
			{

			}
		}

		if (teki_state[6] == 0)
		{
			teki_pos_x[6] = -200.0f;
			teki_pos_z[6] = -200;
			if (teki_pos_z[6] <= -200.0)
			{
				teki_state[6] = 1;
			}
		}

		if (teki_state[6] == 1)
		{
			teki_pos_z[6] += 3.0f;
			teki_pos_x[6] += 0.2f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[6] >= 1500.0)
				{
					beam_state[j] = 0;
					teki_state[6] = 0;
					teki_flag[6] = 0;
				}
			}
		}
	}

	//	���G�l�~�[2	
	if (teki_flag[7] == 1)
	{
		if (player_invincible_time <= 0)
		{
			
			teki_obb[7].Center = teki[7]->GetPosition();
			teki_qt[7] = teki[7]->GetDirectionQuaternion();

			teki_obb[7].SetAxis(teki_qt[7]);
			if (player_obb.Intersects(teki_obb[7]))
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[7] = Player->GetPosition();
				bom_state[7] = 1;
				teki_state[7] = 1;
				teki_flag[7] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else
			{

			}
		}

		if (teki_state[7] == 0)
		{
			teki_pos_x[7] = -300.0f;
			teki_pos_z[7] = -200;
			if (teki_pos_z[7] <= -200.0)
			{
				teki_state[7] = 1;
			}
		}

		if (teki_state[7] == 1)
		{
			teki_pos_z[7] += 3.0f;
			teki_pos_x[7] += 0.2f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[7] >= 1500.0)
				{
					beam_state[j] = 0;
					teki_state[7] = 0;
					teki_flag[7] = 0;
				}
			}
		}
	}

	//�퓬�@1	
	if (teki_flag[0] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[0].Center = teki[0]->GetPosition();
			teki_qt[0] = teki[0]->GetDirectionQuaternion();

			teki_obb[0].SetAxis(teki_qt[0]);
			if (player_obb.Intersects(teki_obb[0])) 
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[0] = Player->GetPosition();
				bom_state[0] = 1;
				teki_state[0] = 0;
				teki_flag[0] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else {

			}
		}

		if (teki_state[0] == 0)
		{
			teki_pos_x[0] = p_pos.x;
			teki_pos_z[0] = 2000;
			if (teki_pos_z[0] >= 2000)
			{
				teki_state[0] = 1;
			}
		}

		if (teki_state[0] == 1)
		{
			teki_pos_x[0] = p_pos.x;
			teki_pos_z[0] -= 4.0f;
			if (teki_pos_z[0] < 400) {
				teki_state[0] = 2;
			}
		}

		if (teki_state[0] == 2)
		{
			Vector3 t_pos = teki[0]->GetPosition();
			teki_pos_z[0] -= 2.5f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[0] <= -200.0)
				{
					beam_state[j] = 0;
					teki_state[0] = 0;
					teki_flag[0] = 0;
				}
			}
		}

	}

	//�퓬�@2
	if (teki_flag[1] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[1].Center = teki[1]->GetPosition();
			teki_qt[1] = teki[1]->GetDirectionQuaternion();

			teki_obb[1].SetAxis(teki_qt[1]);
			if (player_obb.Intersects(teki_obb[1])) 
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[1] = Player->GetPosition();
				bom_state[1] = 1;
				teki_state[1] = 0;
				teki_flag[1] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else {

			}
		}

		if (teki_state[1] == 0)
		{
			teki_pos_x[1] = p_pos.x;
			teki_pos_z[1] = 2000;
			if (teki_pos_z[1] >= 2000)
			{
				teki_state[1] = 1;
			}
		}

		if (teki_state[1] == 1)
		{
			teki_pos_x[1] = p_pos.x;
			teki_pos_z[1] -= 4.0f;
			if (teki_pos_z[1] < 400)
			{
				teki_state[1] = 2;
			}
		}

		if (teki_state[1] == 2)
		{
			Vector3 t_pos = teki[1]->GetPosition();
			teki_pos_z[1] -= 2.5f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[1] <= -200.0)
				{
					beam_state[j] = 0;
					teki_state[1] = 0;
					teki_flag[1] = 0;
				}
			}
		}

	}

	//�퓬�@3
	if (teki_flag[2] == 1)
	{
		if (player_invincible_time <= 0) 
		{
			teki_obb[2].Center = teki[2]->GetPosition();
			teki_qt[2] = teki[2]->GetDirectionQuaternion();

			teki_obb[2].SetAxis(teki_qt[2]);
			if (player_obb.Intersects(teki_obb[2])) 
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[2] = Player->GetPosition();
				bom_state[2] = 1;
				teki_state[2] = 0;
				teki_flag[2] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else {

			}
		}

		if (teki_state[2] == 0)
		{
			teki_pos_x[2] = p_pos.x;
			teki_pos_z[2] = 2000;
			if (teki_pos_z[2] >= 2000)
			{
				teki_state[2] = 1;
			}
		}

		if (teki_state[2] == 1)
		{
			teki_pos_x[2] = p_pos.x;
			teki_pos_z[2] -= 4.0f;
			if (teki_pos_z[2] < 42 * 00) 
			{
				teki_state[2] = 2;
			}
		}

		if (teki_state[2] == 2)
		{
			Vector3 t_pos = teki[2]->GetPosition();
			teki_pos_z[2] -= 2.5f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[2] <= -200.0)
				{
					beam_state[j] = 0;
					teki_state[2] = 0;
					teki_flag[2] = 0;
				}
			}
		}
	}

	//���
	if (teki_flag[5] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[5].Center = teki[5]->GetPosition();
			teki_qt[5] = teki[5]->GetDirectionQuaternion();

			teki_obb[5].SetAxis(teki_qt[5]);
			if (player_obb.Intersects(teki_obb[5]))
			{
				game_seane = OVER_SEANE;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[5] = Player->GetPosition();
				bom_state[5] = 1;
				teki_state[5] = 1;
				teki_flag[5] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else {

			}
		}

		if (teki_state[5] == 0)
		{
			teki_pos_z[5] -= 1.5f;
			if (teki_pos_z[5] <= 1200)
			{
				teki_pos_z[5] = 1200;

			}
		}

	}
}
void GameMain::Stage1_Time_Intialize()
{
	time_frame = 60;
	time_state = 0;
}
void GameMain::Stage1_Time_Update()
{
	//STAGE1
	//teki  0�`2:�퓬�@  3�`4�E�G�l�~�[  6�`7:���G�l�~�[  5:��� 

	if ((int)time_frame == 58 && teki_flag[5] == 0)
	{
		teki_flag[5] = 1;//���
	}

	if ((int)time_frame == 49 && teki_flag[3] == 0)
	{
		teki_flag[3] = 1;//�E�G�l�~�[1
	}

	if ((int)time_frame == 49 && teki_flag[4] == 0)
	{
		teki_flag[4] = 1;//�E�G�l�~�[2
	}

	if ((int)time_frame == 35 && teki_flag[0] == 0)
	{
		teki_flag[0] = 1;//�퓬�@1
	}

	if ((int)time_frame == 29 && teki_flag[6] == 0)
	{
		teki_flag[6] = 1;//���G�l�~�[1
	}

	if ((int)time_frame == 29 && teki_flag[7] == 0)
	{
		teki_flag[7] = 1;//���G�l�~�[2
	}

	if ((int)time_frame == 26 && teki_flag[1] == 0)
	{
		teki_flag[1] = 1;//�퓬�@2
	}

	if ((int)time_frame == 10 && teki_flag[2] == 0)
	{
		teki_flag[2] = 1;//�퓬�@3
	}

}

void GameMain::Stage2_Initialize()
{
	Stage2_Teki_Intialize();
	Stage2_Time_Intialize();

	using namespace Fujimura;
	ReadFireTimeMap().Import(_T("txt/STAGE2.txt"));
	hp_gauge = 1.0f;
}
void GameMain::Stage2_Update()
{
	Stage2_Teki_Update();
	Stage2_Time_Update();
}
void GameMain::Stage2_Teki_Intialize()
{
	//teki  0�`2:�퓬�@  3�`4�A6�`7:�G�l�~�[  5:��� 

	Vector3 p_pos = Player->GetPosition();

	//STAGE2

	for (int i = 0; i < 8; i++)
	{
		teki_state[i] = 0;
		teki_pos_y[i] = p_pos.y;
	}

	//�E�G�l�~�[
	teki_pos_x[3] = 200.0f;
	teki_pos_z[3] = -200.0f;

	teki_pos_x[4] = 300.0f;
	teki_pos_z[4] = -200.0f;

	//���G�l�~�[
	teki_pos_x[6] = -200.0f;
	teki_pos_z[6] = -200.0f;

	teki_pos_x[7] = -300.0f;
	teki_pos_z[7] = -200.0f;
	
	//���
	teki_pos_x[5] = 0;
	teki_pos_z[5] = 1500;

	//�G�l�~�[
	for (int i = 0; i <= 2; i++)
	{
		teki_pos_x[i] = p_pos.x;
		teki_pos_z[i] = 2000;
	}

}
void GameMain::Stage2_Teki_Update()
{
	//�X�e�[�W2
	Vector3 p_pos = Player->GetPosition();

	//	�E�G�l�~�[1	
	if (teki_flag[3] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[3].Center = teki[3]->GetPosition();
			teki_qt[3] = teki[3]->GetDirectionQuaternion();

			teki_obb[3].SetAxis(teki_qt[3]);
			if (player_obb.Intersects(teki_obb[3]))
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[3] = Player->GetPosition();
				bom_state[3] = 1;
				teki_state[3] = 1;
				teki_flag[3] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else 
			{

			}

			if (teki_state[3] == 0)
			{
				teki_pos_x[3] = 200.0f;
				teki_pos_y[3] = p_pos.y;
				teki_pos_z[3] = -200;
				if (teki_pos_z[3] <= -200.0)
				{
					teki_state[3] = 1;
				}
			}

			if (teki_state[3] == 1)
			{
				teki_pos_z[3] += 3.0f;
				teki_pos_x[3] -= 0.2f;
				for (int j = 0; j < 2; j++)
				{
					if (teki_pos_z[3] >= 1500.0)
					{
						beam_state[j] = 0;
						teki_state[3] = 0;
						teki_flag[3] = 0;
					}
				}
			}
		}

	}

	//	�E�G�l�~�[2	
	if (teki_flag[4] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[4].Center = teki[4]->GetPosition();
			teki_qt[4] = teki[4]->GetDirectionQuaternion();

			teki_obb[4].SetAxis(teki_qt[4]);
			if (player_obb.Intersects(teki_obb[4]))
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[4] = Player->GetPosition();
				bom_state[4] = 1;
				teki_state[4] = 1;
				teki_flag[4] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else
			{

			}
		}

		if (teki_state[4] == 0)
		{
			teki_pos_x[4] = 300.0f; //+ i * 50;
			teki_pos_y[4] = p_pos.y;
			teki_pos_z[4] = -200;
			if (teki_pos_z[4] <= -200.0)
			{
				teki_state[4] = 1;
			}
		}

		if (teki_state[4] == 1)
		{
			teki_pos_z[4] += 3.0f;
			teki_pos_x[4] -= 0.2f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[4] >= 1500.0)
				{
					beam_state[j] = 0;
					teki_state[4] = 0;
					teki_flag[4] = 0;
				}
			}
		}
	}

	//	���G�l�~�[1	
	if (teki_flag[6] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[6].Center = teki[6]->GetPosition();
			teki_qt[6] = teki[6]->GetDirectionQuaternion();

			teki_obb[6].SetAxis(teki_qt[6]);
			if (player_obb.Intersects(teki_obb[6]))
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[6] = Player->GetPosition();
				bom_state[6] = 1;
				teki_state[6] = 1;
				teki_flag[6] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else
			{

			}
		}

		if (teki_state[6] == 0)
		{
			teki_pos_x[6] = -200.0f;
			teki_pos_y[6] = p_pos.y;
			teki_pos_z[6] = -200;
			if (teki_pos_z[6] <= -200.0)
			{
				teki_state[6] = 1;
			}
		}

		if (teki_state[6] == 1)
		{
			teki_pos_z[6] += 3.0f;
			teki_pos_x[6] += 0.2f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[6] >= 1500.0)
				{
					beam_state[j] = 0;
					teki_state[6] = 0;
					teki_flag[6] = 0;
				}
			}
		}
	}

	//	���G�l�~�[2	
	if (teki_flag[7] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[7].Center = teki[7]->GetPosition();
			teki_qt[7] = teki[7]->GetDirectionQuaternion();

			teki_obb[7].SetAxis(teki_qt[7]);
			if (player_obb.Intersects(teki_obb[7]))
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[7] = Player->GetPosition();
				bom_state[7] = 1;
				teki_state[7] = 1;
				teki_flag[7] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else
			{

			}
		}

		if (teki_state[7] == 0)
		{
			teki_pos_x[7] = -300.0f; 
			teki_pos_y[7] = p_pos.y;
			teki_pos_z[7] = -200;
			if (teki_pos_z[7] <= -200.0)
			{
				teki_state[7] = 1;
			}
		}

		if (teki_state[7] == 1)
		{
			teki_pos_z[7] += 3.0f;
			teki_pos_x[7] += 0.2f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[7] >= 1500.0)
				{
					beam_state[j] = 0;
					teki_state[7] = 0;
					teki_flag[7] = 0;
				}
			}
		}
	}

	//�퓬�@1	
	if (teki_flag[0] == 1)
	{
		if (player_invincible_time <= 0) 
		{
			teki_obb[0].Center = teki[0]->GetPosition();
			teki_qt[0] = teki[0]->GetDirectionQuaternion();

			teki_obb[0].SetAxis(teki_qt[0]);
			if (player_obb.Intersects(teki_obb[0])) 
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[0] = Player->GetPosition();
				bom_state[0] = 1;
				teki_state[0] = 0;
				teki_flag[0] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else {

			}
		}

		if (teki_state[0] == 0)
		{
			teki_pos_x[0] = p_pos.x;
			teki_pos_y[0] = p_pos.y;
			teki_pos_z[0] = 2000;
			if (teki_pos_z[0] >= 2000)
			{
				teki_state[0] = 1;
			}
		}

		if (teki_state[0] == 1)
		{
			teki_pos_x[0] = p_pos.x;
			teki_pos_y[0] = p_pos.y;
			teki_pos_z[0] -= 4.0f;
			if (teki_pos_z[0] < 300) {
				teki_state[0] = 2;
			}
		}

		if (teki_state[0] == 2)
		{
			Vector3 t_pos = teki[0]->GetPosition();
			teki_pos_x[0] = t_pos.x;
			teki_pos_y[0] = t_pos.y;
			teki_pos_z[0] -= 2.5f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[0] <= -200.0)
				{
					beam_state[j] = 0;
					teki_state[0] = 0;
					teki_flag[0] = 0;
				}
			}
		}
	}

	//�퓬�@2
	if (teki_flag[1] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[1].Center = teki[1]->GetPosition();
			teki_qt[1] = teki[1]->GetDirectionQuaternion();

			teki_obb[1].SetAxis(teki_qt[1]);
			if (player_obb.Intersects(teki_obb[1]))
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[1] = Player->GetPosition();
				bom_state[1] = 1;
				teki_state[1] = 0;
				teki_flag[1] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else {

			}
		}

		if (teki_state[1] == 0)
		{
			teki_pos_x[1] = p_pos.x;
			teki_pos_y[1] = p_pos.y;
			teki_pos_z[1] = 2000;
			if (teki_pos_z[1] >= 2000)
			{
				teki_state[1] = 1;
			}
		}

		if (teki_state[1] == 1)
		{
			teki_pos_x[1] = p_pos.x;
			teki_pos_y[1] = p_pos.y;
			teki_pos_z[1] -= 4.0f;
			if (teki_pos_z[1] < 300)
			{
				teki_state[1] = 2;
			}
		}

		if (teki_state[1] == 2)
		{
			Vector3 t_pos = teki[1]->GetPosition();
			teki_pos_x[1] = t_pos.x;
			teki_pos_y[1] = t_pos.y;
			teki_pos_z[1] -= 2.5f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[1] <= -200.0)
				{
					beam_state[j] = 0;
					teki_state[1] = 0;
					teki_flag[1] = 0;
				}
			}
		}

	}

	//�퓬�@3
	if (teki_flag[2] == 1)
	{
		if (player_invincible_time <= 0) 
		{
			teki_obb[2].Center = teki[2]->GetPosition();
			teki_qt[2] = teki[2]->GetDirectionQuaternion();

			teki_obb[2].SetAxis(teki_qt[2]);
			if (player_obb.Intersects(teki_obb[2])) 
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[2] = Player->GetPosition();
				bom_state[2] = 1;
				teki_state[2] = 0;
				teki_flag[2] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else {

			}
		}


		if (teki_state[2] == 0)
		{
			teki_pos_x[2] = p_pos.x;
			teki_pos_y[2] = p_pos.y;
			teki_pos_z[2] = 2000;
			if (teki_pos_z[2] >= 2000)
			{
				teki_state[2] = 1;
			}
		}

		if (teki_state[2] == 1)
		{
			teki_pos_x[2] = p_pos.x;
			teki_pos_y[2] = p_pos.y;
			teki_pos_z[2] -= 4.0f;
			if (teki_pos_z[2] < 300)
			{
				teki_state[2] = 2;
			}
		}

		if (teki_state[2] == 2)
		{
			Vector3 t_pos = teki[2]->GetPosition();
			teki_pos_x[2] = t_pos.x;
			teki_pos_y[2] = t_pos.y;
			teki_pos_z[2] -= 2.5f;
			for (int j = 0; j < 2; j++)
			{
				if (teki_pos_z[2] <= -200.0)
				{
					beam_state[j] = 0;
					teki_state[2] = 0;
					teki_flag[2] = 0;
				}
			}
		}

	}

	//���
	if (teki_flag[5] == 1)
	{
		if (player_invincible_time <= 0)
		{
			teki_obb[5].Center = teki[5]->GetPosition();
			teki_qt[5] = teki[5]->GetDirectionQuaternion();

			teki_obb[5].SetAxis(teki_qt[5]);
			if (player_obb.Intersects(teki_obb[5])) 
			{
				hp_gauge -= 0.1f;
				damege_se->Play();
				damege_se->IsPlaying();
				if (hp_scene >= 9)
				{
					damege_se->Stop();
				}
				bom_position[5] = Player->GetPosition();
				bom_state[5] = 1;
				teki_state[5] = 1;
				teki_flag[5] = 0;
				player_invincible_time = PLYER_INVINCIBLE_TIME;
				player_flashing = FLASHING_TIME;
				//				break;
			}
			else {

			}
		}

		if (teki_state[5] == 0)
		{
			teki_pos_z[5] -= 1.5f;
			if (teki_pos_z[5] <= 1200)
			{
				teki_pos_z[5] = 1200;

			}
		}

	}
}
void GameMain::Stage2_Time_Intialize()
{
	//STAGE2
	time_frame = 90;
	time_state = 0;
}
void GameMain::Stage2_Time_Update()
{
	//STAGE2

	//teki  0�`2:�퓬�@  3�`4�A6�`7:�G�l�~�[  5:��� 
	if (((int)time_frame == 77 || (int)time_frame == 36) && teki_flag[0] == 0)
	{
		teki_flag[0] = 1;//�퓬�@1
	}

	if (((int)time_frame == 68 || (int)time_frame == 18) && teki_flag[1] == 0)
	{
		teki_flag[1] = 1;//�퓬�@2
	}

	if ((int)time_frame == 45 && teki_flag[2] == 0)
	{
		teki_flag[2] = 1;//�퓬�@3
	}

	if ((int)time_frame == 79 && teki_flag[3] == 0)
	{
		teki_flag[3] = 1;//�E�G�l�~�[1
	}

	if ((int)time_frame == 79 && teki_flag[4] == 0)
	{
		teki_flag[4] = 1;//�E�G�l�~�[2
	}

	if ((int)time_frame == 88 && teki_flag[5] == 0)
	{
		teki_flag[5] = 1;//���
	}

	if ((int)time_frame == 70 && teki_flag[6] == 0)
	{
		teki_flag[6] = 1;//���G�l�~�[1
	}

	if ((int)time_frame == 70 && teki_flag[7] == 0)
	{
		teki_flag[7] = 1;//���G�l�~�[2
	}

}