#pragma once

#include <cstdint>
#include <string>

namespace platon
{
	namespace mpc
	{
		/*
		@desc write a string with no tag to a buffer with protocol buffer encoded format
		@param buffer the output buffer address
		@param len the length of the buffer
		@param value the input string 
		@ret the input string with protocol buffer encoded format
		*/
		int WriteVarString(void* buffer, int len, const std::string&  value);

		/*
		@desc write a 32 bits unsigned int to a buffer with protocol buffer encoded format
		@param buffer the output buffer address
		@param len the length of the buffer
		@param value the input value with uint32_t type
		@ret the input value with protocol buffer encoded format
		*/
		int WriteVarUint32(void* buffer, int len, uint32_t  value);

		/*
		@desc write a 64 bits unsigned int to a buffer with protocol buffer encoded format
		@param buffer the output buffer address
		@param len the length of the buffer
		@param value the input value with uint64_t type
		@ret the input value with protocol buffer encoded format
		*/
		int WriteVarUint64(void* buffer, int len, uint64_t  value);

		/*
		@desc write a 32 bit signed int to a buffer with protocol buffer encoded format
		@param buffer the output buffer address
		@param len the length of the buffer
		@param value the input value with int32_t type
		@ret the input value with protocol buffer encoded format
		*/
		int WriteVarInt32(void* buffer, int len, int32_t  value);

		/*
		@desc write a 64 bit signed int to a buffer with protocol buffer encoded format
		@param buffer the output buffer address
		@param len the length of the buffer
		@param value the input value with int64_t type
		@ret the input value with protocol buffer encoded format
		*/
		int WriteVarInt64(void* buffer, int len, int64_t  value);

		/*
		@desc write a 32 bits float int to a buffer with protocol buffer encoded format
		@param buffer the output buffer address
		@param len the length of the buffer
		@param value the input value with float type
		@ret the input value with protocol buffer encoded format
		*/
		int WriteVarFloat(void* buffer, int len, float value);

		/*
		@desc write a 64 bits double int to a buffer with protocol buffer encoded format
		@param buffer the output buffer address
		@param len the length of the buffer
		@param value the input value with double type
		@ret the input value with protocol buffer encoded format
		*/
		int WriteVarDouble(void* buffer, int len, double value);

		/*
		@desc write a bool value int to a buffer with protocol buffer encoded format
		@param buffer the output buffer address
		@param len the length of the buffer
		@param value the input value with bool type
		@ret the input value with protocol buffer encoded format
		*/
		int WriteVarBool(void* buffer, int len, bool value);


		/*
		@desc read a bool value from the buffer with protocol buffer encoded format
		@param buffer the input buffer address
		@param len the length of the buffer
		@param value the output value with bool type
		@ret the output value with protocol buffer encoded format
		*/
		int ReadVarString(const void* buffer, int len, std::string& value);

		/*
		@desc read a bool value from the buffer with protocol buffer encoded format
		@param buffer the input buffer address
		@param len the length of the buffer
		@param value the output value with bool type
		@ret the output value with protocol buffer encoded format
		*/
		int ReadVarBool(const void* buffer, int len, bool& value);

		/*
		@desc read a 32 bits unsigned int value from the buffer with protocol buffer encoded format
		@param buffer the input buffer address
		@param len the length of the buffer
		@param value the output value with int32 type
		@ret the output value with protocol buffer encoded format
		*/
		int ReadVarUint32(const void* buffer, int len, uint32_t& value);

		/*
		@desc read a 32 bits unsigned int value from the buffer with protocol buffer encoded format
		@param buffer the input buffer address
		@param len the length of the buffer
		@param value the output value with int64 type
		@ret the output value with protocol buffer encoded format
		*/
		int ReadVarUint64(const void* buffer, int len, uint64_t& value);


		/*
		@desc read a 32 bits signed int value from the buffer with protocol buffer encoded format
		@param buffer the input buffer address
		@param len the length of the buffer
		@param value the output value with int32 type
		@ret the output value with protocol buffer encoded format
		*/
		int ReadVarInt32(const void* buffer, int len, int32_t& value);

		/*
		@desc read a 32 bits signed int value from the buffer with protocol buffer encoded format
		@param buffer the input buffer address
		@param len the length of the buffer
		@param value the output value with int64 type
		@ret the output value with protocol buffer encoded format
		*/
		int ReadVarInt64(const void* buffer, int len, int64_t& value);


		/*
		@desc read a 32 bits float value from the buffer with protocol buffer encoded format
		@param buffer the input buffer address
		@param len the length of the buffer
		@param value the output value with float type
		@ret the output value with protocol buffer encoded format
		*/
		int ReadVarFloat(const void* buffer, int len, float& value);

		/*
		@desc read a 64 bits float value from the buffer with protocol buffer encoded format
		@param buffer the input buffer address
		@param len the length of the buffer
		@param value the output value with double type
		@ret the output value with protocol buffer encoded format
		*/
		int ReadVarDouble(const void* buffer, int len, double& value);

	}
}