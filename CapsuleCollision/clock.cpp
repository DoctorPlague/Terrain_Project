// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	Jasper Lyons
// Email		:	Jasper.Lyo7552@mediadesign.school.nz
// File Name	:	clock.cpp
// Description	:	Implimentation for the clock class

// Library Includes
//#include "stdafx.h"
#include <chrono>


// Local Includes

// This Includes
#include "clock.h"

// Static Variables

// Static Function Prototypes

// Implementation

std::shared_ptr<CClock> CClock::s_pClock;

std::shared_ptr<CClock> CClock::GetInstance()
{
	if (!s_pClock)
	{
		s_pClock = std::shared_ptr<CClock>(new CClock());
	}
	return s_pClock;
}

void CClock::DestroyInstance()
{
	s_pClock = nullptr;
}

//****************************************************
// ~CClock: CClock Class Destructor
// @author: 
// @parameter: No parameters
//
// @return: none
//*****************************************************
CClock::~CClock()
{

}


//****************************************************
// CClock: CClock Class Constructor
// @author: 
// @parameter: No parameters
//
// @return: none
//*****************************************************
CClock::CClock() 
	: m_fTimeElapsed(0.0f)
	, m_fDeltaTime(0.0f)
{
	
}

//****************************************************
// Initialise: CClock Class Initialiser - sets the first time values
// @author: 
// @parameter: No parameters
//
// @return: true if initialisation is successful, false if not
//*****************************************************
bool CClock::Initialise()
{
	m_fCurrentTime = std::chrono::high_resolution_clock::now();
	return (true);
}

//****************************************************
// Process: processes the change in time since it was last called
// @author:
// @parameter: No parameters
//
// @return: void
//*****************************************************
void
CClock::Process()
{
	m_fLastTime = m_fCurrentTime;

	m_fCurrentTime = std::chrono::high_resolution_clock::now();

	
	m_fDeltaTime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_fCurrentTime - m_fLastTime).count());

	m_fTimeElapsed += m_fDeltaTime;
}

//****************************************************
// GetDeltaTick: gets the current delta tick value
// @author: 
// @parameter: No parameters
//
// @return: the current delta tick value
//*****************************************************
float CClock::GetDeltaTick()
{
	return (static_cast<float>(m_fDeltaTime));
}