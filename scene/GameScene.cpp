#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "SafeDelete.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	player_ = new Player();
	enemy_ = new Enemy();

	enemy_->SetPlayer(player_);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

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
	player_->Update(viewProjection_);
	enemy_->Update();

	CheckCollision();

	viewProjection_.UpdateMatrix();
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
	
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	
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
