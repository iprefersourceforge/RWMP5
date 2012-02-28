#ifndef UNIQUEIDGENERATOR_H
#define UNIQUEIDGENERATOR_H

class UniqueIDGenerator
{
private:
	static bool m_initialised;
	static UniqueIDGenerator * m_instance;
	static long m_currentID;
	UniqueIDGenerator() {}
public:
	static UniqueIDGenerator * getInstance();
	~UniqueIDGenerator();
	long getNewID();
};
#endif