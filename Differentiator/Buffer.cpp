#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "buffer.h"



/*  �� ��� ������������
*	������������� ��������� ������ ������ �� �������� ��������� �������
*
*	@param buf �����
*/

void RecalcLastChar(buf_t* buf) {
	assert(buf != NULL);

	if (buf->cursor > buf->lastChar) {
		buf->lastChar = buf->cursor;
	}
}


/**
*	������� ����� � ������ ������
*
*	@param[in] mode ����� (������ ���� 'r')
*	@param str ������, ������� ������ � �����
*	@param[in] strSize ������ ������ (������)
*	@param[out] err ��� ������: 1 - mode != 'r'; 0 - ��� ������ ���������
*
*	@return ��������� ����� (� ������ ������ ����� ERR_BUF).
*/

buf_t BufConstructor(const char mode, char* str, const int strSize, int* err) {

	if (mode != 'r') {
		if (err != NULL) {
			*err = 1;
		}
		buf_t errBuf = ERR_BUF;
		return errBuf;
	}
	
	buf_t buf = {};

	buf.str = str;
	buf.cursor = 0;
	buf.size = strSize;
	buf.mode = 'r';
	buf.lastChar = strSize - 1;
	
	*err = 0;
	return buf;
}


/**
*	������� ����� � ������ ������
*
*	@param[in] mode ����� (������ ���� 'w')
*	@param[out] err ��� ������: 1 - mode != 'w'; 0 - ��� ������ ���������
*
*	@return ��������� ����� (� ������ ������ ����� ERR_BUF).
*/

buf_t BufConstructor(const char mode, int* err) {

	const int begSize = 10;

	if (mode != 'w') {
		if (err != NULL) {
			*err = 1;
		}
		buf_t errBuf = ERR_BUF;
		return errBuf;
	}

	buf_t buf = {};

	buf.str = (char*)calloc(begSize, sizeof(char));
	buf.cursor = 0;
	buf.size = begSize;
	buf.mode = 'w';
	buf.lastChar = 0;

	return buf;
}


/**
*	����� ������ � �����
*
*	@param buf �����
*	@param[in] ch ������
*
*	@return 1 - ����� �� � ������ ������; 2 - �� ������� ��������� ������ ������;\
 0 - ��� ������ ���������
*/

int Bputc(buf_t* buf, const char ch) {
	assert(buf != NULL);

	if (buf->mode != 'w') {
		return 1;
	}

	if (buf->cursor >= buf->size - 1) {
		if (realloc(buf->str, buf->size * 2) == NULL) {
			return 2;
		}
	}

	buf->str[buf->cursor++] = ch;

	RecalcLastChar(buf);
	

	return 0;
}


/**
*	������ ������ �� ������
*
*	@param[in] buf �����
*
*	@return ��������� ������. ��� ���������� ����� ������, ���������� EOB.\
 ���� ��������� -2, �� ����� �� � ������ ������.
*/

char Bgetc(buf_t* buf) {
	assert(buf != NULL);

	if (buf->mode != 'r') {
		return -2;
	}

	if (buf->cursor >= buf->size) {
		return EOB;
	}
	return buf->str[buf->cursor++];
}


/**
*	����� ������ � ����� ������
*
*	@param buf
*	@param[in] str
*
*	@return 1 - ����� �� � ������ ������; 2 - �������� ��� �����������; 0 - ��� ������ ���������
*/

int Bufcat(buf_t* buf, const char* str) {
	assert(buf != NULL);
	assert(str != NULL);

	if (buf->mode != 'w') {
		return 1;
	}

	int SLen = sprintf(&buf->str[buf->cursor], "%s", str);

	if (SLen < 0) {
		return 2;
	}

	buf->cursor += SLen;
	buf->size += SLen;
	buf->str[buf->cursor] = '\0';

	return 0;
}


/**
*	��������, �������� �� ������ ����� �� ������
*
*	@param[in] ch ����������� ������
*	@param[in] chars ������ ��������
*
*	@return 0 (false) - �� ��������; 1 (true) - ��������
*/

