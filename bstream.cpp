#include "bstream.h"
#include <cstring>

bStream::bStream(std::string path, Endianess ord, RW rw){
    if(rw == RW::Read) base.open(path, std::ios::in | std::ios::binary);
    if(rw == RW::Write) base.open(path, std::ios::out | std::ios::binary);
	filePath = path;
	order = ord;
	systemOrder = getSystemEndianess();
}

std::fstream &bStream::getStream(){
	return base;
}

std::string bStream::getPath(){
	return filePath;
}

Endianess bStream::getSystemEndianess(){
	union {
		uint32_t integer;
		uint8_t bytes[sizeof(uint32_t)];
	} check;
	check.integer = 0x01020304U;
	return (check.bytes[0] == 0x01 ? Endianess::Big : Endianess::Little);
}

void bStream::seek(long pos){
	base.seekg(pos, base.beg);
}

long bStream::tell(){
	return base.tellg();
}

uint32_t bStream::readUInt32(){
	uint32_t r;
	base.read((char*)&r, sizeof(uint32_t));
	if(order != systemOrder){
		return ( ((r>>24)&0xFF) | ((r<<8) & 0xFF0000) | ((r>>8)&0xFF00) | ((r<<24)&0xFF000000));
	}
	else{
		return r;
	}
}

int32_t bStream::readInt32(){
	int32_t r;
	base.read((char*)&r, sizeof(int32_t));
	if(order != systemOrder){
		return ( ((r>>24)&0xFF) | ((r<<8) & 0xFF0000) | ((r>>8)&0xFF00) | ((r<<24)&0xFF000000));
	}
	else{
		return r;
	}
}

uint16_t bStream::readUInt16(){
	uint16_t r;
	base.read((char*)&r, sizeof(uint16_t));
	if(order != systemOrder){
		return ( ((r<<8)&0xFF00) | ((r>>8)&0x00FF) );
	}
	else{
		return r;
	}
}

int16_t bStream::readInt16(){
	int16_t r;
	base.read((char*)&r, sizeof(int16_t));
	if(order != systemOrder){
		return ( ((r<<8)&0xFF00) | ((r>>8)&0x00FF) );
	}
	else{
		return r;
	}
}

uint8_t bStream::readUInt8(){
	uint8_t r;
	base.read((char*)&r, sizeof(uint8_t));
	return r;
}

int8_t bStream::readInt8(){
	int8_t r;
	base.read((char*)&r, sizeof(int8_t));
	return r;
}

float bStream::readFloat(){
	char buff[sizeof(float)];
	base.read(buff, sizeof(float));
	if(order != systemOrder){
		char temp[sizeof(float)];
		temp[0] = buff[3];
		temp[1] = buff[2];
		temp[2] = buff[1];
		temp[3] = buff[0];
		return *((float*)temp);
	}
	return *((float*)buff);
}

char* bStream::readBytes(int count){
	char* buffer = new char[count];
	base.read(buffer, (size_t)count);
	return buffer;
}

std::string bStream::readString(int len){
	char* str = (char*)std::malloc(len);
	base.read(str, len);
	std::string stdstr(str);
	std::free(str);
	return stdstr;
}

void bStream::writeInt8(int8_t v){
	base.write((char*)&v, 1);
}

void bStream::writeUInt8(uint8_t v){
	base.write((char*)&v, 1);
}

void bStream::writeInt16(int16_t v){
	if(order != systemOrder){
		v = (((v<<8)&0xFF00) | ((v>>8)&0x00FF));
	}
	base.write((char*)&v, sizeof(uint16_t));
}

void bStream::writeUInt16(uint16_t v){
	if(order != systemOrder){
		v = (((v<<8)&0xFF00) | ((v>>8)&0x00FF));
	}
	base.write((char*)&v, sizeof(uint16_t));
}

void bStream::writeInt32(int32_t v){
	if(order != systemOrder){
	   v = (((v>>24)&0xFF) | ((v<<8) & 0xFF0000) | ((v>>8)&0xFF00) | ((v<<24)&0xFF000000));
	}
	base.write((char*)&v, sizeof(int32_t));
}

void bStream::writeUInt32(uint32_t v){
	if(order != systemOrder){
	   v = (((v>>24)&0xFF) | ((v<<8) & 0xFF0000) | ((v>>8)&0xFF00) | ((v<<24)&0xFF000000));
	}
	base.write((char*)&v, sizeof(uint32_t));
}

void bStream::writeFloat(float v){
	char* buff = (char*)&v;
	if(order != systemOrder){
		char temp[sizeof(float)];
		temp[0] = buff[3];
		temp[1] = buff[2];
		temp[2] = buff[1];
		temp[3] = buff[0];
		v = *((float*)temp);
	}
	base.write((char*)&v, sizeof(float));
}

void bStream::writeString(std::string v){
	base.write(v.c_str(), v.size());
}

void bStream::writeBytes(char* v, size_t size){
	base.write(v, size);
}

void bStream::readStruct(void* out, size_t size){
	base.read((char*)out, size);
}

uint8_t bStream::peekU8(int offset){
	uint8_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt8();
	base.seekg(pos, base.beg);
	return ret;
}

int8_t bStream::peekI8(int offset){
	int8_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt8();
	base.seekg(pos, base.beg);
	return ret;
}

uint16_t bStream::peekU16(int offset){
	uint16_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt16();
	base.seekg(pos, base.beg);
	return ret;
}

int16_t bStream::peekI16(int offset){
	int16_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt16();
	base.seekg(pos, base.beg);
	return ret;
}

uint32_t bStream::peekU32(int offset){
	uint32_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readUInt32();
	base.seekg(pos, base.beg);
	return ret;
}

int32_t bStream::peekI32(int offset){
	int32_t ret;
	int pos = base.tellg();
	base.seekg(offset, base.beg);
	ret = readInt32();
	base.seekg(pos, base.beg);
	return ret;
}
