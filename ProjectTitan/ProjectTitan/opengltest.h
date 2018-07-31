#ifndef __CGLES__
#define __CGLES__

#include "define.h"
#include "camera.h"

void Init();
void SetViewportSize(FLOAT width, FLOAT height);
void Draw(UINT elapse);
void OnTouch(FLOAT x, FLOAT y);
void Move(FLOAT x, FLOAT y, FLOAT z = 0);
void RotateViewDir(FLOAT yaw, FLOAT pitch, FLOAT roll = 0);
void CameraForward(FLOAT offset);

#endif
