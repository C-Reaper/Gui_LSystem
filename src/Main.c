#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/LSystem.h"



u64 iterations = 0;
LSystem ls;
TransformedView tv;

void Setup(AlxWindow* w){
	tv = TransformedView_Make(
		(Vec2){ GetWidth(),GetHeight() },
		(Vec2){ 0.0f,0.0f },
		(Vec2){ 0.1f,0.1f },
		(float)GetWidth() / (float)GetHeight()
	);
    AlxFont_Resize(&window.font,50,50);

    ls = LSystem_New();
    LSystem_SetAxiom(&ls,"F");
    LSystem_Set(&ls,'F',"FF+[+F-F-F]-[-F+F+F]");
}

void Update(AlxWindow* w){
    tv.ZoomSpeed = (float)w->ElapsedTime;
	TransformedView_HandlePanZoom(&tv,window.Strokes,(Vec2){ GetMouse().x,GetMouse().y });
	TransformedView_Output(&tv,(Vec2){ GetWidth(),GetHeight() });
    LSystem_Reset(&ls);

    if(Stroke(ALX_KEY_SPACE).PRESSED){
        iterations++;
        LSystem_Iter(&ls);
    }

	Clear(BLACK);

    for (unsigned int i = 0; i < ls.out.size; i++) {
		unsigned char ch = (unsigned char)String_Get(&ls.out,i);
		
        if(ch == 'F'){
            const Vec2 target = { 0.0f,1.0f };
            LSystem_Translate(&ls,target);

            const Vec2 p0 = TransformedView_WorldScreenPos(&tv,LSystem_Calc(&ls,(Vec2){ 0.0f,0.0f }));
            const Vec2 p1 = TransformedView_WorldScreenPos(&tv,LSystem_Calc(&ls,target));
            Line_RenderX(WINDOW_STD_ARGS,p0,p1,WHITE,1.0f);
        }else if(ch == '+'){
            LSystem_Rotate(&ls,F32_PI05 * 0.33f);
        }else if(ch == '-'){
            LSystem_Rotate(&ls,-F32_PI05 * 0.33f);
        }else if(ch == '['){
            LSystem_Push(&ls);
        }else if(ch == ']'){
            LSystem_Pop(&ls);
        }
    }
    
    
    CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0.0f,0.0f,RED,"IT: %d",iterations);
}

void Delete(AlxWindow* w){
    LSystem_Free(&ls);
}

int main(){
    if(Create("L-System",1800,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}