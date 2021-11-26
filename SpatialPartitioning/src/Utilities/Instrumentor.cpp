#include "Instrumentor.h"


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
		std::abort();
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

	// write all averages that the instrumentor has been keeping track of to file
	for (const auto& average : mAverages)
	{
		double averageValue = average.second.first / static_cast<double>(average.second.second);
		WriteData(average.first, averageValue);
	}

	// Close file
	mOutputStream.close();
	mSessionOpen = false;
}

void Instrumentor::WriteResult(const InstrumentorResult& result)
{
	WriteData(result.Name, result.Duration.count());
}

void Instrumentor::TrackAverageValue(const std::string& name, double value)
{
	if (mAverages.find(name) == mAverages.end())
	{
		auto data = std::make_pair(value, size_t(1));
		mAverages.insert(std::make_pair(name, data));
	}
	else
	{
		auto& data = mAverages[name];
		data.first += value;
		data.second++;
	}
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


