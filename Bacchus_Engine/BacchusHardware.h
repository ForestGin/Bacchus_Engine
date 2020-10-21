#ifndef __BACCHUS_HARDWARE_H__
#define __BACCHUS_HARDWARE_H__

#include "Module.h"
#include <string>

struct hw_info {
	char sdl_version[25] = "";
	float ram_gb = 0.f;
	uint cpu_count = 0;
	uint l1_cachekb = 0;
	bool rdtsc = false;
	bool altivec = false;
	bool now3d = false;
	bool mmx = false;
	bool sse = false;
	bool sse2 = false;
	bool sse3 = false;
	bool sse41 = false;
	bool sse42 = false;
	bool avx = false;
	bool avx2 = false;
	std::string gpu_vendor;
	std::string gpu_driver;
	std::string gpu_brand;
	float vram_mb_budget = 0.f;
	float vram_mb_usage = 0.f;
	float vram_mb_available = 0.f;
	float vram_mb_reserved = 0.f;
};

class BacchusHardware : public Module
{

public:

	BacchusHardware(bool start_enabled = true);
	~BacchusHardware();

	const hw_info& GetInfo() const;

private:

	mutable hw_info info;
};

#endif