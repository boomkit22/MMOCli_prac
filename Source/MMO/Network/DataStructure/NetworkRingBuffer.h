// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <stdlib.h>
#include <memory.h>
#include <intrin.h>
#define DEFAULT_RINGBUFFER_SIZE 10000



// useSize = r - f
//
// freeSize = bufferSize - useSize
class  NetworkRingBuffer
{
public:
	NetworkRingBuffer()
	{
		_realBufferSize = _bufferSize + 1;
		_buffer = (char*)FMemory::Malloc(_realBufferSize);
		//_buffer = (char*)malloc(sizeof(char) * _realBufferSize);
		_front = 0;
		_rear = 0;
	}

	NetworkRingBuffer(int bufferSize)
	{
		_realBufferSize = bufferSize + 1;
		_buffer = (char*)FMemory::Malloc(_realBufferSize);
		_bufferSize = bufferSize;
		_front = 0;
		_rear = 0;
	}

	~NetworkRingBuffer()
	{
		FMemory::Free(_buffer);
		//delete _buffer;
	}

	void Resize(int size)
	{
	}

	int	GetBufferSize(void)
	{
		return _bufferSize;
	}
	/////////////////////////////////////////////////////////////////////////
	// ���� ������� �뷮 ���.
	//
	// Parameters: ����.
	// Return: (int)������� �뷮.
	/////////////////////////////////////////////////////////////////////////
	int	GetUseSize()
	{
		int useSize = _rear - _front;
		if (useSize >= 0)
		{
			return useSize;
		}
		else {
			return _realBufferSize + useSize;
		}
	}

	int	GetFreeSize()
	{
		int useSize = GetUseSize();
		return _bufferSize - useSize;
	}

	//data : 10
	//  0   1   2   3   4   5   6   7   8   9
	// [a] [b] [c] [d] [e] [f] [ ] [ ] [ ] [ ]    ��Ȳ 1
	// [ ] [ ] [ ] [a] [b] [c] [d] [e] [f] [ ]    ��Ȳ 2
	// [d] [e] [f] [ ] [ ] [ ] [ ] [a] [b] [c]    ��Ȳ 3
	// [d] [e] [f] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
	//
	// [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
	// rf
	// [h] [i] [j] [a] [b] [c] [d] [e] [f] [g]
	//              rf
	int	Enqueue(char* chpData, int dataSize)
	{
		int f = _front;
		int freeSize = GetFreeSize(f, _rear);
		if (freeSize < dataSize)
		{
			return 0;
		}
		else {
			if (_rear >= f)
			{
				// rear�� ���ΰ��
				int backBufferSize = _realBufferSize - _rear;
				if (backBufferSize >= dataSize)
				{
					//��Ȳ 1
					// �ڿ� �� �־����
					FMemory::Memcpy(&_buffer[_rear], chpData, dataSize);
					//memcpy(&_buffer[_rear], chpData, dataSize);
					_rear = (_rear + dataSize) % _realBufferSize;
				}
				else {
					//��Ȳ 2: �����־����
					FMemory::Memcpy(&_buffer[_rear], chpData, backBufferSize);
					//memcpy(&_buffer[_rear], chpData, backBufferSize);
					FMemory::Memcpy(&_buffer[0], chpData + backBufferSize, dataSize - backBufferSize);
					FMemory::Memcpy(&_buffer[0], chpData + backBufferSize, dataSize - backBufferSize);
					_rear = (_rear + dataSize) % _realBufferSize;
				}
			}
			else {
				// front�� ���ΰ�� : ��Ȳ 3
				// �׳� rear���� �������
				FMemory::Memcpy(&_buffer[_rear], chpData, dataSize);
				//memcpy(&_buffer[_rear], chpData, dataSize);
				_rear = (_rear + dataSize) % _realBufferSize;
			}
			return dataSize;
		}
	}

	//data : 10
	//  0   1   2   3   4   5   6   7   8   9
	// [a] [b] [c] [d] [e] [f] [ ] [ ] [ ] [ ]    ��Ȳ 1
	// [ ] [ ] [ ] [a] [b] [c] [d] [e] [f] [ ]    ��Ȳ 2
	// [d] [e] [f] [ ] [ ] [ ] [ ] [a] [b] [c]    ��Ȳ 3
	// [f] [ ] [ ] [ ] [ ] [a] [b] [c] [d] [e]

	// [a] [b] [c] [d] [e] [f] [g] [h] [i] [j]
	// front
	// rear

