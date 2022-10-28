/*
 * container.h
 *
 *  Created on: 5 avr. 2021
 *      Author: Thierry
 */

typedef void*(*t_ptfV)(void*);
typedef void*(*t_ptfVV)(void*,void*);
typedef void*(*t_ptf)();
typedef void*(*t_ptfVVF)(void*, void*, t_ptf);

typedef struct s_container t_container;

t_container*ContainerNew(t_ptfV pDeleteFunc);
t_container*ContainerDel(t_container*pContainer);
unsigned int ContainerCard(t_container*pContainer);
void*ContainerPushback(t_container*pContainer,void*pElem);
void*ContainerGetback(t_container*pContainer);
void*ContainerParse(t_container*pContainer,t_ptfVV pParseFunc,void*pParam);
void*ContainerPopback(t_container*pContainer);
void*ContainerGetfront(t_container*pContainer);
void* ContainerPopfront(t_container*pContainer);
void*ContainerParseDelIf(t_container*pContainer, t_ptfVV pParseFunc, void*pParam);
void*ContainerIntersectDelIf(t_container*pContainer, t_container* pOthers, t_ptfVVF pIntersectFunc, t_ptf pCallbackFunc);
