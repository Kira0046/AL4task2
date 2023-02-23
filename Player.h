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

#include "PlayerBullet.h"

#include <memory>
#include <list>

class Player
{
public:
	void Initialize(Model* model, uint32_t textureHandle);

	void Update(ViewProjection& viewProjection);

	void Draw(ViewProjection& viewProjection);

	void Shoot();

	void Collision();

	void ReInitialize();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	Vector3 GetWorldPosition();

private:
	Model* model_ = nullptr;
	
	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	Audio* audio_ = nullptr;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

private:
	WorldTransform worldTransform_;

	uint32_t textureHandle_ = 0;
	
	int health = 5;
};