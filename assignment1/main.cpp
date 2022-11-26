#include<iostream>
#include<string>
#include<cstdlib>
#include<algorithm>

using namespace std;
/*
* this method is to strip the leading zeros of a string
*/
string strip_leading_zeros(string x) {
	while (x[0] == '0') {
		if (x.length() != 1) {
			x = x.substr(1, x.length() - 1);
		}
		else {
			return "0";
		}
	}
	return x;
}

/*
* a method that calculate the sum of two unsigned integer, return the unsigned integer result as string
*/
string school_addition(string I1, string I2, int base) {
	I1 = strip_leading_zeros(I1);
	I2 = strip_leading_zeros(I2);
	if (I1.length() > I2.length()) {
		swap(I1, I2);
	}
	//make sure I1 is the number with less digits

	reverse(I1.begin(), I1.end());
	reverse(I2.begin(), I2.end());
	//reverse two strings so we can loop from lower digits to higher digits

	int carries = 0;
	string sum = "", digit;
	for (decltype(I1.length()) i = 0; i < I1.length(); i++) {
		int temp_sum = (I1[i] - '0') + (I2[i] - '0') + carries;
		digit = to_string(temp_sum % base);
		sum += digit;
		carries = temp_sum / base;
	}

	for (decltype(I1.length()) i = I1.length(); i < I2.length(); i++) {
		int temp_sum = (I2[i] - '0') + carries;
		digit = to_string(temp_sum % base);
		sum += digit;
		carries = temp_sum / base;
	}
	//add one by one according to school method
	if (carries != 0) {
		sum += to_string(carries);
	}
	reverse(sum.begin(), sum.end());
	sum = strip_leading_zeros(sum);
	//reverse the string to get the right number and strip the leading zeros

	return sum;
}

/*
* a method that calculate the subtraction of two unsigned integer, return the unsigned integer result as string
* I1 must be bigger or equal to I2
*/
string school_subtraction(string I1, string I2, int base) {
	I1 = strip_leading_zeros(I1);
	I2 = strip_leading_zeros(I2);
	if (I1.length() < I2.length()) {
		swap(I1, I2);
	}
	//make sure I1 is the longer one

	reverse(I1.begin(), I1.end());
	reverse(I2.begin(), I2.end());
	//reverse two strings so we can loop from lower digits to higher digits

	int carries = 0;
	string sub = "", digit;
	for (decltype(I2.length()) i = 0; i < I2.length(); i++) {
		int temp_sub = (I1[i] - '0') - (I2[i] - '0') - carries;
		carries = temp_sub < 0 ? 1 : 0;
		temp_sub = temp_sub < 0 ? temp_sub + base : temp_sub;
		digit = to_string(temp_sub);
		sub += digit;
	}

	for (decltype(I1.length()) i = I2.length(); i < I1.length(); i++) {
		int temp_sub = (I1[i] - '0') - carries;
		carries = temp_sub < 0 ? 1 : 0;
		temp_sub = temp_sub < 0 ? temp_sub + base : temp_sub;
		digit = to_string(temp_sub);
		sub += digit;
	}
	//sub one by one according to school method

	reverse(sub.begin(), sub.end());
	sub = strip_leading_zeros(sub);
	//reverse the string to get the right number and strip the leading zeros
	return sub;
}

/*
* a method that calculate the multiplication of two unsigned integer, return the unsigned integer result as string
*/
string karatsuba_multi(string x, string y, int base) {
	x = strip_leading_zeros(x);
	y = strip_leading_zeros(y);

	if (x.length() == 1 && y.length() == 1) {
		int multi = (x[0] - '0') * (y[0] - '0');
		string res = to_string(multi / base) + to_string(multi % base);
		return strip_leading_zeros(res);
	}
	//base case

	if (x.length() != y.length()) {
		if (x.length() > y.length()) {
			swap(x, y);
		}
		//make sure x is the shorter one

		string y0 = y.substr(y.length() - x.length(), y.length() - 1);
		string y1 = y.substr(0, y.length() - x.length());
		string zeros(x.length(), '0');
		string first_part = karatsuba_multi(x, y1, base) + zeros;
		string second_part = karatsuba_multi(x, y0, base);
		return strip_leading_zeros(school_addition(first_part, second_part, base));
	}
	//if length of x less than the length of y, cut y into y1 and y0, the length of y0 is equal to x
	//result=y1*x*10^(x.length())+y0*x

	else {
		auto m = x.length() / 2;
		auto n = x.length() - m;
		string x1 = x.substr(0, m);
		string x0 = x.substr(m, x.length() - 1);
		string y1 = y.substr(0, m);
		string y0 = y.substr(m, y.length() - 1);
		//divide the two number into four

		string x1y1 = karatsuba_multi(x1, y1, base);
		string x0y0 = karatsuba_multi(x0, y0, base);
		string x1_plus_x0 = school_addition(x1, x0, base);
		string y1_plus_y0 = school_addition(y1, y0, base);
		string x1x0_multi_y1y0 = karatsuba_multi(x1_plus_x0, y1_plus_y0, base);
		string zeros_2m(2 * n, '0');
		string zeros_m(n, '0');
		string first_part = x1y1 + zeros_2m;
		string middle_number = school_subtraction(school_subtraction(x1x0_multi_y1y0, x0y0, base), x1y1, base);
		string middle_part = middle_number + zeros_m;
		//calculate each part and add them all together

		string result = school_addition(school_addition(first_part, middle_part, base), x0y0, base);


		return strip_leading_zeros(result);
	}
	//case when the lengths of two numbers are equal

}

/*
* return 1 if x>y , return -1 if x<y, return 0 if x==y
*/
int compare_value(string x, string y) {
	x = strip_leading_zeros(x);
	y = strip_leading_zeros(y);
	//strip two strings as no leading zeros

	if (x == y) {
		return 0;
	}
	//case when x is equal to y

	if (x.length() < y.length()) {
		return -1;
	}
	else if (x.length() > y.length()) {
		return 1;
	}
	//cases when the lengths of x and y are different

	for (decltype(x.length()) i = 0; i < x.length(); i++) {
		if (x[i] < y[i]) {
			return -1;
		}
		else if (x[i] > y[i]) {
			return 1;
		}
	}
	return 0;
}

/*
* a school method to solve the division
* returns x/y in base of "base"
*/
string division(string x, string y, int base) {
	string numerator;
	string carries = "0";
	int temp_quotient = 0;
	string digit;
	string quotient;
	for (decltype(x.length()) i = 0; i < x.length(); i++) {
		numerator = carries + x[i];
		while (compare_value(numerator, y) >= 0) {
			temp_quotient++;
			numerator = school_subtraction(numerator, y, base);
		}
		carries = numerator;
		digit = to_string(temp_quotient);
		temp_quotient = 0;
		quotient += digit;
	}
	quotient = strip_leading_zeros(quotient);
	return quotient;
}

int main() {
	string I1, I2;
	int B;

	cin >> I1 >> I2 >> B;
	//get two number as strings and one as base
	string sum = school_addition(I1, I2, B);
	string multi = karatsuba_multi(I1, I2, B);
	string ratio = division(I1, I2, B);
	cout << sum << " " << multi << " " << ratio << endl;
	return 0;
}
