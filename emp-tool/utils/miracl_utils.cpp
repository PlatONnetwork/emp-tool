#ifdef OT_NP_USE_MIRACL
#include "emp-tool/utils/miracl_utils.h"
#include <cmath>
#include <algorithm>    // std::max

namespace emp
{
		const static int EP_SIZES = 64;
		const static int FB_BYTES = 32;

        int get_ep_sizes()
        {
            return EP_SIZES;
        }

        int get_fb_bytes()
        {
            return FB_BYTES;
        }
        
		int get_byte_size(big x) {
			int bit_x = numdig(x) << 2;
			int byte_x = (bit_x / 8) + (int)((bit_x % 8) > 0);
			return byte_x;
		}


		int get_bigs_size(const big* x, int num) 
		{
			int size = 0;
			for (int i = 0; i < num; ++i)
			{
				size += get_byte_size(x[i]);
			}

			return size;
		}

		int get_point_size(epoint *ep) {
			big x, y;
			x = mirvar(0);
			y = mirvar(0);
			int len_x, len_y;
			epoint_get(ep, x, y);
			len_x = get_byte_size(x);
			len_y = get_byte_size(y);

			//free mem
			mirkill(x);
			mirkill(y);
			//LOGD("the epoint size: %d ", len_x + len_y);
			return len_x + len_y;
		}

		int get_epoints_size(epoint** eps, size_t num)
		{
			size_t size = 0;
			for (size_t i = 0; i < num; ++i)
				size += get_point_size(*(eps + i));

			return size;
		}

		void point_to_bytes(unsigned char *buf, epoint *ep)
		{
			big x, y;
			//alloc mem
			x = mirvar(0);
			y = mirvar(0);
			epoint_get(ep, x, y);
			big_to_bytes(32, x, (char*)buf, 1);
			big_to_bytes(32, y, (char*)(buf + FB_BYTES), 1);

			//free mem
			mirkill(x);
			mirkill(y);
		}

		int epoint_to_bytes(unsigned char *buf, const epoint *ep)
		{
			int len = 0;
			big x, y;
			//alloc mem
			x = mirvar(0);
			y = mirvar(0);
			epoint_get((epoint*)ep, x, y);

			len += big_to_bytes(32, x, (char*)buf, 0);
			len += big_to_bytes(32, y, (char*)(buf + len), 0);

			//free mem
			mirkill(x);
			mirkill(y);
			//printf("epoint len: %d \n", len);
			return len;
		}

		void bytes_to_point(epoint* ep, unsigned char *buf, int len)
		{
			big x, y;
			x = mirvar(0);
			y = mirvar(0);
			bytes_to_big(FB_BYTES, (const char*)buf, x);
			bytes_to_big(FB_BYTES, (const char*)(buf + FB_BYTES), y);
			epoint_set(x, y, 1, ep);

			//free mem
			mirkill(x);
			mirkill(y);

		}

		int bytes_to_epoint(epoint* ep, const unsigned char *buf, int len)
		{
			big x, y;
			x = mirvar(0);
			y = mirvar(0);
			bytes_to_big(len/2, (const char*)buf, x);
			bytes_to_big(len/2, (const char*)(buf + len/2), y);
			epoint_set(x, y, 1, ep);

			//free mem
			mirkill(x);
			mirkill(y);

			return len;
		}

		//---------------------------------- function for test ---------------------------------------
		void printBig(big a)
		{
			int size = FB_BYTES;
			char* buf = new char[size];

			big_to_bytes(FB_BYTES, a, buf, 1);
			printHex((unsigned char*)buf, size);
			delete[]buf;
		}

		void printHex(const unsigned char* buffer, int size)
		{
			for (int i = 0; i < size; ++i)
			{
				printf("%02X", buffer[i] & 0xff);
			}
			printf("\n");
		}

		void printPoint(epoint *a)
		{
			int size = EP_SIZES;
			unsigned char *buf = new unsigned char[size];
			memset(buf, 0, size);

			point_to_bytes(buf, a);
			printHex(buf, size);

			delete[]buf;
		}

		void printBlock(block *a)
		{
			printHex((unsigned char*)a, sizeof(a));
		}


		static int base_represent_count(int base_old, int base_new)
		{
			int res = 0;
			if (base_new >= base_old)
				return 1;
			
			base_old /= base_new;
			int pad = 0;
			while (base_old)
			{
				res += 1;
				pad = base_old % base_new;
				base_old /= base_new;
			}

			if (pad > 0)
				res += 1;

			//printf("the count: %d \n", res);
			return res;
		}

		std::string transfer_base(const std::string& value, int old_base, int new_base)
		{
			miracl* mip = get_mip();
			int buffer_size = value.size() * base_represent_count(old_base, new_base);
			char* buffer = new char[buffer_size + 1];

			int origin = mip->IOBASE;
			big bigvalue;
			bigvalue = mirvar(0);
			mip->IOBASE = old_base;
			if (value[0] == '-')
			{
				memcpy(buffer, value.data()+1, value.size()-1);
				buffer[value.size()-1] = 0;
			}
			else
			{
				memcpy(buffer, value.data(), value.size());
				buffer[value.size()] = 0;
			}
			
			int len = cinstr(bigvalue, buffer);
			mip->IOBASE = new_base;
			len = cotstr(bigvalue, buffer);

			std::string res;
			if (value[0] == '-')
				res += '-';
			res.append(buffer, len);

			delete []buffer;
			mirkill(bigvalue);

			mip->IOBASE = origin;
			return res;
		}

}//emp

#endif//OT_NP_USE_MIRACL
