#pragma once

#include <chrono>
#include <string>
#include <fstream>


using DurationType = std::chrono::milliseconds;


struct InstrumentorResult
{
	std::string Name;
	DurationType Duration;
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

	void WriteResult(const InstrumentorResult& result);

private:
	Instrumentor() = default;

private:
	std::string mSessionName;
	bool mSessionOpen = false;

	std::ofstream mOutputStream;

public:
	static Instrumentor* Get();
};


class InstrumentorTimer
{
public:
	InstrumentorTimer(const std::string& name);
	~InstrumentorTimer();

private:

	void Stop();

private:
	std::string mName;
	std::chrono::steady_clock::time_point mStartTime;
	bool mStopped = false;
};


// Instrumentor macros
#define ENABLE_PROFILE 0
#if ENABLE_PROFILE

	#define BEGIN_PROFILE_SESSION(name, filepath, append) Instrumentor::Get()->BeginSession(name, filepath, append);
	#define END_PROFILE_SESSION() Instrumentor::Get()->EndSession();
	#define PROFILE_SCOPE_LINE(name, line) InstrumentorTimer timer##line(name);
	#define PROFILE_SCOPE(name)	PROFILE_SCOPE_LINE(name, __LINE__);

#else

	#define BEGIN_PROFILE_SESSION(name, filepath, append)
	#define END_PROFILE_SESSION()
	#define PROFILE_SCOPE_LINE(name, line)
	#define PROFILE_SCOPE(name)

#endif