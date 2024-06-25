// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <new.h>
#include <stdlib.h>
#include <stack>
#include "HAL/PlatformTLS.h"
#include "HAL/PlatformAtomics.h"
struct TlsLog
{
	int threadId;
	int size;
	int mallocCount;
};

struct ObjectPoolUsageMonitor {
	int size;
	int mallocCount;
};

//extern std::map<int, ObjectPoolUsageMonitor> _objectPoolMonitor;
extern int releaseStackSize;
#define MAX_TLS_LOG_INDEX 110
#define DEFAULT_MAX_POOL_SIZE 500

#define DEFAULT_RELEASE_NUM 100
extern TlsLog _objectPoolMonitor[MAX_TLS_LOG_INDEX];




template <class DATA, bool bPlacementNew>
class  TlsObjectPool;


template <class DATA>
class  TlsObjectPool<DATA, true> // bPlacement�� flase�϶��� Ư��ȭ
{
	struct st_BLOCK_NODE
	{
		DATA data;
		st_BLOCK_NODE* next = nullptr;
	};

public:
	TlsObjectPool()
	{
		//printf("TlsObjectPool()\n");
		_maxPoolSize = DEFAULT_MAX_POOL_SIZE;
		_releaseNum = DEFAULT_RELEASE_NUM;
		_PoolSizeToRelease = _maxPoolSize + _releaseNum;
	}

	TlsObjectPool(int blockNum, int releaseNum)
	{
		_maxPoolSize = blockNum;
		_initialBlockNum = blockNum;

		//TODO: iBlockNum��ŭ Alloc
		if (blockNum == 0)
		{
			_maxPoolSize = DEFAULT_MAX_POOL_SIZE;
			// TODO: NOTHING
		}

		if (releaseNum == 0)
		{
			if (_maxPoolSize < 500)
			{
				//500������ ������ �׳� 100��
				_releaseNum = DEFAULT_RELEASE_NUM;
			}
			else {
				//�ƴϸ� 20%
				_releaseNum = _maxPoolSize / 5;
			}
		}
		else {
			_releaseNum = releaseNum;
		}

		_PoolSizeToRelease = _maxPoolSize + _releaseNum;
	}

	int GetSize()
	{
		return _size;
	}

