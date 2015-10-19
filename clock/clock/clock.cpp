/**
 *автор: Демидов Александр
 *учебное заведение: МФТИ(ГУ) ФИВТ 296
 *направление: Прикладная Математика и Информатика
 *
 *отборочное задание на стажировку
 *в отделе разработки геоинформационных сервисов
 *Яндекс, Москва
 *
 *используемая среда разработки: Microsoft VS C++ 2010
 *используемые средства: stl, boost v1.55.0
 */

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <iomanip>
#include <boost/regex.hpp>
#include <boost/rational.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/math/special_functions/modf.hpp>

// требуемое количество аргументов при запуске программы
const int REQUIRED_AGRUMENTS_NUMBER = 4;

// требуемая точность типа double, учитываемая при операциях сравнения
static const double EPSILON = 1E-7;

// доступные форматы строкового представления угла
enum AngleFormat {
	ANGLE_FORMAT_RADIANS,
	ANGLE_FORMAT_DEGREES,
	ANGLE_FORMAT_DEGREES_MINUTES_SECONDS
};

// доступные типы часов
enum ClockType {
	CLOCK_TYPE_MECHANICAL,
	CLOCK_TYPE_QUARTZ
};

// используется для исключительных ситуаций, 
// связанных с неправильным пользовательским вводом
class WrongInputException: public std::runtime_error {
public:
	explicit WrongInputException(const std::string& errorMessage)
		: runtime_error(errorMessage) {

		//do nothing
	}
};

// реализует набор вспомогательных статических методов
class Utils {
private:
	Utils() {
		//do nothing
	}

public:
	static void abortProgram(const std::string& errorMessage) {
		std::cerr << errorMessage << std::endl;
		exit(EXIT_FAILURE);
	}

	static void checkArgumentsNumber(int argumentsNumber, int requiredArgumentsNumber) {
		if (argumentsNumber < requiredArgumentsNumber) {
			abortProgram("too few parameters");
		}
		if (argumentsNumber > requiredArgumentsNumber) {
			abortProgram("too many parameters");
		} 
	}

	static void checkNotNullArguments(int argc, char *argv[]) {
		for (int argumentNumber = 1; argumentNumber < argc; ++argumentNumber) {
			if (argv[argumentNumber] == NULL) {
				abortProgram("NULL argument");
			}
		}
	}

	// возвращает формат угла AngleFormat,
	// содержащийся в строке angleFormatSource,
	// бросает WrongInputException в случае ошибки
	static AngleFormat parseAngleFormat(const std::string& angleFormatSource) {
		if (angleFormatSource == "rad") {
			return ANGLE_FORMAT_RADIANS;
		}
		if (angleFormatSource == "deg") {
			return ANGLE_FORMAT_DEGREES;
		}
		if (angleFormatSource == "dms") {
			return ANGLE_FORMAT_DEGREES_MINUTES_SECONDS;
		}

		throw WrongInputException("incorrect angle format: " + angleFormatSource);
	}

	// возвращает тип часов ClockType,
	// содержащийся в строке clockTypeSource,
	// бросает WrongInputException в случае ошибки
	static ClockType parseClockType(const std::string& clockTypeSource) {
		if (clockTypeSource == "mech") {
			return CLOCK_TYPE_MECHANICAL;
		}
		if (clockTypeSource == "quar") {
			return CLOCK_TYPE_QUARTZ;
		}

		throw WrongInputException("incorrect clockt type: " + clockTypeSource);
	}
	
	static std::string doubleToString(double value, int precision) {
		std::stringstream streamForConvertion;
		streamForConvertion << std::fixed << std::setprecision(precision) << value;
		return streamForConvertion.str();
	}
};

// реализует работу с углами
class Angle {
private:
	static const int DEGREES_IN_PI = 180;
	static const int MINUTES_IN_DEGREE = 60;
	static const int SECONDS_IN_MINUTE = 60;

	// точность округления при выводе угла в радианах или градусах
	static const int ANGLE_PRECISION = 4;

	// угол в виде части числа PI
	boost::rational<int> radDividedByPi_;

	// возвращает строку в формате deg/min/sec 
	// для значений угла в градусах, минутах и секундах
	std::string getDegMinSecForm(double degValue, double minValue, double secValue) const {
		std::string serializedAngle = "";

		if (secValue + 0.5 + EPSILON >= SECONDS_IN_MINUTE) {
			secValue = 0;
			++minValue;
			if (minValue + EPSILON >= MINUTES_IN_DEGREE) {
				minValue = 0;
				++degValue;
			}
		}

		serializedAngle += Utils::doubleToString(degValue, 0) + ".";
		
		if (minValue < 10) {
			serializedAngle += "0";
		}
		serializedAngle += Utils::doubleToString(minValue, 0) + "'";

		if (secValue < 10) {
			serializedAngle += "0";
		}
		serializedAngle += Utils::doubleToString(secValue, 0) + "''";

		return serializedAngle;
	}

public:
	Angle()
		: radDividedByPi_(0) {

		//do nothing
	}

