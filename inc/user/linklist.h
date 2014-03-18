//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

linklist.h

Abstract:  

Macros for linked-list manipulation.

Notes: 


--*/


#ifndef _LINKLIST_H_
#define _LINKLIST_H_

#ifdef __cplusplus
extern "C" {
#endif


/*NOINC*/

#if !defined(WIN32)

typedef struct _LIST_ENTRY {
    struct _LIST_ENTRY *Flink;
    struct _LIST_ENTRY *Blink;
} LIST_ENTRY;
typedef LIST_ENTRY * PLIST_ENTRY;

#endif  // !WIN32

//
// help macro to simplify Doubly-Linked Lists list traversing.
// Note: the node pointed by ListHead will not be included.
#ifndef FOR_EACH_LIST_ENTRY
#  define FOR_EACH_LIST_ENTRY(curr, next, ListHead)\
   for (curr = (ListHead)->Flink, next = curr->Flink; curr != (ListHead); curr = next, next = curr->Flink)
#endif

//
// copied from DDK WDM.h by jidong
//
__inline void InitializeListHead( IN PLIST_ENTRY ListHead   )
{
   ListHead->Flink = ListHead->Blink = ListHead;
}

__inline bool IsListEmpty( IN const LIST_ENTRY * ListHead )
{
   return ListHead->Flink == ListHead;
}

__inline bool RemoveEntryList( IN PLIST_ENTRY Entry )
{
   PLIST_ENTRY Blink = Entry->Blink;
   PLIST_ENTRY Flink = Entry->Flink;
   Blink->Flink = Flink;
   Flink->Blink = Blink;
   return Flink == Blink;
}

__inline PLIST_ENTRY RemoveHeadList( IN PLIST_ENTRY ListHead )
{
   PLIST_ENTRY Entry = ListHead->Flink;
   PLIST_ENTRY Flink = Entry->Flink;
   ListHead->Flink = Flink;
   Flink->Blink = ListHead;
   return Entry;
}

__inline PLIST_ENTRY RemoveTailList( IN PLIST_ENTRY ListHead )
{
   PLIST_ENTRY Entry = ListHead->Blink;
   PLIST_ENTRY Blink = Entry->Blink;
   ListHead->Blink = Blink;
   Blink->Flink = ListHead;
   return Entry;
}

__inline void InsertTailList( IN PLIST_ENTRY ListHead, IN PLIST_ENTRY Entry  )
{
   PLIST_ENTRY Blink = ListHead->Blink;
   Entry->Flink = ListHead;
   Entry->Blink = Blink;
   Blink->Flink = Entry;
   ListHead->Blink = Entry;
}

__inline void InsertHeadList( IN PLIST_ENTRY ListHead, IN PLIST_ENTRY Entry )
{
   PLIST_ENTRY Flink = ListHead->Flink;
   Entry->Flink = Flink;
   Entry->Blink = ListHead;
   Flink->Blink = Entry;
   ListHead->Flink = Entry;
}

__inline void AppendTailList( IN PLIST_ENTRY ListHead, IN PLIST_ENTRY ListToAppend )
{
   PLIST_ENTRY ListEnd = ListHead->Blink;
   ListHead->Blink->Flink = ListToAppend;
   ListHead->Blink = ListToAppend->Blink;
   ListToAppend->Blink->Flink = ListHead;
   ListToAppend->Blink = ListEnd;
}
/*INC*/

#ifdef __cplusplus
}
#endif


#endif  // _LINKLIST_H_
