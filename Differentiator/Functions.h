DEF_FUNC(sin, 1, sin_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = COS(newNode, CLONE(curNode->left));

	/*assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	node_t* leftOld = curNode->left;

	value_t value = { cos_funcI };
	node_t* leftNew = CreateNodeProp(curNode, func_node, value, leftOld, NULL);
	node_t* rightNew = CloneNodes(leftOld);
	rightNew->parent = curNode;

	curNode->type = op_node;
	curNode->value[0] = '*';
	curNode->left = leftNew;
	curNode->right = rightNew;
	
	DifferentiateNode(curNode->right);*/

		/*if (*curSequence != '(') {
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

		val = sin(arg);*/
	})

DEF_FUNC(cos, 1, cos_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = MUL(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, -1);
			newNode->left->right = COS(newNode->left, CLONE(curNode->left));

	/*assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	node_t* leftOld = curNode->left;

	value_t value = { '-' };
	node_t* leftNew = CreateNodeProp(curNode, op_node, value, leftOld, NULL);

	value[0] = sin_funcI;
	node_t* leftNew = CreateNodeProp(curNode, func_node, value, leftOld, NULL);
	node_t* rightNew = CloneNodes(leftOld);
	rightNew->parent = curNode;

	curNode->type = op_node;
	curNode->value[0] = '*';
	curNode->left = leftNew;
	curNode->right = rightNew;
	
	DifferentiateNode(curNode->right);*/

		/*if (*curSequence != '(') {
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

		val = cos(arg);*/
	})

DEF_FUNC(tg, 1, tg_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 1);
			newNode->left->right = MUL(newNode->left, NULL, NULL);
				newNode->left->right->left = COS(newNode->left->right, CLONE(curNode->left));
				newNode->left->right->right = COS(newNode->left->right, CLONE(curNode->left));


//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = tan(arg);
	})

DEF_FUNC(ctg, 1, ctg_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, -1);
			newNode->left->right = MUL(newNode->left, NULL, NULL);
				newNode->left->right->left = SIN(newNode->left->right, CLONE(curNode->left));
				newNode->left->right->right = SIN(newNode->left->right, CLONE(curNode->left));


//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = 1 / tan(arg);
	})

DEF_FUNC(arcsin, 1, arcsin_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 1);
			newNode->left->right = SQRT(newNode->left, NULL);
				newNode->left->right->left = MINUS(newNode->left->right, NULL, NULL);
					newNode->left->right->left->left = NUM(newNode->left->right->left, 1);
					newNode->left->right->left->right = MUL(newNode->left->right->left, CLONE(curNode->left), CLONE(curNode->left));


//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = asin(arg);
	})

DEF_FUNC(arccos, 1, arccos_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, -1);
			newNode->left->right = SQRT(newNode->left, NULL);
				newNode->left->right->left = MINUS(newNode->left->right, NULL, NULL);
					newNode->left->right->left->left = NUM(newNode->left->right->left, 1);
					newNode->left->right->left->right = MUL(newNode->left->right->left, CLONE(curNode->left), CLONE(curNode->left));

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = acos(arg);
	})

DEF_FUNC(arctg, 1, arctg_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 1);
			newNode->left->right = PLUS(newNode->left, NULL, NULL);
				newNode->left->right->left = NUM(newNode->left->right, 1);
				newNode->left->right->right = MUL(newNode->left->right, CLONE(curNode->left), CLONE(curNode->left));

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = atan(arg);
	})

DEF_FUNC(arcctg, 1, arcctg_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, -1);
			newNode->left->right = PLUS(newNode->left, NULL, NULL);
				newNode->left->right->left = NUM(newNode->left->right, 1);
				newNode->left->right->right = MUL(newNode->left->right, CLONE(curNode->left), CLONE(curNode->left));

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = atan(1 / arg);
	})

DEF_FUNC(exp, 1, exp_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = EXP(newNode, CLONE(curNode->left));

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = exp(arg);
	})

DEF_FUNC(abs, 1, abs_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, CLONE(curNode->left), NULL);
			newNode->left->right = ABS(newNode->left, CLONE(curNode->left));

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = fabs(arg);
	})

DEF_FUNC(floor, 1, floor_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 0);
			newNode->left->right = MINUS(newNode->left, CLONE(curNode->left), NULL);
				newNode->left->right->right = FLOOR(newNode->left->right, CLONE(curNode->left));

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = floor(arg);
	})

DEF_FUNC(sqrt, 1, sqrt_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 1);
			newNode->left->right = MUL(newNode->left, NULL, CLONE(curNode));
				newNode->left->right->left = NUM(newNode->left->right, 2);

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = floor(arg);
	})

DEF_FUNC(ln, 1, ln_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, CLONE(curNode->left));
			newNode->left->left = NUM(newNode->left, 1);


//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = log(arg);
	})

DEF_FUNC(lg, 1, lg_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 1);
			newNode->left->right = MUL(newNode->left, CLONE(curNode->left), NULL);
				newNode->left->right->right = LN(newNode->left->right, NULL);
					newNode->left->right->right->left = NUM(newNode->left->right->right, 10);

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = log10(arg);
	})

DEF_FUNC(log, 2, log_funcI, {

	assert(curNode->right != NULL);
	assert(curNode->left != NULL);

	node_t* lnRepres = DIV(curNode->parent, NULL, NULL);
		lnRepres->left = LN(lnRepres, CLONE(curNode->right));
		lnRepres->right = LN(lnRepres, CLONE(curNode->left));

	newNode = DIFF(lnRepres);

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg1 = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ',') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg2 = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = log(arg2) / log(arg1);
	})

DEF_FUNC(pow, 2, pow_funcI, {

	assert(curNode->right != NULL);
	assert(curNode->left != NULL);

	node_t* expRepres = EXP(curNode->parent, NULL);
		expRepres->left = MUL(expRepres, CLONE(curNode->right), NULL);
			expRepres->left->right = LN(expRepres->left, CLONE(curNode->left));

	newNode = DIFF(expRepres);

//		if (*curSequence != '(') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg1 = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ',') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		float arg2 = GetE(syntaxErr);
//		assert_syntax(*syntaxErr);
//
//		if (*curSequence != ')') {
//			*syntaxErr = 1;
//		}
//		assert_syntax(*syntaxErr);
//		curSequence++;
//
//		val = pow(arg1, arg2);
	})