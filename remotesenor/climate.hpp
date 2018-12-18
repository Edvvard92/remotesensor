// Developer note stack size needs to be increased to allow the allocation of memory for this double = 8 bytes
// 8 * 24 * 60 * 60 * 2 arrays of doubles = 1382400 bytes base allocation is 1MB 
// In VS2017 go to project > properties > System > and set Stack Reserve Size and Stack Commit Size to 2000000
// This prevents a stack overflow

#define maximum_readings 24 * 60 * 60
#define invalid_reading  -1000.0


#define fakedata 1	 // NOTE:  Set to 1 use fake sensor values instead of the online values 0

#include <chrono>  // System time
#include <stdexcept>

#if fakedata == 1

#include "fakesensor.hpp"

#else

#include "http.h"

#endif


using namespace std;
using namespace chrono;


class Climate {

private:

#if fakedata == 1	
	FakeSensor sensorDevice;
#else
	Http sensorDevice;
#endif

protected:
	int samples;
public:

	// Constructors
	Climate();

	// Utility
	void	clearSamples();

	// Sensor related
	long readSensor();
	float tempArray[maximum_readings];
	float humArray[maximum_readings];
	system_clock::time_point StartTime;
	long sampleCount(long seconds);
	//long sampleTotal();
	//double getTemperature(long tempSeconds);
	//double getHumidity(long humSeconds);
	double maximumTemperature(long tempSeconds);
};

// Constructor
Climate::Climate() {
	
}


void	Climate::clearSamples() {
	for (int i = 0; i <= maximum_readings; i++) {
		tempArray[i] = 0;
		humArray[i] = 0;
		StartTime = std::chrono::system_clock::now();
	}
	/*for (int i = 1; i < 7; i++) {
		cout << tempArray[i] << endl;
	}*/
	
	//cout << "Samples cleared." << endl;
}


long	Climate::readSensor() {
	//int currentSecond = 0;	
	sensorDevice.read_data();
	system_clock::time_point EndTime = std::chrono::system_clock::now();
	std::chrono::duration<double> currentSecond = EndTime - StartTime;
	int time = (int)currentSecond.count();	
	cout << endl << "Debugging information : " << "Temperature is " << sensorDevice.get_temperature_in_c() << " degrees Celsius " << sensorDevice.get_humidity() << "% humidity" << endl;
	tempArray[time] = sensorDevice.get_temperature_in_c();
	humArray[time] = sensorDevice.get_humidity();	
	/*for (int i = 1; i < 7; i++) {
		cout << tempArray[i] << endl;
	}*/		
	return time;
	// This line is purely for your debugging and can be removes/commented out in the final code.
}

long Climate::sampleCount(long seconds) {
	for (int n = seconds; n > 1; --n) {
		if (n != NULL) {
			samples++;			
		}
	}
	return samples;
}

/*long Climate::sampleTotal() {
	
}*/

/*double Climate::getTemperature(long tempSeconds){
	
}*/

/*double Climate::getHumidity(long humSeconds){

}*/

double Climate::maximumTemperature(long tempSeconds){
	for (int i = 0; i < tempArray[i]; i++)
		if (maximumTemperature < tempArray[i])
}


