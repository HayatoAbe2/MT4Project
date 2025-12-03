#include <Novice.h>
#include <math.h>
#include "../DirectXGame/math/Vector3.h"
#include "../DirectXGame/math/Matrix4x4.h"

float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

const char kWindowTitle[] = "MT4";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 axis = Normalize({ 1.0f, 1.0f, 1.0f });
	float angle = 0.44f;
	Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);

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

		MatrixScreenPrintf(0, 0, rotateMatrix, "rotateMatrix");

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

float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

float Length(const Vector3& v) { return sqrtf(Dot(v, v)); }

Vector3 Normalize(const Vector3& v) {
	if (Length(v) <= 0.0f) {
		return { 0, 0, 0 };
	} // ゼロ除算を回避
	Vector3 result;
	result.x = v.x / Length(v);
	result.y = v.y / Length(v);
	result.z = v.z / Length(v);
	return result;
}

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
};

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	static const int kColumnWidth = 60;
	static const int kRowHeight = 20;

	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight + kRowHeight, "%6.03f", matrix.m[row][column]);
		}
	}
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
	// 拡大縮小行列
	Matrix4x4 s{};
	s.m[0][0] = cosf(angle);
	s.m[1][1] = cosf(angle);
	s.m[2][2] = cosf(angle);
	s.m[3][3] = 1.0f;

	Matrix4x4 p{};
	p.m[0][0] = (1 - cosf(angle)) * axis.x * axis.x;
	p.m[0][1] = (1 - cosf(angle)) * axis.x * axis.y;
	p.m[0][2] = (1 - cosf(angle)) * axis.x * axis.z;
	p.m[1][0] = (1 - cosf(angle)) * axis.y * axis.x;
	p.m[1][1] = (1 - cosf(angle)) * axis.y * axis.y;
	p.m[1][2] = (1 - cosf(angle)) * axis.y * axis.z;
	p.m[2][0] = (1 - cosf(angle)) * axis.z * axis.x;
	p.m[2][1] = (1 - cosf(angle)) * axis.z * axis.y;
	p.m[2][2] = (1 - cosf(angle)) * axis.z * axis.z;
	p.m[3][3] = 1.0f;

	Matrix4x4 c{};
	c.m[0][0] = -sinf(angle) * 0;
	c.m[0][1] = -sinf(angle) * (-axis.z);
	c.m[0][2] = -sinf(angle) * axis.y;
	c.m[1][0] = -sinf(angle) * axis.z;
	c.m[1][1] = -sinf(angle) * 0;
	c.m[1][2] = -sinf(angle) * (-axis.x);
	c.m[2][0] = -sinf(angle) * (-axis.y);
	c.m[2][1] = -sinf(angle) * axis.x;
	c.m[2][2] = -sinf(angle) * 0;

	Matrix4x4 result = Add(Add(s, p), c);

	return result;
}