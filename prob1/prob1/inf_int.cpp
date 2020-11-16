#include "inf_int.h"

/*
Originally written by
��ǻ�Ͱ��к�
������
*/

inf_int::inf_int()
{
	this->digits = new char[2];	// �����Ҵ�

	this->digits[0] = '0';		// default �� 0 ����
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[100];

	if (n < 0) {		// ���� ó��
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n > 0) {			// ���ڸ� ���ڿ��� ��ȯ�ϴ� ����
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// ������ ������ 0�� ���
		new (this) inf_int();	// ������ ��ȣ��...gcc���� �����Ͽ����� �ִٰ� ��. inf_int()�� ��� ������ �ν��Ͻ��� ������. 
	}
	else {
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const char* str)
{
	// to be filled 
	int temp_length = strlen(str);


	// ��ȣ ó��
	if (str[0] == '-') {
		this->thesign = false;
	}
	else {
		this->thesign = true;
	}

	// "100"�� ���Դٸ� ���� ǥ���� �°� "001"�� ��ȯ
	// ex) "-1053" -> thesign=false, digits="3501", len=4
	this->digits = new char[temp_length + 1];

	int i = 0;
	if (this->thesign == false) {
		temp_length--; //���� ��ȣ�� ��ġ ����

		while (i < temp_length) {
			this->digits[i] = str[temp_length - i];

			i++;
		}
	}
	else {
		while (i < temp_length) {
			this->digits[i] = str[temp_length - (i + 1)];

			i++;
		}
	}

	this->digits[temp_length] = '\0';
	this->length = temp_length;

	if (this->length == 1 && digits[0] == '0') {
		new (this) inf_int();
	}
}

inf_int::inf_int(const inf_int& a) {
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}

inf_int::~inf_int() {
	delete digits;		// �޸� �Ҵ� ����
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// �̹� ���ڿ��� ���� ��� ����.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	// we assume 0 is always positive.
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// ��ȣ�� ����, ������ ��ġ�ؾ���.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	// to be filled
	// ���밪 ��
	if ((a.thesign == b.thesign) && (a.thesign == true)) { // �� �� ����� ��� ���� ���� ���� �״�� return
		if (strcmp(a.digits, b.digits) > 0) {
			return true;
		}
		else {
			return false;
		}
	}
	else if ((a.thesign == b.thesign) && (a.thesign == false)) { // �� �� ������ ��� ���� ���� ���� �����Ͽ� return
		if (strcmp(a.digits, b.digits) > 0) {
			return false;
		}
		else {
			return true;
		}
	}
	else { // ��ȣ�� �ٸ� ���, a�� ����� ��� b�� ����, a�� ������ ��� b�� ����̱⿡ a�� ��ȣ�������� ��ȯ�ϸ� ��
		if (a.thesign == true && b.thesign == false) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {	// ������ ��ȣ�� ���� ��� + �����ڷ� ����
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		c.CheckZero();

		return c;
	}
	else {	// ������ ��ȣ�� �ٸ� ��� - �����ڷ� ����
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
	// to be filled
	inf_int c;
	unsigned int i;

	if (a.thesign == true && b.thesign == false) { //a: +, b: -
		c = b;
		c.thesign = true;

		return c = a + c;
	}
	else if (a.thesign == false && b.thesign == true) { //a: -, b: +
		c = b;
		c.thesign = false;

		return c = a + c;
	}
	else { // ������ ��ȣ�� ���� ���
		if (a.length > b.length) {
			if (a.thesign == true) { //a: +, b: +
				//a - b, thesign: true
				c = a;

				for (i = 0; i < b.length; i++) {
					c.Subtract(b.digits[i], i + 1);
				}

				return c;
			}
			else { //a: -, b: -
				//b - a, thesign: false
				c = b;

				for (i = 0; i < a.length; i++) {
					c.Subtract(a.digits[i], i + 1);
				}

				c.CheckZero();
				return c;
			}
		}
		else if (a.length < b.length) {
			if (a.thesign == true) { //a: +, b: +
				//b - a, thesign: false
				c = b;

				for (i = 0; i < a.length; i++) {
					c.Subtract(a.digits[i], i + 1);
				}

				c.thesign = false;

				c.CheckZero();
				return c;
			}
			else { //a: -, b: -
				//a - b, thesign: true
				c = b;

				for (i = 0; i < a.length; i++) {
					c.Subtract(a.digits[i], i + 1);
				}

				c.thesign = true;
				return c;
			}
		}
		else { //���̰� ���� ���
			if (strcmp(a.digits, b.digits) >= 0) { //���밪 a�� ���밪 b���� �� ũ�ų� ������, a: +, b: +, thesign: true / a: -, b: -, thesign: false
				c = a;

				for (i = 0; i < b.length; i++) {
					c.Subtract(b.digits[i], i + 1);
				}

				c.CheckZero();
				return c;
			}
			else { //���밪 a�� ���밪 b���� ���� ��, a: +, b: +, thesign: false / a: -, b: -, thesign: true
				c = b;

				for (i = 0; i < a.length; i++) {
					c.Subtract(a.digits[i], i + 1);
				}

				c.thesign = !c.thesign;
				c.CheckZero();
				return c;
			}
		}
	}
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	// to be filled
	inf_int c;
	unsigned int i;
	unsigned int j;
	char temp_num_a;
	char temp_num_b;
	int temp_num_mul;

	//a, b�� 0�� �ִ� ���
	if ((a.length == 1 && a.digits[0] == '0') || (b.length == 1 && b.digits[0] == '0')) {
		return c;
	}

	for (i = 0; i < a.length; i++) {
		for (j = 0; j < b.length; j++) {
			temp_num_a = a.digits[i];
			temp_num_b = b.digits[j];

			temp_num_mul = (temp_num_a - '0') * (temp_num_b - '0');

			c.Add(temp_num_mul % 10 + '0', (i + j) + 1);
			temp_num_mul /= 10;
			if (temp_num_mul != 0) {
				c.Add(temp_num_mul % 10 + '0', (i + j) + 2);
			}

			if (c.digits[i + j] > '9') {
				c.Subtract('9' + 1, (i + j) + 1);
				c.Add('1', (i + j) + 2);
			}
		}
	}

	if (a.thesign == b.thesign) c.thesign = true;
	else c.thesign = false;

	return c;
}


std::ostream& operator<<(std::ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {
		out << '-';
	}
	for (i = a.length - 1; i >= 0; i--) {
		out << a.digits[i];
	}
	return out;
}

void inf_int::Add(const char num, const unsigned int index)	// a�� index �ڸ����� n�� ���Ѵ�. 0<=n<=9, ex) a�� 391�϶�, a.Add(2, 2)�� ����� 411
{
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
			std::cout << "Memory reallocation failed, the program will terminate." << std::endl;

			exit(0);
		}

		this->length = index;					// ���� ����
		this->digits[this->length] = '\0';	// �ι��� ����
	}

	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// �� ����


	if (this->digits[index - 1] > '9') {	// �ڸ��ø��� �߻��� ���
		this->digits[index - 1] -= 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
		Add('1', index + 1);			// ���ڸ��� 1�� ���Ѵ�
	}
}

void inf_int::Subtract(const char num, const unsigned int index)	// a�� index �ڸ����� n�� ����. 0<=n<=9, ex) a�� 391�϶�, a.Add(2, 2)�� ����� 411
{
	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] += 10;
		this->digits[index] -= 1;
	}

	this->digits[index - 1] -= num - '0';	// �� ����


	if (this->digits[index - 1] < '0') {	// �ڸ������� �߻��� ���
		this->digits[index - 1] += 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ���ϰ�
		Subtract('1', index + 1);			// ���ڸ��� 1�� ����
	}
}

void inf_int::CheckZero() 
{
	for (int i = 0; i < this->length; i++) {
		if (this->digits[i] != '0') { //0�� �ϳ��� �ִٸ� non-zero�̱� ������ ����
			return;
		}
	}

	//��� 0�� ���, �� 0�� ���
	new (this) inf_int();
}