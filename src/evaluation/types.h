//
// Created by Archishmaan Peyyety on 2/25/23.
//
#pragma once
/*
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
*/
//enum Score : int { SCORE_ZERO };
typedef int Score;
constexpr Score SCORE_ZERO = 0;

constexpr Score S(int mg, int eg) {
	return Score((int)((unsigned int)eg << 16) + mg);
}

/// Extracting the signed lower and upper 16 bits is not so trivial because
/// according to the standard a simple cast to short is implementation defined
/// and so is a right shift of a signed integer.
inline int eg_value(Score s) {
	union { uint16_t u; int16_t s; } eg = { uint16_t(unsigned(s + 0x8000) >> 16) };
	return int(eg.s);
}

inline int mg_value(Score s) {
	union { uint16_t u; int16_t s; } mg = { uint16_t(unsigned(s)) };
	return int(mg.s);
}
//#define S(mg, eg) make_score(mg, eg)
