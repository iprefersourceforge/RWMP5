#ifndef TIMER_H
#define TIMER_H

#include <conio.h>
#include <time.h>	// class needs this inclusion

class timer {
	public:
		timer();
		void           start();
		void           stop();
		void           reset();
		bool           isRunning();
		unsigned long  getTime();
		unsigned long  getPreciseTime();
		bool           isOver(unsigned long seconds);
	private:
		bool           resetted;
		bool           running;
		unsigned long  beg;
		unsigned long  end;
};

#endif