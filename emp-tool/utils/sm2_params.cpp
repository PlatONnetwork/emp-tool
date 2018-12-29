#ifdef OT_NP_USE_MIRACL

#include "emp-tool/utils/sm2_params.h"
#include <mutex>
#include <stdio.h>
#include <thread>



#define SM2_NUMWORD 32
#define ERR_ECURVE_INIT 0x00000001
#define ERR_ORDER 0x00000004



namespace emp
{

static bool gs_init = false;
static std::mutex SM2_lock;


thread_local bool SM2_initialized = false;
thread_local big g_p;
thread_local epoint *g_G;

big getP()
{
	return g_p;
}

epoint* getG()
{
	return g_G;
}

//static miracl* s_mr_mip = NULL;

void SM2_load()
{
	//printf("----   initMiracl -----\n");
	if (!gs_init)
	{
		std::lock_guard<std::mutex> lock(SM2_lock);
		if (!gs_init)
		{
			//printf("----  initMiracl impl -----\n");
			mr_init_threading();
			gs_init = true;
		}
	}
}

void SM2_unload()
{
	//printf("----   initMiracl -----\n");
	if (gs_init)
	{
		std::lock_guard<std::mutex> lock(SM2_lock);
		if (gs_init)
		{
			//printf("----  initMiracl impl -----\n");
			mr_end_threading();
			gs_init = false;
		}
	}
}


int SM2_Init()
{
	//printf("----   initMiracl -----\n");
	if (!gs_init)
	{
		SM2_load();
	}

	if (!SM2_initialized)
	{
		//printf("----  SM2_Init impl -----\n");
		miracl *mip = mirsys(10000, 16);

		big Gx;
		big Gy;
		big a;
		big b;
		big n;
		epoint *nG;

		Gx = mirvar(0);
		Gy = mirvar(0);
		g_p = mirvar(0);
		a = mirvar(0);
		b = mirvar(0);
		n = mirvar(0);
		bytes_to_big(SM2_NUMWORD, (const char*)SM2_Gx, Gx);
		bytes_to_big(SM2_NUMWORD, (const char*)SM2_Gy, Gy);
		bytes_to_big(SM2_NUMWORD, (const char*)SM2_p, g_p);
		bytes_to_big(SM2_NUMWORD, (const char*)SM2_a, a);
		bytes_to_big(SM2_NUMWORD, (const char*)SM2_b, b);
		bytes_to_big(SM2_NUMWORD, (const char*)SM2_n, n);
		ecurve_init(a, b, g_p, MR_PROJECTIVE);
		g_G = epoint_init();
		nG = epoint_init();
		if (!epoint_set(Gx, Gy, 0, g_G))//initialise point G
		{
			return ERR_ECURVE_INIT;
		}
		ecurve_mult(n, g_G, nG);
		/*if (!point_at_infinity(nG)) //test if the order of the point is n
		{
			return ERR_ORDER;
		}*/

		mirkill(Gx);
		mirkill(Gy);
		mirkill(a);
		mirkill(b);
		mirkill(n);
		epoint_free(nG);
		SM2_initialized = true;
		//printf("----  SM2_Init impl success -----\n");
	}

	return 0;
}

// unit miracl
int SM2_UnInit()
{
	//printf("----   uninitMiracl -----\n");
	if (!gs_init)
	{
		std::lock_guard<std::mutex> lock(SM2_lock);
		if (!gs_init)
		{
			//printf("----  initMiracl threading impl -----\n");
			mr_init_threading();
			gs_init = true;
		}
	}

	if (SM2_initialized)//for each thread run mpc
	{
	    //thread local heap memory should free
		mirkill(g_p);
		epoint_free(g_G);
		
		//printf("----  SM2_UnInit impl -----\n");
		mirexit();
		SM2_initialized = false;
	}

	return 0;
}

}//emp

#endif//OT_NP_USE_MIRACL
