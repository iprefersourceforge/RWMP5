#include "UniqueIDGenerator.h"

bool UniqueIDGenerator::m_initialised = false;
UniqueIDGenerator * UniqueIDGenerator::m_instance = 0;
long UniqueIDGenerator::m_currentID = 0;

UniqueIDGenerator::~UniqueIDGenerator()
{
	m_initialised = false;
}

UniqueIDGenerator * UniqueIDGenerator::getInstance()
{
	if(!m_initialised)
	{
		m_instance = new UniqueIDGenerator();
		m_currentID = 0;
		m_initialised = true;
	}
	
	return m_instance;
}

long UniqueIDGenerator::getNewID()
{
	long newID = m_currentID++;
	return newID;
}