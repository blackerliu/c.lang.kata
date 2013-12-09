/*
 * general link list
 * (C) 2012, LiuJianhua
 */

#ifndef __DLIST_HEAD__
#define __DLIST_HEAD__

#ifdef __cplusplus
extern "C" {
#endif 

typedef DListRet int;

struct _DListNode;
typedef struct _DListNode DListNode;

struct _DListNode
{
	struct _DListNode* prev;
	struct _DListNode* next;
	void* data;
}

typedef DListRet (*DListDataPrintFunc)(void* data);


DList*  dlist_create(void);
DListRet dlist_destory(DList* thiz);
DListRet dlist_append(DList* thiz, void* data);
DListRet dlist_foreach(DList* thiz, );

#ifdef __cplusplus
}
#endif
