/*
 * container.c
 *
 *  Created on: 5 avr. 20212
 *      Author: Thierry
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "container.h"

/*------------------------Node Component Section----------------------*/
typedef struct tt_node{
	struct tt_node*pPrev;
	struct tt_node*pNext;
	void*pElem;
}t_node;

t_node*NodeNew(t_node*pPrev,t_node*pNext,void*pElem){
	t_node* pNewNode = (t_node*)malloc(sizeof(t_node));

	pNewNode->pPrev = pPrev;
	pNewNode->pNext = pNext;
	pNewNode->pElem = pElem;

	if (pNext) pNewNode->pNext->pPrev = pNewNode;
	if (pPrev) pNewNode->pPrev->pNext = pNewNode;

	return pNewNode;
}

t_node*NodeDel(t_node*pNode, t_ptfV pDeleteFunc){
	t_node* pNodeReturn = pNode->pNext;
	if(pNode->pNext && pNode->pNext->pPrev)pNode->pNext->pPrev = pNode->pPrev;
	if(pNode->pPrev && pNode->pPrev->pNext)pNode->pPrev->pNext = pNode->pNext;
	if(pDeleteFunc) pDeleteFunc(pNode->pElem);
	else free(pNode->pElem);

	free(pNode);
	return pNodeReturn;
	/*We're always returning the next node when we're destroying one, that means that
	 the container deletion process is simplified. */
}

/*-------------------Container Component Section-------------------*/
struct s_container{
	t_node*pHead;
	t_node*pTail;
	t_ptfV pDeleteFunc;
	unsigned int uCard;
};

t_container*ContainerNew(t_ptfV pDeleteFunc){
	t_container* pNewCont=(t_container*)malloc(sizeof(t_container));

	pNewCont->pHead=pNewCont->pTail=NULL;
	pNewCont->uCard=0;
	pNewCont->pDeleteFunc=pDeleteFunc;

	return pNewCont;
}

t_container*ContainerDel(t_container*pContainer){
	while(pContainer->pHead){
		pContainer->pHead=NodeDel(pContainer->pHead, pContainer->pDeleteFunc);
		pContainer->uCard--;
	}
	assert(pContainer->uCard==0);

	free(pContainer);
	return NULL;
}


unsigned int ContainerCard(t_container*pContainer){return pContainer->uCard;}


void*ContainerPushback(t_container*pContainer,void*pElem){
	if(pContainer->uCard==0){ //We assume here that our container is empty
		assert(pContainer->pHead==NULL);
		assert(pContainer->pTail==NULL);
		pContainer->pHead=pContainer->pTail=NodeNew(NULL,NULL,pElem);

		/* The element that we're putting at the end of our list will be the only one
		 * because we didn't have any until now, so it'll be the head element and the
		 * tail element at the same time.
		 */
	}
	else{
		//We assume here that our container has stuff inside

		pContainer->pTail=NodeNew(pContainer->pTail, NULL, pElem);

		/* Here we're assuming that we already have a tail element, so
		 * we're creating a new tail element that has the current tail
		 * element as its previous element (kinda hard to understand ngl)
		 * This will mean that the current tail element is not an actual tail
		 * element anymore, as it is now the element preceding our new tail element
		 */

	}
	assert(pContainer->pTail->pElem==pElem);
	pContainer->uCard++;
	return pContainer->pTail->pElem;
}


void*ContainerGetback(t_container*pContainer){return pContainer->pTail->pElem;}

void*ContainerGetfront(t_container*pContainer){return pContainer->pHead->pElem;}

void*ContainerPopback(t_container*pContainer){
	if(pContainer->pTail){
		void* pElem = ContainerGetback(pContainer);
		pContainer->pTail=NodeDel(pContainer->pTail, pContainer->pDeleteFunc);
		pContainer->uCard--;
		return pElem;
	}
}

void* ContainerPopfront(t_container*pContainer){
	if(pContainer->pHead){
		void* pElem = ContainerGetfront(pContainer);
		pContainer->pHead=NodeDel(pContainer->pHead, pContainer->pDeleteFunc);
		pContainer->uCard--;
		return pElem;
	}
}

void* ContainerParse(t_container* pContainer, t_ptfVV pParseFunc, void*pParam){
	t_node* pScan=pContainer->pHead;

	while(pScan){
		if(pParseFunc(pScan->pElem, pParam)) return pScan->pElem;
		pScan=pScan->pNext;
	}
	return NULL;
}

void*ContainerParseDelIf(t_container*pContainer, t_ptfVV pParseFunc, void*pParam){
	t_node* pScan=pContainer->pHead;

	while(pScan){
		if(pParseFunc(pScan->pElem, pParam)){
			if(pScan==pContainer->pHead)pContainer->pHead=pScan->pNext;
			if(pScan==pContainer->pTail)pContainer->pTail=pScan->pPrev;
			pScan=NodeDel(pScan, pContainer->pDeleteFunc);
			pContainer->uCard--;

		}
		else pScan=pScan->pNext;
	}

	return NULL;
}

void*ContainerIntersectDelIf(t_container*pContainer, t_container* pOthers, t_ptfVVF pIntersectFunc, t_ptf pCallbackFunc){
	t_node* pScan=pContainer->pHead;
	t_node* pOther=pOthers->pHead;
	int iIsIntersecting = 0;
	while(pScan){
		while(pOther){
			if(pIntersectFunc(pScan->pElem, pOther->pElem, pCallbackFunc)){
				if(pOther==pOthers->pHead)pOthers->pHead=pOther->pNext;
				if(pOther==pOthers->pTail)pOthers->pTail=pOther->pPrev;
				pOther=NodeDel(pOther, pOthers->pDeleteFunc);
				pOthers->uCard--;

				iIsIntersecting=1;
				break;
			}
			else pOther=pOther->pNext;
		}
		if(iIsIntersecting){
			if(pScan==pContainer->pHead)pContainer->pHead=pScan->pNext;
			if(pScan==pContainer->pTail)pContainer->pTail=pScan->pPrev;
			pScan=NodeDel(pScan, pContainer->pDeleteFunc);
			pContainer->uCard--;
		}
		else pScan=pScan->pNext;
	}
	return NULL;
}
