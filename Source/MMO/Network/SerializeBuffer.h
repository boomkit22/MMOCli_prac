// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TlsObjectPool.h"
#include "PacketHeader.h"


class MMO_API CPacket
{
	friend class TlsObjectPool<CPacket, false>;

	/*---------------------------------------------------------------
	Packet Enum.

	----------------------------------------------------------------*/
	enum en_PACKET
	{
		BUFFER_DEFAULT_SIZE = 256		// ��Ŷ�� �⺻ ���� ������.
	};

	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////

public:
	CPacket()
	{
		_buffer = (char*)malloc(sizeof(char) * BUFFER_DEFAULT_SIZE);
		if (_buffer == nullptr)
		{
			__debugbreak();
		}
		_bufferSize = BUFFER_DEFAULT_SIZE;
	}

	CPacket(int bufferSize)
	{
		_buffer = (char*)malloc(sizeof(char) * bufferSize);
		if (_buffer == nullptr)
		{
			__debugbreak();
		}
		_bufferSize = bufferSize;
	}

	~CPacket()
	{
		free(_buffer);
	}

public:

	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ û��.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void Clear(void)
	{
		_writePos = 0;
		_readPos = 0;
		_dataSize = 0;
		_bEncoded = false;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)��Ŷ ���� ������ ���.
	//////////////////////////////////////////////////////////////////////////
	int	GetBufferSize(void)
	{
		return _bufferSize;
	}
	//////////////////////////////////////////////////////////////////////////
	// ���� ������� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)������� ����Ÿ ������.
	//////////////////////////////////////////////////////////////////////////
	int	GetDataSize(void)
	{
		return _dataSize;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (char *)���� ������.
	//////////////////////////////////////////////////////////////////////////
	char* GetBufferPtr(void)
	{
		return _buffer;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� Pos �̵�. (�����̵��� �ȵ�)
	// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���.
	//
	// Parameters: (int) �̵� ������.
	// Return: (int) �̵��� ������.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int size)
	{
		//TODO : ����üũ
		_writePos += size;
		_dataSize += size;
		return size;
	}

	int		MoveReadPos(int size)
	{
		// TODO:
		// ����üũ
		_readPos += size;
		_dataSize -= size;
		return size;
	}

	/* ============================================================================= */
	// ������ �����ε�
	/* ============================================================================= */
	CPacket& operator = (CPacket& srcPacket)
	{
		// ���� �����ϰ�
		// read pos, write pos �����ؾ��ϴ°ǰ�
		if (_buffer != nullptr)
		{
			free(_buffer);
		}
		// TODO: �ѹ��� ����
		// �� �� �߸��Ѱ� ��������
		_buffer = (char*)malloc(sizeof(char) * srcPacket._bufferSize);
		if (_buffer == nullptr)
		{
			__debugbreak();
		}

		memcpy(_buffer, srcPacket._buffer, srcPacket._bufferSize);

		_dataSize = srcPacket._dataSize;
		_bufferSize = srcPacket._bufferSize;
		_writePos = srcPacket._writePos;
		_readPos = srcPacket._readPos;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////

	// �ֱ� TODO: writePos ����, ���ۿ� ����
	CPacket& operator << (unsigned char byValue)
	{
		*((unsigned char*)(&_buffer[_writePos])) = byValue;
		//(*_buffer) = byValue;
		_writePos += sizeof(unsigned char);

		_dataSize += sizeof(unsigned char);
		return *this;
	}

	CPacket& operator << (char chValue)
	{
		*((char*)(&_buffer[_writePos])) = chValue;
		_writePos += sizeof(char);

		_dataSize += sizeof(char);
		return *this;
	}

	CPacket& operator << (short shValue)
	{
		*((short*)(&_buffer[_writePos])) = shValue;
		_writePos += sizeof(short);

		_dataSize += sizeof(short);
		return *this;
	}

	CPacket& operator << (unsigned short wValue)
	{
		*((unsigned short*)(&_buffer[_writePos])) = wValue;
		_writePos += sizeof(unsigned short);

		_dataSize += sizeof(unsigned short);
		return *this;
	}

	CPacket& operator << (int iValue)
	{
		*((int*)(&_buffer[_writePos])) = iValue;
		_writePos += sizeof(int);

		_dataSize += sizeof(int);
		return *this;
	}

	CPacket& operator << (long lValue)
	{
		*((long*)(&_buffer[_writePos])) = lValue;
		_writePos += sizeof(long);

		_dataSize += sizeof(long);
		return *this;
	}

	CPacket& operator <<(DWORD dwValue)
	{
		*((DWORD*)(&_buffer[_writePos])) = dwValue;
		_writePos += sizeof(DWORD);

		_dataSize += sizeof(DWORD);
		return *this;
	}

	CPacket& operator << (float fValue)
	{
		*((float*)(&_buffer[_writePos])) = fValue;
		_writePos += sizeof(float);

		_dataSize += sizeof(float);
		return *this;
	}

	CPacket& operator << (__int64 iValue)
	{
		*((__int64*)(&_buffer[_writePos])) = iValue;
		_writePos += sizeof(__int64);

		_dataSize += sizeof(__int64);
		return *this;
	}

	CPacket& operator << (double dValue)
	{
		*((double*)(&_buffer[_writePos])) = dValue;
		_writePos += sizeof(double);

		_dataSize += sizeof(double);
		return *this;
	}






	//////////////////////////////////////////////////////////////////////////
	// ����.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////

	// ���� TODO: readPos ���� src�� ����
	CPacket& operator >> (BYTE& byValue)
	{
		byValue = *(BYTE*)(&_buffer[_readPos]);
		_readPos += sizeof(BYTE);

		_dataSize -= sizeof(BYTE);
		return *this;
	}

	CPacket& operator >> (char& chValue)
	{
		chValue = *(char*)(&_buffer[_readPos]);
		_readPos += sizeof(char);

		_dataSize -= sizeof(char);
		return *this;
	}

	CPacket& operator >> (short& shValue)
	{
		shValue = *(short*)(&_buffer[_readPos]);
		_readPos += sizeof(short);

		_dataSize -= sizeof(shValue);
		return *this;
	}

	CPacket& operator >> (WORD& wValue)
	{
		wValue = *(WORD*)(&_buffer[_readPos]);
		_readPos += sizeof(WORD);

		_dataSize -= sizeof(WORD);
		return *this;
	}

	CPacket& operator >> (int& iValue)
	{
		iValue = *(int*)(&_buffer[_readPos]);
		_readPos += sizeof(int);

		_dataSize -= sizeof(int);
		return *this;
	}

	CPacket& operator >> (DWORD& dwValue)
	{
		dwValue = *(DWORD*)(&_buffer[_readPos]);
		_readPos += sizeof(DWORD);

		_dataSize -= sizeof(DWORD);
		return *this;
	}

	CPacket& operator >> (float& fValue)
	{
		fValue = *(float*)(&_buffer[_readPos]);
		_readPos += sizeof(float);

		_dataSize -= sizeof(float);
		return *this;
	}

	CPacket& operator >> (__int64& iValue)
	{
		iValue = *(__int64*)(&_buffer[_readPos]);
		_readPos += sizeof(__int64);

		_dataSize -= sizeof(__int64);
		return *this;
	}

	CPacket& operator >> (double& dValue)
	{
		dValue = *(double*)(&_buffer[_readPos]);
		_readPos += sizeof(double);

		_dataSize -= sizeof(double);
		return *this;
	}


	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ���.
	//
	// Parameters: (char *)Dest ������. (int)Size.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int	GetData(char* dest, int size)
	{
		//TODO: ��������
		// �� readPos �Űܾ��ϴ°ǰ� �Űܾ��ϳ�
		// header������ ���Ͱ�����
		memcpy(dest, _buffer + _readPos, size);
		_readPos += size;

		_dataSize -= size;
		return size;
	}

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ����.
	//
	// Parameters: (char *)Src ������. (int)SrcSize.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int	PutData(char* src, int size)
	{
		memcpy(_buffer + _writePos, src, size);
		_writePos += size;

		_dataSize += size;
		return size;
	}



public:
	static CPacket* Alloc()
	{
		CPacket* packet = _packetPool.Alloc();
		packet->Clear();
		return packet;
	}



	static void Free(CPacket* packet)
	{
		//wprintf(L"free packet : %llx\n", (void*)packet);
	
			_packetPool.Free(packet);
		/*int64 ret = InterlockedDecrement64(& packet->_refCount);
		if (ret == 0)
		{
			_packetPool.Free(packet);
		}*/

		//if (ret <= 0)
		//{
		//	if (ret == -1)
		//	{
		//		__debugbreak();
		//	}
		//	if (packet->_bufferSize != BUFFER_DEFAULT_SIZE)
		//	{
		//		packet->Resize(BUFFER_DEFAULT_SIZE);
		//	}

		//	_packetPool.Free(packet);
		//	//Free(packet)
		//}
	}

	static int64 GetUseCount()
	{
		return _packetPool._totalUseCount;
	}

	/*static void Free(CPacket* packet)
	{
		_packetPool.Free(packet);
	}*/

public:


	void Encode(uint16 packetKey)
	{
		if (_bEncoded)
		{
			return;
		}
		char* buff = _buffer;
		_bEncoded = true;
		NetHeader* header = (NetHeader*)buff;

		// ��¥ ��Ŷ������ �տ� len�ϰ� ��Ŷ�ڵ�
		//xor�ϰ� �ٽ� xor�ϸ� ���������Ͱ��Ǵµ�
		// d1 ^ ( RK + 1 ) = p1 
		// p1 ^ (k + 1  ) = e1; 

		// ���⼭ e1���� ��� d1���� �ٲ���
		// e1 e2
		// d1 d2
		uint16 len = header->_len;
		uint8 randomKey = header->_randKey;
		uint8 checkSum = 0;

		int iEnd = len + sizeof(NetHeader);


		for (int i = sizeof(NetHeader); i < iEnd; i++)
		{
			checkSum += buff[i];
		}
		checkSum %= 256;

		//header->_checkSum = checkSum;
		//e2�� d2�� �ٲٷ��� e1�� �ʿ��ϳ�
		int p = checkSum ^ (randomKey + 1);
		header->_checkSum = p ^ (packetKey + 1);
		int prevData = header->_checkSum;
		//int e;


		for (int i = sizeof(NetHeader); i < iEnd; i++)
		{
			int add = (i - sizeof(NetHeader) + 2);

			char& c = buff[i];
			p = c ^ (p + randomKey + add);
			c = p ^ (prevData + packetKey + add);
			prevData = c;
			//TODO:
			// d1 > e1;
			// d2 > e2;
			// d3 > e3;
			// d4 > e4;
		}

	}

	bool Decode(NetHeader* header, uint16 packetKkey)
	{
		int size = sizeof(NetHeader);
		uint8 p;
		uint8 randKey = header->_randKey;
		uint8 encodedChecksum = header->_checkSum;

		uint8 pPrev = encodedChecksum ^ (packetKkey + 1); // p1
		uint8 prevData = encodedChecksum;


		uint8 decodedCheckSum = pPrev ^ (randKey + 1); // d1

		//todo : validate checksum
		uint8 checkSum = 0;
		for (int i = 0; i < header->_len; i++)
		{
			char& c = _buffer[i];
			p = c ^ (prevData + packetKkey + (i + 2));
			prevData = c;

			c = p ^ (pPrev + randKey + (i + 2));
			checkSum += c;
			pPrev = p;
			//TODO:
			// e1 > d1
			// e2 > d2
			// e3 > d3
			// e4 > d4
		}

		/*for (int i = 0; i < header->_len; i++)
		{
			checkSum += _buffer[i];
		}*/
		checkSum %= 256;

		if (decodedCheckSum != checkSum)
		{

			return false;
		}
		return true;
	}

	char* GetReadPtr()
	{
		return _buffer + _readPos;
	}

public:
	//Monitor ������ ����°ǵ�
	void Resize(int size)
	{
		free(_buffer);
		_buffer = (char*)malloc(sizeof(char) * size);
		_bufferSize = size;
	}





public:
	char* _buffer;

protected:
	//int64 _refCount = 0;
	//
	int	_bufferSize;
	bool _bEncoded = false;
	//------------------------------------------------------------
	// ���� ���ۿ� ������� ������.
	//------------------------------------------------------------
	int	_dataSize = 0;
	//------------------------------------------------------------
	//
	//------------------------------------------------------------
	int _writePos = 0;
	int _readPos = 0;

private:

	static inline TlsObjectPool<CPacket, false> _packetPool{ 5000, 4000 };

};

