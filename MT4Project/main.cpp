#include <Novice.h>
#include <math.h>
#include <cmath>
#include <numbers>
#include "../DirectXGame/math/Vector3.h"
#include "../DirectXGame/math/Matrix4x4.h"

static const int kColumnWidth = 60;
static const int kRowHeight = 20;

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label);
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
Quaternion IdentityQuaternion(); // 単位Quaternion
Quaternion Conjugate(const Quaternion& quaternion); // 共役
float Norm(const Quaternion& quaternion); // ノーム
Quaternion Normalize(const Quaternion& quaternion);
Quaternion Inverse(const Quaternion& quaternion);

const char kWindowTitle[] = "MT4";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		QuaternionScreenPrintf(0, kRowHeight * 0, identity, "Identity");
		QuaternionScreenPrintf(0, kRowHeight * 1, conj, "Conjugate");
		QuaternionScreenPrintf(0, kRowHeight * 2, inv, "Inverse");
		QuaternionScreenPrintf(0, kRowHeight * 3, normal, "Normalize");
		QuaternionScreenPrintf(0, kRowHeight * 4, mul1, "Multiply(q1, q2)");
		QuaternionScreenPrintf(0, kRowHeight * 5, mul2, "Multiply(q2, q1)");
		Novice::ScreenPrintf(0, kRowHeight * 6, "%.02f", norm);
		Novice::ScreenPrintf(kColumnWidth * 4, kRowHeight * 6, ": Norm");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	return Quaternion{
		lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x,
		lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y,
		lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	};
}

Quaternion IdentityQuaternion() {
	return Quaternion{ 0,0,0,1 };
}

Quaternion Conjugate(const Quaternion& quaternion) {
	return Quaternion(
		-quaternion.x,
		-quaternion.y,
		-quaternion.z,
		quaternion.w
	);
}

float Norm(const Quaternion& quaternion) {
	return sqrtf(
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z +
		quaternion.w * quaternion.w
	);
}

Quaternion Normalize(const Quaternion& quaternion) {
	// 長さ
	float norm = Norm(quaternion);

	if (norm != 0.0f) // ゼロ除算対策
	{
		return Quaternion{
			quaternion.x / norm,
			quaternion.y / norm,
			quaternion.z / norm,
			quaternion.w / norm,
		};
	}

	return quaternion;
}

Quaternion Inverse(const Quaternion& quaternion) {
	Quaternion conj = Conjugate(quaternion);
	float norm = Norm(quaternion);
	return Quaternion{
		conj.x / (norm * norm),
		conj.y / (norm * norm),
		conj.z / (norm * norm),
		conj.w / (norm * norm),
	};
}

void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", quaternion.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", quaternion.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", quaternion.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%.02f", quaternion.w);
	Novice::ScreenPrintf(x + kColumnWidth * 4, y, ": %s", label);
}