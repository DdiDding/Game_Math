
#include "Precompiled.h"
#include "SoftRenderer.h"
#include <random>
using namespace CK::DD;

// ���ڸ� �׸��� �Լ�
void SoftRenderer::DrawGizmo2D()
{
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// �׸��� ����
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// ���� ���� ���
	Vector2 viewPos = g.GetMainCamera().GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// ���� �ϴܿ������� ���� �׸���
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// �׸��尡 ���۵Ǵ� ���ϴ� ��ǥ �� ���
	Vector2 minPos = viewPos - extent;
	Vector2 minGridPos = Vector2(ceilf(minPos.X / (float)_Grid2DUnit), ceilf(minPos.Y / (float)_Grid2DUnit)) * (float)_Grid2DUnit;
	ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(_ScreenSize, minGridPos - viewPos);

	for (int ix = 0; ix < xGridCount; ++ix)
	{
		r.DrawFullVerticalLine(gridBottomLeft.X + ix * _Grid2DUnit, gridColor);
	}

	for (int iy = 0; iy < yGridCount; ++iy)
	{
		r.DrawFullHorizontalLine(gridBottomLeft.Y - iy * _Grid2DUnit, gridColor);
	}

	ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
	r.DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
	r.DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}

// ���� ������Ʈ ���


// ���� �� �ε��� ����ϴ� �Լ�
void SoftRenderer::LoadScene2D()
{
	// ���� �� �ε����� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();

}

// ���� ������ ������ ������ �����ϴ� ����
Vector2 currentPosition(100.f, 100.f);
float currentScale[2] = { 10.f, 10.f };
static float elapsedTime[2] = { 0.f, 0.f };
static float scaleSpeed = 0.5f;
static float alpha; 
static float scaleRad;

// ���� ������ ����ϴ� �Լ�
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// ���� �������� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();
	const InputManager& input = g.GetInputManager();

	// ���� ������ ���� ����
	static float moveSpeed = 100.f;
	static float scaleMin = 0.f;
	static float scaleMax = 20.f;

	// Calculate position
	{
		Vector2 inputVector = Vector2(input.GetAxis(InputAxis::XAxis), input.GetAxis(InputAxis::YAxis)).GetNormalize();
		Vector2 deltaPosition = inputVector * moveSpeed * InDeltaSeconds;
		currentPosition += deltaPosition;

		scaleSpeed += input.GetAxis(InputAxis::ZAxis) * InDeltaSeconds;
	}

	// Calculate scale
	{
		static float duration = 0.5f;
		// elapsedTime�� 0 ~ 1
		elapsedTime[0] += InDeltaSeconds * scaleSpeed;
		elapsedTime[0] = Math::FMod(elapsedTime[0], duration);
		elapsedTime[0] = Math::FMod(elapsedTime[0], duration);

		//radian 0 ~ 2pi(6.28f) ������ �� ���
		scaleRad = elapsedTime[0] * Math::TwoPI;
		//�� ������ ���� �ϱ� ���� sin�׷����� ���� �ø�, �׷��� �ʹ� Ŀ���� �� ����
		alpha = sinf(scaleRad);
		currentScale[0] = Math::Lerp(scaleMin, scaleMax, alpha);
	}
}

// ������ ������ ����ϴ� �Լ�
void SoftRenderer::Render2D()
{
	// ������ �������� ����ϴ� ��� �� �ֿ� ���۷���
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// ��濡 ���� �׸���
	DrawGizmo2D();

	// ������ ������ ���� ����
	float rad = 0.f;
	static float inc = 0.001f;
	static std::vector<Vector2> hearts;

	if (hearts.empty() == true)
	{
		for (rad = 0.f; rad < Math::TwoPI; rad += inc)
		{
			float sin = sinf(rad);
			float cos = cosf(rad);
			float cos2 = cosf(2 * rad);
			float cos3 = cosf(3 * rad);
			float cos4 = cosf(4 * rad);
			float x = 16.f * sin * sin * sin;
			float y = 13 * cos - 5 * cos2 - 2 * cos3 - cos4;
			hearts.push_back(Vector2(x, y));
		}
	}

	for (auto const& v : hearts)
	{
		r.DrawPoint(v * currentScale[0] + currentPosition, LinearColor::Blue);
		//r.DrawPoint(v * currentScale[1] + currentPosition, LinearColor::Red);
	}

	r.PushStatisticText("Coodinate : " + currentPosition.ToString());
	r.PushStatisticText("currnetLength : " + std::to_string(currentPosition.Size()));
	r.PushStatisticText("elapsedTime : " + std::to_string(elapsedTime[0]));
	r.PushStatisticText("ScaleSpeed: " + std::to_string(scaleSpeed));
	r.PushStatisticText("alpha: " + std::to_string(alpha));
	r.PushStatisticText("scaleRad: " + std::to_string(scaleRad));
	r.PushStatisticText("currentScale: " + std::to_string(currentScale[0]));
}

// �޽ø� �׸��� �Լ�
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
}

// �ﰢ���� �׸��� �Լ�
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
}
