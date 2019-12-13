DEF_FUNC(sin, 1, sin_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = COS(newNode, CLONE(curNode->left));

	}, {

	STANDARD_FUNC_OUTPUT

	}, {})

DEF_FUNC(cos, 1, cos_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = MUL(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, -1);
			newNode->left->right = SIN(newNode->left, CLONE(curNode->left));

	}, {

	STANDARD_FUNC_OUTPUT

	}, {})

DEF_FUNC(tg, 1, tg_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 1);
			newNode->left->right = MUL(newNode->left, NULL, NULL);
				newNode->left->right->left = COS(newNode->left->right, CLONE(curNode->left));
				newNode->left->right->right = COS(newNode->left->right, CLONE(curNode->left));

	}, {

	STANDARD_FUNC_OUTPUT

	}, {})

DEF_FUNC(ctg, 1, ctg_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, -1);
			newNode->left->right = MUL(newNode->left, NULL, NULL);
				newNode->left->right->left = SIN(newNode->left->right, CLONE(curNode->left));
				newNode->left->right->right = SIN(newNode->left->right, CLONE(curNode->left));

	}, {

	STANDARD_FUNC_OUTPUT

	}, {})

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

	}, {

	STANDARD_FUNC_OUTPUT

	}, {})

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

	}, {

	STANDARD_FUNC_OUTPUT

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

	}, {

	STANDARD_FUNC_OUTPUT

	}, {})

DEF_FUNC(arcctg, 1, arcctg_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, -1);
			newNode->left->right = PLUS(newNode->left, NULL, NULL);
				newNode->left->right->left = NUM(newNode->left->right, 1);
				newNode->left->right->right = MUL(newNode->left->right, CLONE(curNode->left), CLONE(curNode->left));

	}, {

	STANDARD_FUNC_OUTPUT

	}, {})

DEF_FUNC(exp, 1, exp_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = EXP(newNode, CLONE(curNode->left));

	}, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	fprintf(fout, "e^{");
	NodesToLatex(fout, curNode->left);
	fprintf(fout, "}");

	}, {})

DEF_FUNC(abs, 1, abs_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, CLONE(curNode->left), NULL);
			newNode->left->right = ABS(newNode->left, CLONE(curNode->left));

	}, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	fprintf(fout, "|");
	NodesToLatex(fout, curNode->left);
	fprintf(fout, "|");

	}, {
	
	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	if (curNode->left->type == num_node) {
		float num = *(float*)curNode->left->value;
		num = fabs(num);
		newNode = NUM(curNode->parent, num);
		simplified = 1;
	}

	})

DEF_FUNC(floor, 1, floor_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 0);
			newNode->left->right = MINUS(newNode->left, CLONE(curNode->left), NULL);
				newNode->left->right->right = FLOOR(newNode->left->right, CLONE(curNode->left));

	}, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	fprintf(fout, "[");
	NodesToLatex(fout, curNode->left);
	fprintf(fout, "]");

	}, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	if (curNode->left->type == num_node) {
		float num = *(float*)curNode->left->value;
		num = floor(num);
		newNode = NUM(curNode->parent, num);
		simplified = 1;
	}

	})

DEF_FUNC(sqrt, 1, sqrt_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 1);
			newNode->left->right = MUL(newNode->left, NULL, CLONE(curNode));
				newNode->left->right->left = NUM(newNode->left->right, 2);

	}, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	fprintf(fout, "\\sqrt{");
	NodesToLatex(fout, curNode->left);
	fprintf(fout, "}");

	}, {})

DEF_FUNC(ln, 1, ln_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, CLONE(curNode->left));
			newNode->left->left = NUM(newNode->left, 1);

	}, {

	STANDARD_FUNC_OUTPUT

	}, {})

DEF_FUNC(lg, 1, lg_funcI, {

	assert(curNode->right == NULL);
	assert(curNode->left != NULL);

	newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
		newNode->left = DIV(newNode, NULL, NULL);
			newNode->left->left = NUM(newNode->left, 1);
			newNode->left->right = MUL(newNode->left, CLONE(curNode->left), NULL);
				newNode->left->right->right = LN(newNode->left->right, NULL);
					newNode->left->right->right->left = NUM(newNode->left->right->right, 10);

	}, {

	STANDARD_FUNC_OUTPUT

	}, {})

DEF_FUNC(log, 2, log_funcI, {

	assert(curNode->right != NULL);
	assert(curNode->left != NULL);

	node_t* lnRepres = DIV(curNode->parent, NULL, NULL);
		lnRepres->left = LN(lnRepres, CLONE(curNode->right));
		lnRepres->right = LN(lnRepres, CLONE(curNode->left));

	newNode = DIFF(lnRepres);

	}, {

	assert(curNode->right != NULL);
	assert(curNode->left != NULL);

	fprintf(fout, "%s_{", funcS);
	NodesToLatex(fout, curNode->left);
	fprintf(fout, "}(");
	NodesToLatex(fout, curNode->right);
	fprintf(fout, ")");

	}, {})

DEF_FUNC(pow, 2, pow_funcI, {

	assert(curNode->right != NULL);
	assert(curNode->left != NULL);

	if (curNode->right->type == num_node) {
		newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->left)));
			newNode->left = MUL(newNode, CLONE(curNode->right), NULL);
				newNode->left->right = POW(newNode->left, CLONE(curNode->left), NULL);
					newNode->left->right->right = NUM(newNode->left->right, *((float*)curNode->right->value) - 1);
	}
	else if (curNode->left->type == num_node) {
		newNode = MUL(curNode->parent, NULL, DIFF(CLONE(curNode->right)));
			newNode->left = MUL(newNode, NULL, NULL);
				newNode->left->left = POW(newNode->left, CLONE(curNode->left), CLONE(curNode->right));
				newNode->left->right = LN(newNode->left, CLONE(curNode->left));
	}
	else {
		node_t* expRepres = EXP(curNode->parent, NULL);
			expRepres->left = MUL(expRepres, CLONE(curNode->right), NULL);
				expRepres->left->right = LN(expRepres->left, CLONE(curNode->left));

		newNode = DIFF(expRepres);
	}

	}, {

	assert(curNode->right != NULL);
	assert(curNode->left != NULL);

	if (curNode->left->type == num_node || curNode->left->type == var_node) {
		NodesToLatex(fout, curNode->left);
	}
	else {
		fprintf(fout, "(", funcS);
		NodesToLatex(fout, curNode->left);
		fprintf(fout, ")");
	}
	fprintf(fout, "^{");
	NodesToLatex(fout, curNode->right);
	fprintf(fout, "}");

	}, {

	assert(curNode->right != NULL);
	assert(curNode->left != NULL);

	if (curNode->left->type == num_node && curNode->right->type == num_node) {
		if (*((float*)curNode->left->value) == 1) {
			newNode = NUM(curNode->parent, 1);
			simplified = 1;
		}
	}
	else if (curNode->right->type == num_node) {
		if (*((float*)curNode->right->value) == 1) {
			newNode = CLONE(curNode->left);
			newNode->parent = curNode->parent;
			simplified = 1;
		}
	}

	})