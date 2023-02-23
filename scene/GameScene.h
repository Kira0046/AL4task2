#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Player.h"
#include "Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void CheckCollision();

	void EenmyBulletPlayer();

	void PlayerBulletEnmey();

	//Update
	void GameTitleUpdate();

	void GamePlayUpdate();

	void GameClearUpdate();

	void GameOverUpdate();

	//2D
	void GameTitleDraw2D();

	void GamePlayDraw2D();

	void GameClearDraw2D();

	void GameOverDraw2D();

	//3D
	void GameTitleDraw3D();

	void GamePlayDraw3D();

	void GameClearDraw3D();

	void GameOverDraw3D();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	ViewProjection viewProjection_;

	Model* modelPlayer_ = nullptr;
	uint32_t textureHandlePlayer_ = 0;

	Model* modelEnemy_ = nullptr;
	uint32_t textureHandleEnemy_ = 0;

	int scene_ = 0;
};