	// [a] [b] [c] [d] [e] [f] [g] [h] [i] [j]
	//                 fr
	int	Dequeue(char* chpDest, int dataSize)
	{
		int r = _rear;
		int useSize = GetUseSize(_front, r);

		if (useSize < dataSize)
		{
			return 0;
		}
		else {
			if (_front >= r)
			{
				// front�� ���ΰ��
				int backBufferSize = _realBufferSize - _front;
				if (backBufferSize >= dataSize)
				{
					// �׳� ���� ��
					FMemory::Memcpy(chpDest, &_buffer[_front], dataSize);
					//memcpy(chpDest, &_buffer[_front], dataSize);
					_front = (_front + dataSize) % _realBufferSize;
				}
				else {
					//������ �����
					FMemory::Memcpy(chpDest, &_buffer[_front], backBufferSize);
					//memcpy(chpDest, &_buffer[_front], backBufferSize);
					FMemory::Memcpy(chpDest + backBufferSize, &_buffer[0], dataSize - backBufferSize);
					//memcpy(chpDest + backBufferSize, &_buffer[0], dataSize - backBufferSize);
					_front = (_front + dataSize) % _realBufferSize;
				}
			}
			else {
				// rear�� ���ΰ��
				// �׳� ���� ��
				FMemory::Memcpy(chpDest, &_buffer[_front], dataSize);
				//memcpy(chpDest, &_buffer[_front], dataSize);
				_front = (_front + dataSize) % _realBufferSize;
			}

			return dataSize;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// ReadPos ���� ����Ÿ �о��. ReadPos ����.
	//
	// Parameters: (char *)����Ÿ ������. (int)ũ��.
	// Return: (int)������ ũ��.
	/////////////////////////////////////////////////////////////////////////

	//  0   1   2   3   4   5   6   7   8   9  10
	// [a] [b] [c] [d] [e] [f] [g] [h] [ ] [l] [k]
	//                              r       f
	// ����Ǵ°Թ���
	//
	// backBufferSize �� _realBufferSize - _rear��

	// Dequeue�� Peek�� ��� �����������ϴ°�찡 �ϴ� _front�� ���� ����ε�

	/// <summary>
	/// ��û�� dataSize��ŭ �����Ͱ� ��� useSize��ŭ �����͸� �ش�
	/// </summary>
	/// <param name="chpDest"> ������ ���� ����</param>
	/// <param name="dataSize"> ��û ������ ũ��</param>
	/// <returns> peek�� ������ ũ�� </returns>
	int	Peek(char* chpDest, int dataSize)
	{
		int r = _rear;
		int useSize = GetUseSize(_front, r);
		if (useSize < dataSize)
		{
			return 0;
		}
		else {
			if (_front >= r)
			{
				// front�� ���ΰ��
				int backBufferSize = _realBufferSize - _front;
				if (backBufferSize >= dataSize)
				{
					// �׳� ���� ��
					FMemory::Memcpy(chpDest, &_buffer[_front], dataSize);
					//memcpy(chpDest, &_buffer[_front], dataSize);
				}
				else {
					//������ �����
					FMemory::Memcpy(chpDest, &_buffer[_front], backBufferSize);
					//memcpy(chpDest, &_buffer[_front], backBufferSize);
					FMemory::Memcpy(chpDest + backBufferSize, &_buffer[0], dataSize - backBufferSize);
					//memcpy(chpDest + backBufferSize, &_buffer[0], dataSize - backBufferSize);
				}
			}
			else {
				// rear�� ���ΰ��
				// �׳� ���� ��
				FMemory::Memcpy(chpDest, &_buffer[_front], dataSize);
				//memcpy(chpDest, &_buffer[_front], dataSize);
			}
			return dataSize;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// ������ ��� ����Ÿ ����.
	//
	// Parameters: ����.
	// Return: ����.
	/////////////////////////////////////////////////////////////////////////
	void ClearBuffer()
	{
		_rear = 0;
		_front = 0;
	}

	/////////////////////////////////////////////////////////////////////////
	// ���� �����ͷ� �ܺο��� �ѹ濡 �а�, �� �� �ִ� ����.
	// (������ ���� ����)
	//
	// ���� ť�� ������ ������ ���ܿ� �ִ� �����ʹ� �� -> ó������ ���ư���
	// 2���� �����͸� ��ų� ���� �� ����. �� �κп��� �������� ���� ���̸� �ǹ�
	//
	// Parameters: ����.
	// Return: (int)��밡�� �뷮.
	////////////////////////////////////////////////////////////////////////

	//  0   1   2   3   4   5   6   7   8   9  10
	// [ ] [b] [c] [d] [e] [f] [g] [h] [i] [ ] [ ]
	//      f                               r
	// [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
	//      r           f
	// [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
	//      rf
	// [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]

	int	GetDirectEnqueueSize(void)
	{
		// DirectEnqueueSize
	/*	int r = _rear;
		int f = _front;*/
		int f = _front;

		//full�ΰ��
		if ((_rear + 1) % _realBufferSize == f)
		{
			return 0;
		}

		if (_rear >= f)
		{
			// f = 0�̸� realBufferSize��ŭ ���״ϱ�
			// �̴� ���ϴ� ��Ȳ�� �ƴϴ� bufferSize��ŭ �ش�
			if (f == 0)
			{
				return _realBufferSize - _rear - 1;
			}
			else {
				return _realBufferSize - _rear;
			}

			//int freeSize = GetFreeSize(f, _rear);
			//return _bufferSize -(r - f);
			//return freeSize;
			//return __min(_bufferSize, _realBufferSize - _rear);
		}
		else {
			// r < f �� ����
			// use Size�� rear - front�ϱ�
			//int freeSize = GetFreeSize(f, r);
			int freeSize = GetFreeSize(f, _rear);
			//return _bufferSize -(r - f);
			return freeSize;
		}
	}

	//  0   1   2   3   4   5   6   7   8   9  10
	// [ ] [b] [c] [d] [e] [f] [g] [h] [i] [ ] [ ]
	//      f                               r
	// [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
	//      r           f
	// [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
	//      rf
	// [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]
	int	GetDirectDequeueSize(void)
	{
		//int f = _front;
		int r = _rear;
		if (_front > r)
		{
			return _realBufferSize - _front;
		}
		else {
			//��ų� _rear�� ���ΰ��
			int useSize = GetUseSize(_front, r);
			return useSize;
			//return r - f;
			/*int useSize = GetUseSize();
			return useSize;*/
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// ���ϴ� ���̸�ŭ �б���ġ ���� ���� / ���� ��ġ �̵�
	//
	// Parameters: ����.
	// Return: (int)�̵�ũ��
	/////////////////////////////////////////////////////////////////////////
	int	MoveRear(int size)
	{
		// MoveRear�� �������ǳ�
		// �ϴ� �˷��ֽ��� ����

		// TODO:
		// rear��ġ �����ŭ
		_rear = (_rear + size) % _realBufferSize;
		return size;
	}

	int	MoveFront(int size)
	{
		//����:
		// send �Լ� ȣ��� �ѹ��� �ٷ� ������ ���� ����
		// dequeue �Լ� ȣ���ϰ� �ѹ��� �ٷ� ��������
		// �ٽ� enqueue�� �ؾ��ϴµ�

		// �̷��� ��������
		// peek���� ���� ������ ��������
		// ������ŭ�� MoveFront�ؼ� ����ó���� ����
		/*
			char peekBuffer[1000];
			int ret = sendQueue.Peek(peekBuffer, 1000);
			sendQueue.MoveFront(ret);
		*/

		// TODO :
		// front��ġ size��ŭ �ø���
		_front = (_front + size) % _realBufferSize;

		return size;
	}

	/////////////////////////////////////////////////////////////////////////
	// ������ Front ������ ����.
	//
	// Parameters: ����.
	// Return: (char *) ���� ������.
	/////////////////////////////////////////////////////////////////////////
	char* GetFrontPtr(void)
	{
		return _buffer + _front;
	}

	/////////////////////////////////////////////////////////////////////////
	// ������ RearPos ������ ����.
	//
	// Parameters: ����.
	// Return: (char *) ���� ������.
	/////////////////////////////////////////////////////////////////////////
	char* GetRearPtr(void)
	{
		return _buffer + _rear;
	}

	// ���� ������ ��� WSARecv, WSASend���� �ΰ�
	char* GetBufferPtr(void)
	{
		return _buffer;
	}

private:
	int GetUseSize(int front, int rear)
	{
		int useSize = rear - front;
		if (useSize >= 0)
		{
			return useSize;
		}
		else {
			return _realBufferSize + useSize;
		}
	}

	int GetFreeSize(int front, int rear)
	{
		int useSize = GetUseSize(front, rear);
		return _bufferSize - useSize;
	}

private:
	char* _buffer;
	int _bufferSize = DEFAULT_RINGBUFFER_SIZE;
	int _realBufferSize;
	int _rear;
	int _front;
};
