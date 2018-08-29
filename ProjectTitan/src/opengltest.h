#ifndef __CGLES__
#define __CGLES__

#include "core/define.h"


void Init();
void SetViewportSize(FLOAT width, FLOAT height);
void Draw(FLOAT elapse);
void OnTouch(UINT event, FLOAT x, FLOAT y);
void OnKey(UINT event, CHAR chr);


#endif
