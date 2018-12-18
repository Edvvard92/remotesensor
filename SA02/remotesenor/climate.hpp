// Developer note stack size needs to be increased to allow the allocation of memory for this double = 8 bytes
// 8 * 24 * 60 * 60 * 2 arrays of doubles = 1382400 bytes base allocation is 1MB 
// In VS2017 go to project > properties > System > and set Stack Reserve Size and Stack Commit Size to 2000000
// This prevents a stack overflow

#define maximum_readings 24 * 60 * 60
#define invalid_reading  -1000.0
#include <vector>;
#include <exception>;


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
	double temperature;
	double humidity;
	int samples;
	int totalSamples;
#if fakedata == 1	
	FakeSensor sensorDevice;
#else
	Http sensorDevice;
#endif

protected:
	float tempArray[maximum_readings];
	float humArray[maximum_readings];
public:
	double getTemperature() {
		return temperature;
	}
	double getHumidity() {
		return humidity;
	}
	void setTemperature(double newTemperature) {
		temperature = newTemperature;
	}

	void setHumidity(double newHumidity) {
		humidity = newHumidity;
	}
	system_clock::time_point StartTime;

	// Constructors
	Climate();

	// Utility
	void clearSamples();
	long sampleCount(long);
	long sampleTotal();
	// Sensor related
	long readSensor();

	// Temperature
	double getTemperature(long);
	double averageTemperature(long);
	double maximumTemperature(long);
	double minimumTemperature(long);
	// Humidity
	double getHumidity(long);
	double averageHumidity(long);
	double maximumHumidity(long);
	double minimumHumidity(long);



};

// Constructor
Climate::Climate() {

	StartTime = std::chrono::system_clock::now();

}


void	Climate::clearSamples() {
	for (int i = 0; i < maximum_readings; i++) {
		tempArray[i] = 0;
		humArray[i] = 0;
	}
}


long	Climate::readSensor() {


	sensorDevice.read_data();
	system_clock::time_point EndTime = std::chrono::system_clock::now();
	std::chrono::duration<double> Period = EndTime - StartTime;
	int currentSecond = (int)Period.count();
	// This line is purely for your debugging and can be removes/commented out in the final code.
	cout << endl << "Debugging information : " << "Temperature is " << sensorDevice.get_temperature_in_c() << " in degrees C " << sensorDevice.get_humidity() << "% humidity" << endl;	tempArray[currentSecond] = sensorDevice.get_temperature_in_c();
	//Set
	setHumidity(sensorDevice.get_humidity());
	setTemperature(sensorDevice.get_temperature_in_c());
	//Get
	humArray[currentSecond] = getHumidity();
	tempArray[currentSecond] = getTemperature();
	if (currentSecond > maximum_readings)
	{
		throw out_of_range("Over 24 hour limit");
	}
	if (currentSecond < 1)
	{
		throw underflow_error("Less than a 1 second from last sample ");
	}
	if (sensorDevice.get_temperature_in_c() == NULL || sensorDevice.get_humidity() == NULL)
	{
		throw runtime_error("A read attempt has failed");
	}
	return currentSecond;
}


long Climate::sampleCount(long secs) {

	if (secs > maximum_readings || secs < 1) {
		throw out_of_range("Out of range");
	}

	for (int n = (int)secs; n >= 1; n--) {
		if (tempArray[n] || humArray[n] != NULL) {
			samples++;
		}

	}

	return samples;
}



long Climate::sampleTotal() {
	for (auto it = begin(tempArray); it != end(tempArray); ++it) {
		totalSamples++;
	}
	return totalSamples;
}



double Climate::getTemperature(long number_of_seconds) {
	if (number_of_seconds > maximum_readings || number_of_seconds < 1) {
		throw out_of_range("Out of range");
	}
	float value = tempArray[number_of_seconds];
	if (value == NULL) {
		throw invalid_argument("No samples at specified time");
	}
	return value;
}

double Climate::averageTemperature(long number_of_seconds) {
	if (number_of_seconds > maximum_readings || number_of_seconds < 1) {
		throw out_of_range("Out of range");
	}
	double average;
	double sum = 0;
	for (int i = number_of_seconds; i >= 1; i--) {
		if (tempArray[i] == NULL) {
			throw invalid_argument("No samples at specified time");
		}
		sum += tempArray[i];
	}
	average = sum / number_of_seconds;
	return average;

}

double Climate::maximumTemperature(long number_of_seconds) {
	if (number_of_seconds > maximum_readings || number_of_seconds < 1) {
		throw out_of_range("Out of range");
	}
	double max = tempArray[number_of_seconds];
	for (int i = number_of_seconds; i >= 1; i--)
	{
		if (tempArray[i] == NULL) {
			throw invalid_argument("No samples at specified time");
		}
		if (tempArray[i] > max) {
			max = tempArray[i];
		}
	}
	return max;
}

double Climate::minimumTemperature(long number_of_seconds) {
	if (number_of_seconds > maximum_readings || number_of_seconds < 1) {
		throw out_of_range("Out of range");
	}
	double min = tempArray[number_of_seconds];
	for (int i = number_of_seconds; i >= 1; i--)
	{
		if (tempArray[i] == NULL) {
			throw invalid_argument("No samples at specified time");
		}
		if (tempArray[i] < min) {
			min = tempArray[i];
		}
	}
	return min;
}
double Climate::getHumidity(long number_of_seconds) {
	if (number_of_seconds > maximum_readings || number_of_seconds < 1) {
		throw out_of_range("Out of range");
	}

	float value = humArray[number_of_seconds];
	if (number_of_seconds == NULL) {
		throw invalid_argument("No samples at specified time");
	}
	return value;
}

double Climate::averageHumidity(long number_of_seconds) {
	double average;
	double sum = 0;
	if (number_of_seconds > maximum_readings || number_of_seconds < 1) {
		throw out_of_range("Out of range");
	}
	for (int i = number_of_seconds; i >= 1; i--) {
		if (humArray[i] == NULL) {
			throw invalid_argument("No samples at specified time");
		}
		sum += humArray[i];
	}
	average = sum / number_of_seconds;
	return average;

}

double Climate::maximumHumidity(long number_of_seconds) {
	if (number_of_seconds > maximum_readings || number_of_seconds < 1) {
		throw out_of_range("Out of range");
	}
	double max = humArray[number_of_seconds];
	for (int i = number_of_seconds; i >= 1; i--)
	{
		if (humArray[i] == NULL) {
			throw invalid_argument("No samples at specified time");
		}
		if (humArray[i] > max) {
			max = humArray[i];
		}
	}
	return max;
}

double Climate::minimumHumidity(long number_of_seconds) {
	if (number_of_seconds > maximum_readings || number_of_seconds < 1) {
		throw out_of_range("Out of range");
	}
	double min = humArray[number_of_seconds];
	for (int i = number_of_seconds; i >= 1; i--)
	{
		if (humArray[i] == NULL) {
			throw invalid_argument("No samples at specified time");
		}
		if (humArray[i] < min) {
			min = humArray[i];
		}
	}
	return min;
}






