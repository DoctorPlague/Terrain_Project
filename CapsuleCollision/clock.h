// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	Jasper Lyons
// Email		:	Jasper.Lyo7552@mediadesign.school.nz
// File Name	:	clock.h
// Description	:	Header file defining the clock class

#pragma once

#ifndef _CLOCK_H_
#define _CLOCK_H_

// Library Includes
#include <chrono>
#include <vector>
#include <memory>

// Local Includes

// Types

// Constants

// Prototypes
class CClock
{
	// Member Functions
#pragma region Singleton
public:
	static std::shared_ptr<CClock> GetInstance();
	static void DestroyInstance();
	~CClock();
private:
	static std::shared_ptr<CClock> s_pClock;
	CClock(CClock const&);
	void operator=(CClock const&);
	CClock();

#pragma endregion

public:	
	bool Initialise();
	void Process();
	float GetDeltaTick();

protected:
	double m_fTimeElapsed;
	double m_fDeltaTime;
	std::chrono::high_resolution_clock::time_point m_fLastTime;
	std::chrono::high_resolution_clock::time_point m_fCurrentTime;

	std::vector<double> m_vecTimeHistory;

	long long m_llNumCounts;

private:

};
#endif // 
