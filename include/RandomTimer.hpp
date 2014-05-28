#ifndef RANDOM_TIMER_HPP
#define RANDOM_TIMER_HPP

class RandomTimer {
	public:
		RandomTimer();
		RandomTimer(int min, int max);
		~RandomTimer();

		void SetRange(int min, int max);
		void Reset();
		bool HasExpired(int dt);

	private:
		int min;
		int max;
		int value;
};

#endif