int IsOneOfChars(const char ch, const char* chars) {
	assert(chars != NULL);

	for (int i = 0; i < strlen(chars); i++) {
		if (ch == chars[i]) {
			return 1;
		}
	}
	return 0;
}


/**
*	������ �� ������ �� ������ �� ��������� ��������
*
*	@param[out] str ������, � ������� �����������
*	@param[in] buf �����
*	@param[in] chars ������ �������� ��������
*
*	@return 1 - ������ �� ������, ������ �������� �� ��� �� �����;\
 0 - ��� ������ ���������
*/

int ReadToChar(char* str, buf_t* buf, const char* chars) {
	assert(str != NULL);
	assert(buf != NULL);
	assert(chars != NULL);

	int NChars = strlen(chars);

	int cursorRecover = buf->cursor;

	int strCursor = 0;

	char curCh = Bgetc(buf);

	while (!IsOneOfChars(curCh, chars)) {

		if (curCh == EOB) {
			return 1;
		}

		str[strCursor] = curCh;
		strCursor++;

		curCh = Bgetc(buf);
	}
	buf->cursor--;

	return 0;
}


/**
*	������������� ������ � ������ � ������������ �� ���������� offset � origin
*
*	@param buf �����
*	@param[in] offset ����� ������������ �������, ��������������� origin
*	@param[in] origin ��������� �������: BSEEK_SET - ������ ������;\
 BSEEK_CUR - ������� ������� �������; BSEEK_END - ����� ������
*
*	@return 1 - ������������ �������� origin; 2 - ������ ������� �� ������� ������;\
 0 - ��� ������ ���������
*/

int Bseek(buf_t* buf, int offset, int origin) {
	assert(buf != NULL);
	
	int newCursorPos = 0;
	switch (origin) {
	case BSEEK_SET:
		newCursorPos = offset;
		break;
	case BSEEK_CUR:
		newCursorPos = buf->cursor + offset;
		break;
	case BSEEK_END:
		newCursorPos = buf->lastChar + 1 + offset;
		break;
	default:
		return 1;
	}

	if (newCursorPos<0 || newCursorPos>buf->size) {
		return 2;
	}

	buf->cursor = newCursorPos;

	return 0;
}


/**
*	���������� ������� ��������� ������� � ������
*
*	@param[in] buf �����
*
*	@return ������� ��������� ������� � ������
*/

int Btell(buf_t* buf) {
	assert(buf != NULL);

	return buf->cursor;
}


/*
*	����������� ������ ������
*
*	@param buf �����
*	@param[in] newSize ����� ������
*
*	@return 1 - �� ������� ���������; 2 - ����� ��������� �� � ������ ������;
 0 - ��� ������ ���������
*/

int IncreaseBuf(buf_t* buf, const int newSize) {
	assert(buf != NULL);

	if (buf->mode != 'w') {
		return 2;
	}

	char* newStr = (char*)realloc(buf->str, newSize);
	if (newStr == NULL) {
		return 1;
	}
	buf->str = newStr;
	memset(&buf->str[buf->size], 0, newSize - buf->size);
	buf->size = newSize;

	return 0;
}

/**
*	������� �����. ���� �� ��� � ������ ������, �� ������ �� ������� �� �������������!
*
*	@param[in] buf �����
*
*	@return 1 - ������������ ����� �� �����; 0 - ��� ������ ���������
*/

int BufDestructor(buf_t* buf) {
	assert(buf != NULL);

	if (buf->mode == 'r') {
		buf->str = NULL;
		buf->cursor = 0;
		buf->size = 0;
		buf->mode = 0;
		buf->lastChar = 0;
	}
	else if (buf->mode == 'w') {
		free(buf->str);
		buf->cursor = 0;
		buf->size = 0;
		buf->mode = 0;
		buf->lastChar = 0;
	}
	else {
		return 1;
	}

	return 0;
}