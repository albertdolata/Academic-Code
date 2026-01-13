#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "rand.h"
#include <string>

class CSensor {
private:
friend ostream& operator<<(ostream& os, const CSensor &pSensor);
  float lowLimit;
  float highLimit;

protected:
mutable  float Measurement;

  float makeMeasurement() const { 
	 return Measurement = getRandomNumber(lowLimit, highLimit); 
	  }

  void checkLimits(float baseLowLimit, float baseHighLimit) {
    if (this->lowLimit < baseLowLimit || this->highLimit > baseHighLimit) {
      this->lowLimit = baseLowLimit;
      this->highLimit = baseHighLimit;
      cout << "Warning: Limits of " << this->getName()
           << " sensor are out of range. Sensor limits were "
              "reset to default values. \n"
           << endl;
    }
  }

public:
  CSensor(float lowLimit, float highLimit)
      : lowLimit(lowLimit), highLimit(highLimit) {
		  makeMeasurement();
		  }
      
  ~CSensor() {}
  
  float getMeasurement() const {
	  makeMeasurement();
    return Measurement;
  }

  virtual string getName() const = 0;
  virtual string getUnit() const = 0;
};

class CTemperatureSensor : public CSensor {
public:
  CTemperatureSensor(float lowLimit, float highLimit)
      : CSensor(lowLimit, highLimit) {
    checkLimits(-50.0, 100.0);
  }

  string getName() const { return "temeprature"; };
  string getUnit() const { return "ºC"; };
};

class CHumiditySensor : public CSensor {
public:
  CHumiditySensor(float lowLimit, float highLimit)
      : CSensor(lowLimit, highLimit) {
    checkLimits(0.0, 100.0);
  }

  string getName() const { return "humidity"; };
  string getUnit() const { return "%"; };
};
class CPressureSensor : public CSensor {
public:
  CPressureSensor(float lowLimit, float highLimit)
      : CSensor(lowLimit, highLimit) {
    checkLimits(0.0, 1200.0);
  }

  string getName() const { return "atmosferic preassure"; };
  string getUnit() const { return "Pa"; };
};

class CWindSensor : public CSensor {
public:
  CWindSensor(float lowLimit, float highLimit) : CSensor(lowLimit, highLimit) {
    checkLimits(0.0, 100.0);
  }

  string getName() const { return "wind speed"; };
  string getUnit() const { return "km/h"; };
};

class CInsolationSensor : public CSensor {
public:
  CInsolationSensor(float lowLimit, float highLimit)
      : CSensor(lowLimit, highLimit) {
    checkLimits(0.0, 1500.0);
  }

  string getName() const { return "insolation"; };
  string getUnit() const { return "W/m²"; };
};

ostream& operator<<(ostream& os, const CSensor &pSensor) {
	os << "Sensor: " << pSensor.getName() << " | " << "Sensor measurement: " << pSensor.getMeasurement() << " " << pSensor.getUnit() << endl;
	return os; 
	}

#endif
