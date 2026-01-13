#include <unistd.h>	//Plik nagłówkowy, w kórym jest zadeklarowana użyta poniżej funkcja sleep()
#include <iostream>

#include "exception.h"	// Plik nagłówkowy zawierający definicję klasy wyjątku
#include "sensor.h"		//Plik nagłówkowy, w którym należy zadeklarować klasy reprezentujące czujniki
#include "base.h"		//Plik nagłówkowy, w kórym należy zadeklarować klasę reprezentującą stację bazową

int main()
{
	const CTemperatureSensor TemperatureSensor(-35.0, -30.0);	//Utworzenie obiektu reprezentującego czujnik temperatury
	CHumiditySensor HumiditySensor(95.0, 100.0);				//Utworzenie obiektu reprezentującego czujnik wilgotności
	CPressureSensor PressureSensor(950, 1050);				//Utworzenie obiektu reprezentującego czujnik ciśnienia atmosferycznego
	CWindSensor WindSensor(0, 10000);							//Utworzenie obiektu reprezentującego czujnik prędkości wiatru
	CInsolationSensor InsolationSensor(0, 1000);				//Utworzenie obiektu reprezentującego czujnik nasłonecznienia

	cout << TemperatureSensor;
	cout << HumiditySensor;
	cout << PressureSensor;
	cout << WindSensor;
	cout << InsolationSensor;

	CBase Base(60);	//Utworzenie pierwszego obiektu reprezentującego stację bazową

	try
	{
		Base += &TemperatureSensor;		//Przekazanie stacji bazowej adresu czujnika temperatury
		Base += &HumiditySensor;
		Base += &PressureSensor;
		Base += &WindSensor;
		Base += &InsolationSensor;
	}
	catch (CException& e)
	{
		cout << "Adding sensors failed with message: " << e.GetMessage() << endl;
	}

	cout << Base;

	try
	{
		Base -= &HumiditySensor;		//Usuwanie ze stacji bazowej adresu czujnika wilgotności
		Base -= &HumiditySensor;
	}
	catch (CException& e)
	{
		cout << "Removing sensors failed with message: " << e.GetMessage() << endl;
	}

	cout << Base;

	return 0;
}