	void Init()
	{
		if (_initialBlockNum == 0)
		{
			//printf("init %d", GetCurrentThreadId());
			_size = 0;
		}
		else {
			//printf("init %d", GetCurrentThreadId());
			_freeNode = new st_BLOCK_NODE;
			_freeNode->next = nullptr;
			int i = 0;
			for (; i < _initialBlockNum - 1; i++)
			{
				st_BLOCK_NODE* node = new st_BLOCK_NODE;
				node->next = _freeNode;
				_freeNode = node;
			}
			//_toReleaseNode = _freeNode;s
			_size = _initialBlockNum;
			_mallocNumPerPool = _initialBlockNum;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// �� �ϳ��� �Ҵ�޴´�.
	//
	// Parameters: ����.
	// Return: (DATA *) ����Ÿ �� ������.
	//////////////////////////////////////////////////////////////////////////
	DATA* Alloc(void)
	{
		if (_size == -1)
		{
			Init();
		}
		int currentThreadId = FPlatformTLS::GetCurrentThreadId();
		_objectPoolMonitor[currentThreadId] = { _size, _mallocNumPerPool };
		/*st_BLOCK_NODE* freeNode = _freeNode;*/
		if (_freeNode != nullptr)
		{
			//�ִ°� �����٢Z����
			DATA* ret = new(&_freeNode->data) DATA;
			_freeNode = _freeNode->next;
			_size--;
			return ret;
		}
		else
		{

			int releasedDataStackSize = -1;
			st_BLOCK_NODE* node = nullptr;
			_releasedDataLock.Lock();
			releasedDataStackSize = _releasedData.size();
			if (!_releasedData.empty())
			{
				// ������� ������
				node = _releasedData.top();
				_releasedData.pop();
				_size += _releaseNum;
			}
			_releasedDataLock.Unlock();
			if (node == nullptr)
			{
				//printf("malloc, stack Size : %d\n", releasedDataStackSize);
				// stack���� ������
				node = new st_BLOCK_NODE;
				_mallocNumPerPool++;
				if (_freeNode != nullptr)
				{
					__debugbreak();
				}
				node->next = _freeNode; // _freeNode == nullptr
				return &(node->data);
			}
			else {
				//stack���� _releaseNum��ŭ ��������
				_freeNode = node;
				DATA* ret = &_freeNode->data;
				_freeNode = _freeNode->next;
				_size--;
				return ret;
			}
			// ���� ���������� _capacity++;
		}
	}

	//void Free(DATA* pData)
	//{
	//	if (!_bInit)
	//	{
	//		//Init
	//		_bInit = true;
	//		Init();
	//	}

	//	//_size++;
	//	int size = ++_size;
	//	if (size <= _maxPoolSize)
	//	{
	//		((st_BLOCK_NODE*)pData)->next = _freeNode;

	//		_freeNode = (st_BLOCK_NODE*)pData;
	//		return;
	//	}
	//}


	void	Free(DATA* pData)
	{
		int size = ++_size;
		int currentThreadId = FPlatformTLS::GetCurrentThreadId();
		pData->~DATA();
		_objectPoolMonitor[currentThreadId] = { _size, _mallocNumPerPool };
		if (size == 0)
		{
			Init();
		}
		//_size++;
		if (size <= _maxPoolSize)
		{
			((st_BLOCK_NODE*)pData)->next = _freeNode;
			_freeNode = (st_BLOCK_NODE*)pData;
			return;
		}
		else if (size == _maxPoolSize + 1)
		{
			((st_BLOCK_NODE*)pData)->next = _freeNode;

			/*if (_freeNode == nullptr)
			{
				__debugbreak();
			}*/
			//�̰� ���������Ұ� next nullptr�� ����ƾ��Ұ�
			_toDisconnectNode = (st_BLOCK_NODE*)pData;
			//�̰� stack�� ������
			_freeNodeAfterRelease = _freeNode;
			_freeNode = (st_BLOCK_NODE*)pData;

			return;
		}
		else {
			((st_BLOCK_NODE*)pData)->next = _freeNode;

			_freeNode = (st_BLOCK_NODE*)pData;
			if (size == _PoolSizeToRelease)
			{
				/*if (_freeNode == nullptr)
				{
					__debugbreak();
				}*/

				//���� ����
				_toDisconnectNode->next = nullptr;
				_releasedDataLock.Lock();
				releaseStackSize = _releasedData.size();
				_releasedData.push((st_BLOCK_NODE*)pData);
				_releasedDataLock.UnLock();


				_freeNode = _freeNodeAfterRelease;
				_size -= _releaseNum;
				//_toReleaseNode = _freeNode;
				return;
			}
		}

		// ������ΰ�
		//return true;
	}


public:
	//�� thread���� �����־���ϴ°�
	inline static thread_local st_BLOCK_NODE* _freeNode = nullptr; // ���ʿ� freeNode�� ����? ó������ �� �� �ִ� ��

	inline static thread_local int _size = -1; // ���� Ǯ�� �����ִ� ����

	//int _capacity = 0; // ���� Ȯ�� �� �� ���� ( �޸�Ǯ ������ ��ü ���� )
	inline static thread_local st_BLOCK_NODE* _freeNodeAfterRelease = nullptr;
	//st_BLOCK_NODE* _toReleaseNode = nullptr;
	inline static thread_local st_BLOCK_NODE* _toDisconnectNode = nullptr;


public:
	//TLS Ǯ ����
	FCriticalSection _releasedDataLock;
	std::stack<st_BLOCK_NODE*> _releasedData;
	// int _capacity = 0; // ���� Ȯ�� �� �� ���� ( �޸�Ǯ ������ ��ü ���� ) ��Ƽ������ Ǯ���� capacity�� �ʿ����
	// ���� ��Ǯ�� �����ϴ� ������, ��� �ܺο��� ����������� �߿��ѵ�
	// ��� �ܺο��� ����������� �߿������ʳ�
	// �ٸ�Ǯ�� ���� ���ݾ�

	// Ǯ�� ������ �����Ϸ��� ���� �߿��ұ�?
	// ��Ǯ�� �����ִ� �������
	// �ǳ��� �Ѱ����� �߿����� ������
	// �ǳ��� �Ѱ����� �ʿ����


	// ���
	// �׳� ��Ǯ�� �����ִ� �������
	// ���� ������ ������ ������
	// ���μ������� ��ü ���ŭ�� ��尡 ���ƴٴϴ��� �˼��ְڳ�


	//int _useCount = 0; //  ���� �ܺο��� ������� ������? // �̰� ������// �ʿ��ϸ� _capacity - _size�ϱ�
public:
	//CObjectPool<DATA, false> _tlOp;
	inline static thread_local int _mallocNumPerPool = 0;
	int _maxPoolSize = 0;
	int _PoolSizeToRelease = 0;
	int _releaseNum = 0;
	int _initialBlockNum = 0;
};

template <class DATA>
class  TlsObjectPool<DATA, false> // bPlacement�� flase�϶��� Ư��ȭ
{
	struct st_BLOCK_NODE
	{
		DATA data;
		st_BLOCK_NODE* next = nullptr;
	};

public:
	TlsObjectPool()
	{
		//printf("TlsObjectPool()\n");
		_maxPoolSize = DEFAULT_MAX_POOL_SIZE;
		_releaseNum = DEFAULT_RELEASE_NUM;
		_PoolSizeToRelease = _maxPoolSize + _releaseNum;
	}

	TlsObjectPool(int blockNum, int releaseNum)
	{
		_maxPoolSize = blockNum;
		_initialBlockNum = blockNum;

		//TODO: iBlockNum��ŭ Alloc
		if (blockNum == 0)
		{
			_maxPoolSize = DEFAULT_MAX_POOL_SIZE;
			// TODO: NOTHING
		}

		if (releaseNum == 0)
		{
			if (_maxPoolSize < 500)
			{
				//500������ ������ �׳� 100��
				_releaseNum = DEFAULT_RELEASE_NUM;
			}
			else {
				//�ƴϸ� 20%
				_releaseNum = _maxPoolSize / 5;
			}
		}
		else {
			_releaseNum = releaseNum;
		}

		_PoolSizeToRelease = _maxPoolSize + _releaseNum;
	}

	int GetSize()
	{
		return _size;
	}

	void Init()
	{
		if (_initialBlockNum == 0)
		{
			//printf("init %d", GetCurrentThreadId());
			_size = 0;
		}
		else {
			//printf("init %d", GetCurrentThreadId());
			_freeNode = new st_BLOCK_NODE;
			_freeNode->next = nullptr;
			int i = 0;
			for (; i < _initialBlockNum - 1; i++)
			{
				st_BLOCK_NODE* node = new st_BLOCK_NODE;
				node->next = _freeNode;
				_freeNode = node;
			}
			//_toReleaseNode = _freeNode;s
			_size = _initialBlockNum;
			_mallocNumPerPool = _initialBlockNum;
		}

		_threadId = FPlatformTLS::GetCurrentThreadId();
		_tlsLogIndex = FPlatformAtomics::InterlockedIncrement(&g_tlsLogIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	// �� �ϳ��� �Ҵ�޴´�.
	//
	// Parameters: ����.
	// Return: (DATA *) ����Ÿ �� ������.
	//////////////////////////////////////////////////////////////////////////
	DATA* Alloc(void)
	{
		if (_size == -1)
		{
			Init();
		}
		FPlatformAtomics::InterlockedIncrement(&_totalUseCount);

		//int currentThreadId = GetCurrentThreadId();
		_objectPoolMonitor[_tlsLogIndex] = { _threadId, _size, _mallocNumPerPool };
		/*st_BLOCK_NODE* freeNode = _freeNode;*/
		if (_freeNode != nullptr)
		{
			//�ִ°� �����٢Z����
			DATA* ret = &_freeNode->data;
			_freeNode = _freeNode->next;
			_size--;
			return ret;
		}
		else
		{

			int releasedDataStackSize = -1;
			st_BLOCK_NODE* node = nullptr;
			_releasedDataLock.Lock();
			releasedDataStackSize = _releasedData.size();
			if (!_releasedData.empty())
			{
				// ������� ������
				node = _releasedData.top();
				_releasedData.pop();
				_size += _releaseNum;
			}
			_releasedDataLock.Unlock();

			if (node == nullptr)
			{
				//printf("malloc, stack Size : %d\n", releasedDataStackSize);
				// stack���� ������
				node = new st_BLOCK_NODE;
				_mallocNumPerPool++;
				if (_freeNode != nullptr)
				{
					__debugbreak();
				}
				node->next = _freeNode; // _freeNode == nullptr
				return &(node->data);
			}
			else {
				//stack���� _releaseNum��ŭ ��������
				_freeNode = node;
				DATA* ret = &_freeNode->data;
				_freeNode = _freeNode->next;
				_size--;
				return ret;
			}
			// ���� ���������� _capacity++;
		}
	}

	void	Free(DATA* pData)
	{
		int size = ++_size;
		//int currentThreadId = GetCurrentThreadId();
		if (size == 0)
		{
			Init();
		}
		_objectPoolMonitor[_tlsLogIndex] = { _threadId, _size, _mallocNumPerPool };
		FPlatformAtomics::InterlockedDecrement(&_totalUseCount);
		//_size++;
		if (size <= _maxPoolSize)
		{
			((st_BLOCK_NODE*)pData)->next = _freeNode;
			_freeNode = (st_BLOCK_NODE*)pData;
			return;
		}
		else if (size == _maxPoolSize + 1)
		{
			((st_BLOCK_NODE*)pData)->next = _freeNode;

			/*if (_freeNode == nullptr)
			{
				__debugbreak();
			}*/
			//�̰� ���������Ұ� next nullptr�� ����ƾ��Ұ�
			_toDisconnectNode = (st_BLOCK_NODE*)pData;
			//�̰� stack�� ������
			_freeNodeAfterRelease = _freeNode;
			_freeNode = (st_BLOCK_NODE*)pData;

			return;
		}
		else {
			((st_BLOCK_NODE*)pData)->next = _freeNode;

			_freeNode = (st_BLOCK_NODE*)pData;
			if (size == _PoolSizeToRelease)
			{
				/*if (_freeNode == nullptr)
				{
					__debugbreak();
				}*/

				//���� ����
				_toDisconnectNode->next = nullptr;
				_releasedDataLock.Lock();
				releaseStackSize = _releasedData.size();
				_releasedData.push((st_BLOCK_NODE*)pData);
				_releasedDataLock.Unlock();

				_freeNode = _freeNodeAfterRelease;
				_size -= _releaseNum;
				//_toReleaseNode = _freeNode;
				return;
			}
		}

		// ������ΰ�
		//return true;
	}


public:
	//�� thread���� �����־���ϴ°�
	inline static thread_local st_BLOCK_NODE* _freeNode = nullptr; // ���ʿ� freeNode�� ����? ó������ �� �� �ִ� ��

	inline static thread_local int _size = -1; // ���� Ǯ�� �����ִ� ����

	//int _capacity = 0; // ���� Ȯ�� �� �� ���� ( �޸�Ǯ ������ ��ü ���� )
	inline static thread_local st_BLOCK_NODE* _freeNodeAfterRelease = nullptr;
	//st_BLOCK_NODE* _toReleaseNode = nullptr;
	inline static thread_local st_BLOCK_NODE* _toDisconnectNode = nullptr;


public:
	//TLS Ǯ ����
	FCriticalSection _releasedDataLock;
	//SRWLOCK _releasedDataLock;
	std::stack<st_BLOCK_NODE*> _releasedData;
	// int _capacity = 0; // ���� Ȯ�� �� �� ���� ( �޸�Ǯ ������ ��ü ���� ) ��Ƽ������ Ǯ���� capacity�� �ʿ����
	// ���� ��Ǯ�� �����ϴ� ������, ��� �ܺο��� ����������� �߿��ѵ�
	// ��� �ܺο��� ����������� �߿������ʳ�
	// �ٸ�Ǯ�� ���� ���ݾ�

	// Ǯ�� ������ �����Ϸ��� ���� �߿��ұ�?
	// ��Ǯ�� �����ִ� �������
	// �ǳ��� �Ѱ����� �߿����� ������
	// �ǳ��� �Ѱ����� �ʿ����


	// ���
	// �׳� ��Ǯ�� �����ִ� �������
	// ���� ������ ������ ������
	// ���μ������� ��ü ���ŭ�� ��尡 ���ƴٴϴ��� �˼��ְڳ�


	//int _useCount = 0; //  ���� �ܺο��� ������� ������? // �̰� ������// �ʿ��ϸ� _capacity - _size�ϱ�
public:
	//CObjectPool<DATA, false> _tlOp;
	inline static thread_local int _mallocNumPerPool = 0;
	int _maxPoolSize = 0;
	int _PoolSizeToRelease = 0;
	int _releaseNum = 0;
	int _initialBlockNum = 0;

	inline static int64 _totalUseCount = 0;

	//�α׿�
	inline static thread_local int _threadId = 0;

	inline static thread_local int _tlsLogIndex = 0;
	int g_tlsLogIndex = -1;
};