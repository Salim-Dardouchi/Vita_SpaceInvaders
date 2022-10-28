#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int TextInitGraph(TTF_Font* pFont, SDL_Color cColor, SDL_Renderer* pRenderer);
int TextReleaseGraph(void);

typedef struct s_text t_text;

t_text* TextNew(SDL_Point* poTextPos, char* sText);
t_text* TextDel(t_text* pText);
t_text* TextDraw(t_text* pText);
t_text* TextDestroyTexture(t_text* pText);