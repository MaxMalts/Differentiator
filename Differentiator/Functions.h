#pragma once

DEF_FUNC(sin, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = sin(arg);
	})

DEF_FUNC(cos, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = cos(arg);
	})

DEF_FUNC(tg, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = tan(arg);
	})

DEF_FUNC(ctg, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = 1 / tan(arg);
	})

DEF_FUNC(arcsin, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = asin(arg);
	})

DEF_FUNC(arccos, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = acos(arg);
	})


DEF_FUNC(arctg, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = atan(arg);
	})


DEF_FUNC(arcctg, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = atan(1 / arg);
	})

DEF_FUNC(exp, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = exp(arg);
	})

DEF_FUNC(abs, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = fabs(arg);
	})

DEF_FUNC(floor, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = floor(arg);
	})

DEF_FUNC(sqrt, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = floor(arg);
	})

DEF_FUNC(ln, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = log(arg);
	})

DEF_FUNC(lg, 1, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = log10(arg);
	})

DEF_FUNC(log, 2, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg1 = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ',') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg2 = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = log(arg2) / log(arg1);
	})

DEF_FUNC(pow, 2, {
		if (*curSequence != '(') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg1 = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ',') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		float arg2 = GetE(syntaxErr);
		assert_syntax(*syntaxErr);

		if (*curSequence != ')') {
			*syntaxErr = 1;
		}
		assert_syntax(*syntaxErr);
		curSequence++;

		val = pow(arg1, arg2);
	})