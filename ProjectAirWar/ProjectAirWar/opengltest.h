#ifndef __CGLES__
#define __CGLES__

#include "define.h"


void Init();
void SetViewportSize(FLOAT width, FLOAT height);
void Draw(UINT elapse);
void OnTouch(UINT event, FLOAT x, FLOAT y);
void OnKey(UINT event, CHAR chr);

#endif
