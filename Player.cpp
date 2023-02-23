#include "Player.h"
#include <cassert>

#include "Procession.h"
using namespace MathUtility;
using namespace DirectX;

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translation_ = { 0.0f,0.0f,-8.0f };

	worldTransform_.Initialize();

	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

}

void Player::Update(ViewProjection& viewProjection)
{
	{
		/// <summary>
		/// 機体,カメラ移動////////////////////////////////////////
		/// </summary>
		//Y:4,-6 X:-10,10
		float dcpX = worldTransform_.translation_.x - viewProjection.eye.x;
		float dcpY = worldTransform_.translation_.y - viewProjection.eye.y;

		if (input_->PushKey(DIK_W) == 1) {
			//位置移動
			worldTransform_.translation_.y += 0.5;
			//回転
			worldTransform_.rotation_.x -= 0.1f;
			if (worldTransform_.rotation_.x < -0.4) {
				worldTransform_.rotation_.x = -0.4f;
			}
			//カメラ移動
			if (dcpY > 4) {
				viewProjection.eye.y += 0.5;
				viewProjection.target.y += 0.5;
			}
		}
		if (input_->PushKey(DIK_S) == 1) {
			//位置移動
			worldTransform_.translation_.y -= 0.5;
			//回転
			worldTransform_.rotation_.x += 0.1f;
			if (worldTransform_.rotation_.x > 0.4) {
				worldTransform_.rotation_.x = 0.4f;
			}
			//カメラ移動
			if (dcpY < -6) {
				viewProjection.eye.y -= 0.5;
				viewProjection.target.y -= 0.5;
			}
		}
		if (input_->PushKey(DIK_D) == 1) {
			//位置移動
			worldTransform_.translation_.x += 0.5;
			//回転
			worldTransform_.rotation_.y += 0.1f;
			if (worldTransform_.rotation_.y > 0.4) {
				worldTransform_.rotation_.y = 0.4f;
			}

			//カメラ移動
			if (dcpX > 10) {
				viewProjection.eye.x += 0.5;
				viewProjection.target.x += 0.5;
			}
		}
		if (input_->PushKey(DIK_A) == 1) {
			//位置移動
			worldTransform_.translation_.x -= 0.5;
			//回転
			worldTransform_.rotation_.y -= 0.1f;
			if (worldTransform_.rotation_.y < -0.4) {
				worldTransform_.rotation_.y = -0.4f;
			}

			//カメラ移動
			if (dcpX < -10) {
				viewProjection.eye.x -= 0.5;
				viewProjection.target.x -= 0.5;
			}
		}
		///	<summary>
		/// /////////////////////////////////////////////////
		/// <summary>

		/// <summary>
		/// 機体制御/////////////////////////////////////////
		/// </summary>

		if (input_->PushKey(DIK_W) == 0 && input_->PushKey(DIK_S) == 0) {
			if (worldTransform_.rotation_.x > 0) {
				worldTransform_.rotation_.x -= 0.25;
				if (worldTransform_.rotation_.x < 0) {
					worldTransform_.rotation_.x = 0;
				}
			}
			if (worldTransform_.rotation_.x < 0) {
				worldTransform_.rotation_.x += 0.25;
				if (worldTransform_.rotation_.x > 0) {
					worldTransform_.rotation_.x = 0;
				}
			}
		}

		if (input_->PushKey(DIK_A) == 0 && input_->PushKey(DIK_D) == 0) {
			if (worldTransform_.rotation_.y > 0) {
				worldTransform_.rotation_.y -= 0.25;
				if (worldTransform_.rotation_.y < 0) {
					worldTransform_.rotation_.y = 0;
				}
			}
			if (worldTransform_.rotation_.y < 0) {
				worldTransform_.rotation_.y += 0.25;
				if (worldTransform_.rotation_.y > 0) {
					worldTransform_.rotation_.y = 0;
				}
			}
		}

		/// <summary>
		/// ////////////////////////////////////////////////
		/// </summary>
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		Shoot();
	}

	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
	});

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Shoot()
{
	Vector3 velocity(0.0f, 0.0f, +1.0f);

	std::unique_ptr<PlayerBullet>newBullet = std::make_unique<PlayerBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(std::move(newBullet));
}

void Player::Collision()
{
	health -= 1;
}

void Player::ReInitialize()
{
	int health = 5;
}

Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
