//*****************************************************************************
// Camera
// ------
//   Handles managing the location and rotation of the camera.
//   
//   This is essentially a placeholder until a real system is designed.
//
// (c) May 2013
// @author Doug Frazer
//*****************************************************************************

#include "library/ASPLib.h"

class CAMERA
{
public:
	void Init();
	void Update();
	void Draw();
	void HandleMousePressed(int button, int state, int x, int y);
	vector4 GetLocation();
	float GetAngle() { return Angle; }

private:
	float Angle;
	vector4 Focus;

	struct MOUSE_CLICK_LOCATION {
		int x;
		int y;
	};

	MOUSE_CLICK_LOCATION Current;
	MOUSE_CLICK_LOCATION Previous;
	int MouseState;
};

void Camera_Update();
void Camera_HandleMousePressed(int button, int state, int x, int y);
void Camera_Init();
vector4 Camera_GetLocation();
float Camera_GetAngle();
void Camera_Draw();
