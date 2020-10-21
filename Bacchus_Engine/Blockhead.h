#ifndef __BLOCKHEAD_H__
#define __BLOCKHEAD_H__

class Blockhead
{
public:

	Blockhead(char* title);
	virtual ~Blockhead();

	virtual bool Draw() = 0;

	virtual inline void OnOff();
	virtual inline void SetOnOff(bool set) { enabled = set; }
	virtual inline bool IsEnabled() const;
	virtual inline char* GetName() const { return name; }

protected:

	bool enabled = false;
	char* name = nullptr;
};


#endif