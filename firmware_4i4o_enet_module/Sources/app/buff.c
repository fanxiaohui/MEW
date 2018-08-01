/**
 * Copyright (C), 1993-2016, Hytera Comm. Co., Ltd.
 * @file    buff.c
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


#include "buff.h"

static BuffNode buff[BUFF_SIZE];

static BuffNode * pPush;

static BuffNode * pPop;

static uint16_t buff_count=0;
//static UINT8 isPushing=0,isPoping=0,isRemoving=0;


//int8_t BUFF_check( uint16_t * oi , uint16_t* oj )
//{
//	int16_t i,j;
//	for( i = 0 ; i < BUFF_SIZE ; i ++ )
//	{
//		for( j = 0 ; j < BUFF_SIZE ; j ++ )
//		{
//			if( i != j )
//			{
//				if( BUFF[ i ].Next == BUFF[ j ].Next )
//				{
//					* oi = i;
//					* oj = j;
//					return 0;
//				}
//			}
//		}
//	}
//	return -1;
//}
/**
 * @brief				BUFF��ʼ��������
 *
 * @param[in]		��
 *
 * @return			��
 * @author			������
 * @bug  
 */
void BUFF_Init( void )
{
	uint16_t i=0;
	for( i = 0 ; i < BUFF_SIZE - 1 ; i++ )
	{
		buff[ i ].Data = 0;
		buff[ i ].Next = & ( buff [ i + 1 ] );
	}
	buff[ i ].Data = 0;
	buff[ i ].Next = & ( buff[ 0 ] );
	
	pPush = & ( buff[ 0 ] );
	pPop = & ( buff[ 0 ] );
	
	buff_count = 0;
	
//	isPushing=0;
//	isPoping=0;
//	isRemoving=0;
}
/**
 * @brief				��ȡBUFF���ݸ���
 *
 * @param[in]		��
 *
 * @return			��
 * @author			������
 * @bug  
 */
uint16_t BUFF_GetCount( void )
{
	return buff_count;
}
/**
 * @brief				�����
 *
 * @param[in]		data�������
 *
 * @return			��
 * @author			������
 * @bug  
 */
void BUFF_Push( uint8_t data )
{
	
	if( buff_count < BUFF_SIZE )
	{
//		while(isRemoving);
//		isPushing=1;
		//pPushPre=pPush;
		pPush -> Data = data;
		pPush = pPush -> Next;
		buff_count ++;
//		isPushing=0;

	}
}
/**
 * @brief				������
 *
 * @param[out]		����������
 *
 * @return			��
 * @author			������
 * @bug  
 */
uint8_t BUFF_Pop( void )
{
	uint8_t data = pPop -> Data;
	if( buff_count > 0 )
	{
//		while(isRemoving);
//		isPoping=1;
		pPop = pPop -> Next;
		buff_count --;
//		isPoping=0;
	}
	else
	{
		//todo error
	}
	return data;
}
/**
 * @brief				��ȡĳһλ������
 *
 * @param[in]		index ����ڿ�ʼλ��
 * @param[out]	�õ�������
 *
 * @return			��
 * @author			������
 * @bug  
 */
uint8_t BUFF_GetData( uint16_t index )
{
	uint16_t i=0;
	uint8_t data;
	BuffNode * pTmp = pPop;
	for( i = 0 ; i < index ; i ++ )
	{
		pTmp = pTmp -> Next;
	}
	data = pTmp -> Data;
	return data;
}
/**
 * @brief				ɾ������
 *
 * @param[in]		��ͷ��ʼ��ɾ������
 * @param[out]	��
 *
 * @return			��
 * @author			������
 * @bug  
 */
void BUFF_Remove( uint16_t count )
{
	int16_t i=0;

	
//	while(isPushing||isPoping);
//	isRemoving=1;
		for( i = 0 ; i < count ; i ++ )
		{
			pPop = pPop -> Next;
		}
	
	if( buff_count >= count )
	{
		buff_count -= count;
	}
	else
	{
		//todo error
	}
//	isRemoving=0;		
}

