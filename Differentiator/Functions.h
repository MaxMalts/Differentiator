DEF_FUNC(sin, 1, sin_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = COS(newNode, CLONE(curNode->left), NULL);

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

//DEF_FUNC(tg, 1, tg_funcI, {
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
//	})
//
//DEF_FUNC(ctg, 1, ctg_funcI, {
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
//	})
//
//DEF_FUNC(arcsin, 1, arcsin_funcI, {
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
//	})
//
//DEF_FUNC(arccos, 1, arccos_funcI, {
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
//	})
//
//
//DEF_FUNC(arctg, 1, arctg_funcI, {
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
//	})
//
//
//DEF_FUNC(arcctg, 1, arcctg_funcI, {
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
//	})
//
//DEF_FUNC(exp, 1, exp_funcI, {
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
//	})
//
//DEF_FUNC(abs, 1, abs_funcI, {
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
//	})
//
//DEF_FUNC(floor, 1, floor_funcI, {
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
//	})
//
//DEF_FUNC(sqrt, 1, sqrt_funcI, {
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
//	})
//
//DEF_FUNC(ln, 1, ln_funcI, {
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
//	})
//
//DEF_FUNC(lg, 1, lg_funcI, {
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
//	})
//
//DEF_FUNC(log, 2, log_funcI, {
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
//	})
//
//DEF_FUNC(pow, 2, pow_funcI, {
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
//	})