	explicit Angle(boost::rational<int> radDividedByPi)
		: radDividedByPi_(radDividedByPi) {
	}

	Angle &operator=(const Angle& otherAngle) {
		if (this != &otherAngle) {
			radDividedByPi_ = otherAngle.radDividedByPi_;
        }
        return *this;
    }

	Angle(const Angle& otherAngle) {
		*this = otherAngle;
	}

	virtual ~Angle() {
		//do nothing
	}

	void setAngle(boost::rational<int> radiansDividedByPi) {
		radDividedByPi_ = radiansDividedByPi;
	}

	double getDegreesValue() const {
		return boost::rational_cast<double>(radDividedByPi_) * DEGREES_IN_PI;
	}

	double getRadiansValue() const {
		return boost::rational_cast<double>(radDividedByPi_) * boost::math::double_constants::pi;
	}

	//возвращает строковое представление угла в формате angleFormat
	std::string serializeAngle(const AngleFormat& angleFormat) const {
		if (angleFormat == ANGLE_FORMAT_RADIANS) {
			return Utils::doubleToString(getRadiansValue(), ANGLE_PRECISION);
		}

		if (angleFormat == ANGLE_FORMAT_DEGREES) {
			return Utils::doubleToString(getDegreesValue(), ANGLE_PRECISION);
		}

		double degreesAngleValue = getDegreesValue();

		double intDegreesValue, fractDegreesValue;
		fractDegreesValue = boost::math::modf<double>(degreesAngleValue , &intDegreesValue);

		double intMinutesValue, fractMinutesValue;
		fractMinutesValue = boost::math::modf<double>(fractDegreesValue * MINUTES_IN_DEGREE, &intMinutesValue);

		return getDegMinSecForm(intDegreesValue, intMinutesValue, fractMinutesValue * SECONDS_IN_MINUTE);
	}
};

// реализует работу со временем
class Time {
private:
	int hour_;
	int minute_;

	// устанавливает значения hour_, minute_ 
	// в соответствии со строковым представлением времени serializedTime
	// бросает WrongInputException в случае ошибки 
	void parseTime(const std::string& serializedTime) {
		boost::regex timeFormat12RegExpr("((1[0-1])|(0[0-9])):[0-5][0-9] ((AM)|(PM))");
		if (boost::regex_match(serializedTime, timeFormat12RegExpr)) {
			hour_ = atoi(serializedTime.substr(0, 2).c_str());
			minute_ = atoi(serializedTime.substr(3, 2).c_str());
			if (serializedTime.substr(6, 2) == "PM") {
				hour_ += 12;
			}

			return;
		}

		boost::regex timeFormat24RegExpr("((2[0-3])|([0-1][0-9])):[0-5][0-9]");
		if (boost::regex_match(serializedTime, timeFormat24RegExpr)) {
			hour_ = atoi(serializedTime.substr(0, 2).c_str());
			minute_ = atoi(serializedTime.substr(3, 2).c_str());

			return;
		}

		throw WrongInputException("incorrect time format: " + serializedTime);
	}
	
public:
	Time()
		: minute_(0),
		hour_(0) {

		//do nothing
	}

	explicit Time(const std::string& serializedTime) {
		parseTime(serializedTime);
	}

	Time &operator=(const Time& otherTime) {
		if (this != &otherTime) {
			hour_ = otherTime.hour_;
			minute_ = otherTime.minute_;
        }
        return *this;
    }

	Time(const Time& otherTime) {
		*this = otherTime;
	}

	virtual ~Time() {
		//do nothing
	}

	void setTime(const std::string& serializedTime) {
		parseTime(serializedTime);
	}

	void setTime(int hour, int minute) {
		hour_ = hour;
		minute_ = minute;
	}

	int getHour() const {
		return hour_;
	}

	int getMinute() const {
		return minute_;
	}
};

// реализует абстрактный класс часов
class Clock {
private:
	//количество различных возможных положений стрелки часов
	static const int HAND_POSITIONS_NUMBER = 720;

protected:
	static const int MINUTES_IN_HOUR = 60;
	static const int HOURS_ON_DIAL = 12;

	Time currentTime_;

	// возвращает угол между стрелками часов
	// в положениях hourHandPosition, minuteHandPosition
	Angle countAngle(int hourHandPosition, int minuteHandPosition) const {
		int positionsBetweenHands = std::abs(hourHandPosition - minuteHandPosition);
		if (positionsBetweenHands * 2 > HAND_POSITIONS_NUMBER) {
			positionsBetweenHands = HAND_POSITIONS_NUMBER - positionsBetweenHands;
		}

		return Angle(boost::rational<int>(positionsBetweenHands * 2, HAND_POSITIONS_NUMBER));
	}

public:
	Clock()
		: currentTime_() {

		//do nothing
	}

