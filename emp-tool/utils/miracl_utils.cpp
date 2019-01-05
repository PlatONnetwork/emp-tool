#ifdef OT_NP_USE_MIRACL
#include "emp-tool/utils/miracl_utils.h"
#include "emp-tool/utils/sm2_params.h"
#include "emp-tool/utils/miracl_otnp_utils.h"
#include "emp-tool/utils/miracl_hash.h"
#include <cmath>
#include <algorithm>    // std::max

namespace emp
{
		const static int EP_SIZES = 64;
		const static int FB_BYTES = 32;
		
		extern big getP();
		extern epoint* getG();

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

		void otnp_init(void** epointC)
		{
			SM2_Init();
				
			big c;
			c = mirvar(0);
			bigrand(getP(), c);
			*epointC = epoint_init();
			ecurve_mult(c, getG(), (epoint*)(*epointC));  // C = c*G
			//big value should be free
			mirkill(c);
		}

		void otnp_uninit(void** epointC)
		{
			if (*epointC)
				epoint_free((epoint*)(*epointC));
		}


		int otnp_send(IOChannel* io, void** epointC, const block* data0, const block* data1, int length)
		{
			if (*epointC == nullptr)
				otnp_init(epointC);

			int ret = 0;
			big r = mirvar(0);
			epoint *h1;
			h1 = epoint_init();
			epoint **hList = new epoint*[length];
			epoint **grList = new epoint*[length];
			//epoint **grList = (epoint **)mr_alloc(length, sizeof(epoint*));
			for (int i = 0; i < length; ++i)
			{
				hList[i] = epoint_init();
				grList[i] = epoint_init();
			}
			block *m = new block[length * 2];


			do {

				//recv hlis
				ret = io->recv_ep((void**)hList, length);
				if (ret != 0)
				{
					//LOGE("OTNP recv epoint list failed, eppoint count: %d", length);
					ret = -1;
					break;
				}

				for (int i = 0; i < length; i++)
				{
					bigrand(getP(), r);

					ecurve_mult(r, getG(), grList[i]);  // g^r  
					epoint_copy((epoint*)(*epointC), h1);   //h1 = C
					ecurve_sub(hList[i], h1);    // h1 = C-h
					ecurve_mult(r, hList[i], hList[i]);   // h' = h^r
					ecurve_mult(r, h1, h1);   // h1 = (C-h)^r

					/*m[0] = xorBlocks(data0[i], m0[i]);
					m[1] = xorBlocks(data1[i], m1[i]);*/
					m[2 * i] = xorBlocks(data0[i], Hash::hash_for_epoint(hList[i]));   //H(h^r)
					m[2 * i + 1] = xorBlocks(data1[i], Hash::hash_for_epoint(h1));  // H((C/h)^r) 

				}

				//printf("*** OTNP send blocks: \n");
				//send gr list 
				ret = io->send_ep((void**)grList, length);
				if (ret != 0)
				{
					//LOGE("OTNP send epoint list failed, eppoint count: %d", length);
					ret = -1;
					break;
				}

				//send block list
				ret = io->send_block(m, length * 2);
				if (ret != 0)
				{
					//LOGE("OTNP send blocks list failed, block count: %d", length * 2);
					ret = -1;
					break;
				}

			} while (0);

			//OTNP_SEND_EXIT:
				//free values
			for (int i = 0; i < length; ++i)
			{
				epoint_free(grList[i]);
				epoint_free(hList[i]);
			}

			delete[]grList;
			delete[]m;
			delete[]hList;
			mirkill(r);
			epoint_free(h1);

			return ret;
		}

		int otnp_recv(IOChannel* io, void** epointC, block* data, const bool* b, int length)
		{
			if (*epointC == nullptr)
				otnp_init(epointC);

			int ret = 0;
			big *k = new big[length];
			epoint *pk1;//epoint *h;

			block *m = new block[2 * length];
			epoint **hList = new epoint*[length];
			epoint **grList = new epoint*[length];

			//init k, h, gr list,   notice: big value should free
			for (int i = 0; i < length; ++i)
			{
				k[i] = mirvar(0);
				hList[i] = epoint_init();
				grList[i] = epoint_init();
			}
			pk1 = epoint_init();

			//k random
			for (int i = 0; i < length; i++)
			{
				bigrand(getP(), k[i]);
			}


			//send hList
			for (int i = 0; i < length; i++)
			{
				if (b[i])
				{
					ecurve_mult(k[i], getG(), pk1);   // pk1 = g^ki
					epoint_copy((epoint*)(*epointC), hList[i]);  // h = C
					ecurve_sub(pk1, hList[i]);  // h = C - pk1 = C - g^ki
				}
				else
				{
					ecurve_mult(k[i], getG(), hList[i]);  // h = g^ki
				}
			}

			do {

				ret = io->send_ep((void**)hList, length);
				if (ret != 0)
				{
					//LOGE("OTNP send epoint list failed, eppoint count: %d", length);
					ret = -1;
					break;
				}

				//printf("*** OTNP recv blocks: \n");
				//recv gr list
				ret = io->recv_ep((void**)grList, length);
				if (ret != 0)
				{
					//LOGE("OTNP recv epoint list failed, eppoint count: %d", length);
					ret = -1;
					break;
				}

				//recv block list
				ret = io->recv_block(m, 2 * length);
				if (ret != 0)
				{
					//LOGE("OTNP recv blocks list failed, block count: %d", 2 * length);
					ret = -1;
					break;
				}

				for (int i = 0; i < length; ++i)
				{
					int ind = b[i] ? 1 : 0;

					ecurve_mult(k[i], grList[i], grList[i]);  // gr = gr^ki
					// data_b =H(gr^ki) + m_b
					block tempB1 = Hash::hash_for_epoint(grList[i]);
					block tempB2 = Hash::hash_for_epoint(grList[i]);
					data[i] = xorBlocks(m[2 * i + ind], Hash::hash_for_epoint(grList[i]));
				}

			} while (0);

			//OTNP_RECV_EXIT:

				//free values
			for (int i = 0; i < length; ++i)
			{
				mirkill(k[i]);
				epoint_free(hList[i]);
				epoint_free(grList[i]);
			}
			delete[]k;
			delete[]hList;
			delete[]grList;
			delete[]m;
			epoint_free(pk1);

			return ret;
		}

}//emp

#endif//OT_NP_USE_MIRACL
