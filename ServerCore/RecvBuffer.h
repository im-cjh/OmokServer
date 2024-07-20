#pragma once
#include <vector>  // vector�� ����ϱ� ���� ��� ����
#include <cstdint> // BYTE�� �����ϱ� ���� ��� ����
using std::vector;
using BYTE = uint8_t; // BYTE�� uint8_t�� ����
class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(INT32 bufferSize);
	~RecvBuffer();

	void	Clean();
	bool	OnRead(INT32 numOfBytes);
	bool	OnWrite(INT32 numOfBytes);

	BYTE* ReadPos() { return &_buffer[_readPos]; }
	BYTE* WritePos() { return &_buffer[_writePos]; }
	INT32	DataSize() { return _writePos - _readPos; }
	INT32	FreeSize() { return _capacity - _writePos; }

private:
	INT32	_capacity = 0;
	INT32	_bufferSize = 0;
	INT32	_readPos = 0;
	INT32	_writePos = 0;
	vector<BYTE>	_buffer;
};



