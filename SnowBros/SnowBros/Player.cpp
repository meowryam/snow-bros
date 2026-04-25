#include "Player.h"

const float  Player::FRAME_TIME = 0.12f;
const double Player::GRAVITY = 900.0;
const double Player::JUMP_FORCE = -420.0;
const double Player::MOVE_SPEED = 200.0;

const Player::AnimRow Player::ANIM_IDLE = { 63,   265, 1, 184 };
const Player::AnimRow Player::ANIM_WALK = { 63,   265, 5, 184 };
const Player::AnimRow Player::ANIM_JUMP = { 374,  249, 3, 193 };
const Player::AnimRow Player::ANIM_THROW = { 977,  302, 6, 259 };
const Player::AnimRow Player::ANIM_DEAD = { 2967, 299, 5, 202 };
const Player::AnimRow Player::ANIM_BALLOON = { 2505, 449, 6, 230 };