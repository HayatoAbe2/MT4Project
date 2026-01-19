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

float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);
Vector3 TransformVector(const Vector3& vector, const Matrix4x4 matrix);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label);
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
Quaternion IdentityQuaternion(); // 単位Quaternion
Quaternion Conjugate(const Quaternion& quaternion); // 共役
float Norm(const Quaternion& quaternion); // ノーム
Quaternion Normalize(const Quaternion& quaternion);
Quaternion Inverse(const Quaternion& quaternion);

// 任意軸回転→Quaternion
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

// ベクトルをQuaternionで回転
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

// Quaternion→回転行列
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

const char kWindowTitle[] = "MT4";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f, -0.9f, 1.3f };
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = TransformVector(pointY, rotateMatrix);

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

		QuaternionScreenPrintf(0, kRowHeight * 0, rotation, "rotation");
		MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");
		VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, "rotateByQuaternion");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, "rotateByMatrix");

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

Vector3 TransformVector(const Vector3& vector, const Matrix4x4 matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
	if (w == 0.0f) {
		return { 0.0f,0.0f,0.0f };
	}
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
};

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 4, y, ": %s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + row * kRowHeight + kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
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

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	return Quaternion{
		axis.x * sinf(angle / 2.0f),
		axis.y * sinf(angle / 2.0f),
		axis.z * sinf(angle / 2.0f),
		cosf(angle / 2.0f)
	};
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	Quaternion vec{ vector.x, vector.y, vector.z, 0 };
	Quaternion normalized = Normalize(quaternion);
	Quaternion result = Multiply(Multiply(normalized, vec), Inverse(normalized));

	return Vector3{ result.x, result.y, result.z };
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {
	return Matrix4x4{
		quaternion.w * quaternion.w + quaternion.x * quaternion.x - quaternion.y * quaternion.y - quaternion.z * quaternion.z,
		2.0f * (quaternion.x * quaternion.y + quaternion.w * quaternion.z),
		2.0f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y),
		0,
		2.0f * (quaternion.x * quaternion.y - quaternion.w * quaternion.z),
		quaternion.w * quaternion.w - quaternion.x * quaternion.x + quaternion.y * quaternion.y - quaternion.z * quaternion.z,
		2.0f * (quaternion.y * quaternion.z + quaternion.w * quaternion.x),
		0,
		2.0f * (quaternion.x * quaternion.z + quaternion.w * quaternion.y),
		2.0f * (quaternion.y * quaternion.z - quaternion.w * quaternion.x),
		quaternion.w * quaternion.w - quaternion.x * quaternion.x - quaternion.y * quaternion.y + quaternion.z * quaternion.z,
		0,
		0,
		0,
		0,
		1
	};
}

void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", quaternion.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", quaternion.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", quaternion.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%.02f", quaternion.w);
	Novice::ScreenPrintf(x + kColumnWidth * 4, y, ": %s", label);
}