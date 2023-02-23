#pragma once
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Audio.h"

#include "EnemyBullet.h"

class Player;

class Enemy
{
public:
	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Shoot();

	void SetPlayer(Player* player) {
		player_ = player;
	}

	Vector3 GetWorldPosition();

	void Collision();

	void ReInitialize();

	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	int GetHealth() { return health; }

private:
	Player* player_ = nullptr;

	Model* model_ = nullptr;



	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	Audio* audio_ = nullptr;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

private:
	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0;

	float shootSecond = 1.5;

	float shootTime = shootSecond * 60;

	int health = 30;

	Vector3 velocity;
	Vector3 velocity2;
	Vector3 velocity3;
	Vector3 velocity4;
};