	explicit Clock(const Time& currentTime)
		: currentTime_(currentTime)	{

		//do nothing
	}

	Clock &operator=(const Clock& otherClock) {
		if (this != &otherClock) {
			currentTime_ = otherClock.currentTime_;
        }
        return *this;
    }

	Clock(const Clock& otherClock) {
		*this = otherClock;
	}

	virtual ~Clock() {
		//do nothing
	}

	void setTime(const Time& currentTime) {
		currentTime_ = currentTime;
	}

	Time getTime() const {
		return currentTime_;
	}

	// возвращает значение угла между стрелками
	virtual Angle getCurrentAngle() const = 0;
};

// реализует класс механических часов,
// наследующий абстрактный класс Clock
class MechanicalClock: public Clock {
public:
	MechanicalClock()
		: Clock() {

		//do nothing
	}

	explicit MechanicalClock(const Time& currentTime)
		: Clock(currentTime) {

		//do nothing
	}

	MechanicalClock(const Clock& otherClock) 
		: Clock(otherClock) {

		//do nothing
	}

	virtual ~MechanicalClock() {
		//do nothing
	}

	// возвращает значение угла между стрелками,
	// положение часовой стрелки зависит от положения минутной
	virtual Angle getCurrentAngle() const {
		int hourHandPosition = (currentTime_.getHour() % HOURS_ON_DIAL) * MINUTES_IN_HOUR + currentTime_.getMinute();
		int minuteHandPosition = currentTime_.getMinute() * HOURS_ON_DIAL;
		return countAngle(hourHandPosition, minuteHandPosition);
	}
};

// реализует класс кварцевых часов,
// наследующий абстрактный класс Clock
class QuartzClock: public Clock {
public:
	QuartzClock()
		: Clock() {

		//do nothing
	}

	explicit QuartzClock(const Time& currentTime)
		: Clock(currentTime) {

		//do nothing
	}

	QuartzClock(const Clock& otherClock) 
		: Clock(otherClock) {

		//do nothing
	}

	virtual ~QuartzClock() {
		//do nothing
	}

	// возвращает значение угла между стрелками,
	// положение часовой стрелки не зависит от положения минутной
	virtual Angle getCurrentAngle() const {
		int hourHandPosition = (currentTime_.getHour() % HOURS_ON_DIAL) * MINUTES_IN_HOUR;
		int minuteHandPosition = currentTime_.getMinute() * HOURS_ON_DIAL;
		return countAngle(hourHandPosition, minuteHandPosition);
	}
};

// реализует паттерн фабрика
// для создания часов требуемого типа
class ClockFactory {
public:
	ClockFactory() {
		//do nothing
	}

	virtual	~ClockFactory() {
		//do nothing
	}

	// возвращает указатель на созданные часы
	// требуемого типа clockType
	// с установленным значением времени currentTime
	boost::shared_ptr<Clock> createClock(const ClockType& clockType, const Time& currentTime) {
		if (clockType == CLOCK_TYPE_MECHANICAL) {
			return boost::make_shared<MechanicalClock>(currentTime); 
		} else {
			return boost::make_shared<QuartzClock>(currentTime); 
		}
	}
};

// возвращает указатель на созданные часы,
// соответсвующие входным параметрам argv
boost::shared_ptr<Clock> initializeClock(int argc, char* argv[]) { 
	Time currentTime;
	ClockType clockType;

	try {
		currentTime.setTime(std::string(argv[1]));
		clockType = Utils::parseClockType(std::string(argv[3]));
	} catch (WrongInputException catchedException) {
		Utils::abortProgram(catchedException.what());
	}

	ClockFactory clockFactory;
	return clockFactory.createClock(clockType, currentTime);
}

// печатает в поток outputStream
// угол между стрелками часов clock
// в требуемом формате, содержащемся в строке angleFormatSource
void printAngle(std::ostream& outputStream, const Clock& clock, const std::string& angleFormatSource) {
	Angle angle = clock.getCurrentAngle();

	AngleFormat angleFormat;
	try {
		angleFormat = Utils::parseAngleFormat(angleFormatSource);
	} catch (WrongInputException catchedException) {
		Utils::abortProgram(catchedException.what());
	}

	outputStream << angle.serializeAngle(angleFormat) << std::endl;
}

int main(int argc, char *argv[]) {
	Utils::checkArgumentsNumber(argc, REQUIRED_AGRUMENTS_NUMBER);
	Utils::checkNotNullArguments(argc, argv);

	boost::shared_ptr<Clock> clock = initializeClock(argc, argv);

	printAngle(std::cout, *clock, std::string(argv[2]));
	return 0;
}