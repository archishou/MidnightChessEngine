//
// Created by Archishmaan Peyyety on 2/25/23.
//
#pragma once

constexpr int NGAME_PHASES = 2;
enum GamePhase {
	MIDDLE,
	END
};

struct Score {
	short middle_game;
	short end_game;
	constexpr Score (short m, short e) : middle_game(m), end_game(e) {}
	constexpr Score () : middle_game(0), end_game(0) {}
	Score& operator = (const Score& s) = default;
	constexpr Score operator + (const Score& s) const {
		return {
			(short) (middle_game + s.middle_game),
			(short) (end_game + s.end_game)
		};
	}
	constexpr Score operator * (const short& s) const {
		return {
			(short) (middle_game * s),
			(short) (end_game * s)
		};
	}
	constexpr Score operator - (const Score& s) const {
		return {
			(short) (middle_game - s.middle_game),
			(short) (end_game - s.end_game)
		};
	}
	inline constexpr bool operator == (const Score& s) const {
		return middle_game == s.middle_game && end_game == s.end_game;
	}
	inline Score& operator += (const Score& s) {
		middle_game = short(middle_game + s.middle_game);
		end_game = short(end_game + s.end_game);
		return *this;
	}
	inline Score& operator -= (const Score& s) {
		middle_game = short(middle_game - s.middle_game);
		end_game = short(end_game - s.end_game);
		return *this;
	}
	inline Score& operator *= (const short& s) {
		middle_game = short(middle_game * s);
		end_game = short(end_game * s);
		return *this;
	}
	friend inline std::ostream& operator<<(std::ostream& os, const Score& s) {
		return os << "midgame: " << s.middle_game << " endgame: " << s.end_game;
	}
};

typedef Score S;
