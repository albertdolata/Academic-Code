#pragma once
#include "sensor.h"
#include <vector>
typedef unsigned int uint;

class CBase {
private:
friend ostream& operator<<(ostream& os, const CBase &base);
  vector<const CSensor *> Sensors;
  uint numOfSensors = 0;

public:
  CBase(const uint size) {
    Sensors.resize(size);
  }
  
  CBase(const CBase &base) {
    this->numOfSensors = base.numOfSensors;
    this->Sensors = base.Sensors;
  }

  CBase &operator=(const CBase &base) {
	vector <const CSensor*> tmp = base.Sensors;
	this->Sensors.clear();
    this->Sensors.swap(tmp);
    this->numOfSensors = base.numOfSensors;
    return *this;
  }
  
  CBase &operator+=(const CSensor* pSensor) {
	  setSensor(pSensor);
	  return *this;
	  }
  
  CBase &operator-=(const CSensor* pSensor) {
	  bool erase = false;
	  for(uint i = 0; i < Sensors.size(); i++){
		  if(Sensors[i] == pSensor){
			  Sensors.erase(Sensors.begin() + i);
			  numOfSensors--;
			  erase = true;
			  break;
			  }else{
				  continue;
			}
		  }
		  if(!erase){
			  throw CException("Couldn't find given Sensor. Sensor removal failed.\n");
			  }
		return *this;
	  }
  
  ~CBase() { Sensors.clear(); }

  void setSensor(const CSensor* pSensor) {
    if (numOfSensors == Sensors.size()) {
      throw CException("Exceed size of sesnsors array.\n");
    } else {
      Sensors[numOfSensors++] = pSensor;
      cout << "Added Sensor to the base" << endl;
    }
  }
  
  void displayMeasurements() const {
    for (uint i = 0; i < numOfSensors; i++) {
      cout << "Current: " << Sensors[i]->getName() << ": "
           << Sensors[i]->getMeasurement() << " " << Sensors[i]->getUnit()
           << endl;
    }
    cout << endl;
  }
};

ostream& operator<<(ostream& os, const CBase &base){
	cout << "Sensors in the given base: \n" << endl;
	for( uint i = 0; i < base.numOfSensors; i++){
		os << i+1 << ". " << *(base.Sensors[i]) << endl;
		} 
	return os;
	}
