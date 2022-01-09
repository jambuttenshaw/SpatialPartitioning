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
	for (const auto& [name, value] : mAverages)
	{
		double averageValue = value.first / static_cast<double>(value.second);
		WriteData(name, averageValue);
	}

	// write footer
	
	// spacing for readability
	WriteLabel("\n\n");

	// Close file
	mOutputStream.close();
	mSessionOpen = false;

	// reset data
	mAverages.clear();
}

void Instrumentor::WriteLabel(const std::string& label)
{
	if (!mSessionOpen)
	{
		std::cout << "Error: No session active" << std::endl;
		return;
	}

	// Write result in csv format
	std::stringstream resultString;
	resultString << label << "\n";

	mOutputStream << resultString.str();
	mOutputStream.flush();
}

void Instrumentor::TrackAverageValue(const std::string& name, double value)
{
	// is this the first time this value has been tracked?
	if (mAverages.find(name) == mAverages.end())
	{
		// it is, we need to make a new entry for it in the averages map
		auto data = std::make_pair(value, size_t(1));
		mAverages.insert(std::make_pair(name, data));
	}
	else
	{
		// we have seen this name before
		// we can increment the value and the occurrences
		auto& [totalTime, occurrences] = mAverages[name];
		totalTime += value;
		occurrences++;
	}
}

Instrumentor* Instrumentor::Get()
{
	// instrumentor will be constructed the first time Get() is called
	static Instrumentor instance;
	return &instance;
}




InstrumentorTimer::InstrumentorTimer(const std::string& name, bool average)
	: mName(name), mAveraging(average)
{
	//std::cout << mName << " started timing." << std::endl;
	mStartTime = std::chrono::high_resolution_clock::now();
}

InstrumentorTimer::~InstrumentorTimer()
{
	Stop();
	//std::cout << mName << " stopped timing." << std::endl;
}

void InstrumentorTimer::Stop()
{
	auto endTime = std::chrono::high_resolution_clock::now();

	// get time elapsed in milliseconds
	double elapsedDuration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(endTime - mStartTime).count()) / 1000.0;

	// are we directly storing this result, or storing the average time taken by timers of this name?
	if (mAveraging)
	{
		Instrumentor::Get()->TrackAverageValue(mName, elapsedDuration);
	}
	else
	{
		// store result in instrumentor session
		Instrumentor::Get()->WriteData(mName, elapsedDuration);
	}
}


