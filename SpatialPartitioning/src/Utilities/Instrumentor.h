#pragma once

#include <chrono>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unordered_map>


struct InstrumentorResult
{
	std::string Name;
	// in milliseconds
	double Duration;
};


class Instrumentor
{
public:
	// instrumentor is a singleton, do not allow it to be copied or moved
	Instrumentor(const Instrumentor&) = delete;
	Instrumentor(Instrumentor&&) = delete;

	~Instrumentor();

	void BeginSession(const std::string& name, const std::string& filepath, bool append);
	void EndSession();

	template<typename T>
	void WriteData(std::string label, T data)
	{
		if (!mSessionOpen)
		{
			std::cout << "Error: No session active" << std::endl;
			return;
		}

		// Write result in csv format
		std::stringstream resultString;
		resultString << std::setprecision(3) << std::fixed;
		resultString << label << "," << data << "\n";

		mOutputStream << resultString.str();
		mOutputStream.flush();
	}
	void WriteLabel(const std::string& label);

	void TrackAverageValue(const std::string& name, double value);

private:
	Instrumentor() = default;

private:
	std::string mSessionName;
	bool mSessionOpen = false;

	std::ofstream mOutputStream;

	std::unordered_map<std::string, std::pair<double, size_t>> mAverages;

public:
	static Instrumentor* Get();
};


class InstrumentorTimer
{
public:
	InstrumentorTimer(const std::string& name, bool average);
	~InstrumentorTimer();

private:

	void Stop();

private:
	std::string mName;
	bool mAveraging = false;

	std::chrono::steady_clock::time_point mStartTime;
	bool mStopped = false;
};


// Instrumentor macros
#define ENABLE_INSTRUMENTOR 1
#if ENABLE_INSTRUMENTOR

	#define BEGIN_PROFILE_SESSION(name, filepath, append)	Instrumentor::Get()->BeginSession(name, filepath, append);
	#define END_PROFILE_SESSION()							Instrumentor::Get()->EndSession();

	#define PROFILE_SCOPE_LINE(name, line)					InstrumentorTimer timer##line(name, false);
	#define PROFILE_SCOPE(name)								PROFILE_SCOPE_LINE(name, __LINE__)

	#define PROFILE_SCOPE_AVERAGE_LINE(name, line)			InstrumentorTimer timer##line(name, true);
	#define PROFILE_SCOPE_AVERAGE(name)						PROFILE_SCOPE_AVERAGE_LINE(name, __LINE__)

	#define TRACK_AVERAGE(value)							Instrumentor::Get()->TrackAverageValue(#value, static_cast<double>(value));

#else

	#define BEGIN_PROFILE_SESSION(name, filepath, append)
	#define END_PROFILE_SESSION()

	#define PROFILE_SCOPE_LINE(name, line)
	#define PROFILE_SCOPE(name)

	#define PROFILE_SCOPE_AVERAGE_LINE(name, line)
	#define PROFILE_SCOPE_AVERAGE(name)				

	#define TRACK_AVERAGE(value)

#endif