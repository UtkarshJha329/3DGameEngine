#pragma once

#include <random>
#include <ctime>

class RandomNumberUtils {

public:

	RandomNumberUtils() {

		SetInstance();
		randomGenerator = std::default_random_engine(time(0));
		zeroToOne = std::uniform_real_distribution<double>(0.0, 1.0);

	}

	inline static RandomNumberUtils* Instance() {
		return inst;
	}

	static double RandomNumber() {
		return zeroToOne(randomGenerator);
	}


private:

	void SetInstance() {
		inst = this;
	}

private:

	inline static std::default_random_engine randomGenerator;
	inline static std::uniform_real_distribution<double> zeroToOne;

	inline static RandomNumberUtils* inst;
};