//*****************************************************************************
// Camera
//
// (c) May 2013
// @author Doug Frazer
//*****************************************************************************


#include "GL/glut.h"
#include <math.h>

#include "camera.h"
#include "player.h"
#include "keyboard.h"

#define PI 3.14159265

static CAMERA Camera;

void Camera_ZoomIn() { Camera.ZoomFactor += 1; }
void Camera_ZoomOut() { Camera.ZoomFactor -= 1; }

//*****************************************************************************
// Public functions
//*****************************************************************************
void CAMERA::Init()
{
    ZoomFactor = 10.0;
    MouseState = GLUT_UP;
    Previous.x = 0;
    Previous.y = 0;
    Current.x = 0;
    Current.y = 0;
    Keyboard_RegisterEvent('z', Camera_ZoomIn);
    Keyboard_RegisterEvent('x', Camera_ZoomOut);
}
//*****************************************************************************
void CAMERA::Update()
{
    switch(MouseState) {
        case GLUT_DOWN:
            Angle = Previous.x > Current.x ? Angle + 10 : Angle - 10;
            break;
        default:
            break;
    }

}
//*****************************************************************************
void CAMERA::Draw()
{
    // TODO: why does this need to be done during draw?
    // Set Camera
    vector4 PlayerPosition;
    if(Player_GetPrimaryPlayerPosition(&PlayerPosition)) {
        vector4 CameraLocation = GetLocation();
        gluLookAt( CameraLocation.x, CameraLocation.y, CameraLocation.z,
                   PlayerPosition.x, PlayerPosition.y, PlayerPosition.z, 
                   0.0, 1.0, 0.0 );
    } else {
        gluLookAt( 0.0, ZoomFactor, ZoomFactor, 
                   0.0, 0.0, 0.0, 
                   0.0, 1.0, 0.0 );
    }
}
//*****************************************************************************
void CAMERA::HandleMousePressed(int button, int state, int x, int y)
{
    Previous = Current;
    Current.x = x;
    Current.y = y;
    MouseState = state;
}
//*****************************************************************************
vector4 CAMERA::GetLocation()
{
    vector4 PlayerPosition;
    vector4 Location = { 0.0, 0.0, 0.0, 0.0 };
    if(Player_GetPrimaryPlayerPosition(&PlayerPosition)) {
        Location.x = PlayerPosition.x + sin(Angle*PI/180)*ZoomFactor;
        Location.y = PlayerPosition.y + ZoomFactor;
        Location.z = PlayerPosition.z + cos(Angle*PI/180)*ZoomFactor;
    }
    return Location;
}

//*****************************************************************************
// C-Style interface
//*****************************************************************************
void Camera_Update() { Camera.Update(); }
void Camera_HandleMousePressed(int button, int state, int x, int y) { Camera.HandleMousePressed(button, state, x, y); }
void Camera_Init() { Camera.Init(); }
vector4 Camera_GetLocation() { return Camera.GetLocation(); }
float Camera_GetAngle() { return Camera.GetAngle(); }
void Camera_Draw() { Camera.Draw(); }
//*****************************************************************************
