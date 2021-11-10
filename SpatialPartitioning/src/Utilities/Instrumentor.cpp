#include "Instrumentor.h"

#include <iostream>
#include <sstream>
#include <iomanip>


Instrumentor::~Instrumentor()
{
	if (mSessionOpen) EndSession();
}

void Instrumentor::BeginSession(const std::string& name, const std::string& filepath, bool append)
{
	if (mSessionOpen)
		EndSession();

	// Open output file
	mOutputStream.open(filepath, std::ofstream::out | (append ? std::ofstream::app : 0));
	if (!mOutputStream.good())
	{
		std::cout << "Error: Failed to open output file: " << filepath << std::endl;
		return;
	}

	// Write header
	mSessionName = name;
	mOutputStream << "Session: " << mSessionName << "\n";
	mOutputStream.flush();

	mSessionOpen = true;
}

void Instrumentor::EndSession()
{
	if (!mSessionOpen) return;

	// Close file
	mOutputStream.close();
	mSessionOpen = false;
}

void Instrumentor::WriteResult(const InstrumentorResult& result)
{
	if (!mSessionOpen)
	{
		std::cout << "Error: No session active" << std::endl;
		return;
	}

	// Write result in csv format
	std::stringstream resultString;
	resultString << std::setprecision(3) << std::fixed;
	resultString << result.Name << "," << result.Duration.count() << "\n";
	
	mOutputStream << resultString.str();
	mOutputStream.flush();
}

Instrumentor* Instrumentor::Get()
{
	// instrumentor will be constructed the first time Get() is called
	static Instrumentor instance;
	return &instance;
}




InstrumentorTimer::InstrumentorTimer(const std::string& name)
	: mName(name)
{
	mStartTime = std::chrono::steady_clock::now();
}

InstrumentorTimer::~InstrumentorTimer()
{
	if (!mStopped)
		Stop();
}

void InstrumentorTimer::Stop()
{
	auto endTime = std::chrono::steady_clock::now();

	// get time elapsed
	auto elapsedDuration = std::chrono::duration_cast<DurationType>(endTime - mStartTime);

	// store result in instrumentor session
	Instrumentor::Get()->WriteResult({ mName, elapsedDuration });
}


