#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "SafeDelete.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	SafeDelete(player_);
	SafeDelete(enemy_);
}

void GameScene::Initialize() {
	player_ = new Player();
	enemy_ = new Enemy();

	enemy_->SetPlayer(player_);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, { 0,0 });
	
	textureHandleClear_ = TextureManager::Load("clear.png");
	spriteClear_ = Sprite::Create(textureHandleClear_, { 0,0 });

	textureHandleOver_ = TextureManager::Load("over.png");
	spriteOver_ = Sprite::Create(textureHandleOver_, { 0,0 });

	modelPlayer_ = Model::Create();
	textureHandlePlayer_ = TextureManager::Load("blocktest.png");

	modelEnemy_ = Model::Create();
	textureHandleEnemy_ = TextureManager::Load("blocktest2.png");

	player_->Initialize(modelPlayer_, textureHandlePlayer_);
	enemy_->Initialize(modelEnemy_, textureHandleEnemy_);

	viewProjection_.target = { 0.0f,0.0f,10.0f };
	viewProjection_.eye = { 0.0f,0.0f,-25.0f };

	viewProjection_.Initialize();

}

void GameScene::Update() {

	switch (scene_)
	{
	case 0:
		GameTitleUpdate();
		break;
	case 1:
		GamePlayUpdate();
		break;
	case 2:
		GameClearUpdate();
		break;
	case 3:
		GameOverUpdate();
		break;
	}
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	switch (scene_)
	{
	case 0:
		GameTitleDraw2D();
		break;
	case 1:
		GamePlayDraw2D();
		break;
	case 2:
		GameClearDraw2D();
		break;
	case 3:
		GameOverDraw2D();
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	switch (scene_)
	{
	case 0:
		GameTitleDraw3D();
		break;
	case 1:
		GamePlayDraw3D();
		break;
	case 2:
		GameClearDraw3D();
		break;
	case 3:
		GameOverDraw3D();
		break;
	}

	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckCollision()
{
	EenmyBulletPlayer();
	PlayerBulletEnmey();
}

void GameScene::EenmyBulletPlayer()
{
	Vector3 posA, posB;

	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

	posA = player_->GetWorldPosition();

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();

		if (
			(posA.x - posB.x) * (posA.x - posB.x) +
			(posA.y - posB.y) * (posA.y - posB.y) +
			(posA.z - posB.z) * (posA.z - posB.z)
			<= (1.0f + 1.0f) * (1.0f + 1.0f)
			) {
			bullet->Collision();
			player_->Collision();
		}
	}
}

void GameScene::PlayerBulletEnmey()
{
	Vector3 posA, posB;

	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	posA = enemy_->GetWorldPosition();

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		posB = bullet->GetWorldPosition();

		if (
			(posA.x - posB.x) * (posA.x - posB.x) +
			(posA.y - posB.y) * (posA.y - posB.y) +
			(posA.z - posB.z) * (posA.z - posB.z)
			<= (4.0f + 1.0f) * (2.0f + 1.0f)
			) {
			bullet->Collision();
			enemy_->Collision();
		}
	}
}


/// <summary>
/// 更新
/// </summary>
void GameScene::GameTitleUpdate()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		player_->ReInitialize();
		enemy_->ReInitialize();
		scene_ = 1;
	}
}

void GameScene::GamePlayUpdate()
{
	player_->Update(viewProjection_);
	enemy_->Update();

	CheckCollision();

	if (enemy_->GetHealth() < 0) {
		scene_ = 2;
	}

	if (player_->GetHealth() < 0) {
		scene_ = 3;
	}

	viewProjection_.UpdateMatrix();
}

void GameScene::GameClearUpdate()
{
}

void GameScene::GameOverUpdate()
{
}


/// <summary>
/// 2D描画
/// </summary>
void GameScene::GameTitleDraw2D()
{
	spriteTitle_->Draw();
}

void GameScene::GamePlayDraw2D()
{
}

void GameScene::GameClearDraw2D()
{
	spriteClear_->Draw();
}

void GameScene::GameOverDraw2D()
{
	spriteOver_->Draw();
}


/// <summary>
/// 3D描画
/// </summary>
void GameScene::GameTitleDraw3D()
{
}

void GameScene::GamePlayDraw3D()
{
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

}

void GameScene::GameClearDraw3D()
{
}

void GameScene::GameOverDraw3D()
{
}
