#include <Novice.h>
#include <math.h>
#include <cmath>
#include <numbers>
#include "../DirectXGame/math/Vector3.h"
#include "../DirectXGame/math/Matrix4x4.h"

float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 任意の方向への回転行列を求める
/// </summary>
/// <param name="from">(正規化)元のベクトル</param>
/// <param name="to">(正規化)回転後ベクトル</param>
/// <returns></returns>
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

const char kWindowTitle[] = "MT4";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 from0 = Normalize(Vector3{ 1.0f,0.7f,0.5f });
	Vector3 to0 = { -from0.x,-from0.y,-from0.z };
	Vector3 from1 = Normalize(Vector3{ -0.6f,0.9f,0.2f });
	Vector3 to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });
	Matrix4x4 rotateMatrix0 = DirectionToDirection(Normalize(Vector3{ 1.0f,0.0f,0.0f }), Normalize(Vector3{ -1.0f,0.0f,0.0f }));
	Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
	Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

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

		MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
		MatrixScreenPrintf(0, 100, rotateMatrix1, "rotateMatrix1");
		MatrixScreenPrintf(0, 200, rotateMatrix2, "rotateMatrix2");

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
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
};
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
}

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

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = { 0 };
	result.m[0][0] = std::cosf(radian);
	result.m[0][1] = std::sinf(radian);
	result.m[1][0] = -std::sinf(radian);
	result.m[1][1] = std::cosf(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
};

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
	Matrix4x4 r{};
	if (to.x == -from.x && to.y == -from.y && to.z == -from.z) {
		// ベクトルが真逆のとき
		return MakeRotateZMatrix(float(std::numbers::pi)); // 180度回転
	} else {
		float cosTheta = Dot(from, to);
		float sinTheta = Length(Cross(from, to));
		Vector3 n{};
		n = Normalize(Cross(from, to));

		// 任意軸回転行列
		r = {
			n.x * n.x * (1 - cosTheta) + cosTheta,
			n.x * n.y * (1 - cosTheta) + n.z * sinTheta,
			n.x * n.z * (1 - cosTheta) - n.y * sinTheta,
			0,
			n.x * n.y * (1 - cosTheta) - n.z * sinTheta,
			n.y * n.y * (1 - cosTheta) + cosTheta,
			n.y * n.z * (1 - cosTheta) + n.x * sinTheta,
			0,
			n.x * n.z * (1 - cosTheta) + n.y * sinTheta,
			n.y * n.z * (1 - cosTheta) - n.x * sinTheta,
			n.z * n.z * (1 - cosTheta) + cosTheta,
			0,
			0,
			0,
			0,
			1
		};
	}
	return r;
}