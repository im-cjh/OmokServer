#pragma once

class MyBuffer
{
public:
	MyBuffer() = default;

	MyBuffer(INT32 pBufferSize) :
		bufferSize(pBufferSize)
	{
		buffer = new BYTE(pBufferSize);
	}


public:
	BYTE* buffer;
	int bufferSize;

};

