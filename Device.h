#ifndef __DEVICE_H
#define __DEVICE_H

#include <Arduino.h>
#include "Message.h"

class DataMessage;
class sNET;

class AIRQBaseDevice {
friend class sNET;

protected:
	DataMessage *status;
	sNET *snet;
	
	AIRQBaseDevice(DataMessage *message);
	~AIRQBaseDevice();
	void setSNETReference(sNET *snetptr) {snet = snetptr;};
	void updateFromRawMessage(__data_message *message);
	
public:
	uint8_t *getDeviceID();
	DataMessage *getLastMessage();
	uint8_t getLQI();
	int8_t getRSSI();
	virtual bool statusChanged() {return false;};
};

typedef enum {
	ON = 1,
	OFF = 0
} IO_STATUS;

class AIRQControlBoard: public AIRQBaseDevice {
friend class sNET;

private:
	uint8_t lstatus;

protected:
	AIRQControlBoard(DataMessage *message);
	~AIRQControlBoard() { };

public:
	virtual bool statusChanged();
	void sendSetMessage(uint8_t *data, uint8_t len);
	
#ifdef SNET_ENABLE_CONFIRM
	void setIO(uint8_t *data, uint8_t len, bool check=false, uint8_t timeout=-1);
#else
	void setIO(uint8_t *data, uint8_t len);
#endif
};


class AIRQ300 : public AIRQControlBoard {
friend class sNET;
	
protected:
	AIRQ300(DataMessage *message);
	~AIRQ300() { };
	
public: 
	IO_STATUS getIN1() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getIN1();};
	IO_STATUS getIN2() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getIN2();};
	IO_STATUS getIN3() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getIN3();};
	IO_STATUS getIN4() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getIN4();};
	IO_STATUS getPOWER() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getPOWER();};
	IO_STATUS getRELAY1() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getRELAY1();};
	IO_STATUS getRELAY2() {return (IO_STATUS)((AIRQ300DataMessage*)status)->getRELAY2();};
	
	void setRELAY1(IO_STATUS status, bool check=false, uint8_t timeout=-1);
	void setRELAY2(IO_STATUS status, bool check=false, uint8_t timeout=-1);

};

class AIRQ310 : public AIRQControlBoard {
friend class sNET;

protected:
	AIRQ310(DataMessage *message);
	~AIRQ310() { };
	
public: 
	IO_STATUS getPOWER() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getPOWER();};
	IO_STATUS getRELAY1() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY1();};
	IO_STATUS getRELAY2() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY2();};	
	IO_STATUS getRELAY3() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY3();};	
	IO_STATUS getRELAY4() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY4();};	
	IO_STATUS getRELAY5() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY5();};				
	IO_STATUS getRELAY6() {return (IO_STATUS)((AIRQ310DataMessage*)status)->getRELAY6();};		
	void setRELAY1(IO_STATUS status, bool check=false, uint8_t timeout=-1);
	void setRELAY2(IO_STATUS status, bool check=false, uint8_t timeout=-1);
	void setRELAY3(IO_STATUS status, bool check=false, uint8_t timeout=-1);
	void setRELAY4(IO_STATUS status, bool check=false, uint8_t timeout=-1);
	void setRELAY5(IO_STATUS status, bool check=false, uint8_t timeout=-1);
	void setRELAY6(IO_STATUS status, bool check=false, uint8_t timeout=-1);			
};

#endif //__DEVICE_H