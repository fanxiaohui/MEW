/**
 * Copyright (C), 1993-2016, Hytera Comm. Co., Ltd.
 * @file    buff.h
 * @brief   ���ζ��л�����
 * @author  ������
 * @version v1.0
 * @date    2016-8-23
 */
/*
 *   �����б�
 *
 *   01. BUFF_init								��ʼ��
 *   02. BUFF_count								��ȡ
 *   03. BUFF_push								����
 *   04. BUFF_pop									����
 *   05. BUFF_get									ȡĳһλ������
 *   06. BUFF_remove							ɾ��
 *   07. BUFF_find								����
 *   History:
 *   2016-08-23		������		����
 *   *************************************************************************/
#ifndef __BUFF_H
#define __BUFF_H

#include "stdint.h"

#define BUFF_SIZE 128

typedef struct buffNode
{
	uint8_t Data;
	struct buffNode* Next;
}BuffNode;



void BUFF_Init( void );
uint16_t BUFF_GetCount( void );
void BUFF_Push( uint8_t data);

uint8_t BUFF_Pop( void );

uint8_t BUFF_GetData( uint16_t index );

void BUFF_Remove( uint16_t count );

//INT16 BUFF_find(UINT16 start, UINT8* dest, UINT16 dest_len);

#endif

