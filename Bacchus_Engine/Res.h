#ifndef __RES_H__
#define __RES_H__

#include "Globals.h"

class Res
{
public:
	enum class ResType
	{
		none = 0,
		mesh
	};

public:

	Res(Res::ResType type);
	virtual ~Res();

};

#endif
