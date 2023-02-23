#include "Enemy.h"
#include <cassert>

#include "Procession.h"
#include "Player.h"
using namespace MathUtility;
using namespace DirectX;

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.scale_ = { 3.0f,3.0f,3.0f };
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translation_ = { 0.0f,0.0f,+22.0f };

	worldTransform_.Initialize();

	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

}

void Enemy::Update()
{


	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	shootTime -= 1;
	if (shootTime < 0) {
		Shoot();
		shootTime = shootSecond * 60;
	}


	worldTransform_.matWorld_ = Mat_Identity();
	worldTransform_.matWorld_ = MatWorld(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Shoot()
{
	assert(player_);

	Vector3 distance(0, 0, 0);
	float ALLdistance = 0;

	distance.x = player_->GetWorldPosition().x - GetWorldPosition().x;
	distance.y = player_->GetWorldPosition().y - GetWorldPosition().y;
	distance.z = player_->GetWorldPosition().z - GetWorldPosition().z;

	ALLdistance = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);

	velocity.x = distance.x / ALLdistance;
	velocity.y = distance.y / ALLdistance;
	velocity.z = distance.z / ALLdistance;

	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(std::move(newBullet));

	Vector3 distance2(0, 0, 0);
	float ALLdistance2 = 0;

	distance2.x = (player_->GetWorldPosition().x - rand() % 7 + 1) - GetWorldPosition().x;
	distance2.y = (player_->GetWorldPosition().y - rand() % 7 + 1) - GetWorldPosition().y;
	distance2.z = (player_->GetWorldPosition().z - rand() % 7 + 1) - GetWorldPosition().z;

	ALLdistance2 = sqrt(distance2.x * distance2.x + distance2.y * distance2.y + distance2.z * distance2.z);

	velocity2.x = distance2.x / ALLdistance2;
	velocity2.y = distance2.y / ALLdistance2;
	velocity2.z = distance2.z / ALLdistance2;

	velocity2 *= 2;

	std::unique_ptr<EnemyBullet> newBullet2 = std::make_unique<EnemyBullet>();
	newBullet2->Initialize(model_, worldTransform_.translation_, velocity2);

	bullets_.push_back(std::move(newBullet2));

	//fe3
	Vector3 distance3(0, 0, 0);
	float ALLdistance3 = 0;

	distance3.x = (player_->GetWorldPosition().x + rand() % 7 + 1) - GetWorldPosition().x;
	distance3.y = (player_->GetWorldPosition().y + rand() % 7 + 1) - GetWorldPosition().y;
	distance3.z = (player_->GetWorldPosition().z + rand() % 7 + 1) - GetWorldPosition().z;

	ALLdistance3 = sqrt(distance3.x * distance3.x + distance3.y * distance3.y + distance3.z * distance3.z);

	velocity3.x = distance3.x / ALLdistance3;
	velocity3.y = distance3.y / ALLdistance3;
	velocity3.z = distance3.z / ALLdistance3;

	velocity3 *= 2;

	std::unique_ptr<EnemyBullet> newBullet3 = std::make_unique<EnemyBullet>();
	newBullet3->Initialize(model_, worldTransform_.translation_, velocity3);

	bullets_.push_back(std::move(newBullet3));

	//fe4
	Vector3 distance4(0, 0, 0);
	float ALLdistance4 = 0;

	distance4.x = (player_->GetWorldPosition().x - rand() % 7 + 1) - GetWorldPosition().x;
	distance4.y = (player_->GetWorldPosition().y - rand() % 7 + 1) - GetWorldPosition().y;
	distance4.z = (player_->GetWorldPosition().z - rand() % 7 + 1) - GetWorldPosition().z;

	ALLdistance4 = sqrt(distance4.x * distance4.x + distance4.y * distance4.y + distance4.z * distance3.z);

	velocity4.x = distance4.x / ALLdistance4;
	velocity4.y = distance4.y / ALLdistance4;
	velocity4.z = distance4.z / ALLdistance4;

	velocity4 *= 2;

	std::unique_ptr<EnemyBullet> newBullet4 = std::make_unique<EnemyBullet>();
	newBullet4->Initialize(model_, worldTransform_.translation_, velocity4);

	bullets_.push_back(std::move(newBullet4));
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::Collision()
{
	health -= 1;
}

void Enemy::ReInitialize()
{
	int health = 30;
}
