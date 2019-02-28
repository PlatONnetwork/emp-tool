#include "pb_utils.h"
#include <string>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/wire_format_lite.h>

using namespace google::protobuf::io;
using namespace google::protobuf::internal;

namespace platon
{
	namespace mpc
	{
		int WriteVarString(void* buffer, int len, const std::string&  value)
		{
			ArrayOutputStream zout(buffer, len);
			CodedOutputStream out(&zout);
			out.WriteVarint32(value.size());
			out.WriteString(value);

			return out.ByteCount();
		}

		int WriteVarUint32(void* buffer, int len, uint32_t  value)
		{
			ArrayOutputStream zout(buffer, len);
			CodedOutputStream out(&zout);
			out.WriteVarint32(value);

			return out.ByteCount();
		}

		int WriteVarUint64(void* buffer, int len, uint64_t  value)
		{
			ArrayOutputStream zout(buffer, len);
			CodedOutputStream output(&zout);
			output.WriteVarint64(value);

			return output.ByteCount();
		}

		int WriteVarInt32(void* buffer, int len, int32_t  value)
		{
			ArrayOutputStream zout(buffer, len);
			CodedOutputStream out(&zout);
			out.WriteVarint32SignExtended(value);

			return out.ByteCount();
		}

		int WriteVarInt64(void* buffer, int len, int64_t  value)
		{
			ArrayOutputStream zout(buffer, len);
			CodedOutputStream output(&zout);
			output.WriteVarint64(static_cast<uint64_t>(value));

			return output.ByteCount();
		}

		int WriteVarFloat(void* buffer, int len, float value)
		{
			ArrayOutputStream zout(buffer, len);
			CodedOutputStream output(&zout);
			output.WriteLittleEndian32(WireFormatLite::EncodeFloat(value));

			return output.ByteCount();
		}

		int WriteVarDouble(void* buffer, int len, double value)
		{
			ArrayOutputStream zout(buffer, len);
			CodedOutputStream output(&zout);
			output.WriteLittleEndian64(WireFormatLite::EncodeDouble(value));

			return output.ByteCount();
		}

		int WriteVarBool(void* buffer, int len, bool value)
		{
			ArrayOutputStream zout(buffer, len);
			CodedOutputStream output(&zout);

			output.WriteVarint32(value ? 1 : 0);
			return output.ByteCount();
		}

		int ReadVarString(const void* buffer, int len, std::string& value)
		{
			ArrayInputStream zin(buffer, len);
			CodedInputStream input(&zin);

			uint32_t size;
			if (!input.ReadVarint32(&size))
				return -1;

			if (!input.ReadString(&value, size))
				return -1;

			return value.size();
		}

		int ReadVarBool(const void* buffer, int len, bool& value)
		{
			ArrayInputStream zin(buffer, len);
			CodedInputStream input(&zin);

			uint32_t temp;
			if (!input.ReadVarint32(&temp))
				return -1;

			value = temp != 0;
			return true;
		}

		int ReadVarUint32(const void* buffer, int len, uint32_t& value)
		{
			ArrayInputStream zin(buffer, len);
			CodedInputStream input(&zin);
			if (!input.ReadVarint32(&value))
			{
				printf("read the ret failed\n");
				return -1;
			}

			return input.CurrentPosition();
		}

		int ReadVarUint64(const void* buffer, int len, uint64_t& value)
		{
			ArrayInputStream zin(buffer, len);
			CodedInputStream input(&zin);
			if (!input.ReadVarint64(&value))
				return -1;

			return input.CurrentPosition();
		}

		int ReadVarInt32(const void* buffer, int len, int32_t& value)
		{
			ArrayInputStream zin(buffer, len);

			CodedInputStream input(&zin);
			uint32_t temp;
			if (!input.ReadVarint32(&temp))
			{
				printf("read the ret failed\n");
				return -1;
			}

			value = static_cast<int32_t>(temp);
			return input.CurrentPosition();
		}

		int ReadVarInt64(const void* buffer, int len, int64_t& value)
		{
			uint64_t temp;
			ArrayInputStream zin(buffer, len);
			CodedInputStream input(&zin);

			if (!input.ReadVarint64(&temp))
				return -1;

			value = static_cast<int64_t>(temp);
			return input.CurrentPosition();
		}

		int ReadVarFloat(const void* buffer, int len, float& value)
		{
			uint32_t temp;
			ArrayInputStream zin(buffer, len);
			CodedInputStream input(&zin);
			if (!input.ReadLittleEndian32(&temp))
				return -1;

			value = WireFormatLite::DecodeFloat(temp);
			return input.CurrentPosition();
		}

		int ReadVarDouble(const void* buffer, int len, double& value)
		{
			uint64_t temp;
			ArrayInputStream zin(buffer, len);
			CodedInputStream input(&zin);
			if (!input.ReadLittleEndian64(&temp))
				return -1;

			value = WireFormatLite::DecodeDouble(temp);
			return input.CurrentPosition();
		}

	}
}