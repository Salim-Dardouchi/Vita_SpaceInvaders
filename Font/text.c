#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <container.h>
#include <time.h>

#include "text.h"

static struct{
    SDL_Renderer* pRenderer;
    TTF_Font* pFont;
    SDL_Color cColor;
}graph;


int TextInitGraph(TTF_Font* pFont, SDL_Color cColor, SDL_Renderer* pRenderer){
    graph.pFont = pFont;
    graph.cColor = cColor;
    graph.pRenderer = pRenderer;
    return 0;
}

int TextReleaseGraph(void){
    return 0;
}

//--Object specific implementation--//
struct s_text{
    SDL_Texture*    pTextTex;
    SDL_Rect        pTextBox;
};

t_text* TextNew(SDL_Point* poTextPos, char* sText){
    srand((unsigned int)time(NULL));
    t_text* pNewText = (t_text*)malloc(sizeof(t_text));
    pNewText->pTextBox.x = poTextPos->x;
    pNewText->pTextBox.y = poTextPos->y;

    SDL_Surface* pTextSur = TTF_RenderText_Blended(graph.pFont, sText, graph.cColor);
    pNewText->pTextTex = SDL_CreateTextureFromSurface(graph.pRenderer, pTextSur);

    SDL_QueryTexture(pNewText->pTextTex, NULL, NULL, &pNewText->pTextBox.w, &pNewText->pTextBox.h);
    SDL_FreeSurface(pTextSur);

    return pNewText;
}

t_text* TextDel(t_text* pText){
    free(pText);
    return NULL;
}

t_text* TextDraw(t_text* pText){    
    SDL_RenderCopy(
        graph.pRenderer, 
        pText->pTextTex, 
        NULL, 
        &pText->pTextBox
        );

}

t_text* TextDestroyTexture(t_text* pText){
    SDL_DestroyTexture(pText->pTextTex);
}